#include "./f2c.h"

#ifdef __cplusplus
extern "C"{
#endif
/* ilaver.c */
int ilaver_(integer *vers_major__, integer *vers_minor__, integer *vers_patch__);
/* cgegv.c */
int cgegv_(char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *alpha, complex *beta,complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* dgegv.c */
int dgegv_(char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* sgegv.c */
int sgegv_(char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* zgegv.c */
int zgegv_(char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* cbbcsd.c */
int cbbcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, integer *m, integer *p, integer *q, real *theta, real *phi, complex *u1, integer *ldu1, complex *u2, integer *ldu2, complex *v1t, integer *ldv1t, complex *v2t, integer *ldv2t, real *b11d, real *b11e, real *b12d, real *b12e, real *b21d, real *b21e, real *b22d, real *b22e, real *rwork, integer *lrwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len);
/* cbdsqr.c */
int cbdsqr_(char *uplo, integer *n, integer *ncvt, integer *nru, integer *ncc, real *d__, real *e, complex *vt, integer *ldvt, complex *u, integer *ldu, complex *c__, integer *ldc, real *rwork, integer *info, ftnlen uplo_len);
/* cgbbrd.c */
int cgbbrd_(char *vect, integer *m, integer *n, integer *ncc, integer *kl, integer *ku, complex *ab, integer *ldab, real *d__, real *e, complex *q, integer *ldq, complex *pt, integer *ldpt, complex *c__, integer *ldc, complex *work, real *rwork, integer *info, ftnlen vect_len);
/* cgbcon.c */
int cgbcon_(char *norm, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, integer *ipiv, real *anorm, real *rcond, complex *work, real *rwork, integer *info, ftnlen norm_len);
/* cgbequb.c */
int cgbequb_(integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* cgbequ.c */
int cgbequ_(integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* cgbrfs.c */
int cgbrfs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen trans_len);
/* cgbsv.c */
int cgbsv_(integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, integer *ipiv, complex *b, integer *ldb, integer *info);
/* cgbsvx.c */
int cgbsvx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, char *equed, real *r__, real *c__, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* cgbsvxx.c */
int cgbsvxx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, char *equed, real *r__, real *c__, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* cgbtf2.c */
int cgbtf2_(integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, integer *ipiv, integer *info);
/* cgbtrf.c */
int cgbtrf_(integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, integer *ipiv, integer *info);
/* cgbtrs.c */
int cgbtrs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen trans_len);
/* cgebak.c */
int cgebak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, real *scale, integer *m, complex *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* cgebal.c */
int cgebal_(char *job, integer *n, complex *a, integer *lda, integer *ilo, integer *ihi, real *scale, integer *info, ftnlen job_len);
/* cgebd2.c */
int cgebd2_(integer *m, integer *n, complex *a, integer *lda, real *d__, real *e, complex *tauq, complex *taup, complex *work, integer *info);
/* cgebrd.c */
int cgebrd_(integer *m, integer *n, complex *a, integer *lda, real *d__, real *e, complex *tauq, complex *taup, complex *work, integer *lwork, integer *info);
/* cgecon.c */
int cgecon_(char *norm, integer *n, complex *a, integer *lda, real *anorm, real *rcond, complex *work, real *rwork, integer *info, ftnlen norm_len);
/* cgeequb.c */
int cgeequb_(integer *m, integer *n, complex *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* cgeequ.c */
int cgeequ_(integer *m, integer *n, complex *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* cgees.c */
int cgees_(char *jobvs, char *sort, L_fp select, integer *n, complex *a, integer *lda, integer *sdim, complex *w, complex *vs, integer *ldvs, complex *work, integer *lwork, real *rwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len);
/* cgeesx.c */
int cgeesx_(char *jobvs, char *sort, L_fp select, char *sense, integer *n, complex *a, integer *lda, integer *sdim, complex *w, complex *vs, integer *ldvs, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len, ftnlen sense_len);
/* cgeev.c */
int cgeev_(char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* cgeevx.c */
int cgeevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, complex *a, integer *lda, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* cgehd2.c */
int cgehd2_(integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgehrd.c */
int cgehrd_(integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgelq2.c */
int cgelq2_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgelq.c */
int cgelq_(integer *m, integer *n, complex *a, integer *lda, complex *t, integer *tsize, complex *work, integer *lwork, integer *info);
/* cgelqf.c */
int cgelqf_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgelqt.c */
int cgelqt_(integer *m, integer *n, integer *mb, complex *a, integer *lda, complex *t, integer *ldt, complex *work, integer *info);
/* cgels.c */
int cgels_(char *trans, integer *m, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen trans_len);
/* cgelsd.c */
int cgelsd_(integer *m, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, real *s, real *rcond, integer *rank, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info);
/* cgelss.c */
int cgelss_(integer *m, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, real *s, real *rcond, integer *rank, complex *work, integer *lwork, real *rwork, integer *info);
/* cgelsy.c */
int cgelsy_(integer *m, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, integer *jpvt, real *rcond, integer *rank, complex *work, integer *lwork, real *rwork, integer *info);
/* cgemlq.c */
int cgemlq_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *t, integer *tsize, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cgemlqt.c */
int cgemlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, complex *v, integer *ldv, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cgemqr.c */
int cgemqr_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *t, integer *tsize, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cgemqrt.c */
int cgemqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *nb, complex *v, integer *ldv, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cgeql2.c */
int cgeql2_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgeqlf.c */
int cgeqlf_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgeqp3.c */
int cgeqp3_(integer *m, integer *n, complex *a, integer *lda, integer *jpvt, complex *tau, complex *work, integer *lwork, real *rwork, integer *info);
/* cgeqr2.c */
int cgeqr2_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgeqr2p.c */
int cgeqr2p_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgeqr.c */
int cgeqr_(integer *m, integer *n, complex *a, integer *lda, complex *t, integer *tsize, complex *work, integer *lwork, integer *info);
/* cgeqrf.c */
int cgeqrf_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgeqrfp.c */
int cgeqrfp_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgeqrt2.c */
int cgeqrt2_(integer *m, integer *n, complex *a, integer *lda, complex *t, integer *ldt, integer *info);
/* cgeqrt3.c */
int cgeqrt3_(integer *m, integer *n, complex *a, integer *lda, complex *t, integer *ldt, integer *info);
/* cgeqrt.c */
int cgeqrt_(integer *m, integer *n, integer *nb, complex *a, integer *lda, complex *t, integer *ldt, complex *work, integer *info);
/* cgerfs.c */
int cgerfs_(char *trans, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen trans_len);
/* cgerq2.c */
int cgerq2_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cgerqf.c */
int cgerqf_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cgesc2.c */
int cgesc2_(integer *n, complex *a, integer *lda, complex *rhs, integer *ipiv, integer *jpiv, real *scale);
/* cgesdd.c */
int cgesdd_(char *jobz, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info, ftnlen jobz_len);
/* cgesv.c */
int cgesv_(integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info);
/* cgesvd.c */
int cgesvd_(char *jobu, char *jobvt, integer *m, integer *n, complex *a, integer *lda, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len);
/* cgesvdx.c */
int cgesvdx_(char *jobu, char *jobvt, char *range, integer *m, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, integer *ns, real *s, complex *u, integer *ldu, complex *vt, integer *ldvt, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len, ftnlen range_len);
/* cgesvj.c */
int cgesvj_(char *joba, char *jobu, char *jobv, integer *m, integer *n, complex *a, integer *lda, real *sva, integer *mv, complex *v, integer *ldv, complex *cwork, integer *lwork, real *rwork, integer *lrwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len);
/* cgesvx.c */
int cgesvx_(char *fact, char *trans, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, char *equed, real *r__, real *c__, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* cgesvxx.c */
int cgesvxx_(char *fact, char *trans, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, char *equed, real *r__, real *c__, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* cgetc2.c */
int cgetc2_(integer *n, complex *a, integer *lda, integer *ipiv, integer *jpiv, integer *info);
/* cgetf2.c */
int cgetf2_(integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info);
/* cgetrf2.c */
int cgetrf2_(integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info);
/* cgetrf.c */
int cgetrf_(integer *m, integer *n, complex *a, integer *lda, integer *ipiv, integer *info);
/* cgetri.c */
int cgetri_(integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info);
/* cgetrs.c */
int cgetrs_(char *trans, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen trans_len);
/* cgetsls.c */
int cgetsls_(char *trans, integer *m, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen trans_len);
/* cggbak.c */
int cggbak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, real *lscale, real *rscale, integer *m, complex *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* cggbal.c */
int cggbal_(char *job, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *ilo, integer *ihi, real *lscale, real *rscale, real *work, integer *info, ftnlen job_len);
/* cgges3.c */
int cgges3_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *sdim, complex *alpha, complex *beta, complex *vsl, integer *ldvsl, complex *vsr, integer *ldvsr, complex *work, integer *lwork, real *rwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* cgges.c */
int cgges_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *sdim, complex *alpha, complex *beta, complex *vsl, integer *ldvsl, complex *vsr, integer *ldvsr, complex *work, integer *lwork, real *rwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* cggesx.c */
int cggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, char *sense, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *sdim, complex *alpha, complex *beta, complex *vsl, integer *ldvsl, complex *vsr, integer *ldvsr, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len, ftnlen sense_len);
/* cggev3.c */
int cggev3_(char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *alpha, complex *beta, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* cggev.c */
int cggev_(char *jobvl, char *jobvr, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *alpha, complex *beta, complex *vl, integer *ldvl, complex *vr, integer *ldvr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* cggevx.c */
int cggevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *alpha, complex *beta, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *ilo, integer *ihi, real *lscale, real *rscale, real *abnrm, real *bbnrm, real *rconde, real *rcondv, complex *work, integer *lwork, real *rwork, integer *iwork, logical *bwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* cggglm.c */
int cggglm_(integer *n, integer *m, integer *p, complex *a, integer *lda, complex *b, integer *ldb, complex *d__, complex *x, complex *y, complex *work, integer *lwork, integer *info);
/* cgghd3.c */
int cgghd3_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *b, integer *ldb, complex *q, integer *ldq, complex *z__, integer *ldz, complex *work, integer *lwork, integer *info, ftnlen compq_len, ftnlen compz_len);
/* cgghrd.c */
int cgghrd_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *b, integer *ldb, complex *q, integer *ldq, complex *z__, integer *ldz, integer *info, ftnlen compq_len, ftnlen compz_len);
/* cgglse.c */
int cgglse_(integer *m, integer *n, integer *p, complex *a, integer *lda, complex *b, integer *ldb, complex *c__, complex *d__, complex *x, complex *work, integer *lwork, integer *info);
/* cggqrf.c */
int cggqrf_(integer *n, integer *m, integer *p, complex *a, integer *lda, complex *taua, complex *b, integer *ldb, complex *taub, complex *work, integer *lwork, integer *info);
/* cggrqf.c */
int cggrqf_(integer *m, integer *p, integer *n, complex *a, integer *lda, complex *taua, complex *b, integer *ldb, complex *taub, complex *work, integer *lwork, integer *info);
/* cggsvd3.c */
int cggsvd3_(char *jobu, char *jobv, char *jobq, integer *m, integer *n, integer *p, integer *k, integer *l, complex *a, integer *lda, complex *b, integer *ldb, real *alpha, real *beta, complex *u, integer *ldu, complex *v, integer *ldv, complex *q, integer *ldq, complex *work, integer *lwork, real *rwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* cggsvp3.c */
int cggsvp3_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, complex *a, integer *lda, complex *b, integer *ldb, real *tola, real *tolb, integer *k, integer *l, complex *u, integer *ldu, complex *v, integer *ldv, complex *q, integer *ldq, integer *iwork, real *rwork, complex *tau, complex *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* cgsvj0.c */
int cgsvj0_(char *jobv, integer *m, integer *n, complex *a, integer *lda, complex *d__, real *sva, integer *mv, complex *v, integer *ldv, real *eps, real *sfmin, real *tol, integer *nsweep, complex *work, integer *lwork, integer *info, ftnlen jobv_len);
/* cgsvj1.c */
int cgsvj1_(char *jobv, integer *m, integer *n, integer *n1, complex *a, integer *lda, complex *d__, real *sva, integer *mv, complex *v, integer *ldv, real *eps, real *sfmin, real *tol, integer *nsweep, complex *work, integer *lwork, integer *info, ftnlen jobv_len);
/* cgtcon.c */
int cgtcon_(char *norm, integer *n, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen norm_len);
/* cgtrfs.c */
int cgtrfs_(char *trans, integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *dlf, complex *df, complex *duf, complex *du2, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen trans_len);
/* cgtsv.c */
int cgtsv_(integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *b, integer *ldb, integer *info);
/* cgtsvx.c */
int cgtsvx_(char *fact, char *trans, integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *dlf, complex *df, complex *duf, complex *du2, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen trans_len);
/* cgttrf.c */
int cgttrf_(integer *n, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, integer *info);
/* cgttrs.c */
int cgttrs_(char *trans, integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen trans_len);
/* cgtts2.c */
int cgtts2_(integer *itrans, integer *n, integer *nrhs, complex *dl, complex *d__, complex *du, complex *du2, integer *ipiv, complex *b, integer *ldb);
/* chb2st_kernels.c */
int chb2st_kernels__(char *uplo, logical *wantz, integer *ttype, integer *st, integer *ed, integer *sweep, integer *n, integer *nb, integer *ib, complex *a, integer *lda, complex *v, complex *tau, integer *ldvt, complex *work, ftnlen uplo_len);
/* chbev_2stage.c */
int chbev_2stage__(char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbev.c */
int chbev_(char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbevd_2stage.c */
int chbevd_2stage__(char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbevd.c */
int chbevd_(char *jobz, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbevx_2stage.c */
int chbevx_2stage__(char *jobz, char *range, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, complex *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chbevx.c */
int chbevx_(char *jobz, char *range, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, complex *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chbgst.c */
int chbgst_(char *vect, char *uplo, integer *n, integer *ka, integer *kb, complex *ab, integer *ldab, complex *bb, integer *ldbb, complex *x, integer *ldx, complex *work, real *rwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* chbgv.c */
int chbgv_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, complex *ab, integer *ldab, complex *bb, integer *ldbb, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbgvd.c */
int chbgvd_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, complex *ab, integer *ldab, complex *bb, integer *ldbb, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chbgvx.c */
int chbgvx_(char *jobz, char *range, char *uplo, integer *n, integer *ka, integer *kb, complex *ab, integer *ldab, complex *bb, integer *ldbb, complex *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chbtrd.c */
int chbtrd_(char *vect, char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *d__, real *e, complex *q, integer *ldq, complex *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* checon_3.c */
int checon_3__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* checon.c */
int checon_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* checon_rook.c */
int checon_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* cheequb.c */
int cheequb_(char *uplo, integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, complex *work, integer *info, ftnlen uplo_len);
/* cheev_2stage.c */
int cheev_2stage__(char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* cheev.c */
int cheev_(char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* cheevd_2stage.c */
int cheevd_2stage__(char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* cheevd.c */
int cheevd_(char *jobz, char *uplo, integer *n, complex *a, integer *lda, real *w, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* cheevr_2stage.c */
int cheevr_2stage__(char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, integer *isuppz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* cheevr.c */
int cheevr_(char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, integer *isuppz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* cheevx_2stage.c */
int cheevx_2stage__(char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* cheevx.c */
int cheevx_(char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chegs2.c */
int chegs2_(integer *itype, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chegst.c */
int chegst_(integer *itype, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chegv_2stage.c */
int chegv_2stage__(integer *itype, char *jobz, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, real *w, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chegv.c */
int chegv_(integer *itype, char *jobz, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, real *w, complex *work, integer *lwork, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chegvd.c */
int chegvd_(integer *itype, char *jobz, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, real *w, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chegvx.c */
int chegvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, complex *a, integer *lda, complex *b, integer *ldb, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* cherfs.c */
int cherfs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* chesv_aa_2stage.c */
int chesv_aa_2stage__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chesv_aa.c */
int chesv_aa__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chesv.c */
int chesv_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chesv_rk.c */
int chesv_rk__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *e, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chesv_rook.c */
int chesv_rook__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chesvx.c */
int chesvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* chesvxx.c */
int chesvxx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* cheswapr.c */
int cheswapr_(char *uplo, integer *n, complex *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* chetd2.c */
int chetd2_(char *uplo, integer *n, complex *a, integer *lda, real *d__, real *e, complex *tau, integer *info, ftnlen uplo_len);
/* chetf2.c */
int chetf2_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* chetf2_rk.c */
int chetf2_rk__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* chetf2_rook.c */
int chetf2_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* chetrd_2stage.c */
int chetrd_2stage__(char *vect, char *uplo, integer *n, complex *a, integer *lda, real *d__, real *e, complex *tau, complex *hous2, integer *lhous2, complex *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* chetrd.c */
int chetrd_(char *uplo, integer *n, complex *a, integer *lda, real *d__, real *e, complex *tau, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrd_he2hb.c */
int chetrd_he2hb__(char *uplo, integer *n, integer *kd, complex *a, integer *lda, complex *ab, integer *ldab, complex *tau, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrf_aa_2stage.c */
int chetrf_aa_2stage__(char *uplo, integer *n, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrf_aa.c */
int chetrf_aa__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrf.c */
int chetrf_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrf_rk.c */
int chetrf_rk__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrf_rook.c */
int chetrf_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetri2.c */
int chetri2_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetri2x.c */
int chetri2x_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *nb, integer *info, ftnlen uplo_len);
/* chetri_3.c */
int chetri_3__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetri_3x.c */
int chetri_3x__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *nb, integer *info, ftnlen uplo_len);
/* chetri.c */
int chetri_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* chetri_rook.c */
int chetri_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* chetrs2.c */
int chetrs2_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *info, ftnlen uplo_len);
/* chetrs_3.c */
int chetrs_3__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *e, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chetrs_aa_2stage.c */
int chetrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chetrs_aa.c */
int chetrs_aa__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* chetrs.c */
int chetrs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chetrs_rook.c */
int chetrs_rook__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chfrk.c */
int chfrk_(char *transr, char *uplo, char *trans, integer *n, integer *k, real *alpha, complex *a, integer *lda, real *beta, complex *c__, ftnlen transr_len, ftnlen uplo_len, ftnlen trans_len);
/* chgeqz.c */
int chgeqz_(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi, complex *h__, integer *ldh, complex *t, integer *ldt, complex *alpha, complex *beta, complex *q, integer *ldq, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *info, ftnlen job_len, ftnlen compq_len, ftnlen compz_len);
/* chla_transtype.c */
void chla_transtype__(char *ret_val, ftnlen ret_val_len, integer *trans);
/* chpcon.c */
int chpcon_(char *uplo, integer *n, complex *ap, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* chpev.c */
int chpev_(char *jobz, char *uplo, integer *n, complex *ap, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chpevd.c */
int chpevd_(char *jobz, char *uplo, integer *n, complex *ap, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chpevx.c */
int chpevx_(char *jobz, char *range, char *uplo, integer *n, complex *ap, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chpgst.c */
int chpgst_(integer *itype, char *uplo, integer *n, complex *ap, complex *bp, integer *info, ftnlen uplo_len);
/* chpgv.c */
int chpgv_(integer *itype, char *jobz, char *uplo, integer *n, complex *ap, complex *bp, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chpgvd.c */
int chpgvd_(integer *itype, char *jobz, char *uplo, integer *n, complex *ap, complex *bp, real *w, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* chpgvx.c */
int chpgvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, complex *ap, complex *bp, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, complex *work, real *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* chprfs.c */
int chprfs_(char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* chpsv.c */
int chpsv_(char *uplo, integer *n, integer *nrhs, complex *ap, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chpsvx.c */
int chpsvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* chptrd.c */
int chptrd_(char *uplo, integer *n, complex *ap, real *d__, real *e, complex *tau, integer *info, ftnlen uplo_len);
/* chptrf.c */
int chptrf_(char *uplo, integer *n, complex *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* chptri.c */
int chptri_(char *uplo, integer *n, complex *ap, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* chptrs.c */
int chptrs_(char *uplo, integer *n, integer *nrhs, complex *ap, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* chsein.c */
int chsein_(char *side, char *eigsrc, char *initv, logical *select, integer *n, complex *h__, integer *ldh, complex *w, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *mm, integer *m, complex *work, real *rwork, integer *ifaill, integer *ifailr, integer *info, ftnlen side_len, ftnlen eigsrc_len, ftnlen initv_len);
/* chseqr.c */
int chseqr_(char *job, char *compz, integer *n, integer *ilo, integer *ihi, complex *h__, integer *ldh, complex *w, complex *z__, integer *ldz, complex *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compz_len);
/* clabrd.c */
int clabrd_(integer *m, integer *n, integer *nb, complex *a, integer *lda, real *d__, real *e, complex *tauq, complex *taup, complex *x, integer *ldx, complex *y, integer *ldy);
/* clacgv.c */
int clacgv_(integer *n, complex *x, integer *incx);
/* clacn2.c */
int clacn2_(integer *n, complex *v, complex *x, real *est, integer *kase, integer *isave);
/* clacon.c */
int clacon_(integer *n, complex *v, complex *x, real *est, integer *kase);
/* clacp2.c */
int clacp2_(char *uplo, integer *m, integer *n, real *a, integer *lda, complex *b, integer *ldb, ftnlen uplo_len);
/* clacpy.c */
int clacpy_(char *uplo, integer *m, integer *n, complex *a, integer *lda, complex *b, integer *ldb, ftnlen uplo_len);
/* clacrm.c */
int clacrm_(integer *m, integer *n, complex *a, integer *lda, real *b, integer *ldb, complex *c__, integer *ldc, real *rwork);
/* clacrt.c */
int clacrt_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy, complex *c__, complex *s);
/* cladiv.c */
void cladiv_(complex *ret_val, complex *x, complex *y);
/* claed0.c */
int claed0_(integer *qsiz, integer *n, real *d__, real *e, complex *q, integer *ldq, complex *qstore, integer *ldqs, real *rwork, integer *iwork, integer *info);
/* claed7.c */
int claed7_(integer *n, integer *cutpnt, integer *qsiz, integer *tlvls, integer *curlvl, integer *curpbm, real *d__, complex *q, integer *ldq, real *rho, integer *indxq, real *qstore, integer *qptr, integer *prmptr, integer *perm, integer *givptr, integer *givcol, real *givnum, complex *work, real *rwork, integer *iwork, integer *info);
/* claed8.c */
int claed8_(integer *k, integer *n, integer *qsiz, complex *q, integer *ldq, real *d__, real *rho, integer *cutpnt, real *z__, real *dlamda, complex *q2, integer *ldq2, real *w, integer *indxp, integer *indx, integer *indxq, integer *perm, integer *givptr, integer *givcol, real *givnum, integer *info);
/* claein.c */
int claein_(logical *rightv, logical *noinit, integer *n, complex *h__, integer *ldh, complex *w, complex *v, complex *b, integer *ldb, real *rwork, real *eps3, real *smlnum, integer *info);
/* claesy.c */
int claesy_(complex *a, complex *b, complex *c__, complex *rt1, complex *rt2, complex *evscal, complex *cs1, complex *sn1);
/* claev2.c */
int claev2_(complex *a, complex *b, complex *c__, real *rt1, real *rt2, real *cs1, complex *sn1);
/* clag2z.c */
int clag2z_(integer *m, integer *n, complex *sa, integer *ldsa, doublecomplex *a, integer *lda, integer *info);
/* cla_gbamv.c */
int cla_gbamv__(integer *trans, integer *m, integer *n, integer *kl, integer *ku, real *alpha, complex *ab, integer *ldab, complex *x, integer *incx, real *beta, real *y, integer *incy);
/* cla_gbrcond_c.c */
doublereal cla_gbrcond_c__(char *trans, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, real *c__, logical *capply, integer *info, complex *work, real *rwork, ftnlen trans_len);
/* cla_gbrcond_x.c */
doublereal cla_gbrcond_x__(char *trans, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, complex *x, integer *info, complex *work, real *rwork, ftnlen trans_len);
/* cla_gbrfsx_extended.c */
int cla_gbrfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *kl, integer *ku, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, integer *ipiv, logical *colequ, real *c__, complex *b, integer *ldb, complex *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, complex *res, real *ayb, complex *dy, complex *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info);
/* cla_gbrpvgrw.c */
doublereal cla_gbrpvgrw__(integer *n, integer *kl, integer *ku, integer *ncols, complex *ab, integer *ldab, complex *afb, integer *ldafb);
/* cla_geamv.c */
int cla_geamv__(integer *trans, integer *m, integer *n, real *alpha, complex *a, integer *lda, complex *x, integer *incx, real *beta, real *y, integer *incy);
/* cla_gercond_c.c */
doublereal cla_gercond_c__(char *trans, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, real *c__, logical *capply, integer *info, complex *work, real *rwork, ftnlen trans_len);
/* cla_gercond_x.c */
doublereal cla_gercond_x__(char *trans, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *x, integer *info, complex *work, real *rwork, ftnlen trans_len);
/* cla_gerfsx_extended.c */
int cla_gerfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, logical *colequ, real *c__, complex *b, integer *ldb, complex *y, integer *ldy, real *berr_out__, integer *n_norms__, real *errs_n__, real *errs_c__, complex *res, real *ayb, complex *dy, complex *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info);
/* cla_gerpvgrw.c */
doublereal cla_gerpvgrw__(integer *n, integer *ncols, complex *a, integer *lda, complex *af, integer *ldaf);
/* clags2.c */
int clags2_(logical *upper, real *a1, complex *a2, real *a3, real *b1, complex *b2, real *b3, real *csu, complex *snu, real *csv, complex *snv, real *csq, complex *snq);
/* clagtm.c */
int clagtm_(char *trans, integer *n, integer *nrhs, real *alpha, complex *dl, complex *d__, complex *du, complex *x, integer *ldx, real *beta, complex *b, integer *ldb, ftnlen trans_len);
/* cla_heamv.c */
int cla_heamv__(integer *uplo, integer *n, real *alpha, complex *a, integer *lda, complex *x, integer *incx, real *beta, real *y, integer *incy);
/* clahef_aa.c */
int clahef_aa__(char *uplo, integer *j1, integer *m, integer *nb, complex *a, integer *lda, integer *ipiv, complex *h__, integer *ldh, complex *work, ftnlen uplo_len);
/* clahef.c */
int clahef_(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* clahef_rk.c */
int clahef_rk__(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, complex *e, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* clahef_rook.c */
int clahef_rook__(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* cla_hercond_c.c */
doublereal cla_hercond_c__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, real *c__, logical *capply, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_hercond_x.c */
doublereal cla_hercond_x__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *x, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_herfsx_extended.c */
int cla_herfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, logical *colequ, real *c__, complex *b, integer *ldb, complex *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, complex *res, real *ayb, complex *dy, complex *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* cla_herpvgrw.c */
doublereal cla_herpvgrw__(char *uplo, integer *n, integer *info, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, real *work, ftnlen uplo_len);
/* clahqr.c */
int clahqr_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, complex *h__, integer *ldh, complex *w, integer *iloz, integer *ihiz, complex *z__, integer *ldz, integer *info);
/* clahr2.c */
int clahr2_(integer *n, integer *k, integer *nb, complex *a, integer *lda, complex *tau, complex *t, integer *ldt, complex *y, integer *ldy);
/* claic1.c */
int claic1_(integer *job, integer *j, complex *x, real *sest, complex *w, complex *gamma, real *sestpr, complex *s, complex *c__);
/* cla_lin_berr.c */
int cla_lin_berr__(integer *n, integer *nz, integer *nrhs, complex *res, real *ayb, real *berr);
/* clals0.c */
int clals0_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *nrhs, complex *b, integer *ldb, complex *bx, integer *ldbx, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, real *givnum, integer *ldgnum, real *poles, real *difl, real *difr, real *z__, integer *k, real *c__, real *s, real *rwork, integer *info);
/* clalsa.c */
int clalsa_(integer *icompq, integer *smlsiz, integer *n, integer *nrhs, complex *b, integer *ldb, complex *bx, integer *ldbx, real *u, integer *ldu, real *vt, integer *k, real *difl, real *difr, real *z__, real *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, real *givnum, real *c__, real *s, real *rwork, integer *iwork, integer *info);
/* clalsd.c */
int clalsd_(char *uplo, integer *smlsiz, integer *n, integer *nrhs, real *d__, real *e, complex *b, integer *ldb, real *rcond, integer *rank, complex *work, real *rwork, integer *iwork, integer *info, ftnlen uplo_len);
/* clamswlq.c */
int clamswlq_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, complex *a, integer *lda, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* clamtsqr.c */
int clamtsqr_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, complex *a, integer *lda, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* clangb.c */
doublereal clangb_(char *norm, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, real *work, ftnlen norm_len);
/* clange.c */
doublereal clange_(char *norm, integer *m, integer *n, complex *a, integer *lda, real *work, ftnlen norm_len);
/* clangt.c */
doublereal clangt_(char *norm, integer *n, complex *dl, complex *d__, complex *du, ftnlen norm_len);
/* clanhb.c */
doublereal clanhb_(char *norm, char *uplo, integer *n, integer *k, complex *ab, integer *ldab, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clanhe.c */
doublereal clanhe_(char *norm, char *uplo, integer *n, complex *a, integer *lda, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clanhf.c */
doublereal clanhf_(char *norm, char *transr, char *uplo, integer *n, complex *a, real *work, ftnlen norm_len, ftnlen transr_len, ftnlen uplo_len);
/* clanhp.c */
doublereal clanhp_(char *norm, char *uplo, integer *n, complex *ap, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clanhs.c */
doublereal clanhs_(char *norm, integer *n, complex *a, integer *lda, real *work, ftnlen norm_len);
/* clanht.c */
doublereal clanht_(char *norm, integer *n, real *d__, complex *e, ftnlen norm_len);
/* clansb.c */
doublereal clansb_(char *norm, char *uplo, integer *n, integer *k, complex *ab, integer *ldab, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clansp.c */
doublereal clansp_(char *norm, char *uplo, integer *n, complex *ap, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clansy.c */
doublereal clansy_(char *norm, char *uplo, integer *n, complex *a, integer *lda, real *work, ftnlen norm_len, ftnlen uplo_len);
/* clantb.c */
doublereal clantb_(char *norm, char *uplo, char *diag, integer *n, integer *k, complex *ab, integer *ldab, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* clantp.c */
doublereal clantp_(char *norm, char *uplo, char *diag, integer *n, complex *ap, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* clantr.c */
doublereal clantr_(char *norm, char *uplo, char *diag, integer *m, integer *n, complex *a, integer *lda, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* clapll.c */
int clapll_(integer *n, complex *x, integer *incx, complex *y, integer *incy, real *ssmin);
/* clapmr.c */
int clapmr_(logical *forwrd, integer *m, integer *n, complex *x, integer *ldx, integer *k);
/* clapmt.c */
int clapmt_(logical *forwrd, integer *m, integer *n, complex *x, integer *ldx, integer *k);
/* cla_porcond_c.c */
doublereal cla_porcond_c__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, real *c__, logical *capply, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_porcond_x.c */
doublereal cla_porcond_x__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, complex *x, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_porfsx_extended.c */
int cla_porfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, logical *colequ, real *c__, complex *b, integer *ldb, complex *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, complex *res, real *ayb, complex *dy, complex *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* cla_porpvgrw.c */
doublereal cla_porpvgrw__(char *uplo, integer *ncols, complex *a, integer *lda, complex *af, integer *ldaf, real *work, ftnlen uplo_len);
/* claqgb.c */
int claqgb_(integer *m, integer *n, integer *kl, integer *ku, complex *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, char *equed, ftnlen equed_len);
/* claqge.c */
int claqge_(integer *m, integer *n, complex *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, char *equed, ftnlen equed_len);
/* claqhb.c */
int claqhb_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* claqhe.c */
int claqhe_(char *uplo, integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* claqhp.c */
int claqhp_(char *uplo, integer *n, complex *ap, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* claqp2.c */
int claqp2_(integer *m, integer *n, integer *offset, complex *a, integer *lda, integer *jpvt, complex *tau, real *vn1, real *vn2, complex *work);
/* claqps.c */
int claqps_(integer *m, integer *n, integer *offset, integer *nb, integer *kb, complex *a, integer *lda, integer *jpvt, complex *tau, real *vn1, real *vn2, complex *auxv, complex *f, integer *ldf);
/* claqr0.c */
int claqr0_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, complex *h__, integer *ldh, complex *w, integer *iloz, integer *ihiz, complex *z__, integer *ldz, complex *work, integer *lwork, integer *info);
/* claqr1.c */
int claqr1_(integer *n, complex *h__, integer *ldh, complex *s1, complex *s2, complex *v);
/* claqr2.c */
int claqr2_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, complex *h__, integer *ldh, integer *iloz, integer *ihiz, complex *z__, integer *ldz, integer *ns, integer *nd, complex *sh, complex *v, integer *ldv, integer *nh, complex *t, integer *ldt, integer *nv, complex *wv, integer *ldwv, complex *work, integer *lwork);
/* claqr3.c */
int claqr3_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, complex *h__, integer *ldh, integer *iloz, integer *ihiz, complex *z__, integer *ldz, integer *ns, integer *nd, complex *sh, complex *v, integer *ldv, integer *nh, complex *t, integer *ldt, integer *nv, complex *wv, integer *ldwv, complex *work, integer *lwork);
/* claqr4.c */
int claqr4_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, complex *h__, integer *ldh, complex *w, integer *iloz, integer *ihiz, complex *z__, integer *ldz, complex *work, integer *lwork, integer *info);
/* claqr5.c */
int claqr5_(logical *wantt, logical *wantz, integer *kacc22, integer *n, integer *ktop, integer *kbot, integer *nshfts, complex *s, complex *h__, integer *ldh, integer *iloz, integer *ihiz, complex *z__, integer *ldz, complex *v, integer *ldv, complex *u, integer *ldu, integer *nv, complex *wv, integer *ldwv, integer *nh, complex *wh, integer *ldwh);
/* claqsb.c */
int claqsb_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* claqsp.c */
int claqsp_(char *uplo, integer *n, complex *ap, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* claqsy.c */
int claqsy_(char *uplo, integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* clar1v.c */
int clar1v_(integer *n, integer *b1, integer *bn, real *lambda, real *d__, real *l, real *ld, real *lld, real *pivmin, real *gaptol, complex *z__, logical *wantnc, integer *negcnt, real *ztz, real *mingma, integer *r__, integer *isuppz, real *nrminv, real *resid, real *rqcorr, real *work);
/* clar2v.c */
int clar2v_(integer *n, complex *x, complex *y, complex *z__, integer *incx, real *c__, complex *s, integer *incc);
/* clarcm.c */
int clarcm_(integer *m, integer *n, real *a, integer *lda, complex *b, integer *ldb, complex *c__, integer *ldc, real *rwork);
/* clarfb.c */
int clarfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, complex *v, integer *ldv, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* clarf.c */
int clarf_(char *side, integer *m, integer *n, complex *v, integer *incv, complex *tau, complex *c__, integer *ldc, complex *work, ftnlen side_len);
/* clarfg.c */
int clarfg_(integer *n, complex *alpha, complex *x, integer *incx, complex *tau);
/* clarfgp.c */
int clarfgp_(integer *n, complex *alpha, complex *x, integer *incx, complex *tau);
/* clarft.c */
int clarft_(char *direct, char *storev, integer *n, integer *k, complex *v, integer *ldv, complex *tau, complex *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* clarfx.c */
int clarfx_(char *side, integer *m, integer *n, complex *v, complex *tau, complex *c__, integer *ldc, complex *work, ftnlen side_len);
/* clarfy.c */
int clarfy_(char *uplo, integer *n, complex *v, integer *incv, complex *tau, complex *c__, integer *ldc, complex *work, ftnlen uplo_len);
/* clargv.c */
int clargv_(integer *n, complex *x, integer *incx, complex *y, integer *incy, real *c__, integer *incc);
/* clarnv.c */
int clarnv_(integer *idist, integer *iseed, integer *n, complex *x);
/* clarrv.c */
int clarrv_(integer *n, real *vl, real *vu, real *d__, real *l, real *pivmin, integer *isplit, integer *m, integer *dol, integer *dou, real *minrgp, real *rtol1, real *rtol2, real *w, real *werr, real *wgap, integer *iblock, integer *indexw, real *gers, complex *z__, integer *ldz, integer *isuppz, real *work, integer *iwork, integer *info);
/* clarscl2.c */
int clarscl2_(integer *m, integer *n, real *d__, complex *x, integer *ldx);
/* clartg.c */
int clartg_(complex *f, complex *g, real *cs, complex *sn, complex *r__);
/* clartv.c */
int clartv_(integer *n, complex *x, integer *incx, complex *y, integer *incy, real *c__, complex *s, integer *incc);
/* clarzb.c */
int clarzb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, complex *v, integer *ldv, complex *t, integer *ldt, complex *c__, integer *ldc, complex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* clarz.c */
int clarz_(char *side, integer *m, integer *n, integer *l, complex *v, integer *incv, complex *tau, complex *c__, integer *ldc, complex *work, ftnlen side_len);
/* clarzt.c */
int clarzt_(char *direct, char *storev, integer *n, integer *k, complex *v, integer *ldv, complex *tau, complex *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* clascl2.c */
int clascl2_(integer *m, integer *n, real *d__, complex *x, integer *ldx);
/* clascl.c */
int clascl_(char *type__, integer *kl, integer *ku, real *cfrom, real *cto, integer *m, integer *n, complex *a, integer *lda, integer *info, ftnlen type_len);
/* claset.c */
int claset_(char *uplo, integer *m, integer *n, complex *alpha, complex *beta, complex *a, integer *lda, ftnlen uplo_len);
/* clasr.c */
int clasr_(char *side, char *pivot, char *direct, integer *m, integer *n, real *c__, real *s, complex *a, integer *lda, ftnlen side_len, ftnlen pivot_len, ftnlen direct_len);
/* classq.c */
int classq_(integer *n, complex *x, integer *incx, real *scale, real *sumsq);
/* claswlq.c */
int claswlq_(integer *m, integer *n, integer *mb, integer *nb, complex *a, integer *lda, complex *t, integer *ldt, complex *work, integer *lwork, integer *info);
/* claswp.c */
int claswp_(integer *n, complex *a, integer *lda, integer *k1, integer *k2, integer *ipiv, integer *incx);
/* cla_syamv.c */
int cla_syamv__(integer *uplo, integer *n, real *alpha, complex *a, integer *lda, complex *x, integer *incx, real *beta, real *y, integer *incy);
/* clasyf_aa.c */
int clasyf_aa__(char *uplo, integer *j1, integer *m, integer *nb, complex *a, integer *lda, integer *ipiv, complex *h__, integer *ldh, complex *work, ftnlen uplo_len);
/* clasyf.c */
int clasyf_(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* clasyf_rk.c */
int clasyf_rk__(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, complex *e, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* clasyf_rook.c */
int clasyf_rook__(char *uplo, integer *n, integer *nb, integer *kb, complex *a, integer *lda, integer *ipiv, complex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* cla_syrcond_c.c */
doublereal cla_syrcond_c__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, real *c__, logical *capply, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_syrcond_x.c */
doublereal cla_syrcond_x__(char *uplo, integer *n, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *x, integer *info, complex *work, real *rwork, ftnlen uplo_len);
/* cla_syrfsx_extended.c */
int cla_syrfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, logical *colequ, real *c__, complex *b, integer *ldb, complex *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, complex *res, real *ayb, complex *dy, complex *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* cla_syrpvgrw.c */
doublereal cla_syrpvgrw__(char *uplo, integer *n, integer *info, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, real *work, ftnlen uplo_len);
/* clatbs.c */
int clatbs_(char *uplo, char *trans, char *diag, char *normin, integer *n, integer *kd, complex *ab, integer *ldab, complex *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* clatdf.c */
int clatdf_(integer *ijob, integer *n, complex *z__, integer *ldz, complex *rhs, real *rdsum, real *rdscal, integer *ipiv, integer *jpiv);
/* clatps.c */
int clatps_(char *uplo, char *trans, char *diag, char *normin, integer *n, complex *ap, complex *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* clatrd.c */
int clatrd_(char *uplo, integer *n, integer *nb, complex *a, integer *lda, real *e, complex *tau, complex *w, integer *ldw, ftnlen uplo_len);
/* clatrs.c */
int clatrs_(char *uplo, char *trans, char *diag, char *normin, integer *n, complex *a, integer *lda, complex *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* clatrz.c */
int clatrz_(integer *m, integer *n, integer *l, complex *a, integer *lda, complex *tau, complex *work);
/* clatsqr.c */
int clatsqr_(integer *m, integer *n, integer *mb, integer *nb, complex *a, integer *lda, complex *t, integer *ldt, complex *work, integer *lwork, integer *info);
/* clauu2.c */
int clauu2_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* clauum.c */
int clauum_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* cla_wwaddw.c */
int cla_wwaddw__(integer *n, complex *x, complex *y, complex *w);
/* cpbcon.c */
int cpbcon_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *anorm, real *rcond, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cpbequ.c */
int cpbequ_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, real *s, real *scond, real *amax, integer *info, ftnlen uplo_len);
/* cpbrfs.c */
int cpbrfs_(char *uplo, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cpbstf.c */
int cpbstf_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* cpbsv.c */
int cpbsv_(char *uplo, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cpbsvx.c */
int cpbsvx_(char *fact, char *uplo, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *afb, integer *ldafb, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* cpbtf2.c */
int cpbtf2_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* cpbtrf.c */
int cpbtrf_(char *uplo, integer *n, integer *kd, complex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* cpbtrs.c */
int cpbtrs_(char *uplo, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cpftrf.c */
int cpftrf_(char *transr, char *uplo, integer *n, complex *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* cpftri.c */
int cpftri_(char *transr, char *uplo, integer *n, complex *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* cpftrs.c */
int cpftrs_(char *transr, char *uplo, integer *n, integer *nrhs, complex *a, complex *b, integer *ldb, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* cpocon.c */
int cpocon_(char *uplo, integer *n, complex *a, integer *lda, real *anorm, real *rcond, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cpoequb.c */
int cpoequb_(integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, integer *info);
/* cpoequ.c */
int cpoequ_(integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, integer *info);
/* cporfs.c */
int cporfs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cposv.c */
int cposv_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cposvx.c */
int cposvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* cposvxx.c */
int cposvxx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* cpotf2.c */
int cpotf2_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* cpotrf2.c */
int cpotrf2_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* cpotrf.c */
int cpotrf_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* cpotri.c */
int cpotri_(char *uplo, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* cpotrs.c */
int cpotrs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cppcon.c */
int cppcon_(char *uplo, integer *n, complex *ap, real *anorm, real *rcond, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cppequ.c */
int cppequ_(char *uplo, integer *n, complex *ap, real *s, real *scond, real *amax, integer *info, ftnlen uplo_len);
/* cpprfs.c */
int cpprfs_(char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cppsv.c */
int cppsv_(char *uplo, integer *n, integer *nrhs, complex *ap, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cppsvx.c */
int cppsvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* cpptrf.c */
int cpptrf_(char *uplo, integer *n, complex *ap, integer *info, ftnlen uplo_len);
/* cpptri.c */
int cpptri_(char *uplo, integer *n, complex *ap, integer *info, ftnlen uplo_len);
/* cpptrs.c */
int cpptrs_(char *uplo, integer *n, integer *nrhs, complex *ap, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cpstf2.c */
int cpstf2_(char *uplo, integer *n, complex *a, integer *lda, integer *piv, integer *rank, real *tol, real *work, integer *info, ftnlen uplo_len);
/* cpstrf.c */
int cpstrf_(char *uplo, integer *n, complex *a, integer *lda, integer *piv, integer *rank, real *tol, real *work, integer *info, ftnlen uplo_len);
/* cptcon.c */
int cptcon_(integer *n, real *d__, complex *e, real *anorm, real *rcond, real *rwork, integer *info);
/* cpteqr.c */
int cpteqr_(char *compz, integer *n, real *d__, real *e, complex *z__, integer *ldz, real *work, integer *info, ftnlen compz_len);
/* cptrfs.c */
int cptrfs_(char *uplo, integer *n, integer *nrhs, real *d__, complex *e, real *df, complex *ef, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cptsv.c */
int cptsv_(integer *n, integer *nrhs, real *d__, complex *e, complex *b, integer *ldb, integer *info);
/* cptsvx.c */
int cptsvx_(char *fact, integer *n, integer *nrhs, real *d__, complex *e, real *df, complex *ef, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len);
/* cpttrf.c */
int cpttrf_(integer *n, real *d__, complex *e, integer *info);
/* cpttrs.c */
int cpttrs_(char *uplo, integer *n, integer *nrhs, real *d__, complex *e, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cptts2.c */
int cptts2_(integer *iuplo, integer *n, integer *nrhs, real *d__, complex *e, complex *b, integer *ldb);
/* crot.c */
int crot_(integer *n, complex *cx, integer *incx, complex *cy, integer *incy, real *c__, complex *s);
/* cspcon.c */
int cspcon_(char *uplo, integer *n, complex *ap, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* cspmv.c */
int cspmv_(char *uplo, integer *n, complex *alpha, complex *ap, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen uplo_len);
/* cspr.c */
int cspr_(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *ap, ftnlen uplo_len);
/* csprfs.c */
int csprfs_(char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* cspsv.c */
int cspsv_(char *uplo, integer *n, integer *nrhs, complex *ap, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* cspsvx.c */
int cspsvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *ap, complex *afp, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* csptrf.c */
int csptrf_(char *uplo, integer *n, complex *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* csptri.c */
int csptri_(char *uplo, integer *n, complex *ap, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* csptrs.c */
int csptrs_(char *uplo, integer *n, integer *nrhs, complex *ap, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* csrscl.c */
int csrscl_(integer *n, real *sa, complex *sx, integer *incx);
/* cstedc.c */
int cstedc_(char *compz, integer *n, real *d__, real *e, complex *z__, integer *ldz, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen compz_len);
/* cstegr.c */
int cstegr_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, complex *z__, integer *ldz, integer *isuppz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* cstein.c */
int cstein_(integer *n, real *d__, real *e, integer *m, real *w, integer *iblock, integer *isplit, complex *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info);
/* cstemr.c */
int cstemr_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, integer *m, real *w, complex *z__, integer *ldz, integer *nzc, integer *isuppz, logical *tryrac, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* csteqr.c */
int csteqr_(char *compz, integer *n, real *d__, real *e, complex *z__, integer *ldz, real *work, integer *info, ftnlen compz_len);
/* csycon_3.c */
int csycon_3__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* csycon.c */
int csycon_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* csycon_rook.c */
int csycon_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, real *anorm, real *rcond, complex *work, integer *info, ftnlen uplo_len);
/* csyconv.c */
int csyconv_(char *uplo, char *way, integer *n, complex *a, integer *lda, integer *ipiv, complex *e, integer *info, ftnlen uplo_len, ftnlen way_len);
/* csyconvf.c */
int csyconvf_(char *uplo, char *way, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* csyconvf_rook.c */
int csyconvf_rook__(char *uplo, char *way, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* csyequb.c */
int csyequb_(char *uplo, integer *n, complex *a, integer *lda, real *s, real *scond, real *amax, complex *work, integer *info, ftnlen uplo_len);
/* csymv.c */
int csymv_(char *uplo, integer *n, complex *alpha, complex *a, integer *lda, complex *x, integer *incx, complex *beta, complex *y, integer *incy, ftnlen uplo_len);
/* csyr.c */
int csyr_(char *uplo, integer *n, complex *alpha, complex *x, integer *incx, complex *a, integer *lda, ftnlen uplo_len);
/* csyrfs.c */
int csyrfs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len);
/* csysv_aa_2stage.c */
int csysv_aa_2stage__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csysv_aa.c */
int csysv_aa__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csysv.c */
int csysv_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csysv_rk.c */
int csysv_rk__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *e, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csysv_rook.c */
int csysv_rook__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csysvx.c */
int csysvx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *ferr, real *berr, complex *work, integer *lwork, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* csysvxx.c */
int csysvxx_(char *fact, char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *af, integer *ldaf, integer *ipiv, char *equed, real *s, complex *b, integer *ldb, complex *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, complex *work, real *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* csyswapr.c */
int csyswapr_(char *uplo, integer *n, complex *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* csytf2.c */
int csytf2_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* csytf2_rk.c */
int csytf2_rk__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* csytf2_rook.c */
int csytf2_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* csytrf_aa_2stage.c */
int csytrf_aa_2stage__(char *uplo, integer *n, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytrf_aa.c */
int csytrf_aa__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytrf.c */
int csytrf_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytrf_rk.c */
int csytrf_rk__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytrf_rook.c */
int csytrf_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytri2.c */
int csytri2_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytri2x.c */
int csytri2x_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *nb, integer *info, ftnlen uplo_len);
/* csytri_3.c */
int csytri_3__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytri_3x.c */
int csytri_3x__(char *uplo, integer *n, complex *a, integer *lda, complex *e, integer *ipiv, complex *work, integer *nb, integer *info, ftnlen uplo_len);
/* csytri.c */
int csytri_(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* csytri_rook.c */
int csytri_rook__(char *uplo, integer *n, complex *a, integer *lda, integer *ipiv, complex *work, integer *info, ftnlen uplo_len);
/* csytrs2.c */
int csytrs2_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *info, ftnlen uplo_len);
/* csytrs_3.c */
int csytrs_3__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *e, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* csytrs_aa_2stage.c */
int csytrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, complex *tb, integer *ltb, integer *ipiv, integer *ipiv2, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* csytrs_aa.c */
int csytrs_aa__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* csytrs.c */
int csytrs_(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* csytrs_rook.c */
int csytrs_rook__(char *uplo, integer *n, integer *nrhs, complex *a, integer *lda, integer *ipiv, complex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ctbcon.c */
int ctbcon_(char *norm, char *uplo, char *diag, integer *n, integer *kd, complex *ab, integer *ldab, real *rcond, complex *work, real *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ctbrfs.c */
int ctbrfs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctbtrs.c */
int ctbtrs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, complex *ab, integer *ldab, complex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctfsm.c */
int ctfsm_(char *transr, char *side, char *uplo, char *trans, char *diag, integer *m, integer *n, complex *alpha, complex *a, complex *b, integer *ldb, ftnlen transr_len, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctftri.c */
int ctftri_(char *transr, char *uplo, char *diag, integer *n, complex *a, integer *info, ftnlen transr_len, ftnlen uplo_len, ftnlen diag_len);
/* ctfttp.c */
int ctfttp_(char *transr, char *uplo, integer *n, complex *arf, complex *ap, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ctfttr.c */
int ctfttr_(char *transr, char *uplo, integer *n, complex *arf, complex *a, integer *lda, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ctgevc.c */
int ctgevc_(char *side, char *howmny, logical *select, integer *n, complex *s, integer *lds, complex *p, integer *ldp, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *mm, integer *m, complex *work, real *rwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ctgex2.c */
int ctgex2_(logical *wantq, logical *wantz, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *q, integer *ldq, complex *z__, integer *ldz, integer *j1, integer *info);
/* ctgexc.c */
int ctgexc_(logical *wantq, logical *wantz, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *q, integer *ldq, complex *z__, integer *ldz, integer *ifst, integer *ilst, integer *info);
/* ctgsen.c */
int ctgsen_(integer *ijob, logical *wantq, logical *wantz, logical *select, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *alpha, complex *beta, complex *q, integer *ldq, complex *z__, integer *ldz, integer *m, real *pl, real *pr, real *dif, complex *work, integer *lwork, integer *iwork, integer *liwork, integer *info);
/* ctgsja.c */
int ctgsja_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, integer *k, integer *l, complex *a, integer *lda, complex *b, integer *ldb, real *tola, real *tolb, real *alpha, real *beta, complex *u, integer *ldu, complex *v, integer *ldv, complex *q, integer *ldq, complex *work, integer *ncycle, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* ctgsna.c */
int ctgsna_(char *job, char *howmny, logical *select, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *vl, integer *ldvl, complex *vr, integer *ldvr, real *s, real *dif, integer *mm, integer *m, complex *work, integer *lwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* ctgsy2.c */
int ctgsy2_(char *trans, integer *ijob, integer *m, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *c__, integer *ldc, complex *d__, integer *ldd, complex *e, integer *lde, complex *f, integer *ldf, real *scale, real *rdsum, real *rdscal, integer *info, ftnlen trans_len);
/* ctgsyl.c */
int ctgsyl_(char *trans, integer *ijob, integer *m, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *c__, integer *ldc, complex *d__, integer *ldd, complex *e, integer *lde, complex *f, integer *ldf, real *scale, real *dif, complex *work, integer *lwork, integer *iwork, integer *info, ftnlen trans_len);
/* ctpcon.c */
int ctpcon_(char *norm, char *uplo, char *diag, integer *n, complex *ap, real *rcond, complex *work, real *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ctplqt2.c */
int ctplqt2_(integer *m, integer *n, integer *l, complex *a, integer *lda, complex *b, integer *ldb, complex *t, integer *ldt, integer *info);
/* ctplqt.c */
int ctplqt_(integer *m, integer *n, integer *l, integer *mb, complex *a, integer *lda, complex *b, integer *ldb, complex *t, integer *ldt, complex *work, integer *info);
/* ctpmlqt.c */
int ctpmlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *mb, complex *v, integer *ldv, complex *t, integer *ldt, complex *a, integer *lda, complex *b, integer *ldb, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* ctpmqrt.c */
int ctpmqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *nb, complex *v, integer *ldv, complex *t, integer *ldt, complex *a, integer *lda, complex *b, integer *ldb, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* ctpqrt2.c */
int ctpqrt2_(integer *m, integer *n, integer *l, complex *a, integer *lda, complex *b, integer *ldb, complex *t, integer *ldt, integer *info);
/* ctpqrt.c */
int ctpqrt_(integer *m, integer *n, integer *l, integer *nb, complex *a, integer *lda, complex *b, integer *ldb, complex *t, integer *ldt, complex *work, integer *info);
/* ctprfb.c */
int ctprfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, complex *v, integer *ldv, complex *t, integer *ldt, complex *a, integer *lda, complex *b, integer *ldb, complex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* ctprfs.c */
int ctprfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, complex *ap, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctptri.c */
int ctptri_(char *uplo, char *diag, integer *n, complex *ap, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ctptrs.c */
int ctptrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, complex *ap, complex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctpttf.c */
int ctpttf_(char *transr, char *uplo, integer *n, complex *ap, complex *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ctpttr.c */
int ctpttr_(char *uplo, integer *n, complex *ap, complex *a, integer *lda, integer *info, ftnlen uplo_len);
/* ctrcon.c */
int ctrcon_(char *norm, char *uplo, char *diag, integer *n, complex *a, integer *lda, real *rcond, complex *work, real *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ctrevc3.c */
int ctrevc3_(char *side, char *howmny, logical *select, integer *n, complex *t, integer *ldt, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *mm, integer *m, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ctrevc.c */
int ctrevc_(char *side, char *howmny, logical *select, integer *n, complex *t, integer *ldt, complex *vl, integer *ldvl, complex *vr, integer *ldvr, integer *mm, integer *m, complex *work, real *rwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ctrexc.c */
int ctrexc_(char *compq, integer *n, complex *t, integer *ldt, complex *q, integer *ldq, integer *ifst, integer *ilst, integer *info, ftnlen compq_len);
/* ctrrfs.c */
int ctrrfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, complex *x, integer *ldx, real *ferr, real *berr, complex *work, real *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctrsen.c */
int ctrsen_(char *job, char *compq, logical *select, integer *n, complex *t, integer *ldt, complex *q, integer *ldq, complex *w, integer *m, real *s, real *sep, complex *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compq_len);
/* ctrsna.c */
int ctrsna_(char *job, char *howmny, logical *select, integer *n, complex *t, integer *ldt, complex *vl, integer *ldvl, complex *vr, integer *ldvr, real *s, real *sep, integer *mm, integer *m, complex *work, integer *ldwork, real *rwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* ctrsyl.c */
int ctrsyl_(char *trana, char *tranb, integer *isgn, integer *m, integer *n, complex *a, integer *lda, complex *b, integer *ldb, complex *c__, integer *ldc, real *scale, integer *info, ftnlen trana_len, ftnlen tranb_len);
/* ctrti2.c */
int ctrti2_(char *uplo, char *diag, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ctrtri.c */
int ctrtri_(char *uplo, char *diag, integer *n, complex *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ctrtrs.c */
int ctrtrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ctrttf.c */
int ctrttf_(char *transr, char *uplo, integer *n, complex *a, integer *lda, complex *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ctrttp.c */
int ctrttp_(char *uplo, integer *n, complex *a, integer *lda, complex *ap, integer *info, ftnlen uplo_len);
/* ctzrzf.c */
int ctzrzf_(integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cunbdb1.c */
int cunbdb1_(integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x21, integer *ldx21, real *theta, real *phi, complex *taup1, complex *taup2, complex *tauq1, complex *work, integer *lwork, integer *info);
/* cunbdb2.c */
int cunbdb2_(integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x21, integer *ldx21, real *theta, real *phi, complex *taup1, complex *taup2, complex *tauq1, complex *work, integer *lwork, integer *info);
/* cunbdb3.c */
int cunbdb3_(integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x21, integer *ldx21, real *theta, real *phi, complex *taup1, complex *taup2, complex *tauq1, complex *work, integer *lwork, integer *info);
/* cunbdb4.c */
int cunbdb4_(integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x21, integer *ldx21, real *theta, real *phi, complex *taup1, complex *taup2, complex *tauq1, complex *phantom, complex *work, integer *lwork, integer *info);
/* cunbdb5.c */
int cunbdb5_(integer *m1, integer *m2, integer *n, complex *x1, integer *incx1, complex *x2, integer *incx2, complex *q1, integer *ldq1, complex *q2, integer *ldq2, complex *work, integer *lwork, integer *info);
/* cunbdb6.c */
int cunbdb6_(integer *m1, integer *m2, integer *n, complex *x1, integer *incx1, complex *x2, integer *incx2, complex *q1, integer *ldq1, complex *q2, integer *ldq2, complex *work, integer *lwork, integer *info);
/* cunbdb.c */
int cunbdb_(char *trans, char *signs, integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x12, integer *ldx12, complex *x21, integer *ldx21, complex *x22, integer *ldx22, real *theta, real *phi, complex *taup1, complex *taup2, complex *tauq1, complex *tauq2, complex *work, integer *lwork, integer *info, ftnlen trans_len, ftnlen signs_len);
/* cuncsd2by1.c */
int cuncsd2by1_(char *jobu1, char *jobu2, char *jobv1t, integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x21, integer *ldx21, real *theta, complex *u1, integer *ldu1, complex *u2, integer *ldu2, complex *v1t, integer *ldv1t, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len);
/* cuncsd.c */
int cuncsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, char *signs, integer *m, integer *p, integer *q, complex *x11, integer *ldx11, complex *x12, integer *ldx12, complex *x21, integer *ldx21, complex *x22, integer *ldx22, real *theta, complex *u1, integer *ldu1, complex *u2, integer *ldu2, complex *v1t, integer *ldv1t, complex *v2t, integer *ldv2t, complex *work, integer *lwork, real *rwork, integer *lrwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len, ftnlen signs_len);
/* cung2l.c */
int cung2l_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cung2r.c */
int cung2r_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cungbr.c */
int cungbr_(char *vect, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info, ftnlen vect_len);
/* cunghr.c */
int cunghr_(integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cungl2.c */
int cungl2_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cunglq.c */
int cunglq_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cungql.c */
int cungql_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cungqr.c */
int cungqr_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cungr2.c */
int cungr2_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *info);
/* cungrq.c */
int cungrq_(integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info);
/* cungtr.c */
int cungtr_(char *uplo, integer *n, complex *a, integer *lda, complex *tau, complex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* cunm22.c */
int cunm22_(char *side, char *trans, integer *m, integer *n, integer *n1, integer *n2, complex *q, integer *ldq, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunm2l.c */
int cunm2l_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunm2r.c */
int cunm2r_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmbr.c */
int cunmbr_(char *vect, char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen side_len, ftnlen trans_len);
/* cunmhr.c */
int cunmhr_(char *side, char *trans, integer *m, integer *n, integer *ilo, integer *ihi, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunml2.c */
int cunml2_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmlq.c */
int cunmlq_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmql.c */
int cunmql_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmqr.c */
int cunmqr_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmr2.c */
int cunmr2_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmr3.c */
int cunmr3_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmrq.c */
int cunmrq_(char *side, char *trans, integer *m, integer *n, integer *k, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmrz.c */
int cunmrz_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* cunmtr.c */
int cunmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, complex *a, integer *lda, complex *tau, complex *c__, integer *ldc, complex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* cupgtr.c */
int cupgtr_(char *uplo, integer *n, complex *ap, complex *tau, complex *q, integer *ldq, complex *work, integer *info, ftnlen uplo_len);
/* cupmtr.c */
int cupmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, complex *ap, complex *tau, complex *c__, integer *ldc, complex *work, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* dbbcsd.c */
int dbbcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, integer *m, integer *p, integer *q, doublereal *theta, doublereal *phi, doublereal *u1, integer *ldu1, doublereal *u2, integer *ldu2, doublereal *v1t, integer *ldv1t, doublereal *v2t, integer *ldv2t, doublereal *b11d, doublereal *b11e, doublereal *b12d, doublereal *b12e, doublereal *b21d, doublereal *b21e, doublereal *b22d, doublereal *b22e, doublereal *work, integer *lwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len);
/* dbdsdc.c */
int dbdsdc_(char *uplo, char *compq, integer *n, doublereal *d__, doublereal *e, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *q, integer *iq, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen compq_len);
/* dbdsqr.c */
int dbdsqr_(char *uplo, integer *n, integer *ncvt, integer *nru, integer *ncc, doublereal *d__, doublereal *e, doublereal *vt, integer *ldvt, doublereal *u, integer *ldu, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen uplo_len);
/* ddisna.c */
int ddisna_(char *job, integer *m, integer *n, doublereal *d__, doublereal *sep, integer *info, ftnlen job_len);
/* dgbbrd.c */
int dgbbrd_(char *vect, integer *m, integer *n, integer *ncc, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *d__, doublereal *e, doublereal *q, integer *ldq, doublereal *pt, integer *ldpt, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen vect_len);
/* dgbcon.c */
int dgbcon_(char *norm, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len);
/* dgbequb.c */
int dgbequb_(integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* dgbequ.c */
int dgbequ_(integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* dgbrfs.c */
int dgbrfs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen trans_len);
/* dgbsv.c */
int dgbsv_(integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, integer *ipiv, doublereal *b, integer *ldb, integer *info);
/* dgbsvx.c */
int dgbsvx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* dgbsvxx.c */
int dgbsvxx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* dgbtf2.c */
int dgbtf2_(integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, integer *ipiv, integer *info);
/* dgbtrf.c */
int dgbtrf_(integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, integer *ipiv, integer *info);
/* dgbtrs.c */
int dgbtrs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen trans_len);
/* dgebak.c */
int dgebak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, doublereal *scale, integer *m, doublereal *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* dgebal.c */
int dgebal_(char *job, integer *n, doublereal *a, integer *lda, integer *ilo, integer *ihi, doublereal *scale, integer *info, ftnlen job_len);
/* dgebd2.c */
int dgebd2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tauq, doublereal *taup, doublereal *work, integer *info);
/* dgebrd.c */
int dgebrd_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tauq, doublereal *taup, doublereal *work, integer *lwork, integer *info);
/* dgecon.c */
int dgecon_(char *norm, integer *n, doublereal *a, integer *lda, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len);
/* dgeequb.c */
int dgeequb_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* dgeequ.c */
int dgeequ_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* dgees.c */
int dgees_(char *jobvs, char *sort, L_fp select, integer *n, doublereal *a, integer *lda, integer *sdim, doublereal *wr, doublereal *wi, doublereal *vs, integer *ldvs, doublereal *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len);
/* dgeesx.c */
int dgeesx_(char *jobvs, char *sort, L_fp select, char *sense, integer *n, doublereal *a, integer *lda, integer *sdim, doublereal *wr, doublereal *wi, doublereal *vs, integer *ldvs, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len, ftnlen sense_len);
/* dgeev.c */
int dgeev_(char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* dgeevx.c */
int dgeevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublereal *a, integer *lda, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* dgehd2.c */
int dgehd2_(integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgehrd.c */
int dgehrd_(integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgejsv.c */
int dgejsv_(char *joba, char *jobu, char *jobv, char *jobr, char *jobt, char *jobp, integer *m, integer *n, doublereal *a, integer *lda, doublereal *sva, doublereal *u, integer *ldu, doublereal *v, integer *ldv, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobr_len, ftnlen jobt_len, ftnlen jobp_len);
/* dgelq2.c */
int dgelq2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgelq.c */
int dgelq_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *t, integer *tsize, doublereal *work, integer *lwork, integer *info);
/* dgelqf.c */
int dgelqf_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgelqt.c */
int dgelqt_(integer *m, integer *n, integer *mb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *work, integer *info);
/* dgels.c */
int dgels_(char *trans, integer *m, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen trans_len);
/* dgelsd.c */
int dgelsd_(integer *m, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *s, doublereal *rcond, integer *rank, doublereal *work, integer *lwork, integer *iwork, integer *info);
/* dgelss.c */
int dgelss_(integer *m, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *s, doublereal *rcond, integer *rank, doublereal *work, integer *lwork, integer *info);
/* dgelsy.c */
int dgelsy_(integer *m, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *jpvt, doublereal *rcond, integer *rank, doublereal *work, integer *lwork, integer *info);
/* dgemlq.c */
int dgemlq_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *t, integer *tsize, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dgemlqt.c */
int dgemlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dgemqr.c */
int dgemqr_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *t, integer *tsize, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dgemqrt.c */
int dgemqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *nb, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dgeql2.c */
int dgeql2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgeqlf.c */
int dgeqlf_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgeqp3.c */
int dgeqp3_(integer *m, integer *n, doublereal *a, integer *lda, integer *jpvt, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgeqr2.c */
int dgeqr2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgeqr2p.c */
int dgeqr2p_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgeqr.c */
int dgeqr_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *t, integer *tsize, doublereal *work, integer *lwork, integer *info);
/* dgeqrf.c */
int dgeqrf_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgeqrfp.c */
int dgeqrfp_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgeqrt2.c */
int dgeqrt2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *t, integer *ldt, integer *info);
/* dgeqrt3.c */
int dgeqrt3_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *t, integer *ldt, integer *info);
/* dgeqrt.c */
int dgeqrt_(integer *m, integer *n, integer *nb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *work, integer *info);
/* dgerfs.c */
int dgerfs_(char *trans, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen trans_len);
/* dgerq2.c */
int dgerq2_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dgerqf.c */
int dgerqf_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dgesc2.c */
int dgesc2_(integer *n, doublereal *a, integer *lda, doublereal *rhs, integer *ipiv, integer *jpiv, doublereal *scale);
/* dgesdd.c */
int dgesdd_(char *jobz, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen jobz_len);
/* dgesv.c */
int dgesv_(integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info);
/* dgesvd.c */
int dgesvd_(char *jobu, char *jobvt, integer *m, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len);
/* dgesvdx.c */
int dgesvdx_(char *jobu, char *jobvt, char *range, integer *m, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, integer *ns, doublereal *s, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len, ftnlen range_len);
/* dgesvj.c */
int dgesvj_(char *joba, char *jobu, char *jobv, integer *m, integer *n, doublereal *a, integer *lda, doublereal *sva, integer *mv, doublereal *v, integer *ldv, doublereal *work, integer *lwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len);
/* dgesvx.c */
int dgesvx_(char *fact, char *trans, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* dgesvxx.c */
int dgesvxx_(char *fact, char *trans, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* dgetc2.c */
int dgetc2_(integer *n, doublereal *a, integer *lda, integer *ipiv, integer *jpiv, integer *info);
/* dgetf2.c */
int dgetf2_(integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info);
/* dgetrf2.c */
int dgetrf2_(integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info);
/* dgetrf.c */
int dgetrf_(integer *m, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info);
/* dgetri.c */
int dgetri_(integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info);
/* dgetrs.c */
int dgetrs_(char *trans, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen trans_len);
/* dgetsls.c */
int dgetsls_(char *trans, integer *m, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen trans_len);
/* dggbak.c */
int dggbak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, integer *m, doublereal *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* dggbal.c */
int dggbal_(char *job, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, doublereal *work, integer *info, ftnlen job_len);
/* dgges3.c */
int dgges3_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *sdim, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vsl, integer *ldvsl, doublereal *vsr, integer *ldvsr, doublereal *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* dgges.c */
int dgges_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *sdim, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vsl, integer *ldvsl, doublereal *vsr, integer *ldvsr, doublereal *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* dggesx.c */
int dggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, char *sense, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *sdim, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vsl, integer *ldvsl, doublereal *vsr, integer *ldvsr, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len, ftnlen sense_len);
/* dggev3.c */
int dggev3_(char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* dggev.c */
int dggev_(char *jobvl, char *jobvr, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* dggevx.c */
int dggevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, doublereal *abnrm, doublereal *bbnrm, doublereal *rconde, doublereal *rcondv, doublereal *work, integer *lwork, integer *iwork, logical *bwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* dggglm.c */
int dggglm_(integer *n, integer *m, integer *p, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *d__, doublereal *x, doublereal *y, doublereal *work, integer *lwork, integer *info);
/* dgghd3.c */
int dgghd3_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info, ftnlen compq_len, ftnlen compz_len);
/* dgghrd.c */
int dgghrd_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, integer *info, ftnlen compq_len, ftnlen compz_len);
/* dgglse.c */
int dgglse_(integer *m, integer *n, integer *p, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *c__, doublereal *d__, doublereal *x, doublereal *work, integer *lwork, integer *info);
/* dggqrf.c */
int dggqrf_(integer *n, integer *m, integer *p, doublereal *a, integer *lda, doublereal *taua, doublereal *b, integer *ldb, doublereal *taub, doublereal *work, integer *lwork, integer *info);
/* dggrqf.c */
int dggrqf_(integer *m, integer *p, integer *n, doublereal *a, integer *lda, doublereal *taua, doublereal *b, integer *ldb, doublereal *taub, doublereal *work, integer *lwork, integer *info);
/* dggsvd3.c */
int dggsvd3_(char *jobu, char *jobv, char *jobq, integer *m, integer *n, integer *p, integer *k, integer *l, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alpha, doublereal *beta, doublereal *u, integer *ldu, doublereal *v, integer *ldv, doublereal *q, integer *ldq, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* dggsvp3.c */
int dggsvp3_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *tola, doublereal *tolb, integer *k, integer *l, doublereal *u, integer *ldu, doublereal *v, integer *ldv, doublereal *q, integer *ldq, integer *iwork, doublereal *tau, doublereal *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* dgsvj0.c */
int dgsvj0_(char *jobv, integer *m, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *sva, integer *mv, doublereal *v, integer *ldv, doublereal *eps, doublereal *sfmin, doublereal *tol, integer *nsweep, doublereal *work, integer *lwork, integer *info, ftnlen jobv_len);
/* dgsvj1.c */
int dgsvj1_(char *jobv, integer *m, integer *n, integer *n1, doublereal *a, integer *lda, doublereal *d__, doublereal *sva, integer *mv, doublereal *v, integer *ldv, doublereal *eps, doublereal *sfmin, doublereal *tol, integer *nsweep, doublereal *work, integer *lwork, integer *info, ftnlen jobv_len);
/* dgtcon.c */
int dgtcon_(char *norm, integer *n, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len);
/* dgtrfs.c */
int dgtrfs_(char *trans, integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *dlf, doublereal *df, doublereal *duf, doublereal *du2, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen trans_len);
/* dgtsv.c */
int dgtsv_(integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *b, integer *ldb, integer *info);
/* dgtsvx.c */
int dgtsvx_(char *fact, char *trans, integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *dlf, doublereal *df, doublereal *duf, doublereal *du2, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len);
/* dgttrf.c */
int dgttrf_(integer *n, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, integer *info);
/* dgttrs.c */
int dgttrs_(char *trans, integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen trans_len);
/* dgtts2.c */
int dgtts2_(integer *itrans, integer *n, integer *nrhs, doublereal *dl, doublereal *d__, doublereal *du, doublereal *du2, integer *ipiv, doublereal *b, integer *ldb);
/* dhgeqz.c */
int dhgeqz_(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublereal *h__, integer *ldh, doublereal *t, integer *ldt, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compq_len, ftnlen compz_len);
/* dhsein.c */
int dhsein_(char *side, char *eigsrc, char *initv, logical *select, integer *n, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *mm, integer *m, doublereal *work, integer *ifaill, integer *ifailr, integer *info, ftnlen side_len, ftnlen eigsrc_len, ftnlen initv_len);
/* dhseqr.c */
int dhseqr_(char *job, char *compz, integer *n, integer *ilo, integer *ihi, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compz_len);
/* disnan.c */
logical disnan_(doublereal *din);
/* dlabad.c */
int dlabad_(doublereal *small, doublereal *large);
/* dlabrd.c */
int dlabrd_(integer *m, integer *n, integer *nb, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tauq, doublereal *taup, doublereal *x, integer *ldx, doublereal *y, integer *ldy);
/* dlacn2.c */
int dlacn2_(integer *n, doublereal *v, doublereal *x, integer *isgn, doublereal *est, integer *kase, integer *isave);
/* dlacon.c */
int dlacon_(integer *n, doublereal *v, doublereal *x, integer *isgn, doublereal *est, integer *kase);
/* dlacpy.c */
int dlacpy_(char *uplo, integer *m, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, ftnlen uplo_len);
/* dladiv.c */
int dladiv_(doublereal *a, doublereal *b, doublereal *c__, doublereal *d__, doublereal *p, doublereal *q);
int dladiv1_(doublereal *a, doublereal *b, doublereal *c__, doublereal *d__, doublereal *p, doublereal *q);
doublereal dladiv2_(doublereal *a, doublereal *b, doublereal *c__, doublereal *d__, doublereal *r__, doublereal *t);
/* dlae2.c */
int dlae2_(doublereal *a, doublereal *b, doublereal *c__, doublereal *rt1, doublereal *rt2);
/* dlaebz.c */
int dlaebz_(integer *ijob, integer *nitmax, integer *n, integer *mmax, integer *minp, integer *nbmin, doublereal *abstol, doublereal *reltol, doublereal *pivmin, doublereal *d__, doublereal *e, doublereal *e2, integer *nval, doublereal *ab, doublereal *c__, integer *mout, integer *nab, doublereal *work, integer *iwork, integer *info);
/* dlaed0.c */
int dlaed0_(integer *icompq, integer *qsiz, integer *n, doublereal *d__, doublereal *e, doublereal *q, integer *ldq, doublereal *qstore, integer *ldqs, doublereal *work, integer *iwork, integer *info);
/* dlaed1.c */
int dlaed1_(integer *n, doublereal *d__, doublereal *q, integer *ldq, integer *indxq, doublereal *rho, integer *cutpnt, doublereal *work, integer *iwork, integer *info);
/* dlaed2.c */
int dlaed2_(integer *k, integer *n, integer *n1, doublereal *d__, doublereal *q, integer *ldq, integer *indxq, doublereal *rho, doublereal *z__, doublereal *dlamda, doublereal *w, doublereal *q2, integer *indx, integer *indxc, integer *indxp, integer *coltyp, integer *info);
/* dlaed3.c */
int dlaed3_(integer *k, integer *n, integer *n1, doublereal *d__, doublereal *q, integer *ldq, doublereal *rho, doublereal *dlamda, doublereal *q2, integer *indx, integer *ctot, doublereal *w, doublereal *s, integer *info);
/* dlaed4.c */
int dlaed4_(integer *n, integer *i__, doublereal *d__, doublereal *z__, doublereal *delta, doublereal *rho, doublereal *dlam, integer *info);
/* dlaed5.c */
int dlaed5_(integer *i__, doublereal *d__, doublereal *z__, doublereal *delta, doublereal *rho, doublereal *dlam);
/* dlaed6.c */
int dlaed6_(integer *kniter, logical *orgati, doublereal *rho, doublereal *d__, doublereal *z__, doublereal *finit, doublereal *tau, integer *info);
/* dlaed7.c */
int dlaed7_(integer *icompq, integer *n, integer *qsiz, integer *tlvls, integer *curlvl, integer *curpbm, doublereal *d__, doublereal *q, integer *ldq, integer *indxq, doublereal *rho, integer *cutpnt, doublereal *qstore, integer *qptr, integer *prmptr, integer *perm, integer *givptr, integer *givcol, doublereal *givnum, doublereal *work, integer *iwork, integer *info);
/* dlaed8.c */
int dlaed8_(integer *icompq, integer *k, integer *n, integer *qsiz, doublereal *d__, doublereal *q, integer *ldq, integer *indxq, doublereal *rho, integer *cutpnt, doublereal *z__, doublereal *dlamda, doublereal *q2, integer *ldq2, doublereal *w, integer *perm, integer *givptr, integer *givcol, doublereal *givnum, integer *indxp, integer *indx, integer *info);
/* dlaed9.c */
int dlaed9_(integer *k, integer *kstart, integer *kstop, integer *n, doublereal *d__, doublereal *q, integer *ldq, doublereal *rho, doublereal *dlamda, doublereal *w, doublereal *s, integer *lds, integer *info);
/* dlaeda.c */
int dlaeda_(integer *n, integer *tlvls, integer *curlvl, integer *curpbm, integer *prmptr, integer *perm, integer *givptr, integer *givcol, doublereal *givnum, doublereal *q, integer *qptr, doublereal *z__, doublereal *ztemp, integer *info);
/* dlaein.c */
int dlaein_(logical *rightv, logical *noinit, integer *n, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, doublereal *vr, doublereal *vi, doublereal *b, integer *ldb, doublereal *work, doublereal *eps3, doublereal *smlnum, doublereal *bignum, integer *info);
/* dlaev2.c */
int dlaev2_(doublereal *a, doublereal *b, doublereal *c__, doublereal *rt1, doublereal *rt2, doublereal *cs1, doublereal *sn1);
/* dlaexc.c */
int dlaexc_(logical *wantq, integer *n, doublereal *t, integer *ldt, doublereal *q, integer *ldq, integer *j1, integer *n1, integer *n2, doublereal *work, integer *info);
/* dlag2.c */
int dlag2_(doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *safmin, doublereal *scale1, doublereal *scale2, doublereal *wr1, doublereal *wr2, doublereal *wi);
/* dlag2s.c */
int dlag2s_(integer *m, integer *n, doublereal *a, integer *lda, real *sa, integer *ldsa, integer *info);
/* dla_gbamv.c */
int dla_gbamv__(integer *trans, integer *m, integer *n, integer *kl, integer *ku, doublereal *alpha, doublereal *ab, integer *ldab, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dla_gbrcond.c */
doublereal dla_gbrcond__(char *trans, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, integer *ipiv, integer *cmode, doublereal *c__, integer *info, doublereal *work, integer *iwork, ftnlen trans_len);
/* dla_gbrfsx_extended.c */
int dla_gbrfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *kl, integer *ku, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, integer *ipiv, logical *colequ, doublereal *c__, doublereal *b, integer *ldb, doublereal *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublereal *res, doublereal *ayb, doublereal *dy, doublereal *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info);
/* dla_gbrpvgrw.c */
doublereal dla_gbrpvgrw__(integer *n, integer *kl, integer *ku, integer *ncols, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb);
/* dla_geamv.c */
int dla_geamv__(integer *trans, integer *m, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dla_gercond.c */
doublereal dla_gercond__(char *trans, integer *n, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, integer *cmode, doublereal *c__, integer *info, doublereal *work, integer *iwork, ftnlen trans_len);
/* dla_gerfsx_extended.c */
int dla_gerfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, logical *colequ, doublereal *c__, doublereal *b, integer *ldb, doublereal *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *errs_n__, doublereal *errs_c__, doublereal *res, doublereal *ayb, doublereal *dy, doublereal *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info);
/* dla_gerpvgrw.c */
doublereal dla_gerpvgrw__(integer *n, integer *ncols, doublereal *a, integer *lda, doublereal *af, integer *ldaf);
/* dlags2.c */
int dlags2_(logical *upper, doublereal *a1, doublereal *a2, doublereal *a3, doublereal *b1, doublereal *b2, doublereal *b3, doublereal *csu, doublereal *snu, doublereal *csv, doublereal *snv, doublereal *csq, doublereal *snq);
/* dlagtf.c */
int dlagtf_(integer *n, doublereal *a, doublereal *lambda, doublereal *b, doublereal *c__, doublereal *tol, doublereal *d__, integer *in, integer *info);
/* dlagtm.c */
int dlagtm_(char *trans, integer *n, integer *nrhs, doublereal *alpha, doublereal *dl, doublereal *d__, doublereal *du, doublereal *x, integer *ldx, doublereal *beta, doublereal *b, integer *ldb, ftnlen trans_len);
/* dlagts.c */
int dlagts_(integer *job, integer *n, doublereal *a, doublereal *b, doublereal *c__, doublereal *d__, integer *in, doublereal *y, doublereal *tol, integer *info);
/* dlagv2.c */
int dlagv2_(doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *csl, doublereal *snl, doublereal *csr, doublereal *snr);
/* dlahqr.c */
int dlahqr_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, integer *info);
/* dlahr2.c */
int dlahr2_(integer *n, integer *k, integer *nb, doublereal *a, integer *lda, doublereal *tau, doublereal *t, integer *ldt, doublereal *y, integer *ldy);
/* dlaic1.c */
int dlaic1_(integer *job, integer *j, doublereal *x, doublereal *sest, doublereal *w, doublereal *gamma, doublereal *sestpr, doublereal *s, doublereal *c__);
/* dlaisnan.c */
logical dlaisnan_(doublereal *din1, doublereal *din2);
/* dla_lin_berr.c */
int dla_lin_berr__(integer *n, integer *nz, integer *nrhs, doublereal *res, doublereal *ayb, doublereal *berr);
/* dlaln2.c */
int dlaln2_(logical *ltrans, integer *na, integer *nw, doublereal *smin, doublereal *ca, doublereal *a, integer *lda, doublereal *d1, doublereal *d2, doublereal *b, integer *ldb, doublereal *wr, doublereal *wi, doublereal *x, integer *ldx, doublereal *scale, doublereal *xnorm, integer *info);
/* dlals0.c */
int dlals0_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *nrhs, doublereal *b, integer *ldb, doublereal *bx, integer *ldbx, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, doublereal *givnum, integer *ldgnum, doublereal *poles, doublereal *difl, doublereal *difr, doublereal *z__, integer *k, doublereal *c__, doublereal *s, doublereal *work, integer *info);
/* dlalsa.c */
int dlalsa_(integer *icompq, integer *smlsiz, integer *n, integer *nrhs, doublereal *b, integer *ldb, doublereal *bx, integer *ldbx, doublereal *u, integer *ldu, doublereal *vt, integer *k, doublereal *difl, doublereal *difr, doublereal *z__, doublereal *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, doublereal *givnum, doublereal *c__, doublereal *s, doublereal *work, integer *iwork, integer *info);
/* dlalsd.c */
int dlalsd_(char *uplo, integer *smlsiz, integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *b, integer *ldb, doublereal *rcond, integer *rank, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dlamrg.c */
int dlamrg_(integer *n1, integer *n2, doublereal *a, integer *dtrd1, integer *dtrd2, integer *index);
/* dlamswlq.c */
int dlamswlq_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dlamtsqr.c */
int dlamtsqr_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dlaneg.c */
integer dlaneg_(integer *n, doublereal *d__, doublereal *lld, doublereal *sigma, doublereal *pivmin, integer *r__);
/* dlangb.c */
doublereal dlangb_(char *norm, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *work, ftnlen norm_len);
/* dlange.c */
doublereal dlange_(char *norm, integer *m, integer *n, doublereal *a, integer *lda, doublereal *work, ftnlen norm_len);
/* dlangt.c */
doublereal dlangt_(char *norm, integer *n, doublereal *dl, doublereal *d__, doublereal *du, ftnlen norm_len);
/* dlanhs.c */
doublereal dlanhs_(char *norm, integer *n, doublereal *a, integer *lda, doublereal *work, ftnlen norm_len);
/* dlansb.c */
doublereal dlansb_(char *norm, char *uplo, integer *n, integer *k, doublereal *ab, integer *ldab, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* dlansf.c */
doublereal dlansf_(char *norm, char *transr, char *uplo, integer *n, doublereal *a, doublereal *work, ftnlen norm_len, ftnlen transr_len, ftnlen uplo_len);
/* dlansp.c */
doublereal dlansp_(char *norm, char *uplo, integer *n, doublereal *ap, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* dlanst.c */
doublereal dlanst_(char *norm, integer *n, doublereal *d__, doublereal *e, ftnlen norm_len);
/* dlansy.c */
doublereal dlansy_(char *norm, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* dlantb.c */
doublereal dlantb_(char *norm, char *uplo, char *diag, integer *n, integer *k, doublereal *ab, integer *ldab, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dlantp.c */
doublereal dlantp_(char *norm, char *uplo, char *diag, integer *n, doublereal *ap, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dlantr.c */
doublereal dlantr_(char *norm, char *uplo, char *diag, integer *m, integer *n, doublereal *a, integer *lda, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dlanv2.c */
int dlanv2_(doublereal *a, doublereal *b, doublereal *c__, doublereal *d__, doublereal *rt1r, doublereal *rt1i, doublereal *rt2r, doublereal *rt2i, doublereal *cs, doublereal *sn);
/* dlapll.c */
int dlapll_(integer *n, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *ssmin);
/* dlapmr.c */
int dlapmr_(logical *forwrd, integer *m, integer *n, doublereal *x, integer *ldx, integer *k);
/* dlapmt.c */
int dlapmt_(logical *forwrd, integer *m, integer *n, doublereal *x, integer *ldx, integer *k);
/* dla_porcond.c */
doublereal dla_porcond__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *cmode, doublereal *c__, integer *info, doublereal *work, integer *iwork, ftnlen uplo_len);
/* dla_porfsx_extended.c */
int dla_porfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, logical *colequ, doublereal *c__, doublereal *b, integer *ldb, doublereal *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublereal *res, doublereal *ayb, doublereal *dy, doublereal *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* dla_porpvgrw.c */
doublereal dla_porpvgrw__(char *uplo, integer *ncols, doublereal *a, integer *lda, doublereal *af, integer *ldaf, doublereal *work, ftnlen uplo_len);
/* dlapy2.c */
doublereal dlapy2_(doublereal *x, doublereal *y);
/* dlapy3.c */
doublereal dlapy3_(doublereal *x, doublereal *y, doublereal *z__);
/* dlaqgb.c */
int dlaqgb_(integer *m, integer *n, integer *kl, integer *ku, doublereal *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, char *equed, ftnlen equed_len);
/* dlaqge.c */
int dlaqge_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, char *equed, ftnlen equed_len);
/* dlaqp2.c */
int dlaqp2_(integer *m, integer *n, integer *offset, doublereal *a, integer *lda, integer *jpvt, doublereal *tau, doublereal *vn1, doublereal *vn2, doublereal *work);
/* dlaqps.c */
int dlaqps_(integer *m, integer *n, integer *offset, integer *nb, integer *kb, doublereal *a, integer *lda, integer *jpvt, doublereal *tau, doublereal *vn1, doublereal *vn2, doublereal *auxv, doublereal *f, integer *ldf);
/* dlaqr0.c */
int dlaqr0_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info);
/* dlaqr1.c */
int dlaqr1_(integer *n, doublereal *h__, integer *ldh, doublereal *sr1, doublereal *si1, doublereal *sr2, doublereal *si2, doublereal *v);
/* dlaqr2.c */
int dlaqr2_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, doublereal *h__, integer *ldh, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, integer *ns, integer *nd, doublereal *sr, doublereal *si, doublereal *v, integer *ldv, integer *nh, doublereal *t, integer *ldt, integer *nv, doublereal *wv, integer *ldwv, doublereal *work, integer *lwork);
/* dlaqr3.c */
int dlaqr3_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, doublereal *h__, integer *ldh, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, integer *ns, integer *nd, doublereal *sr, doublereal *si, doublereal *v, integer *ldv, integer *nh, doublereal *t, integer *ldt, integer *nv, doublereal *wv, integer *ldwv, doublereal *work, integer *lwork);
/* dlaqr4.c */
int dlaqr4_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublereal *h__, integer *ldh, doublereal *wr, doublereal *wi, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info);
/* dlaqr5.c */
int dlaqr5_(logical *wantt, logical *wantz, integer *kacc22, integer *n, integer *ktop, integer *kbot, integer *nshfts, doublereal *sr, doublereal *si, doublereal *h__, integer *ldh, integer *iloz, integer *ihiz, doublereal *z__, integer *ldz, doublereal *v, integer *ldv, doublereal *u, integer *ldu, integer *nv, doublereal *wv, integer *ldwv, integer *nh, doublereal *wh, integer *ldwh);
/* dlaqsb.c */
int dlaqsb_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* dlaqsp.c */
int dlaqsp_(char *uplo, integer *n, doublereal *ap, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* dlaqsy.c */
int dlaqsy_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* dlaqtr.c */
int dlaqtr_(logical *ltran, logical *lreal, integer *n, doublereal *t, integer *ldt, doublereal *b, doublereal *w, doublereal *scale, doublereal *x, doublereal *work, integer *info);
/* dlar1v.c */
int dlar1v_(integer *n, integer *b1, integer *bn, doublereal *lambda, doublereal *d__, doublereal *l, doublereal *ld, doublereal *lld, doublereal *pivmin, doublereal *gaptol, doublereal *z__, logical *wantnc, integer *negcnt, doublereal *ztz, doublereal *mingma, integer *r__, integer *isuppz, doublereal *nrminv, doublereal *resid, doublereal *rqcorr, doublereal *work);
/* dlar2v.c */
int dlar2v_(integer *n, doublereal *x, doublereal *y, doublereal *z__, integer *incx, doublereal *c__, doublereal *s, integer *incc);
/* dlarfb.c */
int dlarfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* dlarf.c */
int dlarf_(char *side, integer *m, integer *n, doublereal *v, integer *incv, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, ftnlen side_len);
/* dlarfg.c */
int dlarfg_(integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *tau);
/* dlarfgp.c */
int dlarfgp_(integer *n, doublereal *alpha, doublereal *x, integer *incx, doublereal *tau);
/* dlarft.c */
int dlarft_(char *direct, char *storev, integer *n, integer *k, doublereal *v, integer *ldv, doublereal *tau, doublereal *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* dlarfx.c */
int dlarfx_(char *side, integer *m, integer *n, doublereal *v, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, ftnlen side_len);
/* dlarfy.c */
int dlarfy_(char *uplo, integer *n, doublereal *v, integer *incv, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, ftnlen uplo_len);
/* dlargv.c */
int dlargv_(integer *n, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *c__, integer *incc);
/* dlarnv.c */
int dlarnv_(integer *idist, integer *iseed, integer *n, doublereal *x);
/* dlarra.c */
int dlarra_(integer *n, doublereal *d__, doublereal *e, doublereal *e2, doublereal *spltol, doublereal *tnrm, integer *nsplit, integer *isplit, integer *info);
/* dlarrb.c */
int dlarrb_(integer *n, doublereal *d__, doublereal *lld, integer *ifirst, integer *ilast, doublereal *rtol1, doublereal *rtol2, integer *offset, doublereal *w, doublereal *wgap, doublereal *werr, doublereal *work, integer *iwork, doublereal *pivmin, doublereal *spdiam, integer *twist, integer *info);
/* dlarrc.c */
int dlarrc_(char *jobt, integer *n, doublereal *vl, doublereal *vu, doublereal *d__, doublereal *e, doublereal *pivmin, integer *eigcnt, integer *lcnt, integer *rcnt, integer *info, ftnlen jobt_len);
/* dlarrd.c */
int dlarrd_(char *range, char *order, integer *n, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *gers, doublereal *reltol, doublereal *d__, doublereal *e, doublereal *e2, doublereal *pivmin, integer *nsplit, integer *isplit, integer *m, doublereal *w, doublereal *werr, doublereal *wl, doublereal *wu, integer *iblock, integer *indexw, doublereal *work, integer *iwork, integer *info, ftnlen range_len, ftnlen order_len);
/* dlarre.c */
int dlarre_(char *range, integer *n, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *d__, doublereal *e, doublereal *e2, doublereal *rtol1, doublereal *rtol2, doublereal *spltol, integer *nsplit, integer *isplit, integer *m, doublereal *w, doublereal *werr, doublereal *wgap, integer *iblock, integer *indexw, doublereal *gers, doublereal *pivmin, doublereal *work, integer *iwork, integer *info, ftnlen range_len);
/* dlarrf.c */
int dlarrf_(integer *n, doublereal *d__, doublereal *l, doublereal *ld, integer *clstrt, integer *clend, doublereal *w, doublereal *wgap, doublereal *werr, doublereal *spdiam, doublereal *clgapl, doublereal *clgapr, doublereal *pivmin, doublereal *sigma, doublereal *dplus, doublereal *lplus, doublereal *work, integer *info);
/* dlarrj.c */
int dlarrj_(integer *n, doublereal *d__, doublereal *e2, integer *ifirst, integer *ilast, doublereal *rtol, integer *offset, doublereal *w, doublereal *werr, doublereal *work, integer *iwork, doublereal *pivmin, doublereal *spdiam, integer *info);
/* dlarrk.c */
int dlarrk_(integer *n, integer *iw, doublereal *gl, doublereal *gu, doublereal *d__, doublereal *e2, doublereal *pivmin, doublereal *reltol, doublereal *w, doublereal *werr, integer *info);
/* dlarrr.c */
int dlarrr_(integer *n, doublereal *d__, doublereal *e, integer *info);
/* dlarrv.c */
int dlarrv_(integer *n, doublereal *vl, doublereal *vu, doublereal *d__, doublereal *l, doublereal *pivmin, integer *isplit, integer *m, integer *dol, integer *dou, doublereal *minrgp, doublereal *rtol1, doublereal *rtol2, doublereal *w, doublereal *werr, doublereal *wgap, integer *iblock, integer *indexw, doublereal *gers, doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, integer *iwork, integer *info);
/* dlarscl2.c */
int dlarscl2_(integer *m, integer *n, doublereal *d__, doublereal *x, integer *ldx);
/* dlartg.c */
int dlartg_(doublereal *f, doublereal *g, doublereal *cs, doublereal *sn, doublereal *r__);
/* dlartgp.c */
int dlartgp_(doublereal *f, doublereal *g, doublereal *cs, doublereal *sn, doublereal *r__);
/* dlartgs.c */
int dlartgs_(doublereal *x, doublereal *y, doublereal *sigma, doublereal *cs, doublereal *sn);
/* dlartv.c */
int dlartv_(integer *n, doublereal *x, integer *incx, doublereal *y, integer *incy, doublereal *c__, doublereal *s, integer *incc);
/* dlaruv.c */
int dlaruv_(integer *iseed, integer *n, doublereal *x);
/* dlarzb.c */
int dlarzb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *c__, integer *ldc, doublereal *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* dlarz.c */
int dlarz_(char *side, integer *m, integer *n, integer *l, doublereal *v, integer *incv, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, ftnlen side_len);
/* dlarzt.c */
int dlarzt_(char *direct, char *storev, integer *n, integer *k, doublereal *v, integer *ldv, doublereal *tau, doublereal *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* dlas2.c */
int dlas2_(doublereal *f, doublereal *g, doublereal *h__, doublereal *ssmin, doublereal *ssmax);
/* dlascl2.c */
int dlascl2_(integer *m, integer *n, doublereal *d__, doublereal *x, integer *ldx);
/* dlascl.c */
int dlascl_(char *type__, integer *kl, integer *ku, doublereal *cfrom, doublereal *cto, integer *m, integer *n, doublereal *a, integer *lda, integer *info, ftnlen type_len);
/* dlasd0.c */
int dlasd0_(integer *n, integer *sqre, doublereal *d__, doublereal *e, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, integer *smlsiz, integer *iwork, doublereal *work, integer *info);
/* dlasd1.c */
int dlasd1_(integer *nl, integer *nr, integer *sqre, doublereal *d__, doublereal *alpha, doublereal *beta, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, integer *idxq, integer *iwork, doublereal *work, integer *info);
/* dlasd2.c */
int dlasd2_(integer *nl, integer *nr, integer *sqre, integer *k, doublereal *d__, doublereal *z__, doublereal *alpha, doublereal *beta, doublereal *u, integer *ldu, doublereal *vt, integer *ldvt, doublereal *dsigma, doublereal *u2, integer *ldu2, doublereal *vt2, integer *ldvt2, integer *idxp, integer *idx, integer *idxc, integer *idxq, integer *coltyp, integer *info);
/* dlasd3.c */
int dlasd3_(integer *nl, integer *nr, integer *sqre, integer *k, doublereal *d__, doublereal *q, integer *ldq, doublereal *dsigma, doublereal *u, integer *ldu, doublereal *u2, integer *ldu2, doublereal *vt, integer *ldvt, doublereal *vt2, integer *ldvt2, integer *idxc, integer *ctot, doublereal *z__, integer *info);
/* dlasd4.c */
int dlasd4_(integer *n, integer *i__, doublereal *d__, doublereal *z__, doublereal *delta, doublereal *rho, doublereal *sigma, doublereal *work, integer *info);
/* dlasd5.c */
int dlasd5_(integer *i__, doublereal *d__, doublereal *z__, doublereal *delta, doublereal *rho, doublereal *dsigma, doublereal *work);
/* dlasd6.c */
int dlasd6_(integer *icompq, integer *nl, integer *nr, integer *sqre, doublereal *d__, doublereal *vf, doublereal *vl, doublereal *alpha, doublereal *beta, integer *idxq, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, doublereal *givnum, integer *ldgnum, doublereal *poles, doublereal *difl, doublereal *difr, doublereal *z__, integer *k, doublereal *c__, doublereal *s, doublereal *work, integer *iwork, integer *info);
/* dlasd7.c */
int dlasd7_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *k, doublereal *d__, doublereal *z__, doublereal *zw, doublereal *vf, doublereal *vfw, doublereal *vl, doublereal *vlw, doublereal *alpha, doublereal *beta, doublereal *dsigma, integer *idx, integer *idxp, integer *idxq, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, doublereal *givnum, integer *ldgnum, doublereal *c__, doublereal *s, integer *info);
/* dlasd8.c */
int dlasd8_(integer *icompq, integer *k, doublereal *d__, doublereal *z__, doublereal *vf, doublereal *vl, doublereal *difl, doublereal *difr, integer *lddifr, doublereal *dsigma, doublereal *work, integer *info);
/* dlasda.c */
int dlasda_(integer *icompq, integer *smlsiz, integer *n, integer *sqre, doublereal *d__, doublereal *e, doublereal *u, integer *ldu, doublereal *vt, integer *k, doublereal *difl, doublereal *difr, doublereal *z__, doublereal *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, doublereal *givnum, doublereal *c__, doublereal *s, doublereal *work, integer *iwork, integer *info);
/* dlasdq.c */
int dlasdq_(char *uplo, integer *sqre, integer *n, integer *ncvt, integer *nru, integer *ncc, doublereal *d__, doublereal *e, doublereal *vt, integer *ldvt, doublereal *u, integer *ldu, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen uplo_len);
/* dlasdt.c */
int dlasdt_(integer *n, integer *lvl, integer *nd, integer *inode, integer *ndiml, integer *ndimr, integer *msub);
/* dlaset.c */
int dlaset_(char *uplo, integer *m, integer *n, doublereal *alpha, doublereal *beta, doublereal *a, integer *lda, ftnlen uplo_len);
/* dlasq1.c */
int dlasq1_(integer *n, doublereal *d__, doublereal *e, doublereal *work, integer *info);
/* dlasq2.c */
int dlasq2_(integer *n, doublereal *z__, integer *info);
/* dlasq3.c */
int dlasq3_(integer *i0, integer *n0, doublereal *z__, integer *pp, doublereal *dmin__, doublereal *sigma, doublereal *desig, doublereal *qmax, integer *nfail, integer *iter, integer *ndiv, logical *ieee, integer *ttype, doublereal *dmin1, doublereal *dmin2, doublereal *dn, doublereal *dn1, doublereal *dn2, doublereal *g, doublereal *tau);
/* dlasq4.c */
int dlasq4_(integer *i0, integer *n0, doublereal *z__, integer *pp, integer *n0in, doublereal *dmin__, doublereal *dmin1, doublereal *dmin2, doublereal *dn, doublereal *dn1, doublereal *dn2, doublereal *tau, integer *ttype, doublereal *g);
/* dlasq5.c */
int dlasq5_(integer *i0, integer *n0, doublereal *z__, integer *pp, doublereal *tau, doublereal *sigma, doublereal *dmin__, doublereal *dmin1, doublereal *dmin2, doublereal *dn, doublereal *dnm1, doublereal *dnm2, logical *ieee, doublereal *eps);
/* dlasq6.c */
int dlasq6_(integer *i0, integer *n0, doublereal *z__, integer *pp, doublereal *dmin__, doublereal *dmin1, doublereal *dmin2, doublereal *dn, doublereal *dnm1, doublereal *dnm2);
/* dlasr.c */
int dlasr_(char *side, char *pivot, char *direct, integer *m, integer *n, doublereal *c__, doublereal *s, doublereal *a, integer *lda, ftnlen side_len, ftnlen pivot_len, ftnlen direct_len);
/* dlasrt.c */
int dlasrt_(char *id, integer *n, doublereal *d__, integer *info, ftnlen id_len);
/* dlassq.c */
int dlassq_(integer *n, doublereal *x, integer *incx, doublereal *scale, doublereal *sumsq);
/* dlasv2.c */
int dlasv2_(doublereal *f, doublereal *g, doublereal *h__, doublereal *ssmin, doublereal *ssmax, doublereal *snr, doublereal *csr, doublereal *snl, doublereal *csl);
/* dlaswlq.c */
int dlaswlq_(integer *m, integer *n, integer *mb, integer *nb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *work, integer *lwork, integer *info);
/* dlaswp.c */
int dlaswp_(integer *n, doublereal *a, integer *lda, integer *k1, integer *k2, integer *ipiv, integer *incx);
/* dlasy2.c */
int dlasy2_(logical *ltranl, logical *ltranr, integer *isgn, integer *n1, integer *n2, doublereal *tl, integer *ldtl, doublereal *tr, integer *ldtr, doublereal *b, integer *ldb, doublereal *scale, doublereal *x, integer *ldx, doublereal *xnorm, integer *info);
/* dla_syamv.c */
int dla_syamv__(integer *uplo, integer *n, doublereal *alpha, doublereal *a, integer *lda, doublereal *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* dlasyf_aa.c */
int dlasyf_aa__(char *uplo, integer *j1, integer *m, integer *nb, doublereal *a, integer *lda, integer *ipiv, doublereal *h__, integer *ldh, doublereal *work, ftnlen uplo_len);
/* dlasyf.c */
int dlasyf_(char *uplo, integer *n, integer *nb, integer *kb, doublereal *a, integer *lda, integer *ipiv, doublereal *w, integer *ldw, integer *info, ftnlen uplo_len);
/* dlasyf_rk.c */
int dlasyf_rk__(char *uplo, integer *n, integer *nb, integer *kb, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *w, integer *ldw, integer *info, ftnlen uplo_len);
/* dlasyf_rook.c */
int dlasyf_rook__(char *uplo, integer *n, integer *nb, integer *kb, doublereal *a, integer *lda, integer *ipiv, doublereal *w, integer *ldw, integer *info, ftnlen uplo_len);
/* dla_syrcond.c */
doublereal dla_syrcond__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, integer *cmode, doublereal *c__, integer *info, doublereal *work, integer *iwork, ftnlen uplo_len);
/* dla_syrfsx_extended.c */
int dla_syrfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, logical *colequ, doublereal *c__, doublereal *b, integer *ldb, doublereal *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublereal *res, doublereal *ayb, doublereal *dy, doublereal *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* dla_syrpvgrw.c */
doublereal dla_syrpvgrw__(char *uplo, integer *n, integer *info, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, doublereal *work, ftnlen uplo_len);
/* dlat2s.c */
int dlat2s_(char *uplo, integer *n, doublereal *a, integer *lda, real *sa, integer *ldsa, integer *info, ftnlen uplo_len);
/* dlatbs.c */
int dlatbs_(char *uplo, char *trans, char *diag, char *normin, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* dlatdf.c */
int dlatdf_(integer *ijob, integer *n, doublereal *z__, integer *ldz, doublereal *rhs, doublereal *rdsum, doublereal *rdscal, integer *ipiv, integer *jpiv);
/* dlatps.c */
int dlatps_(char *uplo, char *trans, char *diag, char *normin, integer *n, doublereal *ap, doublereal *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* dlatrd.c */
int dlatrd_(char *uplo, integer *n, integer *nb, doublereal *a, integer *lda, doublereal *e, doublereal *tau, doublereal *w, integer *ldw, ftnlen uplo_len);
/* dlatrs.c */
int dlatrs_(char *uplo, char *trans, char *diag, char *normin, integer *n, doublereal *a, integer *lda, doublereal *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* dlatrz.c */
int dlatrz_(integer *m, integer *n, integer *l, doublereal *a, integer *lda, doublereal *tau, doublereal *work);
/* dlatsqr.c */
int dlatsqr_(integer *m, integer *n, integer *mb, integer *nb, doublereal *a, integer *lda, doublereal *t, integer *ldt, doublereal *work, integer *lwork, integer *info);
/* dlauu2.c */
int dlauu2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dlauum.c */
int dlauum_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dla_wwaddw.c */
int dla_wwaddw__(integer *n, doublereal *x, doublereal *y, doublereal *w);
/* dopgtr.c */
int dopgtr_(char *uplo, integer *n, doublereal *ap, doublereal *tau, doublereal *q, integer *ldq, doublereal *work, integer *info, ftnlen uplo_len);
/* dopmtr.c */
int dopmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, doublereal *ap, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* dorbdb1.c */
int dorbdb1_(integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublereal *taup1, doublereal *taup2, doublereal *tauq1, doublereal *work, integer *lwork, integer *info);
/* dorbdb2.c */
int dorbdb2_(integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublereal *taup1, doublereal *taup2, doublereal *tauq1, doublereal *work, integer *lwork, integer *info);
/* dorbdb3.c */
int dorbdb3_(integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublereal *taup1, doublereal *taup2, doublereal *tauq1, doublereal *work, integer *lwork, integer *info);
/* dorbdb4.c */
int dorbdb4_(integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublereal *taup1, doublereal *taup2, doublereal *tauq1, doublereal *phantom, doublereal *work, integer *lwork, integer *info);
/* dorbdb5.c */
int dorbdb5_(integer *m1, integer *m2, integer *n, doublereal *x1, integer *incx1, doublereal *x2, integer *incx2, doublereal *q1, integer *ldq1, doublereal *q2, integer *ldq2, doublereal *work, integer *lwork, integer *info);
/* dorbdb6.c */
int dorbdb6_(integer *m1, integer *m2, integer *n, doublereal *x1, integer *incx1, doublereal *x2, integer *incx2, doublereal *q1, integer *ldq1, doublereal *q2, integer *ldq2, doublereal *work, integer *lwork, integer *info);
/* dorbdb.c */
int dorbdb_(char *trans, char *signs, integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x12, integer *ldx12, doublereal *x21, integer *ldx21, doublereal *x22, integer *ldx22, doublereal *theta, doublereal *phi, doublereal *taup1, doublereal *taup2, doublereal *tauq1, doublereal *tauq2, doublereal *work, integer *lwork, integer *info, ftnlen trans_len, ftnlen signs_len);
/* dorcsd2by1.c */
int dorcsd2by1_(char *jobu1, char *jobu2, char *jobv1t, integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x21, integer *ldx21, doublereal *theta, doublereal *u1, integer *ldu1, doublereal *u2, integer *ldu2, doublereal *v1t, integer *ldv1t, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len);
/* dorcsd.c */
int dorcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, char *signs, integer *m, integer *p, integer *q, doublereal *x11, integer *ldx11, doublereal *x12, integer *ldx12, doublereal *x21, integer *ldx21, doublereal *x22, integer *ldx22, doublereal *theta, doublereal *u1, integer *ldu1, doublereal *u2, integer *ldu2, doublereal *v1t, integer *ldv1t, doublereal *v2t, integer *ldv2t, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len, ftnlen signs_len);
/* dorg2l.c */
int dorg2l_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dorg2r.c */
int dorg2r_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dorgbr.c */
int dorgbr_(char *vect, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info, ftnlen vect_len);
/* dorghr.c */
int dorghr_(integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dorgl2.c */
int dorgl2_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dorglq.c */
int dorglq_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dorgql.c */
int dorgql_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dorgqr.c */
int dorgqr_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dorgr2.c */
int dorgr2_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *info);
/* dorgrq.c */
int dorgrq_(integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dorgtr.c */
int dorgtr_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dorm22.c */
int dorm22_(char *side, char *trans, integer *m, integer *n, integer *n1, integer *n2, doublereal *q, integer *ldq, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dorm2l.c */
int dorm2l_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dorm2r.c */
int dorm2r_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormbr.c */
int dormbr_(char *vect, char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen side_len, ftnlen trans_len);
/* dormhr.c */
int dormhr_(char *side, char *trans, integer *m, integer *n, integer *ilo, integer *ihi, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dorml2.c */
int dorml2_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormlq.c */
int dormlq_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormql.c */
int dormql_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormqr.c */
int dormqr_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormr2.c */
int dormr2_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormr3.c */
int dormr3_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormrq.c */
int dormrq_(char *side, char *trans, integer *m, integer *n, integer *k, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormrz.c */
int dormrz_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* dormtr.c */
int dormtr_(char *side, char *uplo, char *trans, integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *c__, integer *ldc, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* dpbcon.c */
int dpbcon_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dpbequ.c */
int dpbequ_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *s, doublereal *scond, doublereal *amax, integer *info, ftnlen uplo_len);
/* dpbrfs.c */
int dpbrfs_(char *uplo, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dpbstf.c */
int dpbstf_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* dpbsv.c */
int dpbsv_(char *uplo, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dpbsvx.c */
int dpbsvx_(char *fact, char *uplo, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *afb, integer *ldafb, char *equed, doublereal *s, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* dpbtf2.c */
int dpbtf2_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* dpbtrf.c */
int dpbtrf_(char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* dpbtrs.c */
int dpbtrs_(char *uplo, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dpftrf.c */
int dpftrf_(char *transr, char *uplo, integer *n, doublereal *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dpftri.c */
int dpftri_(char *transr, char *uplo, integer *n, doublereal *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dpftrs.c */
int dpftrs_(char *transr, char *uplo, integer *n, integer *nrhs, doublereal *a, doublereal *b, integer *ldb, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dpocon.c */
int dpocon_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dpoequb.c */
int dpoequb_(integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, integer *info);
/* dpoequ.c */
int dpoequ_(integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, integer *info);
/* dporfs.c */
int dporfs_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dposv.c */
int dposv_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dposvx.c */
int dposvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, char *equed, doublereal *s, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* dposvxx.c */
int dposvxx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, char *equed, doublereal *s, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* dpotf2.c */
int dpotf2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dpotrf2.c */
int dpotrf2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dpotrf.c */
int dpotrf_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dpotri.c */
int dpotri_(char *uplo, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dpotrs.c */
int dpotrs_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dppcon.c */
int dppcon_(char *uplo, integer *n, doublereal *ap, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dppequ.c */
int dppequ_(char *uplo, integer *n, doublereal *ap, doublereal *s, doublereal *scond, doublereal *amax, integer *info, ftnlen uplo_len);
/* dpprfs.c */
int dpprfs_(char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *afp, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dppsv.c */
int dppsv_(char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dppsvx.c */
int dppsvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *afp, char *equed, doublereal *s, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* dpptrf.c */
int dpptrf_(char *uplo, integer *n, doublereal *ap, integer *info, ftnlen uplo_len);
/* dpptri.c */
int dpptri_(char *uplo, integer *n, doublereal *ap, integer *info, ftnlen uplo_len);
/* dpptrs.c */
int dpptrs_(char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dpstf2.c */
int dpstf2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *piv, integer *rank, doublereal *tol, doublereal *work, integer *info, ftnlen uplo_len);
/* dpstrf.c */
int dpstrf_(char *uplo, integer *n, doublereal *a, integer *lda, integer *piv, integer *rank, doublereal *tol, doublereal *work, integer *info, ftnlen uplo_len);
/* dptcon.c */
int dptcon_(integer *n, doublereal *d__, doublereal *e, doublereal *anorm, doublereal *rcond, doublereal *work, integer *info);
/* dpteqr.c */
int dpteqr_(char *compz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen compz_len);
/* dptrfs.c */
int dptrfs_(integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *df, doublereal *ef, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *info);
/* dptsv.c */
int dptsv_(integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *b, integer *ldb, integer *info);
/* dptsvx.c */
int dptsvx_(char *fact, integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *df, doublereal *ef, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *info, ftnlen fact_len);
/* dpttrf.c */
int dpttrf_(integer *n, doublereal *d__, doublereal *e, integer *info);
/* dpttrs.c */
int dpttrs_(integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *b, integer *ldb, integer *info);
/* dptts2.c */
int dptts2_(integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublereal *b, integer *ldb);
/* drscl.c */
int drscl_(integer *n, doublereal *sa, doublereal *sx, integer *incx);
/* dsb2st_kernels.c */
int dsb2st_kernels__(char *uplo, logical *wantz, integer *ttype, integer *st, integer *ed, integer *sweep, integer *n, integer *nb, integer *ib, doublereal *a, integer *lda, doublereal *v, doublereal *tau, integer *ldvt, doublereal *work, ftnlen uplo_len);
/* dsbev_2stage.c */
int dsbev_2stage__(char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbev.c */
int dsbev_(char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbevd_2stage.c */
int dsbevd_2stage__(char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbevd.c */
int dsbevd_(char *jobz, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbevx_2stage.c */
int dsbevx_2stage__(char *jobz, char *range, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsbevx.c */
int dsbevx_(char *jobz, char *range, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsbgst.c */
int dsbgst_(char *vect, char *uplo, integer *n, integer *ka, integer *kb, doublereal *ab, integer *ldab, doublereal *bb, integer *ldbb, doublereal *x, integer *ldx, doublereal *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* dsbgv.c */
int dsbgv_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, doublereal *ab, integer *ldab, doublereal *bb, integer *ldbb, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbgvd.c */
int dsbgvd_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, doublereal *ab, integer *ldab, doublereal *bb, integer *ldbb, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsbgvx.c */
int dsbgvx_(char *jobz, char *range, char *uplo, integer *n, integer *ka, integer *kb, doublereal *ab, integer *ldab, doublereal *bb, integer *ldbb, doublereal *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsbtrd.c */
int dsbtrd_(char *vect, char *uplo, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *d__, doublereal *e, doublereal *q, integer *ldq, doublereal *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* dsfrk.c */
int dsfrk_(char *transr, char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublereal *a, integer *lda, doublereal *beta, doublereal *c__, ftnlen transr_len, ftnlen uplo_len, ftnlen trans_len);
/* dsgesv.c */
int dsgesv_(integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *work, real *swork, integer *iter, integer *info);
/* dspcon.c */
int dspcon_(char *uplo, integer *n, doublereal *ap, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dspev.c */
int dspev_(char *jobz, char *uplo, integer *n, doublereal *ap, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dspevd.c */
int dspevd_(char *jobz, char *uplo, integer *n, doublereal *ap, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dspevx.c */
int dspevx_(char *jobz, char *range, char *uplo, integer *n, doublereal *ap, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dspgst.c */
int dspgst_(integer *itype, char *uplo, integer *n, doublereal *ap, doublereal *bp, integer *info, ftnlen uplo_len);
/* dspgv.c */
int dspgv_(integer *itype, char *jobz, char *uplo, integer *n, doublereal *ap, doublereal *bp, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dspgvd.c */
int dspgvd_(integer *itype, char *jobz, char *uplo, integer *n, doublereal *ap, doublereal *bp, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dspgvx.c */
int dspgvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, doublereal *ap, doublereal *bp, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsposv.c */
int dsposv_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *work, real *swork, integer *iter, integer *info, ftnlen uplo_len);
/* dsprfs.c */
int dsprfs_(char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *afp, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dspsv.c */
int dspsv_(char *uplo, integer *n, integer *nrhs, doublereal *ap, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dspsvx.c */
int dspsvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *ap, doublereal *afp, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* dsptrd.c */
int dsptrd_(char *uplo, integer *n, doublereal *ap, doublereal *d__, doublereal *e, doublereal *tau, integer *info, ftnlen uplo_len);
/* dsptrf.c */
int dsptrf_(char *uplo, integer *n, doublereal *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* dsptri.c */
int dsptri_(char *uplo, integer *n, doublereal *ap, integer *ipiv, doublereal *work, integer *info, ftnlen uplo_len);
/* dsptrs.c */
int dsptrs_(char *uplo, integer *n, integer *nrhs, doublereal *ap, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dstebz.c */
int dstebz_(char *range, char *order, integer *n, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, doublereal *d__, doublereal *e, integer *m, integer *nsplit, doublereal *w, integer *iblock, integer *isplit, doublereal *work, integer *iwork, integer *info, ftnlen range_len, ftnlen order_len);
/* dstedc.c */
int dstedc_(char *compz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen compz_len);
/* dstegr.c */
int dstegr_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* dstein.c */
int dstein_(integer *n, doublereal *d__, doublereal *e, integer *m, doublereal *w, integer *iblock, integer *isplit, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info);
/* dstemr.c */
int dstemr_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, integer *m, doublereal *w, doublereal *z__, integer *ldz, integer *nzc, integer *isuppz, logical *tryrac, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* dsteqr.c */
int dsteqr_(char *compz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen compz_len);
/* dsterf.c */
int dsterf_(integer *n, doublereal *d__, doublereal *e, integer *info);
/* dstev.c */
int dstev_(char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *info, ftnlen jobz_len);
/* dstevd.c */
int dstevd_(char *jobz, integer *n, doublereal *d__, doublereal *e, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len);
/* dstevr.c */
int dstevr_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* dstevx.c */
int dstevx_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len);
/* dsycon_3.c */
int dsycon_3__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dsycon.c */
int dsycon_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dsycon_rook.c */
int dsycon_rook__(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dsyconv.c */
int dsyconv_(char *uplo, char *way, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *e, integer *info, ftnlen uplo_len, ftnlen way_len);
/* dsyconvf.c */
int dsyconvf_(char *uplo, char *way, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* dsyconvf_rook.c */
int dsyconvf_rook__(char *uplo, char *way, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* dsyequb.c */
int dsyequb_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, doublereal *work, integer *info, ftnlen uplo_len);
/* dsyev_2stage.c */
int dsyev_2stage__(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsyev.c */
int dsyev_(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsyevd_2stage.c */
int dsyevd_2stage__(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsyevd.c */
int dsyevd_(char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *w, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsyevr_2stage.c */
int dsyevr_2stage__(char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsyevr.c */
int dsyevr_(char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, integer *isuppz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsyevx_2stage.c */
int dsyevx_2stage__(char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsyevx.c */
int dsyevx_(char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsygs2.c */
int dsygs2_(integer *itype, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dsygst.c */
int dsygst_(integer *itype, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dsygv_2stage.c */
int dsygv_2stage__(integer *itype, char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *w, doublereal *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsygv.c */
int dsygv_(integer *itype, char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *w, doublereal *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsygvd.c */
int dsygvd_(integer *itype, char *jobz, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *w, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* dsygvx.c */
int dsygvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* dsyrfs.c */
int dsyrfs_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len);
/* dsysv_aa_2stage.c */
int dsysv_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsysv_aa.c */
int dsysv_aa__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsysv.c */
int dsysv_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsysv_rk.c */
int dsysv_rk__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsysv_rook.c */
int dsysv_rook__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsysvx.c */
int dsysvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* dsysvxx.c */
int dsysvxx_(char *fact, char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *af, integer *ldaf, integer *ipiv, char *equed, doublereal *s, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublereal *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* dsyswapr.c */
int dsyswapr_(char *uplo, integer *n, doublereal *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* dsytd2.c */
int dsytd2_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tau, integer *info, ftnlen uplo_len);
/* dsytf2.c */
int dsytf2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* dsytf2_rk.c */
int dsytf2_rk__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* dsytf2_rook.c */
int dsytf2_rook__(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* dsytrd_2stage.c */
int dsytrd_2stage__(char *vect, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tau, doublereal *hous2, integer *lhous2, doublereal *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* dsytrd.c */
int dsytrd_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *d__, doublereal *e, doublereal *tau, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrd_sy2sb.c */
int dsytrd_sy2sb__(char *uplo, integer *n, integer *kd, doublereal *a, integer *lda, doublereal *ab, integer *ldab, doublereal *tau, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrf_aa_2stage.c */
int dsytrf_aa_2stage__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrf_aa.c */
int dsytrf_aa__(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrf.c */
int dsytrf_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrf_rk.c */
int dsytrf_rk__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrf_rook.c */
int dsytrf_rook__(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytri2.c */
int dsytri2_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytri2x.c */
int dsytri2x_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *nb, integer *info, ftnlen uplo_len);
/* dsytri_3.c */
int dsytri_3__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytri_3x.c */
int dsytri_3x__(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *work, integer *nb, integer *info, ftnlen uplo_len);
/* dsytri.c */
int dsytri_(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *info, ftnlen uplo_len);
/* dsytri_rook.c */
int dsytri_rook__(char *uplo, integer *n, doublereal *a, integer *lda, integer *ipiv, doublereal *work, integer *info, ftnlen uplo_len);
/* dsytrs2.c */
int dsytrs2_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *info, ftnlen uplo_len);
/* dsytrs_3.c */
int dsytrs_3__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *e, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dsytrs_aa_2stage.c */
int dsytrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dsytrs_aa.c */
int dsytrs_aa__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, doublereal *work, integer *lwork, integer *info, ftnlen uplo_len);
/* dsytrs.c */
int dsytrs_(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dsytrs_rook.c */
int dsytrs_rook__(char *uplo, integer *n, integer *nrhs, doublereal *a, integer *lda, integer *ipiv, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len);
/* dtbcon.c */
int dtbcon_(char *norm, char *uplo, char *diag, integer *n, integer *kd, doublereal *ab, integer *ldab, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dtbrfs.c */
int dtbrfs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtbtrs.c */
int dtbtrs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, doublereal *ab, integer *ldab, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtfsm.c */
int dtfsm_(char *transr, char *side, char *uplo, char *trans, char *diag, integer *m, integer *n, doublereal *alpha, doublereal *a, doublereal *b, integer *ldb, ftnlen transr_len, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtftri.c */
int dtftri_(char *transr, char *uplo, char *diag, integer *n, doublereal *a, integer *info, ftnlen transr_len, ftnlen uplo_len, ftnlen diag_len);
/* dtfttp.c */
int dtfttp_(char *transr, char *uplo, integer *n, doublereal *arf, doublereal *ap, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dtfttr.c */
int dtfttr_(char *transr, char *uplo, integer *n, doublereal *arf, doublereal *a, integer *lda, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dtgevc.c */
int dtgevc_(char *side, char *howmny, logical *select, integer *n, doublereal *s, integer *lds, doublereal *p, integer *ldp, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *mm, integer *m, doublereal *work, integer *info, ftnlen side_len, ftnlen howmny_len);
/* dtgex2.c */
int dtgex2_(logical *wantq, logical *wantz, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, integer *j1, integer *n1, integer *n2, doublereal *work, integer *lwork, integer *info);
/* dtgexc.c */
int dtgexc_(logical *wantq, logical *wantz, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, integer *ifst, integer *ilst, doublereal *work, integer *lwork, integer *info);
/* dtgsen.c */
int dtgsen_(integer *ijob, logical *wantq, logical *wantz, logical *select, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *alphar, doublereal *alphai, doublereal *beta, doublereal *q, integer *ldq, doublereal *z__, integer *ldz, integer *m, doublereal *pl, doublereal *pr, doublereal *dif, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info);
/* dtgsja.c */
int dtgsja_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, integer *k, integer *l, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *tola, doublereal *tolb, doublereal *alpha, doublereal *beta, doublereal *u, integer *ldu, doublereal *v, integer *ldv, doublereal *q, integer *ldq, doublereal *work, integer *ncycle, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* dtgsna.c */
int dtgsna_(char *job, char *howmny, logical *select, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *s, doublereal *dif, integer *mm, integer *m, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* dtgsy2.c */
int dtgsy2_(char *trans, integer *ijob, integer *m, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *c__, integer *ldc, doublereal *d__, integer *ldd, doublereal *e, integer *lde, doublereal *f, integer *ldf, doublereal *scale, doublereal *rdsum, doublereal *rdscal, integer *iwork, integer *pq, integer *info, ftnlen trans_len);
/* dtgsyl.c */
int dtgsyl_(char *trans, integer *ijob, integer *m, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *c__, integer *ldc, doublereal *d__, integer *ldd, doublereal *e, integer *lde, doublereal *f, integer *ldf, doublereal *scale, doublereal *dif, doublereal *work, integer *lwork, integer *iwork, integer *info, ftnlen trans_len);
/* dtpcon.c */
int dtpcon_(char *norm, char *uplo, char *diag, integer *n, doublereal *ap, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dtplqt2.c */
int dtplqt2_(integer *m, integer *n, integer *l, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *t, integer *ldt, integer *info);
/* dtplqt.c */
int dtplqt_(integer *m, integer *n, integer *l, integer *mb, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *t, integer *ldt, doublereal *work, integer *info);
/* dtpmlqt.c */
int dtpmlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *mb, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dtpmqrt.c */
int dtpmqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *nb, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* dtpqrt2.c */
int dtpqrt2_(integer *m, integer *n, integer *l, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *t, integer *ldt, integer *info);
/* dtpqrt.c */
int dtpqrt_(integer *m, integer *n, integer *l, integer *nb, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *t, integer *ldt, doublereal *work, integer *info);
/* dtprfb.c */
int dtprfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, doublereal *v, integer *ldv, doublereal *t, integer *ldt, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* dtprfs.c */
int dtprfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublereal *ap, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtptri.c */
int dtptri_(char *uplo, char *diag, integer *n, doublereal *ap, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* dtptrs.c */
int dtptrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublereal *ap, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtpttf.c */
int dtpttf_(char *transr, char *uplo, integer *n, doublereal *ap, doublereal *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dtpttr.c */
int dtpttr_(char *uplo, integer *n, doublereal *ap, doublereal *a, integer *lda, integer *info, ftnlen uplo_len);
/* dtrcon.c */
int dtrcon_(char *norm, char *uplo, char *diag, integer *n, doublereal *a, integer *lda, doublereal *rcond, doublereal *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* dtrevc3.c */
int dtrevc3_(char *side, char *howmny, logical *select, integer *n, doublereal *t, integer *ldt, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *mm, integer *m, doublereal *work, integer *lwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* dtrevc.c */
int dtrevc_(char *side, char *howmny, logical *select, integer *n, doublereal *t, integer *ldt, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, integer *mm, integer *m, doublereal *work, integer *info, ftnlen side_len, ftnlen howmny_len);
/* dtrexc.c */
int dtrexc_(char *compq, integer *n, doublereal *t, integer *ldt, doublereal *q, integer *ldq, integer *ifst, integer *ilst, doublereal *work, integer *info, ftnlen compq_len);
/* dtrrfs.c */
int dtrrfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *x, integer *ldx, doublereal *ferr, doublereal *berr, doublereal *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtrsen.c */
int dtrsen_(char *job, char *compq, logical *select, integer *n, doublereal *t, integer *ldt, doublereal *q, integer *ldq, doublereal *wr, doublereal *wi, integer *m, doublereal *s, doublereal *sep, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen job_len, ftnlen compq_len);
/* dtrsna.c */
int dtrsna_(char *job, char *howmny, logical *select, integer *n, doublereal *t, integer *ldt, doublereal *vl, integer *ldvl, doublereal *vr, integer *ldvr, doublereal *s, doublereal *sep, integer *mm, integer *m, doublereal *work, integer *ldwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* dtrsyl.c */
int dtrsyl_(char *trana, char *tranb, integer *isgn, integer *m, integer *n, doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *c__, integer *ldc, doublereal *scale, integer *info, ftnlen trana_len, ftnlen tranb_len);
/* dtrti2.c */
int dtrti2_(char *uplo, char *diag, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* dtrtri.c */
int dtrtri_(char *uplo, char *diag, integer *n, doublereal *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* dtrtrs.c */
int dtrtrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublereal *a, integer *lda, doublereal *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* dtrttf.c */
int dtrttf_(char *transr, char *uplo, integer *n, doublereal *a, integer *lda, doublereal *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* dtrttp.c */
int dtrttp_(char *uplo, integer *n, doublereal *a, integer *lda, doublereal *ap, integer *info, ftnlen uplo_len);
/* dtzrzf.c */
int dtzrzf_(integer *m, integer *n, doublereal *a, integer *lda, doublereal *tau, doublereal *work, integer *lwork, integer *info);
/* dzsum1.c */
doublereal dzsum1_(integer *n, doublecomplex *cx, integer *incx);
/* icmax1.c */
integer icmax1_(integer *n, complex *cx, integer *incx);
/* ieeeck.c */
integer ieeeck_(integer *ispec, real *zero, real *one);
/* ilaclc.c */
integer ilaclc_(integer *m, integer *n, complex *a, integer *lda);
/* ilaclr.c */
integer ilaclr_(integer *m, integer *n, complex *a, integer *lda);
/* iladiag.c */
integer iladiag_(char *diag, ftnlen diag_len);
/* iladlc.c */
integer iladlc_(integer *m, integer *n, doublereal *a, integer *lda);
/* iladlr.c */
integer iladlr_(integer *m, integer *n, doublereal *a, integer *lda);
/* ilaenv2stage.c */
integer ilaenv2stage_(integer *ispec, char *name__, char *opts, integer *n1, integer *n2, integer *n3, integer *n4, ftnlen name_len, ftnlen opts_len);
/* ilaenv.c */
integer ilaenv_(integer *ispec, char *name__, char *opts, integer *n1, integer *n2, integer *n3, integer *n4, ftnlen name_len, ftnlen opts_len);
/* ilaprec.c */
integer ilaprec_(char *prec, ftnlen prec_len);
/* ilaslc.c */
integer ilaslc_(integer *m, integer *n, real *a, integer *lda);
/* ilaslr.c */
integer ilaslr_(integer *m, integer *n, real *a, integer *lda);
/* ilatrans.c */
integer ilatrans_(char *trans, ftnlen trans_len);
/* ilauplo.c */
integer ilauplo_(char *uplo, ftnlen uplo_len);
/* ilazlc.c */
integer ilazlc_(integer *m, integer *n, doublecomplex *a, integer *lda);
/* ilazlr.c */
integer ilazlr_(integer *m, integer *n, doublecomplex *a, integer *lda);
/* iparmq.c */
integer iparmq_(integer *ispec, char *name__, char *opts, integer *n, integer *ilo, integer *ihi, integer *lwork, ftnlen name_len, ftnlen opts_len);
/* izmax1.c */
integer izmax1_(integer *n, doublecomplex *zx, integer *incx);
/* lsamen.c */
logical lsamen_(integer *n, char *ca, char *cb, ftnlen ca_len, ftnlen cb_len);
/* sbbcsd.c */
int sbbcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, integer *m, integer *p, integer *q, real *theta, real *phi, real *u1, integer *ldu1, real *u2, integer *ldu2, real *v1t, integer *ldv1t, real *v2t, integer *ldv2t, real *b11d, real *b11e, real *b12d, real *b12e, real *b21d, real *b21e, real *b22d, real *b22e, real *work, integer *lwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len);
/* sbdsdc.c */
int sbdsdc_(char *uplo, char *compq, integer *n, real *d__, real *e, real *u, integer *ldu, real *vt, integer *ldvt, real *q, integer *iq, real *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen compq_len);
/* sbdsqr.c */
int sbdsqr_(char *uplo, integer *n, integer *ncvt, integer *nru, integer *ncc, real *d__, real *e, real *vt, integer *ldvt, real *u, integer *ldu, real *c__, integer *ldc, real *work, integer *info, ftnlen uplo_len);
/* scsum1.c */
doublereal scsum1_(integer *n, complex *cx, integer *incx);
/* sdisna.c */
int sdisna_(char *job, integer *m, integer *n, real *d__, real *sep, integer *info, ftnlen job_len);
/* sgbbrd.c */
int sgbbrd_(char *vect, integer *m, integer *n, integer *ncc, integer *kl, integer *ku, real *ab, integer *ldab, real *d__, real *e, real *q, integer *ldq, real *pt, integer *ldpt, real *c__, integer *ldc, real *work, integer *info, ftnlen vect_len);
/* sgbcon.c */
int sgbcon_(char *norm, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len);
/* sgbequb.c */
int sgbequb_(integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* sgbequ.c */
int sgbequ_(integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* sgbrfs.c */
int sgbrfs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen trans_len);
/* sgbsv.c */
int sgbsv_(integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, integer *ipiv, real *b, integer *ldb, integer *info);
/* sgbsvx.c */
int sgbsvx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, integer *ipiv, char *equed, real *r__, real *c__, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* sgbsvxx.c */
int sgbsvxx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, integer *ipiv, char *equed, real *r__, real *c__, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* sgbtf2.c */
int sgbtf2_(integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, integer *ipiv, integer *info);
/* sgbtrf.c */
int sgbtrf_(integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, integer *ipiv, integer *info);
/* sgbtrs.c */
int sgbtrs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen trans_len);
/* sgebak.c */
int sgebak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, real *scale, integer *m, real *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* sgebal.c */
int sgebal_(char *job, integer *n, real *a, integer *lda, integer *ilo, integer *ihi, real *scale, integer *info, ftnlen job_len);
/* sgebd2.c */
int sgebd2_(integer *m, integer *n, real *a, integer *lda, real *d__, real *e, real *tauq, real *taup, real *work, integer *info);
/* sgebrd.c */
int sgebrd_(integer *m, integer *n, real *a, integer *lda, real *d__, real *e, real *tauq, real *taup, real *work, integer *lwork, integer *info);
/* sgecon.c */
int sgecon_(char *norm, integer *n, real *a, integer *lda, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len);
/* sgeequb.c */
int sgeequb_(integer *m, integer *n, real *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* sgeequ.c */
int sgeequ_(integer *m, integer *n, real *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, integer *info);
/* sgees.c */
int sgees_(char *jobvs, char *sort, L_fp select, integer *n, real *a, integer *lda, integer *sdim, real *wr, real *wi, real *vs, integer *ldvs, real *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len);
/* sgeesx.c */
int sgeesx_(char *jobvs, char *sort, L_fp select, char *sense, integer *n, real *a, integer *lda, integer *sdim, real *wr, real *wi, real *vs, integer *ldvs, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len, ftnlen sense_len);
/* sgeev.c */
int sgeev_(char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* sgeevx.c */
int sgeevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, real *a, integer *lda, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *ilo, integer *ihi, real *scale, real *abnrm, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* sgehd2.c */
int sgehd2_(integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgehrd.c */
int sgehrd_(integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgejsv.c */
int sgejsv_(char *joba, char *jobu, char *jobv, char *jobr, char *jobt, char *jobp, integer *m, integer *n, real *a, integer *lda, real *sva, real *u, integer *ldu, real *v, integer *ldv, real *work, integer *lwork, integer *iwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobr_len, ftnlen jobt_len, ftnlen jobp_len);
/* sgelq2.c */
int sgelq2_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgelq.c */
int sgelq_(integer *m, integer *n, real *a, integer *lda, real *t, integer *tsize, real *work, integer *lwork, integer *info);
/* sgelqf.c */
int sgelqf_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgelqt.c */
int sgelqt_(integer *m, integer *n, integer *mb, real *a, integer *lda, real *t, integer *ldt, real *work, integer *info);
/* sgels.c */
int sgels_(char *trans, integer *m, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen trans_len);
/* sgelsd.c */
int sgelsd_(integer *m, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, real *s, real *rcond, integer *rank, real *work, integer *lwork, integer *iwork, integer *info);
/* sgelss.c */
int sgelss_(integer *m, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, real *s, real *rcond, integer *rank, real *work, integer *lwork, integer *info);
/* sgelsy.c */
int sgelsy_(integer *m, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, integer *jpvt, real *rcond, integer *rank, real *work, integer *lwork, integer *info);
/* sgemlq.c */
int sgemlq_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *t, integer *tsize, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sgemlqt.c */
int sgemlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, real *v, integer *ldv, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sgemqr.c */
int sgemqr_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *t, integer *tsize, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sgemqrt.c */
int sgemqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *nb, real *v, integer *ldv, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sgeql2.c */
int sgeql2_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgeqlf.c */
int sgeqlf_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgeqp3.c */
int sgeqp3_(integer *m, integer *n, real *a, integer *lda, integer *jpvt, real *tau, real *work, integer *lwork, integer *info);
/* sgeqr2.c */
int sgeqr2_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgeqr2p.c */
int sgeqr2p_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgeqr.c */
int sgeqr_(integer *m, integer *n, real *a, integer *lda, real *t, integer *tsize, real *work, integer *lwork, integer *info);
/* sgeqrf.c */
int sgeqrf_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgeqrfp.c */
int sgeqrfp_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgeqrt2.c */
int sgeqrt2_(integer *m, integer *n, real *a, integer *lda, real *t, integer *ldt, integer *info);
/* sgeqrt3.c */
int sgeqrt3_(integer *m, integer *n, real *a, integer *lda, real *t, integer *ldt, integer *info);
/* sgeqrt.c */
int sgeqrt_(integer *m, integer *n, integer *nb, real *a, integer *lda, real *t, integer *ldt, real *work, integer *info);
/* sgerfs.c */
int sgerfs_(char *trans, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen trans_len);
/* sgerq2.c */
int sgerq2_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *info);
/* sgerqf.c */
int sgerqf_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sgesc2.c */
int sgesc2_(integer *n, real *a, integer *lda, real *rhs, integer *ipiv, integer *jpiv, real *scale);
/* sgesdd.c */
int sgesdd_(char *jobz, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *iwork, integer *info, ftnlen jobz_len);
/* sgesv.c */
int sgesv_(integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info);
/* sgesvd.c */
int sgesvd_(char *jobu, char *jobvt, integer *m, integer *n, real *a, integer *lda, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len);
/* sgesvdx.c */
int sgesvdx_(char *jobu, char *jobvt, char *range, integer *m, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, integer *ns, real *s, real *u, integer *ldu, real *vt, integer *ldvt, real *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len, ftnlen range_len);
/* sgesvj.c */
int sgesvj_(char *joba, char *jobu, char *jobv, integer *m, integer *n, real *a, integer *lda, real *sva, integer *mv, real *v, integer *ldv, real *work, integer *lwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len);
/* sgesvx.c */
int sgesvx_(char *fact, char *trans, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, char *equed, real *r__, real *c__, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* sgesvxx.c */
int sgesvxx_(char *fact, char *trans, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, char *equed, real *r__, real *c__, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* sgetc2.c */
int sgetc2_(integer *n, real *a, integer *lda, integer *ipiv, integer *jpiv, integer *info);
/* sgetf2.c */
int sgetf2_(integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info);
/* sgetrf2.c */
int sgetrf2_(integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info);
/* sgetrf.c */
int sgetrf_(integer *m, integer *n, real *a, integer *lda, integer *ipiv, integer *info);
/* sgetri.c */
int sgetri_(integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info);
/* sgetrs.c */
int sgetrs_(char *trans, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen trans_len);
/* sgetsls.c */
int sgetsls_(char *trans, integer *m, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen trans_len);
/* sggbak.c */
int sggbak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, real *lscale, real *rscale, integer *m, real *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* sggbal.c */
int sggbal_(char *job, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *ilo, integer *ihi, real *lscale, real *rscale, real *work, integer *info, ftnlen job_len);
/* sgges3.c */
int sgges3_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *sdim, real *alphar, real *alphai, real *beta, real *vsl, integer *ldvsl, real *vsr, integer *ldvsr, real *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* sgges.c */
int sgges_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *sdim, real *alphar, real *alphai, real *beta, real *vsl, integer *ldvsl, real *vsr, integer *ldvsr, real *work, integer *lwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* sggesx.c */
int sggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, char *sense, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *sdim, real *alphar, real *alphai, real *beta, real *vsl, integer *ldvsl, real *vsr, integer *ldvsr, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len, ftnlen sense_len);
/* sggev3.c */
int sggev3_(char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* sggev.c */
int sggev_(char *jobvl, char *jobvr, integer *n, real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *vl, integer *ldvl, real *vr, integer *ldvr, real *work, integer *lwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* sggevx.c */
int sggevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *ilo, integer *ihi, real *lscale, real *rscale, real *abnrm, real *bbnrm, real *rconde, real *rcondv, real *work, integer *lwork, integer *iwork, logical *bwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* sggglm.c */
int sggglm_(integer *n, integer *m, integer *p, real *a, integer *lda, real *b, integer *ldb, real *d__, real *x, real *y, real *work, integer *lwork, integer *info);
/* sgghd3.c */
int sgghd3_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *b, integer *ldb, real *q, integer *ldq, real *z__, integer *ldz, real *work, integer *lwork, integer *info, ftnlen compq_len, ftnlen compz_len);
/* sgghrd.c */
int sgghrd_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *b, integer *ldb, real *q, integer *ldq, real *z__, integer *ldz, integer *info, ftnlen compq_len, ftnlen compz_len);
/* sgglse.c */
int sgglse_(integer *m, integer *n, integer *p, real *a, integer *lda, real *b, integer *ldb, real *c__, real *d__, real *x, real *work, integer *lwork, integer *info);
/* sggqrf.c */
int sggqrf_(integer *n, integer *m, integer *p, real *a, integer *lda, real *taua, real *b, integer *ldb, real *taub, real *work, integer *lwork, integer *info);
/* sggrqf.c */
int sggrqf_(integer *m, integer *p, integer *n, real *a, integer *lda, real *taua, real *b, integer *ldb, real *taub, real *work, integer *lwork, integer *info);
/* sggsvd3.c */
int sggsvd3_(char *jobu, char *jobv, char *jobq, integer *m, integer *n, integer *p, integer *k, integer *l, real *a, integer *lda, real *b, integer *ldb, real *alpha, real *beta, real *u, integer *ldu, real *v, integer *ldv, real *q, integer *ldq, real *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* sggsvp3.c */
int sggsvp3_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, real *a, integer *lda, real *b, integer *ldb, real *tola, real *tolb, integer *k, integer *l, real *u, integer *ldu, real *v, integer *ldv, real *q, integer *ldq, integer *iwork, real *tau, real *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* sgsvj0.c */
int sgsvj0_(char *jobv, integer *m, integer *n, real *a, integer *lda, real *d__, real *sva, integer *mv, real *v, integer *ldv, real *eps, real *sfmin, real *tol, integer *nsweep, real *work, integer *lwork, integer *info, ftnlen jobv_len);
/* sgsvj1.c */
int sgsvj1_(char *jobv, integer *m, integer *n, integer *n1, real *a, integer *lda, real *d__, real *sva, integer *mv, real *v, integer *ldv, real *eps, real *sfmin, real *tol, integer *nsweep, real *work, integer *lwork, integer *info, ftnlen jobv_len);
/* sgtcon.c */
int sgtcon_(char *norm, integer *n, real *dl, real *d__, real *du, real *du2, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len);
/* sgtrfs.c */
int sgtrfs_(char *trans, integer *n, integer *nrhs, real *dl, real *d__, real *du, real *dlf, real *df, real *duf, real *du2, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen trans_len);
/* sgtsv.c */
int sgtsv_(integer *n, integer *nrhs, real *dl, real *d__, real *du, real *b, integer *ldb, integer *info);
/* sgtsvx.c */
int sgtsvx_(char *fact, char *trans, integer *n, integer *nrhs, real *dl, real *d__, real *du, real *dlf, real *df, real *duf, real *du2, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen trans_len);
/* sgttrf.c */
int sgttrf_(integer *n, real *dl, real *d__, real *du, real *du2, integer *ipiv, integer *info);
/* sgttrs.c */
int sgttrs_(char *trans, integer *n, integer *nrhs, real *dl, real *d__, real *du, real *du2, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen trans_len);
/* sgtts2.c */
int sgtts2_(integer *itrans, integer *n, integer *nrhs, real *dl, real *d__, real *du, real *du2, integer *ipiv, real *b, integer *ldb);
/* shgeqz.c */
int shgeqz_(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi, real *h__, integer *ldh, real *t, integer *ldt, real *alphar, real *alphai, real *beta, real *q, integer *ldq, real *z__, integer *ldz, real *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compq_len, ftnlen compz_len);
/* shsein.c */
int shsein_(char *side, char *eigsrc, char *initv, logical *select, integer *n, real *h__, integer *ldh, real *wr, real *wi, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *mm, integer *m, real *work, integer *ifaill, integer *ifailr, integer *info, ftnlen side_len, ftnlen eigsrc_len, ftnlen initv_len);
/* shseqr.c */
int shseqr_(char *job, char *compz, integer *n, integer *ilo, integer *ihi, real *h__, integer *ldh, real *wr, real *wi, real *z__, integer *ldz, real *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compz_len);
/* sisnan.c */
logical sisnan_(real *sin__);
/* slabad.c */
int slabad_(real *small, real *large);
/* slabrd.c */
int slabrd_(integer *m, integer *n, integer *nb, real *a, integer *lda, real *d__, real *e, real *tauq, real *taup, real *x, integer *ldx, real *y, integer *ldy);
/* slacn2.c */
int slacn2_(integer *n, real *v, real *x, integer *isgn, real *est, integer *kase, integer *isave);
/* slacon.c */
int slacon_(integer *n, real *v, real *x, integer *isgn, real *est, integer *kase);
/* slacpy.c */
int slacpy_(char *uplo, integer *m, integer *n, real *a, integer *lda, real *b, integer *ldb, ftnlen uplo_len);
/* sladiv.c */
int sladiv_(real *a, real *b, real *c__, real *d__, real *p, real *q);
int sladiv1_(real *a, real *b, real *c__, real *d__, real *p, real *q);
doublereal sladiv2_(real *a, real *b, real *c__, real *d__, real *r__, real *t);
/* slae2.c */
int slae2_(real *a, real *b, real *c__, real *rt1, real *rt2);
/* slaebz.c */
int slaebz_(integer *ijob, integer *nitmax, integer *n, integer *mmax, integer *minp, integer *nbmin, real *abstol, real *reltol, real *pivmin, real *d__, real *e, real *e2, integer *nval, real *ab, real *c__, integer *mout, integer *nab, real *work, integer *iwork, integer *info);
/* slaed0.c */
int slaed0_(integer *icompq, integer *qsiz, integer *n, real *d__, real *e, real *q, integer *ldq, real *qstore, integer *ldqs, real *work, integer *iwork, integer *info);
/* slaed1.c */
int slaed1_(integer *n, real *d__, real *q, integer *ldq, integer *indxq, real *rho, integer *cutpnt, real *work, integer *iwork, integer *info);
/* slaed2.c */
int slaed2_(integer *k, integer *n, integer *n1, real *d__, real *q, integer *ldq, integer *indxq, real *rho, real *z__, real *dlamda, real *w, real *q2, integer *indx, integer *indxc, integer *indxp, integer *coltyp, integer *info);
/* slaed3.c */
int slaed3_(integer *k, integer *n, integer *n1, real *d__, real *q, integer *ldq, real *rho, real *dlamda, real *q2, integer *indx, integer *ctot, real *w, real *s, integer *info);
/* slaed4.c */
int slaed4_(integer *n, integer *i__, real *d__, real *z__, real *delta, real *rho, real *dlam, integer *info);
/* slaed5.c */
int slaed5_(integer *i__, real *d__, real *z__, real *delta, real *rho, real *dlam);
/* slaed6.c */
int slaed6_(integer *kniter, logical *orgati, real *rho, real *d__, real *z__, real *finit, real *tau, integer *info);
/* slaed7.c */
int slaed7_(integer *icompq, integer *n, integer *qsiz, integer *tlvls, integer *curlvl, integer *curpbm, real *d__, real *q, integer *ldq, integer *indxq, real *rho, integer *cutpnt, real *qstore, integer *qptr, integer *prmptr, integer *perm, integer *givptr, integer *givcol, real *givnum, real *work, integer *iwork, integer *info);
/* slaed8.c */
int slaed8_(integer *icompq, integer *k, integer *n, integer *qsiz, real *d__, real *q, integer *ldq, integer *indxq, real *rho, integer *cutpnt, real *z__, real *dlamda, real *q2, integer *ldq2, real *w, integer *perm, integer *givptr, integer *givcol, real *givnum, integer *indxp, integer *indx, integer *info);
/* slaed9.c */
int slaed9_(integer *k, integer *kstart, integer *kstop, integer *n, real *d__, real *q, integer *ldq, real *rho, real *dlamda, real *w, real *s, integer *lds, integer *info);
/* slaeda.c */
int slaeda_(integer *n, integer *tlvls, integer *curlvl, integer *curpbm, integer *prmptr, integer *perm, integer *givptr, integer *givcol, real *givnum, real *q, integer *qptr, real *z__, real *ztemp, integer *info);
/* slaein.c */
int slaein_(logical *rightv, logical *noinit, integer *n, real *h__, integer *ldh, real *wr, real *wi, real *vr, real *vi, real *b, integer *ldb, real *work, real *eps3, real *smlnum, real *bignum, integer *info);
/* slaev2.c */
int slaev2_(real *a, real *b, real *c__, real *rt1, real *rt2, real *cs1, real *sn1);
/* slaexc.c */
int slaexc_(logical *wantq, integer *n, real *t, integer *ldt, real *q, integer *ldq, integer *j1, integer *n1, integer *n2, real *work, integer *info);
/* slag2.c */
int slag2_(real *a, integer *lda, real *b, integer *ldb, real *safmin, real *scale1, real *scale2, real *wr1, real *wr2, real *wi);
/* slag2d.c */
int slag2d_(integer *m, integer *n, real *sa, integer *ldsa, doublereal *a, integer *lda, integer *info);
/* sla_gbamv.c */
int sla_gbamv__(integer *trans, integer *m, integer *n, integer *kl, integer *ku, real *alpha, real *ab, integer *ldab, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sla_gbrcond.c */
doublereal sla_gbrcond__(char *trans, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, real *afb, integer *ldafb, integer *ipiv, integer *cmode, real *c__, integer *info, real *work, integer *iwork, ftnlen trans_len);
/* sla_gbrfsx_extended.c */
int sla_gbrfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *kl, integer *ku, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, integer *ipiv, logical *colequ, real *c__, real *b, integer *ldb, real *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, real *res, real *ayb, real *dy, real *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info);
/* sla_gbrpvgrw.c */
doublereal sla_gbrpvgrw__(integer *n, integer *kl, integer *ku, integer *ncols, real *ab, integer *ldab, real *afb, integer *ldafb);
/* sla_geamv.c */
int sla_geamv__(integer *trans, integer *m, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* sla_gercond.c */
doublereal sla_gercond__(char *trans, integer *n, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, integer *cmode, real *c__, integer *info, real *work, integer *iwork, ftnlen trans_len);
/* sla_gerfsx_extended.c */
int sla_gerfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, logical *colequ, real *c__, real *b, integer *ldb, real *y, integer *ldy, real *berr_out__, integer *n_norms__, real *errs_n__, real *errs_c__, real *res, real *ayb, real *dy, real *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info);
/* sla_gerpvgrw.c */
doublereal sla_gerpvgrw__(integer *n, integer *ncols, real *a, integer *lda, real *af, integer *ldaf);
/* slags2.c */
int slags2_(logical *upper, real *a1, real *a2, real *a3, real *b1, real *b2, real *b3, real *csu, real *snu, real *csv, real *snv, real *csq, real *snq);
/* slagtf.c */
int slagtf_(integer *n, real *a, real *lambda, real *b, real *c__, real *tol, real *d__, integer *in, integer *info);
/* slagtm.c */
int slagtm_(char *trans, integer *n, integer *nrhs, real *alpha, real *dl, real *d__, real *du, real *x, integer *ldx, real *beta, real *b, integer *ldb, ftnlen trans_len);
/* slagts.c */
int slagts_(integer *job, integer *n, real *a, real *b, real *c__, real *d__, integer *in, real *y, real *tol, integer *info);
/* slagv2.c */
int slagv2_(real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *csl, real *snl, real *csr, real *snr);
/* slahqr.c */
int slahqr_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, real *h__, integer *ldh, real *wr, real *wi, integer *iloz, integer *ihiz, real *z__, integer *ldz, integer *info);
/* slahr2.c */
int slahr2_(integer *n, integer *k, integer *nb, real *a, integer *lda, real *tau, real *t, integer *ldt, real *y, integer *ldy);
/* slaic1.c */
int slaic1_(integer *job, integer *j, real *x, real *sest, real *w, real *gamma, real *sestpr, real *s, real *c__);
/* slaisnan.c */
logical slaisnan_(real *sin1, real *sin2);
/* sla_lin_berr.c */
int sla_lin_berr__(integer *n, integer *nz, integer *nrhs, real *res, real *ayb, real *berr);
/* slaln2.c */
int slaln2_(logical *ltrans, integer *na, integer *nw, real *smin, real *ca, real *a, integer *lda, real *d1, real *d2, real *b, integer *ldb, real *wr, real *wi, real *x, integer *ldx, real *scale, real *xnorm, integer *info);
/* slals0.c */
int slals0_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *nrhs, real *b, integer *ldb, real *bx, integer *ldbx, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, real *givnum, integer *ldgnum, real *poles, real *difl, real *difr, real *z__, integer *k, real *c__, real *s, real *work, integer *info);
/* slalsa.c */
int slalsa_(integer *icompq, integer *smlsiz, integer *n, integer *nrhs, real *b, integer *ldb, real *bx, integer *ldbx, real *u, integer *ldu, real *vt, integer *k, real *difl, real *difr, real *z__, real *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, real *givnum, real *c__, real *s, real *work, integer *iwork, integer *info);
/* slalsd.c */
int slalsd_(char *uplo, integer *smlsiz, integer *n, integer *nrhs, real *d__, real *e, real *b, integer *ldb, real *rcond, integer *rank, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* slamrg.c */
int slamrg_(integer *n1, integer *n2, real *a, integer *strd1, integer *strd2, integer *index);
/* slamswlq.c */
int slamswlq_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, real *a, integer *lda, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* slamtsqr.c */
int slamtsqr_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, real *a, integer *lda, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* slaneg.c */
integer slaneg_(integer *n, real *d__, real *lld, real *sigma, real *pivmin, integer *r__);
/* slangb.c */
doublereal slangb_(char *norm, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, real *work, ftnlen norm_len);
/* slange.c */
doublereal slange_(char *norm, integer *m, integer *n, real *a, integer *lda, real *work, ftnlen norm_len);
/* slangt.c */
doublereal slangt_(char *norm, integer *n, real *dl, real *d__, real *du, ftnlen norm_len);
/* slanhs.c */
doublereal slanhs_(char *norm, integer *n, real *a, integer *lda, real *work, ftnlen norm_len);
/* slansb.c */
doublereal slansb_(char *norm, char *uplo, integer *n, integer *k, real *ab, integer *ldab, real *work, ftnlen norm_len, ftnlen uplo_len);
/* slansf.c */
doublereal slansf_(char *norm, char *transr, char *uplo, integer *n, real *a, real *work, ftnlen norm_len, ftnlen transr_len, ftnlen uplo_len);
/* slansp.c */
doublereal slansp_(char *norm, char *uplo, integer *n, real *ap, real *work, ftnlen norm_len, ftnlen uplo_len);
/* slanst.c */
doublereal slanst_(char *norm, integer *n, real *d__, real *e, ftnlen norm_len);
/* slansy.c */
doublereal slansy_(char *norm, char *uplo, integer *n, real *a, integer *lda, real *work, ftnlen norm_len, ftnlen uplo_len);
/* slantb.c */
doublereal slantb_(char *norm, char *uplo, char *diag, integer *n, integer *k, real *ab, integer *ldab, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* slantp.c */
doublereal slantp_(char *norm, char *uplo, char *diag, integer *n, real *ap, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* slantr.c */
doublereal slantr_(char *norm, char *uplo, char *diag, integer *m, integer *n, real *a, integer *lda, real *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* slanv2.c */
int slanv2_(real *a, real *b, real *c__, real *d__, real *rt1r, real *rt1i, real *rt2r, real *rt2i, real *cs, real *sn);
/* slapll.c */
int slapll_(integer *n, real *x, integer *incx, real *y, integer *incy, real *ssmin);
/* slapmr.c */
int slapmr_(logical *forwrd, integer *m, integer *n, real *x, integer *ldx, integer *k);
/* slapmt.c */
int slapmt_(logical *forwrd, integer *m, integer *n, real *x, integer *ldx, integer *k);
/* sla_porcond.c */
doublereal sla_porcond__(char *uplo, integer *n, real *a, integer *lda, real *af, integer *ldaf, integer *cmode, real *c__, integer *info, real *work, integer *iwork, ftnlen uplo_len);
/* sla_porfsx_extended.c */
int sla_porfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, logical *colequ, real *c__, real *b, integer *ldb, real *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, real *res, real *ayb, real *dy, real *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* sla_porpvgrw.c */
doublereal sla_porpvgrw__(char *uplo, integer *ncols, real *a, integer *lda, real *af, integer *ldaf, real *work, ftnlen uplo_len);
/* slapy2.c */
doublereal slapy2_(real *x, real *y);
/* slapy3.c */
doublereal slapy3_(real *x, real *y, real *z__);
/* slaqgb.c */
int slaqgb_(integer *m, integer *n, integer *kl, integer *ku, real *ab, integer *ldab, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, char *equed, ftnlen equed_len);
/* slaqge.c */
int slaqge_(integer *m, integer *n, real *a, integer *lda, real *r__, real *c__, real *rowcnd, real *colcnd, real *amax, char *equed, ftnlen equed_len);
/* slaqp2.c */
int slaqp2_(integer *m, integer *n, integer *offset, real *a, integer *lda, integer *jpvt, real *tau, real *vn1, real *vn2, real *work);
/* slaqps.c */
int slaqps_(integer *m, integer *n, integer *offset, integer *nb, integer *kb, real *a, integer *lda, integer *jpvt, real *tau, real *vn1, real *vn2, real *auxv, real *f, integer *ldf);
/* slaqr0.c */
int slaqr0_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, real *h__, integer *ldh, real *wr, real *wi, integer *iloz, integer *ihiz, real *z__, integer *ldz, real *work, integer *lwork, integer *info);
/* slaqr1.c */
int slaqr1_(integer *n, real *h__, integer *ldh, real *sr1, real *si1, real *sr2, real *si2, real *v);
/* slaqr2.c */
int slaqr2_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, real *h__, integer *ldh, integer *iloz, integer *ihiz, real *z__, integer *ldz, integer *ns, integer *nd, real *sr, real *si, real *v, integer *ldv, integer *nh, real *t, integer *ldt, integer *nv, real *wv, integer *ldwv, real *work, integer *lwork);
/* slaqr3.c */
int slaqr3_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, real *h__, integer *ldh, integer *iloz, integer *ihiz, real *z__, integer *ldz, integer *ns, integer *nd, real *sr, real *si, real *v, integer *ldv, integer *nh, real *t, integer *ldt, integer *nv, real *wv, integer *ldwv, real *work, integer *lwork);
/* slaqr4.c */
int slaqr4_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, real *h__, integer *ldh, real *wr, real *wi, integer *iloz, integer *ihiz, real *z__, integer *ldz, real *work, integer *lwork, integer *info);
/* slaqr5.c */
int slaqr5_(logical *wantt, logical *wantz, integer *kacc22, integer *n, integer *ktop, integer *kbot, integer *nshfts, real *sr, real *si, real *h__, integer *ldh, integer *iloz, integer *ihiz, real *z__, integer *ldz, real *v, integer *ldv, real *u, integer *ldu, integer *nv, real *wv, integer *ldwv, integer *nh, real *wh, integer *ldwh);
/* slaqsb.c */
int slaqsb_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* slaqsp.c */
int slaqsp_(char *uplo, integer *n, real *ap, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* slaqsy.c */
int slaqsy_(char *uplo, integer *n, real *a, integer *lda, real *s, real *scond, real *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* slaqtr.c */
int slaqtr_(logical *ltran, logical *lreal, integer *n, real *t, integer *ldt, real *b, real *w, real *scale, real *x, real *work, integer *info);
/* slar1v.c */
int slar1v_(integer *n, integer *b1, integer *bn, real *lambda, real *d__, real *l, real *ld, real *lld, real *pivmin, real *gaptol, real *z__, logical *wantnc, integer *negcnt, real *ztz, real *mingma, integer *r__, integer *isuppz, real *nrminv, real *resid, real *rqcorr, real *work);
/* slar2v.c */
int slar2v_(integer *n, real *x, real *y, real *z__, integer *incx, real *c__, real *s, integer *incc);
/* slarfb.c */
int slarfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, real *v, integer *ldv, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* slarf.c */
int slarf_(char *side, integer *m, integer *n, real *v, integer *incv, real *tau, real *c__, integer *ldc, real *work, ftnlen side_len);
/* slarfg.c */
int slarfg_(integer *n, real *alpha, real *x, integer *incx, real *tau);
/* slarfgp.c */
int slarfgp_(integer *n, real *alpha, real *x, integer *incx, real *tau);
/* slarft.c */
int slarft_(char *direct, char *storev, integer *n, integer *k, real *v, integer *ldv, real *tau, real *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* slarfx.c */
int slarfx_(char *side, integer *m, integer *n, real *v, real *tau, real *c__, integer *ldc, real *work, ftnlen side_len);
/* slarfy.c */
int slarfy_(char *uplo, integer *n, real *v, integer *incv, real *tau, real *c__, integer *ldc, real *work, ftnlen uplo_len);
/* slargv.c */
int slargv_(integer *n, real *x, integer *incx, real *y, integer *incy, real *c__, integer *incc);
/* slarnv.c */
int slarnv_(integer *idist, integer *iseed, integer *n, real *x);
/* slarra.c */
int slarra_(integer *n, real *d__, real *e, real *e2, real *spltol, real *tnrm, integer *nsplit, integer *isplit, integer *info);
/* slarrb.c */
int slarrb_(integer *n, real *d__, real *lld, integer *ifirst, integer *ilast, real *rtol1, real *rtol2, integer *offset, real *w, real *wgap, real *werr, real *work, integer *iwork, real *pivmin, real *spdiam, integer *twist, integer *info);
/* slarrc.c */
int slarrc_(char *jobt, integer *n, real *vl, real *vu, real *d__, real *e, real *pivmin, integer *eigcnt, integer *lcnt, integer *rcnt, integer *info, ftnlen jobt_len);
/* slarrd.c */
int slarrd_(char *range, char *order, integer *n, real *vl, real *vu, integer *il, integer *iu, real *gers, real *reltol, real *d__, real *e, real *e2, real *pivmin, integer *nsplit, integer *isplit, integer *m, real *w, real *werr, real *wl, real *wu, integer *iblock, integer *indexw, real *work, integer *iwork, integer *info, ftnlen range_len, ftnlen order_len);
/* slarre.c */
int slarre_(char *range, integer *n, real *vl, real *vu, integer *il, integer *iu, real *d__, real *e, real *e2, real *rtol1, real *rtol2, real *spltol, integer *nsplit, integer *isplit, integer *m, real *w, real *werr, real *wgap, integer *iblock, integer *indexw, real *gers, real *pivmin, real *work, integer *iwork, integer *info, ftnlen range_len);
/* slarrf.c */
int slarrf_(integer *n, real *d__, real *l, real *ld, integer *clstrt, integer *clend, real *w, real *wgap, real *werr, real *spdiam, real *clgapl, real *clgapr, real *pivmin, real *sigma, real *dplus, real *lplus, real *work, integer *info);
/* slarrj.c */
int slarrj_(integer *n, real *d__, real *e2, integer *ifirst, integer *ilast, real *rtol, integer *offset, real *w, real *werr, real *work, integer *iwork, real *pivmin, real *spdiam, integer *info);
/* slarrk.c */
int slarrk_(integer *n, integer *iw, real *gl, real *gu, real *d__, real *e2, real *pivmin, real *reltol, real *w, real *werr, integer *info);
/* slarrr.c */
int slarrr_(integer *n, real *d__, real *e, integer *info);
/* slarrv.c */
int slarrv_(integer *n, real *vl, real *vu, real *d__, real *l, real *pivmin, integer *isplit, integer *m, integer *dol, integer *dou, real *minrgp, real *rtol1, real *rtol2, real *w, real *werr, real *wgap, integer *iblock, integer *indexw, real *gers, real *z__, integer *ldz, integer *isuppz, real *work, integer *iwork, integer *info);
/* slarscl2.c */
int slarscl2_(integer *m, integer *n, real *d__, real *x, integer *ldx);
/* slartg.c */
int slartg_(real *f, real *g, real *cs, real *sn, real *r__);
/* slartgp.c */
int slartgp_(real *f, real *g, real *cs, real *sn, real *r__);
/* slartgs.c */
int slartgs_(real *x, real *y, real *sigma, real *cs, real *sn);
/* slartv.c */
int slartv_(integer *n, real *x, integer *incx, real *y, integer *incy, real *c__, real *s, integer *incc);
/* slaruv.c */
int slaruv_(integer *iseed, integer *n, real *x);
/* slarzb.c */
int slarzb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, real *v, integer *ldv, real *t, integer *ldt, real *c__, integer *ldc, real *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* slarz.c */
int slarz_(char *side, integer *m, integer *n, integer *l, real *v, integer *incv, real *tau, real *c__, integer *ldc, real *work, ftnlen side_len);
/* slarzt.c */
int slarzt_(char *direct, char *storev, integer *n, integer *k, real *v, integer *ldv, real *tau, real *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* slas2.c */
int slas2_(real *f, real *g, real *h__, real *ssmin, real *ssmax);
/* slascl2.c */
int slascl2_(integer *m, integer *n, real *d__, real *x, integer *ldx);
/* slascl.c */
int slascl_(char *type__, integer *kl, integer *ku, real *cfrom, real *cto, integer *m, integer *n, real *a, integer *lda, integer *info, ftnlen type_len);
/* slasd0.c */
int slasd0_(integer *n, integer *sqre, real *d__, real *e, real *u, integer *ldu, real *vt, integer *ldvt, integer *smlsiz, integer *iwork, real *work, integer *info);
/* slasd1.c */
int slasd1_(integer *nl, integer *nr, integer *sqre, real *d__, real *alpha, real *beta, real *u, integer *ldu, real *vt, integer *ldvt, integer *idxq, integer *iwork, real *work, integer *info);
/* slasd2.c */
int slasd2_(integer *nl, integer *nr, integer *sqre, integer *k, real *d__, real *z__, real *alpha, real *beta, real *u, integer *ldu, real *vt, integer *ldvt, real *dsigma, real *u2, integer *ldu2, real *vt2, integer *ldvt2, integer *idxp, integer *idx, integer *idxc, integer *idxq, integer *coltyp, integer *info);
/* slasd3.c */
int slasd3_(integer *nl, integer *nr, integer *sqre, integer *k, real *d__, real *q, integer *ldq, real *dsigma, real *u, integer *ldu, real *u2, integer *ldu2, real *vt, integer *ldvt, real *vt2, integer *ldvt2, integer *idxc, integer *ctot, real *z__, integer *info);
/* slasd4.c */
int slasd4_(integer *n, integer *i__, real *d__, real *z__, real *delta, real *rho, real *sigma, real *work, integer *info);
/* slasd5.c */
int slasd5_(integer *i__, real *d__, real *z__, real *delta, real *rho, real *dsigma, real *work);
/* slasd6.c */
int slasd6_(integer *icompq, integer *nl, integer *nr, integer *sqre, real *d__, real *vf, real *vl, real *alpha, real *beta, integer *idxq, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, real *givnum, integer *ldgnum, real *poles, real *difl, real *difr, real *z__, integer *k, real *c__, real *s, real *work, integer *iwork, integer *info);
/* slasd7.c */
int slasd7_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *k, real *d__, real *z__, real *zw, real *vf, real *vfw, real *vl, real *vlw, real *alpha, real *beta, real *dsigma, integer *idx, integer *idxp, integer *idxq, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, real *givnum, integer *ldgnum, real *c__, real *s, integer *info);
/* slasd8.c */
int slasd8_(integer *icompq, integer *k, real *d__, real *z__, real *vf, real *vl, real *difl, real *difr, integer *lddifr, real *dsigma, real *work, integer *info);
/* slasda.c */
int slasda_(integer *icompq, integer *smlsiz, integer *n, integer *sqre, real *d__, real *e, real *u, integer *ldu, real *vt, integer *k, real *difl, real *difr, real *z__, real *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, real *givnum, real *c__, real *s, real *work, integer *iwork, integer *info);
/* slasdq.c */
int slasdq_(char *uplo, integer *sqre, integer *n, integer *ncvt, integer *nru, integer *ncc, real *d__, real *e, real *vt, integer *ldvt, real *u, integer *ldu, real *c__, integer *ldc, real *work, integer *info, ftnlen uplo_len);
/* slasdt.c */
int slasdt_(integer *n, integer *lvl, integer *nd, integer *inode, integer *ndiml, integer *ndimr, integer *msub);
/* slaset.c */
int slaset_(char *uplo, integer *m, integer *n, real *alpha, real *beta, real *a, integer *lda, ftnlen uplo_len);
/* slasq1.c */
int slasq1_(integer *n, real *d__, real *e, real *work, integer *info);
/* slasq2.c */
int slasq2_(integer *n, real *z__, integer *info);
/* slasq3.c */
int slasq3_(integer *i0, integer *n0, real *z__, integer *pp, real *dmin__, real *sigma, real *desig, real *qmax, integer *nfail, integer *iter, integer *ndiv, logical *ieee, integer *ttype, real *dmin1, real *dmin2, real *dn, real *dn1, real *dn2, real *g, real *tau);
/* slasq4.c */
int slasq4_(integer *i0, integer *n0, real *z__, integer *pp, integer *n0in, real *dmin__, real *dmin1, real *dmin2, real *dn, real *dn1, real *dn2, real *tau, integer *ttype, real *g);
/* slasq5.c */
int slasq5_(integer *i0, integer *n0, real *z__, integer *pp, real *tau, real *sigma, real *dmin__, real *dmin1, real *dmin2, real *dn, real *dnm1, real *dnm2, logical *ieee, real *eps);
/* slasq6.c */
int slasq6_(integer *i0, integer *n0, real *z__, integer *pp, real *dmin__, real *dmin1, real *dmin2, real *dn, real *dnm1, real *dnm2);
/* slasr.c */
int slasr_(char *side, char *pivot, char *direct, integer *m, integer *n, real *c__, real *s, real *a, integer *lda, ftnlen side_len, ftnlen pivot_len, ftnlen direct_len);
/* slasrt.c */
int slasrt_(char *id, integer *n, real *d__, integer *info, ftnlen id_len);
/* slassq.c */
int slassq_(integer *n, real *x, integer *incx, real *scale, real *sumsq);
/* slasv2.c */
int slasv2_(real *f, real *g, real *h__, real *ssmin, real *ssmax, real *snr, real *csr, real *snl, real *csl);
/* slaswlq.c */
int slaswlq_(integer *m, integer *n, integer *mb, integer *nb, real *a, integer *lda, real *t, integer *ldt, real *work, integer *lwork, integer *info);
/* slaswp.c */
int slaswp_(integer *n, real *a, integer *lda, integer *k1, integer *k2, integer *ipiv, integer *incx);
/* slasy2.c */
int slasy2_(logical *ltranl, logical *ltranr, integer *isgn, integer *n1, integer *n2, real *tl, integer *ldtl, real *tr, integer *ldtr, real *b, integer *ldb, real *scale, real *x, integer *ldx, real *xnorm, integer *info);
/* sla_syamv.c */
int sla_syamv__(integer *uplo, integer *n, real *alpha, real *a, integer *lda, real *x, integer *incx, real *beta, real *y, integer *incy);
/* slasyf_aa.c */
int slasyf_aa__(char *uplo, integer *j1, integer *m, integer *nb, real *a, integer *lda, integer *ipiv, real *h__, integer *ldh, real *work, ftnlen uplo_len);
/* slasyf.c */
int slasyf_(char *uplo, integer *n, integer *nb, integer *kb, real *a, integer *lda, integer *ipiv, real *w, integer *ldw, integer *info, ftnlen uplo_len);
/* slasyf_rk.c */
int slasyf_rk__(char *uplo, integer *n, integer *nb, integer *kb, real *a, integer *lda, real *e, integer *ipiv, real *w, integer *ldw, integer *info, ftnlen uplo_len);
/* slasyf_rook.c */
int slasyf_rook__(char *uplo, integer *n, integer *nb, integer *kb, real *a, integer *lda, integer *ipiv, real *w, integer *ldw, integer *info, ftnlen uplo_len);
/* sla_syrcond.c */
doublereal sla_syrcond__(char *uplo, integer *n, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, integer *cmode, real *c__, integer *info, real *work, integer *iwork, ftnlen uplo_len);
/* sla_syrfsx_extended.c */
int sla_syrfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, logical *colequ, real *c__, real *b, integer *ldb, real *y, integer *ldy, real *berr_out__, integer *n_norms__, real *err_bnds_norm__, real *err_bnds_comp__, real *res, real *ayb, real *dy, real *y_tail__, real *rcond, integer *ithresh, real *rthresh, real *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* sla_syrpvgrw.c */
doublereal sla_syrpvgrw__(char *uplo, integer *n, integer *info, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, real *work, ftnlen uplo_len);
/* slatbs.c */
int slatbs_(char *uplo, char *trans, char *diag, char *normin, integer *n, integer *kd, real *ab, integer *ldab, real *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* slatdf.c */
int slatdf_(integer *ijob, integer *n, real *z__, integer *ldz, real *rhs, real *rdsum, real *rdscal, integer *ipiv, integer *jpiv);
/* slatps.c */
int slatps_(char *uplo, char *trans, char *diag, char *normin, integer *n, real *ap, real *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* slatrd.c */
int slatrd_(char *uplo, integer *n, integer *nb, real *a, integer *lda, real *e, real *tau, real *w, integer *ldw, ftnlen uplo_len);
/* slatrs.c */
int slatrs_(char *uplo, char *trans, char *diag, char *normin, integer *n, real *a, integer *lda, real *x, real *scale, real *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* slatrz.c */
int slatrz_(integer *m, integer *n, integer *l, real *a, integer *lda, real *tau, real *work);
/* slatsqr.c */
int slatsqr_(integer *m, integer *n, integer *mb, integer *nb, real *a, integer *lda, real *t, integer *ldt, real *work, integer *lwork, integer *info);
/* slauu2.c */
int slauu2_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* slauum.c */
int slauum_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* sla_wwaddw.c */
int sla_wwaddw__(integer *n, real *x, real *y, real *w);
/* sopgtr.c */
int sopgtr_(char *uplo, integer *n, real *ap, real *tau, real *q, integer *ldq, real *work, integer *info, ftnlen uplo_len);
/* sopmtr.c */
int sopmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, real *ap, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* sorbdb1.c */
int sorbdb1_(integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x21, integer *ldx21, real *theta, real *phi, real *taup1, real *taup2, real *tauq1, real *work, integer *lwork, integer *info);
/* sorbdb2.c */
int sorbdb2_(integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x21, integer *ldx21, real *theta, real *phi, real *taup1, real *taup2, real *tauq1, real *work, integer *lwork, integer *info);
/* sorbdb3.c */
int sorbdb3_(integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x21, integer *ldx21, real *theta, real *phi, real *taup1, real *taup2, real *tauq1, real *work, integer *lwork, integer *info);
/* sorbdb4.c */
int sorbdb4_(integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x21, integer *ldx21, real *theta, real *phi, real *taup1, real *taup2, real *tauq1, real *phantom, real *work, integer *lwork, integer *info);
/* sorbdb5.c */
int sorbdb5_(integer *m1, integer *m2, integer *n, real *x1, integer *incx1, real *x2, integer *incx2, real *q1, integer *ldq1, real *q2, integer *ldq2, real *work, integer *lwork, integer *info);
/* sorbdb6.c */
int sorbdb6_(integer *m1, integer *m2, integer *n, real *x1, integer *incx1, real *x2, integer *incx2, real *q1, integer *ldq1, real *q2, integer *ldq2, real *work, integer *lwork, integer *info);
/* sorbdb.c */
int sorbdb_(char *trans, char *signs, integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x12, integer *ldx12, real *x21, integer *ldx21, real *x22, integer *ldx22, real *theta, real *phi, real *taup1, real *taup2, real *tauq1, real *tauq2, real *work, integer *lwork, integer *info, ftnlen trans_len, ftnlen signs_len);
/* sorcsd2by1.c */
int sorcsd2by1_(char *jobu1, char *jobu2, char *jobv1t, integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x21, integer *ldx21, real *theta, real *u1, integer *ldu1, real *u2, integer *ldu2, real *v1t, integer *ldv1t, real *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len);
/* sorcsd.c */
int sorcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, char *signs, integer *m, integer *p, integer *q, real *x11, integer *ldx11, real *x12, integer *ldx12, real *x21, integer *ldx21, real *x22, integer *ldx22, real *theta, real *u1, integer *ldu1, real *u2, integer *ldu2, real *v1t, integer *ldv1t, real *v2t, integer *ldv2t, real *work, integer *lwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len, ftnlen signs_len);
/* sorg2l.c */
int sorg2l_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *info);
/* sorg2r.c */
int sorg2r_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *info);
/* sorgbr.c */
int sorgbr_(char *vect, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info, ftnlen vect_len);
/* sorghr.c */
int sorghr_(integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sorgl2.c */
int sorgl2_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *info);
/* sorglq.c */
int sorglq_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sorgql.c */
int sorgql_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sorgqr.c */
int sorgqr_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sorgr2.c */
int sorgr2_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *info);
/* sorgrq.c */
int sorgrq_(integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* sorgtr.c */
int sorgtr_(char *uplo, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* sorm22.c */
int sorm22_(char *side, char *trans, integer *m, integer *n, integer *n1, integer *n2, real *q, integer *ldq, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sorm2l.c */
int sorm2l_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sorm2r.c */
int sorm2r_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormbr.c */
int sormbr_(char *vect, char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen side_len, ftnlen trans_len);
/* sormhr.c */
int sormhr_(char *side, char *trans, integer *m, integer *n, integer *ilo, integer *ihi, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sorml2.c */
int sorml2_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormlq.c */
int sormlq_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormql.c */
int sormql_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormqr.c */
int sormqr_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormr2.c */
int sormr2_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormr3.c */
int sormr3_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormrq.c */
int sormrq_(char *side, char *trans, integer *m, integer *n, integer *k, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormrz.c */
int sormrz_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* sormtr.c */
int sormtr_(char *side, char *uplo, char *trans, integer *m, integer *n, real *a, integer *lda, real *tau, real *c__, integer *ldc, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* spbcon.c */
int spbcon_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* spbequ.c */
int spbequ_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *s, real *scond, real *amax, integer *info, ftnlen uplo_len);
/* spbrfs.c */
int spbrfs_(char *uplo, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* spbstf.c */
int spbstf_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* spbsv.c */
int spbsv_(char *uplo, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* spbsvx.c */
int spbsvx_(char *fact, char *uplo, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *afb, integer *ldafb, char *equed, real *s, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* spbtf2.c */
int spbtf2_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* spbtrf.c */
int spbtrf_(char *uplo, integer *n, integer *kd, real *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* spbtrs.c */
int spbtrs_(char *uplo, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* spftrf.c */
int spftrf_(char *transr, char *uplo, integer *n, real *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* spftri.c */
int spftri_(char *transr, char *uplo, integer *n, real *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* spftrs.c */
int spftrs_(char *transr, char *uplo, integer *n, integer *nrhs, real *a, real *b, integer *ldb, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* spocon.c */
int spocon_(char *uplo, integer *n, real *a, integer *lda, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* spoequb.c */
int spoequb_(integer *n, real *a, integer *lda, real *s, real *scond, real *amax, integer *info);
/* spoequ.c */
int spoequ_(integer *n, real *a, integer *lda, real *s, real *scond, real *amax, integer *info);
/* sporfs.c */
int sporfs_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* sposv.c */
int sposv_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* sposvx.c */
int sposvx_(char *fact, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, char *equed, real *s, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* sposvxx.c */
int sposvxx_(char *fact, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, char *equed, real *s, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* spotf2.c */
int spotf2_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* spotrf2.c */
int spotrf2_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* spotrf.c */
int spotrf_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* spotri.c */
int spotri_(char *uplo, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* spotrs.c */
int spotrs_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* sppcon.c */
int sppcon_(char *uplo, integer *n, real *ap, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* sppequ.c */
int sppequ_(char *uplo, integer *n, real *ap, real *s, real *scond, real *amax, integer *info, ftnlen uplo_len);
/* spprfs.c */
int spprfs_(char *uplo, integer *n, integer *nrhs, real *ap, real *afp, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* sppsv.c */
int sppsv_(char *uplo, integer *n, integer *nrhs, real *ap, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* sppsvx.c */
int sppsvx_(char *fact, char *uplo, integer *n, integer *nrhs, real *ap, real *afp, char *equed, real *s, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* spptrf.c */
int spptrf_(char *uplo, integer *n, real *ap, integer *info, ftnlen uplo_len);
/* spptri.c */
int spptri_(char *uplo, integer *n, real *ap, integer *info, ftnlen uplo_len);
/* spptrs.c */
int spptrs_(char *uplo, integer *n, integer *nrhs, real *ap, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* spstf2.c */
int spstf2_(char *uplo, integer *n, real *a, integer *lda, integer *piv, integer *rank, real *tol, real *work, integer *info, ftnlen uplo_len);
/* spstrf.c */
int spstrf_(char *uplo, integer *n, real *a, integer *lda, integer *piv, integer *rank, real *tol, real *work, integer *info, ftnlen uplo_len);
/* sptcon.c */
int sptcon_(integer *n, real *d__, real *e, real *anorm, real *rcond, real *work, integer *info);
/* spteqr.c */
int spteqr_(char *compz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *info, ftnlen compz_len);
/* sptrfs.c */
int sptrfs_(integer *n, integer *nrhs, real *d__, real *e, real *df, real *ef, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *info);
/* sptsv.c */
int sptsv_(integer *n, integer *nrhs, real *d__, real *e, real *b, integer *ldb, integer *info);
/* sptsvx.c */
int sptsvx_(char *fact, integer *n, integer *nrhs, real *d__, real *e, real *df, real *ef, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *info, ftnlen fact_len);
/* spttrf.c */
int spttrf_(integer *n, real *d__, real *e, integer *info);
/* spttrs.c */
int spttrs_(integer *n, integer *nrhs, real *d__, real *e, real *b, integer *ldb, integer *info);
/* sptts2.c */
int sptts2_(integer *n, integer *nrhs, real *d__, real *e, real *b, integer *ldb);
/* srscl.c */
int srscl_(integer *n, real *sa, real *sx, integer *incx);
/* ssb2st_kernels.c */
int ssb2st_kernels__(char *uplo, logical *wantz, integer *ttype, integer *st, integer *ed, integer *sweep, integer *n, integer *nb, integer *ib, real *a, integer *lda, real *v, real *tau, integer *ldvt, real *work, ftnlen uplo_len);
/* ssbev_2stage.c */
int ssbev_2stage__(char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbev.c */
int ssbev_(char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbevd_2stage.c */
int ssbevd_2stage__(char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbevd.c */
int ssbevd_(char *jobz, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbevx_2stage.c */
int ssbevx_2stage__(char *jobz, char *range, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssbevx.c */
int ssbevx_(char *jobz, char *range, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssbgst.c */
int ssbgst_(char *vect, char *uplo, integer *n, integer *ka, integer *kb, real *ab, integer *ldab, real *bb, integer *ldbb, real *x, integer *ldx, real *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* ssbgv.c */
int ssbgv_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, real *ab, integer *ldab, real *bb, integer *ldbb, real *w, real *z__, integer *ldz, real *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbgvd.c */
int ssbgvd_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, real *ab, integer *ldab, real *bb, integer *ldbb, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssbgvx.c */
int ssbgvx_(char *jobz, char *range, char *uplo, integer *n, integer *ka, integer *kb, real *ab, integer *ldab, real *bb, integer *ldbb, real *q, integer *ldq, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssbtrd.c */
int ssbtrd_(char *vect, char *uplo, integer *n, integer *kd, real *ab, integer *ldab, real *d__, real *e, real *q, integer *ldq, real *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* ssfrk.c */
int ssfrk_(char *transr, char *uplo, char *trans, integer *n, integer *k, real *alpha, real *a, integer *lda, real *beta, real *c__, ftnlen transr_len, ftnlen uplo_len, ftnlen trans_len);
/* sspcon.c */
int sspcon_(char *uplo, integer *n, real *ap, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* sspev.c */
int sspev_(char *jobz, char *uplo, integer *n, real *ap, real *w, real *z__, integer *ldz, real *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* sspevd.c */
int sspevd_(char *jobz, char *uplo, integer *n, real *ap, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* sspevx.c */
int sspevx_(char *jobz, char *range, char *uplo, integer *n, real *ap, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* sspgst.c */
int sspgst_(integer *itype, char *uplo, integer *n, real *ap, real *bp, integer *info, ftnlen uplo_len);
/* sspgv.c */
int sspgv_(integer *itype, char *jobz, char *uplo, integer *n, real *ap, real *bp, real *w, real *z__, integer *ldz, real *work, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* sspgvd.c */
int sspgvd_(integer *itype, char *jobz, char *uplo, integer *n, real *ap, real *bp, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* sspgvx.c */
int sspgvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, real *ap, real *bp, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssprfs.c */
int ssprfs_(char *uplo, integer *n, integer *nrhs, real *ap, real *afp, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* sspsv.c */
int sspsv_(char *uplo, integer *n, integer *nrhs, real *ap, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* sspsvx.c */
int sspsvx_(char *fact, char *uplo, integer *n, integer *nrhs, real *ap, real *afp, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* ssptrd.c */
int ssptrd_(char *uplo, integer *n, real *ap, real *d__, real *e, real *tau, integer *info, ftnlen uplo_len);
/* ssptrf.c */
int ssptrf_(char *uplo, integer *n, real *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* ssptri.c */
int ssptri_(char *uplo, integer *n, real *ap, integer *ipiv, real *work, integer *info, ftnlen uplo_len);
/* ssptrs.c */
int ssptrs_(char *uplo, integer *n, integer *nrhs, real *ap, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* sstebz.c */
int sstebz_(char *range, char *order, integer *n, real *vl, real *vu, integer *il, integer *iu, real *abstol, real *d__, real *e, integer *m, integer *nsplit, real *w, integer *iblock, integer *isplit, real *work, integer *iwork, integer *info, ftnlen range_len, ftnlen order_len);
/* sstedc.c */
int sstedc_(char *compz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen compz_len);
/* sstegr.c */
int sstegr_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, integer *isuppz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* sstein.c */
int sstein_(integer *n, real *d__, real *e, integer *m, real *w, integer *iblock, integer *isplit, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info);
/* sstemr.c */
int sstemr_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, integer *m, real *w, real *z__, integer *ldz, integer *nzc, integer *isuppz, logical *tryrac, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* ssteqr.c */
int ssteqr_(char *compz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *info, ftnlen compz_len);
/* ssterf.c */
int ssterf_(integer *n, real *d__, real *e, integer *info);
/* sstev.c */
int sstev_(char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *info, ftnlen jobz_len);
/* sstevd.c */
int sstevd_(char *jobz, integer *n, real *d__, real *e, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len);
/* sstevr.c */
int sstevr_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, integer *isuppz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* sstevx.c */
int sstevx_(char *jobz, char *range, integer *n, real *d__, real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len);
/* ssycon_3.c */
int ssycon_3__(char *uplo, integer *n, real *a, integer *lda, real *e, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* ssycon.c */
int ssycon_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* ssycon_rook.c */
int ssycon_rook__(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *anorm, real *rcond, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* ssyconv.c */
int ssyconv_(char *uplo, char *way, integer *n, real *a, integer *lda, integer *ipiv, real *e, integer *info, ftnlen uplo_len, ftnlen way_len);
/* ssyconvf.c */
int ssyconvf_(char *uplo, char *way, integer *n, real *a, integer *lda, real *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* ssyconvf_rook.c */
int ssyconvf_rook__(char *uplo, char *way, integer *n, real *a, integer *lda, real *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* ssyequb.c */
int ssyequb_(char *uplo, integer *n, real *a, integer *lda, real *s, real *scond, real *amax, real *work, integer *info, ftnlen uplo_len);
/* ssyev_2stage.c */
int ssyev_2stage__(char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssyev.c */
int ssyev_(char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssyevd_2stage.c */
int ssyevd_2stage__(char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssyevd.c */
int ssyevd_(char *jobz, char *uplo, integer *n, real *a, integer *lda, real *w, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssyevr_2stage.c */
int ssyevr_2stage__(char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, integer *isuppz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssyevr.c */
int ssyevr_(char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, integer *isuppz, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssyevx_2stage.c */
int ssyevx_2stage__(char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssyevx.c */
int ssyevx_(char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssygs2.c */
int ssygs2_(integer *itype, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ssygst.c */
int ssygst_(integer *itype, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ssygv_2stage.c */
int ssygv_2stage__(integer *itype, char *jobz, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, real *w, real *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssygv.c */
int ssygv_(integer *itype, char *jobz, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, real *w, real *work, integer *lwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssygvd.c */
int ssygvd_(integer *itype, char *jobz, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, real *w, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* ssygvx.c */
int ssygvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, real *a, integer *lda, real *b, integer *ldb, real *vl, real *vu, integer *il, integer *iu, real *abstol, integer *m, real *w, real *z__, integer *ldz, real *work, integer *lwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* ssyrfs.c */
int ssyrfs_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len);
/* ssysv_aa_2stage.c */
int ssysv_aa_2stage__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *tb, integer *ltb, integer *ipiv, integer *ipiv2, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssysv_aa.c */
int ssysv_aa__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssysv.c */
int ssysv_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssysv_rk.c */
int ssysv_rk__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *e, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssysv_rook.c */
int ssysv_rook__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssysvx.c */
int ssysvx_(char *fact, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *ferr, real *berr, real *work, integer *lwork, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* ssysvxx.c */
int ssysvxx_(char *fact, char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *af, integer *ldaf, integer *ipiv, char *equed, real *s, real *b, integer *ldb, real *x, integer *ldx, real *rcond, real *rpvgrw, real *berr, integer *n_err_bnds__, real *err_bnds_norm__, real *err_bnds_comp__, integer *nparams, real *params, real *work, integer *iwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* ssyswapr.c */
int ssyswapr_(char *uplo, integer *n, real *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* ssytd2.c */
int ssytd2_(char *uplo, integer *n, real *a, integer *lda, real *d__, real *e, real *tau, integer *info, ftnlen uplo_len);
/* ssytf2.c */
int ssytf2_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* ssytf2_rk.c */
int ssytf2_rk__(char *uplo, integer *n, real *a, integer *lda, real *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* ssytf2_rook.c */
int ssytf2_rook__(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* ssytrd_2stage.c */
int ssytrd_2stage__(char *vect, char *uplo, integer *n, real *a, integer *lda, real *d__, real *e, real *tau, real *hous2, integer *lhous2, real *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* ssytrd.c */
int ssytrd_(char *uplo, integer *n, real *a, integer *lda, real *d__, real *e, real *tau, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrd_sy2sb.c */
int ssytrd_sy2sb__(char *uplo, integer *n, integer *kd, real *a, integer *lda, real *ab, integer *ldab, real *tau, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrf_aa_2stage.c */
int ssytrf_aa_2stage__(char *uplo, integer *n, real *a, integer *lda, real *tb, integer *ltb, integer *ipiv, integer *ipiv2, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrf_aa.c */
int ssytrf_aa__(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrf.c */
int ssytrf_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrf_rk.c */
int ssytrf_rk__(char *uplo, integer *n, real *a, integer *lda, real *e, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrf_rook.c */
int ssytrf_rook__(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytri2.c */
int ssytri2_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytri2x.c */
int ssytri2x_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *nb, integer *info, ftnlen uplo_len);
/* ssytri_3.c */
int ssytri_3__(char *uplo, integer *n, real *a, integer *lda, real *e, integer *ipiv, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytri_3x.c */
int ssytri_3x__(char *uplo, integer *n, real *a, integer *lda, real *e, integer *ipiv, real *work, integer *nb, integer *info, ftnlen uplo_len);
/* ssytri.c */
int ssytri_(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *info, ftnlen uplo_len);
/* ssytri_rook.c */
int ssytri_rook__(char *uplo, integer *n, real *a, integer *lda, integer *ipiv, real *work, integer *info, ftnlen uplo_len);
/* ssytrs2.c */
int ssytrs2_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *info, ftnlen uplo_len);
/* ssytrs_3.c */
int ssytrs_3__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *e, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ssytrs_aa_2stage.c */
int ssytrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, real *tb, integer *ltb, integer *ipiv, integer *ipiv2, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ssytrs_aa.c */
int ssytrs_aa__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, real *work, integer *lwork, integer *info, ftnlen uplo_len);
/* ssytrs.c */
int ssytrs_(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ssytrs_rook.c */
int ssytrs_rook__(char *uplo, integer *n, integer *nrhs, real *a, integer *lda, integer *ipiv, real *b, integer *ldb, integer *info, ftnlen uplo_len);
/* stbcon.c */
int stbcon_(char *norm, char *uplo, char *diag, integer *n, integer *kd, real *ab, integer *ldab, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* stbrfs.c */
int stbrfs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stbtrs.c */
int stbtrs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, real *ab, integer *ldab, real *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stfsm.c */
int stfsm_(char *transr, char *side, char *uplo, char *trans, char *diag, integer *m, integer *n, real *alpha, real *a, real *b, integer *ldb, ftnlen transr_len, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stftri.c */
int stftri_(char *transr, char *uplo, char *diag, integer *n, real *a, integer *info, ftnlen transr_len, ftnlen uplo_len, ftnlen diag_len);
/* stfttp.c */
int stfttp_(char *transr, char *uplo, integer *n, real *arf, real *ap, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* stfttr.c */
int stfttr_(char *transr, char *uplo, integer *n, real *arf, real *a, integer *lda, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* stgevc.c */
int stgevc_(char *side, char *howmny, logical *select, integer *n, real *s, integer *lds, real *p, integer *ldp, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *mm, integer *m, real *work, integer *info, ftnlen side_len, ftnlen howmny_len);
/* stgex2.c */
int stgex2_(logical *wantq, logical *wantz, integer *n, real *a, integer *lda, real *b, integer *ldb, real *q, integer *ldq, real *z__, integer *ldz, integer *j1, integer *n1, integer *n2, real *work, integer *lwork, integer *info);
/* stgexc.c */
int stgexc_(logical *wantq, logical *wantz, integer *n, real *a, integer *lda, real *b, integer *ldb, real *q, integer *ldq, real *z__, integer *ldz, integer *ifst, integer *ilst, real *work, integer *lwork, integer *info);
/* stgsen.c */
int stgsen_(integer *ijob, logical *wantq, logical *wantz, logical *select, integer *n, real *a, integer *lda, real *b, integer *ldb, real *alphar, real *alphai, real *beta, real *q, integer *ldq, real *z__, integer *ldz, integer *m, real *pl, real *pr, real *dif, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info);
/* stgsja.c */
int stgsja_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, integer *k, integer *l, real *a, integer *lda, real *b, integer *ldb, real *tola, real *tolb, real *alpha, real *beta, real *u, integer *ldu, real *v, integer *ldv, real *q, integer *ldq, real *work, integer *ncycle, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* stgsna.c */
int stgsna_(char *job, char *howmny, logical *select, integer *n, real *a, integer *lda, real *b, integer *ldb, real *vl, integer *ldvl, real *vr, integer *ldvr, real *s, real *dif, integer *mm, integer *m, real *work, integer *lwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* stgsy2.c */
int stgsy2_(char *trans, integer *ijob, integer *m, integer *n, real *a, integer *lda, real *b, integer *ldb, real *c__, integer *ldc, real *d__, integer *ldd, real *e, integer *lde, real *f, integer *ldf, real *scale, real *rdsum, real *rdscal, integer *iwork, integer *pq, integer *info, ftnlen trans_len);
/* stgsyl.c */
int stgsyl_(char *trans, integer *ijob, integer *m, integer *n, real *a, integer *lda, real *b, integer *ldb, real *c__, integer *ldc, real *d__, integer *ldd, real *e, integer *lde, real *f, integer *ldf, real *scale, real *dif, real *work, integer *lwork, integer *iwork, integer *info, ftnlen trans_len);
/* stpcon.c */
int stpcon_(char *norm, char *uplo, char *diag, integer *n, real *ap, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* stplqt2.c */
int stplqt2_(integer *m, integer *n, integer *l, real *a, integer *lda, real *b, integer *ldb, real *t, integer *ldt, integer *info);
/* stplqt.c */
int stplqt_(integer *m, integer *n, integer *l, integer *mb, real *a, integer *lda, real *b, integer *ldb, real *t, integer *ldt, real *work, integer *info);
/* stpmlqt.c */
int stpmlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *mb, real *v, integer *ldv, real *t, integer *ldt, real *a, integer *lda, real *b, integer *ldb, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* stpmqrt.c */
int stpmqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *nb, real *v, integer *ldv, real *t, integer *ldt, real *a, integer *lda, real *b, integer *ldb, real *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* stpqrt2.c */
int stpqrt2_(integer *m, integer *n, integer *l, real *a, integer *lda, real *b, integer *ldb, real *t, integer *ldt, integer *info);
/* stpqrt.c */
int stpqrt_(integer *m, integer *n, integer *l, integer *nb, real *a, integer *lda, real *b, integer *ldb, real *t, integer *ldt, real *work, integer *info);
/* stprfb.c */
int stprfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, real *v, integer *ldv, real *t, integer *ldt, real *a, integer *lda, real *b, integer *ldb, real *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* stprfs.c */
int stprfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, real *ap, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stptri.c */
int stptri_(char *uplo, char *diag, integer *n, real *ap, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* stptrs.c */
int stptrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, real *ap, real *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* stpttf.c */
int stpttf_(char *transr, char *uplo, integer *n, real *ap, real *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* stpttr.c */
int stpttr_(char *uplo, integer *n, real *ap, real *a, integer *lda, integer *info, ftnlen uplo_len);
/* strcon.c */
int strcon_(char *norm, char *uplo, char *diag, integer *n, real *a, integer *lda, real *rcond, real *work, integer *iwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* strevc3.c */
int strevc3_(char *side, char *howmny, logical *select, integer *n, real *t, integer *ldt, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *mm, integer *m, real *work, integer *lwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* strevc.c */
int strevc_(char *side, char *howmny, logical *select, integer *n, real *t, integer *ldt, real *vl, integer *ldvl, real *vr, integer *ldvr, integer *mm, integer *m, real *work, integer *info, ftnlen side_len, ftnlen howmny_len);
/* strexc.c */
int strexc_(char *compq, integer *n, real *t, integer *ldt, real *q, integer *ldq, integer *ifst, integer *ilst, real *work, integer *info, ftnlen compq_len);
/* strrfs.c */
int strrfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, real *x, integer *ldx, real *ferr, real *berr, real *work, integer *iwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* strsen.c */
int strsen_(char *job, char *compq, logical *select, integer *n, real *t, integer *ldt, real *q, integer *ldq, real *wr, real *wi, integer *m, real *s, real *sep, real *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen job_len, ftnlen compq_len);
/* strsna.c */
int strsna_(char *job, char *howmny, logical *select, integer *n, real *t, integer *ldt, real *vl, integer *ldvl, real *vr, integer *ldvr, real *s, real *sep, integer *mm, integer *m, real *work, integer *ldwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* strsyl.c */
int strsyl_(char *trana, char *tranb, integer *isgn, integer *m, integer *n, real *a, integer *lda, real *b, integer *ldb, real *c__, integer *ldc, real *scale, integer *info, ftnlen trana_len, ftnlen tranb_len);
/* strti2.c */
int strti2_(char *uplo, char *diag, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* strtri.c */
int strtri_(char *uplo, char *diag, integer *n, real *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* strtrs.c */
int strtrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, real *a, integer *lda, real *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* strttf.c */
int strttf_(char *transr, char *uplo, integer *n, real *a, integer *lda, real *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* strttp.c */
int strttp_(char *uplo, integer *n, real *a, integer *lda, real *ap, integer *info, ftnlen uplo_len);
/* stzrzf.c */
int stzrzf_(integer *m, integer *n, real *a, integer *lda, real *tau, real *work, integer *lwork, integer *info);
/* xerbla_array.c */
int xerbla_array__(char *srname_array__, integer *srname_len__, integer *info, ftnlen srname_array_len);
/* zbbcsd.c */
int zbbcsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, integer *m, integer *p, integer *q, doublereal *theta, doublereal *phi, doublecomplex *u1, integer *ldu1, doublecomplex *u2, integer *ldu2, doublecomplex *v1t, integer *ldv1t, doublecomplex *v2t, integer *ldv2t, doublereal *b11d, doublereal *b11e, doublereal *b12d, doublereal *b12e, doublereal *b21d, doublereal *b21e, doublereal *b22d, doublereal *b22e, doublereal *rwork, integer *lrwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len);
/* zbdsqr.c */
int zbdsqr_(char *uplo, integer *n, integer *ncvt, integer *nru, integer *ncc, doublereal *d__, doublereal *e, doublecomplex *vt, integer *ldvt, doublecomplex *u, integer *ldu, doublecomplex *c__, integer *ldc, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zcgesv.c */
int zcgesv_(integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublecomplex *work, complex *swork, doublereal *rwork, integer *iter, integer *info);
/* zcposv.c */
int zcposv_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublecomplex *work, complex *swork, doublereal *rwork, integer *iter, integer *info, ftnlen uplo_len);
/* zdrscl.c */
int zdrscl_(integer *n, doublereal *sa, doublecomplex *sx, integer *incx);
/* zgbbrd.c */
int zgbbrd_(char *vect, integer *m, integer *n, integer *ncc, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublereal *d__, doublereal *e, doublecomplex *q, integer *ldq, doublecomplex *pt, integer *ldpt, doublecomplex *c__, integer *ldc, doublecomplex *work, doublereal *rwork, integer *info, ftnlen vect_len);
/* zgbcon.c */
int zgbcon_(char *norm, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen norm_len);
/* zgbequb.c */
int zgbequb_(integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* zgbequ.c */
int zgbequ_(integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* zgbrfs.c */
int zgbrfs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen trans_len);
/* zgbsv.c */
int zgbsv_(integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, integer *ipiv, doublecomplex *b, integer *ldb, integer *info);
/* zgbsvx.c */
int zgbsvx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* zgbsvxx.c */
int zgbsvxx_(char *fact, char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* zgbtf2.c */
int zgbtf2_(integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, integer *ipiv, integer *info);
/* zgbtrf.c */
int zgbtrf_(integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, integer *ipiv, integer *info);
/* zgbtrs.c */
int zgbtrs_(char *trans, integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen trans_len);
/* zgebak.c */
int zgebak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, doublereal *scale, integer *m, doublecomplex *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* zgebal.c */
int zgebal_(char *job, integer *n, doublecomplex *a, integer *lda, integer *ilo, integer *ihi, doublereal *scale, integer *info, ftnlen job_len);
/* zgebd2.c */
int zgebd2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tauq, doublecomplex *taup, doublecomplex *work, integer *info);
/* zgebrd.c */
int zgebrd_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tauq, doublecomplex *taup, doublecomplex *work, integer *lwork, integer *info);
/* zgecon.c */
int zgecon_(char *norm, integer *n, doublecomplex *a, integer *lda, doublereal *anorm, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen norm_len);
/* zgeequb.c */
int zgeequb_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* zgeequ.c */
int zgeequ_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *info);
/* zgees.c */
int zgees_(char *jobvs, char *sort, L_fp select, integer *n, doublecomplex *a, integer *lda, integer *sdim, doublecomplex *w, doublecomplex *vs, integer *ldvs, doublecomplex *work, integer *lwork, doublereal *rwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len);
/* zgeesx.c */
int zgeesx_(char *jobvs, char *sort, L_fp select, char *sense, integer *n, doublecomplex *a, integer *lda, integer *sdim, doublecomplex *w, doublecomplex *vs, integer *ldvs, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, logical *bwork, integer *info, ftnlen jobvs_len, ftnlen sort_len, ftnlen sense_len);
/* zgeev.c */
int zgeev_(char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* zgeevx.c */
int zgeevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *scale, doublereal *abnrm, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* zgehd2.c */
int zgehd2_(integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgehrd.c */
int zgehrd_(integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgelq2.c */
int zgelq2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgelq.c */
int zgelq_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *t, integer *tsize, doublecomplex *work, integer *lwork, integer *info);
/* zgelqf.c */
int zgelqf_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgelqt.c */
int zgelqt_(integer *m, integer *n, integer *mb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *work, integer *info);
/* zgels.c */
int zgels_(char *trans, integer *m, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen trans_len);
/* zgelsd.c */
int zgelsd_(integer *m, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *s, doublereal *rcond, integer *rank, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info);
/* zgelss.c */
int zgelss_(integer *m, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *s, doublereal *rcond, integer *rank, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info);
/* zgelsy.c */
int zgelsy_(integer *m, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *jpvt, doublereal *rcond, integer *rank, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info);
/* zgemlq.c */
int zgemlq_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *t, integer *tsize, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zgemlqt.c */
int zgemlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zgemqr.c */
int zgemqr_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *t, integer *tsize, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zgemqrt.c */
int zgemqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *nb, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zgeql2.c */
int zgeql2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgeqlf.c */
int zgeqlf_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgeqp3.c */
int zgeqp3_(integer *m, integer *n, doublecomplex *a, integer *lda, integer *jpvt, doublecomplex *tau, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info);
/* zgeqr2.c */
int zgeqr2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgeqr2p.c */
int zgeqr2p_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgeqr.c */
int zgeqr_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *t, integer *tsize, doublecomplex *work, integer *lwork, integer *info);
/* zgeqrf.c */
int zgeqrf_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgeqrfp.c */
int zgeqrfp_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgeqrt2.c */
int zgeqrt2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, integer *info);
/* zgeqrt3.c */
int zgeqrt3_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, integer *info);
/* zgeqrt.c */
int zgeqrt_(integer *m, integer *n, integer *nb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *work, integer *info);
/* zgerfs.c */
int zgerfs_(char *trans, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen trans_len);
/* zgerq2.c */
int zgerq2_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zgerqf.c */
int zgerqf_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zgesc2.c */
int zgesc2_(integer *n, doublecomplex *a, integer *lda, doublecomplex *rhs, integer *ipiv, integer *jpiv, doublereal *scale);
/* zgesdd.c */
int zgesdd_(char *jobz, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info, ftnlen jobz_len);
/* zgesv.c */
int zgesv_(integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info);
/* zgesvd.c */
int zgesvd_(char *jobu, char *jobvt, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len);
/* zgesvdx.c */
int zgesvdx_(char *jobu, char *jobvt, char *range, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, integer *ns, doublereal *s, doublecomplex *u, integer *ldu, doublecomplex *vt, integer *ldvt, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobvt_len, ftnlen range_len);
/* zgesvj.c */
int zgesvj_(char *joba, char *jobu, char *jobv, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *sva, integer *mv, doublecomplex *v, integer *ldv, doublecomplex *cwork, integer *lwork, doublereal *rwork, integer *lrwork, integer *info, ftnlen joba_len, ftnlen jobu_len, ftnlen jobv_len);
/* zgesvx.c */
int zgesvx_(char *fact, char *trans, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* zgesvxx.c */
int zgesvxx_(char *fact, char *trans, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, char *equed, doublereal *r__, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen trans_len, ftnlen equed_len);
/* zgetc2.c */
int zgetc2_(integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *jpiv, integer *info);
/* zgetf2.c */
int zgetf2_(integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info);
/* zgetrf2.c */
int zgetrf2_(integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info);
/* zgetrf.c */
int zgetrf_(integer *m, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info);
/* zgetri.c */
int zgetri_(integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info);
/* zgetrs.c */
int zgetrs_(char *trans, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen trans_len);
/* zgetsls.c */
int zgetsls_(char *trans, integer *m, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen trans_len);
/* zggbak.c */
int zggbak_(char *job, char *side, integer *n, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, integer *m, doublecomplex *v, integer *ldv, integer *info, ftnlen job_len, ftnlen side_len);
/* zggbal.c */
int zggbal_(char *job, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, doublereal *work, integer *info, ftnlen job_len);
/* zgges3.c */
int zgges3_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *sdim, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vsl, integer *ldvsl, doublecomplex *vsr, integer *ldvsr, doublecomplex *work, integer *lwork, doublereal *rwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* zgges.c */
int zgges_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *sdim, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vsl, integer *ldvsl, doublecomplex *vsr, integer *ldvsr, doublecomplex *work, integer *lwork, doublereal *rwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len);
/* zggesx.c */
int zggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp selctg, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *sdim, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vsl, integer *ldvsl, doublecomplex *vsr, integer *ldvsr, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *liwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen jobvsr_len, ftnlen sort_len, ftnlen sense_len);
/* zggev3.c */
int zggev3_(char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* zggev.c */
int zggev_(char *jobvl, char *jobvr, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobvl_len, ftnlen jobvr_len);
/* zggevx.c */
int zggevx_(char *balanc, char *jobvl, char *jobvr, char *sense, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *alpha, doublecomplex *beta, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *ilo, integer *ihi, doublereal *lscale, doublereal *rscale, doublereal *abnrm, doublereal *bbnrm, doublereal *rconde, doublereal *rcondv, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, logical *bwork, integer *info, ftnlen balanc_len, ftnlen jobvl_len, ftnlen jobvr_len, ftnlen sense_len);
/* zggglm.c */
int zggglm_(integer *n, integer *m, integer *p, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *d__, doublecomplex *x, doublecomplex *y, doublecomplex *work, integer *lwork, integer *info);
/* zgghd3.c */
int zgghd3_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, integer *info, ftnlen compq_len, ftnlen compz_len);
/* zgghrd.c */
int zgghrd_(char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, integer *info, ftnlen compq_len, ftnlen compz_len);
/* zgglse.c */
int zgglse_(integer *m, integer *n, integer *p, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *c__, doublecomplex *d__, doublecomplex *x, doublecomplex *work, integer *lwork, integer *info);
/* zggqrf.c */
int zggqrf_(integer *n, integer *m, integer *p, doublecomplex *a, integer *lda, doublecomplex *taua, doublecomplex *b, integer *ldb, doublecomplex *taub, doublecomplex *work, integer *lwork, integer *info);
/* zggrqf.c */
int zggrqf_(integer *m, integer *p, integer *n, doublecomplex *a, integer *lda, doublecomplex *taua, doublecomplex *b, integer *ldb, doublecomplex *taub, doublecomplex *work, integer *lwork, integer *info);
/* zggsvd3.c */
int zggsvd3_(char *jobu, char *jobv, char *jobq, integer *m, integer *n, integer *p, integer *k, integer *l, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *alpha, doublereal *beta, doublecomplex *u, integer *ldu, doublecomplex *v, integer *ldv, doublecomplex *q, integer *ldq, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* zggsvp3.c */
int zggsvp3_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *tola, doublereal *tolb, integer *k, integer *l, doublecomplex *u, integer *ldu, doublecomplex *v, integer *ldv, doublecomplex *q, integer *ldq, integer *iwork, doublereal *rwork, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* zgsvj0.c */
int zgsvj0_(char *jobv, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *d__, doublereal *sva, integer *mv, doublecomplex *v, integer *ldv, doublereal *eps, doublereal *sfmin, doublereal *tol, integer *nsweep, doublecomplex *work, integer *lwork, integer *info, ftnlen jobv_len);
/* zgsvj1.c */
int zgsvj1_(char *jobv, integer *m, integer *n, integer *n1, doublecomplex *a, integer *lda, doublecomplex *d__, doublereal *sva, integer *mv, doublecomplex *v, integer *ldv, doublereal *eps, doublereal *sfmin, doublereal *tol, integer *nsweep, doublecomplex *work, integer *lwork, integer *info, ftnlen jobv_len);
/* zgtcon.c */
int zgtcon_(char *norm, integer *n, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen norm_len);
/* zgtrfs.c */
int zgtrfs_(char *trans, integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *dlf, doublecomplex *df, doublecomplex *duf, doublecomplex *du2, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen trans_len);
/* zgtsv.c */
int zgtsv_(integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *b, integer *ldb, integer *info);
/* zgtsvx.c */
int zgtsvx_(char *fact, char *trans, integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *dlf, doublecomplex *df, doublecomplex *duf, doublecomplex *du2, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen trans_len);
/* zgttrf.c */
int zgttrf_(integer *n, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, integer *info);
/* zgttrs.c */
int zgttrs_(char *trans, integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen trans_len);
/* zgtts2.c */
int zgtts2_(integer *itrans, integer *n, integer *nrhs, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *du2, integer *ipiv, doublecomplex *b, integer *ldb);
/* zhb2st_kernels.c */
int zhb2st_kernels__(char *uplo, logical *wantz, integer *ttype, integer *st, integer *ed, integer *sweep, integer *n, integer *nb, integer *ib, doublecomplex *a, integer *lda, doublecomplex *v, doublecomplex *tau, integer *ldvt, doublecomplex *work, ftnlen uplo_len);
/* zhbev_2stage.c */
int zhbev_2stage__(char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbev.c */
int zhbev_(char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbevd_2stage.c */
int zhbevd_2stage__(char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbevd.c */
int zhbevd_(char *jobz, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbevx_2stage.c */
int zhbevx_2stage__(char *jobz, char *range, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublecomplex *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhbevx.c */
int zhbevx_(char *jobz, char *range, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublecomplex *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhbgst.c */
int zhbgst_(char *vect, char *uplo, integer *n, integer *ka, integer *kb, doublecomplex *ab, integer *ldab, doublecomplex *bb, integer *ldbb, doublecomplex *x, integer *ldx, doublecomplex *work, doublereal *rwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* zhbgv.c */
int zhbgv_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, doublecomplex *ab, integer *ldab, doublecomplex *bb, integer *ldbb, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbgvd.c */
int zhbgvd_(char *jobz, char *uplo, integer *n, integer *ka, integer *kb, doublecomplex *ab, integer *ldab, doublecomplex *bb, integer *ldbb, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhbgvx.c */
int zhbgvx_(char *jobz, char *range, char *uplo, integer *n, integer *ka, integer *kb, doublecomplex *ab, integer *ldab, doublecomplex *bb, integer *ldbb, doublecomplex *q, integer *ldq, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhbtrd.c */
int zhbtrd_(char *vect, char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *d__, doublereal *e, doublecomplex *q, integer *ldq, doublecomplex *work, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* zhecon_3.c */
int zhecon_3__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhecon.c */
int zhecon_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhecon_rook.c */
int zhecon_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zheequb.c */
int zheequb_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zheev_2stage.c */
int zheev_2stage__(char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zheev.c */
int zheev_(char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zheevd_2stage.c */
int zheevd_2stage__(char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zheevd.c */
int zheevd_(char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zheevr_2stage.c */
int zheevr_2stage__(char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, integer *isuppz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zheevr.c */
int zheevr_(char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, integer *isuppz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zheevx_2stage.c */
int zheevx_2stage__(char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zheevx.c */
int zheevx_(char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhegs2.c */
int zhegs2_(integer *itype, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhegst.c */
int zhegst_(integer *itype, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhegv_2stage.c */
int zhegv_2stage__(integer *itype, char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhegv.c */
int zhegv_(integer *itype, char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhegvd.c */
int zhegvd_(integer *itype, char *jobz, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *w, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhegvx.c */
int zhegvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zherfs.c */
int zherfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zhesv_aa_2stage.c */
int zhesv_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhesv_aa.c */
int zhesv_aa__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhesv.c */
int zhesv_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhesv_rk.c */
int zhesv_rk__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhesv_rook.c */
int zhesv_rook__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhesvx.c */
int zhesvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* zhesvxx.c */
int zhesvxx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zheswapr.c */
int zheswapr_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* zhetd2.c */
int zhetd2_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tau, integer *info, ftnlen uplo_len);
/* zhetf2.c */
int zhetf2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* zhetf2_rk.c */
int zhetf2_rk__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* zhetf2_rook.c */
int zhetf2_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* zhetrd_2stage.c */
int zhetrd_2stage__(char *vect, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tau, doublecomplex *hous2, integer *lhous2, doublecomplex *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen uplo_len);
/* zhetrd.c */
int zhetrd_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrd_he2hb.c */
int zhetrd_he2hb__(char *uplo, integer *n, integer *kd, doublecomplex *a, integer *lda, doublecomplex *ab, integer *ldab, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrf_aa_2stage.c */
int zhetrf_aa_2stage__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrf_aa.c */
int zhetrf_aa__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrf.c */
int zhetrf_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrf_rk.c */
int zhetrf_rk__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrf_rook.c */
int zhetrf_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetri2.c */
int zhetri2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetri2x.c */
int zhetri2x_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *nb, integer *info, ftnlen uplo_len);
/* zhetri_3.c */
int zhetri_3__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetri_3x.c */
int zhetri_3x__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *nb, integer *info, ftnlen uplo_len);
/* zhetri.c */
int zhetri_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhetri_rook.c */
int zhetri_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhetrs2.c */
int zhetrs2_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhetrs_3.c */
int zhetrs_3__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhetrs_aa_2stage.c */
int zhetrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhetrs_aa.c */
int zhetrs_aa__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zhetrs.c */
int zhetrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhetrs_rook.c */
int zhetrs_rook__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhfrk.c */
int zhfrk_(char *transr, char *uplo, char *trans, integer *n, integer *k, doublereal *alpha, doublecomplex *a, integer *lda, doublereal *beta, doublecomplex *c__, ftnlen transr_len, ftnlen uplo_len, ftnlen trans_len);
/* zhgeqz.c */
int zhgeqz_(char *job, char *compq, char *compz, integer *n, integer *ilo, integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *t, integer *ldt, doublecomplex *alpha, doublecomplex *beta, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen job_len, ftnlen compq_len, ftnlen compz_len);
/* zhpcon.c */
int zhpcon_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhpev.c */
int zhpev_(char *jobz, char *uplo, integer *n, doublecomplex *ap, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhpevd.c */
int zhpevd_(char *jobz, char *uplo, integer *n, doublecomplex *ap, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhpevx.c */
int zhpevx_(char *jobz, char *range, char *uplo, integer *n, doublecomplex *ap, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhpgst.c */
int zhpgst_(integer *itype, char *uplo, integer *n, doublecomplex *ap, doublecomplex *bp, integer *info, ftnlen uplo_len);
/* zhpgv.c */
int zhpgv_(integer *itype, char *jobz, char *uplo, integer *n, doublecomplex *ap, doublecomplex *bp, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhpgvd.c */
int zhpgvd_(integer *itype, char *jobz, char *uplo, integer *n, doublecomplex *ap, doublecomplex *bp, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen uplo_len);
/* zhpgvx.c */
int zhpgvx_(integer *itype, char *jobz, char *range, char *uplo, integer *n, doublecomplex *ap, doublecomplex *bp, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, doublecomplex *work, doublereal *rwork, integer *iwork, integer *ifail, integer *info, ftnlen jobz_len, ftnlen range_len, ftnlen uplo_len);
/* zhprfs.c */
int zhprfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zhpsv.c */
int zhpsv_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhpsvx.c */
int zhpsvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* zhptrd.c */
int zhptrd_(char *uplo, integer *n, doublecomplex *ap, doublereal *d__, doublereal *e, doublecomplex *tau, integer *info, ftnlen uplo_len);
/* zhptrf.c */
int zhptrf_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* zhptri.c */
int zhptri_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zhptrs.c */
int zhptrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zhsein.c */
int zhsein_(char *side, char *eigsrc, char *initv, logical *select, integer *n, doublecomplex *h__, integer *ldh, doublecomplex *w, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *mm, integer *m, doublecomplex *work, doublereal *rwork, integer *ifaill, integer *ifailr, integer *info, ftnlen side_len, ftnlen eigsrc_len, ftnlen initv_len);
/* zhseqr.c */
int zhseqr_(char *job, char *compz, integer *n, integer *ilo, integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *w, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compz_len);
/* zlabrd.c */
int zlabrd_(integer *m, integer *n, integer *nb, doublecomplex *a, integer *lda, doublereal *d__, doublereal *e, doublecomplex *tauq, doublecomplex *taup, doublecomplex *x, integer *ldx, doublecomplex *y, integer *ldy);
/* zlacgv.c */
int zlacgv_(integer *n, doublecomplex *x, integer *incx);
/* zlacn2.c */
int zlacn2_(integer *n, doublecomplex *v, doublecomplex *x, doublereal *est, integer *kase, integer *isave);
/* zlacon.c */
int zlacon_(integer *n, doublecomplex *v, doublecomplex *x, doublereal *est, integer *kase);
/* zlacp2.c */
int zlacp2_(char *uplo, integer *m, integer *n, doublereal *a, integer *lda, doublecomplex *b, integer *ldb, ftnlen uplo_len);
/* zlacpy.c */
int zlacpy_(char *uplo, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, ftnlen uplo_len);
/* zlacrm.c */
int zlacrm_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *b, integer *ldb, doublecomplex *c__, integer *ldc, doublereal *rwork);
/* zlacrt.c */
int zlacrt_(integer *n, doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy, doublecomplex *c__, doublecomplex *s);
/* zladiv.c */
void zladiv_(doublecomplex *ret_val, doublecomplex *x, doublecomplex *y);
/* zlaed0.c */
int zlaed0_(integer *qsiz, integer *n, doublereal *d__, doublereal *e, doublecomplex *q, integer *ldq, doublecomplex *qstore, integer *ldqs, doublereal *rwork, integer *iwork, integer *info);
/* zlaed7.c */
int zlaed7_(integer *n, integer *cutpnt, integer *qsiz, integer *tlvls, integer *curlvl, integer *curpbm, doublereal *d__, doublecomplex *q, integer *ldq, doublereal *rho, integer *indxq, doublereal *qstore, integer *qptr, integer *prmptr, integer *perm, integer *givptr, integer *givcol, doublereal *givnum, doublecomplex *work, doublereal *rwork, integer *iwork, integer *info);
/* zlaed8.c */
int zlaed8_(integer *k, integer *n, integer *qsiz, doublecomplex *q, integer *ldq, doublereal *d__, doublereal *rho, integer *cutpnt, doublereal *z__, doublereal *dlamda, doublecomplex *q2, integer *ldq2, doublereal *w, integer *indxp, integer *indx, integer *indxq, integer *perm, integer *givptr, integer *givcol, doublereal *givnum, integer *info);
/* zlaein.c */
int zlaein_(logical *rightv, logical *noinit, integer *n, doublecomplex *h__, integer *ldh, doublecomplex *w, doublecomplex *v, doublecomplex *b, integer *ldb, doublereal *rwork, doublereal *eps3, doublereal *smlnum, integer *info);
/* zlaesy.c */
int zlaesy_(doublecomplex *a, doublecomplex *b, doublecomplex *c__, doublecomplex *rt1, doublecomplex *rt2, doublecomplex *evscal, doublecomplex *cs1, doublecomplex *sn1);
/* zlaev2.c */
int zlaev2_(doublecomplex *a, doublecomplex *b, doublecomplex *c__, doublereal *rt1, doublereal *rt2, doublereal *cs1, doublecomplex *sn1);
/* zlag2c.c */
int zlag2c_(integer *m, integer *n, doublecomplex *a, integer *lda, complex *sa, integer *ldsa, integer *info);
/* zla_gbamv.c */
int zla_gbamv__(integer *trans, integer *m, integer *n, integer *kl, integer *ku, doublereal *alpha, doublecomplex *ab, integer *ldab, doublecomplex *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* zla_gbrcond_c.c */
doublereal zla_gbrcond_c__(char *trans, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, doublereal *c__, logical *capply, integer *info, doublecomplex *work, doublereal *rwork, ftnlen trans_len);
/* zla_gbrcond_x.c */
doublereal zla_gbrcond_x__(char *trans, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, doublecomplex *x, integer *info, doublecomplex *work, doublereal *rwork, ftnlen trans_len);
/* zla_gbrfsx_extended.c */
int zla_gbrfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *kl, integer *ku, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, integer *ipiv, logical *colequ, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublecomplex *res, doublereal *ayb, doublecomplex *dy, doublecomplex *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info);
/* zla_gbrpvgrw.c */
doublereal zla_gbrpvgrw__(integer *n, integer *kl, integer *ku, integer *ncols, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb);
/* zla_geamv.c */
int zla_geamv__(integer *trans, integer *m, integer *n, doublereal *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* zla_gercond_c.c */
doublereal zla_gercond_c__(char *trans, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublereal *c__, logical *capply, integer *info, doublecomplex *work, doublereal *rwork, ftnlen trans_len);
/* zla_gercond_x.c */
doublereal zla_gercond_x__(char *trans, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *x, integer *info, doublecomplex *work, doublereal *rwork, ftnlen trans_len);
/* zla_gerfsx_extended.c */
int zla_gerfsx_extended__(integer *prec_type__, integer *trans_type__, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, logical *colequ, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *errs_n__, doublereal *errs_c__, doublecomplex *res, doublereal *ayb, doublecomplex *dy, doublecomplex *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info);
/* zla_gerpvgrw.c */
doublereal zla_gerpvgrw__(integer *n, integer *ncols, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf);
/* zlags2.c */
int zlags2_(logical *upper, doublereal *a1, doublecomplex *a2, doublereal *a3, doublereal *b1, doublecomplex *b2, doublereal *b3, doublereal *csu, doublecomplex *snu, doublereal *csv, doublecomplex *snv, doublereal *csq, doublecomplex *snq);
/* zlagtm.c */
int zlagtm_(char *trans, integer *n, integer *nrhs, doublereal *alpha, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, doublecomplex *x, integer *ldx, doublereal *beta, doublecomplex *b, integer *ldb, ftnlen trans_len);
/* zla_heamv.c */
int zla_heamv__(integer *uplo, integer *n, doublereal *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* zlahef_aa.c */
int zlahef_aa__(char *uplo, integer *j1, integer *m, integer *nb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *h__, integer *ldh, doublecomplex *work, ftnlen uplo_len);
/* zlahef.c */
int zlahef_(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zlahef_rk.c */
int zlahef_rk__(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zlahef_rook.c */
int zlahef_rook__(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zla_hercond_c.c */
doublereal zla_hercond_c__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublereal *c__, logical *capply, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_hercond_x.c */
doublereal zla_hercond_x__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *x, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_herfsx_extended.c */
int zla_herfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, logical *colequ, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublecomplex *res, doublereal *ayb, doublecomplex *dy, doublecomplex *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* zla_herpvgrw.c */
doublereal zla_herpvgrw__(char *uplo, integer *n, integer *info, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublereal *work, ftnlen uplo_len);
/* zlahqr.c */
int zlahqr_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *w, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, integer *info);
/* zlahr2.c */
int zlahr2_(integer *n, integer *k, integer *nb, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *t, integer *ldt, doublecomplex *y, integer *ldy);
/* zlaic1.c */
int zlaic1_(integer *job, integer *j, doublecomplex *x, doublereal *sest, doublecomplex *w, doublecomplex *gamma, doublereal *sestpr, doublecomplex *s, doublecomplex *c__);
/* zla_lin_berr.c */
int zla_lin_berr__(integer *n, integer *nz, integer *nrhs, doublecomplex *res, doublereal *ayb, doublereal *berr);
/* zlals0.c */
int zlals0_(integer *icompq, integer *nl, integer *nr, integer *sqre, integer *nrhs, doublecomplex *b, integer *ldb, doublecomplex *bx, integer *ldbx, integer *perm, integer *givptr, integer *givcol, integer *ldgcol, doublereal *givnum, integer *ldgnum, doublereal *poles, doublereal *difl, doublereal *difr, doublereal *z__, integer *k, doublereal *c__, doublereal *s, doublereal *rwork, integer *info);
/* zlalsa.c */
int zlalsa_(integer *icompq, integer *smlsiz, integer *n, integer *nrhs, doublecomplex *b, integer *ldb, doublecomplex *bx, integer *ldbx, doublereal *u, integer *ldu, doublereal *vt, integer *k, doublereal *difl, doublereal *difr, doublereal *z__, doublereal *poles, integer *givptr, integer *givcol, integer *ldgcol, integer *perm, doublereal *givnum, doublereal *c__, doublereal *s, doublereal *rwork, integer *iwork, integer *info);
/* zlalsd.c */
int zlalsd_(char *uplo, integer *smlsiz, integer *n, integer *nrhs, doublereal *d__, doublereal *e, doublecomplex *b, integer *ldb, doublereal *rcond, integer *rank, doublecomplex *work, doublereal *rwork, integer *iwork, integer *info, ftnlen uplo_len);
/* zlamswlq.c */
int zlamswlq_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zlamtsqr.c */
int zlamtsqr_(char *side, char *trans, integer *m, integer *n, integer *k, integer *mb, integer *nb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zlangb.c */
doublereal zlangb_(char *norm, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublereal *work, ftnlen norm_len);
/* zlange.c */
doublereal zlange_(char *norm, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *work, ftnlen norm_len);
/* zlangt.c */
doublereal zlangt_(char *norm, integer *n, doublecomplex *dl, doublecomplex *d__, doublecomplex *du, ftnlen norm_len);
/* zlanhb.c */
doublereal zlanhb_(char *norm, char *uplo, integer *n, integer *k, doublecomplex *ab, integer *ldab, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlanhe.c */
doublereal zlanhe_(char *norm, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlanhf.c */
doublereal zlanhf_(char *norm, char *transr, char *uplo, integer *n, doublecomplex *a, doublereal *work, ftnlen norm_len, ftnlen transr_len, ftnlen uplo_len);
/* zlanhp.c */
doublereal zlanhp_(char *norm, char *uplo, integer *n, doublecomplex *ap, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlanhs.c */
doublereal zlanhs_(char *norm, integer *n, doublecomplex *a, integer *lda, doublereal *work, ftnlen norm_len);
/* zlanht.c */
doublereal zlanht_(char *norm, integer *n, doublereal *d__, doublecomplex *e, ftnlen norm_len);
/* zlansb.c */
doublereal zlansb_(char *norm, char *uplo, integer *n, integer *k, doublecomplex *ab, integer *ldab, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlansp.c */
doublereal zlansp_(char *norm, char *uplo, integer *n, doublecomplex *ap, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlansy.c */
doublereal zlansy_(char *norm, char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *work, ftnlen norm_len, ftnlen uplo_len);
/* zlantb.c */
doublereal zlantb_(char *norm, char *uplo, char *diag, integer *n, integer *k, doublecomplex *ab, integer *ldab, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* zlantp.c */
doublereal zlantp_(char *norm, char *uplo, char *diag, integer *n, doublecomplex *ap, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* zlantr.c */
doublereal zlantr_(char *norm, char *uplo, char *diag, integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *work, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* zlapll.c */
int zlapll_(integer *n, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublereal *ssmin);
/* zlapmr.c */
int zlapmr_(logical *forwrd, integer *m, integer *n, doublecomplex *x, integer *ldx, integer *k);
/* zlapmt.c */
int zlapmt_(logical *forwrd, integer *m, integer *n, doublecomplex *x, integer *ldx, integer *k);
/* zla_porcond_c.c */
doublereal zla_porcond_c__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, doublereal *c__, logical *capply, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_porcond_x.c */
doublereal zla_porcond_x__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, doublecomplex *x, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_porfsx_extended.c */
int zla_porfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, logical *colequ, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublecomplex *res, doublereal *ayb, doublecomplex *dy, doublecomplex *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* zla_porpvgrw.c */
doublereal zla_porpvgrw__(char *uplo, integer *ncols, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, doublereal *work, ftnlen uplo_len);
/* zlaqgb.c */
int zlaqgb_(integer *m, integer *n, integer *kl, integer *ku, doublecomplex *ab, integer *ldab, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, char *equed, ftnlen equed_len);
/* zlaqge.c */
int zlaqge_(integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *r__, doublereal *c__, doublereal *rowcnd, doublereal *colcnd, doublereal *amax, char *equed, ftnlen equed_len);
/* zlaqhb.c */
int zlaqhb_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlaqhe.c */
int zlaqhe_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlaqhp.c */
int zlaqhp_(char *uplo, integer *n, doublecomplex *ap, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlaqp2.c */
int zlaqp2_(integer *m, integer *n, integer *offset, doublecomplex *a, integer *lda, integer *jpvt, doublecomplex *tau, doublereal *vn1, doublereal *vn2, doublecomplex *work);
/* zlaqps.c */
int zlaqps_(integer *m, integer *n, integer *offset, integer *nb, integer *kb, doublecomplex *a, integer *lda, integer *jpvt, doublecomplex *tau, doublereal *vn1, doublereal *vn2, doublecomplex *auxv, doublecomplex *f, integer *ldf);
/* zlaqr0.c */
int zlaqr0_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *w, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, integer *info);
/* zlaqr1.c */
int zlaqr1_(integer *n, doublecomplex *h__, integer *ldh, doublecomplex *s1, doublecomplex *s2, doublecomplex *v);
/* zlaqr2.c */
int zlaqr2_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, doublecomplex *h__, integer *ldh, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, integer *ns, integer *nd, doublecomplex *sh, doublecomplex *v, integer *ldv, integer *nh, doublecomplex *t, integer *ldt, integer *nv, doublecomplex *wv, integer *ldwv, doublecomplex *work, integer *lwork);
/* zlaqr3.c */
int zlaqr3_(logical *wantt, logical *wantz, integer *n, integer *ktop, integer *kbot, integer *nw, doublecomplex *h__, integer *ldh, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, integer *ns, integer *nd, doublecomplex *sh, doublecomplex *v, integer *ldv, integer *nh, doublecomplex *t, integer *ldt, integer *nv, doublecomplex *wv, integer *ldwv, doublecomplex *work, integer *lwork);
/* zlaqr4.c */
int zlaqr4_(logical *wantt, logical *wantz, integer *n, integer *ilo, integer *ihi, doublecomplex *h__, integer *ldh, doublecomplex *w, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, integer *info);
/* zlaqr5.c */
int zlaqr5_(logical *wantt, logical *wantz, integer *kacc22, integer *n, integer *ktop, integer *kbot, integer *nshfts, doublecomplex *s, doublecomplex *h__, integer *ldh, integer *iloz, integer *ihiz, doublecomplex *z__, integer *ldz, doublecomplex *v, integer *ldv, doublecomplex *u, integer *ldu, integer *nv, doublecomplex *wv, integer *ldwv, integer *nh, doublecomplex *wh, integer *ldwh);
/* zlaqsb.c */
int zlaqsb_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlaqsp.c */
int zlaqsp_(char *uplo, integer *n, doublecomplex *ap, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlaqsy.c */
int zlaqsy_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, char *equed, ftnlen uplo_len, ftnlen equed_len);
/* zlar1v.c */
int zlar1v_(integer *n, integer *b1, integer *bn, doublereal *lambda, doublereal *d__, doublereal *l, doublereal *ld, doublereal *lld, doublereal *pivmin, doublereal *gaptol, doublecomplex *z__, logical *wantnc, integer *negcnt, doublereal *ztz, doublereal *mingma, integer *r__, integer *isuppz, doublereal *nrminv, doublereal *resid, doublereal *rqcorr, doublereal *work);
/* zlar2v.c */
int zlar2v_(integer *n, doublecomplex *x, doublecomplex *y, doublecomplex *z__, integer *incx, doublereal *c__, doublecomplex *s, integer *incc);
/* zlarcm.c */
int zlarcm_(integer *m, integer *n, doublereal *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *c__, integer *ldc, doublereal *rwork);
/* zlarfb.c */
int zlarfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* zlarf.c */
int zlarf_(char *side, integer *m, integer *n, doublecomplex *v, integer *incv, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, ftnlen side_len);
/* zlarfg.c */
int zlarfg_(integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *tau);
/* zlarfgp.c */
int zlarfgp_(integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *tau);
/* zlarft.c */
int zlarft_(char *direct, char *storev, integer *n, integer *k, doublecomplex *v, integer *ldv, doublecomplex *tau, doublecomplex *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* zlarfx.c */
int zlarfx_(char *side, integer *m, integer *n, doublecomplex *v, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, ftnlen side_len);
/* zlarfy.c */
int zlarfy_(char *uplo, integer *n, doublecomplex *v, integer *incv, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, ftnlen uplo_len);
/* zlargv.c */
int zlargv_(integer *n, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublereal *c__, integer *incc);
/* zlarnv.c */
int zlarnv_(integer *idist, integer *iseed, integer *n, doublecomplex *x);
/* zlarrv.c */
int zlarrv_(integer *n, doublereal *vl, doublereal *vu, doublereal *d__, doublereal *l, doublereal *pivmin, integer *isplit, integer *m, integer *dol, integer *dou, doublereal *minrgp, doublereal *rtol1, doublereal *rtol2, doublereal *w, doublereal *werr, doublereal *wgap, integer *iblock, integer *indexw, doublereal *gers, doublecomplex *z__, integer *ldz, integer *isuppz, doublereal *work, integer *iwork, integer *info);
/* zlarscl2.c */
int zlarscl2_(integer *m, integer *n, doublereal *d__, doublecomplex *x, integer *ldx);
/* zlartg.c */
int zlartg_(doublecomplex *f, doublecomplex *g, doublereal *cs, doublecomplex *sn, doublecomplex *r__);
/* zlartv.c */
int zlartv_(integer *n, doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, doublereal *c__, doublecomplex *s, integer *incc);
/* zlarzb.c */
int zlarzb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* zlarz.c */
int zlarz_(char *side, integer *m, integer *n, integer *l, doublecomplex *v, integer *incv, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, ftnlen side_len);
/* zlarzt.c */
int zlarzt_(char *direct, char *storev, integer *n, integer *k, doublecomplex *v, integer *ldv, doublecomplex *tau, doublecomplex *t, integer *ldt, ftnlen direct_len, ftnlen storev_len);
/* zlascl2.c */
int zlascl2_(integer *m, integer *n, doublereal *d__, doublecomplex *x, integer *ldx);
/* zlascl.c */
int zlascl_(char *type__, integer *kl, integer *ku, doublereal *cfrom, doublereal *cto, integer *m, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen type_len);
/* zlaset.c */
int zlaset_(char *uplo, integer *m, integer *n, doublecomplex *alpha, doublecomplex *beta, doublecomplex *a, integer *lda, ftnlen uplo_len);
/* zlasr.c */
int zlasr_(char *side, char *pivot, char *direct, integer *m, integer *n, doublereal *c__, doublereal *s, doublecomplex *a, integer *lda, ftnlen side_len, ftnlen pivot_len, ftnlen direct_len);
/* zlassq.c */
int zlassq_(integer *n, doublecomplex *x, integer *incx, doublereal *scale, doublereal *sumsq);
/* zlaswlq.c */
int zlaswlq_(integer *m, integer *n, integer *mb, integer *nb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *work, integer *lwork, integer *info);
/* zlaswp.c */
int zlaswp_(integer *n, doublecomplex *a, integer *lda, integer *k1, integer *k2, integer *ipiv, integer *incx);
/* zla_syamv.c */
int zla_syamv__(integer *uplo, integer *n, doublereal *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublereal *beta, doublereal *y, integer *incy);
/* zlasyf_aa.c */
int zlasyf_aa__(char *uplo, integer *j1, integer *m, integer *nb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *h__, integer *ldh, doublecomplex *work, ftnlen uplo_len);
/* zlasyf.c */
int zlasyf_(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zlasyf_rk.c */
int zlasyf_rk__(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zlasyf_rook.c */
int zlasyf_rook__(char *uplo, integer *n, integer *nb, integer *kb, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *w, integer *ldw, integer *info, ftnlen uplo_len);
/* zla_syrcond_c.c */
doublereal zla_syrcond_c__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublereal *c__, logical *capply, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_syrcond_x.c */
doublereal zla_syrcond_x__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *x, integer *info, doublecomplex *work, doublereal *rwork, ftnlen uplo_len);
/* zla_syrfsx_extended.c */
int zla_syrfsx_extended__(integer *prec_type__, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, logical *colequ, doublereal *c__, doublecomplex *b, integer *ldb, doublecomplex *y, integer *ldy, doublereal *berr_out__, integer *n_norms__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, doublecomplex *res, doublereal *ayb, doublecomplex *dy, doublecomplex *y_tail__, doublereal *rcond, integer *ithresh, doublereal *rthresh, doublereal *dz_ub__, logical *ignore_cwise__, integer *info, ftnlen uplo_len);
/* zla_syrpvgrw.c */
doublereal zla_syrpvgrw__(char *uplo, integer *n, integer *info, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublereal *work, ftnlen uplo_len);
/* zlat2c.c */
int zlat2c_(char *uplo, integer *n, doublecomplex *a, integer *lda, complex *sa, integer *ldsa, integer *info, ftnlen uplo_len);
/* zlatbs.c */
int zlatbs_(char *uplo, char *trans, char *diag, char *normin, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublecomplex *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* zlatdf.c */
int zlatdf_(integer *ijob, integer *n, doublecomplex *z__, integer *ldz, doublecomplex *rhs, doublereal *rdsum, doublereal *rdscal, integer *ipiv, integer *jpiv);
/* zlatps.c */
int zlatps_(char *uplo, char *trans, char *diag, char *normin, integer *n, doublecomplex *ap, doublecomplex *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* zlatrd.c */
int zlatrd_(char *uplo, integer *n, integer *nb, doublecomplex *a, integer *lda, doublereal *e, doublecomplex *tau, doublecomplex *w, integer *ldw, ftnlen uplo_len);
/* zlatrs.c */
int zlatrs_(char *uplo, char *trans, char *diag, char *normin, integer *n, doublecomplex *a, integer *lda, doublecomplex *x, doublereal *scale, doublereal *cnorm, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len, ftnlen normin_len);
/* zlatrz.c */
int zlatrz_(integer *m, integer *n, integer *l, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work);
/* zlatsqr.c */
int zlatsqr_(integer *m, integer *n, integer *mb, integer *nb, doublecomplex *a, integer *lda, doublecomplex *t, integer *ldt, doublecomplex *work, integer *lwork, integer *info);
/* zlauu2.c */
int zlauu2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zlauum.c */
int zlauum_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zla_wwaddw.c */
int zla_wwaddw__(integer *n, doublecomplex *x, doublecomplex *y, doublecomplex *w);
/* zpbcon.c */
int zpbcon_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *anorm, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zpbequ.c */
int zpbequ_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *s, doublereal *scond, doublereal *amax, integer *info, ftnlen uplo_len);
/* zpbrfs.c */
int zpbrfs_(char *uplo, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zpbstf.c */
int zpbstf_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* zpbsv.c */
int zpbsv_(char *uplo, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zpbsvx.c */
int zpbsvx_(char *fact, char *uplo, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *ldafb, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zpbtf2.c */
int zpbtf2_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* zpbtrf.c */
int zpbtrf_(char *uplo, integer *n, integer *kd, doublecomplex *ab, integer *ldab, integer *info, ftnlen uplo_len);
/* zpbtrs.c */
int zpbtrs_(char *uplo, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zpftrf.c */
int zpftrf_(char *transr, char *uplo, integer *n, doublecomplex *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* zpftri.c */
int zpftri_(char *transr, char *uplo, integer *n, doublecomplex *a, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* zpftrs.c */
int zpftrs_(char *transr, char *uplo, integer *n, integer *nrhs, doublecomplex *a, doublecomplex *b, integer *ldb, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* zpocon.c */
int zpocon_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *anorm, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zpoequb.c */
int zpoequb_(integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, integer *info);
/* zpoequ.c */
int zpoequ_(integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, integer *info);
/* zporfs.c */
int zporfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zposv.c */
int zposv_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zposvx.c */
int zposvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zposvxx.c */
int zposvxx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zpotf2.c */
int zpotf2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zpotrf2.c */
int zpotrf2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zpotrf.c */
int zpotrf_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zpotri.c */
int zpotri_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* zpotrs.c */
int zpotrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zppcon.c */
int zppcon_(char *uplo, integer *n, doublecomplex *ap, doublereal *anorm, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zppequ.c */
int zppequ_(char *uplo, integer *n, doublecomplex *ap, doublereal *s, doublereal *scond, doublereal *amax, integer *info, ftnlen uplo_len);
/* zpprfs.c */
int zpprfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zppsv.c */
int zppsv_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zppsvx.c */
int zppsvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zpptrf.c */
int zpptrf_(char *uplo, integer *n, doublecomplex *ap, integer *info, ftnlen uplo_len);
/* zpptri.c */
int zpptri_(char *uplo, integer *n, doublecomplex *ap, integer *info, ftnlen uplo_len);
/* zpptrs.c */
int zpptrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zpstf2.c */
int zpstf2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *piv, integer *rank, doublereal *tol, doublereal *work, integer *info, ftnlen uplo_len);
/* zpstrf.c */
int zpstrf_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *piv, integer *rank, doublereal *tol, doublereal *work, integer *info, ftnlen uplo_len);
/* zptcon.c */
int zptcon_(integer *n, doublereal *d__, doublecomplex *e, doublereal *anorm, doublereal *rcond, doublereal *rwork, integer *info);
/* zpteqr.c */
int zpteqr_(char *compz, integer *n, doublereal *d__, doublereal *e, doublecomplex *z__, integer *ldz, doublereal *work, integer *info, ftnlen compz_len);
/* zptrfs.c */
int zptrfs_(char *uplo, integer *n, integer *nrhs, doublereal *d__, doublecomplex *e, doublereal *df, doublecomplex *ef, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zptsv.c */
int zptsv_(integer *n, integer *nrhs, doublereal *d__, doublecomplex *e, doublecomplex *b, integer *ldb, integer *info);
/* zptsvx.c */
int zptsvx_(char *fact, integer *n, integer *nrhs, doublereal *d__, doublecomplex *e, doublereal *df, doublecomplex *ef, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len);
/* zpttrf.c */
int zpttrf_(integer *n, doublereal *d__, doublecomplex *e, integer *info);
/* zpttrs.c */
int zpttrs_(char *uplo, integer *n, integer *nrhs, doublereal *d__, doublecomplex *e, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zptts2.c */
int zptts2_(integer *iuplo, integer *n, integer *nrhs, doublereal *d__, doublecomplex *e, doublecomplex *b, integer *ldb);
/* zrot.c */
int zrot_(integer *n, doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy, doublereal *c__, doublecomplex *s);
/* zspcon.c */
int zspcon_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zspmv.c */
int zspmv_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *ap, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen uplo_len);
/* zspr.c */
int zspr_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *ap, ftnlen uplo_len);
/* zsprfs.c */
int zsprfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zspsv.c */
int zspsv_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zspsvx.c */
int zspsvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *afp, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* zsptrf.c */
int zsptrf_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, integer *info, ftnlen uplo_len);
/* zsptri.c */
int zsptri_(char *uplo, integer *n, doublecomplex *ap, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsptrs.c */
int zsptrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *ap, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zstedc.c */
int zstedc_(char *compz, integer *n, doublereal *d__, doublereal *e, doublecomplex *z__, integer *ldz, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *liwork, integer *info, ftnlen compz_len);
/* zstegr.c */
int zstegr_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, doublereal *abstol, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, integer *isuppz, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* zstein.c */
int zstein_(integer *n, doublereal *d__, doublereal *e, integer *m, doublereal *w, integer *iblock, integer *isplit, doublecomplex *z__, integer *ldz, doublereal *work, integer *iwork, integer *ifail, integer *info);
/* zstemr.c */
int zstemr_(char *jobz, char *range, integer *n, doublereal *d__, doublereal *e, doublereal *vl, doublereal *vu, integer *il, integer *iu, integer *m, doublereal *w, doublecomplex *z__, integer *ldz, integer *nzc, integer *isuppz, logical *tryrac, doublereal *work, integer *lwork, integer *iwork, integer *liwork, integer *info, ftnlen jobz_len, ftnlen range_len);
/* zsteqr.c */
int zsteqr_(char *compz, integer *n, doublereal *d__, doublereal *e, doublecomplex *z__, integer *ldz, doublereal *work, integer *info, ftnlen compz_len);
/* zsycon_3.c */
int zsycon_3__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsycon.c */
int zsycon_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsycon_rook.c */
int zsycon_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublereal *anorm, doublereal *rcond, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsyconv.c */
int zsyconv_(char *uplo, char *way, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *e, integer *info, ftnlen uplo_len, ftnlen way_len);
/* zsyconvf.c */
int zsyconvf_(char *uplo, char *way, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* zsyconvf_rook.c */
int zsyconvf_rook__(char *uplo, char *way, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, integer *info, ftnlen uplo_len, ftnlen way_len);
/* zsyequb.c */
int zsyequb_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublereal *s, doublereal *scond, doublereal *amax, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsymv.c */
int zsymv_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *incy, ftnlen uplo_len);
/* zsyr.c */
int zsyr_(char *uplo, integer *n, doublecomplex *alpha, doublecomplex *x, integer *incx, doublecomplex *a, integer *lda, ftnlen uplo_len);
/* zsyrfs.c */
int zsyrfs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len);
/* zsysv_aa_2stage.c */
int zsysv_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsysv_aa.c */
int zsysv_aa__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsysv.c */
int zsysv_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsysv_rk.c */
int zsysv_rk__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsysv_rook.c */
int zsysv_rook__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsysvx.c */
int zsysvx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *ferr, doublereal *berr, doublecomplex *work, integer *lwork, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len);
/* zsysvxx.c */
int zsysvxx_(char *fact, char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *af, integer *ldaf, integer *ipiv, char *equed, doublereal *s, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *rcond, doublereal *rpvgrw, doublereal *berr, integer *n_err_bnds__, doublereal *err_bnds_norm__, doublereal *err_bnds_comp__, integer *nparams, doublereal *params, doublecomplex *work, doublereal *rwork, integer *info, ftnlen fact_len, ftnlen uplo_len, ftnlen equed_len);
/* zsyswapr.c */
int zsyswapr_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *i1, integer *i2, ftnlen uplo_len);
/* zsytf2.c */
int zsytf2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* zsytf2_rk.c */
int zsytf2_rk__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, integer *info, ftnlen uplo_len);
/* zsytf2_rook.c */
int zsytf2_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, integer *info, ftnlen uplo_len);
/* zsytrf_aa_2stage.c */
int zsytrf_aa_2stage__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytrf_aa.c */
int zsytrf_aa__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytrf.c */
int zsytrf_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytrf_rk.c */
int zsytrf_rk__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytrf_rook.c */
int zsytrf_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytri2.c */
int zsytri2_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytri2x.c */
int zsytri2x_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *nb, integer *info, ftnlen uplo_len);
/* zsytri_3.c */
int zsytri_3__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytri_3x.c */
int zsytri_3x__(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *work, integer *nb, integer *info, ftnlen uplo_len);
/* zsytri.c */
int zsytri_(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsytri_rook.c */
int zsytri_rook__(char *uplo, integer *n, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsytrs2.c */
int zsytrs2_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zsytrs_3.c */
int zsytrs_3__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *e, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zsytrs_aa_2stage.c */
int zsytrs_aa_2stage__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *tb, integer *ltb, integer *ipiv, integer *ipiv2, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zsytrs_aa.c */
int zsytrs_aa__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zsytrs.c */
int zsytrs_(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* zsytrs_rook.c */
int zsytrs_rook__(char *uplo, integer *n, integer *nrhs, doublecomplex *a, integer *lda, integer *ipiv, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len);
/* ztbcon.c */
int ztbcon_(char *norm, char *uplo, char *diag, integer *n, integer *kd, doublecomplex *ab, integer *ldab, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ztbrfs.c */
int ztbrfs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztbtrs.c */
int ztbtrs_(char *uplo, char *trans, char *diag, integer *n, integer *kd, integer *nrhs, doublecomplex *ab, integer *ldab, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztfsm.c */
int ztfsm_(char *transr, char *side, char *uplo, char *trans, char *diag, integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, doublecomplex *b, integer *ldb, ftnlen transr_len, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztftri.c */
int ztftri_(char *transr, char *uplo, char *diag, integer *n, doublecomplex *a, integer *info, ftnlen transr_len, ftnlen uplo_len, ftnlen diag_len);
/* ztfttp.c */
int ztfttp_(char *transr, char *uplo, integer *n, doublecomplex *arf, doublecomplex *ap, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ztfttr.c */
int ztfttr_(char *transr, char *uplo, integer *n, doublecomplex *arf, doublecomplex *a, integer *lda, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ztgevc.c */
int ztgevc_(char *side, char *howmny, logical *select, integer *n, doublecomplex *s, integer *lds, doublecomplex *p, integer *ldp, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *mm, integer *m, doublecomplex *work, doublereal *rwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ztgex2.c */
int ztgex2_(logical *wantq, logical *wantz, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, integer *j1, integer *info);
/* ztgexc.c */
int ztgexc_(logical *wantq, logical *wantz, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, integer *ifst, integer *ilst, integer *info);
/* ztgsen.c */
int ztgsen_(integer *ijob, logical *wantq, logical *wantz, logical *select, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *alpha, doublecomplex *beta, doublecomplex *q, integer *ldq, doublecomplex *z__, integer *ldz, integer *m, doublereal *pl, doublereal *pr, doublereal *dif, doublecomplex *work, integer *lwork, integer *iwork, integer *liwork, integer *info);
/* ztgsja.c */
int ztgsja_(char *jobu, char *jobv, char *jobq, integer *m, integer *p, integer *n, integer *k, integer *l, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublereal *tola, doublereal *tolb, doublereal *alpha, doublereal *beta, doublecomplex *u, integer *ldu, doublecomplex *v, integer *ldv, doublecomplex *q, integer *ldq, doublecomplex *work, integer *ncycle, integer *info, ftnlen jobu_len, ftnlen jobv_len, ftnlen jobq_len);
/* ztgsna.c */
int ztgsna_(char *job, char *howmny, logical *select, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublereal *s, doublereal *dif, integer *mm, integer *m, doublecomplex *work, integer *lwork, integer *iwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* ztgsy2.c */
int ztgsy2_(char *trans, integer *ijob, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *c__, integer *ldc, doublecomplex *d__, integer *ldd, doublecomplex *e, integer *lde, doublecomplex *f, integer *ldf, doublereal *scale, doublereal *rdsum, doublereal *rdscal, integer *info, ftnlen trans_len);
/* ztgsyl.c */
int ztgsyl_(char *trans, integer *ijob, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *c__, integer *ldc, doublecomplex *d__, integer *ldd, doublecomplex *e, integer *lde, doublecomplex *f, integer *ldf, doublereal *scale, doublereal *dif, doublecomplex *work, integer *lwork, integer *iwork, integer *info, ftnlen trans_len);
/* ztpcon.c */
int ztpcon_(char *norm, char *uplo, char *diag, integer *n, doublecomplex *ap, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ztplqt2.c */
int ztplqt2_(integer *m, integer *n, integer *l, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *t, integer *ldt, integer *info);
/* ztplqt.c */
int ztplqt_(integer *m, integer *n, integer *l, integer *mb, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *t, integer *ldt, doublecomplex *work, integer *info);
/* ztpmlqt.c */
int ztpmlqt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *mb, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* ztpmqrt.c */
int ztpmqrt_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, integer *nb, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* ztpqrt2.c */
int ztpqrt2_(integer *m, integer *n, integer *l, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *t, integer *ldt, integer *info);
/* ztpqrt.c */
int ztpqrt_(integer *m, integer *n, integer *l, integer *nb, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *t, integer *ldt, doublecomplex *work, integer *info);
/* ztprfb.c */
int ztprfb_(char *side, char *trans, char *direct, char *storev, integer *m, integer *n, integer *k, integer *l, doublecomplex *v, integer *ldv, doublecomplex *t, integer *ldt, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *work, integer *ldwork, ftnlen side_len, ftnlen trans_len, ftnlen direct_len, ftnlen storev_len);
/* ztprfs.c */
int ztprfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztptri.c */
int ztptri_(char *uplo, char *diag, integer *n, doublecomplex *ap, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ztptrs.c */
int ztptrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublecomplex *ap, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztpttf.c */
int ztpttf_(char *transr, char *uplo, integer *n, doublecomplex *ap, doublecomplex *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ztpttr.c */
int ztpttr_(char *uplo, integer *n, doublecomplex *ap, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len);
/* ztrcon.c */
int ztrcon_(char *norm, char *uplo, char *diag, integer *n, doublecomplex *a, integer *lda, doublereal *rcond, doublecomplex *work, doublereal *rwork, integer *info, ftnlen norm_len, ftnlen uplo_len, ftnlen diag_len);
/* ztrevc3.c */
int ztrevc3_(char *side, char *howmny, logical *select, integer *n, doublecomplex *t, integer *ldt, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *mm, integer *m, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ztrevc.c */
int ztrevc_(char *side, char *howmny, logical *select, integer *n, doublecomplex *t, integer *ldt, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, integer *mm, integer *m, doublecomplex *work, doublereal *rwork, integer *info, ftnlen side_len, ftnlen howmny_len);
/* ztrexc.c */
int ztrexc_(char *compq, integer *n, doublecomplex *t, integer *ldt, doublecomplex *q, integer *ldq, integer *ifst, integer *ilst, integer *info, ftnlen compq_len);
/* ztrrfs.c */
int ztrrfs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx, doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztrsen.c */
int ztrsen_(char *job, char *compq, logical *select, integer *n, doublecomplex *t, integer *ldt, doublecomplex *q, integer *ldq, doublecomplex *w, integer *m, doublereal *s, doublereal *sep, doublecomplex *work, integer *lwork, integer *info, ftnlen job_len, ftnlen compq_len);
/* ztrsna.c */
int ztrsna_(char *job, char *howmny, logical *select, integer *n, doublecomplex *t, integer *ldt, doublecomplex *vl, integer *ldvl, doublecomplex *vr, integer *ldvr, doublereal *s, doublereal *sep, integer *mm, integer *m, doublecomplex *work, integer *ldwork, doublereal *rwork, integer *info, ftnlen job_len, ftnlen howmny_len);
/* ztrsyl.c */
int ztrsyl_(char *trana, char *tranb, integer *isgn, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, doublecomplex *c__, integer *ldc, doublereal *scale, integer *info, ftnlen trana_len, ftnlen tranb_len);
/* ztrti2.c */
int ztrti2_(char *uplo, char *diag, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ztrtri.c */
int ztrtri_(char *uplo, char *diag, integer *n, doublecomplex *a, integer *lda, integer *info, ftnlen uplo_len, ftnlen diag_len);
/* ztrtrs.c */
int ztrtrs_(char *uplo, char *trans, char *diag, integer *n, integer *nrhs, doublecomplex *a, integer *lda, doublecomplex *b, integer *ldb, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen diag_len);
/* ztrttf.c */
int ztrttf_(char *transr, char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *arf, integer *info, ftnlen transr_len, ftnlen uplo_len);
/* ztrttp.c */
int ztrttp_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *ap, integer *info, ftnlen uplo_len);
/* ztzrzf.c */
int ztzrzf_(integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb1.c */
int zunbdb1_(integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublecomplex *taup1, doublecomplex *taup2, doublecomplex *tauq1, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb2.c */
int zunbdb2_(integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublecomplex *taup1, doublecomplex *taup2, doublecomplex *tauq1, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb3.c */
int zunbdb3_(integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublecomplex *taup1, doublecomplex *taup2, doublecomplex *tauq1, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb4.c */
int zunbdb4_(integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x21, integer *ldx21, doublereal *theta, doublereal *phi, doublecomplex *taup1, doublecomplex *taup2, doublecomplex *tauq1, doublecomplex *phantom, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb5.c */
int zunbdb5_(integer *m1, integer *m2, integer *n, doublecomplex *x1, integer *incx1, doublecomplex *x2, integer *incx2, doublecomplex *q1, integer *ldq1, doublecomplex *q2, integer *ldq2, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb6.c */
int zunbdb6_(integer *m1, integer *m2, integer *n, doublecomplex *x1, integer *incx1, doublecomplex *x2, integer *incx2, doublecomplex *q1, integer *ldq1, doublecomplex *q2, integer *ldq2, doublecomplex *work, integer *lwork, integer *info);
/* zunbdb.c */
int zunbdb_(char *trans, char *signs, integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x12, integer *ldx12, doublecomplex *x21, integer *ldx21, doublecomplex *x22, integer *ldx22, doublereal *theta, doublereal *phi, doublecomplex *taup1, doublecomplex *taup2, doublecomplex *tauq1, doublecomplex *tauq2, doublecomplex *work, integer *lwork, integer *info, ftnlen trans_len, ftnlen signs_len);
/* zuncsd2by1.c */
int zuncsd2by1_(char *jobu1, char *jobu2, char *jobv1t, integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x21, integer *ldx21, doublereal *theta, doublecomplex *u1, integer *ldu1, doublecomplex *u2, integer *ldu2, doublecomplex *v1t, integer *ldv1t, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len);
/* zuncsd.c */
int zuncsd_(char *jobu1, char *jobu2, char *jobv1t, char *jobv2t, char *trans, char *signs, integer *m, integer *p, integer *q, doublecomplex *x11, integer *ldx11, doublecomplex *x12, integer *ldx12, doublecomplex *x21, integer *ldx21, doublecomplex *x22, integer *ldx22, doublereal *theta, doublecomplex *u1, integer *ldu1, doublecomplex *u2, integer *ldu2, doublecomplex *v1t, integer *ldv1t, doublecomplex *v2t, integer *ldv2t, doublecomplex *work, integer *lwork, doublereal *rwork, integer *lrwork, integer *iwork, integer *info, ftnlen jobu1_len, ftnlen jobu2_len, ftnlen jobv1t_len, ftnlen jobv2t_len, ftnlen trans_len, ftnlen signs_len);
/* zung2l.c */
int zung2l_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zung2r.c */
int zung2r_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zungbr.c */
int zungbr_(char *vect, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info, ftnlen vect_len);
/* zunghr.c */
int zunghr_(integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zungl2.c */
int zungl2_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zunglq.c */
int zunglq_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zungql.c */
int zungql_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zungqr.c */
int zungqr_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zungr2.c */
int zungr2_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *info);
/* zungrq.c */
int zungrq_(integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info);
/* zungtr.c */
int zungtr_(char *uplo, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *work, integer *lwork, integer *info, ftnlen uplo_len);
/* zunm22.c */
int zunm22_(char *side, char *trans, integer *m, integer *n, integer *n1, integer *n2, doublecomplex *q, integer *ldq, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunm2l.c */
int zunm2l_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunm2r.c */
int zunm2r_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmbr.c */
int zunmbr_(char *vect, char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen vect_len, ftnlen side_len, ftnlen trans_len);
/* zunmhr.c */
int zunmhr_(char *side, char *trans, integer *m, integer *n, integer *ilo, integer *ihi, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunml2.c */
int zunml2_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmlq.c */
int zunmlq_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmql.c */
int zunmql_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmqr.c */
int zunmqr_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmr2.c */
int zunmr2_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmr3.c */
int zunmr3_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmrq.c */
int zunmrq_(char *side, char *trans, integer *m, integer *n, integer *k, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmrz.c */
int zunmrz_(char *side, char *trans, integer *m, integer *n, integer *k, integer *l, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen trans_len);
/* zunmtr.c */
int zunmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, doublecomplex *a, integer *lda, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *lwork, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
/* zupgtr.c */
int zupgtr_(char *uplo, integer *n, doublecomplex *ap, doublecomplex *tau, doublecomplex *q, integer *ldq, doublecomplex *work, integer *info, ftnlen uplo_len);
/* zupmtr.c */
int zupmtr_(char *side, char *uplo, char *trans, integer *m, integer *n, doublecomplex *ap, doublecomplex *tau, doublecomplex *c__, integer *ldc, doublecomplex *work, integer *info, ftnlen side_len, ftnlen uplo_len, ftnlen trans_len);
#ifdef __cplusplus
}
#endif
