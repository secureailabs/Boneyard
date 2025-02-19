
/* Complex object implementation */

/* Borrows heavily from floatobject.c */

/* Submitted by Jim Hugunin */

#include "Python.h"
#include "structmember.h"

#ifndef WITHOUT_COMPLEX

/* Precisions used by repr() and str(), respectively.

   The repr() precision (17 significant decimal digits) is the minimal number
   that is guaranteed to have enough precision so that if the number is read
   back in the exact same binary value is recreated.  This is true for IEEE
   floating point by design, and also happens to work for all other modern
   hardware.

   The str() precision is chosen so that in most cases, the rounding noise
   created by various operations is suppressed, while giving plenty of
   precision for practical use.
*/

#define PREC_REPR       17
#define PREC_STR        12

/* elementary operations on complex numbers */

static Py_complex c_1 = {1., 0.};

Py_complex
c_sum(Py_complex a, Py_complex b)
{
    Py_complex r;
    r.real = a.real + b.real;
    r.imag = a.imag + b.imag;
    return r;
}

Py_complex
c_diff(Py_complex a, Py_complex b)
{
    Py_complex r;
    r.real = a.real - b.real;
    r.imag = a.imag - b.imag;
    return r;
}

Py_complex
c_neg(Py_complex a)
{
    Py_complex r;
    r.real = -a.real;
    r.imag = -a.imag;
    return r;
}

Py_complex
c_prod(Py_complex a, Py_complex b)
{
    Py_complex r;
    r.real = a.real*b.real - a.imag*b.imag;
    r.imag = a.real*b.imag + a.imag*b.real;
    return r;
}

Py_complex
c_quot(Py_complex a, Py_complex b)
{
    /******************************************************************
    This was the original algorithm.  It's grossly prone to spurious
    overflow and underflow errors.  It also merrily divides by 0 despite
    checking for that(!).  The code still serves a doc purpose here, as
    the algorithm following is a simple by-cases transformation of this
    one:

    Py_complex r;
    double d = b.real*b.real + b.imag*b.imag;
    if (d == 0.)
        errno = EDOM;
    r.real = (a.real*b.real + a.imag*b.imag)/d;
    r.imag = (a.imag*b.real - a.real*b.imag)/d;
    return r;
    ******************************************************************/

    /* This algorithm is better, and is pretty obvious:  first divide the
     * numerators and denominator by whichever of {b.real, b.imag} has
     * larger magnitude.  The earliest reference I found was to CACM
     * Algorithm 116 (Complex Division, Robert L. Smith, Stanford
     * University).  As usual, though, we're still ignoring all IEEE
     * endcases.
     */
     Py_complex r;      /* the result */
     const double abs_breal = b.real < 0 ? -b.real : b.real;
     const double abs_bimag = b.imag < 0 ? -b.imag : b.imag;

    if (abs_breal >= abs_bimag) {
        /* divide tops and bottom by b.real */
        if (abs_breal == 0.0) {
            errno = EDOM;
            r.real = r.imag = 0.0;
        }
        else {
            const double ratio = b.imag / b.real;
            const double denom = b.real + b.imag * ratio;
            r.real = (a.real + a.imag * ratio) / denom;
            r.imag = (a.imag - a.real * ratio) / denom;
        }
    }
    else if (abs_bimag >= abs_breal) {
        /* divide tops and bottom by b.imag */
        const double ratio = b.real / b.imag;
        const double denom = b.real * ratio + b.imag;
        assert(b.imag != 0.0);
        r.real = (a.real * ratio + a.imag) / denom;
        r.imag = (a.imag * ratio - a.real) / denom;
    }
    else {
        /* At least one of b.real or b.imag is a NaN */
        r.real = r.imag = Py_NAN;
    }
    return r;
}

Py_complex
c_pow(Py_complex a, Py_complex b)
{
    Py_complex r;
    double vabs,len,at,phase;
    if (b.real == 0. && b.imag == 0.) {
        r.real = 1.;
        r.imag = 0.;
    }
    else if (a.real == 0. && a.imag == 0.) {
        if (b.imag != 0. || b.real < 0.)
            errno = EDOM;
        r.real = 0.;
        r.imag = 0.;
    }
    else {
        vabs = hypot(a.real,a.imag);
        len = pow(vabs,b.real);
        at = atan2(a.imag, a.real);
        phase = at*b.real;
        if (b.imag != 0.0) {
            len /= exp(at*b.imag);
            phase += b.imag*log(vabs);
        }
        r.real = len*cos(phase);
        r.imag = len*sin(phase);
    }
    return r;
}

static Py_complex
c_powu(Py_complex x, long n)
{
    Py_complex r, p;
    long mask = 1;
    r = c_1;
    p = x;
    while (mask > 0 && n >= mask) {
        if (n & mask)
            r = c_prod(r,p);
        mask <<= 1;
        p = c_prod(p,p);
    }
    return r;
}

static Py_complex
c_powi(Py_complex x, long n)
{
    Py_complex cn;

    if (n > 100 || n < -100) {
        cn.real = (double) n;
        cn.imag = 0.;
        return c_pow(x,cn);
    }
    else if (n > 0)
        return c_powu(x,n);
    else
        return c_quot(c_1,c_powu(x,-n));

}

double
c_absp(Py_complex z)
{
    /* sets errno = ERANGE on overflow;  otherwise errno = 0 */
    double result;

    if (!Py_IS_FINITE(z.real) || !Py_IS_FINITE(z.imag)) {
        /* C99 rules: if either the real or the imaginary part is an
           infinity, return infinity, even if the other part is a
           NaN. */
        if (Py_IS_INFINITY(z.real)) {
            result = fabs(z.real);
            errno = 0;
            return result;
        }
        if (Py_IS_INFINITY(z.imag)) {
            result = fabs(z.imag);
            errno = 0;
            return result;
        }
        /* either the real or imaginary part is a NaN,
           and neither is infinite. Result should be NaN. */
        return Py_NAN;
    }
    result = hypot(z.real, z.imag);
    if (!Py_IS_FINITE(result))
        errno = ERANGE;
    else
        errno = 0;
    return result;
}

static PyObject *
complex_subtype_from_c_complex(PyTypeObject *type, Py_complex cval)
{
    PyObject *op;

    op = type->tp_alloc(type, 0);
    if (op != NULL)
        ((PyComplexObject *)op)->cval = cval;
    return op;
}

PyObject *
PyComplex_FromCComplex(Py_complex cval)
{
    register PyComplexObject *op;

    /* Inline PyObject_New */
    op = (PyComplexObject *) PyObject_MALLOC(sizeof(PyComplexObject));
    if (op == NULL)
        return PyErr_NoMemory();
    (void)PyObject_INIT(op, &PyComplex_Type);
    op->cval = cval;
    return (PyObject *) op;
}

static PyObject *
complex_subtype_from_doubles(PyTypeObject *type, double real, double imag)
{
    Py_complex c;
    c.real = real;
    c.imag = imag;
    return complex_subtype_from_c_complex(type, c);
}

PyObject *
PyComplex_FromDoubles(double real, double imag)
{
    Py_complex c;
    c.real = real;
    c.imag = imag;
    return PyComplex_FromCComplex(c);
}

double
PyComplex_RealAsDouble(PyObject *op)
{
    if (PyComplex_Check(op)) {
        return ((PyComplexObject *)op)->cval.real;
    }
    else {
        return PyFloat_AsDouble(op);
    }
}

double
PyComplex_ImagAsDouble(PyObject *op)
{
    if (PyComplex_Check(op)) {
        return ((PyComplexObject *)op)->cval.imag;
    }
    else {
        return 0.0;
    }
}

static PyObject *
try_complex_special_method(PyObject *op) {
    PyObject *f;
    static PyObject *complexstr;

    if (complexstr == NULL) {
        complexstr = PyString_InternFromString("__complex__");
        if (complexstr == NULL)
            return NULL;
    }
    if (PyInstance_Check(op)) {
        f = PyObject_GetAttr(op, complexstr);
        if (f == NULL) {
            if (PyErr_ExceptionMatches(PyExc_AttributeError))
                PyErr_Clear();
            else
                return NULL;
        }
    }
    else {
        f = _PyObject_LookupSpecial(op, "__complex__", &complexstr);
        if (f == NULL && PyErr_Occurred())
            return NULL;
    }
    if (f != NULL) {
        PyObject *res = PyObject_CallFunctionObjArgs(f, NULL);
        Py_DECREF(f);
        return res;
    }
    return NULL;
}

Py_complex
PyComplex_AsCComplex(PyObject *op)
{
    Py_complex cv;
    PyObject *newop = NULL;

    assert(op);
    /* If op is already of type PyComplex_Type, return its value */
    if (PyComplex_Check(op)) {
        return ((PyComplexObject *)op)->cval;
    }
    /* If not, use op's __complex__  method, if it exists */

    /* return -1 on failure */
    cv.real = -1.;
    cv.imag = 0.;

    newop = try_complex_special_method(op);

    if (newop) {
        if (!PyComplex_Check(newop)) {
            PyErr_SetString(PyExc_TypeError,
                "__complex__ should return a complex object");
            Py_DECREF(newop);
            return cv;
        }
        cv = ((PyComplexObject *)newop)->cval;
        Py_DECREF(newop);
        return cv;
    }
    else if (PyErr_Occurred()) {
        return cv;
    }
    /* If neither of the above works, interpret op as a float giving the
       real part of the result, and fill in the imaginary part as 0. */
    else {
        /* PyFloat_AsDouble will return -1 on failure */
        cv.real = PyFloat_AsDouble(op);
        return cv;
    }
}

static void
complex_dealloc(PyObject *op)
{
    op->ob_type->tp_free(op);
}


static PyObject *
complex_format(PyComplexObject *v, int precision, char format_code)
{
    PyObject *result = NULL;
    Py_ssize_t len;

    /* If these are non-NULL, they'll need to be freed. */
    char *pre = NULL;
    char *im = NULL;
    char *buf = NULL;

    /* These do not need to be freed. re is either an alias
       for pre or a pointer to a constant.  lead and tail
       are pointers to constants. */
    char *re = NULL;
    char *lead = "";
    char *tail = "";

    if (v->cval.real == 0. && copysign(1.0, v->cval.real)==1.0) {
        re = "";
        im = PyOS_double_to_string(v->cval.imag, format_code,
                                   precision, 0, NULL);
        if (!im) {
            PyErr_NoMemory();
            goto done;
        }
    } else {
        /* Format imaginary part with sign, real part without */
        pre = PyOS_double_to_string(v->cval.real, format_code,
                                    precision, 0, NULL);
        if (!pre) {
            PyErr_NoMemory();
            goto done;
        }
        re = pre;

        im = PyOS_double_to_string(v->cval.imag, format_code,
                                   precision, Py_DTSF_SIGN, NULL);
        if (!im) {
            PyErr_NoMemory();
            goto done;
        }
        lead = "(";
        tail = ")";
    }
    /* Alloc the final buffer. Add one for the "j" in the format string,
       and one for the trailing zero. */
    len = strlen(lead) + strlen(re) + strlen(im) + strlen(tail) + 2;
    buf = PyMem_Malloc(len);
    if (!buf) {
        PyErr_NoMemory();
        goto done;
    }
    PyOS_snprintf(buf, len, "%s%s%sj%s", lead, re, im, tail);
    result = PyString_FromString(buf);
  done:
    PyMem_Free(im);
    PyMem_Free(pre);
    PyMem_Free(buf);

    return result;
}

static int
complex_print(PyComplexObject *v, FILE *fp, int flags)
{
    PyObject *formatv;
    char *buf;
    if (flags & Py_PRINT_RAW)
        formatv = complex_format(v, PyFloat_STR_PRECISION, 'g');
    else
        formatv = complex_format(v, 0, 'r');
    if (formatv == NULL)
        return -1;
    buf = PyString_AS_STRING(formatv);
    Py_BEGIN_ALLOW_THREADS
    fputs(buf, fp);
    Py_END_ALLOW_THREADS
    Py_DECREF(formatv);
    return 0;
}

static PyObject *
complex_repr(PyComplexObject *v)
{
    return complex_format(v, 0, 'r');
}

static PyObject *
complex_str(PyComplexObject *v)
{
    return complex_format(v, PyFloat_STR_PRECISION, 'g');
}

static long
complex_hash(PyComplexObject *v)
{
    long hashreal, hashimag, combined;
    hashreal = _Py_HashDouble(v->cval.real);
    if (hashreal == -1)
        return -1;
    hashimag = _Py_HashDouble(v->cval.imag);
    if (hashimag == -1)
        return -1;
    /* Note:  if the imaginary part is 0, hashimag is 0 now,
     * so the following returns hashreal unchanged.  This is
     * important because numbers of different types that
     * compare equal must have the same hash value, so that
     * hash(x + 0*j) must equal hash(x).
     */
    combined = hashreal + 1000003 * hashimag;
    if (combined == -1)
        combined = -2;
    return combined;
}

/* This macro may return! */
#define TO_COMPLEX(obj, c) \
    if (PyComplex_Check(obj)) \
        c = ((PyComplexObject *)(obj))->cval; \
    else if (to_complex(&(obj), &(c)) < 0) \
        return (obj)

static int
to_complex(PyObject **pobj, Py_complex *pc)
{
    PyObject *obj = *pobj;

    pc->real = pc->imag = 0.0;
    if (PyInt_Check(obj)) {
        pc->real = PyInt_AS_LONG(obj);
        return 0;
    }
    if (PyLong_Check(obj)) {
        pc->real = PyLong_AsDouble(obj);
        if (pc->real == -1.0 && PyErr_Occurred()) {
            *pobj = NULL;
            return -1;
        }
        return 0;
    }
    if (PyFloat_Check(obj)) {
        pc->real = PyFloat_AsDouble(obj);
        return 0;
    }
    Py_INCREF(Py_NotImplemented);
    *pobj = Py_NotImplemented;
    return -1;
}


static PyObject *
complex_add(PyObject *v, PyObject *w)
{
    Py_complex result;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    PyFPE_START_PROTECT("complex_add", return 0)
    result = c_sum(a, b);
    PyFPE_END_PROTECT(result)
    return PyComplex_FromCComplex(result);
}

static PyObject *
complex_sub(PyObject *v, PyObject *w)
{
    Py_complex result;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);;
    PyFPE_START_PROTECT("complex_sub", return 0)
    result = c_diff(a, b);
    PyFPE_END_PROTECT(result)
    return PyComplex_FromCComplex(result);
}

static PyObject *
complex_mul(PyObject *v, PyObject *w)
{
    Py_complex result;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    PyFPE_START_PROTECT("complex_mul", return 0)
    result = c_prod(a, b);
    PyFPE_END_PROTECT(result)
    return PyComplex_FromCComplex(result);
}

static PyObject *
complex_div(PyObject *v, PyObject *w)
{
    Py_complex quot;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    PyFPE_START_PROTECT("complex_div", return 0)
    errno = 0;
    quot = c_quot(a, b);
    PyFPE_END_PROTECT(quot)
    if (errno == EDOM) {
        PyErr_SetString(PyExc_ZeroDivisionError, "complex division by zero");
        return NULL;
    }
    return PyComplex_FromCComplex(quot);
}

static PyObject *
complex_classic_div(PyObject *v, PyObject *w)
{
    Py_complex quot;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    if (Py_DivisionWarningFlag >= 2 &&
        PyErr_Warn(PyExc_DeprecationWarning,
                   "classic complex division") < 0)
        return NULL;

    PyFPE_START_PROTECT("complex_classic_div", return 0)
    errno = 0;
    quot = c_quot(a, b);
    PyFPE_END_PROTECT(quot)
    if (errno == EDOM) {
        PyErr_SetString(PyExc_ZeroDivisionError, "complex division by zero");
        return NULL;
    }
    return PyComplex_FromCComplex(quot);
}

static PyObject *
complex_remainder(PyObject *v, PyObject *w)
{
    Py_complex div, mod;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    if (PyErr_Warn(PyExc_DeprecationWarning,
                   "complex divmod(), // and % are deprecated") < 0)
        return NULL;

    errno = 0;
    div = c_quot(a, b); /* The raw divisor value. */
    if (errno == EDOM) {
        PyErr_SetString(PyExc_ZeroDivisionError, "complex remainder");
        return NULL;
    }
    div.real = floor(div.real); /* Use the floor of the real part. */
    div.imag = 0.0;
    mod = c_diff(a, c_prod(b, div));

    return PyComplex_FromCComplex(mod);
}


static PyObject *
complex_divmod(PyObject *v, PyObject *w)
{
    Py_complex div, mod;
    PyObject *d, *m, *z;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    if (PyErr_Warn(PyExc_DeprecationWarning,
                   "complex divmod(), // and % are deprecated") < 0)
        return NULL;

    errno = 0;
    div = c_quot(a, b); /* The raw divisor value. */
    if (errno == EDOM) {
        PyErr_SetString(PyExc_ZeroDivisionError, "complex divmod()");
        return NULL;
    }
    div.real = floor(div.real); /* Use the floor of the real part. */
    div.imag = 0.0;
    mod = c_diff(a, c_prod(b, div));
    d = PyComplex_FromCComplex(div);
    m = PyComplex_FromCComplex(mod);
    z = PyTuple_Pack(2, d, m);
    Py_XDECREF(d);
    Py_XDECREF(m);
    return z;
}

static PyObject *
complex_pow(PyObject *v, PyObject *w, PyObject *z)
{
    Py_complex p;
    Py_complex exponent;
    long int_exponent;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    if (z!=Py_None) {
        PyErr_SetString(PyExc_ValueError, "complex modulo");
        return NULL;
    }
    PyFPE_START_PROTECT("complex_pow", return 0)
    errno = 0;
    exponent = b;
    int_exponent = (long)exponent.real;
    if (exponent.imag == 0. && exponent.real == int_exponent)
        p = c_powi(a,int_exponent);
    else
        p = c_pow(a,exponent);

    PyFPE_END_PROTECT(p)
    Py_ADJUST_ERANGE2(p.real, p.imag);
    if (errno == EDOM) {
        PyErr_SetString(PyExc_ZeroDivisionError,
                        "0.0 to a negative or complex power");
        return NULL;
    }
    else if (errno == ERANGE) {
        PyErr_SetString(PyExc_OverflowError,
                        "complex exponentiation");
        return NULL;
    }
    return PyComplex_FromCComplex(p);
}

static PyObject *
complex_int_div(PyObject *v, PyObject *w)
{
    PyObject *t, *r;
    Py_complex a, b;
    TO_COMPLEX(v, a);
    TO_COMPLEX(w, b);
    if (PyErr_Warn(PyExc_DeprecationWarning,
                   "complex divmod(), // and % are deprecated") < 0)
        return NULL;

    t = complex_divmod(v, w);
    if (t != NULL) {
        r = PyTuple_GET_ITEM(t, 0);
        Py_INCREF(r);
        Py_DECREF(t);
        return r;
    }
    return NULL;
}

static PyObject *
complex_neg(PyComplexObject *v)
{
    Py_complex neg;
    neg.real = -v->cval.real;
    neg.imag = -v->cval.imag;
    return PyComplex_FromCComplex(neg);
}

static PyObject *
complex_pos(PyComplexObject *v)
{
    if (PyComplex_CheckExact(v)) {
        Py_INCREF(v);
        return (PyObject *)v;
    }
    else
        return PyComplex_FromCComplex(v->cval);
}

static PyObject *
complex_abs(PyComplexObject *v)
{
    double result;

    PyFPE_START_PROTECT("complex_abs", return 0)
    result = c_absp(v->cval);
    PyFPE_END_PROTECT(result)

    if (errno == ERANGE) {
        PyErr_SetString(PyExc_OverflowError,
                        "absolute value too large");
        return NULL;
    }
    return PyFloat_FromDouble(result);
}

static int
complex_nonzero(PyComplexObject *v)
{
    return v->cval.real != 0.0 || v->cval.imag != 0.0;
}

static int
complex_coerce(PyObject **pv, PyObject **pw)
{
    Py_complex cval;
    cval.imag = 0.;
    if (PyInt_Check(*pw)) {
        cval.real = (double)PyInt_AsLong(*pw);
        *pw = PyComplex_FromCComplex(cval);
        Py_INCREF(*pv);
        return 0;
    }
    else if (PyLong_Check(*pw)) {
        cval.real = PyLong_AsDouble(*pw);
        if (cval.real == -1.0 && PyErr_Occurred())
            return -1;
        *pw = PyComplex_FromCComplex(cval);
        Py_INCREF(*pv);
        return 0;
    }
    else if (PyFloat_Check(*pw)) {
        cval.real = PyFloat_AsDouble(*pw);
        *pw = PyComplex_FromCComplex(cval);
        Py_INCREF(*pv);
        return 0;
    }
    else if (PyComplex_Check(*pw)) {
        Py_INCREF(*pv);
        Py_INCREF(*pw);
        return 0;
    }
    return 1; /* Can't do it */
}

static PyObject *
complex_richcompare(PyObject *v, PyObject *w, int op)
{
    PyObject *res;
    Py_complex i;
    int equal;

    if (op != Py_EQ && op != Py_NE) {
        /* for backwards compatibility, comparisons with non-numbers return
         * NotImplemented.  Only comparisons with core numeric types raise
         * TypeError.
         */
        if (PyInt_Check(w) || PyLong_Check(w) ||
            PyFloat_Check(w) || PyComplex_Check(w)) {
            PyErr_SetString(PyExc_TypeError,
                            "no ordering relation is defined "
                            "for complex numbers");
            return NULL;
        }
        goto Unimplemented;
    }

    assert(PyComplex_Check(v));
    TO_COMPLEX(v, i);

    if (PyInt_Check(w) || PyLong_Check(w)) {
        /* Check for 0.0 imaginary part first to avoid the rich
         * comparison when possible.
         */
        if (i.imag == 0.0) {
            PyObject *j, *sub_res;
            j = PyFloat_FromDouble(i.real);
            if (j == NULL)
                return NULL;

            sub_res = PyObject_RichCompare(j, w, op);
            Py_DECREF(j);
            return sub_res;
        }
        else {
            equal = 0;
        }
    }
    else if (PyFloat_Check(w)) {
        equal = (i.real == PyFloat_AsDouble(w) && i.imag == 0.0);
    }
    else if (PyComplex_Check(w)) {
        Py_complex j;

        TO_COMPLEX(w, j);
        equal = (i.real == j.real && i.imag == j.imag);
    }
    else {
        goto Unimplemented;
    }

    if (equal == (op == Py_EQ))
         res = Py_True;
    else
         res = Py_False;

    Py_INCREF(res);
    return res;

  Unimplemented:
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

static PyObject *
complex_int(PyObject *v)
{
    PyErr_SetString(PyExc_TypeError,
               "can't convert complex to int");
    return NULL;
}

static PyObject *
complex_long(PyObject *v)
{
    PyErr_SetString(PyExc_TypeError,
               "can't convert complex to long");
    return NULL;
}

static PyObject *
complex_float(PyObject *v)
{
    PyErr_SetString(PyExc_TypeError,
               "can't convert complex to float");
    return NULL;
}

static PyObject *
complex_conjugate(PyObject *self)
{
    Py_complex c;
    c = ((PyComplexObject *)self)->cval;
    c.imag = -c.imag;
    return PyComplex_FromCComplex(c);
}

PyDoc_STRVAR(complex_conjugate_doc,
"complex.conjugate() -> complex\n"
"\n"
"Return the complex conjugate of its argument. (3-4j).conjugate() == 3+4j.");

static PyObject *
complex_getnewargs(PyComplexObject *v)
{
    Py_complex c = v->cval;
    return Py_BuildValue("(dd)", c.real, c.imag);
}

PyDoc_STRVAR(complex__format__doc,
"complex.__format__() -> str\n"
"\n"
"Convert to a string according to format_spec.");

static PyObject *
complex__format__(PyObject* self, PyObject* args)
{
    PyObject *format_spec;

    if (!PyArg_ParseTuple(args, "O:__format__", &format_spec))
        return NULL;
    if (PyBytes_Check(format_spec))
        return _PyComplex_FormatAdvanced(self,
                                         PyBytes_AS_STRING(format_spec),
                                         PyBytes_GET_SIZE(format_spec));
    if (PyUnicode_Check(format_spec)) {
        /* Convert format_spec to a str */
        PyObject *result;
        PyObject *str_spec = PyObject_Str(format_spec);

        if (str_spec == NULL)
            return NULL;

        result = _PyComplex_FormatAdvanced(self,
                                           PyBytes_AS_STRING(str_spec),
                                           PyBytes_GET_SIZE(str_spec));

        Py_DECREF(str_spec);
        return result;
    }
    PyErr_SetString(PyExc_TypeError, "__format__ requires str or unicode");
    return NULL;
}

#if 0
static PyObject *
complex_is_finite(PyObject *self)
{
    Py_complex c;
    c = ((PyComplexObject *)self)->cval;
    return PyBool_FromLong((long)(Py_IS_FINITE(c.real) &&
                                  Py_IS_FINITE(c.imag)));
}

PyDoc_STRVAR(complex_is_finite_doc,
"complex.is_finite() -> bool\n"
"\n"
"Returns True if the real and the imaginary part is finite.");
#endif

static PyMethodDef complex_methods[] = {
    {"conjugate",       (PyCFunction)complex_conjugate, METH_NOARGS,
     complex_conjugate_doc},
#if 0
    {"is_finite",       (PyCFunction)complex_is_finite, METH_NOARGS,
     complex_is_finite_doc},
#endif
    {"__getnewargs__",          (PyCFunction)complex_getnewargs,        METH_NOARGS},
    {"__format__",          (PyCFunction)complex__format__,
                                       METH_VARARGS, complex__format__doc},
    {NULL,              NULL}           /* sentinel */
};

static PyMemberDef complex_members[] = {
    {"real", T_DOUBLE, offsetof(PyComplexObject, cval.real), READONLY,
     "the real part of a complex number"},
    {"imag", T_DOUBLE, offsetof(PyComplexObject, cval.imag), READONLY,
     "the imaginary part of a complex number"},
    {0},
};

static PyObject *
complex_subtype_from_string(PyTypeObject *type, PyObject *v)
{
    const char *s, *start;
    char *end;
    double x=0.0, y=0.0, z;
    int got_bracket=0;
#ifdef Py_USING_UNICODE
    char *s_buffer = NULL;
#endif
    Py_ssize_t len;

    if (PyString_Check(v)) {
        s = PyString_AS_STRING(v);
        len = PyString_GET_SIZE(v);
    }
#ifdef Py_USING_UNICODE
    else if (PyUnicode_Check(v)) {
        s_buffer = (char *)PyMem_MALLOC(PyUnicode_GET_SIZE(v)+1);
        if (s_buffer == NULL)
            return PyErr_NoMemory();
        if (PyUnicode_EncodeDecimal(PyUnicode_AS_UNICODE(v),
                                    PyUnicode_GET_SIZE(v),
                                    s_buffer,
                                    NULL))
            goto error;
        s = s_buffer;
        len = strlen(s);
    }
#endif
    else {
        PyErr_SetString(PyExc_TypeError,
                        "complex() arg is not a string");
        return NULL;
    }

    /* position on first nonblank */
    start = s;
    while (Py_ISSPACE(*s))
        s++;
    if (*s == '(') {
        /* Skip over possible bracket from repr(). */
        got_bracket = 1;
        s++;
        while (Py_ISSPACE(*s))
            s++;
    }

    /* a valid complex string usually takes one of the three forms:

         <float>                  - real part only
         <float>j                 - imaginary part only
         <float><signed-float>j   - real and imaginary parts

       where <float> represents any numeric string that's accepted by the
       float constructor (including 'nan', 'inf', 'infinity', etc.), and
       <signed-float> is any string of the form <float> whose first
       character is '+' or '-'.

       For backwards compatibility, the extra forms

         <float><sign>j
         <sign>j
         j

       are also accepted, though support for these forms may be removed from
       a future version of Python.
    */

    /* first look for forms starting with <float> */
    z = PyOS_string_to_double(s, &end, NULL);
    if (z == -1.0 && PyErr_Occurred()) {
        if (PyErr_ExceptionMatches(PyExc_ValueError))
            PyErr_Clear();
        else
            goto error;
    }
    if (end != s) {
        /* all 4 forms starting with <float> land here */
        s = end;
        if (*s == '+' || *s == '-') {
            /* <float><signed-float>j | <float><sign>j */
            x = z;
            y = PyOS_string_to_double(s, &end, NULL);
            if (y == -1.0 && PyErr_Occurred()) {
                if (PyErr_ExceptionMatches(PyExc_ValueError))
                    PyErr_Clear();
                else
                    goto error;
            }
            if (end != s)
                /* <float><signed-float>j */
                s = end;
            else {
                /* <float><sign>j */
                y = *s == '+' ? 1.0 : -1.0;
                s++;
            }
            if (!(*s == 'j' || *s == 'J'))
                goto parse_error;
            s++;
        }
        else if (*s == 'j' || *s == 'J') {
            /* <float>j */
            s++;
            y = z;
        }
        else
            /* <float> */
            x = z;
    }
    else {
        /* not starting with <float>; must be <sign>j or j */
        if (*s == '+' || *s == '-') {
            /* <sign>j */
            y = *s == '+' ? 1.0 : -1.0;
            s++;
        }
        else
            /* j */
            y = 1.0;
        if (!(*s == 'j' || *s == 'J'))
            goto parse_error;
        s++;
    }

    /* trailing whitespace and closing bracket */
    while (Py_ISSPACE(*s))
        s++;
    if (got_bracket) {
        /* if there was an opening parenthesis, then the corresponding
           closing parenthesis should be right here */
        if (*s != ')')
            goto parse_error;
        s++;
        while (Py_ISSPACE(*s))
            s++;
    }

    /* we should now be at the end of the string */
    if (s-start != len)
        goto parse_error;


#ifdef Py_USING_UNICODE
    if (s_buffer)
        PyMem_FREE(s_buffer);
#endif
    return complex_subtype_from_doubles(type, x, y);

  parse_error:
    PyErr_SetString(PyExc_ValueError,
                    "complex() arg is a malformed string");
  error:
#ifdef Py_USING_UNICODE
    if (s_buffer)
        PyMem_FREE(s_buffer);
#endif
    return NULL;
}

static PyObject *
complex_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *r, *i, *tmp;
    PyNumberMethods *nbr, *nbi = NULL;
    Py_complex cr, ci;
    int own_r = 0;
    int cr_is_complex = 0;
    int ci_is_complex = 0;
    static char *kwlist[] = {"real", "imag", 0};

    r = Py_False;
    i = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO:complex", kwlist,
                                     &r, &i))
        return NULL;

    /* Special-case for a single argument when type(arg) is complex. */
    if (PyComplex_CheckExact(r) && i == NULL &&
        type == &PyComplex_Type) {
        /* Note that we can't know whether it's safe to return
           a complex *subclass* instance as-is, hence the restriction
           to exact complexes here.  If either the input or the
           output is a complex subclass, it will be handled below
           as a non-orthogonal vector.  */
        Py_INCREF(r);
        return r;
    }
    if (PyString_Check(r) || PyUnicode_Check(r)) {
        if (i != NULL) {
            PyErr_SetString(PyExc_TypeError,
                            "complex() can't take second arg"
                            " if first is a string");
            return NULL;
        }
        return complex_subtype_from_string(type, r);
    }
    if (i != NULL && (PyString_Check(i) || PyUnicode_Check(i))) {
        PyErr_SetString(PyExc_TypeError,
                        "complex() second arg can't be a string");
        return NULL;
    }

    tmp = try_complex_special_method(r);
    if (tmp) {
        r = tmp;
        own_r = 1;
    }
    else if (PyErr_Occurred()) {
        return NULL;
    }

    nbr = r->ob_type->tp_as_number;
    if (i != NULL)
        nbi = i->ob_type->tp_as_number;
    if (nbr == NULL || nbr->nb_float == NULL ||
        ((i != NULL) && (nbi == NULL || nbi->nb_float == NULL))) {
        PyErr_SetString(PyExc_TypeError,
                   "complex() argument must be a string or a number");
        if (own_r) {
            Py_DECREF(r);
        }
        return NULL;
    }

    /* If we get this far, then the "real" and "imag" parts should
       both be treated as numbers, and the constructor should return a
       complex number equal to (real + imag*1j).

       Note that we do NOT assume the input to already be in canonical
       form; the "real" and "imag" parts might themselves be complex
       numbers, which slightly complicates the code below. */
    if (PyComplex_Check(r)) {
        /* Note that if r is of a complex subtype, we're only
           retaining its real & imag parts here, and the return
           value is (properly) of the builtin complex type. */
        cr = ((PyComplexObject*)r)->cval;
        cr_is_complex = 1;
        if (own_r) {
            Py_DECREF(r);
        }
    }
    else {
        /* The "real" part really is entirely real, and contributes
           nothing in the imaginary direction.
           Just treat it as a double. */
        tmp = PyNumber_Float(r);
        if (own_r) {
            /* r was a newly created complex number, rather
               than the original "real" argument. */
            Py_DECREF(r);
        }
        if (tmp == NULL)
            return NULL;
        if (!PyFloat_Check(tmp)) {
            PyErr_SetString(PyExc_TypeError,
                            "float(r) didn't return a float");
            Py_DECREF(tmp);
            return NULL;
        }
        cr.real = PyFloat_AsDouble(tmp);
        cr.imag = 0.0;
        Py_DECREF(tmp);
    }
    if (i == NULL) {
        ci.real = cr.imag;
    }
    else if (PyComplex_Check(i)) {
        ci = ((PyComplexObject*)i)->cval;
        ci_is_complex = 1;
    } else {
        /* The "imag" part really is entirely imaginary, and
           contributes nothing in the real direction.
           Just treat it as a double. */
        tmp = (*nbi->nb_float)(i);
        if (tmp == NULL)
            return NULL;
        ci.real = PyFloat_AsDouble(tmp);
        Py_DECREF(tmp);
    }
    /*  If the input was in canonical form, then the "real" and "imag"
        parts are real numbers, so that ci.imag and cr.imag are zero.
        We need this correction in case they were not real numbers. */

    if (ci_is_complex) {
        cr.real -= ci.imag;
    }
    if (cr_is_complex && i != NULL) {
        ci.real += cr.imag;
    }
    return complex_subtype_from_doubles(type, cr.real, ci.real);
}

PyDoc_STRVAR(complex_doc,
"complex(real[, imag]) -> complex number\n"
"\n"
"Create a complex number from a real part and an optional imaginary part.\n"
"This is equivalent to (real + imag*1j) where imag defaults to 0.");

static PyNumberMethods complex_as_number = {
    (binaryfunc)complex_add,                    /* nb_add */
    (binaryfunc)complex_sub,                    /* nb_subtract */
    (binaryfunc)complex_mul,                    /* nb_multiply */
    (binaryfunc)complex_classic_div,            /* nb_divide */
    (binaryfunc)complex_remainder,              /* nb_remainder */
    (binaryfunc)complex_divmod,                 /* nb_divmod */
    (ternaryfunc)complex_pow,                   /* nb_power */
    (unaryfunc)complex_neg,                     /* nb_negative */
    (unaryfunc)complex_pos,                     /* nb_positive */
    (unaryfunc)complex_abs,                     /* nb_absolute */
    (inquiry)complex_nonzero,                   /* nb_nonzero */
    0,                                          /* nb_invert */
    0,                                          /* nb_lshift */
    0,                                          /* nb_rshift */
    0,                                          /* nb_and */
    0,                                          /* nb_xor */
    0,                                          /* nb_or */
    complex_coerce,                             /* nb_coerce */
    complex_int,                                /* nb_int */
    complex_long,                               /* nb_long */
    complex_float,                              /* nb_float */
    0,                                          /* nb_oct */
    0,                                          /* nb_hex */
    0,                                          /* nb_inplace_add */
    0,                                          /* nb_inplace_subtract */
    0,                                          /* nb_inplace_multiply*/
    0,                                          /* nb_inplace_divide */
    0,                                          /* nb_inplace_remainder */
    0,                                          /* nb_inplace_power */
    0,                                          /* nb_inplace_lshift */
    0,                                          /* nb_inplace_rshift */
    0,                                          /* nb_inplace_and */
    0,                                          /* nb_inplace_xor */
    0,                                          /* nb_inplace_or */
    (binaryfunc)complex_int_div,                /* nb_floor_divide */
    (binaryfunc)complex_div,                    /* nb_true_divide */
    0,                                          /* nb_inplace_floor_divide */
    0,                                          /* nb_inplace_true_divide */
};

PyTypeObject PyComplex_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "complex",
    sizeof(PyComplexObject),
    0,
    complex_dealloc,                            /* tp_dealloc */
    (printfunc)complex_print,                   /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    (reprfunc)complex_repr,                     /* tp_repr */
    &complex_as_number,                         /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    (hashfunc)complex_hash,                     /* tp_hash */
    0,                                          /* tp_call */
    (reprfunc)complex_str,                      /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES |
        Py_TPFLAGS_BASETYPE,                    /* tp_flags */
    complex_doc,                                /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    complex_richcompare,                        /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    complex_methods,                            /* tp_methods */
    complex_members,                            /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    PyType_GenericAlloc,                        /* tp_alloc */
    complex_new,                                /* tp_new */
    PyObject_Del,                               /* tp_free */
};

#endif
