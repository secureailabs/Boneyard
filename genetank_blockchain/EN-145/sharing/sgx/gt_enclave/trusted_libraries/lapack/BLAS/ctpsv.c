#include "blas.h"
/* ctpsv.f -- translated by f2c (version 20160102).
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

/* > \brief \b CTPSV */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE CTPSV(UPLO,TRANS,DIAG,N,AP,X,INCX) */

/*       .. Scalar Arguments .. */
/*       INTEGER INCX,N */
/*       CHARACTER DIAG,TRANS,UPLO */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX AP(*),X(*) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > CTPSV  solves one of the systems of equations */
/* > */
/* >    A*x = b,   or   A**T*x = b,   or   A**H*x = b, */
/* > */
/* > where b and x are n element vectors and A is an n by n unit, or */
/* > non-unit, upper or lower triangular matrix, supplied in packed form. */
/* > */
/* > No test for singularity or near-singularity is included in this */
/* > routine. Such tests must be performed before calling this routine. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >           On entry, UPLO specifies whether the matrix is an upper or */
/* >           lower triangular matrix as follows: */
/* > */
/* >              UPLO = 'U' or 'u'   A is an upper triangular matrix. */
/* > */
/* >              UPLO = 'L' or 'l'   A is a lower triangular matrix. */
/* > \endverbatim */
/* > */
/* > \param[in] TRANS */
/* > \verbatim */
/* >          TRANS is CHARACTER*1 */
/* >           On entry, TRANS specifies the equations to be solved as */
/* >           follows: */
/* > */
/* >              TRANS = 'N' or 'n'   A*x = b. */
/* > */
/* >              TRANS = 'T' or 't'   A**T*x = b. */
/* > */
/* >              TRANS = 'C' or 'c'   A**H*x = b. */
/* > \endverbatim */
/* > */
/* > \param[in] DIAG */
/* > \verbatim */
/* >          DIAG is CHARACTER*1 */
/* >           On entry, DIAG specifies whether or not A is unit */
/* >           triangular as follows: */
/* > */
/* >              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */
/* > */
/* >              DIAG = 'N' or 'n'   A is not assumed to be unit */
/* >                                  triangular. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >           On entry, N specifies the order of the matrix A. */
/* >           N must be at least zero. */
/* > \endverbatim */
/* > */
/* > \param[in] AP */
/* > \verbatim */
/* >          AP is COMPLEX array, dimension at least */
/* >           ( ( n*( n + 1 ) )/2 ). */
/* >           Before entry with  UPLO = 'U' or 'u', the array AP must */
/* >           contain the upper triangular matrix packed sequentially, */
/* >           column by column, so that AP( 1 ) contains a( 1, 1 ), */
/* >           AP( 2 ) and AP( 3 ) contain a( 1, 2 ) and a( 2, 2 ) */
/* >           respectively, and so on. */
/* >           Before entry with UPLO = 'L' or 'l', the array AP must */
/* >           contain the lower triangular matrix packed sequentially, */
/* >           column by column, so that AP( 1 ) contains a( 1, 1 ), */
/* >           AP( 2 ) and AP( 3 ) contain a( 2, 1 ) and a( 3, 1 ) */
/* >           respectively, and so on. */
/* >           Note that when  DIAG = 'U' or 'u', the diagonal elements of */
/* >           A are not referenced, but are assumed to be unity. */
/* > \endverbatim */
/* > */
/* > \param[in,out] X */
/* > \verbatim */
/* >          X is COMPLEX array, dimension at least */
/* >           ( 1 + ( n - 1 )*abs( INCX ) ). */
/* >           Before entry, the incremented array X must contain the n */
/* >           element right-hand side vector b. On exit, X is overwritten */
/* >           with the solution vector x. */
/* > \endverbatim */
/* > */
/* > \param[in] INCX */
/* > \verbatim */
/* >          INCX is INTEGER */
/* >           On entry, INCX specifies the increment for the elements of */
/* >           X. INCX must not be zero. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complex_blas_level2 */

/* > \par Further Details: */
/*  ===================== */
/* > */
/* > \verbatim */
/* > */
/* >  Level 2 Blas routine. */
/* > */
/* >  -- Written on 22-October-1986. */
/* >     Jack Dongarra, Argonne National Lab. */
/* >     Jeremy Du Croz, Nag Central Office. */
/* >     Sven Hammarling, Nag Central Office. */
/* >     Richard Hanson, Sandia National Labs. */
/* > \endverbatim */
/* > */
/*  ===================================================================== */
/* Subroutine */ int ctpsv_(char *uplo, char *trans, char *diag, integer *n, 
	complex *ap, complex *x, integer *incx, ftnlen uplo_len, ftnlen 
	trans_len, ftnlen diag_len)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);

    /* Local variables */
    static integer i__, j, k, kk, ix, jx, kx, info;
    static complex temp;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static logical noconj, nounit;


/*  -- Reference BLAS level2 routine (version 3.7.0) -- */
/*  -- Reference BLAS is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     December 2016 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --x;
    --ap;

    /* Function Body */
    info = 0;
    if (! lsame_(uplo, "U", (ftnlen)1, (ftnlen)1) && ! lsame_(uplo, "L", (
	    ftnlen)1, (ftnlen)1)) {
	info = 1;
    } else if (! lsame_(trans, "N", (ftnlen)1, (ftnlen)1) && ! lsame_(trans, 
	    "T", (ftnlen)1, (ftnlen)1) && ! lsame_(trans, "C", (ftnlen)1, (
	    ftnlen)1)) {
	info = 2;
    } else if (! lsame_(diag, "U", (ftnlen)1, (ftnlen)1) && ! lsame_(diag, 
	    "N", (ftnlen)1, (ftnlen)1)) {
	info = 3;
    } else if (*n < 0) {
	info = 4;
    } else if (*incx == 0) {
	info = 7;
    }
    if (info != 0) {
	xerbla_("CTPSV ", &info, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

    noconj = lsame_(trans, "T", (ftnlen)1, (ftnlen)1);
    nounit = lsame_(diag, "N", (ftnlen)1, (ftnlen)1);

/*     Set up the start point in X if the increment is not unity. This */
/*     will be  ( N - 1 )*INCX  too small for descending loops. */

    if (*incx <= 0) {
	kx = 1 - (*n - 1) * *incx;
    } else if (*incx != 1) {
	kx = 1;
    }

/*     Start the operations. In this version the elements of AP are */
/*     accessed sequentially with one pass through AP. */

    if (lsame_(trans, "N", (ftnlen)1, (ftnlen)1)) {

/*        Form  x := inv( A )*x. */

	if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
	    kk = *n * (*n + 1) / 2;
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    i__1 = j;
		    if (x[i__1].r != 0.f || x[i__1].i != 0.f) {
			if (nounit) {
			    i__1 = j;
			    c_div(&q__1, &x[j], &ap[kk]);
			    x[i__1].r = q__1.r, x[i__1].i = q__1.i;
			}
			i__1 = j;
			temp.r = x[i__1].r, temp.i = x[i__1].i;
			k = kk - 1;
			for (i__ = j - 1; i__ >= 1; --i__) {
			    i__1 = i__;
			    i__2 = i__;
			    i__3 = k;
			    q__2.r = temp.r * ap[i__3].r - temp.i * ap[i__3]
				    .i, q__2.i = temp.r * ap[i__3].i + temp.i 
				    * ap[i__3].r;
			    q__1.r = x[i__2].r - q__2.r, q__1.i = x[i__2].i - 
				    q__2.i;
			    x[i__1].r = q__1.r, x[i__1].i = q__1.i;
			    --k;
/* L10: */
			}
		    }
		    kk -= j;
/* L20: */
		}
	    } else {
		jx = kx + (*n - 1) * *incx;
		for (j = *n; j >= 1; --j) {
		    i__1 = jx;
		    if (x[i__1].r != 0.f || x[i__1].i != 0.f) {
			if (nounit) {
			    i__1 = jx;
			    c_div(&q__1, &x[jx], &ap[kk]);
			    x[i__1].r = q__1.r, x[i__1].i = q__1.i;
			}
			i__1 = jx;
			temp.r = x[i__1].r, temp.i = x[i__1].i;
			ix = jx;
			i__1 = kk - j + 1;
			for (k = kk - 1; k >= i__1; --k) {
			    ix -= *incx;
			    i__2 = ix;
			    i__3 = ix;
			    i__4 = k;
			    q__2.r = temp.r * ap[i__4].r - temp.i * ap[i__4]
				    .i, q__2.i = temp.r * ap[i__4].i + temp.i 
				    * ap[i__4].r;
			    q__1.r = x[i__3].r - q__2.r, q__1.i = x[i__3].i - 
				    q__2.i;
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
/* L30: */
			}
		    }
		    jx -= *incx;
		    kk -= j;
/* L40: */
		}
	    }
	} else {
	    kk = 1;
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    if (x[i__2].r != 0.f || x[i__2].i != 0.f) {
			if (nounit) {
			    i__2 = j;
			    c_div(&q__1, &x[j], &ap[kk]);
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
			}
			i__2 = j;
			temp.r = x[i__2].r, temp.i = x[i__2].i;
			k = kk + 1;
			i__2 = *n;
			for (i__ = j + 1; i__ <= i__2; ++i__) {
			    i__3 = i__;
			    i__4 = i__;
			    i__5 = k;
			    q__2.r = temp.r * ap[i__5].r - temp.i * ap[i__5]
				    .i, q__2.i = temp.r * ap[i__5].i + temp.i 
				    * ap[i__5].r;
			    q__1.r = x[i__4].r - q__2.r, q__1.i = x[i__4].i - 
				    q__2.i;
			    x[i__3].r = q__1.r, x[i__3].i = q__1.i;
			    ++k;
/* L50: */
			}
		    }
		    kk += *n - j + 1;
/* L60: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = jx;
		    if (x[i__2].r != 0.f || x[i__2].i != 0.f) {
			if (nounit) {
			    i__2 = jx;
			    c_div(&q__1, &x[jx], &ap[kk]);
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
			}
			i__2 = jx;
			temp.r = x[i__2].r, temp.i = x[i__2].i;
			ix = jx;
			i__2 = kk + *n - j;
			for (k = kk + 1; k <= i__2; ++k) {
			    ix += *incx;
			    i__3 = ix;
			    i__4 = ix;
			    i__5 = k;
			    q__2.r = temp.r * ap[i__5].r - temp.i * ap[i__5]
				    .i, q__2.i = temp.r * ap[i__5].i + temp.i 
				    * ap[i__5].r;
			    q__1.r = x[i__4].r - q__2.r, q__1.i = x[i__4].i - 
				    q__2.i;
			    x[i__3].r = q__1.r, x[i__3].i = q__1.i;
/* L70: */
			}
		    }
		    jx += *incx;
		    kk += *n - j + 1;
/* L80: */
		}
	    }
	}
    } else {

/*        Form  x := inv( A**T )*x  or  x := inv( A**H )*x. */

	if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
	    kk = 1;
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    temp.r = x[i__2].r, temp.i = x[i__2].i;
		    k = kk;
		    if (noconj) {
			i__2 = j - 1;
			for (i__ = 1; i__ <= i__2; ++i__) {
			    i__3 = k;
			    i__4 = i__;
			    q__2.r = ap[i__3].r * x[i__4].r - ap[i__3].i * x[
				    i__4].i, q__2.i = ap[i__3].r * x[i__4].i 
				    + ap[i__3].i * x[i__4].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ++k;
/* L90: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &ap[kk + j - 1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
			i__2 = j - 1;
			for (i__ = 1; i__ <= i__2; ++i__) {
			    r_cnjg(&q__3, &ap[k]);
			    i__3 = i__;
			    q__2.r = q__3.r * x[i__3].r - q__3.i * x[i__3].i, 
				    q__2.i = q__3.r * x[i__3].i + q__3.i * x[
				    i__3].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ++k;
/* L100: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &ap[kk + j - 1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__2 = j;
		    x[i__2].r = temp.r, x[i__2].i = temp.i;
		    kk += j;
/* L110: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = jx;
		    temp.r = x[i__2].r, temp.i = x[i__2].i;
		    ix = kx;
		    if (noconj) {
			i__2 = kk + j - 2;
			for (k = kk; k <= i__2; ++k) {
			    i__3 = k;
			    i__4 = ix;
			    q__2.r = ap[i__3].r * x[i__4].r - ap[i__3].i * x[
				    i__4].i, q__2.i = ap[i__3].r * x[i__4].i 
				    + ap[i__3].i * x[i__4].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix += *incx;
/* L120: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &ap[kk + j - 1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
			i__2 = kk + j - 2;
			for (k = kk; k <= i__2; ++k) {
			    r_cnjg(&q__3, &ap[k]);
			    i__3 = ix;
			    q__2.r = q__3.r * x[i__3].r - q__3.i * x[i__3].i, 
				    q__2.i = q__3.r * x[i__3].i + q__3.i * x[
				    i__3].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix += *incx;
/* L130: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &ap[kk + j - 1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__2 = jx;
		    x[i__2].r = temp.r, x[i__2].i = temp.i;
		    jx += *incx;
		    kk += j;
/* L140: */
		}
	    }
	} else {
	    kk = *n * (*n + 1) / 2;
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    i__1 = j;
		    temp.r = x[i__1].r, temp.i = x[i__1].i;
		    k = kk;
		    if (noconj) {
			i__1 = j + 1;
			for (i__ = *n; i__ >= i__1; --i__) {
			    i__2 = k;
			    i__3 = i__;
			    q__2.r = ap[i__2].r * x[i__3].r - ap[i__2].i * x[
				    i__3].i, q__2.i = ap[i__2].r * x[i__3].i 
				    + ap[i__2].i * x[i__3].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    --k;
/* L150: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &ap[kk - *n + j]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
			i__1 = j + 1;
			for (i__ = *n; i__ >= i__1; --i__) {
			    r_cnjg(&q__3, &ap[k]);
			    i__2 = i__;
			    q__2.r = q__3.r * x[i__2].r - q__3.i * x[i__2].i, 
				    q__2.i = q__3.r * x[i__2].i + q__3.i * x[
				    i__2].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    --k;
/* L160: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &ap[kk - *n + j]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__1 = j;
		    x[i__1].r = temp.r, x[i__1].i = temp.i;
		    kk -= *n - j + 1;
/* L170: */
		}
	    } else {
		kx += (*n - 1) * *incx;
		jx = kx;
		for (j = *n; j >= 1; --j) {
		    i__1 = jx;
		    temp.r = x[i__1].r, temp.i = x[i__1].i;
		    ix = kx;
		    if (noconj) {
			i__1 = kk - (*n - (j + 1));
			for (k = kk; k >= i__1; --k) {
			    i__2 = k;
			    i__3 = ix;
			    q__2.r = ap[i__2].r * x[i__3].r - ap[i__2].i * x[
				    i__3].i, q__2.i = ap[i__2].r * x[i__3].i 
				    + ap[i__2].i * x[i__3].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix -= *incx;
/* L180: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &ap[kk - *n + j]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
			i__1 = kk - (*n - (j + 1));
			for (k = kk; k >= i__1; --k) {
			    r_cnjg(&q__3, &ap[k]);
			    i__2 = ix;
			    q__2.r = q__3.r * x[i__2].r - q__3.i * x[i__2].i, 
				    q__2.i = q__3.r * x[i__2].i + q__3.i * x[
				    i__2].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix -= *incx;
/* L190: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &ap[kk - *n + j]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__1 = jx;
		    x[i__1].r = temp.r, x[i__1].i = temp.i;
		    jx -= *incx;
		    kk -= *n - j + 1;
/* L200: */
		}
	    }
	}
    }

    return 0;

/*     End of CTPSV . */

} /* ctpsv_ */

