#include "blas.h"
/* zher2.f -- translated by f2c (version 20160102).
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

/* > \brief \b ZHER2 */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE ZHER2(UPLO,N,ALPHA,X,INCX,Y,INCY,A,LDA) */

/*       .. Scalar Arguments .. */
/*       COMPLEX*16 ALPHA */
/*       INTEGER INCX,INCY,LDA,N */
/*       CHARACTER UPLO */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX*16 A(LDA,*),X(*),Y(*) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZHER2  performs the hermitian rank 2 operation */
/* > */
/* >    A := alpha*x*y**H + conjg( alpha )*y*x**H + A, */
/* > */
/* > where alpha is a scalar, x and y are n element vectors and A is an n */
/* > by n hermitian matrix. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >           On entry, UPLO specifies whether the upper or lower */
/* >           triangular part of the array A is to be referenced as */
/* >           follows: */
/* > */
/* >              UPLO = 'U' or 'u'   Only the upper triangular part of A */
/* >                                  is to be referenced. */
/* > */
/* >              UPLO = 'L' or 'l'   Only the lower triangular part of A */
/* >                                  is to be referenced. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >           On entry, N specifies the order of the matrix A. */
/* >           N must be at least zero. */
/* > \endverbatim */
/* > */
/* > \param[in] ALPHA */
/* > \verbatim */
/* >          ALPHA is COMPLEX*16 */
/* >           On entry, ALPHA specifies the scalar alpha. */
/* > \endverbatim */
/* > */
/* > \param[in] X */
/* > \verbatim */
/* >          X is COMPLEX*16 array, dimension at least */
/* >           ( 1 + ( n - 1 )*abs( INCX ) ). */
/* >           Before entry, the incremented array X must contain the n */
/* >           element vector x. */
/* > \endverbatim */
/* > */
/* > \param[in] INCX */
/* > \verbatim */
/* >          INCX is INTEGER */
/* >           On entry, INCX specifies the increment for the elements of */
/* >           X. INCX must not be zero. */
/* > \endverbatim */
/* > */
/* > \param[in] Y */
/* > \verbatim */
/* >          Y is COMPLEX*16 array, dimension at least */
/* >           ( 1 + ( n - 1 )*abs( INCY ) ). */
/* >           Before entry, the incremented array Y must contain the n */
/* >           element vector y. */
/* > \endverbatim */
/* > */
/* > \param[in] INCY */
/* > \verbatim */
/* >          INCY is INTEGER */
/* >           On entry, INCY specifies the increment for the elements of */
/* >           Y. INCY must not be zero. */
/* > \endverbatim */
/* > */
/* > \param[in,out] A */
/* > \verbatim */
/* >          A is COMPLEX*16 array, dimension ( LDA, N ) */
/* >           Before entry with  UPLO = 'U' or 'u', the leading n by n */
/* >           upper triangular part of the array A must contain the upper */
/* >           triangular part of the hermitian matrix and the strictly */
/* >           lower triangular part of A is not referenced. On exit, the */
/* >           upper triangular part of the array A is overwritten by the */
/* >           upper triangular part of the updated matrix. */
/* >           Before entry with UPLO = 'L' or 'l', the leading n by n */
/* >           lower triangular part of the array A must contain the lower */
/* >           triangular part of the hermitian matrix and the strictly */
/* >           upper triangular part of A is not referenced. On exit, the */
/* >           lower triangular part of the array A is overwritten by the */
/* >           lower triangular part of the updated matrix. */
/* >           Note that the imaginary parts of the diagonal elements need */
/* >           not be set, they are assumed to be zero, and on exit they */
/* >           are set to zero. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >           On entry, LDA specifies the first dimension of A as declared */
/* >           in the calling (sub) program. LDA must be at least */
/* >           max( 1, n ). */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complex16_blas_level2 */

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
/* Subroutine */ int zher2_(char *uplo, integer *n, doublecomplex *alpha, 
	doublecomplex *x, integer *incx, doublecomplex *y, integer *incy, 
	doublecomplex *a, integer *lda, ftnlen uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4, i__5, i__6;
    doublereal d__1;
    doublecomplex z__1, z__2, z__3, z__4;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer i__, j, ix, iy, jx, jy, kx, ky, info;
    static doublecomplex temp1, temp2;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);


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
    --y;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    info = 0;
    if (! lsame_(uplo, "U", (ftnlen)1, (ftnlen)1) && ! lsame_(uplo, "L", (
	    ftnlen)1, (ftnlen)1)) {
	info = 1;
    } else if (*n < 0) {
	info = 2;
    } else if (*incx == 0) {
	info = 5;
    } else if (*incy == 0) {
	info = 7;
    } else if (*lda < max(1,*n)) {
	info = 9;
    }
    if (info != 0) {
	xerbla_("ZHER2 ", &info, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0 || alpha->r == 0. && alpha->i == 0.) {
	return 0;
    }

/*     Set up the start points in X and Y if the increments are not both */
/*     unity. */

    if (*incx != 1 || *incy != 1) {
	if (*incx > 0) {
	    kx = 1;
	} else {
	    kx = 1 - (*n - 1) * *incx;
	}
	if (*incy > 0) {
	    ky = 1;
	} else {
	    ky = 1 - (*n - 1) * *incy;
	}
	jx = kx;
	jy = ky;
    }

/*     Start the operations. In this version the elements of A are */
/*     accessed sequentially with one pass through the triangular part */
/*     of A. */

    if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {

/*        Form  A  when A is stored in the upper triangle. */

	if (*incx == 1 && *incy == 1) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = j;
		i__3 = j;
		if (x[i__2].r != 0. || x[i__2].i != 0. || (y[i__3].r != 0. || 
			y[i__3].i != 0.)) {
		    d_cnjg(&z__2, &y[j]);
		    z__1.r = alpha->r * z__2.r - alpha->i * z__2.i, z__1.i = 
			    alpha->r * z__2.i + alpha->i * z__2.r;
		    temp1.r = z__1.r, temp1.i = z__1.i;
		    i__2 = j;
		    z__2.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__2.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    d_cnjg(&z__1, &z__2);
		    temp2.r = z__1.r, temp2.i = z__1.i;
		    i__2 = j - 1;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * a_dim1;
			i__4 = i__ + j * a_dim1;
			i__5 = i__;
			z__3.r = x[i__5].r * temp1.r - x[i__5].i * temp1.i, 
				z__3.i = x[i__5].r * temp1.i + x[i__5].i * 
				temp1.r;
			z__2.r = a[i__4].r + z__3.r, z__2.i = a[i__4].i + 
				z__3.i;
			i__6 = i__;
			z__4.r = y[i__6].r * temp2.r - y[i__6].i * temp2.i, 
				z__4.i = y[i__6].r * temp2.i + y[i__6].i * 
				temp2.r;
			z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
			a[i__3].r = z__1.r, a[i__3].i = z__1.i;
/* L10: */
		    }
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    i__4 = j;
		    z__2.r = x[i__4].r * temp1.r - x[i__4].i * temp1.i, 
			    z__2.i = x[i__4].r * temp1.i + x[i__4].i * 
			    temp1.r;
		    i__5 = j;
		    z__3.r = y[i__5].r * temp2.r - y[i__5].i * temp2.i, 
			    z__3.i = y[i__5].r * temp2.i + y[i__5].i * 
			    temp2.r;
		    z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
		    d__1 = a[i__3].r + z__1.r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		} else {
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    d__1 = a[i__3].r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		}
/* L20: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = jx;
		i__3 = jy;
		if (x[i__2].r != 0. || x[i__2].i != 0. || (y[i__3].r != 0. || 
			y[i__3].i != 0.)) {
		    d_cnjg(&z__2, &y[jy]);
		    z__1.r = alpha->r * z__2.r - alpha->i * z__2.i, z__1.i = 
			    alpha->r * z__2.i + alpha->i * z__2.r;
		    temp1.r = z__1.r, temp1.i = z__1.i;
		    i__2 = jx;
		    z__2.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__2.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    d_cnjg(&z__1, &z__2);
		    temp2.r = z__1.r, temp2.i = z__1.i;
		    ix = kx;
		    iy = ky;
		    i__2 = j - 1;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * a_dim1;
			i__4 = i__ + j * a_dim1;
			i__5 = ix;
			z__3.r = x[i__5].r * temp1.r - x[i__5].i * temp1.i, 
				z__3.i = x[i__5].r * temp1.i + x[i__5].i * 
				temp1.r;
			z__2.r = a[i__4].r + z__3.r, z__2.i = a[i__4].i + 
				z__3.i;
			i__6 = iy;
			z__4.r = y[i__6].r * temp2.r - y[i__6].i * temp2.i, 
				z__4.i = y[i__6].r * temp2.i + y[i__6].i * 
				temp2.r;
			z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
			a[i__3].r = z__1.r, a[i__3].i = z__1.i;
			ix += *incx;
			iy += *incy;
/* L30: */
		    }
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    i__4 = jx;
		    z__2.r = x[i__4].r * temp1.r - x[i__4].i * temp1.i, 
			    z__2.i = x[i__4].r * temp1.i + x[i__4].i * 
			    temp1.r;
		    i__5 = jy;
		    z__3.r = y[i__5].r * temp2.r - y[i__5].i * temp2.i, 
			    z__3.i = y[i__5].r * temp2.i + y[i__5].i * 
			    temp2.r;
		    z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
		    d__1 = a[i__3].r + z__1.r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		} else {
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    d__1 = a[i__3].r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		}
		jx += *incx;
		jy += *incy;
/* L40: */
	    }
	}
    } else {

/*        Form  A  when A is stored in the lower triangle. */

	if (*incx == 1 && *incy == 1) {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = j;
		i__3 = j;
		if (x[i__2].r != 0. || x[i__2].i != 0. || (y[i__3].r != 0. || 
			y[i__3].i != 0.)) {
		    d_cnjg(&z__2, &y[j]);
		    z__1.r = alpha->r * z__2.r - alpha->i * z__2.i, z__1.i = 
			    alpha->r * z__2.i + alpha->i * z__2.r;
		    temp1.r = z__1.r, temp1.i = z__1.i;
		    i__2 = j;
		    z__2.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__2.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    d_cnjg(&z__1, &z__2);
		    temp2.r = z__1.r, temp2.i = z__1.i;
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    i__4 = j;
		    z__2.r = x[i__4].r * temp1.r - x[i__4].i * temp1.i, 
			    z__2.i = x[i__4].r * temp1.i + x[i__4].i * 
			    temp1.r;
		    i__5 = j;
		    z__3.r = y[i__5].r * temp2.r - y[i__5].i * temp2.i, 
			    z__3.i = y[i__5].r * temp2.i + y[i__5].i * 
			    temp2.r;
		    z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
		    d__1 = a[i__3].r + z__1.r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		    i__2 = *n;
		    for (i__ = j + 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * a_dim1;
			i__4 = i__ + j * a_dim1;
			i__5 = i__;
			z__3.r = x[i__5].r * temp1.r - x[i__5].i * temp1.i, 
				z__3.i = x[i__5].r * temp1.i + x[i__5].i * 
				temp1.r;
			z__2.r = a[i__4].r + z__3.r, z__2.i = a[i__4].i + 
				z__3.i;
			i__6 = i__;
			z__4.r = y[i__6].r * temp2.r - y[i__6].i * temp2.i, 
				z__4.i = y[i__6].r * temp2.i + y[i__6].i * 
				temp2.r;
			z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
			a[i__3].r = z__1.r, a[i__3].i = z__1.i;
/* L50: */
		    }
		} else {
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    d__1 = a[i__3].r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		}
/* L60: */
	    }
	} else {
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = jx;
		i__3 = jy;
		if (x[i__2].r != 0. || x[i__2].i != 0. || (y[i__3].r != 0. || 
			y[i__3].i != 0.)) {
		    d_cnjg(&z__2, &y[jy]);
		    z__1.r = alpha->r * z__2.r - alpha->i * z__2.i, z__1.i = 
			    alpha->r * z__2.i + alpha->i * z__2.r;
		    temp1.r = z__1.r, temp1.i = z__1.i;
		    i__2 = jx;
		    z__2.r = alpha->r * x[i__2].r - alpha->i * x[i__2].i, 
			    z__2.i = alpha->r * x[i__2].i + alpha->i * x[i__2]
			    .r;
		    d_cnjg(&z__1, &z__2);
		    temp2.r = z__1.r, temp2.i = z__1.i;
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    i__4 = jx;
		    z__2.r = x[i__4].r * temp1.r - x[i__4].i * temp1.i, 
			    z__2.i = x[i__4].r * temp1.i + x[i__4].i * 
			    temp1.r;
		    i__5 = jy;
		    z__3.r = y[i__5].r * temp2.r - y[i__5].i * temp2.i, 
			    z__3.i = y[i__5].r * temp2.i + y[i__5].i * 
			    temp2.r;
		    z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
		    d__1 = a[i__3].r + z__1.r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		    ix = jx;
		    iy = jy;
		    i__2 = *n;
		    for (i__ = j + 1; i__ <= i__2; ++i__) {
			ix += *incx;
			iy += *incy;
			i__3 = i__ + j * a_dim1;
			i__4 = i__ + j * a_dim1;
			i__5 = ix;
			z__3.r = x[i__5].r * temp1.r - x[i__5].i * temp1.i, 
				z__3.i = x[i__5].r * temp1.i + x[i__5].i * 
				temp1.r;
			z__2.r = a[i__4].r + z__3.r, z__2.i = a[i__4].i + 
				z__3.i;
			i__6 = iy;
			z__4.r = y[i__6].r * temp2.r - y[i__6].i * temp2.i, 
				z__4.i = y[i__6].r * temp2.i + y[i__6].i * 
				temp2.r;
			z__1.r = z__2.r + z__4.r, z__1.i = z__2.i + z__4.i;
			a[i__3].r = z__1.r, a[i__3].i = z__1.i;
/* L70: */
		    }
		} else {
		    i__2 = j + j * a_dim1;
		    i__3 = j + j * a_dim1;
		    d__1 = a[i__3].r;
		    a[i__2].r = d__1, a[i__2].i = 0.;
		}
		jx += *incx;
		jy += *incy;
/* L80: */
	    }
	}
    }

    return 0;

/*     End of ZHER2 . */

} /* zher2_ */

