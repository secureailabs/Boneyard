cdef extern from "_complexstuff.h":
    # numpy/npy_math.h doesn't have correct extern "C" declarations,
    # so we must include a wrapped version first
    pass

cdef extern from "numpy/npy_math.h":
    double NPY_NAN

cimport numpy as np
from numpy cimport (
    npy_float, npy_double, npy_longdouble,
    npy_cfloat, npy_cdouble, npy_clongdouble,
    npy_int, npy_long,
    NPY_FLOAT, NPY_DOUBLE, NPY_LONGDOUBLE,
    NPY_CFLOAT, NPY_CDOUBLE, NPY_CLONGDOUBLE,
    NPY_INT, NPY_LONG)

ctypedef double complex double_complex

cdef extern from "numpy/ufuncobject.h":
    int PyUFunc_getfperr() nogil

cdef public int wrap_PyUFunc_getfperr_ufunc() nogil:
    """
    Call PyUFunc_getfperr in a context where PyUFunc_API array is initialized;
    this avoids messing with the UNIQUE_SYMBOL #defines
    """
    return PyUFunc_getfperr()

cimport libc

from . cimport sf_error

np.import_array()
np.import_ufunc()

cdef void _set_action(sf_error.sf_error_t code,
                      sf_error.sf_action_t action) nogil:
    sf_error.set_action(code, action)
