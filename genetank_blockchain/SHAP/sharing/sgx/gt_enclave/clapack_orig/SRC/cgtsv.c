/* cgtsv.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int cgtsv_(integer *n, integer *nrhs, complex *dl, complex *
	d__, complex *du, complex *b, integer *ldb, integer *info)
{
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2, i__3, i__4, i__5, i__6, i__7;
    real r__1, r__2, r__3, r__4;
    complex q__1, q__2, q__3, q__4, q__5;

    /* Builtin functions */
    double r_imag(complex *);
    void c_div(complex *, complex *, complex *);

    /* Local variables */
    integer j, k;
    complex temp, mult;
    extern /* Subroutine */ int xerbla_(char *, integer *);


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CGTSV  solves the equation */

/*     A*X = B, */

/*  where A is an N-by-N tridiagonal matrix, by Gaussian elimination with */
/*  partial pivoting. */

/*  Note that the equation  A'*X = B  may be solved by interchanging the */
/*  order of the arguments DU and DL. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right hand sides, i.e., the number of columns */
/*          of the matrix B.  NRHS >= 0. */

/*  DL      (input/output) COMPLEX array, dimension (N-1) */
/*          On entry, DL must contain the (n-1) subdiagonal elements of */
/*          A. */
/*          On exit, DL is overwritten by the (n-2) elements of the */
/*          second superdiagonal of the upper triangular matrix U from */
/*          the LU factorization of A, in DL(1), ..., DL(n-2). */

/*  D       (input/output) COMPLEX array, dimension (N) */
/*          On entry, D must contain the diagonal elements of A. */
/*          On exit, D is overwritten by the n diagonal elements of U. */

/*  DU      (input/output) COMPLEX array, dimension (N-1) */
/*          On entry, DU must contain the (n-1) superdiagonal elements */
/*          of A. */
/*          On exit, DU is overwritten by the (n-1) elements of the first */
/*          superdiagonal of U. */

/*  B       (input/output) COMPLEX array, dimension (LDB,NRHS) */
/*          On entry, the N-by-NRHS right hand side matrix B. */
/*          On exit, if INFO = 0, the N-by-NRHS solution matrix X. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= max(1,N). */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, U(i,i) is exactly zero, and the solution */
/*                has not been computed.  The factorization has not been */
/*                completed unless i = N. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --dl;
    --d__;
    --du;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;

    /* Function Body */
    *info = 0;
    if (*n < 0) {
	*info = -1;
    } else if (*nrhs < 0) {
	*info = -2;
    } else if (*ldb < max(1,*n)) {
	*info = -7;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CGTSV ", &i__1);
	return 0;
    }

    if (*n == 0) {
	return 0;
    }

    i__1 = *n - 1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	if (dl[i__2].r == 0.f && dl[i__2].i == 0.f) {

/*           Subdiagonal is zero, no elimination is required. */

	    i__2 = k;
	    if (d__[i__2].r == 0.f && d__[i__2].i == 0.f) {

/*              Diagonal is zero: set INFO = K and return; a unique */
/*              solution can not be found. */

		*info = k;
		return 0;
	    }
	} else /* if(complicated condition) */ {
	    i__2 = k;
	    i__3 = k;
	    if ((r__1 = d__[i__2].r, dabs(r__1)) + (r__2 = r_imag(&d__[k]), 
		    dabs(r__2)) >= (r__3 = dl[i__3].r, dabs(r__3)) + (r__4 = 
		    r_imag(&dl[k]), dabs(r__4))) {

/*           No row interchange required */

		c_div(&q__1, &dl[k], &d__[k]);
		mult.r = q__1.r, mult.i = q__1.i;
		i__2 = k + 1;
		i__3 = k + 1;
		i__4 = k;
		q__2.r = mult.r * du[i__4].r - mult.i * du[i__4].i, q__2.i = 
			mult.r * du[i__4].i + mult.i * du[i__4].r;
		q__1.r = d__[i__3].r - q__2.r, q__1.i = d__[i__3].i - q__2.i;
		d__[i__2].r = q__1.r, d__[i__2].i = q__1.i;
		i__2 = *nrhs;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = k + 1 + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    i__5 = k + j * b_dim1;
		    q__2.r = mult.r * b[i__5].r - mult.i * b[i__5].i, q__2.i =
			     mult.r * b[i__5].i + mult.i * b[i__5].r;
		    q__1.r = b[i__4].r - q__2.r, q__1.i = b[i__4].i - q__2.i;
		    b[i__3].r = q__1.r, b[i__3].i = q__1.i;
/* L10: */
		}
		if (k < *n - 1) {
		    i__2 = k;
		    dl[i__2].r = 0.f, dl[i__2].i = 0.f;
		}
	    } else {

/*           Interchange rows K and K+1 */

		c_div(&q__1, &d__[k], &dl[k]);
		mult.r = q__1.r, mult.i = q__1.i;
		i__2 = k;
		i__3 = k;
		d__[i__2].r = dl[i__3].r, d__[i__2].i = dl[i__3].i;
		i__2 = k + 1;
		temp.r = d__[i__2].r, temp.i = d__[i__2].i;
		i__2 = k + 1;
		i__3 = k;
		q__2.r = mult.r * temp.r - mult.i * temp.i, q__2.i = mult.r * 
			temp.i + mult.i * temp.r;
		q__1.r = du[i__3].r - q__2.r, q__1.i = du[i__3].i - q__2.i;
		d__[i__2].r = q__1.r, d__[i__2].i = q__1.i;
		if (k < *n - 1) {
		    i__2 = k;
		    i__3 = k + 1;
		    dl[i__2].r = du[i__3].r, dl[i__2].i = du[i__3].i;
		    i__2 = k + 1;
		    q__2.r = -mult.r, q__2.i = -mult.i;
		    i__3 = k;
		    q__1.r = q__2.r * dl[i__3].r - q__2.i * dl[i__3].i, 
			    q__1.i = q__2.r * dl[i__3].i + q__2.i * dl[i__3]
			    .r;
		    du[i__2].r = q__1.r, du[i__2].i = q__1.i;
		}
		i__2 = k;
		du[i__2].r = temp.r, du[i__2].i = temp.i;
		i__2 = *nrhs;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = k + j * b_dim1;
		    temp.r = b[i__3].r, temp.i = b[i__3].i;
		    i__3 = k + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    b[i__3].r = b[i__4].r, b[i__3].i = b[i__4].i;
		    i__3 = k + 1 + j * b_dim1;
		    i__4 = k + 1 + j * b_dim1;
		    q__2.r = mult.r * b[i__4].r - mult.i * b[i__4].i, q__2.i =
			     mult.r * b[i__4].i + mult.i * b[i__4].r;
		    q__1.r = temp.r - q__2.r, q__1.i = temp.i - q__2.i;
		    b[i__3].r = q__1.r, b[i__3].i = q__1.i;
/* L20: */
		}
	    }
	}
/* L30: */
    }
    i__1 = *n;
    if (d__[i__1].r == 0.f && d__[i__1].i == 0.f) {
	*info = *n;
	return 0;
    }

/*     Back solve with the matrix U from the factorization. */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n + j * b_dim1;
	c_div(&q__1, &b[*n + j * b_dim1], &d__[*n]);
	b[i__2].r = q__1.r, b[i__2].i = q__1.i;
	if (*n > 1) {
	    i__2 = *n - 1 + j * b_dim1;
	    i__3 = *n - 1 + j * b_dim1;
	    i__4 = *n - 1;
	    i__5 = *n + j * b_dim1;
	    q__3.r = du[i__4].r * b[i__5].r - du[i__4].i * b[i__5].i, q__3.i =
		     du[i__4].r * b[i__5].i + du[i__4].i * b[i__5].r;
	    q__2.r = b[i__3].r - q__3.r, q__2.i = b[i__3].i - q__3.i;
	    c_div(&q__1, &q__2, &d__[*n - 1]);
	    b[i__2].r = q__1.r, b[i__2].i = q__1.i;
	}
	for (k = *n - 2; k >= 1; --k) {
	    i__2 = k + j * b_dim1;
	    i__3 = k + j * b_dim1;
	    i__4 = k;
	    i__5 = k + 1 + j * b_dim1;
	    q__4.r = du[i__4].r * b[i__5].r - du[i__4].i * b[i__5].i, q__4.i =
		     du[i__4].r * b[i__5].i + du[i__4].i * b[i__5].r;
	    q__3.r = b[i__3].r - q__4.r, q__3.i = b[i__3].i - q__4.i;
	    i__6 = k;
	    i__7 = k + 2 + j * b_dim1;
	    q__5.r = dl[i__6].r * b[i__7].r - dl[i__6].i * b[i__7].i, q__5.i =
		     dl[i__6].r * b[i__7].i + dl[i__6].i * b[i__7].r;
	    q__2.r = q__3.r - q__5.r, q__2.i = q__3.i - q__5.i;
	    c_div(&q__1, &q__2, &d__[k]);
	    b[i__2].r = q__1.r, b[i__2].i = q__1.i;
/* L40: */
	}
/* L50: */
    }

    return 0;

/*     End of CGTSV */

} /* cgtsv_ */
