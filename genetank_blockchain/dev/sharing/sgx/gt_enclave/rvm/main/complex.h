#ifndef _COMPLEX_H
#define _COMPLEX_H

long double operator "" _fi(long double);
#include <complex.h>

#ifdef __cplusplus
extern "C" {
#endif


#undef complex
#define complex _Complex
//#ifdef __GNUC__
//#define _Complex_I (__extension__ 1.0fi)
//#else
#define _Complex_I 1.0_fi
//#endif
#undef I
#define I _Complex_I

double complex cacos(double complex);
float complex cacosf(float complex);
long double complex cacosl(long double complex);

double complex casin(double complex);
float complex casinf(float complex);
long double complex casinl(long double complex);

double complex catan(double complex);
float complex catanf(float complex);
long double complex catanl(long double complex);

double complex ccos(double complex);
float complex ccosf(float complex);
long double complex ccosl(long double complex);

double complex csin(double complex);
float complex csinf(float complex);
long double complex csinl(long double complex);

double complex ctan(double complex);
float complex ctanf(float complex);
long double complex ctanl(long double complex);

double complex cacosh(double complex);
float complex cacoshf(float complex);
long double complex cacoshl(long double complex);

double complex casinh(double complex);
float complex casinhf(float complex);
long double complex casinhl(long double complex);

double complex catanh(double complex);
float complex catanhf(float complex);
long double complex catanhl(long double complex);

double complex ccosh(double complex);
float complex ccoshf(float complex);
long double complex ccoshl(long double complex);

double complex csinh(double complex);
float complex csinhf(float complex);
long double complex csinhl(long double complex);

double complex ctanh(double complex);
float complex ctanhf(float complex);
long double complex ctanhl(long double complex);

double complex cexp(double complex);
float complex cexpf(float complex);
long double complex cexpl(long double complex);

double complex clog(double complex);
float complex clogf(float complex);
long double complex clogl(long double complex);

double cabs(double complex);
float cabsf(float complex);
long double cabsl(long double complex);

double complex cpow(double complex, double complex);
float complex cpowf(float complex, float complex);
long double complex cpowl(long double complex, long double complex);

double complex csqrt(double complex);
float complex csqrtf(float complex);
long double complex csqrtl(long double complex);

double carg(double complex);
float cargf(float complex);
long double cargl(long double complex);

double cimag(double complex);
float cimagf(float complex);
long double cimagl(long double complex);

double complex conj(double complex);
float complex conjf(float complex);
long double complex conjl(long double complex);

double complex cproj(double complex);
float complex cprojf(float complex);
long double complex cprojl(long double complex);

double creal(double complex);
float crealf(float complex);
long double creall(long double complex);

double complex CMPLX(double x, double y);
float complex CMPLXF(float x, float y);
long double complex CMPLXL(long double x, long double y);
//#define __CIMAG(x, t) \
	((union { _Complex t __z; t __xy[2]; }){(_Complex t)(x)}.__xy[1])
double creal(double complex x);
float crealf(float complex x);
long double creall(long double complex x);
//#define creal(x) ((double)(__real__ x))
//#define crealf(x) (__real__ x)
//#define creall(x) (__real__ x)
double cimag(double complex x);
float cimagf(float complex x);
long double cimagl(long double complex x);
//#define cimag(x) ((double)(__imag__ x))//__CIMAG(x, double)
//#define cimagf(x) (__imag__ x)//__CIMAG(x, float)
//#define cimagl(x) (__imag__ x)//__CIMAG(x, long double)

//#define __CMPLX(x, y, t) \
//	((union { _Complex t __z; t __xy[2]; }){.__xy = {(x),(y)}}.__z)
//    {t complex ans = (t complex) 0; __real__ ans = x; __imag__ ans = y; return ans}

//#define CMPLX(x, y) __CMPLX(x, y, double)
//#define CMPLXF(x, y) __CMPLX(x, y, float)
//#define CMPLXL(x, y) __CMPLX(x, y, long double)

double complex __ldexp_cexp(double complex,int);
float complex __ldexp_cexpf(float complex,int);

double complex __muldc3(double, double, double, double);

#define EXTRACT_WORDS(hi,lo,d)                    \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (hi) = __u.i >> 32;                             \
  (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Get the more significant 32 bit int from a double.  */
#define GET_HIGH_WORD(hi,d)                       \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (hi) = __u.i >> 32;                             \
} while (0)

/* Get the less significant 32 bit int from a double.  */
#define GET_LOW_WORD(lo,d)                        \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Set a double from two 32 bit ints.  */
#define INSERT_WORDS(d,hi,lo)                     \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.i = ((uint64_t)(hi)<<32) | (uint32_t)(lo);  \
  (d) = __u.f;                                    \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */
#define SET_HIGH_WORD(d,hi)                       \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  __u.i &= 0xffffffff;                            \
  __u.i |= (uint64_t)(hi) << 32;                  \
  (d) = __u.f;                                    \
} while (0)

/* Set the less significant 32 bits of a double from an int.  */
#define SET_LOW_WORD(d,lo)                        \
do {                                              \
  union {double f; uint64_t i;} __u;              \
  __u.f = (d);                                    \
  __u.i &= 0xffffffff00000000ull;                 \
  __u.i |= (uint32_t)(lo);                        \
  (d) = __u.f;                                    \
} while (0)

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(w,d)                       \
do {                                              \
  union {float f; uint32_t i;} __u;               \
  __u.f = (d);                                    \
  (w) = __u.i;                                    \
} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d,w)                       \
do {                                              \
  union {float f; uint32_t i;} __u;               \
  __u.i = (w);                                    \
  (d) = __u.f;                                    \
} while (0)

#ifdef __cplusplus
}
#endif
#endif


