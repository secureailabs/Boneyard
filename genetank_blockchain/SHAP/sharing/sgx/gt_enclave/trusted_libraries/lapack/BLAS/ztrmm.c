#include "blas.h"
/* ztrmm.f -- translated by f2c (version 20160102).
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

/* > \brief \b ZTRMM */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE ZTRMM(SIDE,UPLO,TRANSA,DIAG,M,N,ALPHA,A,LDA,B,LDB) */

/*       .. Scalar Arguments .. */
/*       COMPLEX*16 ALPHA */
/*       INTEGER LDA,LDB,M,N */
/*       CHARACTER DIAG,SIDE,TRANSA,UPLO */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX*16 A(LDA,*),B(LDB,*) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZTRMM  performs one of the matrix-matrix operations */
/* > */
/* >    B := alpha*op( A )*B,   or   B := alpha*B*op( A ) */
/* > */
/* > where  alpha  is a scalar,  B  is an m by n matrix,  A  is a unit, or */
/* > non-unit,  upper or lower triangular matrix  and  op( A )  is one  of */
/* > */
/* >    op( A ) = A   or   op( A ) = A**T   or   op( A ) = A**H. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] SIDE */
/* > \verbatim */
/* >          SIDE is CHARACTER*1 */
/* >           On entry,  SIDE specifies whether  op( A ) multiplies B from */
/* >           the left or right as follows: */
/* > */
/* >              SIDE = 'L' or 'l'   B := alpha*op( A )*B. */
/* > */
/* >              SIDE = 'R' or 'r'   B := alpha*B*op( A ). */
/* > \endverbatim */
/* > */
/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >           On entry, UPLO specifies whether the matrix A is an upper or */
/* >           lower triangular matrix as follows: */
/* > */
/* >              UPLO = 'U' or 'u'   A is an upper triangular matrix. */
/* > */
/* >              UPLO = 'L' or 'l'   A is a lower triangular matrix. */
/* > \endverbatim */
/* > */
/* > \param[in] TRANSA */
/* > \verbatim */
/* >          TRANSA is CHARACTER*1 */
/* >           On entry, TRANSA specifies the form of op( A ) to be used in */
/* >           the matrix multiplication as follows: */
/* > */
/* >              TRANSA = 'N' or 'n'   op( A ) = A. */
/* > */
/* >              TRANSA = 'T' or 't'   op( A ) = A**T. */
/* > */
/* >              TRANSA = 'C' or 'c'   op( A ) = A**H. */
/* > \endverbatim */
/* > */
/* > \param[in] DIAG */
/* > \verbatim */
/* >          DIAG is CHARACTER*1 */
/* >           On entry, DIAG specifies whether or not A is unit triangular */
/* >           as follows: */
/* > */
/* >              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */
/* > */
/* >              DIAG = 'N' or 'n'   A is not assumed to be unit */
/* >                                  triangular. */
/* > \endverbatim */
/* > */
/* > \param[in] M */
/* > \verbatim */
/* >          M is INTEGER */
/* >           On entry, M specifies the number of rows of B. M must be at */
/* >           least zero. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >           On entry, N specifies the number of columns of B.  N must be */
/* >           at least zero. */
/* > \endverbatim */
/* > */
/* > \param[in] ALPHA */
/* > \verbatim */
/* >          ALPHA is COMPLEX*16 */
/* >           On entry,  ALPHA specifies the scalar  alpha. When  alpha is */
/* >           zero then  A is not referenced and  B need not be set before */
/* >           entry. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* >          A is COMPLEX*16 array, dimension ( LDA, k ), where k is m */
/* >           when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'. */
/* >           Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k */
/* >           upper triangular part of the array  A must contain the upper */
/* >           triangular matrix  and the strictly lower triangular part of */
/* >           A is not referenced. */
/* >           Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k */
/* >           lower triangular part of the array  A must contain the lower */
/* >           triangular matrix  and the strictly upper triangular part of */
/* >           A is not referenced. */
/* >           Note that when  DIAG = 'U' or 'u',  the diagonal elements of */
/* >           A  are not referenced either,  but are assumed to be  unity. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >           On entry, LDA specifies the first dimension of A as declared */
/* >           in the calling (sub) program.  When  SIDE = 'L' or 'l'  then */
/* >           LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r' */
/* >           then LDA must be at least max( 1, n ). */
/* > \endverbatim */
/* > */
/* > \param[in,out] B */
/* > \verbatim */
/* >          B is COMPLEX*16 array, dimension ( LDB, N ). */
/* >           Before entry,  the leading  m by n part of the array  B must */
/* >           contain the matrix  B,  and  on exit  is overwritten  by the */
/* >           transformed matrix. */
/* > \endverbatim */
/* > */
/* > \param[in] LDB */
/* > \verbatim */
/* >          LDB is INTEGER */
/* >           On entry, LDB specifies the first dimension of B as declared */
/* >           in  the  calling  (sub)  program.   LDB  must  be  at  least */
/* >           max( 1, m ). */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complex16_blas_level3 */

/* > \par Further Details: */
/*  ===================== */
/* > */
/* > \verbatim */
/* > */
/* >  Level 3 Blas routine. */
/* > */
/* >  -- Written on 8-February-1989. */
/* >     Jack Dongarra, Argonne National Laboratory. */
/* >     Iain Duff, AERE Harwell. */
/* >     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
/* >     Sven Hammarling, Numerical Algorithms Group Ltd. */
/* > \endverbatim */
/* > */
/*  ===================================================================== */
/* Subroutine */ int ztrmm_(char *side, char *uplo, char *transa, char *diag, 
	integer *m, integer *n, doublecomplex *alpha, doublecomplex *a, 
	integer *lda, doublecomplex *b, integer *ldb, ftnlen side_len, ftnlen 
	uplo_len, ftnlen transa_len, ftnlen diag_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3, i__4, i__5, 
	    i__6;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer i__, j, k, info;
    static doublecomplex temp;
    static logical lside;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer nrowa;
    static logical upper;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static logical noconj, nounit;


/*  -- Reference BLAS level3 routine (version 3.7.0) -- */
/*  -- Reference BLAS is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     December 2016 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  ===================================================================== */

/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Parameters .. */
/*     .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    lside = lsame_(side, "L", (ftnlen)1, (ftnlen)1);
    if (lside) {
	nrowa = *m;
    } else {
	nrowa = *n;
    }
    noconj = lsame_(transa, "T", (ftnlen)1, (ftnlen)1);
    nounit = lsame_(diag, "N", (ftnlen)1, (ftnlen)1);
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);

    info = 0;
    if (! lside && ! lsame_(side, "R", (ftnlen)1, (ftnlen)1)) {
	info = 1;
    } else if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	info = 2;
    } else if (! lsame_(transa, "N", (ftnlen)1, (ftnlen)1) && ! lsame_(transa,
	     "T", (ftnlen)1, (ftnlen)1) && ! lsame_(transa, "C", (ftnlen)1, (
	    ftnlen)1)) {
	info = 3;
    } else if (! lsame_(diag, "U", (ftnlen)1, (ftnlen)1) && ! lsame_(diag, 
	    "N", (ftnlen)1, (ftnlen)1)) {
	info = 4;
    } else if (*m < 0) {
	info = 5;
    } else if (*n < 0) {
	info = 6;
    } else if (*lda < max(1,nrowa)) {
	info = 9;
    } else if (*ldb < max(1,*m)) {
	info = 11;
    }
    if (info != 0) {
	xerbla_("ZTRMM ", &info, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible. */

    if (*m == 0 || *n == 0) {
	return 0;
    }

/*     And when  alpha.eq.zero. */

    if (alpha->r == 0. && alpha->i == 0.) {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		i__3 = i__ + j * b_dim1;
		b[i__3].r = 0., b[i__3].i = 0.;
/* L10: */
	    }
/* L20: */
	}
	return 0;
    }

/*     Start the operations. */

    if (lside) {
	if (lsame_(transa, "N", (ftnlen)1, (ftnlen)1)) {

/*           Form  B := alpha*A*B. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * b_dim1;
			if (b[i__3].r != 0. || b[i__3].i != 0.) {
			    i__3 = k + j * b_dim1;
			    z__1.r = alpha->r * b[i__3].r - alpha->i * b[i__3]
				    .i, z__1.i = alpha->r * b[i__3].i + 
				    alpha->i * b[i__3].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__3 = k - 1;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + j * b_dim1;
				i__6 = i__ + k * a_dim1;
				z__2.r = temp.r * a[i__6].r - temp.i * a[i__6]
					.i, z__2.i = temp.r * a[i__6].i + 
					temp.i * a[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L30: */
			    }
			    if (nounit) {
				i__3 = k + k * a_dim1;
				z__1.r = temp.r * a[i__3].r - temp.i * a[i__3]
					.i, z__1.i = temp.r * a[i__3].i + 
					temp.i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = k + j * b_dim1;
			    b[i__3].r = temp.r, b[i__3].i = temp.i;
			}
/* L40: */
		    }
/* L50: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (k = *m; k >= 1; --k) {
			i__2 = k + j * b_dim1;
			if (b[i__2].r != 0. || b[i__2].i != 0.) {
			    i__2 = k + j * b_dim1;
			    z__1.r = alpha->r * b[i__2].r - alpha->i * b[i__2]
				    .i, z__1.i = alpha->r * b[i__2].i + 
				    alpha->i * b[i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__2 = k + j * b_dim1;
			    b[i__2].r = temp.r, b[i__2].i = temp.i;
			    if (nounit) {
				i__2 = k + j * b_dim1;
				i__3 = k + j * b_dim1;
				i__4 = k + k * a_dim1;
				z__1.r = b[i__3].r * a[i__4].r - b[i__3].i * 
					a[i__4].i, z__1.i = b[i__3].r * a[
					i__4].i + b[i__3].i * a[i__4].r;
				b[i__2].r = z__1.r, b[i__2].i = z__1.i;
			    }
			    i__2 = *m;
			    for (i__ = k + 1; i__ <= i__2; ++i__) {
				i__3 = i__ + j * b_dim1;
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + k * a_dim1;
				z__2.r = temp.r * a[i__5].r - temp.i * a[i__5]
					.i, z__2.i = temp.r * a[i__5].i + 
					temp.i * a[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L60: */
			    }
			}
/* L70: */
		    }
/* L80: */
		}
	    }
	} else {

/*           Form  B := alpha*A**T*B   or   B := alpha*A**H*B. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (i__ = *m; i__ >= 1; --i__) {
			i__2 = i__ + j * b_dim1;
			temp.r = b[i__2].r, temp.i = b[i__2].i;
			if (noconj) {
			    if (nounit) {
				i__2 = i__ + i__ * a_dim1;
				z__1.r = temp.r * a[i__2].r - temp.i * a[i__2]
					.i, z__1.i = temp.r * a[i__2].i + 
					temp.i * a[i__2].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = i__ - 1;
			    for (k = 1; k <= i__2; ++k) {
				i__3 = k + i__ * a_dim1;
				i__4 = k + j * b_dim1;
				z__2.r = a[i__3].r * b[i__4].r - a[i__3].i * 
					b[i__4].i, z__2.i = a[i__3].r * b[
					i__4].i + a[i__3].i * b[i__4].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L90: */
			    }
			} else {
			    if (nounit) {
				d_cnjg(&z__2, &a[i__ + i__ * a_dim1]);
				z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
					z__1.i = temp.r * z__2.i + temp.i * 
					z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = i__ - 1;
			    for (k = 1; k <= i__2; ++k) {
				d_cnjg(&z__3, &a[k + i__ * a_dim1]);
				i__3 = k + j * b_dim1;
				z__2.r = z__3.r * b[i__3].r - z__3.i * b[i__3]
					.i, z__2.i = z__3.r * b[i__3].i + 
					z__3.i * b[i__3].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L100: */
			    }
			}
			i__2 = i__ + j * b_dim1;
			z__1.r = alpha->r * temp.r - alpha->i * temp.i, 
				z__1.i = alpha->r * temp.i + alpha->i * 
				temp.r;
			b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L110: */
		    }
/* L120: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * b_dim1;
			temp.r = b[i__3].r, temp.i = b[i__3].i;
			if (noconj) {
			    if (nounit) {
				i__3 = i__ + i__ * a_dim1;
				z__1.r = temp.r * a[i__3].r - temp.i * a[i__3]
					.i, z__1.i = temp.r * a[i__3].i + 
					temp.i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (k = i__ + 1; k <= i__3; ++k) {
				i__4 = k + i__ * a_dim1;
				i__5 = k + j * b_dim1;
				z__2.r = a[i__4].r * b[i__5].r - a[i__4].i * 
					b[i__5].i, z__2.i = a[i__4].r * b[
					i__5].i + a[i__4].i * b[i__5].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L130: */
			    }
			} else {
			    if (nounit) {
				d_cnjg(&z__2, &a[i__ + i__ * a_dim1]);
				z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
					z__1.i = temp.r * z__2.i + temp.i * 
					z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (k = i__ + 1; k <= i__3; ++k) {
				d_cnjg(&z__3, &a[k + i__ * a_dim1]);
				i__4 = k + j * b_dim1;
				z__2.r = z__3.r * b[i__4].r - z__3.i * b[i__4]
					.i, z__2.i = z__3.r * b[i__4].i + 
					z__3.i * b[i__4].r;
				z__1.r = temp.r + z__2.r, z__1.i = temp.i + 
					z__2.i;
				temp.r = z__1.r, temp.i = z__1.i;
/* L140: */
			    }
			}
			i__3 = i__ + j * b_dim1;
			z__1.r = alpha->r * temp.r - alpha->i * temp.i, 
				z__1.i = alpha->r * temp.i + alpha->i * 
				temp.r;
			b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L150: */
		    }
/* L160: */
		}
	    }
	}
    } else {
	if (lsame_(transa, "N", (ftnlen)1, (ftnlen)1)) {

/*           Form  B := alpha*B*A. */

	    if (upper) {
		for (j = *n; j >= 1; --j) {
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			i__1 = j + j * a_dim1;
			z__1.r = temp.r * a[i__1].r - temp.i * a[i__1].i, 
				z__1.i = temp.r * a[i__1].i + temp.i * a[i__1]
				.r;
			temp.r = z__1.r, temp.i = z__1.i;
		    }
		    i__1 = *m;
		    for (i__ = 1; i__ <= i__1; ++i__) {
			i__2 = i__ + j * b_dim1;
			i__3 = i__ + j * b_dim1;
			z__1.r = temp.r * b[i__3].r - temp.i * b[i__3].i, 
				z__1.i = temp.r * b[i__3].i + temp.i * b[i__3]
				.r;
			b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L170: */
		    }
		    i__1 = j - 1;
		    for (k = 1; k <= i__1; ++k) {
			i__2 = k + j * a_dim1;
			if (a[i__2].r != 0. || a[i__2].i != 0.) {
			    i__2 = k + j * a_dim1;
			    z__1.r = alpha->r * a[i__2].r - alpha->i * a[i__2]
				    .i, z__1.i = alpha->r * a[i__2].i + 
				    alpha->i * a[i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__2 = *m;
			    for (i__ = 1; i__ <= i__2; ++i__) {
				i__3 = i__ + j * b_dim1;
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + k * b_dim1;
				z__2.r = temp.r * b[i__5].r - temp.i * b[i__5]
					.i, z__2.i = temp.r * b[i__5].i + 
					temp.i * b[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L180: */
			    }
			}
/* L190: */
		    }
/* L200: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			i__2 = j + j * a_dim1;
			z__1.r = temp.r * a[i__2].r - temp.i * a[i__2].i, 
				z__1.i = temp.r * a[i__2].i + temp.i * a[i__2]
				.r;
			temp.r = z__1.r, temp.i = z__1.i;
		    }
		    i__2 = *m;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			i__3 = i__ + j * b_dim1;
			i__4 = i__ + j * b_dim1;
			z__1.r = temp.r * b[i__4].r - temp.i * b[i__4].i, 
				z__1.i = temp.r * b[i__4].i + temp.i * b[i__4]
				.r;
			b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L210: */
		    }
		    i__2 = *n;
		    for (k = j + 1; k <= i__2; ++k) {
			i__3 = k + j * a_dim1;
			if (a[i__3].r != 0. || a[i__3].i != 0.) {
			    i__3 = k + j * a_dim1;
			    z__1.r = alpha->r * a[i__3].r - alpha->i * a[i__3]
				    .i, z__1.i = alpha->r * a[i__3].i + 
				    alpha->i * a[i__3].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			    i__3 = *m;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + j * b_dim1;
				i__6 = i__ + k * b_dim1;
				z__2.r = temp.r * b[i__6].r - temp.i * b[i__6]
					.i, z__2.i = temp.r * b[i__6].i + 
					temp.i * b[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L220: */
			    }
			}
/* L230: */
		    }
/* L240: */
		}
	    }
	} else {

/*           Form  B := alpha*B*A**T   or   B := alpha*B*A**H. */

	    if (upper) {
		i__1 = *n;
		for (k = 1; k <= i__1; ++k) {
		    i__2 = k - 1;
		    for (j = 1; j <= i__2; ++j) {
			i__3 = j + k * a_dim1;
			if (a[i__3].r != 0. || a[i__3].i != 0.) {
			    if (noconj) {
				i__3 = j + k * a_dim1;
				z__1.r = alpha->r * a[i__3].r - alpha->i * a[
					i__3].i, z__1.i = alpha->r * a[i__3]
					.i + alpha->i * a[i__3].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    } else {
				d_cnjg(&z__2, &a[j + k * a_dim1]);
				z__1.r = alpha->r * z__2.r - alpha->i * 
					z__2.i, z__1.i = alpha->r * z__2.i + 
					alpha->i * z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__3 = *m;
			    for (i__ = 1; i__ <= i__3; ++i__) {
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + j * b_dim1;
				i__6 = i__ + k * b_dim1;
				z__2.r = temp.r * b[i__6].r - temp.i * b[i__6]
					.i, z__2.i = temp.r * b[i__6].i + 
					temp.i * b[i__6].r;
				z__1.r = b[i__5].r + z__2.r, z__1.i = b[i__5]
					.i + z__2.i;
				b[i__4].r = z__1.r, b[i__4].i = z__1.i;
/* L250: */
			    }
			}
/* L260: */
		    }
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			if (noconj) {
			    i__2 = k + k * a_dim1;
			    z__1.r = temp.r * a[i__2].r - temp.i * a[i__2].i, 
				    z__1.i = temp.r * a[i__2].i + temp.i * a[
				    i__2].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			} else {
			    d_cnjg(&z__2, &a[k + k * a_dim1]);
			    z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
				    z__1.i = temp.r * z__2.i + temp.i * 
				    z__2.r;
			    temp.r = z__1.r, temp.i = z__1.i;
			}
		    }
		    if (temp.r != 1. || temp.i != 0.) {
			i__2 = *m;
			for (i__ = 1; i__ <= i__2; ++i__) {
			    i__3 = i__ + k * b_dim1;
			    i__4 = i__ + k * b_dim1;
			    z__1.r = temp.r * b[i__4].r - temp.i * b[i__4].i, 
				    z__1.i = temp.r * b[i__4].i + temp.i * b[
				    i__4].r;
			    b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L270: */
			}
		    }
/* L280: */
		}
	    } else {
		for (k = *n; k >= 1; --k) {
		    i__1 = *n;
		    for (j = k + 1; j <= i__1; ++j) {
			i__2 = j + k * a_dim1;
			if (a[i__2].r != 0. || a[i__2].i != 0.) {
			    if (noconj) {
				i__2 = j + k * a_dim1;
				z__1.r = alpha->r * a[i__2].r - alpha->i * a[
					i__2].i, z__1.i = alpha->r * a[i__2]
					.i + alpha->i * a[i__2].r;
				temp.r = z__1.r, temp.i = z__1.i;
			    } else {
				d_cnjg(&z__2, &a[j + k * a_dim1]);
				z__1.r = alpha->r * z__2.r - alpha->i * 
					z__2.i, z__1.i = alpha->r * z__2.i + 
					alpha->i * z__2.r;
				temp.r = z__1.r, temp.i = z__1.i;
			    }
			    i__2 = *m;
			    for (i__ = 1; i__ <= i__2; ++i__) {
				i__3 = i__ + j * b_dim1;
				i__4 = i__ + j * b_dim1;
				i__5 = i__ + k * b_dim1;
				z__2.r = temp.r * b[i__5].r - temp.i * b[i__5]
					.i, z__2.i = temp.r * b[i__5].i + 
					temp.i * b[i__5].r;
				z__1.r = b[i__4].r + z__2.r, z__1.i = b[i__4]
					.i + z__2.i;
				b[i__3].r = z__1.r, b[i__3].i = z__1.i;
/* L290: */
			    }
			}
/* L300: */
		    }
		    temp.r = alpha->r, temp.i = alpha->i;
		    if (nounit) {
			if (noconj) {
			    i__1 = k + k * a_dim1;
			    z__1.r = temp.r * a[i__1].r - temp.i * a[i__1].i, 
				    z__1.i = temp.r * a[i__1].i + temp.i * a[
				    i__1].r;
			    temp.r = z__1.r, temp.i = z__1.i;
			} else {
			    d_cnjg(&z__2, &a[k + k * a_dim1]);
			    z__1.r = temp.r * z__2.r - temp.i * z__2.i, 
				    z__1.i = temp.r * z__2.i + temp.i * 
				    z__2.r;
			    temp.r = z__1.r, temp.i = z__1.i;
			}
		    }
		    if (temp.r != 1. || temp.i != 0.) {
			i__1 = *m;
			for (i__ = 1; i__ <= i__1; ++i__) {
			    i__2 = i__ + k * b_dim1;
			    i__3 = i__ + k * b_dim1;
			    z__1.r = temp.r * b[i__3].r - temp.i * b[i__3].i, 
				    z__1.i = temp.r * b[i__3].i + temp.i * b[
				    i__3].r;
			    b[i__2].r = z__1.r, b[i__2].i = z__1.i;
/* L310: */
			}
		    }
/* L320: */
		}
	    }
	}
    }

    return 0;

/*     End of ZTRMM . */

} /* ztrmm_ */

