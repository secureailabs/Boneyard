/* File: convolvemodule.c
 * This file is auto-generated with f2py (version:2).
 * f2py is a Fortran to Python Interface Generator (FPIG), Second Edition,
 * written by Pearu Peterson <pearu@cens.ioc.ee>.
 * Generation date: Fri Mar  1 18:44:25 2019
 * Do not edit this file directly unless you know what you are doing!!!
 */

#ifdef __cplusplus
extern "C" {
#endif

/*********************** See f2py2e/cfuncs.py: includes ***********************/
#include "Python.h"
#include <stdarg.h>
#include "fortranobject.h"
#include <setjmp.h>
#include <math.h>

/**************** See f2py2e/rules.py: mod_rules['modulebody'] ****************/
static PyObject *convolve_error;
static PyObject *convolve_module;

/*********************** See f2py2e/cfuncs.py: typedefs ***********************/
typedef signed char signed_char;

/****************** See f2py2e/cfuncs.py: typedefs_generated ******************/
typedef double(*cb_kernel_func_in_convolve__user__routines_typedef)(int);

/********************** See f2py2e/cfuncs.py: cppmacros **********************/
#ifndef max
#define max(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a < b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b) ((a > b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a < b) ? (a) : (b))
#endif

#define PRINTPYOBJERR(obj)\
    fprintf(stderr,"convolve.error is related to ");\
    PyObject_Print((PyObject *)obj,stderr,Py_PRINT_RAW);\
    fprintf(stderr,"\n");

#define GETSCALARFROMPYTUPLE(tuple,index,var,ctype,mess) {\
        if ((capi_tmp = PyTuple_GetItem((tuple),(index)))==NULL) goto capi_fail;\
        if (!(ctype ## _from_pyobj((var),capi_tmp,mess)))\
            goto capi_fail;\
    }

#define pyobj_from_int1(v) (PyInt_FromLong(v))
#ifdef DEBUGCFUNCS
#define CFUNCSMESS(mess) fprintf(stderr,"debug-capi:"mess);
#define CFUNCSMESSPY(mess,obj) CFUNCSMESS(mess) \
    PyObject_Print((PyObject *)obj,stderr,Py_PRINT_RAW);\
    fprintf(stderr,"\n");
#else
#define CFUNCSMESS(mess)
#define CFUNCSMESSPY(mess,obj)
#endif

#define rank(var) var ## _Rank
#define shape(var,dim) var ## _Dims[dim]
#define old_rank(var) (PyArray_NDIM((PyArrayObject *)(capi_ ## var ## _tmp)))
#define old_shape(var,dim) PyArray_DIM(((PyArrayObject *)(capi_ ## var ## _tmp)),dim)
#define fshape(var,dim) shape(var,rank(var)-dim-1)
#define len(var) shape(var,0)
#define flen(var) fshape(var,0)
#define old_size(var) PyArray_SIZE((PyArrayObject *)(capi_ ## var ## _tmp))
/* #define index(i) capi_i ## i */
#define slen(var) capi_ ## var ## _len
#define size(var, ...) f2py_size((PyArrayObject *)(capi_ ## var ## _tmp), ## __VA_ARGS__, -1)

#define SWAP(a,b,t) {\
    t *c;\
    c = a;\
    a = b;\
    b = c;}

#define CHECKSCALAR(check,tcheck,name,show,var)\
    if (!(check)) {\
        char errstring[256];\
        sprintf(errstring, "%s: "show, "("tcheck") failed for "name, var);\
        PyErr_SetString(convolve_error,errstring);\
        /*goto capi_fail;*/\
    } else 
#if defined(PREPEND_FORTRAN)
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F
#else
#define F_FUNC(f,F) _##f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) _##F##_
#else
#define F_FUNC(f,F) _##f##_
#endif
#endif
#else
#if defined(NO_APPEND_FORTRAN)
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F
#else
#define F_FUNC(f,F) f
#endif
#else
#if defined(UPPERCASE_FORTRAN)
#define F_FUNC(f,F) F##_
#else
#define F_FUNC(f,F) f##_
#endif
#endif
#endif
#if defined(UNDERSCORE_G77)
#define F_FUNC_US(f,F) F_FUNC(f##_,F##_)
#else
#define F_FUNC_US(f,F) F_FUNC(f,F)
#endif


/************************ See f2py2e/cfuncs.py: cfuncs ************************/
static int f2py_size(PyArrayObject* var, ...)
{
  npy_int sz = 0;
  npy_int dim;
  npy_int rank;
  va_list argp;
  va_start(argp, var);
  dim = va_arg(argp, npy_int);
  if (dim==-1)
    {
      sz = PyArray_SIZE(var);
    }
  else
    {
      rank = PyArray_NDIM(var);
      if (dim>=1 && dim<=rank)
        sz = PyArray_DIM(var, dim-1);
      else
        fprintf(stderr, "f2py_size: 2nd argument value=%d fails to satisfy 1<=value<=%d. Result will be 0.\n", dim, rank);
    }
  va_end(argp);
  return sz;
}

static int double_from_pyobj(double* v,PyObject *obj,const char *errmess) {
    PyObject* tmp = NULL;
    if (PyFloat_Check(obj)) {
#ifdef __sgi
        *v = PyFloat_AsDouble(obj);
#else
        *v = PyFloat_AS_DOUBLE(obj);
#endif
        return 1;
    }
    tmp = PyNumber_Float(obj);
    if (tmp) {
#ifdef __sgi
        *v = PyFloat_AsDouble(tmp);
#else
        *v = PyFloat_AS_DOUBLE(tmp);
#endif
        Py_DECREF(tmp);
        return 1;
    }
    if (PyComplex_Check(obj))
        tmp = PyObject_GetAttrString(obj,"real");
    else if (PyString_Check(obj) || PyUnicode_Check(obj))
        /*pass*/;
    else if (PySequence_Check(obj))
        tmp = PySequence_GetItem(obj,0);
    if (tmp) {
        PyErr_Clear();
        if (double_from_pyobj(v,tmp,errmess)) {Py_DECREF(tmp); return 1;}
        Py_DECREF(tmp);
    }
    {
        PyObject* err = PyErr_Occurred();
        if (err==NULL) err = convolve_error;
        PyErr_SetString(err,errmess);
    }
    return 0;
}

static int create_cb_arglist(PyObject* fun,PyTupleObject* xa,const int maxnofargs,const int nofoptargs,int *nofargs,PyTupleObject **args,const char *errmess) {
    PyObject *tmp = NULL;
    PyObject *tmp_fun = NULL;
    int tot,opt,ext,siz,i,di=0;
    CFUNCSMESS("create_cb_arglist\n");
    tot=opt=ext=siz=0;
    /* Get the total number of arguments */
    if (PyFunction_Check(fun))
        tmp_fun = fun;
    else {
        di = 1;
        if (PyObject_HasAttrString(fun,"im_func")) {
            tmp_fun = PyObject_GetAttrString(fun,"im_func");
        }
        else if (PyObject_HasAttrString(fun,"__call__")) {
            tmp = PyObject_GetAttrString(fun,"__call__");
            if (PyObject_HasAttrString(tmp,"im_func"))
                tmp_fun = PyObject_GetAttrString(tmp,"im_func");
            else {
                tmp_fun = fun; /* built-in function */
                tot = maxnofargs;
                if (xa != NULL)
                    tot += PyTuple_Size((PyObject *)xa);
            }
            Py_XDECREF(tmp);
        }
        else if (PyFortran_Check(fun) || PyFortran_Check1(fun)) {
            tot = maxnofargs;
            if (xa != NULL)
                tot += PyTuple_Size((PyObject *)xa);
            tmp_fun = fun;
        }
        else if (F2PyCapsule_Check(fun)) {
            tot = maxnofargs;
            if (xa != NULL)
                ext = PyTuple_Size((PyObject *)xa);
            if(ext>0) {
                fprintf(stderr,"extra arguments tuple cannot be used with CObject call-back\n");
                goto capi_fail;
            }
            tmp_fun = fun;
        }
    }
if (tmp_fun==NULL) {
fprintf(stderr,"Call-back argument must be function|instance|instance.__call__|f2py-function but got %s.\n",(fun==NULL?"NULL":Py_TYPE(fun)->tp_name));
goto capi_fail;
}
#if PY_VERSION_HEX >= 0x03000000
    if (PyObject_HasAttrString(tmp_fun,"__code__")) {
        if (PyObject_HasAttrString(tmp = PyObject_GetAttrString(tmp_fun,"__code__"),"co_argcount"))
#else
    if (PyObject_HasAttrString(tmp_fun,"func_code")) {
        if (PyObject_HasAttrString(tmp = PyObject_GetAttrString(tmp_fun,"func_code"),"co_argcount"))
#endif
            tot = PyInt_AsLong(PyObject_GetAttrString(tmp,"co_argcount")) - di;
        Py_XDECREF(tmp);
    }
    /* Get the number of optional arguments */
#if PY_VERSION_HEX >= 0x03000000
    if (PyObject_HasAttrString(tmp_fun,"__defaults__")) {
        if (PyTuple_Check(tmp = PyObject_GetAttrString(tmp_fun,"__defaults__")))
#else
    if (PyObject_HasAttrString(tmp_fun,"func_defaults")) {
        if (PyTuple_Check(tmp = PyObject_GetAttrString(tmp_fun,"func_defaults")))
#endif
            opt = PyTuple_Size(tmp);
        Py_XDECREF(tmp);
    }
    /* Get the number of extra arguments */
    if (xa != NULL)
        ext = PyTuple_Size((PyObject *)xa);
    /* Calculate the size of call-backs argument list */
    siz = MIN(maxnofargs+ext,tot);
    *nofargs = MAX(0,siz-ext);
#ifdef DEBUGCFUNCS
    fprintf(stderr,"debug-capi:create_cb_arglist:maxnofargs(-nofoptargs),tot,opt,ext,siz,nofargs=%d(-%d),%d,%d,%d,%d,%d\n",maxnofargs,nofoptargs,tot,opt,ext,siz,*nofargs);
#endif
    if (siz<tot-opt) {
        fprintf(stderr,"create_cb_arglist: Failed to build argument list (siz) with enough arguments (tot-opt) required by user-supplied function (siz,tot,opt=%d,%d,%d).\n",siz,tot,opt);
        goto capi_fail;
    }
    /* Initialize argument list */
    *args = (PyTupleObject *)PyTuple_New(siz);
    for (i=0;i<*nofargs;i++) {
        Py_INCREF(Py_None);
        PyTuple_SET_ITEM((PyObject *)(*args),i,Py_None);
    }
    if (xa != NULL)
        for (i=(*nofargs);i<siz;i++) {
            tmp = PyTuple_GetItem((PyObject *)xa,i-(*nofargs));
            Py_INCREF(tmp);
            PyTuple_SET_ITEM(*args,i,tmp);
        }
    CFUNCSMESS("create_cb_arglist-end\n");
    return 1;
capi_fail:
    if ((PyErr_Occurred())==NULL)
        PyErr_SetString(convolve_error,errmess);
    return 0;
}

static int int_from_pyobj(int* v,PyObject *obj,const char *errmess) {
    PyObject* tmp = NULL;
    if (PyInt_Check(obj)) {
        *v = (int)PyInt_AS_LONG(obj);
        return 1;
    }
    tmp = PyNumber_Int(obj);
    if (tmp) {
        *v = PyInt_AS_LONG(tmp);
        Py_DECREF(tmp);
        return 1;
    }
    if (PyComplex_Check(obj))
        tmp = PyObject_GetAttrString(obj,"real");
    else if (PyString_Check(obj) || PyUnicode_Check(obj))
        /*pass*/;
    else if (PySequence_Check(obj))
        tmp = PySequence_GetItem(obj,0);
    if (tmp) {
        PyErr_Clear();
        if (int_from_pyobj(v,tmp,errmess)) {Py_DECREF(tmp); return 1;}
        Py_DECREF(tmp);
    }
    {
        PyObject* err = PyErr_Occurred();
        if (err==NULL) err = convolve_error;
        PyErr_SetString(err,errmess);
    }
    return 0;
}


/********************* See f2py2e/cfuncs.py: userincludes *********************/
/*need_userincludes*/

/********************* See f2py2e/capi_rules.py: usercode *********************/


/* See f2py2e/rules.py */
extern void init_convolution_kernel(int,double*,int,cb_kernel_func_in_convolve__user__routines_typedef,int);
extern void destroy_convolve_cache(void);
extern void convolve(int,double*,double*,int);
extern void convolve_z(int,double*,double*,double*);
/*eof externroutines*/

/******************** See f2py2e/capi_rules.py: usercode1 ********************/


/******************* See f2py2e/cb_rules.py: buildcallback *******************/

/***************** cb_kernel_func_in_convolve__user__routines *****************/
PyObject *cb_kernel_func_in_convolve__user__routines_capi = NULL;/*was Py_None*/
PyTupleObject *cb_kernel_func_in_convolve__user__routines_args_capi = NULL;
int cb_kernel_func_in_convolve__user__routines_nofargs = 0;
jmp_buf cb_kernel_func_in_convolve__user__routines_jmpbuf;
/*typedef double(*cb_kernel_func_in_convolve__user__routines_typedef)(int);*/
static double cb_kernel_func_in_convolve__user__routines (int k) {
  PyTupleObject *capi_arglist = cb_kernel_func_in_convolve__user__routines_args_capi;
  PyObject *capi_return = NULL;
  PyObject *capi_tmp = NULL;
  int capi_j,capi_i = 0;
  int capi_longjmp_ok = 1;
/*decl*/
  double return_value;
#ifdef F2PY_REPORT_ATEXIT
f2py_cb_start_clock();
#endif
  CFUNCSMESS("cb:Call-back function cb_kernel_func_in_convolve__user__routines (maxnofargs=1(-0))\n");
  CFUNCSMESSPY("cb:cb_kernel_func_in_convolve__user__routines_capi=",cb_kernel_func_in_convolve__user__routines_capi);
  if (cb_kernel_func_in_convolve__user__routines_capi==NULL) {
    capi_longjmp_ok = 0;
    cb_kernel_func_in_convolve__user__routines_capi = PyObject_GetAttrString(convolve_module,"kernel_func");
  }
  if (cb_kernel_func_in_convolve__user__routines_capi==NULL) {
    PyErr_SetString(convolve_error,"cb: Callback kernel_func not defined (as an argument or module convolve attribute).\n");
    goto capi_fail;
  }
  if (F2PyCapsule_Check(cb_kernel_func_in_convolve__user__routines_capi)) {
  cb_kernel_func_in_convolve__user__routines_typedef cb_kernel_func_in_convolve__user__routines_cptr;
  cb_kernel_func_in_convolve__user__routines_cptr = F2PyCapsule_AsVoidPtr(cb_kernel_func_in_convolve__user__routines_capi);
  return_value=(*cb_kernel_func_in_convolve__user__routines_cptr)(k);
    return return_value;
  }
  if (capi_arglist==NULL) {
    capi_longjmp_ok = 0;
    capi_tmp = PyObject_GetAttrString(convolve_module,"kernel_func_extra_args");
    if (capi_tmp) {
      capi_arglist = (PyTupleObject *)PySequence_Tuple(capi_tmp);
      if (capi_arglist==NULL) {
        PyErr_SetString(convolve_error,"Failed to convert convolve.kernel_func_extra_args to tuple.\n");
        goto capi_fail;
      }
    } else {
      PyErr_Clear();
      capi_arglist = (PyTupleObject *)Py_BuildValue("()");
    }
  }
  if (capi_arglist == NULL) {
    PyErr_SetString(convolve_error,"Callback kernel_func argument list is not set.\n");
    goto capi_fail;
  }
/*setdims*/
/*pyobjfrom*/
  if (cb_kernel_func_in_convolve__user__routines_nofargs>capi_i)
    if (PyTuple_SetItem((PyObject *)capi_arglist,capi_i++,pyobj_from_int1(k)))
      goto capi_fail;
  CFUNCSMESSPY("cb:capi_arglist=",capi_arglist);
  CFUNCSMESS("cb:Call-back calling Python function kernel_func.\n");
#ifdef F2PY_REPORT_ATEXIT
f2py_cb_start_call_clock();
#endif
  capi_return = PyObject_CallObject(cb_kernel_func_in_convolve__user__routines_capi,(PyObject *)capi_arglist);
#ifdef F2PY_REPORT_ATEXIT
f2py_cb_stop_call_clock();
#endif
  CFUNCSMESSPY("cb:capi_return=",capi_return);
  if (capi_return == NULL) {
    fprintf(stderr,"capi_return is NULL\n");
    goto capi_fail;
  }
  if (capi_return == Py_None) {
    Py_DECREF(capi_return);
    capi_return = Py_BuildValue("()");
  }
  else if (!PyTuple_Check(capi_return)) {
    capi_return = Py_BuildValue("(N)",capi_return);
  }
  capi_j = PyTuple_Size(capi_return);
  capi_i = 0;
/*frompyobj*/
  if (capi_j>capi_i)
    GETSCALARFROMPYTUPLE(capi_return,capi_i++,&return_value,double,"double_from_pyobj failed in converting return_value of call-back function cb_kernel_func_in_convolve__user__routines to C double\n");
  CFUNCSMESS("cb:cb_kernel_func_in_convolve__user__routines:successful\n");
  Py_DECREF(capi_return);
#ifdef F2PY_REPORT_ATEXIT
f2py_cb_stop_clock();
#endif
  goto capi_return_pt;
capi_fail:
  fprintf(stderr,"Call-back cb_kernel_func_in_convolve__user__routines failed.\n");
  Py_XDECREF(capi_return);
  if (capi_longjmp_ok)
    longjmp(cb_kernel_func_in_convolve__user__routines_jmpbuf,-1);
capi_return_pt:
  ;
  return return_value;
}
/************* end of cb_kernel_func_in_convolve__user__routines *************/


/*********************** See f2py2e/rules.py: buildapi ***********************/

/************************** init_convolution_kernel **************************/
static char doc_f2py_rout_convolve_init_convolution_kernel[] = "\
omega = init_convolution_kernel(n,kernel_func,[d,zero_nyquist,kernel_func_extra_args])\n\nWrapper for ``init_convolution_kernel``.\
\n\nParameters\n----------\n"
"n : input int\n"
"kernel_func : call-back function\n"
"\nOther Parameters\n----------------\n"
"d : input int, optional\n    Default: 0\n"
"kernel_func_extra_args : input tuple, optional\n    Default: ()\n"
"zero_nyquist : input int, optional\n    Default: d%2\n"
"\nReturns\n-------\n"
"omega : rank-1 array('d') with bounds (n)\n"
"\nNotes\n-----\nCall-back functions::\n\n"
"  def kernel_func(k): return kernel_func\n\
  Required arguments:\n"
"    k : input int\n"
"  Return objects:\n"
"    kernel_func : float";
/* extern void init_convolution_kernel(int,double*,int,cb_kernel_func_in_convolve__user__routines_typedef,int); */
static PyObject *f2py_rout_convolve_init_convolution_kernel(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int,double*,int,cb_kernel_func_in_convolve__user__routines_typedef,int)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  int n = 0;
  PyObject *n_capi = Py_None;
  double *omega = NULL;
  npy_intp omega_Dims[1] = {-1};
  const int omega_Rank = 1;
  PyArrayObject *capi_omega_tmp = NULL;
  int capi_omega_intent = 0;
  int d = 0;
  PyObject *d_capi = Py_None;
  PyObject *kernel_func_capi = Py_None;
  PyTupleObject *kernel_func_xa_capi = NULL;
  PyTupleObject *kernel_func_args_capi = NULL;
  int kernel_func_nofargs_capi = 0;
  cb_kernel_func_in_convolve__user__routines_typedef kernel_func_cptr;
  int zero_nyquist = 0;
  PyObject *zero_nyquist_capi = Py_None;
  static char *capi_kwlist[] = {"n","kernel_func","d","zero_nyquist","kernel_func_extra_args",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    "OO|OOO!:convolve.init_convolution_kernel",\
    capi_kwlist,&n_capi,&kernel_func_capi,&d_capi,&zero_nyquist_capi,&PyTuple_Type,&kernel_func_xa_capi))
    return NULL;
/*frompyobj*/
  /* Processing variable kernel_func */
if(F2PyCapsule_Check(kernel_func_capi)) {
  kernel_func_cptr = F2PyCapsule_AsVoidPtr(kernel_func_capi);
} else {
  kernel_func_cptr = cb_kernel_func_in_convolve__user__routines;
}

  kernel_func_nofargs_capi = cb_kernel_func_in_convolve__user__routines_nofargs;
  if (create_cb_arglist(kernel_func_capi,kernel_func_xa_capi,1,0,&cb_kernel_func_in_convolve__user__routines_nofargs,&kernel_func_args_capi,"failed in processing argument list for call-back kernel_func.")) {
    jmp_buf kernel_func_jmpbuf;
    CFUNCSMESS("Saving jmpbuf for `kernel_func`.\n");
    SWAP(kernel_func_capi,cb_kernel_func_in_convolve__user__routines_capi,PyObject);
    SWAP(kernel_func_args_capi,cb_kernel_func_in_convolve__user__routines_args_capi,PyTupleObject);
    memcpy(&kernel_func_jmpbuf,&cb_kernel_func_in_convolve__user__routines_jmpbuf,sizeof(jmp_buf));
  /* Processing variable d */
  if (d_capi == Py_None) d = 0; else
    f2py_success = int_from_pyobj(&d,d_capi,"convolve.init_convolution_kernel() 1st keyword (d) can't be converted to int");
  if (f2py_success) {
  /* Processing variable n */
    f2py_success = int_from_pyobj(&n,n_capi,"convolve.init_convolution_kernel() 1st argument (n) can't be converted to int");
  if (f2py_success) {
  CHECKSCALAR(n>0,"n>0","1st argument n","init_convolution_kernel:n=%d",n) {
  /* Processing variable zero_nyquist */
  if (zero_nyquist_capi == Py_None) zero_nyquist = d%2; else
    f2py_success = int_from_pyobj(&zero_nyquist,zero_nyquist_capi,"convolve.init_convolution_kernel() 2nd keyword (zero_nyquist) can't be converted to int");
  if (f2py_success) {
  /* Processing variable omega */
  omega_Dims[0]=n;
  capi_omega_intent |= F2PY_INTENT_C|F2PY_INTENT_OUT|F2PY_INTENT_HIDE;
  capi_omega_tmp = array_from_pyobj(NPY_DOUBLE,omega_Dims,omega_Rank,capi_omega_intent,Py_None);
  if (capi_omega_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting hidden `omega' of convolve.init_convolution_kernel to C/Fortran array" );
  } else {
    omega = (double *)(PyArray_DATA(capi_omega_tmp));

/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
    if ((setjmp(cb_kernel_func_in_convolve__user__routines_jmpbuf))) {
      f2py_success = 0;
    } else {
        (*f2py_func)(n,omega,d,kernel_func_cptr,zero_nyquist);
    }
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("N",capi_omega_tmp);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
  }  /*if (capi_omega_tmp == NULL) ... else of omega*/
  /* End of cleaning variable omega */
  } /*if (f2py_success) of zero_nyquist*/
  /* End of cleaning variable zero_nyquist */
  } /*CHECKSCALAR(n>0)*/
  } /*if (f2py_success) of n*/
  /* End of cleaning variable n */
  } /*if (f2py_success) of d*/
  /* End of cleaning variable d */
    CFUNCSMESS("Restoring jmpbuf for `kernel_func`.\n");
    cb_kernel_func_in_convolve__user__routines_capi = kernel_func_capi;
    Py_DECREF(cb_kernel_func_in_convolve__user__routines_args_capi);
    cb_kernel_func_in_convolve__user__routines_args_capi = kernel_func_args_capi;
    cb_kernel_func_in_convolve__user__routines_nofargs = kernel_func_nofargs_capi;
    memcpy(&cb_kernel_func_in_convolve__user__routines_jmpbuf,&kernel_func_jmpbuf,sizeof(jmp_buf));
  }
  /* End of cleaning variable kernel_func */
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/*********************** end of init_convolution_kernel ***********************/

/*************************** destroy_convolve_cache ***************************/
static char doc_f2py_rout_convolve_destroy_convolve_cache[] = "\
destroy_convolve_cache()\n\nWrapper for ``destroy_convolve_cache``.\
\n";
/* extern void destroy_convolve_cache(void); */
static PyObject *f2py_rout_convolve_destroy_convolve_cache(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(void)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  static char *capi_kwlist[] = {NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    ":convolve.destroy_convolve_cache",\
    capi_kwlist))
    return NULL;
/*frompyobj*/
/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
        (*f2py_func)();
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("");
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/*********************** end of destroy_convolve_cache ***********************/

/********************************** convolve **********************************/
static char doc_f2py_rout_convolve_convolve[] = "\
y = convolve(x,omega,[swap_real_imag,overwrite_x])\n\nWrapper for ``convolve``.\
\n\nParameters\n----------\n"
"x : input rank-1 array('d') with bounds (n)\n"
"omega : input rank-1 array('d') with bounds (n)\n"
"\nOther Parameters\n----------------\n"
"overwrite_x : input int, optional\n    Default: 0\n"
"swap_real_imag : input int, optional\n    Default: 0\n"
"\nReturns\n-------\n"
"y : rank-1 array('d') with bounds (n) and x storage";
/* extern void convolve(int,double*,double*,int); */
static PyObject *f2py_rout_convolve_convolve(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int,double*,double*,int)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  int n = 0;
  double *x = NULL;
  npy_intp x_Dims[1] = {-1};
  const int x_Rank = 1;
  PyArrayObject *capi_x_tmp = NULL;
  int capi_x_intent = 0;
  int capi_overwrite_x = 0;
  PyObject *x_capi = Py_None;
  double *omega = NULL;
  npy_intp omega_Dims[1] = {-1};
  const int omega_Rank = 1;
  PyArrayObject *capi_omega_tmp = NULL;
  int capi_omega_intent = 0;
  PyObject *omega_capi = Py_None;
  int swap_real_imag = 0;
  PyObject *swap_real_imag_capi = Py_None;
  static char *capi_kwlist[] = {"x","omega","swap_real_imag","overwrite_x",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    "OO|Oi:convolve.convolve",\
    capi_kwlist,&x_capi,&omega_capi,&swap_real_imag_capi,&capi_overwrite_x))
    return NULL;
/*frompyobj*/
  /* Processing variable x */
  capi_x_intent |= (capi_overwrite_x?0:F2PY_INTENT_COPY);
  ;
  capi_x_intent |= F2PY_INTENT_C|F2PY_INTENT_IN|F2PY_INTENT_OUT;
  capi_x_tmp = array_from_pyobj(NPY_DOUBLE,x_Dims,x_Rank,capi_x_intent,x_capi);
  if (capi_x_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting 1st argument `x' of convolve.convolve to C/Fortran array" );
  } else {
    x = (double *)(PyArray_DATA(capi_x_tmp));

  /* Processing variable swap_real_imag */
  if (swap_real_imag_capi == Py_None) swap_real_imag = 0; else
    f2py_success = int_from_pyobj(&swap_real_imag,swap_real_imag_capi,"convolve.convolve() 1st keyword (swap_real_imag) can't be converted to int");
  if (f2py_success) {
  /* Processing variable n */
  n = len(x);
  /* Processing variable omega */
  omega_Dims[0]=n;
  capi_omega_intent |= F2PY_INTENT_C|F2PY_INTENT_CACHE|F2PY_INTENT_IN;
  capi_omega_tmp = array_from_pyobj(NPY_DOUBLE,omega_Dims,omega_Rank,capi_omega_intent,omega_capi);
  if (capi_omega_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting 2nd argument `omega' of convolve.convolve to C/Fortran array" );
  } else {
    omega = (double *)(PyArray_DATA(capi_omega_tmp));

/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
        (*f2py_func)(n,x,omega,swap_real_imag);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("N",capi_x_tmp);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
  if((PyObject *)capi_omega_tmp!=omega_capi) {
    Py_XDECREF(capi_omega_tmp); }
  }  /*if (capi_omega_tmp == NULL) ... else of omega*/
  /* End of cleaning variable omega */
  /* End of cleaning variable n */
  } /*if (f2py_success) of swap_real_imag*/
  /* End of cleaning variable swap_real_imag */
  }  /*if (capi_x_tmp == NULL) ... else of x*/
  /* End of cleaning variable x */
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/****************************** end of convolve ******************************/

/********************************* convolve_z *********************************/
static char doc_f2py_rout_convolve_convolve_z[] = "\
y = convolve_z(x,omega_real,omega_imag,[overwrite_x])\n\nWrapper for ``convolve_z``.\
\n\nParameters\n----------\n"
"x : input rank-1 array('d') with bounds (n)\n"
"omega_real : input rank-1 array('d') with bounds (n)\n"
"omega_imag : input rank-1 array('d') with bounds (n)\n"
"\nOther Parameters\n----------------\n"
"overwrite_x : input int, optional\n    Default: 0\n"
"\nReturns\n-------\n"
"y : rank-1 array('d') with bounds (n) and x storage";
/* extern void convolve_z(int,double*,double*,double*); */
static PyObject *f2py_rout_convolve_convolve_z(const PyObject *capi_self,
                           PyObject *capi_args,
                           PyObject *capi_keywds,
                           void (*f2py_func)(int,double*,double*,double*)) {
  PyObject * volatile capi_buildvalue = NULL;
  volatile int f2py_success = 1;
/*decl*/

  int n = 0;
  double *x = NULL;
  npy_intp x_Dims[1] = {-1};
  const int x_Rank = 1;
  PyArrayObject *capi_x_tmp = NULL;
  int capi_x_intent = 0;
  int capi_overwrite_x = 0;
  PyObject *x_capi = Py_None;
  double *omega_real = NULL;
  npy_intp omega_real_Dims[1] = {-1};
  const int omega_real_Rank = 1;
  PyArrayObject *capi_omega_real_tmp = NULL;
  int capi_omega_real_intent = 0;
  PyObject *omega_real_capi = Py_None;
  double *omega_imag = NULL;
  npy_intp omega_imag_Dims[1] = {-1};
  const int omega_imag_Rank = 1;
  PyArrayObject *capi_omega_imag_tmp = NULL;
  int capi_omega_imag_intent = 0;
  PyObject *omega_imag_capi = Py_None;
  static char *capi_kwlist[] = {"x","omega_real","omega_imag","overwrite_x",NULL};

/*routdebugenter*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_clock();
#endif
  if (!PyArg_ParseTupleAndKeywords(capi_args,capi_keywds,\
    "OOO|i:convolve.convolve_z",\
    capi_kwlist,&x_capi,&omega_real_capi,&omega_imag_capi,&capi_overwrite_x))
    return NULL;
/*frompyobj*/
  /* Processing variable x */
  capi_x_intent |= (capi_overwrite_x?0:F2PY_INTENT_COPY);
  ;
  capi_x_intent |= F2PY_INTENT_C|F2PY_INTENT_IN|F2PY_INTENT_OUT;
  capi_x_tmp = array_from_pyobj(NPY_DOUBLE,x_Dims,x_Rank,capi_x_intent,x_capi);
  if (capi_x_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting 1st argument `x' of convolve.convolve_z to C/Fortran array" );
  } else {
    x = (double *)(PyArray_DATA(capi_x_tmp));

  /* Processing variable n */
  n = len(x);
  /* Processing variable omega_imag */
  omega_imag_Dims[0]=n;
  capi_omega_imag_intent |= F2PY_INTENT_C|F2PY_INTENT_CACHE|F2PY_INTENT_IN;
  capi_omega_imag_tmp = array_from_pyobj(NPY_DOUBLE,omega_imag_Dims,omega_imag_Rank,capi_omega_imag_intent,omega_imag_capi);
  if (capi_omega_imag_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting 3rd argument `omega_imag' of convolve.convolve_z to C/Fortran array" );
  } else {
    omega_imag = (double *)(PyArray_DATA(capi_omega_imag_tmp));

  /* Processing variable omega_real */
  omega_real_Dims[0]=n;
  capi_omega_real_intent |= F2PY_INTENT_C|F2PY_INTENT_CACHE|F2PY_INTENT_IN;
  capi_omega_real_tmp = array_from_pyobj(NPY_DOUBLE,omega_real_Dims,omega_real_Rank,capi_omega_real_intent,omega_real_capi);
  if (capi_omega_real_tmp == NULL) {
    if (!PyErr_Occurred())
      PyErr_SetString(convolve_error,"failed in converting 2nd argument `omega_real' of convolve.convolve_z to C/Fortran array" );
  } else {
    omega_real = (double *)(PyArray_DATA(capi_omega_real_tmp));

/*end of frompyobj*/
#ifdef F2PY_REPORT_ATEXIT
f2py_start_call_clock();
#endif
/*callfortranroutine*/
        (*f2py_func)(n,x,omega_real,omega_imag);
if (PyErr_Occurred())
  f2py_success = 0;
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_call_clock();
#endif
/*end of callfortranroutine*/
    if (f2py_success) {
/*pyobjfrom*/
/*end of pyobjfrom*/
    CFUNCSMESS("Building return value.\n");
    capi_buildvalue = Py_BuildValue("N",capi_x_tmp);
/*closepyobjfrom*/
/*end of closepyobjfrom*/
    } /*if (f2py_success) after callfortranroutine*/
/*cleanupfrompyobj*/
  if((PyObject *)capi_omega_real_tmp!=omega_real_capi) {
    Py_XDECREF(capi_omega_real_tmp); }
  }  /*if (capi_omega_real_tmp == NULL) ... else of omega_real*/
  /* End of cleaning variable omega_real */
  if((PyObject *)capi_omega_imag_tmp!=omega_imag_capi) {
    Py_XDECREF(capi_omega_imag_tmp); }
  }  /*if (capi_omega_imag_tmp == NULL) ... else of omega_imag*/
  /* End of cleaning variable omega_imag */
  /* End of cleaning variable n */
  }  /*if (capi_x_tmp == NULL) ... else of x*/
  /* End of cleaning variable x */
/*end of cleanupfrompyobj*/
  if (capi_buildvalue == NULL) {
/*routdebugfailure*/
  } else {
/*routdebugleave*/
  }
  CFUNCSMESS("Freeing memory.\n");
/*freemem*/
#ifdef F2PY_REPORT_ATEXIT
f2py_stop_clock();
#endif
  return capi_buildvalue;
}
/***************************** end of convolve_z *****************************/
/*eof body*/

/******************* See f2py2e/f90mod_rules.py: buildhooks *******************/
/*need_f90modhooks*/

/************** See f2py2e/rules.py: module_rules['modulebody'] **************/

/******************* See f2py2e/common_rules.py: buildhooks *******************/

/*need_commonhooks*/

/**************************** See f2py2e/rules.py ****************************/

static FortranDataDef f2py_routine_defs[] = {
  {"init_convolution_kernel",-1,{{-1}},0,(char *)init_convolution_kernel,(f2py_init_func)f2py_rout_convolve_init_convolution_kernel,doc_f2py_rout_convolve_init_convolution_kernel},
  {"destroy_convolve_cache",-1,{{-1}},0,(char *)destroy_convolve_cache,(f2py_init_func)f2py_rout_convolve_destroy_convolve_cache,doc_f2py_rout_convolve_destroy_convolve_cache},
  {"convolve",-1,{{-1}},0,(char *)convolve,(f2py_init_func)f2py_rout_convolve_convolve,doc_f2py_rout_convolve_convolve},
  {"convolve_z",-1,{{-1}},0,(char *)convolve_z,(f2py_init_func)f2py_rout_convolve_convolve_z,doc_f2py_rout_convolve_convolve_z},

/*eof routine_defs*/
  {NULL}
};

static PyMethodDef f2py_module_methods[] = {

  {NULL,NULL}
};

#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef moduledef = {
  PyModuleDef_HEAD_INIT,
  "convolve",
  NULL,
  -1,
  f2py_module_methods,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif

#if PY_VERSION_HEX >= 0x03000000
#define RETVAL m
PyMODINIT_FUNC PyInit_convolve(void) {
#else
#define RETVAL
PyMODINIT_FUNC initconvolve(void) {
#endif
  int i;
  PyObject *m,*d, *s;
#if PY_VERSION_HEX >= 0x03000000
  m = convolve_module = PyModule_Create(&moduledef);
#else
  m = convolve_module = Py_InitModule("convolve", f2py_module_methods);
#endif
  Py_TYPE(&PyFortran_Type) = &PyType_Type;
  import_array();
  if (PyErr_Occurred())
    {PyErr_SetString(PyExc_ImportError, "can't initialize module convolve (failed to import numpy)"); return RETVAL;}
  d = PyModule_GetDict(m);
  s = PyString_FromString("$Revision: $");
  PyDict_SetItemString(d, "__version__", s);
#if PY_VERSION_HEX >= 0x03000000
  s = PyUnicode_FromString(
#else
  s = PyString_FromString(
#endif
    "This module 'convolve' is auto-generated with f2py (version:2).\nFunctions:\n"
"  omega = init_convolution_kernel(n,kernel_func,d=0,zero_nyquist=d%2,kernel_func_extra_args=())\n"
"  destroy_convolve_cache()\n"
"  y = convolve(x,omega,swap_real_imag=0,overwrite_x=0)\n"
"  y = convolve_z(x,omega_real,omega_imag,overwrite_x=0)\n"
".");
  PyDict_SetItemString(d, "__doc__", s);
  convolve_error = PyErr_NewException ("convolve.error", NULL, NULL);
  Py_DECREF(s);
  for(i=0;f2py_routine_defs[i].name!=NULL;i++)
    PyDict_SetItemString(d, f2py_routine_defs[i].name,PyFortranObject_NewAsAttr(&f2py_routine_defs[i]));




/*eof initf2pywraphooks*/
/*eof initf90modhooks*/

/*eof initcommonhooks*/


#ifdef F2PY_REPORT_ATEXIT
  if (! PyErr_Occurred())
    on_exit(f2py_report_on_exit,(void*)"convolve");
#endif

  return RETVAL;
}
#ifdef __cplusplus
}
#endif
