#ifndef _F2CF_H
#define _F2CF_H

typedef long longint;
typedef int integer;

#ifdef __cplusplus
extern "C" {
#endif

/* abort_.c */
int abort_(void);
/* arithchk.c */
void get_nanbits(unsigned int *b, int k);
/* backspac.c */
integer f_back(alist *a);
/* c_abs.c */
double c_abs(complex *z);
/* cabs.c */
double f__cabs(double real, double imag);
/* c_cos.c */
void c_cos(complex *r, complex *z);
/* c_div.c */
void c_div(complex *c, complex *a, complex *b);
/* c_exp.c */
void c_exp(complex *r, complex *z);
/* c_log.c */
void c_log(complex *r, complex *z);
/* close.c */
integer f_clos(cllist *a);
void f_exit(void);
int flush_(void);
/* c_sin.c */
void c_sin(complex *r, complex *z);
/* c_sqrt.c */
void c_sqrt(complex *r, complex *z);
/* ctype.c */
/* d_abs.c */
double d_abs(doublereal *x);
/* d_acos.c */
double d_acos(doublereal *x);
/* d_asin.c */
double d_asin(doublereal *x);
/* d_atan.c */
double d_atan(doublereal *x);
/* d_atn2.c */
double d_atn2(doublereal *x, doublereal *y);
/* d_cnjg.c */
void d_cnjg(doublecomplex *r, doublecomplex *z);
/* d_cos.c */
double d_cos(doublereal *x);
/* d_cosh.c */
double d_cosh(doublereal *x);
/* d_dim.c */
double d_dim(doublereal *a, doublereal *b);
/* derf_.c */
double derf_(doublereal *x);
/* derfc_.c */
double derfc_(doublereal *x);
/* d_exp.c */
double d_exp(doublereal *x);
/* dfe.c */
int y_rsk(void);
int y_getc(void);
int c_dfe(cilist *a);
integer s_rdfe(cilist *a);
integer s_wdfe(cilist *a);
integer e_rdfe(void);
integer e_wdfe(void);
/* d_imag.c */
double d_imag(doublecomplex *z);
/* d_int.c */
double d_int(doublereal *x);
/* d_lg10.c */
double d_lg10(doublereal *x);
/* d_log.c */
double d_log(doublereal *x);
/* d_mod.c */
double d_mod(doublereal *x, doublereal *y);
/* d_nint.c */
double d_nint(doublereal *x);
/* dolio.c */
integer do_lio(ftnint *type, ftnint *number, char *ptr, ftnlen len);
/* d_prod.c */
double d_prod(real *x, real *y);
/* d_sign.c */
double d_sign(doublereal *a, doublereal *b);
/* d_sin.c */
double d_sin(doublereal *x);
/* d_sinh.c */
double d_sinh(doublereal *x);
/* d_sqrt.c */
double d_sqrt(doublereal *x);
/* d_tan.c */
double d_tan(doublereal *x);
/* d_tanh.c */
double d_tanh(doublereal *x);
/* dtime_.c */
double dtime_(float *tarray);
/* due.c */
int c_due(cilist *a);
integer s_rdue(cilist *a);
integer s_wdue(cilist *a);
integer e_rdue(void);
integer e_wdue(void);
/* ef1asc_.c */
int ef1asc_(ftnint *a, ftnlen *la, ftnint *b, ftnlen *lb);
/* ef1cmc_.c */
integer ef1cmc_(ftnint *a, ftnlen *la, ftnint *b, ftnlen *lb);
/* endfile.c */
integer f_end(alist *a);
int t_runc(alist *a);
/* erf_.c */
double erf_(real *x);
/* erfc_.c */
double erfc_(real *x);
/* err.c */
//int f__canseek(FILE *f);
void f__fatal(int n, const char *s);
void f_init(void);
//int f__nowreading(unit *x);
//int f__nowwriting(unit *x);
int err__fl(int f, int m, const char *s);
/* etime_.c */
double etime_(float *tarray);
/* exit_.c */
void exit_(integer *rc);
/* f77_aloc.c */
char *F77_aloc(integer Len, const char *whence);
/* f77vers.c */
/* fmt.c */
int pars_f(const char *s);
integer do_fio(ftnint *number, char *ptr, ftnlen len);
int en_fio(void);
void fmt_bg(void);
/* fmtlib.c */
char *f__icvt(long value, int *ndigit, int *sign, int base);
/* ftell64_.c */
//static FILE* unit_chk(integer Unit, char *who);
//longint ftell64_(integer *Unit);
/* ftell_.c */
//integer ftell_(integer *Unit);
//int fseek_(integer *Unit, integer *offset, integer *whence);
/* getarg_.c */
void getarg_(ftnint *n, char *s, ftnlen ls);
/* getenv_.c */
void getenv_(char *fname, char *value, ftnlen flen, ftnlen vlen);
/* h_abs.c */
shortint h_abs(shortint *x);
/* h_dim.c */
shortint h_dim(shortint *a, shortint *b);
/* h_dnnt.c */
shortint h_dnnt(doublereal *x);
/* h_indx.c */
shortint h_indx(char *a, char *b, ftnlen la, ftnlen lb);
/* h_len.c */
shortint h_len(char *s, ftnlen n);
/* hl_ge.c */
shortlogical hl_ge(char *a, char *b, ftnlen la, ftnlen lb);
/* hl_gt.c */
shortlogical hl_gt(char *a, char *b, ftnlen la, ftnlen lb);
/* hl_le.c */
shortlogical hl_le(char *a, char *b, ftnlen la, ftnlen lb);
/* hl_lt.c */
shortlogical hl_lt(char *a, char *b, ftnlen la, ftnlen lb);
/* h_mod.c */
shortint h_mod(short *a, short *b);
/* h_nint.c */
shortint h_nint(real *x);
/* h_sign.c */
shortint h_sign(shortint *a, shortint *b);
/* i77vers.c */
/* i_abs.c */
integer i_abs(integer *x);
/* iargc_.c */
ftnint iargc_(void);
/* i_ceiling.c */
integer i_sceiling(real *x);
integer i_dceiling(doublereal *x);
/* i_dim.c */
integer i_dim(integer *a, integer *b);
/* i_dnnt.c */
integer i_dnnt(doublereal *x);
/* i_indx.c */
integer i_indx(char *a, char *b, ftnlen la, ftnlen lb);
/* iio.c */
int z_getc(void);
void z_putc(int c);
int z_rnew(void);
int c_si(icilist *a);
int iw_rev(void);
integer s_rsfi(icilist *a);
int z_wnew(void);
integer s_wsfi(icilist *a);
integer e_rsfi(void);
integer e_wsfi(void);
/* i_len.c */
integer i_len(char *s, ftnlen n);
/* i_len_trim.c */
integer i_len_trim(char *s, ftnlen n);
/* ilnw.c */
integer s_wsni(icilist *a);
integer s_wsli(icilist *a);
integer e_wsli(void);
/* i_mod.c */
integer i_mod(integer *a, integer *b);
/* i_nint.c */
integer i_nint(real *x);
/* inquire.c */
integer f_inqu(inlist *a);
/* i_sign.c */
integer i_sign(integer *a, integer *b);
/* lbitbits.c */
integer lbit_bits(integer a, integer b, integer len);
integer lbit_cshift(integer a, integer b, integer len);
/* lbitshft.c */
integer lbit_shift(integer a, integer b);
/* l_ge.c */
logical l_ge(char *a, char *b, ftnlen la, ftnlen lb);
/* l_gt.c */
logical l_gt(char *a, char *b, ftnlen la, ftnlen lb);
/* l_le.c */
logical l_le(char *a, char *b, ftnlen la, ftnlen lb);
/* l_lt.c */
logical l_lt(char *a, char *b, ftnlen la, ftnlen lb);
/* lread.c */
int t_getc(void);
integer e_rsle(void);
int c_le(cilist *a);
int l_read(ftnint *number, char *ptr, ftnlen len, ftnint type);
integer s_rsle(cilist *a);
/* lwrite.c */
int l_write(ftnint *number, char *ptr, ftnlen len, ftnint type);
/* open.c */
int f__putbuf(int c);
void x_putc(int c);
integer f_open(olist *a);
int fk_open(int seq, int fmt, ftnint n);
/* pow_ci.c */
void pow_ci(complex *p, complex *a, integer *b);
/* pow_dd.c */
double pow_dd(doublereal *ap, doublereal *bp);
/* pow_di.c */
double pow_di(doublereal *ap, integer *bp);
/* pow_hh.c */
shortint pow_hh(shortint *ap, shortint *bp);
/* pow_ii.c */
integer pow_ii(integer *ap, integer *bp);
/* pow_qq.c */
longint pow_qq(longint *ap, longint *bp);
/* pow_ri.c */
double pow_ri(real *ap, integer *bp);
/* pow_zi.c */
void pow_zi(doublecomplex *p, doublecomplex *a, integer *b);
/* pow_zz.c */
void pow_zz(doublecomplex *r, doublecomplex *a, doublecomplex *b);
/* qbitbits.c */
longint qbit_bits(longint a, integer b, integer len);
longint qbit_cshift(longint a, integer b, integer len);
/* qbitshft.c */
longint qbit_shift(longint a, integer b);
/* r_abs.c */
double r_abs(real *x);
/* r_acos.c */
double r_acos(real *x);
/* r_asin.c */
double r_asin(real *x);
/* r_atan.c */
double r_atan(real *x);
/* r_atn2.c */
double r_atn2(real *x, real *y);
/* r_cnjg.c */
void r_cnjg(complex *r, complex *z);
/* r_cos.c */
double r_cos(real *x);
/* r_cosh.c */
double r_cosh(real *x);
/* rdfmt.c */
//int rd_ed(struct syl *p, char *ptr, ftnlen len);
//int rd_ned(struct syl *p);
/* r_dim.c */
double r_dim(real *a, real *b);
/* rewind.c */
integer f_rew(alist *a);
/* r_exp.c */
double r_exp(real *x);
/* r_imag.c */
double r_imag(complex *z);
/* r_int.c */
double r_int(real *x);
/* r_lg10.c */
double r_lg10(real *x);
/* r_log.c */
double r_log(real *x);
/* r_mod.c */
double r_mod(real *x, real *y);
/* r_nint.c */
double r_nint(real *x);
/* rsfe.c */
int xrd_SL(void);
int x_getc(void);
int x_endp(void);
int x_rev(void);
integer s_rsfe(cilist *a);
/* r_sign.c */
double r_sign(real *a, real *b);
/* r_sin.c */
double r_sin(real *x);
/* r_sinh.c */
double r_sinh(real *x);
/* rsli.c */
integer s_rsli(icilist *a);
integer e_rsli(void);
integer s_rsni(icilist *a);
/* rsne.c */
//hashtab *mk_hashtab(Namelist *nl);
int x_rsne(cilist *a);
integer s_rsne(cilist *a);
/* r_sqrt.c */
double r_sqrt(real *x);
/* r_tan.c */
double r_tan(real *x);
/* r_tanh.c */
double r_tanh(real *x);
/* s_cat.c */
void s_cat(char *lp, char *rpp[], ftnint rnp[], ftnint *np, ftnlen ll);
/* s_cmp.c */
integer s_cmp(char *a0, char *b0, ftnlen la, ftnlen lb);
/* s_copy.c */
void s_copy(register char *a, register char *b, ftnlen la, ftnlen lb);
/* sfe.c */
integer e_rsfe(void);
int c_sfe(cilist *a);
integer e_wsfe(void);
/* sig_die.c */
void sig_die(const char *s, int kill);
/* signal_.c */
//ftnint signal_(integer *sigp, sig_pf proc);
/* signbit.c */
int signbit_f2c(double *x);
/* s_paus.c */
int s_paus(char *s, ftnlen n);
/* s_rnge.c */
integer s_rnge(char *varn, ftnint offset, char *procn, ftnint line);
/* s_stop.c */
int s_stop(char *s, ftnlen n);
/* sue.c */
int c_sue(cilist *a);
integer s_rsue(cilist *a);
integer s_wsue(cilist *a);
integer e_wsue(void);
integer e_rsue(void);
/* system_.c */
integer system_(register char *s, ftnlen n);
/* typesize.c */
/* uio.c */
int do_us(ftnint *number, char *ptr, ftnlen len);
integer do_ud(ftnint *number, char *ptr, ftnlen len);
integer do_uio(ftnint *number, char *ptr, ftnlen len);
/* uninit.c */
void _uninit_f2c(void *x, int type, long len);
/* util.c */
void g_char(const char *a, ftnlen alen, char *b);
void b_char(const char *a, char *b, ftnlen blen);
long f__inode(char *a, int *dev);
/* wref.c */
//int wrt_E(ufloat *p, int w, int d, int e, ftnlen len);
//int wrt_F(ufloat *p, int w, int d, ftnlen len);
/* wrtfmt.c */
//int wrt_L(Uint *n, int len, ftnlen sz);
//int w_ed(struct syl *p, char *ptr, ftnlen len);
//int w_ned(struct syl *p);
/* wsfe.c */
int x_wSL(void);
integer s_wsfe(cilist *a);
/* wsle.c */
integer s_wsle(cilist *a);
integer e_wsle(void);
/* wsne.c */
integer s_wsne(cilist *a);
/* xwsne.c */
void x_wsne(cilist *a);
/* z_abs.c */
double z_abs(doublecomplex *z);
/* z_cos.c */
void z_cos(doublecomplex *r, doublecomplex *z);
/* z_div.c */
void z_div(doublecomplex *c, doublecomplex *a, doublecomplex *b);
/* z_exp.c */
void z_exp(doublecomplex *r, doublecomplex *z);
/* z_log.c */
void z_log(doublecomplex *r, doublecomplex *z);
/* z_sin.c */
void z_sin(doublecomplex *r, doublecomplex *z);
/* z_sqrt.c */
void z_sqrt(doublecomplex *r, doublecomplex *z);

#ifdef __cplusplus
}
#endif
#endif
