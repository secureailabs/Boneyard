#include "./complex.h"
#include <math.h>
#include <float.h>

#define M_PI_2 1.57079632679489661923
#define M_PI 3.1415926535897932846


double cabs(double complex z)
{
	return hypot(creal(z), cimag(z));
}


float cabsf(float complex z)
{
	return hypotf(crealf(z), cimagf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double cabsl(long double complex z)
{
	return cabs(z);
}
#else
long double cabsl(long double complex z)
{
	return hypotl(creall(z), cimagl(z));
}
#endif


// FIXME: Hull et al. "Implementing the complex arcsine and arccosine functions using exception handling" 1997

/* acos(z) = pi/2 - asin(z) */

double complex cacos(double complex z)
{
	z = casin(z);
	return CMPLX(M_PI_2 - creal(z), -cimag(z));
}


// FIXME

float complex cacosf(float complex z)
{
	z = casinf(z);
	return CMPLXF((float)M_PI_2 - crealf(z), -cimagf(z));
}


/* acosh(z) = i acos(z) */

double complex cacosh(double complex z)
{
	z = cacos(z);
	return CMPLX(-cimag(z), creal(z));
}


float complex cacoshf(float complex z)
{
	z = cacosf(z);
	return CMPLXF(-cimagf(z), crealf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex cacoshl(long double complex z)
{
	return cacosh(z);
}
#else
long double complex cacoshl(long double complex z)
{
	z = cacosl(z);
	return CMPLXL(-cimagl(z), creall(z));
}
#endif


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex cacosl(long double complex z)
{
	return cacos(z);
}
#else
// FIXME
#define PI_2 1.57079632679489661923132169163975144L
long double complex cacosl(long double complex z)
{
	z = casinl(z);
	return CMPLXL(PI_2 - creall(z), -cimagl(z));
}
#endif


double carg(double complex z)
{
	return atan2(cimag(z), creal(z));
}


float cargf(float complex z)
{
	return atan2f(cimagf(z), crealf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double cargl(long double complex z)
{
	return carg(z);
}
#else
long double cargl(long double complex z)
{
	return atan2l(cimagl(z), creall(z));
}
#endif


// FIXME

/* asin(z) = -i log(i z + sqrt(1 - z*z)) */

double complex casin(double complex z)
{
	double complex w;
	double x, y;

	x = creal(z);
	y = cimag(z);
	w = CMPLX(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clog(CMPLX(-y, x) + csqrt(w));
}


// FIXME

float complex casinf(float complex z)
{
	float complex w;
	float x, y;

	x = crealf(z);
	y = cimagf(z);
	w = CMPLXF(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clogf(CMPLXF(-y, x) + csqrtf(w));
}


/* asinh(z) = -i asin(i z) */

double complex casinh(double complex z)
{
	z = casin(CMPLX(-cimag(z), creal(z)));
	return CMPLX(cimag(z), -creal(z));
}


float complex casinhf(float complex z)
{
	z = casinf(CMPLXF(-cimagf(z), crealf(z)));
	return CMPLXF(cimagf(z), -crealf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex casinhl(long double complex z)
{
	return casinh(z);
}
#else
long double complex casinhl(long double complex z)
{
	z = casinl(CMPLXL(-cimagl(z), creall(z)));
	return CMPLXL(cimagl(z), -creall(z));
}
#endif


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex casinl(long double complex z)
{
	return casin(z);
}
#else
// FIXME
long double complex casinl(long double complex z)
{
	long double complex w;
	long double x, y;

	x = creall(z);
	y = cimagl(z);
	w = CMPLXL(1.0 - (x - y)*(x + y), -2.0*x*y);
	return clogl(CMPLXL(-y, x) + csqrtl(w));
}
#endif
/* origin: OpenBSD /usr/src/lib/libm/src/s_catan.c */
/*
 * Copyright (c) 2008 Stephen L. Moshier <steve@moshier.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 *      Complex circular arc tangent
 *
 *
 * SYNOPSIS:
 *
 * double complex catan();
 * double complex z, w;
 *
 * w = catan (z);
 *
 *
 * DESCRIPTION:
 *
 * If
 *     z = x + iy,
 *
 * then
 *          1       (    2x     )
 * Re w  =  - arctan(-----------)  +  k PI
 *          2       (     2    2)
 *                  (1 - x  - y )
 *
 *               ( 2         2)
 *          1    (x  +  (y+1) )
 * Im w  =  - log(------------)
 *          4    ( 2         2)
 *               (x  +  (y-1) )
 *
 * Where k is an arbitrary integer.
 *
 * catan(z) = -i catanh(iz).
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       -10,+10      5900       1.3e-16     7.8e-18
 *    IEEE      -10,+10     30000       2.3e-15     8.5e-17
 * The check catan( ctan(z) )  =  z, with |x| and |y| < PI/2,
 * had peak relative error 1.5e-16, rms relative error
 * 2.9e-17.  See also clog().
 */



#define MAXNUM 1.0e308

static const double DP1L = 3.14159265160560607910E0;
static const double DP2L = 1.98418714791870343106E-9;
static const double DP3L = 1.14423774522196636802E-17;

static double _redupi(double x)
{
	double t;
	long i;

	t = x/M_PI;
	if (t >= 0.0)
		t += 0.5;
	else
		t -= 0.5;

	i = t;  /* the multiple */
	t = i;
	t = ((x - t * DP1L) - t * DP2L) - t * DP3L;
	return t;
}

double complex catan(double complex z)
{
	double complex w;
	double a, t, x, x2, y;

	x = creal(z);
	y = cimag(z);

	if (x == 0.0 && y > 1.0)
		goto ovrf;

	x2 = x * x;
	a = 1.0 - x2 - (y * y);
	if (a == 0.0)
		goto ovrf;

	t = 0.5 * atan2(2.0 * x, a);
	w = _redupi(t);

	t = y - 1.0;
	a = x2 + (t * t);
	if (a == 0.0)
		goto ovrf;

	t = y + 1.0;
	a = (x2 + t * t)/a;
	w = w + (0.25 * log(a)) * I;
	return w;

ovrf:
	// FIXME
	w = MAXNUM + MAXNUM * I;
	return w;
}
/* origin: OpenBSD /usr/src/lib/libm/src/s_catanf.c */
/*
 * Copyright (c) 2008 Stephen L. Moshier <steve@moshier.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 *      Complex circular arc tangent
 *
 *
 * SYNOPSIS:
 *
 * float complex catanf();
 * float complex z, w;
 *
 * w = catanf( z );
 *
 *
 * DESCRIPTION:
 *
 * If
 *     z = x + iy,
 *
 * then
 *          1       (    2x     )
 * Re w  =  - arctan(-----------)  +  k PI
 *          2       (     2    2)
 *                  (1 - x  - y )
 *
 *               ( 2         2)
 *          1    (x  +  (y+1) )
 * Im w  =  - log(------------)
 *          4    ( 2         2)
 *               (x  +  (y-1) )
 *
 * Where k is an arbitrary integer.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -10,+10     30000        2.3e-6      5.2e-8
 */



#define MAXNUMF 1.0e38F

static const double DP1 = 3.140625;
static const double DP2 = 9.67502593994140625E-4;
static const double DP3 = 1.509957990978376432E-7;

static float _redupif(float xx)
{
	float x, t;
	long i;

	x = xx;
	t = x/(float)M_PI;
	if (t >= 0.0f)
		t += 0.5f;
	else
		t -= 0.5f;

	i = t;  /* the multiple */
	t = i;
	t = ((x - t * DP1) - t * DP2) - t * DP3;
	return t;
}

float complex catanf(float complex z)
{
	float complex w;
	float a, t, x, x2, y;

	x = crealf(z);
	y = cimagf(z);

	if ((x == 0.0f) && (y > 1.0f))
		goto ovrf;

	x2 = x * x;
	a = 1.0f - x2 - (y * y);
	if (a == 0.0f)
		goto ovrf;

	t = 0.5f * atan2f(2.0f * x, a);
	w = _redupif(t);

	t = y - 1.0f;
	a = x2 + (t * t);
	if (a == 0.0f)
		goto ovrf;

	t = y + 1.0f;
	a = (x2 + (t * t))/a;
	w = w + (0.25f * logf (a)) * I;
	return w;

ovrf:
	// FIXME
	w = MAXNUMF + MAXNUMF * I;
	return w;
}


/* atanh = -i atan(i z) */

double complex catanh(double complex z)
{
	z = catan(CMPLX(-cimag(z), creal(z)));
	return CMPLX(cimag(z), -creal(z));
}


float complex catanhf(float complex z)
{
	z = catanf(CMPLXF(-cimagf(z), crealf(z)));
	return CMPLXF(cimagf(z), -crealf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex catanhl(long double complex z)
{
	return catanh(z);
}
#else
long double complex catanhl(long double complex z)
{
	z = catanl(CMPLXL(-cimagl(z), creall(z)));
	return CMPLXL(cimagl(z), -creall(z));
}
#endif
/* origin: OpenBSD /usr/src/lib/libm/src/s_catanl.c */
/*
 * Copyright (c) 2008 Stephen L. Moshier <steve@moshier.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*
 *      Complex circular arc tangent
 *
 *
 * SYNOPSIS:
 *
 * long double complex catanl();
 * long double complex z, w;
 *
 * w = catanl( z );
 *
 *
 * DESCRIPTION:
 *
 * If
 *     z = x + iy,
 *
 * then
 *          1       (    2x     )
 * Re w  =  - arctan(-----------)  +  k PI
 *          2       (     2    2)
 *                  (1 - x  - y )
 *
 *               ( 2         2)
 *          1    (x  +  (y+1) )
 * Im w  =  - log(------------)
 *          4    ( 2         2)
 *               (x  +  (y-1) )
 *
 * Where k is an arbitrary integer.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       -10,+10      5900       1.3e-16     7.8e-18
 *    IEEE      -10,+10     30000       2.3e-15     8.5e-17
 * The check catan( ctan(z) )  =  z, with |x| and |y| < PI/2,
 * had peak relative error 1.5e-16, rms relative error
 * 2.9e-17.  See also clog().
 */

#include <complex.h>
#include <float.h>


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex catanl(long double complex z)
{
	return catan(z);
}
#else
static const long double PIL = 3.141592653589793238462643383279502884197169L;
static const long double DP1LL = 3.14159265358979323829596852490908531763125L;
static const long double DP2LL = 1.6667485837041756656403424829301998703007e-19L;
static const long double DP3LL = 1.8830410776607851167459095484560349402753e-39L;

static long double redupil(long double x)
{
	long double t;
	long i;

	t = x / PIL;
	if (t >= 0.0L)
		t += 0.5L;
	else
		t -= 0.5L;

	i = t;  /* the multiple */
	t = i;
	t = ((x - t * DP1LL) - t * DP2LL) - t * DP3LL;
	return t;
}

long double complex catanl(long double complex z)
{
	long double complex w;
	long double a, t, x, x2, y;

	x = creall(z);
	y = cimagl(z);

	if ((x == 0.0L) && (y > 1.0L))
		goto ovrf;

	x2 = x * x;
	a = 1.0L - x2 - (y * y);
	if (a == 0.0L)
		goto ovrf;

	t = atan2l(2.0L * x, a) * 0.5L;
	w = redupil(t);

	t = y - 1.0L;
	a = x2 + (t * t);
	if (a == 0.0L)
		goto ovrf;

	t = y + 1.0L;
	a = (x2 + (t * t)) / a;
	w = w + (0.25L * logl(a)) * I;
	return w;

ovrf:
	// FIXME
	w = LDBL_MAX + LDBL_MAX * I;
	return w;
}
#endif


/* cos(z) = cosh(i z) */

double complex ccos(double complex z)
{
	return ccosh(CMPLX(-cimag(z), creal(z)));
}


float complex ccosf(float complex z)
{
	return ccoshf(CMPLXF(-cimagf(z), crealf(z)));
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_ccosh.c */
/*-
 * Copyright (c) 2005 Bruce D. Evans and Steven G. Kargl
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic cosine of a complex argument z = x + i y.
 *
 * cosh(z) = cosh(x+iy)
 *         = cosh(x) cos(y) + i sinh(x) sin(y).
 *
 * Exceptional values are noted in the comments within the source code.
 * These values and the return value were taken from n1124.pdf.
 */



static const double hugep = 0x1p1023;

double complex ccosh(double complex z)
{
	double x, y, h;
	int32_t hx, hy, ix, iy, lx, ly;

	x = creal(z);
	y = cimag(z);

	EXTRACT_WORDS(hx, lx, x);
	EXTRACT_WORDS(hy, ly, y);

	ix = 0x7fffffff & hx;
	iy = 0x7fffffff & hy;

	/* Handle the nearly-non-exceptional cases where x and y are finite. */
	if (ix < 0x7ff00000 && iy < 0x7ff00000) {
		if ((iy | ly) == 0)
			return CMPLX(cosh(x), x * y);
		if (ix < 0x40360000)    /* small x: normal case */
			return CMPLX(cosh(x) * cos(y), sinh(x) * sin(y));

		/* |x| >= 22, so cosh(x) ~= exp(|x|) */
		if (ix < 0x40862e42) {
			/* x < 710: exp(|x|) won't overflow */
			h = exp(fabs(x)) * 0.5;
			return CMPLX(h * cos(y), copysign(h, x) * sin(y));
		} else if (ix < 0x4096bbaa) {
			/* x < 1455: scale to avoid overflow */
			z = __ldexp_cexp(CMPLX(fabs(x), y), -1);
			return CMPLX(creal(z), cimag(z) * copysign(1, x));
		} else {
			/* x >= 1455: the result always overflows */
			h = hugep * x;
			return CMPLX(h * h * cos(y), h * sin(y));
		}
	}

	/*
	 * cosh(+-0 +- I Inf) = dNaN + I sign(d(+-0, dNaN))0.
	 * The sign of 0 in the result is unspecified.  Choice = normally
	 * the same as dNaN.  Raise the invalid floating-point exception.
	 *
	 * cosh(+-0 +- I NaN) = d(NaN) + I sign(d(+-0, NaN))0.
	 * The sign of 0 in the result is unspecified.  Choice = normally
	 * the same as d(NaN).
	 */
	if ((ix | lx) == 0 && iy >= 0x7ff00000)
		return CMPLX(y - y, copysign(0, x * (y - y)));

	/*
	 * cosh(+-Inf +- I 0) = +Inf + I (+-)(+-)0.
	 *
	 * cosh(NaN +- I 0)   = d(NaN) + I sign(d(NaN, +-0))0.
	 * The sign of 0 in the result is unspecified.
	 */
	if ((iy | ly) == 0 && ix >= 0x7ff00000) {
		if (((hx & 0xfffff) | lx) == 0)
			return CMPLX(x * x, copysign(0, x) * y);
		return CMPLX(x * x, copysign(0, (x + x) * y));
	}

	/*
	 * cosh(x +- I Inf) = dNaN + I dNaN.
	 * Raise the invalid floating-point exception for finite nonzero x.
	 *
	 * cosh(x + I NaN) = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception for finite
	 * nonzero x.  Choice = don't raise (except for signaling NaNs).
	 */
	if (ix < 0x7ff00000 && iy >= 0x7ff00000)
		return CMPLX(y - y, x * (y - y));

	/*
	 * cosh(+-Inf + I NaN)  = +Inf + I d(NaN).
	 *
	 * cosh(+-Inf +- I Inf) = +Inf + I dNaN.
	 * The sign of Inf in the result is unspecified.  Choice = always +.
	 * Raise the invalid floating-point exception.
	 *
	 * cosh(+-Inf + I y)   = +Inf cos(y) +- I Inf sin(y)
	 */
	if (ix >= 0x7ff00000 && ((hx & 0xfffff) | lx) == 0) {
		if (iy >= 0x7ff00000)
			return CMPLX(x * x, x * (y - y));
		return CMPLX((x * x) * cos(y), x * sin(y));
	}

	/*
	 * cosh(NaN + I NaN)  = d(NaN) + I d(NaN).
	 *
	 * cosh(NaN +- I Inf) = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception.
	 * Choice = raise.
	 *
	 * cosh(NaN + I y)    = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception for finite
	 * nonzero y.  Choice = don't raise (except for signaling NaNs).
	 */
	return CMPLX((x * x) * (y - y), (x + x) * (y - y));
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_ccoshf.c */
/*-
 * Copyright (c) 2005 Bruce D. Evans and Steven G. Kargl
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic cosine of a complex argument.  See s_ccosh.c for details.
 */



static const float huge = 0x1p127;

float complex ccoshf(float complex z)
{
	float x, y, h;
	int32_t hx, hy, ix, iy;

	x = crealf(z);
	y = cimagf(z);

	GET_FLOAT_WORD(hx, x);
	GET_FLOAT_WORD(hy, y);

	ix = 0x7fffffff & hx;
	iy = 0x7fffffff & hy;

	if (ix < 0x7f800000 && iy < 0x7f800000) {
		if (iy == 0)
			return CMPLXF(coshf(x), x * y);
		if (ix < 0x41100000)    /* small x: normal case */
			return CMPLXF(coshf(x) * cosf(y), sinhf(x) * sinf(y));

		/* |x| >= 9, so cosh(x) ~= exp(|x|) */
		if (ix < 0x42b17218) {
			/* x < 88.7: expf(|x|) won't overflow */
			h = expf(fabsf(x)) * 0.5f;
			return CMPLXF(h * cosf(y), copysignf(h, x) * sinf(y));
		} else if (ix < 0x4340b1e7) {
			/* x < 192.7: scale to avoid overflow */
			z = __ldexp_cexpf(CMPLXF(fabsf(x), y), -1);
			return CMPLXF(crealf(z), cimagf(z) * copysignf(1, x));
		} else {
			/* x >= 192.7: the result always overflows */
			h = huge * x;
			return CMPLXF(h * h * cosf(y), h * sinf(y));
		}
	}

	if (ix == 0 && iy >= 0x7f800000)
		return CMPLXF(y - y, copysignf(0, x * (y - y)));

	if (iy == 0 && ix >= 0x7f800000) {
		if ((hx & 0x7fffff) == 0)
			return CMPLXF(x * x, copysignf(0, x) * y);
		return CMPLXF(x * x, copysignf(0, (x + x) * y));
	}

	if (ix < 0x7f800000 && iy >= 0x7f800000)
		return CMPLXF(y - y, x * (y - y));

	if (ix >= 0x7f800000 && (hx & 0x7fffff) == 0) {
		if (iy >= 0x7f800000)
			return CMPLXF(x * x, x * (y - y));
		return CMPLXF((x * x) * cosf(y), x * sinf(y));
	}

	return CMPLXF((x * x) * (y - y), (x + x) * (y - y));
}


//FIXME
long double complex ccoshl(long double complex z)
{
	return ccosh(z);
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex ccosl(long double complex z)
{
	return ccos(z);
}
#else
long double complex ccosl(long double complex z)
{
	return ccoshl(CMPLXL(-cimagl(z), creall(z)));
}
#endif
/* origin: FreeBSD /usr/src/lib/msun/src/k_exp.c */
/*-
 * Copyright (c) 2011 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



static const uint32_t k = 1799; /* constant for reduction */
static const double kln2 = 1246.97177782734161156; /* k * ln2 */

/*
 * Compute exp(x), scaled to avoid spurious overflow.  An exponent is
 * returned separately in 'expt'.
 *
 * Input:  ln(DBL_MAX) <= x < ln(2 * DBL_MAX / DBL_MIN_DENORM) ~= 1454.91
 * Output: 2**1023 <= y < 2**1024
 */
static double __frexp_exp(double x, int *expt)
{
	double exp_x;
	uint32_t hx;

	/*
	 * We use exp(x) = exp(x - kln2) * 2**k, carefully chosen to
	 * minimize |exp(kln2) - 2**k|.  We also scale the exponent of
	 * exp_x to MAX_EXP so that the result can be multiplied by
	 * a tiny number without losing accuracy due to denormalization.
	 */
	exp_x = exp(x - kln2);
	GET_HIGH_WORD(hx, exp_x);
	*expt = (hx >> 20) - (0x3ff + 1023) + k;
	SET_HIGH_WORD(exp_x, (hx & 0xfffff) | ((0x3ff + 1023) << 20));
	return exp_x;
}

/*
 * __ldexp_cexp(x, expt) compute exp(x) * 2**expt.
 * It is intended for large arguments (real part >= ln(DBL_MAX))
 * where care is needed to avoid overflow.
 *
 * The present implementation is narrowly tailored for our hyperbolic and
 * exponential functions.  We assume expt is small (0 or -1), and the caller
 * has filtered out very large x, for which overflow would be inevitable.
 */
double complex __ldexp_cexp(double complex z, int expt)
{
	double x, y, exp_x, scale1, scale2;
	int ex_expt, half_expt;

	x = creal(z);
	y = cimag(z);
	exp_x = __frexp_exp(x, &ex_expt);
	expt += ex_expt;

	/*
	 * Arrange so that scale1 * scale2 == 2**expt.  We use this to
	 * compensate for scalbn being horrendously slow.
	 */
	half_expt = expt / 2;
	INSERT_WORDS(scale1, (0x3ff + half_expt) << 20, 0);
	half_expt = expt - half_expt;
	INSERT_WORDS(scale2, (0x3ff + half_expt) << 20, 0);

	return CMPLX(cos(y) * exp_x * scale1 * scale2, sin(y) * exp_x * scale1 * scale2);
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_cexp.c */
/*-
 * Copyright (c) 2011 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



static const uint32_t
exp_ovfl  = 0x40862e42,  /* high bits of MAX_EXP * ln2 ~= 710 */
cexp_ovfl = 0x4096b8e4;  /* (MAX_EXP - MIN_DENORM_EXP) * ln2 */

double complex cexp(double complex z)
{
	double x, y, exp_x;
	uint32_t hx, hy, lx, ly;

	x = creal(z);
	y = cimag(z);

	EXTRACT_WORDS(hy, ly, y);
	hy &= 0x7fffffff;

	/* cexp(x + I 0) = exp(x) + I 0 */
	if ((hy | ly) == 0)
		return CMPLX(exp(x), y);
	EXTRACT_WORDS(hx, lx, x);
	/* cexp(0 + I y) = cos(y) + I sin(y) */
	if (((hx & 0x7fffffff) | lx) == 0)
		return CMPLX(cos(y), sin(y));

	if (hy >= 0x7ff00000) {
		if (lx != 0 || (hx & 0x7fffffff) != 0x7ff00000) {
			/* cexp(finite|NaN +- I Inf|NaN) = NaN + I NaN */
			return CMPLX(y - y, y - y);
		} else if (hx & 0x80000000) {
			/* cexp(-Inf +- I Inf|NaN) = 0 + I 0 */
			return CMPLX(0.0, 0.0);
		} else {
			/* cexp(+Inf +- I Inf|NaN) = Inf + I NaN */
			return CMPLX(x, y - y);
		}
	}

	if (hx >= exp_ovfl && hx <= cexp_ovfl) {
		/*
		 * x is between 709.7 and 1454.3, so we must scale to avoid
		 * overflow in exp(x).
		 */
		return __ldexp_cexp(z, 0);
	} else {
		/*
		 * Cases covered here:
		 *  -  x < exp_ovfl and exp(x) won't overflow (common case)
		 *  -  x > cexp_ovfl, so exp(x) * s overflows for all s > 0
		 *  -  x = +-Inf (generated by exp())
		 *  -  x = NaN (spurious inexact exception from y)
		 */
		exp_x = exp(x);
		return CMPLX(exp_x * cos(y), exp_x * sin(y));
	}
}
/* origin: FreeBSD /usr/src/lib/msun/src/k_expf.c */
/*-
 * Copyright (c) 2011 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



static const uint32_t kre = 235; /* constant for reduction */
static const float kln2re = 162.88958740F; /* k * ln2 */

/*
 * See __cexp.c for details.
 *
 * Input:  ln(FLT_MAX) <= x < ln(2 * FLT_MAX / FLT_MIN_DENORM) ~= 192.7
 * Output: 2**127 <= y < 2**128
 */
static float __frexp_expf(float x, int *expt)
{
	float exp_x;
	uint32_t hx;

	exp_x = expf(x - kln2re);
	GET_FLOAT_WORD(hx, exp_x);
	*expt = (hx >> 23) - (0x7f + 127) + kre;
	SET_FLOAT_WORD(exp_x, (hx & 0x7fffff) | ((0x7f + 127) << 23));
	return exp_x;
}

float complex __ldexp_cexpf(float complex z, int expt)
{
	float x, y, exp_x, scale1, scale2;
	int ex_expt, half_expt;

	x = crealf(z);
	y = cimagf(z);
	exp_x = __frexp_expf(x, &ex_expt);
	expt += ex_expt;

	half_expt = expt / 2;
	SET_FLOAT_WORD(scale1, (0x7f + half_expt) << 23);
	half_expt = expt - half_expt;
	SET_FLOAT_WORD(scale2, (0x7f + half_expt) << 23);

	return CMPLXF(cosf(y) * exp_x * scale1 * scale2,
	  sinf(y) * exp_x * scale1 * scale2);
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_cexpf.c */
/*-
 * Copyright (c) 2011 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



static const uint32_t
exp_ovflre  = 0x42b17218,  /* MAX_EXP * ln2 ~= 88.722839355 */
cexp_ovflre = 0x43400074;  /* (MAX_EXP - MIN_DENORM_EXP) * ln2 */

float complex cexpf(float complex z)
{
	float x, y, exp_x;
	uint32_t hx, hy;

	x = crealf(z);
	y = cimagf(z);

	GET_FLOAT_WORD(hy, y);
	hy &= 0x7fffffff;

	/* cexp(x + I 0) = exp(x) + I 0 */
	if (hy == 0)
		return CMPLXF(expf(x), y);
	GET_FLOAT_WORD(hx, x);
	/* cexp(0 + I y) = cos(y) + I sin(y) */
	if ((hx & 0x7fffffff) == 0)
		return CMPLXF(cosf(y), sinf(y));

	if (hy >= 0x7f800000) {
		if ((hx & 0x7fffffff) != 0x7f800000) {
			/* cexp(finite|NaN +- I Inf|NaN) = NaN + I NaN */
			return CMPLXF(y - y, y - y);
		} else if (hx & 0x80000000) {
			/* cexp(-Inf +- I Inf|NaN) = 0 + I 0 */
			return CMPLXF(0.0, 0.0);
		} else {
			/* cexp(+Inf +- I Inf|NaN) = Inf + I NaN */
			return CMPLXF(x, y - y);
		}
	}

	if (hx >= exp_ovflre && hx <= cexp_ovflre) {
		/*
		 * x is between 88.7 and 192, so we must scale to avoid
		 * overflow in expf(x).
		 */
		return __ldexp_cexpf(z, 0);
	} else {
		/*
		 * Cases covered here:
		 *  -  x < exp_ovfl and exp(x) won't overflow (common case)
		 *  -  x > cexp_ovfl, so exp(x) * s overflows for all s > 0
		 *  -  x = +-Inf (generated by exp())
		 *  -  x = NaN (spurious inexact exception from y)
		 */
		exp_x = expf(x);
		return CMPLXF(exp_x * cosf(y), exp_x * sinf(y));
	}
}


//FIXME
long double complex cexpl(long double complex z)
{
	return cexp(z);
}

// FIXME

/* log(z) = log(|z|) + i arg(z) */

double complex clog(double complex z)
{
	double r, phi;

	r = cabs(z);
	phi = carg(z);
	return CMPLX(log(r), phi);
}


// FIXME

float complex clogf(float complex z)
{
	float r, phi;

	r = cabsf(z);
	phi = cargf(z);
	return CMPLXF(logf(r), phi);
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex clogl(long double complex z)
{
	return clog(z);
}
#else
// FIXME
long double complex clogl(long double complex z)
{
	long double r, phi;

	r = cabsl(z);
	phi = cargl(z);
	return CMPLXL(logl(r), phi);
}
#endif


double complex conj(double complex z)
{
	return CMPLX(creal(z), -cimag(z));
}


float complex conjf(float complex z)
{
	return CMPLXF(crealf(z), -cimagf(z));
}


long double complex conjl(long double complex z)
{
	return CMPLXL(creall(z), -cimagl(z));
}


/* pow(z, c) = exp(c log(z)), See C99 G.6.4.1 */

double complex cpow(double complex z, double complex c)
{
	return cexp(c * clog(z));
}


float complex cpowf(float complex z, float complex c)
{
	return cexpf(c * clogf(z));
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex cpowl(long double complex z, long double complex c)
{
	return cpow(z, c);
}
#else
long double complex cpowl(long double complex z, long double complex c)
{
	return cexpl(c * clogl(z));
}
#endif


double complex cproj(double complex z)
{
	if (isinf(creal(z)) || isinf(cimag(z)))
		return CMPLX(INFINITY, copysign(0.0, creal(z)));
	return z;
}


float complex cprojf(float complex z)
{
	if (isinf(crealf(z)) || isinf(cimagf(z)))
		return CMPLXF(INFINITY, copysignf(0.0, crealf(z)));
	return z;
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex cprojl(long double complex z)
{
	return cproj(z);
}
#else
long double complex cprojl(long double complex z)
{
	if (isinf(creall(z)) || isinf(cimagl(z)))
		return CMPLXL(INFINITY, copysignl(0.0, creall(z)));
	return z;
}
#endif
#include <complex.h>

/* sin(z) = -i sinh(i z) */

double complex csin(double complex z)
{
	z = csinh(CMPLX(-cimag(z), creal(z)));
	return CMPLX(cimag(z), -creal(z));
}


float complex csinf(float complex z)
{
	z = csinhf(CMPLXF(-cimagf(z), crealf(z)));
	return CMPLXF(cimagf(z), -crealf(z));
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_csinh.c */
/*-
 * Copyright (c) 2005 Bruce D. Evans and Steven G. Kargl
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic sine of a complex argument z = x + i y.
 *
 * sinh(z) = sinh(x+iy)
 *         = sinh(x) cos(y) + i cosh(x) sin(y).
 *
 * Exceptional values are noted in the comments within the source code.
 * These values and the return value were taken from n1124.pdf.
 */



static const double hugere = 0x1p1023;

double complex csinh(double complex z)
{
	double x, y, h;
	int32_t hx, hy, ix, iy, lx, ly;

	x = creal(z);
	y = cimag(z);

	EXTRACT_WORDS(hx, lx, x);
	EXTRACT_WORDS(hy, ly, y);

	ix = 0x7fffffff & hx;
	iy = 0x7fffffff & hy;

	/* Handle the nearly-non-exceptional cases where x and y are finite. */
	if (ix < 0x7ff00000 && iy < 0x7ff00000) {
		if ((iy | ly) == 0)
			return CMPLX(sinh(x), y);
		if (ix < 0x40360000)    /* small x: normal case */
			return CMPLX(sinh(x) * cos(y), cosh(x) * sin(y));

		/* |x| >= 22, so cosh(x) ~= exp(|x|) */
		if (ix < 0x40862e42) {
			/* x < 710: exp(|x|) won't overflow */
			h = exp(fabs(x)) * 0.5;
			return CMPLX(copysign(h, x) * cos(y), h * sin(y));
		} else if (ix < 0x4096bbaa) {
			/* x < 1455: scale to avoid overflow */
			z = __ldexp_cexp(CMPLX(fabs(x), y), -1);
			return CMPLX(creal(z) * copysign(1, x), cimag(z));
		} else {
			/* x >= 1455: the result always overflows */
			h = hugere * x;
			return CMPLX(h * cos(y), h * h * sin(y));
		}
	}

	/*
	 * sinh(+-0 +- I Inf) = sign(d(+-0, dNaN))0 + I dNaN.
	 * The sign of 0 in the result is unspecified.  Choice = normally
	 * the same as dNaN.  Raise the invalid floating-point exception.
	 *
	 * sinh(+-0 +- I NaN) = sign(d(+-0, NaN))0 + I d(NaN).
	 * The sign of 0 in the result is unspecified.  Choice = normally
	 * the same as d(NaN).
	 */
	if ((ix | lx) == 0 && iy >= 0x7ff00000)
		return CMPLX(copysign(0, x * (y - y)), y - y);

	/*
	 * sinh(+-Inf +- I 0) = +-Inf + I +-0.
	 *
	 * sinh(NaN +- I 0)   = d(NaN) + I +-0.
	 */
	if ((iy | ly) == 0 && ix >= 0x7ff00000) {
		if (((hx & 0xfffff) | lx) == 0)
			return CMPLX(x, y);
		return CMPLX(x, copysign(0, y));
	}

	/*
	 * sinh(x +- I Inf) = dNaN + I dNaN.
	 * Raise the invalid floating-point exception for finite nonzero x.
	 *
	 * sinh(x + I NaN) = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception for finite
	 * nonzero x.  Choice = don't raise (except for signaling NaNs).
	 */
	if (ix < 0x7ff00000 && iy >= 0x7ff00000)
		return CMPLX(y - y, x * (y - y));

	/*
	 * sinh(+-Inf + I NaN)  = +-Inf + I d(NaN).
	 * The sign of Inf in the result is unspecified.  Choice = normally
	 * the same as d(NaN).
	 *
	 * sinh(+-Inf +- I Inf) = +Inf + I dNaN.
	 * The sign of Inf in the result is unspecified.  Choice = always +.
	 * Raise the invalid floating-point exception.
	 *
	 * sinh(+-Inf + I y)   = +-Inf cos(y) + I Inf sin(y)
	 */
	if (ix >= 0x7ff00000 && ((hx & 0xfffff) | lx) == 0) {
		if (iy >= 0x7ff00000)
			return CMPLX(x * x, x * (y - y));
		return CMPLX(x * cos(y), INFINITY * sin(y));
	}

	/*
	 * sinh(NaN + I NaN)  = d(NaN) + I d(NaN).
	 *
	 * sinh(NaN +- I Inf) = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception.
	 * Choice = raise.
	 *
	 * sinh(NaN + I y)    = d(NaN) + I d(NaN).
	 * Optionally raises the invalid floating-point exception for finite
	 * nonzero y.  Choice = don't raise (except for signaling NaNs).
	 */
	return CMPLX((x * x) * (y - y), (x + x) * (y - y));
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_csinhf.c */
/*-
 * Copyright (c) 2005 Bruce D. Evans and Steven G. Kargl
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic sine of a complex argument z.  See s_csinh.c for details.
 */



static const float hugere1 = 0x1p127;

float complex csinhf(float complex z)
{
	float x, y, h;
	int32_t hx, hy, ix, iy;

	x = crealf(z);
	y = cimagf(z);

	GET_FLOAT_WORD(hx, x);
	GET_FLOAT_WORD(hy, y);

	ix = 0x7fffffff & hx;
	iy = 0x7fffffff & hy;

	if (ix < 0x7f800000 && iy < 0x7f800000) {
		if (iy == 0)
			return CMPLXF(sinhf(x), y);
		if (ix < 0x41100000)    /* small x: normal case */
			return CMPLXF(sinhf(x) * cosf(y), coshf(x) * sinf(y));

		/* |x| >= 9, so cosh(x) ~= exp(|x|) */
		if (ix < 0x42b17218) {
			/* x < 88.7: expf(|x|) won't overflow */
			h = expf(fabsf(x)) * 0.5f;
			return CMPLXF(copysignf(h, x) * cosf(y), h * sinf(y));
		} else if (ix < 0x4340b1e7) {
			/* x < 192.7: scale to avoid overflow */
			z = __ldexp_cexpf(CMPLXF(fabsf(x), y), -1);
			return CMPLXF(crealf(z) * copysignf(1, x), cimagf(z));
		} else {
			/* x >= 192.7: the result always overflows */
			h = hugere1 * x;
			return CMPLXF(h * cosf(y), h * h * sinf(y));
		}
	}

	if (ix == 0 && iy >= 0x7f800000)
		return CMPLXF(copysignf(0, x * (y - y)), y - y);

	if (iy == 0 && ix >= 0x7f800000) {
		if ((hx & 0x7fffff) == 0)
			return CMPLXF(x, y);
		return CMPLXF(x, copysignf(0, y));
	}

	if (ix < 0x7f800000 && iy >= 0x7f800000)
		return CMPLXF(y - y, x * (y - y));

	if (ix >= 0x7f800000 && (hx & 0x7fffff) == 0) {
		if (iy >= 0x7f800000)
			return CMPLXF(x * x, x * (y - y));
		return CMPLXF(x * cosf(y), INFINITY * sinf(y));
	}

	return CMPLXF((x * x) * (y - y), (x + x) * (y - y));
}


//FIXME
long double complex csinhl(long double complex z)
{
	return csinh(z);
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex csinl(long double complex z)
{
	return csin(z);
}
#else
long double complex csinl(long double complex z)
{
	z = csinhl(CMPLXL(-cimagl(z), creall(z)));
	return CMPLXL(cimagl(z), -creall(z));
}
#endif
/* origin: FreeBSD /usr/src/lib/msun/src/s_csqrt.c */
/*-
 * Copyright (c) 2007 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



/*
 * gcc doesn't implement complex multiplication or division correctly,
 * so we need to handle infinities specially. We turn on this pragma to
 * notify conforming c99 compilers that the fast-but-incorrect code that
 * gcc generates is acceptable, since the special cases have already been
 * handled.
 */
#pragma STDC CX_LIMITED_RANGE ON

/* We risk spurious overflow for components >= DBL_MAX / (1 + sqrt(2)). */
#define THRESH  0x1.a827999fcef32p+1022

double complex csqrt(double complex z)
{
	double complex result;
	double a, b;
	double t;
	int scale;

	a = creal(z);
	b = cimag(z);

	/* Handle special cases. */
	if (z == 0)
		return CMPLX(0, b);
	if (isinf(b))
		return CMPLX(INFINITY, b);
	if (isnan(a)) {
		t = (b - b) / (b - b);  /* raise invalid if b is not a NaN */
		return CMPLX(a, t);   /* return NaN + NaN i */
	}
	if (isinf(a)) {
		/*
		 * csqrt(inf + NaN i)  = inf +  NaN i
		 * csqrt(inf + y i)    = inf +  0 i
		 * csqrt(-inf + NaN i) = NaN +- inf i
		 * csqrt(-inf + y i)   = 0   +  inf i
		 */
		if (signbit(a))
			return CMPLX(fabs(b - b), copysign(a, b));
		else
			return CMPLX(a, copysign(b - b, b));
	}
	/*
	 * The remaining special case (b is NaN) is handled just fine by
	 * the normal code path below.
	 */

	/* Scale to avoid overflow. */
	if (fabs(a) >= THRESH || fabs(b) >= THRESH) {
		a *= 0.25;
		b *= 0.25;
		scale = 1;
	} else {
		scale = 0;
	}

	/* Algorithm 312, CACM vol 10, Oct 1967. */
	if (a >= 0) {
		t = sqrt((a + hypot(a, b)) * 0.5);
		result = CMPLX(t, b / (2 * t));
	} else {
		t = sqrt((-a + hypot(a, b)) * 0.5);
		result = CMPLX(fabs(b) / (2 * t), copysign(t, b));
	}

	/* Rescale. */
	if (scale)
		result *= 2;
	return result;
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_csqrtf.c */
/*-
 * Copyright (c) 2007 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



/*
 * gcc doesn't implement complex multiplication or division correctly,
 * so we need to handle infinities specially. We turn on this pragma to
 * notify conforming c99 compilers that the fast-but-incorrect code that
 * gcc generates is acceptable, since the special cases have already been
 * handled.
 */
#pragma STDC CX_LIMITED_RANGE ON

float complex csqrtf(float complex z)
{
	float a = crealf(z), b = cimagf(z);
	double t;

	/* Handle special cases. */
	if (z == 0)
		return CMPLXF(0, b);
	if (isinf(b))
		return CMPLXF(INFINITY, b);
	if (isnan(a)) {
		t = (b - b) / (b - b);  /* raise invalid if b is not a NaN */
		return CMPLXF(a, t);  /* return NaN + NaN i */
	}
	if (isinf(a)) {
		/*
		 * csqrtf(inf + NaN i)  = inf +  NaN i
		 * csqrtf(inf + y i)    = inf +  0 i
		 * csqrtf(-inf + NaN i) = NaN +- inf i
		 * csqrtf(-inf + y i)   = 0   +  inf i
		 */
		if (signbit(a))
			return CMPLXF(fabsf(b - b), copysignf(a, b));
		else
			return CMPLXF(a, copysignf(b - b, b));
	}
	/*
	 * The remaining special case (b is NaN) is handled just fine by
	 * the normal code path below.
	 */

	/*
	 * We compute t in double precision to avoid overflow and to
	 * provide correct rounding in nearly all cases.
	 * This is Algorithm 312, CACM vol 10, Oct 1967.
	 */
	if (a >= 0) {
		t = sqrt((a + hypot(a, b)) * 0.5);
		return CMPLXF(t, b / (2.0 * t));
	} else {
		t = sqrt((-a + hypot(a, b)) * 0.5);
		return CMPLXF(fabsf(b) / (2.0 * t), copysignf(t, b));
	}
}


//FIXME
long double complex csqrtl(long double complex z)
{
	return csqrt(z);
}


/* tan(z) = -i tanh(i z) */

double complex ctan(double complex z)
{
	z = ctanh(CMPLX(-cimag(z), creal(z)));
	return CMPLX(cimag(z), -creal(z));
}


float complex ctanf(float complex z)
{
	z = ctanhf(CMPLXF(-cimagf(z), crealf(z)));
	return CMPLXF(cimagf(z), -crealf(z));
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_ctanh.c */
/*-
 * Copyright (c) 2011 David Schultz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic tangent of a complex argument z = x + i y.
 *
 * The algorithm is from:
 *
 *   W. Kahan.  Branch Cuts for Complex Elementary Functions or Much
 *   Ado About Nothing's Sign Bit.  In The State of the Art in
 *   Numerical Analysis, pp. 165 ff.  Iserles and Powell, eds., 1987.
 *
 * Method:
 *
 *   Let t    = tan(x)
 *       beta = 1/cos^2(y)
 *       s    = sinh(x)
 *       rho  = cosh(x)
 *
 *   We have:
 *
 *   tanh(z) = sinh(z) / cosh(z)
 *
 *             sinh(x) cos(y) + i cosh(x) sin(y)
 *           = ---------------------------------
 *             cosh(x) cos(y) + i sinh(x) sin(y)
 *
 *             cosh(x) sinh(x) / cos^2(y) + i tan(y)
 *           = -------------------------------------
 *                    1 + sinh^2(x) / cos^2(y)
 *
 *             beta rho s + i t
 *           = ----------------
 *               1 + beta s^2
 *
 * Modifications:
 *
 *   I omitted the original algorithm's handling of overflow in tan(x) after
 *   verifying with nearpi.c that this can't happen in IEEE single or double
 *   precision.  I also handle large x differently.
 */



double complex ctanh(double complex z)
{
	double x, y;
	double t, beta, s, rho, denom;
	uint32_t hx, ix, lx;

	x = creal(z);
	y = cimag(z);

	EXTRACT_WORDS(hx, lx, x);
	ix = hx & 0x7fffffff;

	/*
	 * ctanh(NaN + i 0) = NaN + i 0
	 *
	 * ctanh(NaN + i y) = NaN + i NaN               for y != 0
	 *
	 * The imaginary part has the sign of x*sin(2*y), but there's no
	 * special effort to get this right.
	 *
	 * ctanh(+-Inf +- i Inf) = +-1 +- 0
	 *
	 * ctanh(+-Inf + i y) = +-1 + 0 sin(2y)         for y finite
	 *
	 * The imaginary part of the sign is unspecified.  This special
	 * case is only needed to avoid a spurious invalid exception when
	 * y is infinite.
	 */
	if (ix >= 0x7ff00000) {
		if ((ix & 0xfffff) | lx)        /* x is NaN */
			return CMPLX(x, (y == 0 ? y : x * y));
		SET_HIGH_WORD(x, hx - 0x40000000);      /* x = copysign(1, x) */
		return CMPLX(x, copysign(0, isinf(y) ? y : sin(y) * cos(y)));
	}

	/*
	 * ctanh(x + i NAN) = NaN + i NaN
	 * ctanh(x +- i Inf) = NaN + i NaN
	 */
	if (!isfinite(y))
		return CMPLX(y - y, y - y);

	/*
	 * ctanh(+-huge + i +-y) ~= +-1 +- i 2sin(2y)/exp(2x), using the
	 * approximation sinh^2(huge) ~= exp(2*huge) / 4.
	 * We use a modified formula to avoid spurious overflow.
	 */
	if (ix >= 0x40360000) { /* x >= 22 */
		double exp_mx = exp(-fabs(x));
		return CMPLX(copysign(1, x), 4 * sin(y) * cos(y) * exp_mx * exp_mx);
	}

	/* Kahan's algorithm */
	t = tan(y);
	beta = 1.0 + t * t;     /* = 1 / cos^2(y) */
	s = sinh(x);
	rho = sqrt(1 + s * s);  /* = cosh(x) */
	denom = 1 + beta * s * s;
	return CMPLX((beta * rho * s) / denom, t / denom);
}
/* origin: FreeBSD /usr/src/lib/msun/src/s_ctanhf.c */
/*-
 * Copyright (c) 2011 David Schultz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Hyperbolic tangent of a complex argument z.  See s_ctanh.c for details.
 */



float complex ctanhf(float complex z)
{
	float x, y;
	float t, beta, s, rho, denom;
	uint32_t hx, ix;

	x = crealf(z);
	y = cimagf(z);

	GET_FLOAT_WORD(hx, x);
	ix = hx & 0x7fffffff;

	if (ix >= 0x7f800000) {
		if (ix & 0x7fffff)
			return CMPLXF(x, (y == 0 ? y : x * y));
		SET_FLOAT_WORD(x, hx - 0x40000000);
		return CMPLXF(x, copysignf(0, isinf(y) ? y : sinf(y) * cosf(y)));
	}

	if (!isfinite(y))
		return CMPLXF(y - y, y - y);

	if (ix >= 0x41300000) { /* x >= 11 */
		float exp_mx = expf(-fabsf(x));
		return CMPLXF(copysignf(1, x), 4 * sinf(y) * cosf(y) * exp_mx * exp_mx);
	}

	t = tanf(y);
	beta = 1.0 + t * t;
	s = sinhf(x);
	rho = sqrtf(1 + s * s);
	denom = 1 + beta * s * s;
	return CMPLXF((beta * rho * s) / denom, t / denom);
}


//FIXME
long double complex ctanhl(long double complex z)
{
	return ctanh(z);
}


#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
long double complex ctanl(long double complex z)
{
	return ctan(z);
}
#else
long double complex ctanl(long double complex z)
{
	z = ctanhl(CMPLXL(-cimagl(z), creall(z)));
	return CMPLXL(cimagl(z), -creall(z));
}
#endif

double complex CMPLX(double x, double y){
	double complex ans = (double complex) 0;
	__real__ ans = x;
	__imag__ ans = y;
	return ans;
}

float complex CMPLX(float x, float y){
	float complex ans = (float complex) 0;
	__real__ ans = x;
	__imag__ ans = y;
	return ans;
}
long double complex CMPLX(long double x, long double y){
	long double complex ans = (long double complex) 0;
	__real__ ans = x;
	__imag__ ans = y;
	return ans;
}

double creal(double complex x){
	return __real__ x;
}
float crealf(float complex x){
	return __real__ x;
}
long double creall(long double complex x){
	return __real__ x;
}

double cimag(double complex x){
	return __imag__ x;
}
float cimagf(float complex x){
	return __imag__ x;
}
long double cimagl(long double complex x){
	return __imag__ x;
}

double complex __muldc3(double __a, double __b, double __c, double __d){
	double complex ret;
	__real__ ret = __a*__c-__b*__d;
	__imag__ ret = __a*__d+__b*__c;
	return ret;
}
