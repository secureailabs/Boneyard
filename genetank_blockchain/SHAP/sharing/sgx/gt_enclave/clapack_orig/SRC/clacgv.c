/* clacgv.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int clacgv_(integer *n, complex *x, integer *incx)
{
    /* System generated locals */
    integer i__1, i__2;
    complex q__1;

    /* Builtin functions */
    void r_cnjg(complex *, complex *);

    /* Local variables */
    integer i__, ioff;


/*  -- LAPACK auxiliary routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CLACGV conjugates a complex vector of length N. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The length of the vector X.  N >= 0. */

/*  X       (input/output) COMPLEX array, dimension */
/*                         (1+(N-1)*abs(INCX)) */
/*          On entry, the vector of length N to be conjugated. */
/*          On exit, X is overwritten with conjg(X). */

/*  INCX    (input) INTEGER */
/*          The spacing between successive elements of X. */

/* ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --x;

    /* Function Body */
    if (*incx == 1) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = i__;
	    r_cnjg(&q__1, &x[i__]);
	    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
/* L10: */
	}
    } else {
	ioff = 1;
	if (*incx < 0) {
	    ioff = 1 - (*n - 1) * *incx;
	}
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = ioff;
	    r_cnjg(&q__1, &x[ioff]);
	    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
	    ioff += *incx;
/* L20: */
	}
    }
    return 0;

/*     End of CLACGV */

} /* clacgv_ */
