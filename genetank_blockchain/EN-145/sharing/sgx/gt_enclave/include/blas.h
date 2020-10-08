#include "f2c.h"

#ifdef __cplusplus
extern "C"{
#endif

/* caxpy.c */
int caxpy_(integer *n, complex *ca, complex *cx, integer *incx, complex *cy, integer *incy);
/* ccopy.c */
int ccopy_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cdotc.c */
void cdotc_(complex *ret_val, integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cdotu.c */
void cdotu_(complex *ret_val, integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cgbmv.c */
int cgbmv_(char *trans, integer *m, integer *n, integer *kl, integer *ku, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen trans_len);
/* cgemm.c */
int cgemm_(char *transa, char *transb, integer *m, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc, ftnlen transa_len, ftnlen transb_len);
/* cgemv.c */
int cgemv_(char *trans, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen trans_len);
/* cgerc.c */
int cgerc_(integer *m, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda);
/* cgeru.c */
int cgeru_(integer *m, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda);
/* chbmv.c */
int chbmv_(char *uplo, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen uplo_len);
/* chemm.c */
int chemm_(char *side, char *uplo, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* chemv.c */
int chemv_(char *uplo, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen uplo_len);
/* cher2.c */
int cher2_(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda, ftnlen uplo_len);
/* cher2k.c */
int cher2k_(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, real *beta, complex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* cher.c */
int cher_(char *uplo, integer *n, real *alpha, complex *x, integer *incx, complex *a, integer *lda, ftnlen uplo_len);
/* cherk.c */
int cherk_(char *uplo, char *trans, integer *n, integer *k, real *alpha, complex *a, integer *lda, real *beta, complex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* chpmv.c */
int chpmv_(char *uplo, integer *n, complex *alpha, complex *ap, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen uplo_len);
/* chpr2.c */
int chpr2_(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *ap, ftnlen uplo_len);
/* chpr.c */
int chpr_(char *uplo, integer *n, real *alpha, complex *x, integer *incx, complex *ap, ftnlen uplo_len);
/* crotg.c */
int crotg_(complex *ca, complex *cb, real *c__, complex *s);
/* cscal.c */
int cscal_(integer *n, complex *ca, complex *cx, integer *incx);
/* csrot.c */
int csrot_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy, real *c__, real *s);
/* csscal.c */
int csscal_(integer *n, real *sa, complex *cx, integer *incx);
/* cswap.c */
int cswap_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* csymm.c */
int csymm_(char *side, char *uplo, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* csyr2k.c */
int csyr2k_(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* csyrk.c */
int csyrk_(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *beta, complex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* ctbmv.c */
int ctbmv_(char *uplo, char *trans, char *diag, integer *n, integer *k, complex *a, integer *lda, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctbsv.c */
int ctbsv_(char *uplo, char *trans, char *diag, integer *n, integer *k, complex *a, integer *lda, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctpmv.c */
int ctpmv_(char *uplo, char *trans, char *diag, integer *n, complex *ap, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctpsv.c */
int ctpsv_(char *uplo, char *trans, char *diag, integer *n, complex *ap, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctrmm.c */
int ctrmm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* ctrmv.c */
int ctrmv_(char *uplo, char *trans, char *diag, integer *n, complex *a, integer *lda, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctrsm.c */
int ctrsm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* ctrsv.c */
int ctrsv_(char *uplo, char *trans, char *diag, integer *n, complex *a, integer *lda, complex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dasum.c */
doublereal dasum_(integer *n, doublereal *dx, integer *incx);
/* daxpy.c */
int daxpy_(integer *n, doublereal *da, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dcabs1.c */
doublereal dcabs1_(doublecomplex *z__);
/* dcopy.c */
int dcopy_(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* ddot.c */
doublereal ddot_(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dgbmv.c */
int dgbmv_(char *trans, integer *m, integer *n, integer *kl, integer *ku, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy, ftnlen trans_len);
/* dgemm.c */
int dgemm_(char *transa, char *transb, integer *m, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc, ftnlen transa_len, ftnlen transb_len);
/* dgemv.c */
int dgemv_(char *trans, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy, ftnlen trans_len);
/* dger.c */
int dger_(integer *m, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *a, integer *lda);
/* dnrm2.c */
doublereal dnrm2_(integer *n, doublereal *x, integer *incx);
/* drot.c */
int drot_(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy, doublereal *c__, doublereal *s);
/* drotg.c */
int drotg_(doublereal *da, doublereal *db, doublereal *c__, doublereal *s);
/* drotm.c */
int drotm_(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy, doublereal *dparam);
/* drotmg.c */
int drotmg_(doublereal *dd1, doublereal *dd2, doublereal *dx1, doublereal *dy1, doublereal *dparam);
/* dsbmv.c */
int dsbmv_(char *uplo, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy, ftnlen uplo_len);
/* dscal.c */
int dscal_(integer *n, doublereal *da, doublereal *dx, integer *incx);
/* dsdot.c */
doublereal dsdot_(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* dspmv.c */
int dspmv_(char *uplo, integer *n, doublereal *alpha, doublereal *ap, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy, ftnlen uplo_len);
/* dspr2.c */
int dspr2_(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *ap, ftnlen uplo_len);
/* dspr.c */
int dspr_(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *ap, ftnlen uplo_len);
/* dswap.c */
int dswap_(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dsymm.c */
int dsymm_(char *side, char *uplo, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* dsymv.c */
int dsymv_(char *uplo, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy, ftnlen uplo_len);
/* dsyr2.c */
int dsyr2_(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *a, integer *lda, ftnlen uplo_len);
/* dsyr2k.c */
int dsyr2k_(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* dsyr.c */
int dsyr_(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *a, integer *lda, ftnlen uplo_len);
/* dsyrk.c */
int dsyrk_(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *beta, doublereal *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* dtbmv.c */
int dtbmv_(char *uplo, char *trans, char *diag, integer *n, integer *k, doublereal *a, integer *lda, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtbsv.c */
int dtbsv_(char *uplo, char *trans, char *diag, integer *n, integer *k, doublereal *a, integer *lda, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtpmv.c */
int dtpmv_(char *uplo, char *trans, char *diag, integer *n, doublereal *ap, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtpsv.c */
int dtpsv_(char *uplo, char *trans, char *diag, integer *n, doublereal *ap, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtrmm.c */
int dtrmm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* dtrmv.c */
int dtrmv_(char *uplo, char *trans, char *diag, integer *n, doublereal *a, integer *lda, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtrsm.c */
int dtrsm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* dtrsv.c */
int dtrsv_(char *uplo, char *trans, char *diag, integer *n, doublereal *a, integer *lda, doublereal *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dzasum.c */
doublereal dzasum_(integer *n, doublecomplex *zx, integer *incx);
/* dznrm2.c */
doublereal dznrm2_(integer *n, doublecomplex *x, integer *incx);
/* icamax.c */
integer icamax_(integer *n, complex *cx, integer *incx);
/* idamax.c */
integer idamax_(integer *n, doublereal *dx, integer *incx);
/* isamax.c */
integer isamax_(integer *n, real *sx, integer *incx);
/* izamax.c */
integer izamax_(integer *n, doublecomplex *zx, integer *incx);
/* lsame.c */
logical lsame_(char *ca, char *cb, ftnlen ca_len, ftnlen cb_len);
/* sasum.c */
doublereal sasum_(integer *n, real *sx, integer *incx);
/* saxpy.c */
int saxpy_(integer *n, real *sa, real *sx, integer *incx, real *sy, integer *incy);
/* scabs1.c */
doublereal scabs1_(complex *z__);
/* scasum.c */
doublereal scasum_(integer *n, complex *cx, integer *incx);
/* scnrm2.c */
doublereal scnrm2_(integer *n, complex *x, integer *incx);
/* scopy.c */
int scopy_(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* sdot.c */
doublereal sdot_(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* sdsdot.c */
doublereal sdsdot_(integer *n, real *sb, real *sx, integer *incx, real *sy, integer *incy);
/* sgbmv.c */
int sgbmv_(char *trans, integer *m, integer *n, integer *kl, integer *ku, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy, ftnlen trans_len);
/* sgemm.c */
int sgemm_(char *transa, char *transb, integer *m, integer *n, integer *k, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc, ftnlen transa_len, ftnlen transb_len);
/* sgemv.c */
int sgemv_(char *trans, integer *m, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy, ftnlen trans_len);
/* sger.c */
int sger_(integer *m, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *a, integer *lda);
/* snrm2.c */
doublereal snrm2_(integer *n, real *x, integer *incx);
/* srot.c */
int srot_(integer *n, real *sx, integer *incx, real *sy, integer *incy, real *c__, real *s);
/* srotg.c */
int srotg_(real *sa, real *sb, real *c__, real *s);
/* srotm.c */
int srotm_(integer *n, real *sx, integer *incx, real *sy, integer *incy, real *sparam);
/* srotmg.c */
int srotmg_(real *sd1, real *sd2, real *sx1, real *sy1, real *sparam);
/* ssbmv.c */
int ssbmv_(char *uplo, integer *n, integer *k, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy, ftnlen uplo_len);
/* sscal.c */
int sscal_(integer *n, real *sa, real *sx, integer *incx);
/* sspmv.c */
int sspmv_(char *uplo, integer *n, real *alpha, real *ap, real *x, integer *incx, real *beta, real *y, integer *incy, ftnlen uplo_len);
/* sspr2.c */
int sspr2_(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *ap, ftnlen uplo_len);
/* sspr.c */
int sspr_(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *ap, ftnlen uplo_len);
/* sswap.c */
int sswap_(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* ssymm.c */
int ssymm_(char *side, char *uplo, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* ssymv.c */
int ssymv_(char *uplo, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy, ftnlen uplo_len);
/* ssyr2.c */
int ssyr2_(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *a, integer *lda, ftnlen uplo_len);
/* ssyr2k.c */
int ssyr2k_(char *uplo, char *trans, integer *n, integer *k, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* ssyr.c */
int ssyr_(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *a, integer *lda, ftnlen uplo_len);
/* ssyrk.c */
int ssyrk_(char *uplo, char *trans, integer *n, integer *k, real *alpha, real *a, integer *lda, real *beta, real *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* stbmv.c */
int stbmv_(char *uplo, char *trans, char *diag, integer *n, integer *k, real *a, integer *lda, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stbsv.c */
int stbsv_(char *uplo, char *trans, char *diag, integer *n, integer *k, real *a, integer *lda, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stpmv.c */
int stpmv_(char *uplo, char *trans, char *diag, integer *n, real *ap, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stpsv.c */
int stpsv_(char *uplo, char *trans, char *diag, integer *n, real *ap, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* strmm.c */
int strmm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* strmv.c */
int strmv_(char *uplo, char *trans, char *diag, integer *n, real *a, integer *lda, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* strsm.c */
int strsm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* strsv.c */
int strsv_(char *uplo, char *trans, char *diag, integer *n, real *a, integer *lda, real *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* zaxpy.c */
int zaxpy_(integer *n, doublecomplex *za, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zcopy.c */
int zcopy_(integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdotc.c */
void zdotc_(doublecomplex *ret_val, integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdotu.c */
void zdotu_(doublecomplex *ret_val, integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdrot.c */
int zdrot_(integer *n, doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy, doublereal *c__, doublereal *s);
/* zdscal.c */
int zdscal_(integer *n, doublereal *da, doublecomplex *zx, integer *incx);
/* zgbmv.c */
int zgbmv_(char *trans, integer *m, integer *n, integer *kl, integer *ku, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen trans_len);
/* zgemm.c */
int zgemm_(char *transa, char *transb, integer *m, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc, ftnlen transa_len, ftnlen transb_len);
/* zgemv.c */
int zgemv_(char *trans, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen trans_len);
/* zgerc.c */
int zgerc_(integer *m, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda);
/* zgeru.c */
int zgeru_(integer *m, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda);
/* zhbmv.c */
int zhbmv_(char *uplo, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen uplo_len);
/* zhemm.c */
int zhemm_(char *side, char *uplo, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* zhemv.c */
int zhemv_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen uplo_len);
/* zher2.c */
int zher2_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda, ftnlen uplo_len);
/* zher2k.c */
int zher2k_(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *beta, doublecomplex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* zher.c */
int zher_(char *uplo, integer *n, doublereal *alpha, doublecomplex *x, integer *incx, doublecomplex *a, integer *lda, ftnlen uplo_len);
/* zherk.c */
int zherk_(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublecomplex *a, integer *lda, doublereal *beta, doublecomplex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* zhpmv.c */
int zhpmv_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *ap, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen uplo_len);
/* zhpr2.c */
int zhpr2_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *ap, ftnlen uplo_len);
/* zhpr.c */
int zhpr_(char *uplo, integer *n, doublereal *alpha, doublecomplex *x, integer *incx, doublecomplex *ap, ftnlen uplo_len);
/* zrotg.c */
int zrotg_(doublecomplex *ca, doublecomplex *cb, doublereal *c__, doublecomplex *s);
/* zscal.c */
int zscal_(integer *n, doublecomplex *za, doublecomplex *zx, integer *incx);
/* zswap.c */
int zswap_(integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zsymm.c */
int zsymm_(char *side, char *uplo, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc, ftnlen side_len, ftnlen uplo_len);
/* zsyr2k.c */
int zsyr2k_(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* zsyrk.c */
int zsyrk_(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *beta, doublecomplex *c__, integer *ldc, ftnlen uplo_len, ftnlen trans_len);
/* ztbmv.c */
int ztbmv_(char *uplo, char *trans, char *diag, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztbsv.c */
int ztbsv_(char *uplo, char *trans, char *diag, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztpmv.c */
int ztpmv_(char *uplo, char *trans, char *diag, integer *n, doublecomplex *ap, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztpsv.c */
int ztpsv_(char *uplo, char *trans, char *diag, integer *n, doublecomplex *ap, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztrmm.c */
int ztrmm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* ztrmv.c */
int ztrmv_(char *uplo, char *trans, char *diag, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztrsm.c */
int ztrsm_(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, ftnlen side_len, ftnlen uplo_len, ftnlen transa_len, ftnlen diag_len);
/* ztrsv.c */
int ztrsv_(char *uplo, char *trans, char *diag, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);

#ifdef __cplusplus
}
#endif
