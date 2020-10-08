#ifndef Py_PYTHON_Hq
#define Py_PYTHON_H
//--------------------------------by Jingwei
//--------------------------------by Jingwei
#include "./enclave_wrapper/io.h"
#include "./enclave_wrapper/string_sgx.h"
#include "./enclave_wrapper/signal.h"
#include "./enclave_wrapper/sys.h"
//------------------------------------------


/* Since this is a "meta-include" file, no #ifdef __cplusplus / extern "C" { */

/* Include nearly all Python header files */

#include "patchlevel.h"
#include "pyconfig.h"
#include "pymacconfig.h"

/* Cyclic gc is always enabled, starting with release 2.3a1.  Supply the
 * old symbol for the benefit of extension modules written before then
 * that may be conditionalizing on it.  The core doesn't use it anymore.
 */
#ifndef WITH_CYCLE_GC
#define WITH_CYCLE_GC 1
#endif

#include <limits.h>

#ifndef UCHAR_MAX
#error "Something's broken.  UCHAR_MAX should be defined in limits.h."
#endif

#if UCHAR_MAX != 255
#error "Python's source code assumes C's unsigned char is an 8-bit type."
#endif

#if defined(__sgi) && defined(WITH_THREAD) && !defined(_SGI_MP_SOURCE)
#define _SGI_MP_SOURCE
#endif

#include <stdio.h>
#ifndef NULL
#   error "Python.h requires that stdio.h define NULL."
#endif

#include <string.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif

/* For size_t? */
#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

/* CAUTION:  Build setups should ensure that NDEBUG is defined on the
 * compiler command line when building Python in release mode; else
 * assert() calls won't be removed.
 */
#include <assert.h>

#include "pyport.h"

/* pyconfig.h or pyport.h may or may not define DL_IMPORT */
#ifndef DL_IMPORT	/* declarations for DLL import/export */
#define DL_IMPORT(RTYPE) RTYPE
#endif
#ifndef DL_EXPORT	/* declarations for DLL import/export */
#define DL_EXPORT(RTYPE) RTYPE
#endif

/* Debug-mode build with pymalloc implies PYMALLOC_DEBUG.
 *  PYMALLOC_DEBUG is in error if pymalloc is not in use.
 */
#if defined(Py_DEBUG) && defined(WITH_PYMALLOC) && !defined(PYMALLOC_DEBUG)
#define PYMALLOC_DEBUG
#endif
#if defined(PYMALLOC_DEBUG) && !defined(WITH_PYMALLOC)
#error "PYMALLOC_DEBUG requires WITH_PYMALLOC"
#endif
#include "pymath.h"
#include "pymem.h"

#include "object.h"
#include "objimpl.h"

#include "pydebug.h"

#include "unicodeobject.h"
#include "intobject.h"
#include "boolobject.h"
#include "longobject.h"
#include "floatobject.h"
#ifndef WITHOUT_COMPLEX
#include "complexobject.h"
#endif
#include "rangeobject.h"
#include "stringobject.h"
#include "memoryobject.h"
#include "bufferobject.h"
#include "bytesobject.h"
#include "bytearrayobject.h"
#include "tupleobject.h"
#include "listobject.h"
#include "dictobject.h"
#include "enumobject.h"
#include "setobject.h"
#include "methodobject.h"
#include "moduleobject.h"
#include "funcobject.h"
#include "classobject.h"
#include "fileobject.h"
#include "cobject.h"
#include "pycapsule.h"
#include "traceback.h"
#include "sliceobject.h"
#include "cellobject.h"
#include "iterobject.h"
#include "genobject.h"
#include "descrobject.h"
#include "warnings.h"
#include "weakrefobject.h"

#include "codecs.h"
#include "pyerrors.h"

#include "pystate.h"

#include "pyarena.h"
#include "modsupport.h"
#include "pythonrun.h"
#include "ceval.h"
#include "sysmodule.h"
#include "intrcheck.h"
#include "import.h"

#include "abstract.h"

#include "compile.h"
#include "eval.h"

#include "pyctype.h"
#include "pystrtod.h"
#include "pystrcmp.h"
#include "dtoa.h"

#ifdef __cplusplus
extern "C"{
#endif
/* _Py_Mangle is defined in compile.c */
PyAPI_FUNC(PyObject*) _Py_Mangle(PyObject *p, PyObject *name);
#ifdef __cplusplus
}
#endif

/* PyArg_GetInt is deprecated and should not be used, use PyArg_Parse(). */
#define PyArg_GetInt(v, a)	PyArg_Parse((v), "i", (a))

/* PyArg_NoArgs should not be necessary.
   Set ml_flags in the PyMethodDef to METH_NOARGS. */
#define PyArg_NoArgs(v)		PyArg_Parse(v, "")

/* Argument must be a char or an int in [-128, 127] or [0, 255]. */
#define Py_CHARMASK(c)		((unsigned char)((c) & 0xff))

#include "pyfpe.h"

/* These definitions must match corresponding definitions in graminit.h.
   There's code in compile.c that checks that they are the same. */
#define Py_single_input 256
#define Py_file_input 257
#define Py_eval_input 258

#ifdef HAVE_PTH
/* GNU pth user-space thread support */
#include <pth.h>
#endif

/* Define macros for inline documentation. */
#define PyDoc_VAR(name) static char name[]
#define PyDoc_STRVAR(name,str) PyDoc_VAR(name) = PyDoc_STR(str)
#ifdef WITH_DOC_STRINGS
#define PyDoc_STR(str) str
#else
#define PyDoc_STR(str) ""
#endif

#ifdef __cplusplus
extern "C"{
#endif

//statically initialized functions
PyMODINIT_FUNC initerrno(void);
PyMODINIT_FUNC initsignal(void);
PyMODINIT_FUNC initposix(void);
PyMODINIT_FUNC init_functools(void);
PyMODINIT_FUNC initoperator(void);
PyMODINIT_FUNC init_weakref_(void);
PyMODINIT_FUNC init_collections(void);
PyMODINIT_FUNC inititertools(void);
PyMODINIT_FUNC init_heapq(void);
PyMODINIT_FUNC initimp(void);
PyMODINIT_FUNC init_sre(void);
PyMODINIT_FUNC initcPickle(void);
PyMODINIT_FUNC initcStringIO(void);
PyMODINIT_FUNC initmath(void);
PyMODINIT_FUNC init_io(void);
PyMODINIT_FUNC initfuture_builtins(void);
PyMODINIT_FUNC init_codecs(void);
PyMODINIT_FUNC inittime(void);
PyMODINIT_FUNC init_struct(void);
PyMODINIT_FUNC init_ctypes(void);
PyMODINIT_FUNC initbinascii(void);
PyMODINIT_FUNC init_random(void);
PyMODINIT_FUNC init_md5(void);
PyMODINIT_FUNC init_sha256(void);
PyMODINIT_FUNC init_sha512(void);
PyMODINIT_FUNC init_sha(void);
PyMODINIT_FUNC init_csv(void);
PyMODINIT_FUNC initdatetime(void);
PyMODINIT_FUNC initunicodedata(void);
PyMODINIT_FUNC initarray(void);
PyMODINIT_FUNC init_zlib(void);
PyMODINIT_FUNC initselect(void);
PyMODINIT_FUNC init_federated(void);
PyMODINIT_FUNC init_httpsend(void);

//from numpy package
PyMODINIT_FUNC init_multiarray_umath(void);
PyMODINIT_FUNC initmtrand(void);
PyMODINIT_FUNC init_umath_linalg(void);
PyMODINIT_FUNC initlapack_lite(void);
PyMODINIT_FUNC initfftpack_lite(void);

//from scipy package
//----from .sparse
PyMODINIT_FUNC init_csparsetools(void);
PyMODINIT_FUNC init_arpack(void);
PyMODINIT_FUNC init_iterative(void);
PyMODINIT_FUNC init_superlu(void);
PyMODINIT_FUNC init_traversal(void);
PyMODINIT_FUNC init_reordering(void);
PyMODINIT_FUNC init_min_spanning_tree(void);
PyMODINIT_FUNC init_shortest_path(void);
PyMODINIT_FUNC init_tools(void);
PyMODINIT_FUNC init_sparsetools(void);
//----from .optimize
PyMODINIT_FUNC init_cobyla(void);
PyMODINIT_FUNC initmoduleTNC(void);
PyMODINIT_FUNC init_lbfgsb(void);
PyMODINIT_FUNC init_minpack(void);
PyMODINIT_FUNC init_zeros(void);
PyMODINIT_FUNC initminpack2(void);
PyMODINIT_FUNC init_nnls(void);
PyMODINIT_FUNC init_slsqp(void);
PyMODINIT_FUNC init_group_columns(void);
PyMODINIT_FUNC initgivens_elimination(void);
PyMODINIT_FUNC init_trlib(void);
//----from .special
PyMODINIT_FUNC init_comb(void);
PyMODINIT_FUNC initcython_special(void);
PyMODINIT_FUNC init_ellip_harm_2(void);
PyMODINIT_FUNC initspecfun(void);
PyMODINIT_FUNC init_ufuncs(void);
PyMODINIT_FUNC init_ufuncs_cxx(void);
//----from ._lib
PyMODINIT_FUNC init_ccallback_c(void);
PyMODINIT_FUNC init_fpumode(void);
PyMODINIT_FUNC initmessagestream(void);
//----from .stats
PyMODINIT_FUNC init_stats(void);
PyMODINIT_FUNC initstatlib(void);
PyMODINIT_FUNC initmvn(void);
//----from .integrate
PyMODINIT_FUNC initvode(void);
PyMODINIT_FUNC initlsoda(void);
PyMODINIT_FUNC init_dop(void);
PyMODINIT_FUNC init_odepack(void);
PyMODINIT_FUNC init_quadpack(void);
//----from .linalg
PyMODINIT_FUNC init_flapack(void);
PyMODINIT_FUNC init_interpolative(void);
PyMODINIT_FUNC init_solve_toeplitz(void);
PyMODINIT_FUNC initcython_lapack(void);
PyMODINIT_FUNC init_decomp_update(void);
PyMODINIT_FUNC initcython_blas(void);
PyMODINIT_FUNC init_fblas(void);
PyMODINIT_FUNC init_flinalg(void);
//----from .spatial
PyMODINIT_FUNC initqhull(void);
PyMODINIT_FUNC initckdtree(void);
PyMODINIT_FUNC initckdtree(void);
PyMODINIT_FUNC init_voronoi(void);
PyMODINIT_FUNC init_hausdorff(void);
PyMODINIT_FUNC init_distance_wrap(void);
//---from .signal
PyMODINIT_FUNC initsigtools(void);
PyMODINIT_FUNC init_spectral(void);
PyMODINIT_FUNC init_max_len_seq_inner(void);
PyMODINIT_FUNC init_peak_finding_utils(void);
PyMODINIT_FUNC init_upfirdn_apply(void);
PyMODINIT_FUNC initspline(void);
//----from .ndimage
PyMODINIT_FUNC init_nd_image(void);
PyMODINIT_FUNC init_ni_label(void);
//----from .odr
PyMODINIT_FUNC init__odrpack(void);
//----from .interpolate
PyMODINIT_FUNC initinterpnd(void);
PyMODINIT_FUNC init_ppoly(void);
PyMODINIT_FUNC init_bspl(void);
PyMODINIT_FUNC init_fitpack(void);
PyMODINIT_FUNC initdfitpack(void);
PyMODINIT_FUNC init_interpolate(void);
//----from .fftpack
PyMODINIT_FUNC initconvolve(void);
PyMODINIT_FUNC init_fftpack(void);
//----from .cluster
PyMODINIT_FUNC init_vq(void);
PyMODINIT_FUNC init_hierarchy(void);
PyMODINIT_FUNC init_optimal_leaf_ordering(void);
//finish of scipy

//from scikit-learn
//from .
PyMODINIT_FUNC init_isotonic(void);
//from .util
PyMODINIT_FUNC init_logistic_sigmoid(void);
PyMODINIT_FUNC initarrayfuncs(void);
PyMODINIT_FUNC initgraph_shortest_path(void);
PyMODINIT_FUNC initlgamma(void);
PyMODINIT_FUNC initmurmurhash(void);
PyMODINIT_FUNC initseq_dataset(void);
PyMODINIT_FUNC initsparsefuncs_fast(void);
PyMODINIT_FUNC initweight_vector(void);
PyMODINIT_FUNC init_sk_random(void);
PyMODINIT_FUNC initfast_dict(void);
//from .svm
PyMODINIT_FUNC initlibsvm(void);
PyMODINIT_FUNC initlibsvm_sparse(void);
PyMODINIT_FUNC initliblinear(void);
//from .tree
PyMODINIT_FUNC init_criterion(void);
PyMODINIT_FUNC init_splitter(void);
PyMODINIT_FUNC init_tree(void);
PyMODINIT_FUNC inittree_utils(void);
//from .cluster
PyMODINIT_FUNC init_hierarchical(void);
PyMODINIT_FUNC init_dbscan_inner(void);
PyMODINIT_FUNC init_k_means(void);
PyMODINIT_FUNC init_k_means_elkan(void);
//from .linear_model
PyMODINIT_FUNC initsag_fast(void);
PyMODINIT_FUNC initsgd_fast(void);
PyMODINIT_FUNC initcd_fast(void);
//from .check_build
PyMODINIT_FUNC init_check_build(void);
//from .metrics
PyMODINIT_FUNC initpairwise_fast(void);
PyMODINIT_FUNC initexpected_mutual_info_fast(void);
//from .ensemble
PyMODINIT_FUNC init_gradient_boosting(void);
//from .manifold
PyMODINIT_FUNC init_barnes_hut_tsne(void);
PyMODINIT_FUNC init_mani_utils(void);
//from .dataset
PyMODINIT_FUNC init_svmlight_format(void);
//from .feature_extraction
PyMODINIT_FUNC init_hashing(void);
//from .neighbors
PyMODINIT_FUNC initball_tree(void);
PyMODINIT_FUNC initkd_tree(void);
PyMODINIT_FUNC initquad_tree(void);
PyMODINIT_FUNC inittypedefs(void);
PyMODINIT_FUNC initdist_metrics(void);
//from .decomposition
PyMODINIT_FUNC init_online_lda(void);
PyMODINIT_FUNC initcdnmf_fast(void);
//finish of scikit-learn

//from pandas
//from ._lib
PyMODINIT_FUNC inithashtable(void);
PyMODINIT_FUNC initindexing(void);
PyMODINIT_FUNC initinternals(void);
PyMODINIT_FUNC initsparse(void);
PyMODINIT_FUNC initalgos(void);
PyMODINIT_FUNC initreshape(void);
PyMODINIT_FUNC inittslib(void);
PyMODINIT_FUNC initindex(void);
PyMODINIT_FUNC initjoin(void);
PyMODINIT_FUNC initskiplist(void);
PyMODINIT_FUNC initinterval(void);
PyMODINIT_FUNC initwindow(void);
PyMODINIT_FUNC initproperties(void);
PyMODINIT_FUNC initlib(void);
PyMODINIT_FUNC initparsers(void);
PyMODINIT_FUNC initwriters(void);
PyMODINIT_FUNC inithashing(void);
PyMODINIT_FUNC initjson(void);
PyMODINIT_FUNC initops(void);
PyMODINIT_FUNC initmissing(void);
PyMODINIT_FUNC initgroupby(void);
PyMODINIT_FUNC initreduction(void);
//from .lib.tslib
PyMODINIT_FUNC initstrptime(void);
PyMODINIT_FUNC inittimedeltas(void);
PyMODINIT_FUNC initparsing(void);
PyMODINIT_FUNC initresolution(void);
PyMODINIT_FUNC inittimezones(void);
PyMODINIT_FUNC initccalendar(void);
PyMODINIT_FUNC initperiod(void);
PyMODINIT_FUNC initoffsets(void);
PyMODINIT_FUNC inittimestamps(void);
PyMODINIT_FUNC initnp_datetime(void);
PyMODINIT_FUNC initfields(void);
PyMODINIT_FUNC initfrequencies(void);
PyMODINIT_FUNC initconversion(void);
PyMODINIT_FUNC initnattype(void);

//from .io.sas
PyMODINIT_FUNC init_sas(void);
//from .util
PyMODINIT_FUNC init_move(void);

//for pyvcf
PyMODINIT_FUNC initcparse(void);

//----from matplotlib
PyMODINIT_FUNC init_backend_agg(void);
PyMODINIT_FUNC init_contour(void);
PyMODINIT_FUNC init_image(void);
PyMODINIT_FUNC init_path(void);
PyMODINIT_FUNC initttconv(void);
PyMODINIT_FUNC init_png(void);
PyMODINIT_FUNC initft2font(void);
PyMODINIT_FUNC initkiwisolver( void );

#ifdef __cplusplus
}
#endif

#endif /* !Py_PYTHON_H */

