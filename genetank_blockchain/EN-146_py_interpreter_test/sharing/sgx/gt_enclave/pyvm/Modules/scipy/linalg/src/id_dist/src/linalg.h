#include "../../../../../../../include/f2c.h"

typedef void (*S_fp1)(integer*, doublereal*, integer*, doublereal*,
		doublereal*, doublereal*, doublereal*, doublereal*);
typedef void (*S_fp2)(integer*, doublecomplex*, integer*, doublecomplex*,
		doublecomplex*, doublecomplex*, doublecomplex*, doublecomplex*);


#ifdef __cplusplus
extern "C" {
#endif
///* dfft.c */
//int zfftb1_(integer *n, doublereal *c__, doublereal *ch, doublereal *wa, integer *ifac);
//int zfftb_(integer *n, doublereal *c__, doublereal *wsave);
//int zfftf1_(integer *n, doublereal *c__, doublereal *ch, doublereal *wa, integer *ifac);
int zfftf_(integer *n, doublereal *c__, doublereal *wsave);
//int zffti1_(integer *n, doublereal *wa, integer *ifac);
int zffti_(integer *n, doublereal *wsave);
//int dcosqb1_(integer *n, doublereal *x, doublereal *w, doublereal *xh);
//int dcosqf1_(integer *n, doublereal *x, doublereal *w, doublereal *xh);
//int dcosqi_(integer *n, doublereal *wsave);
//int dcost_(integer *n, doublereal *x, doublereal *wsave);
//int dzfft1_(integer *n, doublereal *wa, integer *ifac);
//int dcosqb_(integer *n, doublereal *x, doublereal *wsave);
//int dcosqf_(integer *n, doublereal *x, doublereal *wsave);
//int dcosti_(integer *n, doublereal *wsave);
//int dzfftb_(integer *n, doublereal *r__, doublereal *azero, doublereal *a, doublereal *b, doublereal *wsave);
//int dzfftf_(integer *n, doublereal *r__, doublereal *azero, doublereal *a, doublereal *b, doublereal *wsave);
//int dzffti_(integer *n, doublereal *wsave);
//int dpassb_(integer *nac, integer *ido, integer *ip, integer *l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, doublereal *ch2, doublereal *wa);
//int dpassb2_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1);
//int dpassb3_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2);
//int dpassb4_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3);
//int dpassb5_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, doublereal *wa4);
//int dpassf_(integer *nac, integer *ido, integer *ip, integer *l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, doublereal *ch2, doublereal *wa);
//int dpassf2_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1);
//int dpassf3_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2);
//int dpassf4_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3);
//int dpassf5_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, doublereal *wa4);
//int dradb2_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1);
//int dradb3_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2);
//int dradb4_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3);
//int dradb5_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, doublereal *wa4);
//int dradbg_(integer *ido, integer *ip, integer *l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, doublereal *ch2, doublereal *wa);
//int dradf2_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1);
//int dradf3_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2);
//int dradf4_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3);
//int dradf5_(integer *ido, integer *l1, doublereal *cc, doublereal *ch, doublereal *wa1, doublereal *wa2, doublereal *wa3, doublereal *wa4);
//int dradfg_(integer *ido, integer *ip, integer *l1, integer *idl1, doublereal *cc, doublereal *c1, doublereal *c2, doublereal *ch, doublereal *ch2, doublereal *wa);
//int dfftb1_(integer *n, doublereal *c__, doublereal *ch, doublereal *wa, integer *ifac);
//int dfftb_(integer *n, doublereal *r__, doublereal *wsave);
//int dfftf1_(integer *n, doublereal *c__, doublereal *ch, doublereal *wa, integer *ifac);
int dfftf_(integer *n, doublereal *r__, doublereal *wsave);
//int dffti1_(integer *n, doublereal *wa, integer *ifac);
int dffti_(integer *n, doublereal *wsave);
//int dsinqb_(integer *n, doublereal *x, doublereal *wsave);
//int dsinqf_(integer *n, doublereal *x, doublereal *wsave);
//int dsinqi_(integer *n, doublereal *wsave);
//int dsint1_(integer *n, doublereal *war, doublereal *was, doublereal *xh, doublereal *x, integer *ifac);
//int dsint_(integer *n, doublereal *x, doublereal *wsave);
//int dsinti_(integer *n, doublereal *wsave);
/* idd_frm.c */
int idd_frm_(integer *m, integer *n, doublereal *w, doublereal *x, doublereal *y);
int idd_sfrm_(integer *l, integer *m, integer *n, doublereal *w, doublereal *x, doublereal *y);
int idd_pairsamps_(integer *n, integer *l, integer *ind, integer *l2, integer *ind2, integer *marker);
int idd_permute_(integer *n, integer *ind, doublereal *x, doublereal *y);
int idd_subselect_(integer *n, integer *ind, integer *m, doublereal *x, doublereal *y);
int idd_frmi_(integer *m, integer *n, doublereal *w);
int idd_sfrmi_(integer *l, integer *m, integer *n, doublereal *w);
int idd_copyints_(integer *n, integer *ia, integer *ib);
int idd_poweroftwo_(integer *m, integer *l, integer *n);
/* idd_house.c */
int idd_houseapp_(integer *n, doublereal *vn, doublereal *u, integer *ifrescal, doublereal *scal, doublereal *v);
int idd_house_(integer *n, doublereal *x, doublereal *rss, doublereal *vn, doublereal *scal);
int idd_housemat_(integer *n, doublereal *vn, doublereal *scal, doublereal *h__);
/* idd_id2svd.c */
int idd_id2svd_(integer *m, integer *krank, doublereal *b, integer *n, integer *list, doublereal *proj, doublereal *u, doublereal *v, doublereal *s, integer *ier, doublereal *w);
int idd_id2svd0_(integer *m, integer *krank, doublereal *b, integer *n, integer *list, doublereal *proj, doublereal *u, doublereal *v, doublereal *s, integer *ier, doublereal *work, doublereal *p, doublereal *t, doublereal *r__, doublereal *r2, doublereal *r3, integer *ind, integer *indt);
int idd_mattrans_(integer *m, integer *n, doublereal *a, doublereal *at);
int idd_matmultt_(integer *l, integer *m, doublereal *a, integer *n, doublereal *b, doublereal *c__);
int idd_rearr_(integer *krank, integer *ind, integer *m, integer *n, doublereal *a);
int idd_rinqr_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *r__);
/* idd_id.c */
int iddp_id_(doublereal *eps, integer *m, integer *n, doublereal *a, integer *krank, integer *list, doublereal *rnorms);
int iddr_id_(integer *m, integer *n, doublereal *a, integer *krank, integer *list, doublereal *rnorms);
int idd_reconid_(integer *m, integer *krank, doublereal *col, integer *n, integer *list, doublereal *proj, doublereal *approx);
int idd_lssolve_(integer *m, integer *n, doublereal *a, integer *krank);
int idd_moverup_(integer *m, integer *n, integer *krank, doublereal *a);
int idd_getcols_(integer *m, integer *n, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, integer *list, doublereal *col, doublereal *x);
int idd_reconint_(integer *n, integer *list, integer *krank, doublereal *proj, doublereal *p);
int idd_copycols_(integer *m, integer *n, doublereal *a, integer *krank, integer *list, doublereal *col);
/* iddp_aid.c */
int iddp_aid_(doublereal *eps, integer *m, integer *n, doublereal *a, doublereal *work, integer *krank, integer *list, doublereal *proj);
int iddp_aid0_(doublereal *eps, integer *m, integer *n, doublereal *a, integer *krank, integer *list, doublereal *proj, doublereal *rnorms);
int iddp_aid1_(doublereal *eps, integer *n2, integer *n, integer *kranki, doublereal *proj, integer *krank, integer *list, doublereal *rnorms);
int idd_estrank_(doublereal *eps, integer *m, integer *n, doublereal *a, doublereal *w, integer *krank, doublereal *ra);
int idd_estrank0_(doublereal *eps, integer *m, integer *n, doublereal *a, doublereal *w, integer *n2, integer *krank, doublereal *ra, doublereal *rat, doublereal *scal);
int idd_atransposer_(integer *m, integer *n, doublereal *a, doublereal *at);
/* iddp_asvd.c */
int iddp_asvd_(integer *lw, doublereal *eps, integer *m, integer *n, doublereal *a, doublereal *winit, integer *krank, integer *iu, integer *iv, integer *is, doublereal *w, integer *ier);
int iddp_asvd0_(integer *m, integer *n, doublereal *a, integer *krank, integer *list, doublereal *proj, doublereal *u, doublereal *v, doublereal *s, integer *ier, doublereal *col, doublereal *work);
/* iddp_rid.c */
int iddp_rid_(integer *lproj, doublereal *eps, integer *m, integer *n, S_fp1 matvect, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, integer *list, doublereal *proj, integer *ier);
int idd_findrank_(integer *lra, doublereal *eps, integer *m, integer *n, S_fp1 matvect, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, doublereal *ra, integer *ier, doublereal *w);
int idd_findrank0_(integer *lra, doublereal *eps, integer *m, integer *n, S_fp1 matvect, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, doublereal *ra, integer *ier, doublereal *x, doublereal *y, doublereal *scal);
int idd_crunch_(integer *n, integer *l, doublereal *a);
int idd_rtransposer_(integer *m, integer *n, doublereal *a, doublereal *at);
/* iddp_rsvd.c */
int iddp_rsvd_(integer *lw, doublereal *eps, integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, integer *iu, integer *iv, integer *is, doublereal *w, integer *ier);
int iddp_rsvd0_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, doublereal *u, doublereal *v, doublereal *s, integer *ier, integer *list, doublereal *proj, doublereal *col, doublereal *work);
/* idd_qrpiv.c */
int idd_permmult_(integer *m, integer *ind, integer *n, integer *indprod);
int idd_qinqr_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *q);
int idd_qmatvec_(integer *iftranspose, integer *m, integer *n, doublereal *a, integer *krank, doublereal *v);
int idd_qmatmat_(integer *iftranspose, integer *m, integer *n, doublereal *a, integer *krank, integer *l, doublereal *b, doublereal *work);
int iddp_qrpiv_(doublereal *eps, integer *m, integer *n, doublereal *a, integer *krank, integer *ind, doublereal *ss);
int iddr_qrpiv_(integer *m, integer *n, doublereal *a, integer *krank, integer *ind, doublereal *ss);
/* iddr_aid.c */
int iddr_aid_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *w, integer *list, doublereal *proj);
int iddr_aid0_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *w, integer *list, doublereal *proj, doublereal *r__);
int iddr_copydarr_(integer *n, doublereal *a, doublereal *b);
int iddr_aidi_(integer *m, integer *n, integer *krank, doublereal *w);
/* iddr_asvd.c */
int iddr_asvd_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *w, doublereal *u, doublereal *v, doublereal *s, integer *ier);
int iddr_asvd0_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *winit, doublereal *u, doublereal *v, doublereal *s, integer *ier, integer *list, doublereal *proj, doublereal *col, doublereal *work);
/* iddr_rid.c */
int iddr_rid_(integer *m, integer *n, S_fp1 matvect, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, integer *list, doublereal *proj);
int iddr_ridall0_(integer *m, integer *n, S_fp1 matvect, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, integer *list, doublereal *r__, doublereal *x, doublereal *y);
/* iddr_rsvd.c */
int iddr_rsvd_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, doublereal *u, doublereal *v, doublereal *s, integer *ier, doublereal *w);
int iddr_rsvd0_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *krank, doublereal *u, doublereal *v, doublereal *s, integer *ier, integer *list, doublereal *proj, doublereal *col, doublereal *work);
/* idd_sfft.c */
int idd_ldiv_(integer *l, integer *n, integer *m);
int idd_sffti_(integer *l, integer *ind, integer *n, doublecomplex *wsave);
int idd_sffti1_(integer *ind, integer *n, doublereal *wsave);
int idd_sffti2_(integer *l, integer *ind, integer *n, doublecomplex *wsave);
int idd_sfft_(integer *l, integer *ind, integer *n, doublecomplex *wsave, doublereal *v);
int idd_sfft1_(integer *ind, integer *n, doublereal *v, doublereal *wsave);
int idd_sfft2_(integer *l, integer *ind, integer *n, doublereal *v, doublecomplex *wsave);
/* idd_snorm.c */
int idd_snorm_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, integer *its, doublereal *snorm, doublereal *v, doublereal *u);
int idd_enorm_(integer *n, doublereal *v, doublereal *enorm);
int idd_diffsnorm_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvect2, doublereal *p1t2, doublereal *p2t2, doublereal *p3t2, doublereal *p4t2, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, S_fp1 matvec2, doublereal *p12, doublereal *p22, doublereal *p32, doublereal *p42, integer *its, doublereal *snorm, doublereal *w);
int idd_diffsnorm0_(integer *m, integer *n, S_fp1 matvect, doublereal *p1t, doublereal *p2t, doublereal *p3t, doublereal *p4t, S_fp1 matvect2, doublereal *p1t2, doublereal *p2t2, doublereal *p3t2, doublereal *p4t2, S_fp1 matvec, doublereal *p1, doublereal *p2, doublereal *p3, doublereal *p4, S_fp1 matvec2, doublereal *p12, doublereal *p22, doublereal *p32, doublereal *p42, integer *its, doublereal *snorm, doublereal *u, doublereal *u1, doublereal *u2, doublereal *v, doublereal *v1, doublereal *v2);
/* idd_svd.c */
int iddr_svd_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *u, doublereal *v, doublereal *s, integer *ier, doublereal *r__);
int iddp_svd_(integer *lw, doublereal *eps, integer *m, integer *n, doublereal *a, integer *krank, integer *iu, integer *iv, integer *is, doublereal *w, integer *ier);
int idd_permuter_(integer *krank, integer *ind, integer *m, integer *n, doublereal *a);
int idd_retriever_(integer *m, integer *n, doublereal *a, integer *krank, doublereal *r__);
int idd_transer_(integer *m, integer *n, doublereal *a, doublereal *at);
/* id_rand.c */
int id_frand__0_(int n__, integer *n, doublereal *r__, doublereal *t);
int id_frand_(integer *n, doublereal *r__);
int id_frandi_(doublereal *t);
int id_frando_(void);
int id_srand__0_(int n__, integer *n, doublereal *r__, doublereal *t);
int id_srand_(integer *n, doublereal *r__);
int id_srandi_(doublereal *t);
int id_srando_(void);
int id_randperm_(integer *n, integer *ind);
/* id_rtrans.c */
int idd_random_transf_init_(integer *nsteps, integer *n, doublereal *w, integer *keep);
int idz_random_transf_init_(integer *nsteps, integer *n, doublereal *w, integer *keep);
int idd_random_transf_(doublereal *x, doublereal *y, doublereal *w);
int idd_random_transf_inverse_(doublereal *x, doublereal *y, doublereal *w);
int idz_random_transf_(doublecomplex *x, doublecomplex *y, doublereal *w);
int idz_random_transf_inverse_(doublecomplex *x, doublecomplex *y, doublereal *w);
int idd_random_transf0_inv_(integer *nsteps, doublereal *x, doublereal *y, integer *n, doublereal *w2, doublereal *albetas, integer *iixs);
int idd_random_transf00_inv_(doublereal *x, doublereal *y, integer *n, doublereal *albetas, integer *ixs);
int idz_random_transf0_inv_(integer *nsteps, doublecomplex *x, doublecomplex *y, integer *n, doublecomplex *w2, doublereal *albetas, doublecomplex *gammas, integer *iixs);
int idz_random_transf00_inv_(doublecomplex *x, doublecomplex *y, integer *n, doublereal *albetas, doublecomplex *gammas, integer *ixs);
int idd_random_transf0_(integer *nsteps, doublereal *x, doublereal *y, integer *n, doublereal *w2, doublereal *albetas, integer *iixs);
int idd_random_transf00_(doublereal *x, doublereal *y, integer *n, doublereal *albetas, integer *ixs);
int idz_random_transf_init0_(integer *nsteps, integer *n, doublereal *albetas, doublecomplex *gammas, integer *ixs);
int idz_random_transf_init00_(integer *n, doublereal *albetas, doublereal *gammas, integer *ixs);
int idz_random_transf0_(integer *nsteps, doublecomplex *x, doublecomplex *y, integer *n, doublecomplex *w2, doublereal *albetas, doublecomplex *gammas, integer *iixs);
int idz_random_transf00_(doublecomplex *x, doublecomplex *y, integer *n, doublereal *albetas, doublecomplex *gammas, integer *ixs);
int idd_random_transf_init0_(integer *nsteps, integer *n, doublereal *albetas, integer *ixs);
int idd_random_transf_init00_(integer *n, doublereal *albetas, integer *ixs);
/* idz_frm.c */
int idz_frm_(integer *m, integer *n, doublecomplex *w, doublecomplex *x, doublecomplex *y);
int idz_sfrm_(integer *l, integer *m, integer *n, doublecomplex *w, doublecomplex *x, doublecomplex *y);
int idz_permute_(integer *n, integer *ind, doublecomplex *x, doublecomplex *y);
int idz_subselect_(integer *n, integer *ind, integer *m, doublecomplex *x, doublecomplex *y);
int idz_frmi_(integer *m, integer *n, doublecomplex *w);
int idz_sfrmi_(integer *l, integer *m, integer *n, doublecomplex *w);
int idz_poweroftwo_(integer *m, integer *l, integer *n);
/* idz_house.c */
int idz_houseapp_(integer *n, doublecomplex *vn, doublecomplex *u, integer *ifrescal, doublereal *scal, doublecomplex *v);
int idz_house_(integer *n, doublecomplex *x, doublecomplex *css, doublecomplex *vn, doublereal *scal);
int idz_housemat_(integer *n, doublecomplex *vn, doublereal *scal, doublecomplex *h__);
/* idz_id2svd.c */
int idz_id2svd_(integer *m, integer *krank, doublecomplex *b, integer *n, integer *list, doublecomplex *proj, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *w);
int idz_id2svd0_(integer *m, integer *krank, doublecomplex *b, integer *n, integer *list, doublecomplex *proj, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *work, doublecomplex *p, doublecomplex *t, doublecomplex *r__, doublecomplex *r2, doublecomplex *r3, integer *ind, integer *indt);
int idz_matadj_(integer *m, integer *n, doublecomplex *a, doublecomplex *aa);
int idz_matmulta_(integer *l, integer *m, doublecomplex *a, integer *n, doublecomplex *b, doublecomplex *c__);
int idz_rearr_(integer *krank, integer *ind, integer *m, integer *n, doublecomplex *a);
int idz_rinqr_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *r__);
/* idz_id.c */
int idzp_id_(doublereal *eps, integer *m, integer *n, doublecomplex *a, integer *krank, integer *list, doublereal *rnorms);
int idzr_id_(integer *m, integer *n, doublecomplex *a, integer *krank, integer *list, doublereal *rnorms);
int idz_reconid_(integer *m, integer *krank, doublecomplex *col, integer *n, integer *list, doublecomplex *proj, doublecomplex *approx);
int idz_lssolve_(integer *m, integer *n, doublecomplex *a, integer *krank);
int idz_moverup_(integer *m, integer *n, integer *krank, doublecomplex *a);
int idz_getcols_(integer *m, integer *n, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, integer *list, doublecomplex *col, doublecomplex *x);
int idz_reconint_(integer *n, integer *list, integer *krank, doublecomplex *proj, doublecomplex *p);
int idz_copycols_(integer *m, integer *n, doublecomplex *a, integer *krank, integer *list, doublecomplex *col);
/* idzp_aid.c */
int idzp_aid_(doublereal *eps, integer *m, integer *n, doublecomplex *a, doublecomplex *work, integer *krank, integer *list, doublecomplex *proj);
int idzp_aid0_(doublereal *eps, integer *m, integer *n, doublecomplex *a, integer *krank, integer *list, doublecomplex *proj, doublereal *rnorms);
int idzp_aid1_(doublereal *eps, integer *n2, integer *n, integer *kranki, doublecomplex *proj, integer *krank, integer *list, doublereal *rnorms);
int idz_estrank_(doublereal *eps, integer *m, integer *n, doublecomplex *a, doublecomplex *w, integer *krank, doublecomplex *ra);
int idz_estrank0_(doublereal *eps, integer *m, integer *n, doublecomplex *a, doublecomplex *w, integer *n2, integer *krank, doublecomplex *ra, doublecomplex *rat, doublereal *scal);
int idz_transposer_(integer *m, integer *n, doublecomplex *a, doublecomplex *at);
/* idzp_asvd.c */
int idzp_asvd_(integer *lw, doublereal *eps, integer *m, integer *n, doublecomplex *a, doublecomplex *winit, integer *krank, integer *iu, integer *iv, integer *is, doublecomplex *w, integer *ier);
int idzp_asvd0_(integer *m, integer *n, doublecomplex *a, integer *krank, integer *list, doublecomplex *proj, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *col, doublecomplex *work);
int idz_realcomplex_(integer *n, doublereal *a, doublecomplex *b);
/* idzp_rid.c */
int idzp_rid_(integer *lproj, doublereal *eps, integer *m, integer *n, S_fp2 matveca, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, integer *list, doublecomplex *proj, integer *ier);
int idz_findrank_(integer *lra, doublereal *eps, integer *m, integer *n, S_fp2 matveca, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *ra, integer *ier, doublecomplex *w);
int idz_findrank0_(integer *lra, doublereal *eps, integer *m, integer *n, S_fp2 matveca, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *ra, integer *ier, doublecomplex *x, doublecomplex *y, doublecomplex *scal);
int idz_crunch_(integer *n, integer *l, doublecomplex *a);
int idz_adjointer_(integer *m, integer *n, doublecomplex *a, doublecomplex *aa);
/* idzp_rsvd.c */
int idzp_rsvd_(integer *lw, doublereal *eps, integer *m, integer *n, S_fp2 matveca, doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, doublecomplex *p4t, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, integer *iu, integer *iv, integer *is, doublecomplex *w, integer *ier);
int idzp_rsvd0_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, doublecomplex *p4t, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, integer *list, doublecomplex *proj, doublecomplex *col, doublecomplex *work);
int idz_reco_(integer *n, doublereal *a, doublecomplex *b);
/* idz_qrpiv.c */
int idz_permmult_(integer *m, integer *ind, integer *n, integer *indprod);
int idz_qinqr_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *q);
int idz_qmatvec_(integer *ifadjoint, integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *v);
int idz_qmatmat_(integer *ifadjoint, integer *m, integer *n, doublecomplex *a, integer *krank, integer *l, doublecomplex *b, doublereal *work);
int idzp_qrpiv_(doublereal *eps, integer *m, integer *n, doublecomplex *a, integer *krank, integer *ind, doublereal *ss);
int idzr_qrpiv_(integer *m, integer *n, doublecomplex *a, integer *krank, integer *ind, doublereal *ss);
/* idzr_aid.c */
int idzr_aid_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *w, integer *list, doublecomplex *proj);
int idzr_aid0_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *w, integer *list, doublecomplex *proj, doublecomplex *r__);
int idzr_copyzarr_(integer *n, doublecomplex *a, doublecomplex *b);
int idzr_aidi_(integer *m, integer *n, integer *krank, doublecomplex *w);
/* idzr_asvd.c */
int idzr_asvd_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *w, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier);
int idzr_asvd0_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *winit, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, integer *list, doublecomplex *proj, doublecomplex *col, doublecomplex *work);
/* idzr_rid.c */
int idzr_rid_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, integer *list, doublecomplex *proj);
int idzr_ridall0_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, integer *list, doublecomplex *r__, doublecomplex *x, doublecomplex *y);
/* idzr_rsvd.c */
int idzr_rsvd_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, doublecomplex *p4t, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *w);
int idzr_rsvd0_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, doublecomplex *p4t, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, integer *list, doublecomplex *proj, doublecomplex *col, doublecomplex *work);
/* idz_sfft.c */
int idz_ldiv_(integer *l, integer *n, integer *m);
int idz_sffti_(integer *l, integer *ind, integer *n, doublecomplex *wsave);
int idz_sfft_(integer *l, integer *ind, integer *n, doublecomplex *wsave, doublecomplex *v);
/* idz_snorm.c */
int idz_snorm_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, doublecomplex *p4a, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, integer *its, doublereal *snorm, doublecomplex *v, doublecomplex *u);
int idz_enorm_(integer *n, doublecomplex *v, doublereal *enorm);
int idz_diffsnorm_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, doublecomplex *p4a, S_fp2 matveca2, doublecomplex *p1a2, doublecomplex *p2a2, doublecomplex *p3a2, doublecomplex *p4a2, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, S_fp2 matvec2, doublecomplex *p12, doublecomplex *p22, doublecomplex *p32, doublecomplex *p42, integer *its, doublereal *snorm, doublecomplex *w);
int idz_diffsnorm0_(integer *m, integer *n, S_fp2 matveca, doublecomplex *p1a, doublecomplex *p2a, doublecomplex *p3a, doublecomplex *p4a, S_fp2 matveca2, doublecomplex *p1a2, doublecomplex *p2a2, doublecomplex *p3a2, doublecomplex *p4a2, S_fp2 matvec, doublecomplex *p1, doublecomplex *p2, doublecomplex *p3, doublecomplex *p4, S_fp2 matvec2, doublecomplex *p12, doublecomplex *p22, doublecomplex *p32, doublecomplex *p42, integer *its, doublereal *snorm, doublecomplex *u, doublecomplex *u1, doublecomplex *u2, doublecomplex *v, doublecomplex *v1, doublecomplex *v2);
/* idz_svd.c */
int idzr_svd_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *u, doublecomplex *v, doublereal *s, integer *ier, doublecomplex *r__);
int idzp_svd_(integer *lw, doublereal *eps, integer *m, integer *n, doublecomplex *a, integer *krank, integer *iu, integer *iv, integer *is, doublecomplex *w, integer *ier);
int idz_realcomp_(integer *n, doublereal *a, doublecomplex *b);
int idz_permuter_(integer *krank, integer *ind, integer *m, integer *n, doublecomplex *a);
int idz_retriever_(integer *m, integer *n, doublecomplex *a, integer *krank, doublecomplex *r__);
int idz_adjer_(integer *m, integer *n, doublecomplex *a, doublecomplex *aa);
/* prini.c */
int prini_0_(int n__, integer *ip1, integer *iq1, char *mes, real *a, integer *n, doublereal *a2, doublereal *a4, integer *ia, shortint *ia2, char *aa, ftnlen mes_len, ftnlen aa_len);
int prini_(integer *ip1, integer *iq1);
int prin_(char *mes, real *a, integer *n, ftnlen mes_len);
int prin2_(char *mes, doublereal *a2, integer *n, ftnlen mes_len);
int prin2_long_(char *mes, doublereal *a2, integer *n, ftnlen mes_len);
int prinq_(char *mes, doublereal *a4, integer *n, ftnlen mes_len);
int prinf_(char *mes, integer *ia, integer *n, ftnlen mes_len);
int prinf2_(char *mes, shortint *ia2, integer *n, ftnlen mes_len);
int prina_(char *mes, char *aa, integer *n, ftnlen mes_len, ftnlen aa_len);
int messpr_(char *mes, integer *ip, integer *iq, ftnlen mes_len);
int ztime_(integer *i__);
int msgmerge_(char *a, char *b, char *c__, ftnlen a_len, ftnlen b_len, ftnlen c_len);
int fileflush_(integer *iw);
int mach_zero_(doublereal *zero_mach__);
int mach_zero0_(doublereal *a, doublereal *b, doublereal *c__);
/* ../../lu.c */
int dlu_c_(doublereal *p, doublereal *l, doublereal *u, doublereal *a, integer *m, integer *n, integer *k, integer *piv, integer *info, integer *permute_l__, integer *m1);
int zlu_c_(doublereal *p, doublecomplex *l, doublecomplex *u, doublecomplex *a, integer *m, integer *n, integer *k, integer *piv, integer *info, integer *permute_l__, integer *m1);
int slu_c_(real *p, real *l, real *u, real *a, integer *m, integer *n, integer *k, integer *piv, integer *info, integer *permute_l__, integer *m1);
int clu_c_(real *p, complex *l, complex *u, complex *a, integer *m, integer *n, integer *k, integer *piv, integer *info, integer *permute_l__, integer *m1);
/* ../../det.c */
int ddet_c_(doublereal *det, doublereal *a, integer *n, integer *piv, integer *info);
int ddet_r_(doublereal *det, doublereal *a, integer *n, integer *piv, integer *info);
int sdet_c_(real *det, real *a, integer *n, integer *piv, integer *info);
int sdet_r_(real *det, real *a, integer *n, integer *piv, integer *info);
int zdet_c_(doublecomplex *det, doublecomplex *a, integer *n, integer *piv, integer *info);
int zdet_r_(doublecomplex *det, doublecomplex *a, integer *n, integer *piv, integer *info);
int cdet_c_(complex *det, complex *a, integer *n, integer *piv, integer *info);
int cdet_r_(complex *det, complex *a, integer *n, integer *piv, integer *info);
#ifdef __cplusplus
}
#endif
