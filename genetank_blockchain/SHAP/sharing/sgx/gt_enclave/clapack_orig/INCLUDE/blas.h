#ifndef BLAS_H
#define BLAS_H

#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif
/* caxpy.c */
int f2c_caxpy(integer *n, complex *ca, complex *cx, integer *incx, complex *cy, integer *incy);
/* ccopy.c */
int f2c_ccopy(integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cdotc.c */
void f2c_cdotc(complex *ret_val, integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cdotu.c */
void f2c_cdotu(complex *ret_val, integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* cgbmv.c */
int f2c_cgbmv(char *trans, integer *m, integer *n, integer *kl, integer *ku, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy);
/* cgemm.c */
int f2c_cgemm(char *transa, char *transb, integer *m, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc);
/* cgemv.c */
int f2c_cgemv(char *trans, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy);
/* cgerc.c */
int f2c_cgerc(integer *m, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda);
/* cgeru.c */
int f2c_cgeru(integer *m, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda);
/* chbmv.c */
int f2c_chbmv(char *uplo, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy);
/* chemm.c */
int f2c_chemm(char *side, char *uplo, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc);
/* chemv.c */
int f2c_chemv(char *uplo, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy);
/* cher2.c */
int f2c_cher2(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *a, integer *lda);
/* cher2k.c */
int f2c_cher2k(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, real *beta, complex *c__, integer *ldc);
/* cher.c */
int f2c_cher(char *uplo, integer *n, real *alpha, complex *x, integer *incx, complex *a, integer *lda);
/* cherk.c */
int f2c_cherk(char *uplo, char *trans, integer *n, integer *k, real *alpha, complex *a, integer *lda, real *beta, complex *c__, integer *ldc);
/* chpmv.c */
int f2c_chpmv(char *uplo, integer *n, complex *alpha, complex *ap, complex *x, integer *incx, complex *beta, complex *y, integer *incy);
/* chpr2.c */
int f2c_chpr2(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *y, integer *incy, complex *ap);
/* chpr.c */
int f2c_chpr(char *uplo, integer *n, real *alpha, complex *x, integer *incx, complex *ap);
/* crotg.c */
int f2c_crotg(complex *ca, complex *cb, real *c__, complex *s);
/* cscal.c */
int f2c_cscal(integer *n, complex *ca, complex *cx, integer *incx);
/* csrot.c */
int csrot_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy, real *c__, real *s);
/* csscal.c */
int f2c_csscal(integer *n, real *sa, complex *cx, integer *incx);
/* cswap.c */
int f2c_cswap(integer *n, complex *cx, integer *incx, complex *cy, integer *incy);
/* csymm.c */
int f2c_csymm(char *side, char *uplo, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc);
/* csyr2k.c */
int f2c_csyr2k(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb, complex *beta, complex *c__, integer *ldc);
/* csyrk.c */
int f2c_csyrk(char *uplo, char *trans, integer *n, integer *k, complex *alpha, complex *a, integer *lda, complex *beta, complex *c__, integer *ldc);
/* ctbmv.c */
int f2c_ctbmv(char *uplo, char *trans, char *diag, integer *n, integer *k, complex *a, integer *lda, complex *x, integer *incx);
/* ctbsv.c */
int f2c_ctbsv(char *uplo, char *trans, char *diag, integer *n, integer *k, complex *a, integer *lda, complex *x, integer *incx);
/* ctpmv.c */
int f2c_ctpmv(char *uplo, char *trans, char *diag, integer *n, complex *ap, complex *x, integer *incx);
/* ctpsv.c */
int f2c_ctpsv(char *uplo, char *trans, char *diag, integer *n, complex *ap, complex *x, integer *incx);
/* ctrmm.c */
int f2c_ctrmm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb);
/* ctrmv.c */
int f2c_ctrmv(char *uplo, char *trans, char *diag, integer *n, complex *a, integer *lda, complex *x, integer *incx);
/* ctrsm.c */
int f2c_ctrsm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, complex *alpha, complex *a, integer *lda, complex *b, integer *ldb);
/* ctrsv.c */
int f2c_ctrsv(char *uplo, char *trans, char *diag, integer *n, complex *a, integer *lda, complex *x, integer *incx);
/* dasum.c */
doublereal f2c_dasum(integer *n, doublereal *dx, integer *incx);
/* daxpy.c */
int f2c_daxpy(integer *n, doublereal *da, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dcabs1.c */
doublereal dcabs1_(doublecomplex *z__);
/* dcopy.c */
int f2c_dcopy(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* ddot.c */
doublereal f2c_ddot(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dgbmv.c */
int f2c_dgbmv(char *trans, integer *m, integer *n, integer *kl, integer *ku, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dgemm.c */
int f2c_dgemm(char *transa, char *transb, integer *m, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc);
/* dgemv.c */
int f2c_dgemv(char *trans, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dger.c */
int f2c_dger(integer *m, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *a, integer *lda);
/* dnrm2.c */
doublereal f2c_dnrm2(integer *n, doublereal *x, integer *incx);
/* drot.c */
int f2c_drot(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy, doublereal *c__, doublereal *s);
/* drotg.c */
int f2c_drotg(doublereal *da, doublereal *db, doublereal *c__, doublereal *s);
/* drotm.c */
int f2c_drotm(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy, doublereal *dparam);
/* drotmg.c */
int f2c_drotmg(doublereal *dd1, doublereal *dd2, doublereal *dx1, doublereal *dy1, doublereal *dparam);
/* dsbmv.c */
int f2c_dsbmv(char *uplo, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dscal.c */
int f2c_dscal(integer *n, doublereal *da, doublereal *dx, integer *incx);
/* dsdot.c */
doublereal dsdot_(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* dspmv.c */
int f2c_dspmv(char *uplo, integer *n, doublereal *alpha, doublereal *ap, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dspr2.c */
int f2c_dspr2(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *ap);
/* dspr.c */
int f2c_dspr(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *ap);
/* dswap.c */
int f2c_dswap(integer *n, doublereal *dx, integer *incx, doublereal *dy, integer *incy);
/* dsymm.c */
int f2c_dsymm(char *side, char *uplo, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc);
/* dsymv.c */
int f2c_dsymv(char *uplo, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dsyr2.c */
int f2c_dsyr2(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *a, integer *lda);
/* dsyr2k.c */
int f2c_dsyr2k(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc);
/* dsyr.c */
int f2c_dsyr(char *uplo, integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *a, integer *lda);
/* dsyrk.c */
int f2c_dsyrk(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *beta, doublereal *c__, integer *ldc);
/* dtbmv.c */
int f2c_dtbmv(char *uplo, char *trans, char *diag, integer *n, integer *k, doublereal *a, integer *lda, doublereal *x, integer *incx);
/* dtbsv.c */
int f2c_dtbsv(char *uplo, char *trans, char *diag, integer *n, integer *k, doublereal *a, integer *lda, doublereal *x, integer *incx);
/* dtpmv.c */
int f2c_dtpmv(char *uplo, char *trans, char *diag, integer *n, doublereal *ap, doublereal *x, integer *incx);
/* dtpsv.c */
int f2c_dtpsv(char *uplo, char *trans, char *diag, integer *n, doublereal *ap, doublereal *x, integer *incx);
/* dtrmm.c */
int f2c_dtrmm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb);
/* dtrmv.c */
int f2c_dtrmv(char *uplo, char *trans, char *diag, integer *n, doublereal *a, integer *lda, doublereal *x, integer *incx);
/* dtrsm.c */
int f2c_dtrsm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *b, integer *ldb);
/* dtrsv.c */
int f2c_dtrsv(char *uplo, char *trans, char *diag, integer *n, doublereal *a, integer *lda, doublereal *x, integer *incx);
/* dzasum.c */
doublereal f2c_dzasum(integer *n, doublecomplex *zx, integer *incx);
/* dznrm2.c */
doublereal f2c_dznrm2(integer *n, doublecomplex *x, integer *incx);
/* icamax.c */
integer f2c_icamax(integer *n, complex *cx, integer *incx);
/* idamax.c */
integer f2c_idamax(integer *n, doublereal *dx, integer *incx);
/* isamax.c */
integer f2c_isamax(integer *n, real *sx, integer *incx);
/* izamax.c */
integer f2c_izamax(integer *n, doublecomplex *zx, integer *incx);
/* lsame.c */
logical lsame_(char *ca, char *cb);
/* sasum.c */
doublereal f2c_sasum(integer *n, real *sx, integer *incx);
/* saxpy.c */
int f2c_saxpy(integer *n, real *sa, real *sx, integer *incx, real *sy, integer *incy);
/* scabs1.c */
doublereal scabs1_(complex *z__);
/* scasum.c */
doublereal f2c_scasum(integer *n, complex *cx, integer *incx);
/* scnrm2.c */
doublereal f2c_scnrm2(integer *n, complex *x, integer *incx);
/* scopy.c */
int f2c_scopy(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* sdot.c */
doublereal f2c_sdot(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* sdsdot.c */
doublereal sdsdot_(integer *n, real *sb, real *sx, integer *incx, real *sy, integer *incy);
/* sgbmv.c */
int f2c_sgbmv(char *trans, integer *m, integer *n, integer *kl, integer *ku, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sgemm.c */
int f2c_sgemm(char *transa, char *transb, integer *m, integer *n, integer *k, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc);
/* sgemv.c */
int f2c_sgemv(char *trans, integer *m, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sger.c */
int f2c_sger(integer *m, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *a, integer *lda);
/* snrm2.c */
doublereal f2c_snrm2(integer *n, real *x, integer *incx);
/* srot.c */
int f2c_srot(integer *n, real *sx, integer *incx, real *sy, integer *incy, real *c__, real *s);
/* srotg.c */
int f2c_srotg(real *sa, real *sb, real *c__, real *s);
/* srotm.c */
int f2c_srotm(integer *n, real *sx, integer *incx, real *sy, integer *incy, real *sparam);
/* srotmg.c */
int f2c_srotmg(real *sd1, real *sd2, real *sx1, real *sy1, real *sparam);
/* ssbmv.c */
int f2c_ssbmv(char *uplo, integer *n, integer *k, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sscal.c */
int f2c_sscal(integer *n, real *sa, real *sx, integer *incx);
/* sspmv.c */
int f2c_sspmv(char *uplo, integer *n, real *alpha, real *ap, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sspr2.c */
int f2c_sspr2(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *ap);
/* sspr.c */
int f2c_sspr(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *ap);
/* sswap.c */
int f2c_sswap(integer *n, real *sx, integer *incx, real *sy, integer *incy);
/* ssymm.c */
int f2c_ssymm(char *side, char *uplo, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc);
/* ssymv.c */
int f2c_ssymv(char *uplo, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* ssyr2.c */
int f2c_ssyr2(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *y, integer *incy, real *a, integer *lda);
/* ssyr2k.c */
int f2c_ssyr2k(char *uplo, char *trans, integer *n, integer *k, real *alpha, real *a, integer *lda, real *b, integer *ldb, real *beta, real *c__, integer *ldc);
/* ssyr.c */
int f2c_ssyr(char *uplo, integer *n, real *alpha, real *x, integer *incx, real *a, integer *lda);
/* ssyrk.c */
int f2c_ssyrk(char *uplo, char *trans, integer *n, integer *k, real *alpha, real *a, integer *lda, real *beta, real *c__, integer *ldc);
/* stbmv.c */
int f2c_stbmv(char *uplo, char *trans, char *diag, integer *n, integer *k, real *a, integer *lda, real *x, integer *incx);
/* stbsv.c */
int f2c_stbsv(char *uplo, char *trans, char *diag, integer *n, integer *k, real *a, integer *lda, real *x, integer *incx);
/* stpmv.c */
int f2c_stpmv(char *uplo, char *trans, char *diag, integer *n, real *ap, real *x, integer *incx);
/* stpsv.c */
int f2c_stpsv(char *uplo, char *trans, char *diag, integer *n, real *ap, real *x, integer *incx);
/* strmm.c */
int f2c_strmm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb);
/* strmv.c */
int f2c_strmv(char *uplo, char *trans, char *diag, integer *n, real *a, integer *lda, real *x, integer *incx);
/* strsm.c */
int f2c_strsm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, real *alpha, real *a, integer *lda, real *b, integer *ldb);
/* strsv.c */
int f2c_strsv(char *uplo, char *trans, char *diag, integer *n, real *a, integer *lda, real *x, integer *incx);
/* xerbla_array.c */
int xerbla_array__(char *srname_array__, integer *srname_len__, integer *info, ftnlen srname_array_len);
/* xerbla.c */
int xerbla_(char *srname, integer *info);
/* zaxpy.c */
int f2c_zaxpy(integer *n, doublecomplex *za, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zcopy.c */
int f2c_zcopy(integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdotc.c */
void f2c_zdotc(doublecomplex *ret_val, integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdotu.c */
void f2c_zdotu(doublecomplex *ret_val, integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zdrot.c */
int zdrot_(integer *n, doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy, doublereal *c__, doublereal *s);
/* zdscal.c */
int f2c_zdscal(integer *n, doublereal *da, doublecomplex *zx, integer *incx);
/* zgbmv.c */
int f2c_zgbmv(char *trans, integer *m, integer *n, integer *kl, integer *ku, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy);
/* zgemm.c */
int f2c_zgemm(char *transa, char *transb, integer *m, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc);
/* zgemv.c */
int f2c_zgemv(char *trans, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy);
/* zgerc.c */
int f2c_zgerc(integer *m, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda);
/* zgeru.c */
int f2c_zgeru(integer *m, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda);
/* zhbmv.c */
int f2c_zhbmv(char *uplo, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy);
/* zhemm.c */
int f2c_zhemm(char *side, char *uplo, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc);
/* zhemv.c */
int f2c_zhemv(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy);
/* zher2.c */
int f2c_zher2(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *a, integer *lda);
/* zher2k.c */
int f2c_zher2k(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *beta, doublecomplex *c__, integer *ldc);
/* zher.c */
int f2c_zher(char *uplo, integer *n, doublereal *alpha, doublecomplex *x, integer *incx, doublecomplex *a, integer *lda);
/* zherk.c */
int f2c_zherk(char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublecomplex *a, integer *lda, doublereal *beta, doublecomplex *c__, integer *ldc);
/* zhpmv.c */
int f2c_zhpmv(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *ap, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy);
/* zhpr2.c */
int f2c_zhpr2(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublecomplex *ap);
/* zhpr.c */
int f2c_zhpr(char *uplo, integer *n, doublereal *alpha, doublecomplex *x, integer *incx, doublecomplex *ap);
/* zrotg.c */
int f2c_zrotg(doublecomplex *ca, doublecomplex *cb, doublereal *c__, doublecomplex *s);
/* zscal.c */
int f2c_zscal(integer *n, doublecomplex *za, doublecomplex *zx, integer *incx);
/* zswap.c */
int f2c_zswap(integer *n, doublecomplex *zx, integer *incx, doublecomplex *zy, integer *incy);
/* zsymm.c */
int f2c_zsymm(char *side, char *uplo, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc);
/* zsyr2k.c */
int f2c_zsyr2k(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *beta, doublecomplex *c__, integer *ldc);
/* zsyrk.c */
int f2c_zsyrk(char *uplo, char *trans, integer *n, integer *k, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *beta, doublecomplex *c__, integer *ldc);
/* ztbmv.c */
int f2c_ztbmv(char *uplo, char *trans, char *diag, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx);
/* ztbsv.c */
int f2c_ztbsv(char *uplo, char *trans, char *diag, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx);
/* ztpmv.c */
int f2c_ztpmv(char *uplo, char *trans, char *diag, integer *n, doublecomplex *ap, doublecomplex *x, integer *incx);
/* ztpsv.c */
int f2c_ztpsv(char *uplo, char *trans, char *diag, integer *n, doublecomplex *ap, doublecomplex *x, integer *incx);
/* ztrmm.c */
int f2c_ztrmm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb);
/* ztrmv.c */
int f2c_ztrmv(char *uplo, char *trans, char *diag, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx);
/* ztrsm.c */
int f2c_ztrsm(char *side, char *uplo, char *transa, char *diag, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb);
/* ztrsv.c */
int f2c_ztrsv(char *uplo, char *trans, char *diag, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx);

#ifdef __cplusplus
}
#endif

#endif
