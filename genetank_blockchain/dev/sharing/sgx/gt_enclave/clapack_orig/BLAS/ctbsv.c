/* ctbsv.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
$(CLPACKDIR)/BLAS/		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

$(CLPACKDIR)/BLAS/		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"
#include "blas.h"

/* Subroutine */ int ctbsv_(char *uplo, char *trans, char *diag, integer *n, 
	integer *k, complex *a, integer *lda, complex *x, integer *incx)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4, i__5;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);

    /* Local variables */
    integer i__, j, l, ix, jx, kx, info;
    complex temp;
    extern logical lsame_(char *, char *);
    integer kplus1;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    logical noconj, nounit;

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CTBSV  solves one of the systems of equations */

/*     A*x = b,   or   A'*x = b,   or   conjg( A' )*x = b, */

/*  where b and x are n element vectors and A is an n by n unit, or */
/*  non-unit, upper or lower triangular band matrix, with ( k + 1 ) */
/*  diagonals. */

/*  No test for singularity or near-singularity is included in this */
/*  routine. Such tests must be performed before calling this routine. */

/*  Arguments */
/*  ========== */

/*  UPLO   - CHARACTER*1. */
/*           On entry, UPLO specifies whether the matrix is an upper or */
/*           lower triangular matrix as follows: */

/*              UPLO = 'U' or 'u'   A is an upper triangular matrix. */

/*              UPLO = 'L' or 'l'   A is a lower triangular matrix. */

/*           Unchanged on exit. */

/*  TRANS  - CHARACTER*1. */
/*           On entry, TRANS specifies the equations to be solved as */
/*           follows: */

/*              TRANS = 'N' or 'n'   A*x = b. */

/*              TRANS = 'T' or 't'   A'*x = b. */

/*              TRANS = 'C' or 'c'   conjg( A' )*x = b. */

/*           Unchanged on exit. */

/*  DIAG   - CHARACTER*1. */
/*           On entry, DIAG specifies whether or not A is unit */
/*           triangular as follows: */

/*              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */

/*              DIAG = 'N' or 'n'   A is not assumed to be unit */
/*                                  triangular. */

/*           Unchanged on exit. */

/*  N      - INTEGER. */
/*           On entry, N specifies the order of the matrix A. */
/*           N must be at least zero. */
/*           Unchanged on exit. */

/*  K      - INTEGER. */
/*           On entry with UPLO = 'U' or 'u', K specifies the number of */
/*           super-diagonals of the matrix A. */
/*           On entry with UPLO = 'L' or 'l', K specifies the number of */
/*           sub-diagonals of the matrix A. */
/*           K must satisfy  0 .le. K. */
/*           Unchanged on exit. */

/*  A      - COMPLEX          array of DIMENSION ( LDA, n ). */
/*           Before entry with UPLO = 'U' or 'u', the leading ( k + 1 ) */
/*           by n part of the array A must contain the upper triangular */
/*           band part of the matrix of coefficients, supplied column by */
/*           column, with the leading diagonal of the matrix in row */
/*           ( k + 1 ) of the array, the first super-diagonal starting at */
/*           position 2 in row k, and so on. The top left k by k triangle */
/*           of the array A is not referenced. */
/*           The following program segment will transfer an upper */
/*           triangular band matrix from conventional full matrix storage */
/*           to band storage: */

/*                 DO 20, J = 1, N */
/*                    M = K + 1 - J */
/*                    DO 10, I = MAX( 1, J - K ), J */
/*                       A( M + I, J ) = matrix( I, J ) */
/*              10    CONTINUE */
/*              20 CONTINUE */

/*           Before entry with UPLO = 'L' or 'l', the leading ( k + 1 ) */
/*           by n part of the array A must contain the lower triangular */
/*           band part of the matrix of coefficients, supplied column by */
/*           column, with the leading diagonal of the matrix in row 1 of */
/*           the array, the first sub-diagonal starting at position 1 in */
/*           row 2, and so on. The bottom right k by k triangle of the */
/*           array A is not referenced. */
/*           The following program segment will transfer a lower */
/*           triangular band matrix from conventional full matrix storage */
/*           to band storage: */

/*                 DO 20, J = 1, N */
/*                    M = 1 - J */
/*                    DO 10, I = J, MIN( N, J + K ) */
/*                       A( M + I, J ) = matrix( I, J ) */
/*              10    CONTINUE */
/*              20 CONTINUE */

/*           Note that when DIAG = 'U' or 'u' the elements of the array A */
/*           corresponding to the diagonal elements of the matrix are not */
/*           referenced, but are assumed to be unity. */
/*           Unchanged on exit. */

/*  LDA    - INTEGER. */
/*           On entry, LDA specifies the first dimension of A as declared */
/*           in the calling (sub) program. LDA must be at least */
/*           ( k + 1 ). */
/*           Unchanged on exit. */

/*  X      - COMPLEX          array of dimension at least */
/*           ( 1 + ( n - 1 )*abs( INCX ) ). */
/*           Before entry, the incremented array X must contain the n */
/*           element right-hand side vector b. On exit, X is overwritten */
/*           with the solution vector x. */

/*  INCX   - INTEGER. */
/*           On entry, INCX specifies the increment for the elements of */
/*           X. INCX must not be zero. */
/*           Unchanged on exit. */


/*  Level 2 Blas routine. */

/*  -- Written on 22-October-1986. */
/*     Jack Dongarra, Argonne National Lab. */
/*     Jeremy Du Croz, Nag Central Office. */
/*     Sven Hammarling, Nag Central Office. */
/*     Richard Hanson, Sandia National Labs. */


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
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --x;

    /* Function Body */
    info = 0;
    if (! lsame_(uplo, "U") && ! lsame_(uplo, "L")) {
	info = 1;
    } else if (! lsame_(trans, "N") && ! lsame_(trans, 
	    "T") && ! lsame_(trans, "C")) {
	info = 2;
    } else if (! lsame_(diag, "U") && ! lsame_(diag, 
	    "N")) {
	info = 3;
    } else if (*n < 0) {
	info = 4;
    } else if (*k < 0) {
	info = 5;
    } else if (*lda < *k + 1) {
	info = 7;
    } else if (*incx == 0) {
	info = 9;
    }
    if (info != 0) {
	xerbla_("CTBSV ", &info);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

    noconj = lsame_(trans, "T");
    nounit = lsame_(diag, "N");

/*     Set up the start point in X if the increment is not unity. This */
/*     will be  ( N - 1 )*INCX  too small for descending loops. */

    if (*incx <= 0) {
	kx = 1 - (*n - 1) * *incx;
    } else if (*incx != 1) {
	kx = 1;
    }

/*     Start the operations. In this version the elements of A are */
/*     accessed by sequentially with one pass through A. */

    if (lsame_(trans, "N")) {

/*        Form  x := inv( A )*x. */

	if (lsame_(uplo, "U")) {
	    kplus1 = *k + 1;
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    i__1 = j;
		    if (x[i__1].r != 0.f || x[i__1].i != 0.f) {
			l = kplus1 - j;
			if (nounit) {
			    i__1 = j;
			    c_div(&q__1, &x[j], &a[kplus1 + j * a_dim1]);
			    x[i__1].r = q__1.r, x[i__1].i = q__1.i;
			}
			i__1 = j;
			temp.r = x[i__1].r, temp.i = x[i__1].i;
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__1 = max(i__2,i__3);
			for (i__ = j - 1; i__ >= i__1; --i__) {
			    i__2 = i__;
			    i__3 = i__;
			    i__4 = l + i__ + j * a_dim1;
			    q__2.r = temp.r * a[i__4].r - temp.i * a[i__4].i, 
				    q__2.i = temp.r * a[i__4].i + temp.i * a[
				    i__4].r;
			    q__1.r = x[i__3].r - q__2.r, q__1.i = x[i__3].i - 
				    q__2.i;
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
/* L10: */
			}
		    }
/* L20: */
		}
	    } else {
		kx += (*n - 1) * *incx;
		jx = kx;
		for (j = *n; j >= 1; --j) {
		    kx -= *incx;
		    i__1 = jx;
		    if (x[i__1].r != 0.f || x[i__1].i != 0.f) {
			ix = kx;
			l = kplus1 - j;
			if (nounit) {
			    i__1 = jx;
			    c_div(&q__1, &x[jx], &a[kplus1 + j * a_dim1]);
			    x[i__1].r = q__1.r, x[i__1].i = q__1.i;
			}
			i__1 = jx;
			temp.r = x[i__1].r, temp.i = x[i__1].i;
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__1 = max(i__2,i__3);
			for (i__ = j - 1; i__ >= i__1; --i__) {
			    i__2 = ix;
			    i__3 = ix;
			    i__4 = l + i__ + j * a_dim1;
			    q__2.r = temp.r * a[i__4].r - temp.i * a[i__4].i, 
				    q__2.i = temp.r * a[i__4].i + temp.i * a[
				    i__4].r;
			    q__1.r = x[i__3].r - q__2.r, q__1.i = x[i__3].i - 
				    q__2.i;
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
			    ix -= *incx;
/* L30: */
			}
		    }
		    jx -= *incx;
/* L40: */
		}
	    }
	} else {
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    if (x[i__2].r != 0.f || x[i__2].i != 0.f) {
			l = 1 - j;
			if (nounit) {
			    i__2 = j;
			    c_div(&q__1, &x[j], &a[j * a_dim1 + 1]);
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
			}
			i__2 = j;
			temp.r = x[i__2].r, temp.i = x[i__2].i;
/* Computing MIN */
			i__3 = *n, i__4 = j + *k;
			i__2 = min(i__3,i__4);
			for (i__ = j + 1; i__ <= i__2; ++i__) {
			    i__3 = i__;
			    i__4 = i__;
			    i__5 = l + i__ + j * a_dim1;
			    q__2.r = temp.r * a[i__5].r - temp.i * a[i__5].i, 
				    q__2.i = temp.r * a[i__5].i + temp.i * a[
				    i__5].r;
			    q__1.r = x[i__4].r - q__2.r, q__1.i = x[i__4].i - 
				    q__2.i;
			    x[i__3].r = q__1.r, x[i__3].i = q__1.i;
/* L50: */
			}
		    }
/* L60: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    kx += *incx;
		    i__2 = jx;
		    if (x[i__2].r != 0.f || x[i__2].i != 0.f) {
			ix = kx;
			l = 1 - j;
			if (nounit) {
			    i__2 = jx;
			    c_div(&q__1, &x[jx], &a[j * a_dim1 + 1]);
			    x[i__2].r = q__1.r, x[i__2].i = q__1.i;
			}
			i__2 = jx;
			temp.r = x[i__2].r, temp.i = x[i__2].i;
/* Computing MIN */
			i__3 = *n, i__4 = j + *k;
			i__2 = min(i__3,i__4);
			for (i__ = j + 1; i__ <= i__2; ++i__) {
			    i__3 = ix;
			    i__4 = ix;
			    i__5 = l + i__ + j * a_dim1;
			    q__2.r = temp.r * a[i__5].r - temp.i * a[i__5].i, 
				    q__2.i = temp.r * a[i__5].i + temp.i * a[
				    i__5].r;
			    q__1.r = x[i__4].r - q__2.r, q__1.i = x[i__4].i - 
				    q__2.i;
			    x[i__3].r = q__1.r, x[i__3].i = q__1.i;
			    ix += *incx;
/* L70: */
			}
		    }
		    jx += *incx;
/* L80: */
		}
	    }
	}
    } else {

/*        Form  x := inv( A' )*x  or  x := inv( conjg( A') )*x. */

	if (lsame_(uplo, "U")) {
	    kplus1 = *k + 1;
	    if (*incx == 1) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = j;
		    temp.r = x[i__2].r, temp.i = x[i__2].i;
		    l = kplus1 - j;
		    if (noconj) {
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__4 = j - 1;
			for (i__ = max(i__2,i__3); i__ <= i__4; ++i__) {
			    i__2 = l + i__ + j * a_dim1;
			    i__3 = i__;
			    q__2.r = a[i__2].r * x[i__3].r - a[i__2].i * x[
				    i__3].i, q__2.i = a[i__2].r * x[i__3].i + 
				    a[i__2].i * x[i__3].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
/* L90: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &a[kplus1 + j * a_dim1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
/* Computing MAX */
			i__4 = 1, i__2 = j - *k;
			i__3 = j - 1;
			for (i__ = max(i__4,i__2); i__ <= i__3; ++i__) {
			    r_cnjg(&q__3, &a[l + i__ + j * a_dim1]);
			    i__4 = i__;
			    q__2.r = q__3.r * x[i__4].r - q__3.i * x[i__4].i, 
				    q__2.i = q__3.r * x[i__4].i + q__3.i * x[
				    i__4].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
/* L100: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &a[kplus1 + j * a_dim1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__3 = j;
		    x[i__3].r = temp.r, x[i__3].i = temp.i;
/* L110: */
		}
	    } else {
		jx = kx;
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__3 = jx;
		    temp.r = x[i__3].r, temp.i = x[i__3].i;
		    ix = kx;
		    l = kplus1 - j;
		    if (noconj) {
/* Computing MAX */
			i__3 = 1, i__4 = j - *k;
			i__2 = j - 1;
			for (i__ = max(i__3,i__4); i__ <= i__2; ++i__) {
			    i__3 = l + i__ + j * a_dim1;
			    i__4 = ix;
			    q__2.r = a[i__3].r * x[i__4].r - a[i__3].i * x[
				    i__4].i, q__2.i = a[i__3].r * x[i__4].i + 
				    a[i__3].i * x[i__4].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix += *incx;
/* L120: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &a[kplus1 + j * a_dim1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
/* Computing MAX */
			i__2 = 1, i__3 = j - *k;
			i__4 = j - 1;
			for (i__ = max(i__2,i__3); i__ <= i__4; ++i__) {
			    r_cnjg(&q__3, &a[l + i__ + j * a_dim1]);
			    i__2 = ix;
			    q__2.r = q__3.r * x[i__2].r - q__3.i * x[i__2].i, 
				    q__2.i = q__3.r * x[i__2].i + q__3.i * x[
				    i__2].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix += *incx;
/* L130: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &a[kplus1 + j * a_dim1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__4 = jx;
		    x[i__4].r = temp.r, x[i__4].i = temp.i;
		    jx += *incx;
		    if (j > *k) {
			kx += *incx;
		    }
/* L140: */
		}
	    }
	} else {
	    if (*incx == 1) {
		for (j = *n; j >= 1; --j) {
		    i__1 = j;
		    temp.r = x[i__1].r, temp.i = x[i__1].i;
		    l = 1 - j;
		    if (noconj) {
/* Computing MIN */
			i__1 = *n, i__4 = j + *k;
			i__2 = j + 1;
			for (i__ = min(i__1,i__4); i__ >= i__2; --i__) {
			    i__1 = l + i__ + j * a_dim1;
			    i__4 = i__;
			    q__2.r = a[i__1].r * x[i__4].r - a[i__1].i * x[
				    i__4].i, q__2.i = a[i__1].r * x[i__4].i + 
				    a[i__1].i * x[i__4].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
/* L150: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &a[j * a_dim1 + 1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
/* Computing MIN */
			i__2 = *n, i__1 = j + *k;
			i__4 = j + 1;
			for (i__ = min(i__2,i__1); i__ >= i__4; --i__) {
			    r_cnjg(&q__3, &a[l + i__ + j * a_dim1]);
			    i__2 = i__;
			    q__2.r = q__3.r * x[i__2].r - q__3.i * x[i__2].i, 
				    q__2.i = q__3.r * x[i__2].i + q__3.i * x[
				    i__2].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
/* L160: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &a[j * a_dim1 + 1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__4 = j;
		    x[i__4].r = temp.r, x[i__4].i = temp.i;
/* L170: */
		}
	    } else {
		kx += (*n - 1) * *incx;
		jx = kx;
		for (j = *n; j >= 1; --j) {
		    i__4 = jx;
		    temp.r = x[i__4].r, temp.i = x[i__4].i;
		    ix = kx;
		    l = 1 - j;
		    if (noconj) {
/* Computing MIN */
			i__4 = *n, i__2 = j + *k;
			i__1 = j + 1;
			for (i__ = min(i__4,i__2); i__ >= i__1; --i__) {
			    i__4 = l + i__ + j * a_dim1;
			    i__2 = ix;
			    q__2.r = a[i__4].r * x[i__2].r - a[i__4].i * x[
				    i__2].i, q__2.i = a[i__4].r * x[i__2].i + 
				    a[i__4].i * x[i__2].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix -= *incx;
/* L180: */
			}
			if (nounit) {
			    c_div(&q__1, &temp, &a[j * a_dim1 + 1]);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    } else {
/* Computing MIN */
			i__1 = *n, i__4 = j + *k;
			i__2 = j + 1;
			for (i__ = min(i__1,i__4); i__ >= i__2; --i__) {
			    r_cnjg(&q__3, &a[l + i__ + j * a_dim1]);
			    i__1 = ix;
			    q__2.r = q__3.r * x[i__1].r - q__3.i * x[i__1].i, 
				    q__2.i = q__3.r * x[i__1].i + q__3.i * x[
				    i__1].r;
			    q__1.r = temp.r - q__2.r, q__1.i = temp.i - 
				    q__2.i;
			    temp.r = q__1.r, temp.i = q__1.i;
			    ix -= *incx;
/* L190: */
			}
			if (nounit) {
			    r_cnjg(&q__2, &a[j * a_dim1 + 1]);
			    c_div(&q__1, &temp, &q__2);
			    temp.r = q__1.r, temp.i = q__1.i;
			}
		    }
		    i__2 = jx;
		    x[i__2].r = temp.r, x[i__2].i = temp.i;
		    jx -= *incx;
		    if (*n - j >= *k) {
			kx -= *incx;
		    }
/* L200: */
		}
	    }
	}
    }

    return 0;

/*     End of CTBSV . */

} /* ctbsv_ */
