#include "blas.h"
#include "clapack.h"
/* zspr.f -- translated by f2c (version 20160102).
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

/* > \brief \b ZSPR performs the symmetrical rank-1 update of a complex symmetric packed matrix. */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download ZSPR + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/zspr.f"
> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/zspr.f"
> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/zspr.f"
> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE ZSPR( UPLO, N, ALPHA, X, INCX, AP ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          UPLO */
/*       INTEGER            INCX, N */
/*       COMPLEX*16         ALPHA */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX*16         AP( * ), X( * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZSPR    performs the symmetric rank 1 operation */
/* > */
/* >    A := alpha*x*x**H + A, */
/* > */
/* > where alpha is a complex scalar, x is an n element vector and A is an */
/* > n by n symmetric matrix, supplied in packed form. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >           On entry, UPLO specifies whether the upper or lower */
/* >           triangular part of the matrix A is supplied in the packed */
/* >           array AP as follows: */
/* > */
/* >              UPLO = 'U' or 'u'   The upper triangular part of A is */
/* >                                  supplied in AP. */
/* > */
/* >              UPLO = 'L' or 'l'   The lower triangular part of A is */
/* >                                  supplied in AP. */
/* > */
/* >           Unchanged on exit. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >           On entry, N specifies the order of the matrix A. */
/* >           N must be at least zero. */
/* >           Unchanged on exit. */
/* > \endverbatim */
/* > */
/* > \param[in] ALPHA */
/* > \verbatim */
/* >          ALPHA is COMPLEX*16 */
/* >           On entry, ALPHA specifies the scalar alpha. */
/* >           Unchanged on exit. */
/* > \endverbatim */
/* > */
/* > \param[in] X */
/* > \verbatim */
/* >          X is COMPLEX*16 array, dimension at least */
/* >           ( 1 + ( N - 1 )*abs( INCX ) ). */
/* >           Before entry, the incremented array X must contain the N- */
/* >           element vector x. */
/* >           Unchanged on exit. */
/* > \endverbatim */
/* > */
/* > \param[in] INCX */
/* > \verbatim */
/* >          INCX is INTEGER */
/* >           On entry, INCX specifies the increment for the elements of */
/* >           X. INCX must not be zero. */
/* >           Unchanged on exit. */
/* > \endverbatim */
/* > */
/* > \param[in,out] AP */
/* > \verbatim */
/* >          AP is COMPLEX*16 array, dimension at least */
/* >           ( ( N*( N + 1 ) )/2 ). */
/* >           Before entry, with  UPLO = 'U' or 'u', the array AP must */
/* >           contain the upper triangular part of the symmetric matrix */
/* >           packed sequentially, column by column, so that AP( 1 ) */
/* >           contains a( 1, 1 ), AP( 2 ) and AP( 3 ) contain a( 1, 2 ) */
/* >           and a( 2, 2 ) respectively, and so on. On exit, the array */
/* >           AP is overwritten by the upper triangular part of the */
/* >           updated matrix. */
/* >           Before entry, with UPLO = 'L' or 'l', the array AP must */
/* >           contain the lower triangular part of the symmetric matrix */
/* >           packed sequentially, column by column, so that AP( 1 ) */
/* >           contains a( 1, 1 ), AP( 2 ) and AP( 3 ) contain a( 2, 1 ) */
/* >           and a( 3, 1 ) respectively, and so on. On exit, the array */
/* >           AP is overwritten by the lower triangular part of the */
/* >           updated matrix. */
/* >           Note that the imaginary parts of the diagonal elements need */
/* >           not be set, they are assumed to be zero, and on exit they */
/* >           are set to zero. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complex16OTHERauxiliary */

/*  ===================================================================== */
/* Subroutine */ int zspr_(char *uplo, integer *n, doublecomplex *alpha, 
	doublecomplex *x, integer *incx, doublecomplex *ap, ftnlen uplo_len)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublecomplex z__1, z__2;

    /* Local variables */
    static integer i__, j, k, kk, ix, jx, kx, info;
    static doublecomplex temp;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);


/*  -- LAPACK auxiliary routine (version 3.7.0) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     December 2016 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/* ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --ap;
    --x;

    /* Function Body */
    info = 0;
    if (! lsame_(uplo, "U", (ftnlen)1, (ftnlen)1) && ! lsame_(uplo, "L", (
	    ftnlen)1, (ftnlen)1)) {
	info = 1;
    } else if (*n < 0) {
	info = 2;
    } else if (*incx == 0) {
	info = 5;
    }
    if (info != 0) {
	xerbla_("ZSPR  ", &info, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0 || alpha->r == 0. && alpha->i == 0.) {
	return 0;
    }

/*     Set the start point in X if the increment is not unity. */

    if (*incx <= 0) {
	kx = 1 - (*n - 1) * *incx;
    } else if (*incx != 1) {
	kx = 1;
    }

/*     Start the operations. In this version the elements of the array AP */
/*     are accessed sequentially with one pass through AP. */

    kk = 1;
    if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {

/*        Form  A  when upper triangle is stored in AP. */

	if (*incx == 1) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = j;
		if (x[i__2].r != 0. || x[i__2].i != 0.) {
		    i__2 = j;
		    z__1.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__1.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    k = kk;
		    i__2 = j - 1;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = k;
			i__4 = k;
			i__5 = i__;
			z__2.r = x[i__5].r * temp.r - x[i__5].i * temp.i, 
				z__2.i = x[i__5].r * temp.i + x[i__5].i * 
				temp.r;
			z__1.r = ap[i__4].r + z__2.r, z__1.i = ap[i__4].i + 
				z__2.i;
			ap[i__3].r = z__1.r, ap[i__3].i = z__1.i;
			++k;
/* L10: */
		    }
		    i__2 = kk + j - 1;
		    i__3 = kk + j - 1;
		    i__4 = j;
		    z__2.r = x[i__4].r * temp.r - x[i__4].i * temp.i, z__2.i =
			     x[i__4].r * temp.i + x[i__4].i * temp.r;
		    z__1.r = ap[i__3].r + z__2.r, z__1.i = ap[i__3].i + 
			    z__2.i;
		    ap[i__2].r = z__1.r, ap[i__2].i = z__1.i;
		} else {
		    i__2 = kk + j - 1;
		    i__3 = kk + j - 1;
		    ap[i__2].r = ap[i__3].r, ap[i__2].i = ap[i__3].i;
		}
		kk += j;
/* L20: */
	    }
	} else {
	    jx = kx;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = jx;
		if (x[i__2].r != 0. || x[i__2].i != 0.) {
		    i__2 = jx;
		    z__1.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__1.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    ix = kx;
		    i__2 = kk + j - 2;
		    for (k = kk; k <= i__2; ++k) {
			i__3 = k;
			i__4 = k;
			i__5 = ix;
			z__2.r = x[i__5].r * temp.r - x[i__5].i * temp.i, 
				z__2.i = x[i__5].r * temp.i + x[i__5].i * 
				temp.r;
			z__1.r = ap[i__4].r + z__2.r, z__1.i = ap[i__4].i + 
				z__2.i;
			ap[i__3].r = z__1.r, ap[i__3].i = z__1.i;
			ix += *incx;
/* L30: */
		    }
		    i__2 = kk + j - 1;
		    i__3 = kk + j - 1;
		    i__4 = jx;
		    z__2.r = x[i__4].r * temp.r - x[i__4].i * temp.i, z__2.i =
			     x[i__4].r * temp.i + x[i__4].i * temp.r;
		    z__1.r = ap[i__3].r + z__2.r, z__1.i = ap[i__3].i + 
			    z__2.i;
		    ap[i__2].r = z__1.r, ap[i__2].i = z__1.i;
		} else {
		    i__2 = kk + j - 1;
		    i__3 = kk + j - 1;
		    ap[i__2].r = ap[i__3].r, ap[i__2].i = ap[i__3].i;
		}
		jx += *incx;
		kk += j;
/* L40: */
	    }
	}
    } else {

/*        Form  A  when lower triangle is stored in AP. */

	if (*incx == 1) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = j;
		if (x[i__2].r != 0. || x[i__2].i != 0.) {
		    i__2 = j;
		    z__1.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__1.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    i__2 = kk;
		    i__3 = kk;
		    i__4 = j;
		    z__2.r = temp.r * x[i__4].r - temp.i * x[i__4].i, z__2.i =
			     temp.r * x[i__4].i + temp.i * x[i__4].r;
		    z__1.r = ap[i__3].r + z__2.r, z__1.i = ap[i__3].i + 
			    z__2.i;
		    ap[i__2].r = z__1.r, ap[i__2].i = z__1.i;
		    k = kk + 1;
		    i__2 = *n;
		    for (i__ = j + 1; i__ <= i__2; ++i__) {
			i__3 = k;
			i__4 = k;
			i__5 = i__;
			z__2.r = x[i__5].r * temp.r - x[i__5].i * temp.i, 
				z__2.i = x[i__5].r * temp.i + x[i__5].i * 
				temp.r;
			z__1.r = ap[i__4].r + z__2.r, z__1.i = ap[i__4].i + 
				z__2.i;
			ap[i__3].r = z__1.r, ap[i__3].i = z__1.i;
			++k;
/* L50: */
		    }
		} else {
		    i__2 = kk;
		    i__3 = kk;
		    ap[i__2].r = ap[i__3].r, ap[i__2].i = ap[i__3].i;
		}
		kk = kk + *n - j + 1;
/* L60: */
	    }
	} else {
	    jx = kx;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = jx;
		if (x[i__2].r != 0. || x[i__2].i != 0.) {
		    i__2 = jx;
		    z__1.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__1.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    i__2 = kk;
		    i__3 = kk;
		    i__4 = jx;
		    z__2.r = temp.r * x[i__4].r - temp.i * x[i__4].i, z__2.i =
			     temp.r * x[i__4].i + temp.i * x[i__4].r;
		    z__1.r = ap[i__3].r + z__2.r, z__1.i = ap[i__3].i + 
			    z__2.i;
		    ap[i__2].r = z__1.r, ap[i__2].i = z__1.i;
		    ix = jx;
		    i__2 = kk + *n - j;
		    for (k = kk + 1; k <= i__2; ++k) {
			ix += *incx;
			i__3 = k;
			i__4 = k;
			i__5 = ix;
			z__2.r = x[i__5].r * temp.r - x[i__5].i * temp.i, 
				z__2.i = x[i__5].r * temp.i + x[i__5].i * 
				temp.r;
			z__1.r = ap[i__4].r + z__2.r, z__1.i = ap[i__4].i + 
				z__2.i;
			ap[i__3].r = z__1.r, ap[i__3].i = z__1.i;
/* L70: */
		    }
		} else {
		    i__2 = kk;
		    i__3 = kk;
		    ap[i__2].r = ap[i__3].r, ap[i__2].i = ap[i__3].i;
		}
		jx += *incx;
		kk = kk + *n - j + 1;
/* L80: */
	    }
	}
    }

    return 0;

/*     End of ZSPR */

} /* zspr_ */
