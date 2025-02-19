/* dlargv.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"
#include "blas.h"
#include "clapack.h"

/* Subroutine */ int dlargv_(integer *n, doublereal *x, integer *incx, 
	doublereal *y, integer *incy, doublereal *c__, integer *incc)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    doublereal f, g;
    integer i__;
    doublereal t;
    integer ic, ix, iy;
    doublereal tt;


/*  -- LAPACK auxiliary routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DLARGV generates a vector of real plane rotations, determined by */
/*  elements of the real vectors x and y. For i = 1,2,...,n */

/*     (  c(i)  s(i) ) ( x(i) ) = ( a(i) ) */
/*     ( -s(i)  c(i) ) ( y(i) ) = (   0  ) */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The number of plane rotations to be generated. */

/*  X       (input/output) DOUBLE PRECISION array, */
/*                         dimension (1+(N-1)*INCX) */
/*          On entry, the vector x. */
/*          On exit, x(i) is overwritten by a(i), for i = 1,...,n. */

/*  INCX    (input) INTEGER */
/*          The increment between elements of X. INCX > 0. */

/*  Y       (input/output) DOUBLE PRECISION array, */
/*                         dimension (1+(N-1)*INCY) */
/*          On entry, the vector y. */
/*          On exit, the sines of the plane rotations. */

/*  INCY    (input) INTEGER */
/*          The increment between elements of Y. INCY > 0. */

/*  C       (output) DOUBLE PRECISION array, dimension (1+(N-1)*INCC) */
/*          The cosines of the plane rotations. */

/*  INCC    (input) INTEGER */
/*          The increment between elements of C. INCC > 0. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --c__;
    --y;
    --x;

    /* Function Body */
    ix = 1;
    iy = 1;
    ic = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	f = x[ix];
	g = y[iy];
	if (g == 0.) {
	    c__[ic] = 1.;
	} else if (f == 0.) {
	    c__[ic] = 0.;
	    y[iy] = 1.;
	    x[ix] = g;
	} else if (abs(f) > abs(g)) {
	    t = g / f;
	    tt = sqrt(t * t + 1.);
	    c__[ic] = 1. / tt;
	    y[iy] = t * c__[ic];
	    x[ix] = f * tt;
	} else {
	    t = f / g;
	    tt = sqrt(t * t + 1.);
	    y[iy] = 1. / tt;
	    c__[ic] = t * y[iy];
	    x[ix] = g * tt;
	}
	ic += *incc;
	iy += *incy;
	ix += *incx;
/* L10: */
    }
    return 0;

/*     End of DLARGV */

} /* dlargv_ */
