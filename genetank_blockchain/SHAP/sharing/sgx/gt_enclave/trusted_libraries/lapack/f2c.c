/*
  Functions here are copied from the source code for libf2c.

  Typically each function there is in its own file.

  We don't link against libf2c directly, because we can't guarantee
  it is available, and shipping a static library isn't portable.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "f2c.h"

#include "../pyvm/Include/enclave_wrapper/io.h"
#include "../pyvm/Include/enclave_wrapper/sys.h"


extern void s_wsfe(cilist *f) {;}
extern void s_wsfi(icilist *) {;}
extern void e_wsfe(void) {;}
extern void e_wsfi(void) {;}
extern void do_fio(integer *c, char *s, ftnlen l) {;}
extern void s_wsle(cilist *f) {;}
extern void e_wsle(void) {;}
extern void do_lio(ftnint *a, ftnint *b, char *s, ftnlen l) {;}


/* You'll want this if you redo the f2c_*.c files with the -C option
 * to f2c for checking array subscripts. (It's not suggested you do that
 * for production use, of course.) */
extern int
s_rnge(char *var, int index, char *routine, int lineno)
{
    fprintf(stderr, "array index out-of-bounds for %s[%d] in routine %s:%d\n",
            var, index, routine, lineno);
    fflush(stderr);
    abort();
}

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
integer i_len(s, n) char *s; ftnlen n;
#else
integer i_len(char *s, ftnlen n)
#endif
{
return(n);
}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
extern double sqrt();
double d_sqrt(x) doublereal *x;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
double d_sqrt(doublereal *x)
#endif
{
return( sqrt(*x) );
}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
extern float sqrtf();
double f__cabsf(real, imag) float real, imag;
#else
#undef abs

double f__cabsf(float real, float imag)
#endif
{
float temp;

if(real < 0.0f)
	real = -real;
if(imag < 0.0f)
	imag = -imag;
if(imag > real){
	temp = real;
	real = imag;
	imag = temp;
}
if((imag+real) == real)
	return((float)real);

temp = imag/real;
temp = real*sqrtf(1.0 + temp*temp);  /*overflow!!*/
return(temp);
}


#ifdef KR_headers
extern double sqrt();
double f__cabs(real, imag) double real, imag;
#else
#undef abs

double f__cabs(double real, double imag)
#endif
{
double temp;

if(real < 0)
	real = -real;
if(imag < 0)
	imag = -imag;
if(imag > real){
	temp = real;
	real = imag;
	imag = temp;
}
if((imag+real) == real)
	return((double)real);

temp = imag/real;
temp = real*sqrt(1.0 + temp*temp);  /*overflow!!*/
return(temp);
}

 VOID
#ifdef KR_headers
r_cnjg(r, z) complex *r, *z;
#else
r_cnjg(complex *r, complex *z)
#endif
{
r->r = z->r;
r->i = - z->i;
}

 VOID
#ifdef KR_headers
d_cnjg(r, z) doublecomplex *r, *z;
#else
d_cnjg(doublecomplex *r, doublecomplex *z)
#endif
{
r->r = z->r;
r->i = - z->i;
}


#ifdef KR_headers
double r_imag(z) complex *z;
#else
double r_imag(complex *z)
#endif
{
return(z->i);
}

#ifdef KR_headers
double d_imag(z) doublecomplex *z;
#else
double d_imag(doublecomplex *z)
#endif
{
return(z->i);
}


#define log10e 0.43429448190325182765

#ifdef KR_headers
float logf();
float r_lg10(x) real *x;
#else
#undef abs

float r_lg10(real *x)
#endif
{
return( log10e * logf(*x) );
}

#ifdef KR_headers
double log();
double d_lg10(x) doublereal *x;
#else
#undef abs

double d_lg10(doublereal *x)
#endif
{
return( log10e * log(*x) );
}

#ifdef KR_headers
float r_sign(a,b) real *a, *b;
#else
float r_sign(real *a, real *b)
#endif
{
float x;
x = (*a >= 0.0f ? *a : - *a);
return( *b >= 0.0f ? x : -x);
}

#ifdef KR_headers
double d_sign(a,b) doublereal *a, *b;
#else
double d_sign(doublereal *a, doublereal *b)
#endif
{
double x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}


#ifdef KR_headers
double floor();
integer i_dnnt(x) doublereal *x;
#else
#undef abs

integer i_dnnt(doublereal *x)
#endif
{
return( (*x)>=0 ?
	floor(*x + .5) : -floor(.5 - *x) );
}


#ifdef KR_headers
double floor();
integer i_nint(x) real *x;
#else
#undef abs
integer i_nint(real *x)
#endif
{
return (integer)(*x >= 0 ? floor(*x + .5) : -floor(.5 - *x));
}

#ifdef KR_headers
double pow();
double pow_dd(ap, bp) doublereal *ap, *bp;
#else
#undef abs

double pow_dd(doublereal *ap, doublereal *bp)
#endif
{
return(pow(*ap, *bp) );
}


#ifdef KR_headers
double pow_ri(ap, bp) real *ap; integer *bp;
#else
double pow_ri(real *ap, integer *bp)
#endif
{
double pow, x;
integer n;
unsigned long u;

pow = 1;
x = *ap;
n = *bp;

if(n != 0)
	{
	if(n < 0)
		{
		n = -n;
		x = 1.0f/x;
		}
	for(u = n; ; )
		{
		if(u & 01)
			pow *= x;
		if(u >>= 1)
			x *= x;
		else
			break;
		}
	}
return(pow);
}

#ifdef KR_headers
double pow_di(ap, bp) doublereal *ap; integer *bp;
#else
double pow_di(doublereal *ap, integer *bp)
#endif
{
double pow, x;
integer n;
unsigned long u;

pow = 1;
x = *ap;
n = *bp;

if(n != 0)
	{
	if(n < 0)
		{
		n = -n;
		x = 1/x;
		}
	for(u = n; ; )
		{
		if(u & 01)
			pow *= x;
		if(u >>= 1)
			x *= x;
		else
			break;
		}
	}
return(pow);
}

#ifdef KR_headers
VOID pow_zi(p, a, b) 	/* p = a**b  */
 doublecomplex *p, *a; integer *b;
#else
extern void z_div(doublecomplex*, doublecomplex*, doublecomplex*);
void pow_zi(doublecomplex *p, doublecomplex *a, integer *b) 	/* p = a**b  */
#endif
{
	integer n;
	unsigned long u;
	double t;
	doublecomplex q, x;
	static doublecomplex one = {1.0, 0.0};

	n = *b;
	q.r = 1;
	q.i = 0;

	if(n == 0)
		goto done;
	if(n < 0)
		{
		n = -n;
		z_div(&x, &one, a);
		}
	else
		{
		x.r = a->r;
		x.i = a->i;
		}

	for(u = n; ; )
		{
		if(u & 01)
			{
			t = q.r * x.r - q.i * x.i;
			q.i = q.r * x.i + q.i * x.r;
			q.r = t;
			}
		if(u >>= 1)
			{
			t = x.r * x.r - x.i * x.i;
			x.i = 2 * x.r * x.i;
			x.r = t;
			}
		else
			break;
		}
 done:
	p->i = q.i;
	p->r = q.r;
	}

#ifdef KR_headers
VOID pow_ci(p, a, b) 	/* p = a**b  */
 complex *p, *a; integer *b;
#else
extern void pow_zi(doublecomplex*, doublecomplex*, integer*);
void pow_ci(complex *p, complex *a, integer *b) 	/* p = a**b  */
#endif
{
doublecomplex p1, a1;

a1.r = a->r;
a1.i = a->i;

pow_zi(&p1, &a1, b);

p->r = p1.r;
p->i = p1.i;
}

/* Unless compiled with -DNO_OVERWRITE, this variant of s_cat allows the
 * target of a concatenation to appear on its right-hand side (contrary
 * to the Fortran 77 Standard, but in accordance with Fortran 90).
 */
#define NO_OVERWRITE


#ifndef NO_OVERWRITE

#undef abs
#ifdef KR_headers
 extern char *F77_aloc();
 extern void free();
 extern void exit_();
#else

 extern char *F77_aloc(ftnlen, char*);
#endif

#endif /* NO_OVERWRITE */

 int
#ifdef KR_headers
s_cat(lp, rpp, rnp, np, ll) char *lp, *rpp[]; ftnlen rnp[], *np, ll;
#else
s_cat(char *lp, char **rpp, ftnlen *rnp, ftnlen *np, ftnlen ll)
#endif
{
	ftnlen i, nc;
	char *rp;
	ftnlen n = *np;
#ifndef NO_OVERWRITE
	ftnlen L, m;
	char *lp0, *lp1;

	lp0 = 0;
	lp1 = lp;
	L = ll;
	i = 0;
	while(i < n) {
		rp = rpp[i];
		m = rnp[i++];
		if (rp >= lp1 || rp + m <= lp) {
			if ((L -= m) <= 0) {
				n = i;
				break;
				}
			lp1 += m;
			continue;
			}
		lp0 = lp;
		lp = lp1 = F77_aloc(L = ll, "s_cat");
		break;
		}
	lp1 = lp;
#endif /* NO_OVERWRITE */
	for(i = 0 ; i < n ; ++i) {
		nc = ll;
		if(rnp[i] < nc)
			nc = rnp[i];
		ll -= nc;
		rp = rpp[i];
		while(--nc >= 0)
			*lp++ = *rp++;
		}
	while(--ll >= 0)
		*lp++ = ' ';
#ifndef NO_OVERWRITE
	if (lp0) {
		memmove(lp0, lp1, L);
		free(lp1);
		}
#endif
	return 0;
	}


/* compare two strings */

#ifdef KR_headers
integer s_cmp(a0, b0, la, lb) char *a0, *b0; ftnlen la, lb;
#else
integer s_cmp(char *a0, char *b0, ftnlen la, ftnlen lb)
#endif
{
register unsigned char *a, *aend, *b, *bend;
a = (unsigned char *)a0;
b = (unsigned char *)b0;
aend = a + la;
bend = b + lb;

if(la <= lb)
	{
	while(a < aend)
		if(*a != *b)
			return( *a - *b );
		else
			{ ++a; ++b; }

	while(b < bend)
		if(*b != ' ')
			return( ' ' - *b );
		else	++b;
	}

else
	{
	while(b < bend)
		if(*a == *b)
			{ ++a; ++b; }
		else
			return( *a - *b );
	while(a < aend)
		if(*a != ' ')
			return(*a - ' ');
		else	++a;
	}
return(0);
}
/* Unless compiled with -DNO_OVERWRITE, this variant of s_copy allows the
 * target of an assignment to appear on its right-hand side (contrary
 * to the Fortran 77 Standard, but in accordance with Fortran 90),
 * as in  a(2:5) = a(4:7) .
 */



/* assign strings:  a = b */

#ifdef KR_headers
VOID s_copy(a, b, la, lb) register char *a, *b; ftnlen la, lb;
#else
int s_copy(register char *a, register char *b, ftnlen la, ftnlen lb)
#endif
{
	register char *aend, *bend;

	aend = a + la;

	if(la <= lb)
#ifndef NO_OVERWRITE
		if (a <= b || a >= b + la)
#endif
			while(a < aend)
				*a++ = *b++;
#ifndef NO_OVERWRITE
		else
			for(b += la; a < aend; )
				*--aend = *--b;
#endif

	else {
		bend = b + lb;
#ifndef NO_OVERWRITE
		if (a <= b || a >= bend)
#endif
			while(b < bend)
				*a++ = *b++;
#ifndef NO_OVERWRITE
		else {
			a += lb;
			while(b < bend)
				*--a = *--bend;
			a += lb;
			}
#endif
		while(a < aend)
			*a++ = ' ';
		}
	return 0;
	}


#ifdef KR_headers
double f__cabsf();
double c_abs(z) complex *z;
#else
double f__cabsf(float, float);
double c_abs(complex *z)
#endif
{
return( f__cabsf( z->r, z->i ) );
}

#ifdef KR_headers
double f__cabs();
double z_abs(z) doublecomplex *z;
#else
double f__cabs(double, double);
double z_abs(doublecomplex *z)
#endif
{
return( f__cabs( z->r, z->i ) );
}


#ifdef KR_headers
extern void sig_die();
VOID c_div(c, a, b) complex *a, *b, *c;
#else
extern void sig_die(char*, int);
void c_div(complex *c, complex *a, complex *b)
#endif
{
float ratio, den;
float abr, abi;

if( (abr = b->r) < 0.f)
	abr = - abr;
if( (abi = b->i) < 0.f)
	abi = - abi;
if( abr <= abi )
	{
	  /*Let IEEE Infinties handle this ;( */
	  /*if(abi == 0)
		sig_die("complex division by zero", 1);*/
	ratio = b->r / b->i ;
	den = b->i * (1 + ratio*ratio);
	c->r = (a->r*ratio + a->i) / den;
	c->i = (a->i*ratio - a->r) / den;
	}

else
	{
	ratio = b->i / b->r ;
	den = b->r * (1.f + ratio*ratio);
	c->r = (a->r + a->i*ratio) / den;
	c->i = (a->i - a->r*ratio) / den;
	}

}

#ifdef KR_headers
extern void sig_die();
VOID z_div(c, a, b) doublecomplex *a, *b, *c;
#else
extern void sig_die(char*, int);
void z_div(doublecomplex *c, doublecomplex *a, doublecomplex *b)
#endif
{
double ratio, den;
double abr, abi;

if( (abr = b->r) < 0.)
	abr = - abr;
if( (abi = b->i) < 0.)
	abi = - abi;
if( abr <= abi )
	{
	  /*Let IEEE Infinties handle this ;( */
	  /*if(abi == 0)
		sig_die("complex division by zero", 1);*/
	ratio = b->r / b->i ;
	den = b->i * (1 + ratio*ratio);
	c->r = (a->r*ratio + a->i) / den;
	c->i = (a->i*ratio - a->r) / den;
	}

else
	{
	ratio = b->i / b->r ;
	den = b->r * (1 + ratio*ratio);
	c->r = (a->r + a->i*ratio) / den;
	c->i = (a->i - a->r*ratio) / den;
	}

}


#ifdef KR_headers
float sqrtf(), f__cabsf();
VOID c_sqrt(r, z) complex *r, *z;
#else
#undef abs

extern double f__cabsf(float, float);
void c_sqrt(complex *r, complex *z)
#endif
{
float mag;

if( (mag = f__cabsf(z->r, z->i)) == 0.f)
	r->r = r->i = 0.f;
else if(z->r > 0.0f)
	{
	r->r = sqrtf(0.5f * (mag + z->r) );
	r->i = z->i / r->r / 2.0f;
	}
else
	{
	r->i = sqrtf(0.5f * (mag - z->r) );
	if(z->i < 0.0f)
		r->i = - r->i;
	r->r = z->i / r->i / 2.0f;
	}
}


#ifdef KR_headers
double sqrt(), f__cabs();
VOID z_sqrt(r, z) doublecomplex *r, *z;
#else
#undef abs

extern double f__cabs(double, double);
void z_sqrt(doublecomplex *r, doublecomplex *z)
#endif
{
double mag;

if( (mag = f__cabs(z->r, z->i)) == 0.)
	r->r = r->i = 0.;
else if(z->r > 0)
	{
	r->r = sqrt(0.5 * (mag + z->r) );
	r->i = z->i / r->r / 2;
	}
else
	{
	r->i = sqrt(0.5 * (mag - z->r) );
	if(z->i < 0)
		r->i = - r->i;
	r->r = z->i / r->i / 2;
	}
}
#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
integer pow_ii(ap, bp) integer *ap, *bp;
#else
integer pow_ii(integer *ap, integer *bp)
#endif
{
	integer pow, x, n;
	unsigned long u;

	x = *ap;
	n = *bp;

	if (n <= 0) {
		if (n == 0 || x == 1)
			return 1;
		if (x != -1)
			return x == 0 ? 1/x : 0;
		n = -n;
		}
	u = n;
	for(pow = 1; ; )
		{
		if(u & 01)
			pow *= x;
		if(u >>= 1)
			x *= x;
		else
			break;
		}
	return(pow);
	}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
extern void f_exit();
VOID s_stop(s, n) char *s; ftnlen n;
#else
#undef abs
#undef min
#undef max
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
extern "C" {
#endif
void f_exit(void);

int s_stop(char *s, ftnlen n)
#endif
{
int i;

if(n > 0)
	{
	fprintf(stderr, "STOP ");
	for(i = 0; i<n ; ++i)
		putc(*s++, stderr);
	fprintf(stderr, " statement executed\n");
	}
#ifdef NO_ONEXIT
f_exit();
#endif
exit(0);

/* We cannot avoid (useless) compiler diagnostics here:		*/
/* some compilers complain if there is no return statement,	*/
/* and others complain that this one cannot be reached.		*/

return 0; /* NOT REACHED */
}
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
}
#endif


#ifdef KR_headers
#ifdef IEEE_drem
double drem();
#else
double floor();
#endif
double d_mod(x,y) doublereal *x, *y;
#else
#ifdef IEEE_drem
double drem(double, double);
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
#endif
double d_mod(doublereal *x, doublereal *y)
#endif
{
#ifdef IEEE_drem
	double xa, ya, z;
	if ((ya = *y) < 0.)
		ya = -ya;
	z = drem(xa = *x, ya);
	if (xa > 0) {
		if (z < 0)
			z += ya;
		}
	else if (z > 0)
		z -= ya;
	return z;
#else
	double quotient;
	if( (quotient = *x / *y) >= 0)
		quotient = floor(quotient);
	else
		quotient = -floor(-quotient);
	return(*x - (*y) * quotient );
#endif
}
#ifdef __cplusplus
}
#endif


#ifdef KR_headers
double floor();
double d_int(x) doublereal *x;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
double d_int(doublereal *x)
#endif
{
return( (*x>0) ? floor(*x) : -floor(- *x) );
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef KR_headers
integer i_sign(a,b) integer *a, *b;
#else
integer i_sign(integer *a, integer *b)
#endif
{
integer x;
x = (*a >= 0 ? *a : - *a);
return( *b >= 0 ? x : -x);
}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
double sin(), cos(), sinh(), cosh();
VOID z_sin(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
void z_sin(doublecomplex *r, doublecomplex *z)
#endif
{
	double zi = z->i, zr = z->r;
	r->r = sin(zr) * cosh(zi);
	r->i = cos(zr) * sinh(zi);
	}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
double sin(), cos(), sinh(), cosh();
VOID z_cos(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
void z_cos(doublecomplex *r, doublecomplex *z)
#endif
{
	double zi = z->i, zr = z->r;
	r->r =   cos(zr) * cosh(zi);
	r->i = - sin(zr) * sinh(zi);
	}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
double log(), f__cabs(), atan2();
#define ANSI(x) ()
#else
#define ANSI(x) x
#undef abs
#include "math.h"
extern double f__cabs(double, double);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NO_DOUBLE_EXTENDED
#ifndef GCC_COMPARE_BUG_FIXED
#ifndef Pre20000310
#ifdef Comment
Some versions of gcc, such as 2.95.3 and 3.0.4, are buggy under -O2 or -O3:
on IA32 (Intel 80x87) systems, they may do comparisons on values computed
in extended-precision registers.  This can lead to the test "s > s0" that
was used below being carried out incorrectly.  The fix below cannot be
spoiled by overzealous optimization, since the compiler cannot know
whether gcc_bug_bypass_diff_F2C will be nonzero.  (We expect it always
to be zero.  The weird name is unlikely to collide with anything.)

An example (provided by Ulrich Jakobus) where the bug fix matters is

	double complex a, b
	a = (.1099557428756427618354862829619, .9857360542953131909982289471372)
	b = log(a)

An alternative to the fix below would be to use 53-bit rounding precision,
but the means of specifying this 80x87 feature are highly unportable.
#endif /*Comment*/
#define BYPASS_GCC_COMPARE_BUG
double (*gcc_bug_bypass_diff_F2C) ANSI((double*,double*));
 static double
#ifdef KR_headers
diff1(a,b) double *a, *b;
#else
diff1(double *a, double *b)
#endif
{ return *a - *b; }
#endif /*Pre20000310*/
#endif /*GCC_COMPARE_BUG_FIXED*/
#endif /*NO_DOUBLE_EXTENDED*/

#ifdef KR_headers
VOID z_log(r, z) doublecomplex *r, *z;
#else
void z_log(doublecomplex *r, doublecomplex *z)
#endif
{
	double s, s0, t, t2, u, v;
	double zi = z->i, zr = z->r;
#ifdef BYPASS_GCC_COMPARE_BUG
	double (*diff) ANSI((double*,double*));
#endif

	r->i = atan2(zi, zr);
#ifdef Pre20000310
	r->r = log( f__cabs( zr, zi ) );
#else
	if (zi < 0)
		zi = -zi;
	if (zr < 0)
		zr = -zr;
	if (zr < zi) {
		t = zi;
		zi = zr;
		zr = t;
		}
	t = zi/zr;
	s = zr * sqrt(1 + t*t);
	/* now s = f__cabs(zi,zr), and zr = |zr| >= |zi| = zi */
	if ((t = s - 1) < 0)
		t = -t;
	if (t > .01)
		r->r = log(s);
	else {

#ifdef Comment

	log(1+x) = x - x^2/2 + x^3/3 - x^4/4 + - ...

		 = x(1 - x/2 + x^2/3 -+...)

	[sqrt(y^2 + z^2) - 1] * [sqrt(y^2 + z^2) + 1] = y^2 + z^2 - 1, so

	sqrt(y^2 + z^2) - 1 = (y^2 + z^2 - 1) / [sqrt(y^2 + z^2) + 1]

#endif /*Comment*/

#ifdef BYPASS_GCC_COMPARE_BUG
		if (!(diff = gcc_bug_bypass_diff_F2C))
			diff = diff1;
#endif
		t = ((zr*zr - 1.) + zi*zi) / (s + 1);
		t2 = t*t;
		s = 1. - 0.5*t;
		u = v = 1;
		do {
			s0 = s;
			u *= t2;
			v += 2;
			s += u/v - t*u/(v+1);
			}
#ifdef BYPASS_GCC_COMPARE_BUG
			while(s - s0 > 1e-18 || (*diff)(&s,&s0) > 0.);
#else
			while(s > s0);
#endif
		r->r = s*t;
		}
#endif
	}
#ifdef __cplusplus
}
#endif


#ifdef KR_headers
double log(), exp(), cos(), sin(), atan2(), f__cabs();
VOID pow_zz(r,a,b) doublecomplex *r, *a, *b;
#else
#undef abs
#include "math.h"

extern double f__cabs(double,double);
#ifdef __cplusplus
extern "C" {
#endif
void pow_zz(doublecomplex *r, doublecomplex *a, doublecomplex *b)
#endif
{
double logr, logi, x, y;

logr = log( f__cabs(a->r, a->i) );
logi = atan2(a->i, a->r);

x = exp( logr * b->r - logi * b->i );
y = logr * b->i + logi * b->r;

r->r = x * cos(y);
r->i = x * sin(y);
}
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef REAL
#define REAL double
#endif

#ifdef KR_headers
double erf();
REAL erf_(x) real *x;
#else
extern double erf(double);
REAL erf_(real *x)
#endif
{
return( erf((double)*x) );
}
#ifdef __cplusplus
}
#endif


#ifdef KR_headers
double exp(), cos(), sin();
VOID z_exp(r, z) doublecomplex *r, *z;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif
void z_exp(doublecomplex *r, doublecomplex *z)
#endif
{
	double expx, zi = z->i;

	expx = exp(z->r);
	r->r = expx * cos(zi);
	r->i = expx * sin(zi);
	}
#ifdef __cplusplus
}
#endif

#ifdef KR_headers
extern double exp(), cos(), sin();

 VOID c_exp(r, z) complex *r, *z;
#else
#undef abs
#include "math.h"
#ifdef __cplusplus
extern "C" {
#endif

void c_exp(complex *r, complex *z)
#endif
{
	double expx, zi = z->i;

	expx = exp(z->r);
	r->r = expx * cos(zi);
	r->i = expx * sin(zi);
	}
#ifdef __cplusplus
}
#endif

