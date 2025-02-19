/*
 * C Extension module to test Python interpreter C APIs.
 *
 * The 'test_*' functions exported by this module are run as part of the
 * standard Python regression test, via Lib/test/test_capi.py.
 */

#include "Python.h"
#include <float.h>
#include "structmember.h"
#include "datetime.h"
#include "marshal.h"
#include "../Include/enclave_wrapper/signal.h"
#ifdef MS_WINDOWS
#  include <crtdbg.h>
#endif

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>           /* For W_STOPCODE */
#endif

#ifdef WITH_THREAD
#include "pythread.h"
#endif /* WITH_THREAD */
static PyObject *TestError;     /* set to exception object in init */

/* Raise TestError with test_name + ": " + msg, and return NULL. */

static PyObject *
raiseTestError(const char* test_name, const char* msg)
{
    char buf[2048];

    if (strlen(test_name) + strlen(msg) > sizeof(buf) - 50)
        PyErr_SetString(TestError, "internal error msg too large");
    else {
        PyOS_snprintf(buf, sizeof(buf), "%s: %s", test_name, msg);
        PyErr_SetString(TestError, buf);
    }
    return NULL;
}

/* Test #defines from pyconfig.h (particularly the SIZEOF_* defines).

   The ones derived from autoconf on the UNIX-like OSes can be relied
   upon (in the absence of sloppy cross-compiling), but the Windows
   platforms have these hardcoded.  Better safe than sorry.
*/
static PyObject*
sizeof_error(const char* fatname, const char* typname,
    int expected, int got)
{
    char buf[1024];
    PyOS_snprintf(buf, sizeof(buf),
        "%.200s #define == %d but sizeof(%.200s) == %d",
        fatname, expected, typname, got);
    PyErr_SetString(TestError, buf);
    return (PyObject*)NULL;
}

static PyObject*
test_config(PyObject *self)
{
#define CHECK_SIZEOF(FATNAME, TYPE) \
            if (FATNAME != sizeof(TYPE)) \
                return sizeof_error(#FATNAME, #TYPE, FATNAME, sizeof(TYPE))

    CHECK_SIZEOF(SIZEOF_SHORT, short);
    CHECK_SIZEOF(SIZEOF_INT, int);
    CHECK_SIZEOF(SIZEOF_LONG, long);
    CHECK_SIZEOF(SIZEOF_VOID_P, void*);
    CHECK_SIZEOF(SIZEOF_TIME_T, time_t);
#ifdef HAVE_LONG_LONG
    CHECK_SIZEOF(SIZEOF_LONG_LONG, PY_LONG_LONG);
#endif

#undef CHECK_SIZEOF

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
test_list_api(PyObject *self)
{
    PyObject* list;
    int i;

    /* SF bug 132008:  PyList_Reverse segfaults */
#define NLIST 30
    list = PyList_New(NLIST);
    if (list == (PyObject*)NULL)
        return (PyObject*)NULL;
    /* list = range(NLIST) */
    for (i = 0; i < NLIST; ++i) {
        PyObject* anint = PyInt_FromLong(i);
        if (anint == (PyObject*)NULL) {
            Py_DECREF(list);
            return (PyObject*)NULL;
        }
        PyList_SET_ITEM(list, i, anint);
    }
    /* list.reverse(), via PyList_Reverse() */
    i = PyList_Reverse(list);   /* should not blow up! */
    if (i != 0) {
        Py_DECREF(list);
        return (PyObject*)NULL;
    }
    /* Check that list == range(29, -1, -1) now */
    for (i = 0; i < NLIST; ++i) {
        PyObject* anint = PyList_GET_ITEM(list, i);
        if (PyInt_AS_LONG(anint) != NLIST-1-i) {
            PyErr_SetString(TestError,
                            "test_list_api: reverse screwed up");
            Py_DECREF(list);
            return (PyObject*)NULL;
        }
    }
    Py_DECREF(list);
#undef NLIST

    Py_INCREF(Py_None);
    return Py_None;
}

static int
test_dict_inner(int count)
{
    Py_ssize_t pos = 0, iterations = 0;
    int i;
    PyObject *dict = PyDict_New();
    PyObject *v, *k;

    if (dict == NULL)
        return -1;

    for (i = 0; i < count; i++) {
        v = PyInt_FromLong(i);
        PyDict_SetItem(dict, v, v);
        Py_DECREF(v);
    }

    while (PyDict_Next(dict, &pos, &k, &v)) {
        PyObject *o;
        iterations++;

        i = PyInt_AS_LONG(v) + 1;
        o = PyInt_FromLong(i);
        if (o == NULL)
            return -1;
        if (PyDict_SetItem(dict, k, o) < 0) {
            Py_DECREF(o);
            return -1;
        }
        Py_DECREF(o);
    }

    Py_DECREF(dict);

    if (iterations != count) {
        PyErr_SetString(
            TestError,
            "test_dict_iteration: dict iteration went wrong ");
        return -1;
    } else {
        return 0;
    }
}

static PyObject*
test_dict_iteration(PyObject* self)
{
    int i;

    for (i = 0; i < 200; i++) {
        if (test_dict_inner(i) < 0) {
            return NULL;
        }
    }

    Py_INCREF(Py_None);
    return Py_None;
}


/* Issue #4701: Check that PyObject_Hash implicitly calls
 *   PyType_Ready if it hasn't already been called
 */
static PyTypeObject _HashInheritanceTester_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "hashinheritancetester",            /* Name of this type */
    sizeof(PyObject),           /* Basic object size */
    0,                          /* Item size for varobject */
    (destructor)PyObject_Del, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    PyObject_GenericGetAttr,  /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,         /* tp_flags */
    0,                          /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    0,                          /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    0,                          /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
};

static PyObject*
test_lazy_hash_inheritance(PyObject* self)
{
    PyTypeObject *type;
    PyObject *obj;
    long hash;

    type = &_HashInheritanceTester_Type;

    if (type->tp_dict != NULL)
        /* The type has already been initialized. This probably means
           -R is being used. */
        Py_RETURN_NONE;


    obj = PyObject_New(PyObject, type);
    if (obj == NULL) {
        PyErr_Clear();
        PyErr_SetString(
            TestError,
            "test_lazy_hash_inheritance: failed to create object");
        return NULL;
    }

    if (type->tp_dict != NULL) {
        PyErr_SetString(
            TestError,
            "test_lazy_hash_inheritance: type initialised too soon");
        Py_DECREF(obj);
        return NULL;
    }

    hash = PyObject_Hash(obj);
    if ((hash == -1) && PyErr_Occurred()) {
        PyErr_Clear();
        PyErr_SetString(
            TestError,
            "test_lazy_hash_inheritance: could not hash object");
        Py_DECREF(obj);
        return NULL;
    }

    if (type->tp_dict == NULL) {
        PyErr_SetString(
            TestError,
            "test_lazy_hash_inheritance: type not initialised by hash()");
        Py_DECREF(obj);
        return NULL;
    }

    if (type->tp_hash != PyType_Type.tp_hash) {
        PyErr_SetString(
            TestError,
            "test_lazy_hash_inheritance: unexpected hash function");
        Py_DECREF(obj);
        return NULL;
    }

    Py_DECREF(obj);

    Py_RETURN_NONE;
}


/* Issue #7385: Check that memoryview() does not crash
 *   when bf_getbuffer returns an error
 */

static int
broken_buffer_getbuffer(PyObject *self, Py_buffer *view, int flags)
{
    PyErr_SetString(
        TestError,
        "test_broken_memoryview: expected error in bf_getbuffer");
    return -1;
}

static PyBufferProcs memoryviewtester_as_buffer = {
    0,          /* bf_getreadbuffer */
    0,          /* bf_getwritebuffer */
    0,          /* bf_getsegcount */
    0,          /* bf_getcharbuffer */
    (getbufferproc)broken_buffer_getbuffer,     /* bf_getbuffer */
    0,          /* bf_releasebuffer */
};

static PyTypeObject _MemoryViewTester_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "memoryviewtester",         /* Name of this type */
    sizeof(PyObject),           /* Basic object size */
    0,                          /* Item size for varobject */
    (destructor)PyObject_Del, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    PyObject_GenericGetAttr,  /* tp_getattro */
    0,                          /* tp_setattro */
    &memoryviewtester_as_buffer,                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_NEWBUFFER,     /* tp_flags */
    0,                          /* tp_doc */
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    0,                          /* tp_methods */
    0,                          /* tp_members */
    0,                          /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    0,                          /* tp_dictoffset */
    0,                          /* tp_init */
    0,                          /* tp_alloc */
    PyType_GenericNew,                  /* tp_new */
};

static PyObject*
test_broken_memoryview(PyObject* self)
{
    PyObject *obj = PyObject_New(PyObject, &_MemoryViewTester_Type);
    PyObject *res;

    if (obj == NULL) {
        PyErr_Clear();
        PyErr_SetString(
            TestError,
            "test_broken_memoryview: failed to create object");
        return NULL;
    }

    res = PyMemoryView_FromObject(obj);
    if (res || !PyErr_Occurred()){
        PyErr_SetString(
            TestError,
            "test_broken_memoryview: memoryview() didn't raise an Exception");
        Py_XDECREF(res);
        Py_DECREF(obj);
        return NULL;
    }

    PyErr_Clear();
    Py_DECREF(obj);
    Py_RETURN_NONE;
}

static PyObject *
test_to_contiguous(PyObject* self, PyObject *noargs)
{
    int data[9] = {0, -1, 1, -1, 2, -1,  3, -1, 4};
    int result[5];
    Py_ssize_t itemsize = sizeof(int);
    Py_ssize_t shape = 5;
    Py_ssize_t strides = 2 * itemsize;
    Py_buffer view = {
        data,
        NULL,
        5 * itemsize,
        itemsize,
        1,
        1,
        NULL,
        &shape,
        &strides,
        NULL,
        {0, 0},
        NULL
    };
    int i;

    PyBuffer_ToContiguous(result, &view, view.len, 'C');
    for (i = 0; i < 5; i++) {
        if (result[i] != i) {
            PyErr_SetString(TestError,
                "test_to_contiguous: incorrect result");
            return NULL;
        }
    }

    view.buf = &data[8];
    view.strides[0] = -2 * itemsize;

    PyBuffer_ToContiguous(result, &view, view.len, 'C');
    for (i = 0; i < 5; i++) {
        if (result[i] != 4-i) {
            PyErr_SetString(TestError,
                "test_to_contiguous: incorrect result");
            return NULL;
        }
    }

    Py_RETURN_NONE;
}

static PyObject *
test_from_contiguous(PyObject* self, PyObject *noargs)
{
    int data[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int init[5] = {0, 1, 2, 3, 4};
    Py_ssize_t itemsize = sizeof(int);
    Py_ssize_t shape = 5;
    Py_ssize_t strides = 2 * itemsize;
    Py_buffer view = {
        data,
        NULL,
        5 * itemsize,
        itemsize,
        1,
        1,
        NULL,
        &shape,
        &strides,
        NULL,
        {0, 0},
        NULL
    };
    int *ptr;
    int i;

    PyBuffer_FromContiguous(&view, init, view.len, 'C');
    ptr = view.buf;
    for (i = 0; i < 5; i++) {
        if (ptr[2*i] != i) {
            PyErr_SetString(TestError,
                "test_from_contiguous: incorrect result");
            return NULL;
        }
    }

    view.buf = &data[8];
    view.strides[0] = -2 * itemsize;

    PyBuffer_FromContiguous(&view, init, view.len, 'C');
    ptr = view.buf;
    for (i = 0; i < 5; i++) {
        if (*(ptr-2*i) != i) {
            PyErr_SetString(TestError,
                "test_from_contiguous: incorrect result");
            return NULL;
        }
    }

    Py_RETURN_NONE;
}


/* Tests of PyLong_{As, From}{Unsigned,}Long(), and (#ifdef HAVE_LONG_LONG)
   PyLong_{As, From}{Unsigned,}LongLong().

   Note that the meat of the test is contained in testcapi_long.h.
   This is revolting, but delicate code duplication is worse:  "almost
   exactly the same" code is needed to test PY_LONG_LONG, but the ubiquitous
   dependence on type names makes it impossible to use a parameterized
   function.  A giant macro would be even worse than this.  A C++ template
   would be perfect.

   The "report an error" functions are deliberately not part of the #include
   file:  if the test fails, you can set a breakpoint in the appropriate
   error function directly, and crawl back from there in the debugger.
*/

#define UNBIND(X)  Py_DECREF(X); (X) = NULL

static PyObject *
raise_test_long_error(const char* msg)
{
    return raiseTestError("test_long_api", msg);
}

#define TESTNAME        test_long_api_inner
#define TYPENAME        long
#define F_S_TO_PY       PyLong_FromLong
#define F_PY_TO_S       PyLong_AsLong
#define F_U_TO_PY       PyLong_FromUnsignedLong
#define F_PY_TO_U       PyLong_AsUnsignedLong

#include "testcapi_long.h"

static PyObject *
test_long_api(PyObject* self)
{
    return TESTNAME(raise_test_long_error);
}

#undef TESTNAME
#undef TYPENAME
#undef F_S_TO_PY
#undef F_PY_TO_S
#undef F_U_TO_PY
#undef F_PY_TO_U

#ifdef HAVE_LONG_LONG

static PyObject *
raise_test_longlong_error(const char* msg)
{
    return raiseTestError("test_longlong_api", msg);
}

#define TESTNAME        test_longlong_api_inner
#define TYPENAME        PY_LONG_LONG
#define F_S_TO_PY       PyLong_FromLongLong
#define F_PY_TO_S       PyLong_AsLongLong
#define F_U_TO_PY       PyLong_FromUnsignedLongLong
#define F_PY_TO_U       PyLong_AsUnsignedLongLong

#include "testcapi_long.h"

static PyObject *
test_longlong_api(PyObject* self, PyObject *args)
{
    return TESTNAME(raise_test_longlong_error);
}

#undef TESTNAME
#undef TYPENAME
#undef F_S_TO_PY
#undef F_PY_TO_S
#undef F_U_TO_PY
#undef F_PY_TO_U

/* Test the PyLong_AsLongAndOverflow API. General conversion to PY_LONG
   is tested by test_long_api_inner. This test will concentrate on proper
   handling of overflow.
*/

static PyObject *
test_long_and_overflow(PyObject *self)
{
    PyObject *num, *one, *temp;
    long value;
    int overflow;

    /* Test that overflow is set properly for a large value. */
    /* num is a number larger than LONG_MAX even on 64-bit platforms */
    num = PyLong_FromString("FFFFFFFFFFFFFFFFFFFFFFFF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_and_overflow",
            "return value was not set to -1");
    if (overflow != 1)
        return raiseTestError("test_long_and_overflow",
            "overflow was not set to 1");

    /* Same again, with num = LONG_MAX + 1 */
    num = PyLong_FromLong(LONG_MAX);
    if (num == NULL)
        return NULL;
    one = PyLong_FromLong(1L);
    if (one == NULL) {
        Py_DECREF(num);
        return NULL;
    }
    temp = PyNumber_Add(num, one);
    Py_DECREF(one);
    Py_DECREF(num);
    num = temp;
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_and_overflow",
            "return value was not set to -1");
    if (overflow != 1)
        return raiseTestError("test_long_and_overflow",
            "overflow was not set to 1");

    /* Test that overflow is set properly for a large negative value. */
    /* num is a number smaller than LONG_MIN even on 64-bit platforms */
    num = PyLong_FromString("-FFFFFFFFFFFFFFFFFFFFFFFF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_and_overflow",
            "return value was not set to -1");
    if (overflow != -1)
        return raiseTestError("test_long_and_overflow",
            "overflow was not set to -1");

    /* Same again, with num = LONG_MIN - 1 */
    num = PyLong_FromLong(LONG_MIN);
    if (num == NULL)
        return NULL;
    one = PyLong_FromLong(1L);
    if (one == NULL) {
        Py_DECREF(num);
        return NULL;
    }
    temp = PyNumber_Subtract(num, one);
    Py_DECREF(one);
    Py_DECREF(num);
    num = temp;
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_and_overflow",
            "return value was not set to -1");
    if (overflow != -1)
        return raiseTestError("test_long_and_overflow",
            "overflow was not set to -1");

    /* Test that overflow is cleared properly for small values. */
    num = PyLong_FromString("FF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != 0xFF)
        return raiseTestError("test_long_and_overflow",
            "expected return value 0xFF");
    if (overflow != 0)
        return raiseTestError("test_long_and_overflow",
            "overflow was not cleared");

    num = PyLong_FromString("-FF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -0xFF)
        return raiseTestError("test_long_and_overflow",
            "expected return value 0xFF");
    if (overflow != 0)
        return raiseTestError("test_long_and_overflow",
            "overflow was set incorrectly");

    num = PyLong_FromLong(LONG_MAX);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != LONG_MAX)
        return raiseTestError("test_long_and_overflow",
            "expected return value LONG_MAX");
    if (overflow != 0)
        return raiseTestError("test_long_and_overflow",
            "overflow was not cleared");

    num = PyLong_FromLong(LONG_MIN);
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != LONG_MIN)
        return raiseTestError("test_long_and_overflow",
            "expected return value LONG_MIN");
    if (overflow != 0)
        return raiseTestError("test_long_and_overflow",
            "overflow was not cleared");

    Py_INCREF(Py_None);
    return Py_None;
}

/* Test the PyLong_AsLongLongAndOverflow API. General conversion to
   PY_LONG_LONG is tested by test_long_api_inner. This test will
   concentrate on proper handling of overflow.
*/

static PyObject *
test_long_long_and_overflow(PyObject *self)
{
    PyObject *num, *one, *temp;
    PY_LONG_LONG value;
    int overflow;

    /* Test that overflow is set properly for a large value. */
    /* num is a number larger than PY_LLONG_MAX on a typical machine. */
    num = PyLong_FromString("FFFFFFFFFFFFFFFFFFFFFFFF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_long_and_overflow",
            "return value was not set to -1");
    if (overflow != 1)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not set to 1");

    /* Same again, with num = PY_LLONG_MAX + 1 */
    num = PyLong_FromLongLong(PY_LLONG_MAX);
    if (num == NULL)
        return NULL;
    one = PyLong_FromLong(1L);
    if (one == NULL) {
        Py_DECREF(num);
        return NULL;
    }
    temp = PyNumber_Add(num, one);
    Py_DECREF(one);
    Py_DECREF(num);
    num = temp;
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_long_and_overflow",
            "return value was not set to -1");
    if (overflow != 1)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not set to 1");

    /* Test that overflow is set properly for a large negative value. */
    /* num is a number smaller than PY_LLONG_MIN on a typical platform */
    num = PyLong_FromString("-FFFFFFFFFFFFFFFFFFFFFFFF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_long_and_overflow",
            "return value was not set to -1");
    if (overflow != -1)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not set to -1");

    /* Same again, with num = PY_LLONG_MIN - 1 */
    num = PyLong_FromLongLong(PY_LLONG_MIN);
    if (num == NULL)
        return NULL;
    one = PyLong_FromLong(1L);
    if (one == NULL) {
        Py_DECREF(num);
        return NULL;
    }
    temp = PyNumber_Subtract(num, one);
    Py_DECREF(one);
    Py_DECREF(num);
    num = temp;
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -1)
        return raiseTestError("test_long_long_and_overflow",
            "return value was not set to -1");
    if (overflow != -1)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not set to -1");

    /* Test that overflow is cleared properly for small values. */
    num = PyLong_FromString("FF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != 0xFF)
        return raiseTestError("test_long_long_and_overflow",
            "expected return value 0xFF");
    if (overflow != 0)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not cleared");

    num = PyLong_FromString("-FF", NULL, 16);
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != -0xFF)
        return raiseTestError("test_long_long_and_overflow",
            "expected return value 0xFF");
    if (overflow != 0)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was set incorrectly");

    num = PyLong_FromLongLong(PY_LLONG_MAX);
    if (num == NULL)
        return NULL;
    overflow = 1234;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != PY_LLONG_MAX)
        return raiseTestError("test_long_long_and_overflow",
            "expected return value PY_LLONG_MAX");
    if (overflow != 0)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not cleared");

    num = PyLong_FromLongLong(PY_LLONG_MIN);
    if (num == NULL)
        return NULL;
    overflow = 0;
    value = PyLong_AsLongLongAndOverflow(num, &overflow);
    Py_DECREF(num);
    if (value == -1 && PyErr_Occurred())
        return NULL;
    if (value != PY_LLONG_MIN)
        return raiseTestError("test_long_long_and_overflow",
            "expected return value PY_LLONG_MIN");
    if (overflow != 0)
        return raiseTestError("test_long_long_and_overflow",
            "overflow was not cleared");

    Py_INCREF(Py_None);
    return Py_None;
}

/* Test the L code for PyArg_ParseTuple.  This should deliver a PY_LONG_LONG
   for both long and int arguments.  The test may leak a little memory if
   it fails.
*/
static PyObject *
test_L_code(PyObject *self)
{
    PyObject *tuple, *num;
    PY_LONG_LONG value;

    tuple = PyTuple_New(1);
    if (tuple == NULL)
        return NULL;

    num = PyLong_FromLong(42);
    if (num == NULL)
        return NULL;

    PyTuple_SET_ITEM(tuple, 0, num);

    value = -1;
    if (!PyArg_ParseTuple(tuple, "L:test_L_code", &value)) {
        return NULL;
    }
    if (value != 42)
        return raiseTestError("test_L_code",
            "L code returned wrong value for long 42");

    Py_DECREF(num);
    num = PyInt_FromLong(42);
    if (num == NULL)
        return NULL;

    PyTuple_SET_ITEM(tuple, 0, num);

    value = -1;
    if (!PyArg_ParseTuple(tuple, "L:test_L_code", &value)) {
        return NULL;
    }
    if (value != 42)
        return raiseTestError("test_L_code",
            "L code returned wrong value for int 42");

    Py_DECREF(tuple);
    Py_INCREF(Py_None);
    return Py_None;
}

#endif  /* ifdef HAVE_LONG_LONG */

static PyObject *
return_none(void *unused)
{
    Py_RETURN_NONE;
}

static PyObject *
raise_error(void *unused)
{
    PyErr_SetNone(PyExc_ValueError);
    return NULL;
}

static int
test_buildvalue_N_error(const char *fmt)
{
    PyObject *arg, *res;

    arg = PyList_New(0);
    if (arg == NULL) {
        return -1;
    }

    Py_INCREF(arg);
    res = Py_BuildValue(fmt, return_none, NULL, arg);
    if (res == NULL) {
        return -1;
    }
    Py_DECREF(res);
    if (Py_REFCNT(arg) != 1) {
        PyErr_Format(TestError, "test_buildvalue_N: "
                     "arg was not decrefed in successful "
                     "Py_BuildValue(\"%s\")", fmt);
        return -1;
    }

    Py_INCREF(arg);
    res = Py_BuildValue(fmt, raise_error, NULL, arg);
    if (res != NULL || !PyErr_Occurred()) {
        PyErr_Format(TestError, "test_buildvalue_N: "
                     "Py_BuildValue(\"%s\") didn't complain", fmt);
        return -1;
    }
    PyErr_Clear();
    if (Py_REFCNT(arg) != 1) {
        PyErr_Format(TestError, "test_buildvalue_N: "
                     "arg was not decrefed in failed "
                     "Py_BuildValue(\"%s\")", fmt);
        return -1;
    }
    Py_DECREF(arg);
    return 0;
}

static PyObject *
test_buildvalue_N(PyObject *self, PyObject *noargs)
{
    PyObject *arg, *res;

    arg = PyList_New(0);
    if (arg == NULL) {
        return NULL;
    }
    Py_INCREF(arg);
    res = Py_BuildValue("N", arg);
    if (res == NULL) {
        return NULL;
    }
    if (res != arg) {
        return raiseTestError("test_buildvalue_N",
                              "Py_BuildValue(\"N\") returned wrong result");
    }
    if (Py_REFCNT(arg) != 2) {
        return raiseTestError("test_buildvalue_N",
                              "arg was not decrefed in Py_BuildValue(\"N\")");
    }
    Py_DECREF(res);
    Py_DECREF(arg);

    if (test_buildvalue_N_error("O&N") < 0)
        return NULL;
    if (test_buildvalue_N_error("(O&N)") < 0)
        return NULL;
    if (test_buildvalue_N_error("[O&N]") < 0)
        return NULL;
    if (test_buildvalue_N_error("{O&N}") < 0)
        return NULL;
    if (test_buildvalue_N_error("{()O&(())N}") < 0)
        return NULL;

    Py_RETURN_NONE;
}


static PyObject *
get_args(PyObject *self, PyObject *args)
{
    if (args == NULL) {
        args = Py_None;
    }
    Py_INCREF(args);
    return args;
}

static PyObject *
get_kwargs(PyObject *self, PyObject *args, PyObject *kwargs)
{
    if (kwargs == NULL) {
        kwargs = Py_None;
    }
    Py_INCREF(kwargs);
    return kwargs;
}

/* Test tuple argument processing */
static PyObject *
getargs_tuple(PyObject *self, PyObject *args)
{
    int a, b, c;
    if (!PyArg_ParseTuple(args, "i(ii)", &a, &b, &c))
        return NULL;
    return Py_BuildValue("iii", a, b, c);
}

/* test PyArg_ParseTupleAndKeywords */
static PyObject *getargs_keywords(PyObject *self, PyObject *args, PyObject *kwargs)
{
    static char *keywords[] = {"arg1","arg2","arg3","arg4","arg5", NULL};
    static char *fmt="(ii)i|(i(ii))(iii)i";
    int int_args[10]={-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, fmt, keywords,
        &int_args[0], &int_args[1], &int_args[2], &int_args[3], &int_args[4],
        &int_args[5], &int_args[6], &int_args[7], &int_args[8], &int_args[9]))
        return NULL;
    return Py_BuildValue("iiiiiiiiii",
        int_args[0], int_args[1], int_args[2], int_args[3], int_args[4],
        int_args[5], int_args[6], int_args[7], int_args[8], int_args[9]);
}

/* Functions to call PyArg_ParseTuple with integer format codes,
   and return the result.
*/
static PyObject *
getargs_b(PyObject *self, PyObject *args)
{
    unsigned char value;
    if (!PyArg_ParseTuple(args, "b", &value))
        return NULL;
    return PyLong_FromUnsignedLong((unsigned long)value);
}

static PyObject *
getargs_B(PyObject *self, PyObject *args)
{
    unsigned char value;
    if (!PyArg_ParseTuple(args, "B", &value))
        return NULL;
    return PyLong_FromUnsignedLong((unsigned long)value);
}

static PyObject *
getargs_h(PyObject *self, PyObject *args)
{
    short value;
    if (!PyArg_ParseTuple(args, "h", &value))
        return NULL;
    return PyLong_FromLong((long)value);
}

static PyObject *
getargs_H(PyObject *self, PyObject *args)
{
    unsigned short value;
    if (!PyArg_ParseTuple(args, "H", &value))
        return NULL;
    return PyLong_FromUnsignedLong((unsigned long)value);
}

static PyObject *
getargs_I(PyObject *self, PyObject *args)
{
    unsigned int value;
    if (!PyArg_ParseTuple(args, "I", &value))
        return NULL;
    return PyLong_FromUnsignedLong((unsigned long)value);
}

static PyObject *
getargs_k(PyObject *self, PyObject *args)
{
    unsigned long value;
    if (!PyArg_ParseTuple(args, "k", &value))
        return NULL;
    return PyLong_FromUnsignedLong(value);
}

static PyObject *
getargs_i(PyObject *self, PyObject *args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    return PyLong_FromLong((long)value);
}

static PyObject *
getargs_l(PyObject *self, PyObject *args)
{
    long value;
    if (!PyArg_ParseTuple(args, "l", &value))
        return NULL;
    return PyLong_FromLong(value);
}

static PyObject *
getargs_n(PyObject *self, PyObject *args)
{
    Py_ssize_t value;
    if (!PyArg_ParseTuple(args, "n", &value))
    return NULL;
    return PyInt_FromSsize_t(value);
}

#ifdef HAVE_LONG_LONG
static PyObject *
getargs_L(PyObject *self, PyObject *args)
{
    PY_LONG_LONG value;
    if (!PyArg_ParseTuple(args, "L", &value))
        return NULL;
    return PyLong_FromLongLong(value);
}

static PyObject *
getargs_K(PyObject *self, PyObject *args)
{
    unsigned PY_LONG_LONG value;
    if (!PyArg_ParseTuple(args, "K", &value))
        return NULL;
    return PyLong_FromUnsignedLongLong(value);
}
#endif

/* This function not only tests the 'k' getargs code, but also the
   PyInt_AsUnsignedLongMask() and PyInt_AsUnsignedLongMask() functions. */
static PyObject *
test_k_code(PyObject *self)
{
    PyObject *tuple, *num;
    unsigned long value;

    tuple = PyTuple_New(1);
    if (tuple == NULL)
        return NULL;

    /* a number larger than ULONG_MAX even on 64-bit platforms */
    num = PyLong_FromString("FFFFFFFFFFFFFFFFFFFFFFFF", NULL, 16);
    if (num == NULL)
        return NULL;

    value = PyInt_AsUnsignedLongMask(num);
    if (value != ULONG_MAX)
        return raiseTestError("test_k_code",
        "PyInt_AsUnsignedLongMask() returned wrong value for long 0xFFF...FFF");

    PyTuple_SET_ITEM(tuple, 0, num);

    value = 0;
    if (!PyArg_ParseTuple(tuple, "k:test_k_code", &value)) {
        return NULL;
    }
    if (value != ULONG_MAX)
        return raiseTestError("test_k_code",
            "k code returned wrong value for long 0xFFF...FFF");

    Py_DECREF(num);
    num = PyLong_FromString("-FFFFFFFF000000000000000042", NULL, 16);
    if (num == NULL)
        return NULL;

    value = PyInt_AsUnsignedLongMask(num);
    if (value != (unsigned long)-0x42)
        return raiseTestError("test_k_code",
        "PyInt_AsUnsignedLongMask() returned wrong value for long 0xFFF...FFF");

    PyTuple_SET_ITEM(tuple, 0, num);

    value = 0;
    if (!PyArg_ParseTuple(tuple, "k:test_k_code", &value)) {
        return NULL;
    }
    if (value != (unsigned long)-0x42)
        return raiseTestError("test_k_code",
            "k code returned wrong value for long -0xFFF..000042");

    Py_DECREF(tuple);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
getargs_f(PyObject *self, PyObject *args)
{
    float f;
    if (!PyArg_ParseTuple(args, "f", &f))
        return NULL;
    return PyFloat_FromDouble(f);
}

static PyObject *
getargs_d(PyObject *self, PyObject *args)
{
    double d;
    if (!PyArg_ParseTuple(args, "d", &d))
        return NULL;
    return PyFloat_FromDouble(d);
}

static PyObject *
getargs_D(PyObject *self, PyObject *args)
{
    Py_complex cval;
    if (!PyArg_ParseTuple(args, "D", &cval))
        return NULL;
    return PyComplex_FromCComplex(cval);
}

static PyObject *
getargs_S(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "S", &obj))
        return NULL;
    Py_INCREF(obj);
    return obj;
}

static PyObject *
getargs_Y(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "Y", &obj))
        return NULL;
    Py_INCREF(obj);
    return obj;
}

#ifdef Py_USING_UNICODE
static PyObject *
getargs_U(PyObject *self, PyObject *args)
{
    PyObject *obj;
    if (!PyArg_ParseTuple(args, "U", &obj))
        return NULL;
    Py_INCREF(obj);
    return obj;
}
#endif

static PyObject *
getargs_c(PyObject *self, PyObject *args)
{
    char c;
    if (!PyArg_ParseTuple(args, "c", &c))
        return NULL;
    return PyInt_FromLong((unsigned char)c);
}

static PyObject *
getargs_s(PyObject *self, PyObject *args)
{
    const char *str;
    if (!PyArg_ParseTuple(args, "s", &str))
        return NULL;
    return PyBytes_FromString(str);
}

static PyObject *
getargs_s_star(PyObject *self, PyObject *args)
{
    Py_buffer buffer;
    PyObject *bytes;
    if (!PyArg_ParseTuple(args, "s*", &buffer))
        return NULL;
    bytes = PyBytes_FromStringAndSize(buffer.buf, buffer.len);
    PyBuffer_Release(&buffer);
    return bytes;
}

static PyObject *
getargs_s_hash(PyObject *self, PyObject *args)
{
    const char *str;
    int size;
    if (!PyArg_ParseTuple(args, "s#", &str, &size))
        return NULL;
    return PyBytes_FromStringAndSize(str, size);
}

static PyObject *
getargs_t_hash(PyObject *self, PyObject *args)
{
    const char *str;
    int size;
    if (!PyArg_ParseTuple(args, "t#", &str, &size))
        return NULL;
    return PyBytes_FromStringAndSize(str, size);
}

static PyObject *
getargs_z(PyObject *self, PyObject *args)
{
    const char *str;
    if (!PyArg_ParseTuple(args, "z", &str))
        return NULL;
    if (str != NULL)
        return PyBytes_FromString(str);
    else
        Py_RETURN_NONE;
}

static PyObject *
getargs_z_star(PyObject *self, PyObject *args)
{
    Py_buffer buffer;
    PyObject *bytes;
    if (!PyArg_ParseTuple(args, "z*", &buffer))
        return NULL;
    if (buffer.buf != NULL)
        bytes = PyBytes_FromStringAndSize(buffer.buf, buffer.len);
    else {
        Py_INCREF(Py_None);
        bytes = Py_None;
    }
    PyBuffer_Release(&buffer);
    return bytes;
}

static PyObject *
getargs_z_hash(PyObject *self, PyObject *args)
{
    const char *str;
    int size;
    if (!PyArg_ParseTuple(args, "z#", &str, &size))
        return NULL;
    if (str != NULL)
        return PyBytes_FromStringAndSize(str, size);
    else
        Py_RETURN_NONE;
}

static PyObject *
getargs_w(PyObject *self, PyObject *args)
{
    char *str;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "wn", &str, &size))
        return NULL;

    if (2 <= size) {
        str[0] = '[';
        str[size-1] = ']';
    }

    return PyBytes_FromStringAndSize(str, size);
}

static PyObject *
getargs_w_hash(PyObject *self, PyObject *args)
{
    char *str;
    int size;

    if (!PyArg_ParseTuple(args, "w#", &str, &size))
        return NULL;

    if (2 <= size) {
        str[0] = '[';
        str[size-1] = ']';
    }

    return PyBytes_FromStringAndSize(str, size);
}

static PyObject *
getargs_w_star(PyObject *self, PyObject *args)
{
    Py_buffer buffer;
    PyObject *result;
    char *str;

    if (!PyArg_ParseTuple(args, "w*", &buffer))
        return NULL;

    if (2 <= buffer.len) {
        str = buffer.buf;
        str[0] = '[';
        str[buffer.len-1] = ']';
    }

    result = PyBytes_FromStringAndSize(buffer.buf, buffer.len);
    PyBuffer_Release(&buffer);
    return result;
}

#ifdef Py_USING_UNICODE

static int
_ustrlen(const Py_UNICODE *u)
{
    int i = 0;
    const Py_UNICODE *v = u;
    while (*v != 0) { i++; v++; }
    return i;
}

static PyObject *
getargs_u(PyObject *self, PyObject *args)
{
    const Py_UNICODE *str;
    int size;
    if (!PyArg_ParseTuple(args, "u", &str))
        return NULL;
    size = _ustrlen(str);
    return PyUnicode_FromUnicode(str, size);
}

static PyObject *
getargs_u_hash(PyObject *self, PyObject *args)
{
    const Py_UNICODE *str;
    int size;
    if (!PyArg_ParseTuple(args, "u#", &str, &size))
        return NULL;
    return PyUnicode_FromUnicode(str, size);
}

#endif

static PyObject *
getargs_es(PyObject *self, PyObject *args)
{
    PyObject *arg, *result;
    const char *encoding = NULL;
    char *str;

    if (!PyArg_ParseTuple(args, "O|s", &arg, &encoding))
        return NULL;
    if (!PyArg_Parse(arg, "es", encoding, &str))
        return NULL;
    result = PyBytes_FromString(str);
    PyMem_Free(str);
    return result;
}

static PyObject *
getargs_et(PyObject *self, PyObject *args)
{
    PyObject *arg, *result;
    const char *encoding = NULL;
    char *str;

    if (!PyArg_ParseTuple(args, "O|s", &arg, &encoding))
        return NULL;
    if (!PyArg_Parse(arg, "et", encoding, &str))
        return NULL;
    result = PyBytes_FromString(str);
    PyMem_Free(str);
    return result;
}

static PyObject *
getargs_es_hash(PyObject *self, PyObject *args)
{
    PyObject *arg, *result;
    const char *encoding = NULL;
    PyByteArrayObject *buffer = NULL;
    char *str = NULL;
    int size;

    if (!PyArg_ParseTuple(args, "O|sO!",
                          &arg, &encoding, &PyByteArray_Type, &buffer))
        return NULL;
    if (buffer != NULL) {
        str = PyByteArray_AS_STRING(buffer);
        size = PyByteArray_GET_SIZE(buffer);
    }
    if (!PyArg_Parse(arg, "es#", encoding, &str, &size))
        return NULL;
    result = PyBytes_FromStringAndSize(str, size);
    if (buffer == NULL)
        PyMem_Free(str);
    return result;
}

static PyObject *
getargs_et_hash(PyObject *self, PyObject *args)
{
    PyObject *arg, *result;
    const char *encoding = NULL;
    PyByteArrayObject *buffer = NULL;
    char *str = NULL;
    int size;

    if (!PyArg_ParseTuple(args, "O|sO!",
                          &arg, &encoding, &PyByteArray_Type, &buffer))
        return NULL;
    if (buffer != NULL) {
        str = PyByteArray_AS_STRING(buffer);
        size = PyByteArray_GET_SIZE(buffer);
    }
    if (!PyArg_Parse(arg, "et#", encoding, &str, &size))
        return NULL;
    result = PyBytes_FromStringAndSize(str, size);
    if (buffer == NULL)
        PyMem_Free(str);
    return result;
}

#ifdef Py_USING_UNICODE

static volatile int x;

/* Test the u and u# codes for PyArg_ParseTuple. May leak memory in case
   of an error.
*/
static PyObject *
test_u_code(PyObject *self)
{
    PyObject *tuple, *obj;
    Py_UNICODE *value;
    int len;

    /* issue4122: Undefined reference to _Py_ascii_whitespace on Windows */
    /* Just use the macro and check that it compiles */
    x = Py_UNICODE_ISSPACE(25);

    tuple = PyTuple_New(1);
    if (tuple == NULL)
        return NULL;

    obj = PyUnicode_Decode("test", strlen("test"),
                           "ascii", NULL);
    if (obj == NULL)
        return NULL;

    PyTuple_SET_ITEM(tuple, 0, obj);

    value = 0;
    if (!PyArg_ParseTuple(tuple, "u:test_u_code", &value)) {
        return NULL;
    }
    if (value != PyUnicode_AS_UNICODE(obj))
        return raiseTestError("test_u_code",
            "u code returned wrong value for u'test'");
    value = 0;
    if (!PyArg_ParseTuple(tuple, "u#:test_u_code", &value, &len)) {
        return NULL;
    }
    if (value != PyUnicode_AS_UNICODE(obj) ||
        len != PyUnicode_GET_SIZE(obj))
        return raiseTestError("test_u_code",
            "u# code returned wrong values for u'test'");

    Py_DECREF(tuple);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
test_widechar(PyObject *self)
{
#if defined(SIZEOF_WCHAR_T) && (SIZEOF_WCHAR_T == 4)
    const wchar_t wtext[2] = {(wchar_t)0x10ABCDu};
    size_t wtextlen = 1;
#else
    const wchar_t wtext[3] = {(wchar_t)0xDBEAu, (wchar_t)0xDFCDu};
    size_t wtextlen = 2;
#endif
    PyObject *wide, *utf8;

    wide = PyUnicode_FromWideChar(wtext, wtextlen);
    if (wide == NULL)
        return NULL;

    utf8 = PyUnicode_FromString("\xf4\x8a\xaf\x8d");
    if (utf8 == NULL) {
        Py_DECREF(wide);
        return NULL;
    }

    if (PyUnicode_GET_SIZE(wide) != PyUnicode_GET_SIZE(utf8)) {
        Py_DECREF(wide);
        Py_DECREF(utf8);
        return raiseTestError("test_widechar",
                        "wide string and utf8 string have different length");
    }
    if (PyUnicode_Compare(wide, utf8)) {
        Py_DECREF(wide);
        Py_DECREF(utf8);
        if (PyErr_Occurred())
            return NULL;
        return raiseTestError("test_widechar",
                        "wide string and utf8 string are differents");
    }

    Py_DECREF(wide);
    Py_DECREF(utf8);
    Py_RETURN_NONE;
}

static PyObject *
unicode_encodedecimal(PyObject *self, PyObject *args)
{
    Py_UNICODE *unicode;
    int length;
    char *errors = NULL;
    PyObject *decimal;
    Py_ssize_t decimal_length, new_length;
    int res;

    if (!PyArg_ParseTuple(args, "u#|s", &unicode, &length, &errors))
        return NULL;

    decimal_length = length * 10; /* len('&#1114111;') */
    decimal = PyBytes_FromStringAndSize(NULL, decimal_length);
    if (decimal == NULL)
        return NULL;

    res = PyUnicode_EncodeDecimal(unicode, length,
                                  PyBytes_AS_STRING(decimal),
                                  errors);
    if (res < 0) {
        Py_DECREF(decimal);
        return NULL;
    }

    new_length = strlen(PyBytes_AS_STRING(decimal));
    assert(new_length <= decimal_length);
    res = _PyBytes_Resize(&decimal, new_length);
    if (res < 0)
        return NULL;

    return decimal;
}

static PyObject *
test_empty_argparse(PyObject *self)
{
    /* Test that formats can begin with '|'. See issue #4720. */
    PyObject *tuple, *dict = NULL;
    static char *kwlist[] = {NULL};
    int result;
    tuple = PyTuple_New(0);
    if (!tuple)
        return NULL;
    if (!(result = PyArg_ParseTuple(tuple, "|:test_empty_argparse"))) {
        goto done;
    }
    dict = PyDict_New();
    if (!dict)
        goto done;
    result = PyArg_ParseTupleAndKeywords(tuple, dict, "|:test_empty_argparse", kwlist);
  done:
    Py_DECREF(tuple);
    Py_XDECREF(dict);
    if (!result) {
        return NULL;
    }
    else {
        Py_RETURN_NONE;
    }
}

static PyObject *
codec_incrementalencoder(PyObject *self, PyObject *args)
{
    const char *encoding, *errors = NULL;
    if (!PyArg_ParseTuple(args, "s|s:test_incrementalencoder",
                          &encoding, &errors))
        return NULL;
    return PyCodec_IncrementalEncoder(encoding, errors);
}

static PyObject *
codec_incrementaldecoder(PyObject *self, PyObject *args)
{
    const char *encoding, *errors = NULL;
    if (!PyArg_ParseTuple(args, "s|s:test_incrementaldecoder",
                          &encoding, &errors))
        return NULL;
    return PyCodec_IncrementalDecoder(encoding, errors);
}

#endif

/* Simple test of _PyLong_NumBits and _PyLong_Sign. */
static PyObject *
test_long_numbits(PyObject *self)
{
    struct triple {
        long input;
        size_t nbits;
        int sign;
    } testcases[] = {{0, 0, 0},
                     {1L, 1, 1},
                     {-1L, 1, -1},
                     {2L, 2, 1},
                     {-2L, 2, -1},
                     {3L, 2, 1},
                     {-3L, 2, -1},
                     {4L, 3, 1},
                     {-4L, 3, -1},
                     {0x7fffL, 15, 1},          /* one Python long digit */
             {-0x7fffL, 15, -1},
             {0xffffL, 16, 1},
             {-0xffffL, 16, -1},
             {0xfffffffL, 28, 1},
             {-0xfffffffL, 28, -1}};
    int i;

    for (i = 0; i < sizeof(testcases) / sizeof(struct triple); ++i) {
        PyObject *plong = PyLong_FromLong(testcases[i].input);
        size_t nbits = _PyLong_NumBits(plong);
        int sign = _PyLong_Sign(plong);

        Py_DECREF(plong);
        if (nbits != testcases[i].nbits)
            return raiseTestError("test_long_numbits",
                            "wrong result for _PyLong_NumBits");
        if (sign != testcases[i].sign)
            return raiseTestError("test_long_numbits",
                            "wrong result for _PyLong_Sign");
    }
    Py_INCREF(Py_None);
    return Py_None;
}

/* Example passing NULLs to PyObject_Str(NULL) and PyObject_Unicode(NULL). */

static PyObject *
test_null_strings(PyObject *self)
{
    PyObject *o1 = PyObject_Str(NULL);
#ifdef Py_USING_UNICODE
    PyObject *o2 = PyObject_Unicode(NULL);
#else
    PyObject *o2 = PyObject_Str(NULL);
#endif
    PyObject *tuple = PyTuple_Pack(2, o1, o2);
    Py_XDECREF(o1);
    Py_XDECREF(o2);
    return tuple;
}

static PyObject *
raise_exception(PyObject *self, PyObject *args)
{
    PyObject *exc;
    PyObject *exc_args, *v;
    int num_args, i;

    if (!PyArg_ParseTuple(args, "Oi:raise_exception",
                          &exc, &num_args))
        return NULL;
    if (!PyExceptionClass_Check(exc)) {
        PyErr_Format(PyExc_TypeError, "an exception class is required");
        return NULL;
    }

    exc_args = PyTuple_New(num_args);
    if (exc_args == NULL)
        return NULL;
    for (i = 0; i < num_args; ++i) {
        v = PyInt_FromLong(i);
        if (v == NULL) {
            Py_DECREF(exc_args);
            return NULL;
        }
        PyTuple_SET_ITEM(exc_args, i, v);
    }
    PyErr_SetObject(exc, exc_args);
    Py_DECREF(exc_args);
    return NULL;
}

static PyObject *
set_errno(PyObject *self, PyObject *args)
{
    int new_errno;

    if (!PyArg_ParseTuple(args, "i:set_errno", &new_errno))
        return NULL;

    errno = new_errno;
    Py_RETURN_NONE;
}

#if defined(Py_USING_UNICODE) && !defined(Py_BUILD_CORE)
static int test_run_counter = 0;

static PyObject *
test_datetime_capi(PyObject *self, PyObject *args) {
    if (PyDateTimeAPI) {
        if (test_run_counter) {
            /* Probably regrtest.py -R */
            Py_RETURN_NONE;
        }
        else {
            PyErr_SetString(PyExc_AssertionError,
                            "PyDateTime_CAPI somehow initialized");
            return NULL;
        }
    }
    test_run_counter++;
    PyDateTime_IMPORT;
    if (PyDateTimeAPI)
        Py_RETURN_NONE;
    else
        return NULL;
}
#endif


#ifdef WITH_THREAD

/* test_thread_state spawns a thread of its own, and that thread releases
 * `thread_done` when it's finished.  The driver code has to know when the
 * thread finishes, because the thread uses a PyObject (the callable) that
 * may go away when the driver finishes.  The former lack of this explicit
 * synchronization caused rare segfaults, so rare that they were seen only
 * on a Mac buildbot (although they were possible on any box).
 */
static PyThread_type_lock thread_done = NULL;

static int
_make_call(void *callable)
{
    PyObject *rc;
    int success;
    PyGILState_STATE s = PyGILState_Ensure();
    rc = PyObject_CallFunction((PyObject *)callable, "");
    success = (rc != NULL);
    Py_XDECREF(rc);
    PyGILState_Release(s);
    return success;
}

/* Same thing, but releases `thread_done` when it returns.  This variant
 * should be called only from threads spawned by test_thread_state().
 */
static void
_make_call_from_thread(void *callable)
{
    _make_call(callable);
    PyThread_release_lock(thread_done);
}

static PyObject *
test_thread_state(PyObject *self, PyObject *args)
{
    PyObject *fn;
    int success = 1;

    if (!PyArg_ParseTuple(args, "O:test_thread_state", &fn))
        return NULL;

    if (!PyCallable_Check(fn)) {
        PyErr_Format(PyExc_TypeError, "'%s' object is not callable",
            fn->ob_type->tp_name);
        return NULL;
    }

    /* Ensure Python is set up for threading */
    PyEval_InitThreads();
    thread_done = PyThread_allocate_lock();
    if (thread_done == NULL)
        return PyErr_NoMemory();
    PyThread_acquire_lock(thread_done, 1);

    /* Start a new thread with our callback. */
    PyThread_start_new_thread(_make_call_from_thread, fn);
    /* Make the callback with the thread lock held by this thread */
    success &= _make_call(fn);
    /* Do it all again, but this time with the thread-lock released */
    Py_BEGIN_ALLOW_THREADS
    success &= _make_call(fn);
    PyThread_acquire_lock(thread_done, 1);  /* wait for thread to finish */
    Py_END_ALLOW_THREADS

    /* And once more with and without a thread
       XXX - should use a lock and work out exactly what we are trying
       to test <wink>
    */
    Py_BEGIN_ALLOW_THREADS
    PyThread_start_new_thread(_make_call_from_thread, fn);
    success &= _make_call(fn);
    PyThread_acquire_lock(thread_done, 1);  /* wait for thread to finish */
    Py_END_ALLOW_THREADS

    /* Release lock we acquired above.  This is required on HP-UX. */
    PyThread_release_lock(thread_done);

    PyThread_free_lock(thread_done);
    if (!success)
        return NULL;
    Py_RETURN_NONE;
}

/* test Py_AddPendingCalls using threads */
static int _pending_callback(void *arg)
{
    /* we assume the argument is callable object to which we own a reference */
    PyObject *callable = (PyObject *)arg;
    PyObject *r = PyObject_CallObject(callable, NULL);
    Py_DECREF(callable);
    Py_XDECREF(r);
    return r != NULL ? 0 : -1;
}

/* The following requests n callbacks to _pending_callback.  It can be
 * run from any python thread.
 */
PyObject *pending_threadfunc(PyObject *self, PyObject *arg)
{
    PyObject *callable;
    int r;
    if (PyArg_ParseTuple(arg, "O", &callable) == 0)
        return NULL;

    /* create the reference for the callbackwhile we hold the lock */
    Py_INCREF(callable);

    Py_BEGIN_ALLOW_THREADS
    r = Py_AddPendingCall(&_pending_callback, callable);
    Py_END_ALLOW_THREADS

    if (r<0) {
        Py_DECREF(callable); /* unsuccessful add, destroy the extra reference */
        Py_INCREF(Py_False);
        return Py_False;
    }
    Py_INCREF(Py_True);
    return Py_True;
}
#endif

/* Some tests of PyString_FromFormat().  This needs more tests. */
static PyObject *
test_string_from_format(PyObject *self, PyObject *args)
{
    PyObject *result;
    char *msg;

#define CHECK_1_FORMAT(FORMAT, TYPE)                    \
    result = PyString_FromFormat(FORMAT, (TYPE)1);      \
    if (result == NULL)                                 \
        return NULL;                                    \
    if (strcmp(PyString_AsString(result), "1")) {       \
        msg = FORMAT " failed at 1";                    \
        goto Fail;                                      \
    }                                                   \
    Py_DECREF(result)

    CHECK_1_FORMAT("%d", int);
    CHECK_1_FORMAT("%ld", long);
    /* The z width modifier was added in Python 2.5. */
    CHECK_1_FORMAT("%zd", Py_ssize_t);

    /* The u type code was added in Python 2.5. */
    CHECK_1_FORMAT("%u", unsigned int);
    CHECK_1_FORMAT("%lu", unsigned long);
    CHECK_1_FORMAT("%zu", size_t);

    /* "%lld" and "%llu" support added in Python 2.7. */
#ifdef HAVE_LONG_LONG
    CHECK_1_FORMAT("%llu", unsigned PY_LONG_LONG);
    CHECK_1_FORMAT("%lld", PY_LONG_LONG);
#endif

    Py_RETURN_NONE;

 Fail:
    Py_XDECREF(result);
    return raiseTestError("test_string_from_format", msg);

#undef CHECK_1_FORMAT
}

/* Coverage testing of capsule objects. */

static const char *capsule_name = "capsule name";
static       char *capsule_pointer = "capsule pointer";
static       char *capsule_context = "capsule context";
static const char *capsule_error = NULL;
static int
capsule_destructor_call_count = 0;

static void
capsule_destructor(PyObject *o) {
    capsule_destructor_call_count++;
    if (PyCapsule_GetContext(o) != capsule_context) {
        capsule_error = "context did not match in destructor!";
    } else if (PyCapsule_GetDestructor(o) != capsule_destructor) {
        capsule_error = "destructor did not match in destructor!  (woah!)";
    } else if (PyCapsule_GetName(o) != capsule_name) {
        capsule_error = "name did not match in destructor!";
    } else if (PyCapsule_GetPointer(o, capsule_name) != capsule_pointer) {
        capsule_error = "pointer did not match in destructor!";
    }
}

typedef struct {
    char *name;
    char *module;
    char *attribute;
} known_capsule;

static PyObject *
test_capsule(PyObject *self, PyObject *args)
{
    PyObject *object;
    const char *error = NULL;
    void *pointer;
    void *pointer2;
    known_capsule known_capsules[] = {
        #define KNOWN_CAPSULE(module, name)             { module "." name, module, name }
        KNOWN_CAPSULE("_socket", "CAPI"),
        KNOWN_CAPSULE("_curses", "_C_API"),
        KNOWN_CAPSULE("datetime", "datetime_CAPI"),
        { NULL, NULL },
    };
    known_capsule *known = &known_capsules[0];

#define FAIL(x) { error = (x); goto exit; }

#define CHECK_DESTRUCTOR \
    if (capsule_error) { \
        FAIL(capsule_error); \
    } \
    else if (!capsule_destructor_call_count) {          \
        FAIL("destructor not called!"); \
    } \
    capsule_destructor_call_count = 0; \

    object = PyCapsule_New(capsule_pointer, capsule_name, capsule_destructor);
    PyCapsule_SetContext(object, capsule_context);
    capsule_destructor(object);
    CHECK_DESTRUCTOR;
    Py_DECREF(object);
    CHECK_DESTRUCTOR;

    object = PyCapsule_New(known, "ignored", NULL);
    PyCapsule_SetPointer(object, capsule_pointer);
    PyCapsule_SetName(object, capsule_name);
    PyCapsule_SetDestructor(object, capsule_destructor);
    PyCapsule_SetContext(object, capsule_context);
    capsule_destructor(object);
    CHECK_DESTRUCTOR;
    /* intentionally access using the wrong name */
    pointer2 = PyCapsule_GetPointer(object, "the wrong name");
    if (!PyErr_Occurred()) {
        FAIL("PyCapsule_GetPointer should have failed but did not!");
    }
    PyErr_Clear();
    if (pointer2) {
        if (pointer2 == capsule_pointer) {
            FAIL("PyCapsule_GetPointer should not have"
                     " returned the internal pointer!");
        } else {
            FAIL("PyCapsule_GetPointer should have "
                     "returned NULL pointer but did not!");
        }
    }
    PyCapsule_SetDestructor(object, NULL);
    Py_DECREF(object);
    if (capsule_destructor_call_count) {
        FAIL("destructor called when it should not have been!");
    }

    for (known = &known_capsules[0]; known->module != NULL; known++) {
        /* yeah, ordinarily I wouldn't do this either,
           but it's fine for this test harness.
        */
        static char buffer[256];
#undef FAIL
#define FAIL(x) \
        { \
        sprintf(buffer, "%s module: \"%s\" attribute: \"%s\"", \
            x, known->module, known->attribute); \
        error = buffer; \
        goto exit; \
        } \

        PyObject *module = PyImport_ImportModule(known->module);
        if (module) {
            pointer = PyCapsule_Import(known->name, 0);
            if (!pointer) {
                Py_DECREF(module);
                FAIL("PyCapsule_GetPointer returned NULL unexpectedly!");
            }
            object = PyObject_GetAttrString(module, known->attribute);
            if (!object) {
                Py_DECREF(module);
                return NULL;
            }
            pointer2 = PyCapsule_GetPointer(object,
                                    "weebles wobble but they don't fall down");
            if (!PyErr_Occurred()) {
                Py_DECREF(object);
                Py_DECREF(module);
                FAIL("PyCapsule_GetPointer should have failed but did not!");
            }
            PyErr_Clear();
            if (pointer2) {
                Py_DECREF(module);
                Py_DECREF(object);
                if (pointer2 == pointer) {
                    FAIL("PyCapsule_GetPointer should not have"
                             " returned its internal pointer!");
                } else {
                    FAIL("PyCapsule_GetPointer should have"
                             " returned NULL pointer but did not!");
                }
            }
            Py_DECREF(object);
            Py_DECREF(module);
        }
        else
            PyErr_Clear();
    }

  exit:
    if (error) {
        return raiseTestError("test_capsule", error);
    }
    Py_RETURN_NONE;
#undef FAIL
}

/* This is here to provide a docstring for test_descr. */
static PyObject *
test_with_docstring(PyObject *self)
{
    Py_RETURN_NONE;
}

/* To test the format of tracebacks as printed out. */
static PyObject *
traceback_print(PyObject *self, PyObject *args)
{
    PyObject *file;
    PyObject *traceback;
    int result;

    if (!PyArg_ParseTuple(args, "OO:traceback_print",
                            &traceback, &file))
        return NULL;

    result = PyTraceBack_Print(traceback, file);
    if (result < 0)
        return NULL;
    Py_RETURN_NONE;
}

/* To test that the result of PyCode_NewEmpty has the right members. */
static PyObject *
code_newempty(PyObject *self, PyObject *args)
{
    const char *filename;
    const char *funcname;
    int firstlineno;

    if (!PyArg_ParseTuple(args, "ssi:code_newempty",
                          &filename, &funcname, &firstlineno))
        return NULL;

    return (PyObject *)PyCode_NewEmpty(filename, funcname, firstlineno);
}

/* Test PyErr_NewExceptionWithDoc (also exercise PyErr_NewException).
   Run via Lib/test/test_exceptions.py */
static PyObject *
make_exception_with_doc(PyObject *self, PyObject *args, PyObject *kwargs)
{
    char *name;
    char *doc = NULL;
    PyObject *base = NULL;
    PyObject *dict = NULL;

    static char *kwlist[] = {"name", "doc", "base", "dict", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs,
                    "s|sOO:make_exception_with_doc", kwlist,
                                     &name, &doc, &base, &dict))
        return NULL;

    return PyErr_NewExceptionWithDoc(name, doc, base, dict);
}

static PyObject *
sequence_delitem(PyObject *self, PyObject *args)
{
    PyObject *seq;
    Py_ssize_t i;

    if (!PyArg_ParseTuple(args, "On", &seq, &i))
        return NULL;
    if (PySequence_DelItem(seq, i) < 0)
        return NULL;
    Py_RETURN_NONE;
}

#ifdef WITH_THREAD
typedef struct {
    PyThread_type_lock start_event;
    PyThread_type_lock exit_event;
    PyObject *callback;
} test_c_thread_t;

static void
temporary_c_thread(void *data)
{
    test_c_thread_t *test_c_thread = data;
    PyGILState_STATE state;
    PyObject *res;

    PyThread_release_lock(test_c_thread->start_event);

    /* Allocate a Python thread state for this thread */
    state = PyGILState_Ensure();

    res = PyObject_CallFunction(test_c_thread->callback, "", NULL);
    Py_CLEAR(test_c_thread->callback);

    if (res == NULL) {
        PyErr_Print();
    }
    else {
        Py_DECREF(res);
    }

    /* Destroy the Python thread state for this thread */
    PyGILState_Release(state);

    PyThread_release_lock(test_c_thread->exit_event);

    PyThread_exit_thread();
}

static PyObject *
call_in_temporary_c_thread(PyObject *self, PyObject *callback)
{
    PyObject *res = NULL;
    test_c_thread_t test_c_thread;
    long thread;

    PyEval_InitThreads();

    test_c_thread.start_event = PyThread_allocate_lock();
    test_c_thread.exit_event = PyThread_allocate_lock();
    test_c_thread.callback = NULL;
    if (!test_c_thread.start_event || !test_c_thread.exit_event) {
        PyErr_SetString(PyExc_RuntimeError, "could not allocate lock");
        goto exit;
    }

    Py_INCREF(callback);
    test_c_thread.callback = callback;

    PyThread_acquire_lock(test_c_thread.start_event, 1);
    PyThread_acquire_lock(test_c_thread.exit_event, 1);

    thread = PyThread_start_new_thread(temporary_c_thread, &test_c_thread);
    if (thread == -1) {
        PyErr_SetString(PyExc_RuntimeError, "unable to start the thread");
        PyThread_release_lock(test_c_thread.start_event);
        PyThread_release_lock(test_c_thread.exit_event);
        goto exit;
    }

    PyThread_acquire_lock(test_c_thread.start_event, 1);
    PyThread_release_lock(test_c_thread.start_event);

    Py_BEGIN_ALLOW_THREADS
        PyThread_acquire_lock(test_c_thread.exit_event, 1);
        PyThread_release_lock(test_c_thread.exit_event);
    Py_END_ALLOW_THREADS

    Py_INCREF(Py_None);
    res = Py_None;

exit:
    Py_CLEAR(test_c_thread.callback);
    if (test_c_thread.start_event)
        PyThread_free_lock(test_c_thread.start_event);
    if (test_c_thread.exit_event)
        PyThread_free_lock(test_c_thread.exit_event);
    return res;
}
#endif   /* WITH_THREAD */

/* marshal */

static PyObject*
pymarshal_write_long_to_file(PyObject* self, PyObject *args)
{
    long value;
    char *filename;
    int version;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "lsi:pymarshal_write_long_to_file",
                          &value, &filename, &version))
        return NULL;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    PyMarshal_WriteLongToFile(value, fp, version);

    fclose(fp);
    if (PyErr_Occurred())
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
pymarshal_write_object_to_file(PyObject* self, PyObject *args)
{
    PyObject *obj;
    char *filename;
    int version;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "Osi:pymarshal_write_object_to_file",
                          &obj, &filename, &version))
        return NULL;

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    PyMarshal_WriteObjectToFile(obj, fp, version);

    fclose(fp);
    if (PyErr_Occurred())
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
pymarshal_read_short_from_file(PyObject* self, PyObject *args)
{
    int value;
    long pos;
    char *filename;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "s:pymarshal_read_short_from_file", &filename))
        return NULL;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    value = PyMarshal_ReadShortFromFile(fp);
    pos = ftell(fp);

    fclose(fp);
    if (PyErr_Occurred())
        return NULL;
    return Py_BuildValue("il", value, pos);
}

static PyObject*
pymarshal_read_long_from_file(PyObject* self, PyObject *args)
{
    long value, pos;
    char *filename;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "s:pymarshal_read_long_from_file", &filename))
        return NULL;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    value = PyMarshal_ReadLongFromFile(fp);
    pos = ftell(fp);

    fclose(fp);
    if (PyErr_Occurred())
        return NULL;
    return Py_BuildValue("ll", value, pos);
}

static PyObject*
pymarshal_read_last_object_from_file(PyObject* self, PyObject *args)
{
    PyObject *obj;
    long pos;
    char *filename;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "s:pymarshal_read_last_object_from_file", &filename))
        return NULL;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    obj = PyMarshal_ReadLastObjectFromFile(fp);
    pos = ftell(fp);

    fclose(fp);
    return Py_BuildValue("Nl", obj, pos);
}

static PyObject*
pymarshal_read_object_from_file(PyObject* self, PyObject *args)
{
    PyObject *obj;
    long pos;
    char *filename;
    FILE *fp;

    if (!PyArg_ParseTuple(args, "s:pymarshal_read_object_from_file", &filename))
        return NULL;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }

    obj = PyMarshal_ReadObjectFromFile(fp);
    pos = ftell(fp);

    fclose(fp);
    return Py_BuildValue("Nl", obj, pos);
}

static PyObject*
test_raise_signal(PyObject* self, PyObject *args)
{
    int signum, err;

    if (!PyArg_ParseTuple(args, "i:raise_signal", &signum)) {
        return NULL;
    }

    err = raise(signum);
    if (err)
        return PyErr_SetFromErrno(PyExc_OSError);

    if (PyErr_CheckSignals() < 0)
        return NULL;

    Py_RETURN_NONE;
}


#ifdef MS_WINDOWS
static PyObject*
msvcrt_CrtSetReportMode(PyObject* self, PyObject *args)
{
    int type, mode;
    int res;

    if (!PyArg_ParseTuple(args, "ii:CrtSetReportMode", &type, &mode)) {
        return NULL;
    }

    res = _CrtSetReportMode(type, mode);
    if (res == -1) {
        PyErr_SetFromErrno(PyExc_OSError);
        return NULL;
    }
    return PyInt_FromLong(res);
}


static PyObject*
msvcrt_CrtSetReportFile(PyObject* self, PyObject *args)
{
    int type, file;
    long res;

    if (!PyArg_ParseTuple(args, "ii:CrtSetReportFile", &type, &file)) {
        return NULL;
    }

    res = (long)_CrtSetReportFile(type, (_HFILE)file);

    return PyInt_FromLong(res);
}
#endif


#ifdef W_STOPCODE
static PyObject*
py_w_stopcode(PyObject *self, PyObject *args)
{
    int sig, status;
    if (!PyArg_ParseTuple(args, "i", &sig)) {
        return NULL;
    }
    status = W_STOPCODE(sig);
    return PyLong_FromLong(status);
}
#endif


/* Read memory from NULL (address 0) to raise a SIGSEGV or SIGBUS signal
   depending on the platform. This function is used by
   test.support._crash_python() to "crash" Python. */
static PyObject *
read_null(PyObject *self, PyObject *args)
{
    volatile int *x;
    volatile int y;

    x = NULL;
    y = *x;
    return PyLong_FromLong(y);

}


static PyMethodDef TestMethods[] = {
    {"raise_exception",         raise_exception,                 METH_VARARGS},
    {"set_errno",               set_errno,                       METH_VARARGS},
    {"test_config",             (PyCFunction)test_config,        METH_NOARGS},
#if defined(Py_USING_UNICODE) && !defined(Py_BUILD_CORE)
    {"test_datetime_capi",  test_datetime_capi,              METH_NOARGS},
#endif
    {"test_list_api",           (PyCFunction)test_list_api,      METH_NOARGS},
    {"test_dict_iteration",     (PyCFunction)test_dict_iteration,METH_NOARGS},
    {"test_lazy_hash_inheritance",      (PyCFunction)test_lazy_hash_inheritance,METH_NOARGS},
    {"test_broken_memoryview",          (PyCFunction)test_broken_memoryview,METH_NOARGS},
    {"test_to_contiguous",      (PyCFunction)test_to_contiguous, METH_NOARGS},
    {"test_from_contiguous",    (PyCFunction)test_from_contiguous, METH_NOARGS},
    {"test_long_api",           (PyCFunction)test_long_api,      METH_NOARGS},
    {"test_long_and_overflow", (PyCFunction)test_long_and_overflow,
     METH_NOARGS},
    {"test_long_numbits",       (PyCFunction)test_long_numbits,  METH_NOARGS},
    {"test_k_code",             (PyCFunction)test_k_code,        METH_NOARGS},
#ifdef Py_USING_UNICODE
    {"test_empty_argparse", (PyCFunction)test_empty_argparse,METH_NOARGS},
#endif
    {"test_null_strings",       (PyCFunction)test_null_strings,  METH_NOARGS},
    {"test_string_from_format", (PyCFunction)test_string_from_format, METH_NOARGS},
    {"test_with_docstring", (PyCFunction)test_with_docstring, METH_NOARGS,
     PyDoc_STR("This is a pretty normal docstring.")},

    {"test_buildvalue_N",       test_buildvalue_N,               METH_NOARGS},
    {"get_args", get_args, METH_VARARGS},
    {"get_kwargs", (PyCFunction)get_kwargs, METH_VARARGS|METH_KEYWORDS},
    {"getargs_tuple",           getargs_tuple,                   METH_VARARGS},
    {"getargs_keywords", (PyCFunction)getargs_keywords,
      METH_VARARGS|METH_KEYWORDS},
    {"getargs_b",               getargs_b,                       METH_VARARGS},
    {"getargs_B",               getargs_B,                       METH_VARARGS},
    {"getargs_h",               getargs_h,                       METH_VARARGS},
    {"getargs_H",               getargs_H,                       METH_VARARGS},
    {"getargs_I",               getargs_I,                       METH_VARARGS},
    {"getargs_k",               getargs_k,                       METH_VARARGS},
    {"getargs_i",               getargs_i,                       METH_VARARGS},
    {"getargs_l",               getargs_l,                       METH_VARARGS},
    {"getargs_n",               getargs_n,                       METH_VARARGS},
#ifdef HAVE_LONG_LONG
    {"getargs_L",               getargs_L,                       METH_VARARGS},
    {"getargs_K",               getargs_K,                       METH_VARARGS},
    {"test_longlong_api",       test_longlong_api,               METH_NOARGS},
    {"test_long_long_and_overflow",
        (PyCFunction)test_long_long_and_overflow, METH_NOARGS},
    {"test_L_code",             (PyCFunction)test_L_code,        METH_NOARGS},
#endif
    {"getargs_f",               getargs_f,                       METH_VARARGS},
    {"getargs_d",               getargs_d,                       METH_VARARGS},
    {"getargs_D",               getargs_D,                       METH_VARARGS},
    {"getargs_S",               getargs_S,                       METH_VARARGS},
    {"getargs_Y",               getargs_Y,                       METH_VARARGS},
#ifdef Py_USING_UNICODE
    {"getargs_U",               getargs_U,                       METH_VARARGS},
#endif
    {"getargs_c",               getargs_c,                       METH_VARARGS},
    {"getargs_s",               getargs_s,                       METH_VARARGS},
    {"getargs_s_star",          getargs_s_star,                  METH_VARARGS},
    {"getargs_s_hash",          getargs_s_hash,                  METH_VARARGS},
    {"getargs_t_hash",          getargs_t_hash,                  METH_VARARGS},
    {"getargs_z",               getargs_z,                       METH_VARARGS},
    {"getargs_z_star",          getargs_z_star,                  METH_VARARGS},
    {"getargs_z_hash",          getargs_z_hash,                  METH_VARARGS},
    {"getargs_w",               getargs_w,                       METH_VARARGS},
    {"getargs_w_star",          getargs_w_star,                  METH_VARARGS},
    {"getargs_w_hash",          getargs_w_hash,                  METH_VARARGS},
#ifdef Py_USING_UNICODE
    {"getargs_u",               getargs_u,                       METH_VARARGS},
    {"getargs_u_hash",          getargs_u_hash,                  METH_VARARGS},
#endif
    {"getargs_es",              getargs_es,                      METH_VARARGS},
    {"getargs_et",              getargs_et,                      METH_VARARGS},
    {"getargs_es_hash",         getargs_es_hash,                 METH_VARARGS},
    {"getargs_et_hash",         getargs_et_hash,                 METH_VARARGS},
#ifdef Py_USING_UNICODE
    {"codec_incrementalencoder",
     (PyCFunction)codec_incrementalencoder,      METH_VARARGS},
    {"codec_incrementaldecoder",
     (PyCFunction)codec_incrementaldecoder,      METH_VARARGS},
    {"test_u_code",             (PyCFunction)test_u_code,        METH_NOARGS},
    {"test_widechar",           (PyCFunction)test_widechar,      METH_NOARGS},
    {"unicode_encodedecimal",   unicode_encodedecimal,           METH_VARARGS},
#endif
#ifdef WITH_THREAD
    {"_test_thread_state",  test_thread_state,                   METH_VARARGS},
    {"_pending_threadfunc",     pending_threadfunc,              METH_VARARGS},
#endif
    {"test_capsule", (PyCFunction)test_capsule, METH_NOARGS},
    {"traceback_print", traceback_print,                 METH_VARARGS},
    {"code_newempty", code_newempty,                     METH_VARARGS},
    {"make_exception_with_doc", (PyCFunction)make_exception_with_doc,
     METH_VARARGS | METH_KEYWORDS},
    {"sequence_delitem", (PyCFunction)sequence_delitem, METH_VARARGS},
#ifdef WITH_THREAD
    {"call_in_temporary_c_thread", call_in_temporary_c_thread, METH_O,
     PyDoc_STR("set_error_class(error_class) -> None")},
#endif
    {"pymarshal_write_long_to_file",
        pymarshal_write_long_to_file, METH_VARARGS},
    {"pymarshal_write_object_to_file",
        pymarshal_write_object_to_file, METH_VARARGS},
    {"pymarshal_read_short_from_file",
        pymarshal_read_short_from_file, METH_VARARGS},
    {"pymarshal_read_long_from_file",
        pymarshal_read_long_from_file, METH_VARARGS},
    {"pymarshal_read_last_object_from_file",
        pymarshal_read_last_object_from_file, METH_VARARGS},
    {"pymarshal_read_object_from_file",
        pymarshal_read_object_from_file, METH_VARARGS},
    {"raise_signal", (PyCFunction)test_raise_signal, METH_VARARGS},
#ifdef MS_WINDOWS
    {"CrtSetReportMode", (PyCFunction)msvcrt_CrtSetReportMode, METH_VARARGS},
    {"CrtSetReportFile", (PyCFunction)msvcrt_CrtSetReportFile, METH_VARARGS},
#endif
#ifdef W_STOPCODE
    {"W_STOPCODE", py_w_stopcode, METH_VARARGS},
#endif
    {"_read_null", (PyCFunction)read_null, METH_NOARGS},
    {NULL, NULL} /* sentinel */
};

#define AddSym(d, n, f, v) {PyObject *o = f(v); PyDict_SetItemString(d, n, o); Py_DECREF(o);}

typedef struct {
    char bool_member;
    char byte_member;
    unsigned char ubyte_member;
    short short_member;
    unsigned short ushort_member;
    int int_member;
    unsigned int uint_member;
    long long_member;
    unsigned long ulong_member;
    float float_member;
    double double_member;
    char inplace_member[6];
#ifdef HAVE_LONG_LONG
    PY_LONG_LONG longlong_member;
    unsigned PY_LONG_LONG ulonglong_member;
#endif
} all_structmembers;

typedef struct {
    PyObject_HEAD
    all_structmembers structmembers;
} test_structmembers;

static struct PyMemberDef test_members[] = {
    {"T_BOOL", T_BOOL, offsetof(test_structmembers, structmembers.bool_member), 0, NULL},
    {"T_BYTE", T_BYTE, offsetof(test_structmembers, structmembers.byte_member), 0, NULL},
    {"T_UBYTE", T_UBYTE, offsetof(test_structmembers, structmembers.ubyte_member), 0, NULL},
    {"T_SHORT", T_SHORT, offsetof(test_structmembers, structmembers.short_member), 0, NULL},
    {"T_USHORT", T_USHORT, offsetof(test_structmembers, structmembers.ushort_member), 0, NULL},
    {"T_INT", T_INT, offsetof(test_structmembers, structmembers.int_member), 0, NULL},
    {"T_UINT", T_UINT, offsetof(test_structmembers, structmembers.uint_member), 0, NULL},
    {"T_LONG", T_LONG, offsetof(test_structmembers, structmembers.long_member), 0, NULL},
    {"T_ULONG", T_ULONG, offsetof(test_structmembers, structmembers.ulong_member), 0, NULL},
    {"T_FLOAT", T_FLOAT, offsetof(test_structmembers, structmembers.float_member), 0, NULL},
    {"T_DOUBLE", T_DOUBLE, offsetof(test_structmembers, structmembers.double_member), 0, NULL},
    {"T_STRING_INPLACE", T_STRING_INPLACE, offsetof(test_structmembers, structmembers.inplace_member), 0, NULL},
#ifdef HAVE_LONG_LONG
    {"T_LONGLONG", T_LONGLONG, offsetof(test_structmembers, structmembers.longlong_member), 0, NULL},
    {"T_ULONGLONG", T_ULONGLONG, offsetof(test_structmembers, structmembers.ulonglong_member), 0, NULL},
#endif
    {NULL}
};


static PyObject *
test_structmembers_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    static char *keywords[] = {
        "T_BOOL", "T_BYTE", "T_UBYTE", "T_SHORT", "T_USHORT",
        "T_INT", "T_UINT", "T_LONG", "T_ULONG",
        "T_FLOAT", "T_DOUBLE", "T_STRING_INPLACE",
#ifdef HAVE_LONG_LONG
        "T_LONGLONG", "T_ULONGLONG",
#endif
        NULL};
    static char *fmt = "|bbBhHiIlkfds#"
#ifdef HAVE_LONG_LONG
        "LK"
#endif
        ;
    test_structmembers *ob;
    const char *s = NULL;
    int string_len = 0;
    ob = PyObject_New(test_structmembers, type);
    if (ob == NULL)
        return NULL;
    memset(&ob->structmembers, 0, sizeof(all_structmembers));
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, fmt, keywords,
                                     &ob->structmembers.bool_member,
                                     &ob->structmembers.byte_member,
                                     &ob->structmembers.ubyte_member,
                                     &ob->structmembers.short_member,
                                     &ob->structmembers.ushort_member,
                                     &ob->structmembers.int_member,
                                     &ob->structmembers.uint_member,
                                     &ob->structmembers.long_member,
                                     &ob->structmembers.ulong_member,
                                     &ob->structmembers.float_member,
                                     &ob->structmembers.double_member,
                                     &s, &string_len
#ifdef HAVE_LONG_LONG
                                     , &ob->structmembers.longlong_member,
                                     &ob->structmembers.ulonglong_member
#endif
        )) {
        Py_DECREF(ob);
        return NULL;
    }
    if (s != NULL) {
        if (string_len > 5) {
            Py_DECREF(ob);
            PyErr_SetString(PyExc_ValueError, "string too long");
            return NULL;
        }
        strcpy(ob->structmembers.inplace_member, s);
    }
    else {
        strcpy(ob->structmembers.inplace_member, "");
    }
    return (PyObject *)ob;
}

static void
test_structmembers_free(PyObject *ob)
{
    PyObject_FREE(ob);
}

static PyTypeObject test_structmembersType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "test_structmembersType",
    sizeof(test_structmembers),         /* tp_basicsize */
    0,                                  /* tp_itemsize */
    test_structmembers_free,            /* destructor tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    PyObject_GenericGetAttr,            /* tp_getattro */
    PyObject_GenericSetAttr,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    0,                                  /* tp_flags */
    "Type containing all structmember types",
    0,                                  /* traverseproc tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    test_members,                       /* tp_members */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    test_structmembers_new,             /* tp_new */
};


PyMODINIT_FUNC
init_testcapi(void)
{
    PyObject *m;

    m = Py_InitModule("_testcapi", TestMethods);
    if (m == NULL)
        return;

    Py_TYPE(&_HashInheritanceTester_Type)=&PyType_Type;

    Py_TYPE(&test_structmembersType)=&PyType_Type;
    Py_INCREF(&test_structmembersType);
    /* don't use a name starting with "test", since we don't want
       test_capi to automatically call this */
    PyModule_AddObject(m, "_test_structmembersType", (PyObject *)&test_structmembersType);

    PyModule_AddObject(m, "CHAR_MAX", PyInt_FromLong(CHAR_MAX));
    PyModule_AddObject(m, "CHAR_MIN", PyInt_FromLong(CHAR_MIN));
    PyModule_AddObject(m, "UCHAR_MAX", PyInt_FromLong(UCHAR_MAX));
    PyModule_AddObject(m, "SHRT_MAX", PyInt_FromLong(SHRT_MAX));
    PyModule_AddObject(m, "SHRT_MIN", PyInt_FromLong(SHRT_MIN));
    PyModule_AddObject(m, "USHRT_MAX", PyInt_FromLong(USHRT_MAX));
    PyModule_AddObject(m, "INT_MAX",  PyLong_FromLong(INT_MAX));
    PyModule_AddObject(m, "INT_MIN",  PyLong_FromLong(INT_MIN));
    PyModule_AddObject(m, "UINT_MAX",  PyLong_FromUnsignedLong(UINT_MAX));
    PyModule_AddObject(m, "LONG_MAX", PyInt_FromLong(LONG_MAX));
    PyModule_AddObject(m, "LONG_MIN", PyInt_FromLong(LONG_MIN));
    PyModule_AddObject(m, "ULONG_MAX", PyLong_FromUnsignedLong(ULONG_MAX));
    PyModule_AddObject(m, "FLT_MAX", PyFloat_FromDouble(FLT_MAX));
    PyModule_AddObject(m, "FLT_MIN", PyFloat_FromDouble(FLT_MIN));
    PyModule_AddObject(m, "DBL_MAX", PyFloat_FromDouble(DBL_MAX));
    PyModule_AddObject(m, "DBL_MIN", PyFloat_FromDouble(DBL_MIN));
    PyModule_AddObject(m, "LLONG_MAX", PyLong_FromLongLong(PY_LLONG_MAX));
    PyModule_AddObject(m, "LLONG_MIN", PyLong_FromLongLong(PY_LLONG_MIN));
    PyModule_AddObject(m, "ULLONG_MAX", PyLong_FromUnsignedLongLong(PY_ULLONG_MAX));
    PyModule_AddObject(m, "PY_SSIZE_T_MAX", PyInt_FromSsize_t(PY_SSIZE_T_MAX));
    PyModule_AddObject(m, "PY_SSIZE_T_MIN", PyInt_FromSsize_t(PY_SSIZE_T_MIN));
    PyModule_AddObject(m, "SIZEOF_PYGC_HEAD", PyInt_FromSsize_t(sizeof(PyGC_Head)));

#ifdef MS_WINDOWS
    PyModule_AddIntConstant(m, "CRT_WARN", _CRT_WARN);
    PyModule_AddIntConstant(m, "CRT_ERROR", _CRT_ERROR);
    PyModule_AddIntConstant(m, "CRT_ASSERT", _CRT_ASSERT);
    PyModule_AddIntConstant(m, "CRTDBG_MODE_FILE", _CRTDBG_MODE_FILE);
    PyModule_AddIntConstant(m, "CRTDBG_FILE_STDERR", (int)_CRTDBG_FILE_STDERR);
#endif

    TestError = PyErr_NewException("_testcapi.error", NULL, NULL);
    Py_INCREF(TestError);
    PyModule_AddObject(m, "error", TestError);
}
