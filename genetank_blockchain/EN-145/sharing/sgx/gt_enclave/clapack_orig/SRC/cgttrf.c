/* cgttrf.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int cgttrf_(integer *n, complex *dl, complex *d__, complex *
	du, complex *du2, integer *ipiv, integer *info)
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    real r__1, r__2, r__3, r__4;
    complex q__1, q__2;

    /* Builtin functions */
    double r_imag(complex *);
    void c_div(complex *, complex *, complex *);

    /* Local variables */
    integer i__;
    complex fact, temp;
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

/*  CGTTRF computes an LU factorization of a complex tridiagonal matrix A */
/*  using elimination with partial pivoting and row interchanges. */

/*  The factorization has the form */
/*     A = L * U */
/*  where L is a product of permutation and unit lower bidiagonal */
/*  matrices and U is upper triangular with nonzeros in only the main */
/*  diagonal and first two superdiagonals. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The order of the matrix A. */

/*  DL      (input/output) COMPLEX array, dimension (N-1) */
/*          On entry, DL must contain the (n-1) sub-diagonal elements of */
/*          A. */

/*          On exit, DL is overwritten by the (n-1) multipliers that */
/*          define the matrix L from the LU factorization of A. */

/*  D       (input/output) COMPLEX array, dimension (N) */
/*          On entry, D must contain the diagonal elements of A. */

/*          On exit, D is overwritten by the n diagonal elements of the */
/*          upper triangular matrix U from the LU factorization of A. */

/*  DU      (input/output) COMPLEX array, dimension (N-1) */
/*          On entry, DU must contain the (n-1) super-diagonal elements */
/*          of A. */

/*          On exit, DU is overwritten by the (n-1) elements of the first */
/*          super-diagonal of U. */

/*  DU2     (output) COMPLEX array, dimension (N-2) */
/*          On exit, DU2 is overwritten by the (n-2) elements of the */
/*          second super-diagonal of U. */

/*  IPIV    (output) INTEGER array, dimension (N) */
/*          The pivot indices; for 1 <= i <= n, row i of the matrix was */
/*          interchanged with row IPIV(i).  IPIV(i) will always be either */
/*          i or i+1; IPIV(i) = i indicates a row interchange was not */
/*          required. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -k, the k-th argument had an illegal value */
/*          > 0:  if INFO = k, U(k,k) is exactly zero. The factorization */
/*                has been completed, but the factor U is exactly */
/*                singular, and division by zero will occur if it is used */
/*                to solve a system of equations. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --ipiv;
    --du2;
    --du;
    --d__;
    --dl;

    /* Function Body */
    *info = 0;
    if (*n < 0) {
	*info = -1;
	i__1 = -(*info);
	xerbla_("CGTTRF", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     Initialize IPIV(i) = i and DU2(i) = 0 */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ipiv[i__] = i__;
/* L10: */
    }
    i__1 = *n - 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	du2[i__2].r = 0.f, du2[i__2].i = 0.f;
/* L20: */
    }

    i__1 = *n - 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	i__3 = i__;
	if ((r__1 = d__[i__2].r, dabs(r__1)) + (r__2 = r_imag(&d__[i__]), 
		dabs(r__2)) >= (r__3 = dl[i__3].r, dabs(r__3)) + (r__4 = 
		r_imag(&dl[i__]), dabs(r__4))) {

/*           No row interchange required, eliminate DL(I) */

	    i__2 = i__;
	    if ((r__1 = d__[i__2].r, dabs(r__1)) + (r__2 = r_imag(&d__[i__]), 
		    dabs(r__2)) != 0.f) {
		c_div(&q__1, &dl[i__], &d__[i__]);
		fact.r = q__1.r, fact.i = q__1.i;
		i__2 = i__;
		dl[i__2].r = fact.r, dl[i__2].i = fact.i;
		i__2 = i__ + 1;
		i__3 = i__ + 1;
		i__4 = i__;
		q__2.r = fact.r * du[i__4].r - fact.i * du[i__4].i, q__2.i = 
			fact.r * du[i__4].i + fact.i * du[i__4].r;
		q__1.r = d__[i__3].r - q__2.r, q__1.i = d__[i__3].i - q__2.i;
		d__[i__2].r = q__1.r, d__[i__2].i = q__1.i;
	    }
	} else {

/*           Interchange rows I and I+1, eliminate DL(I) */

	    c_div(&q__1, &d__[i__], &dl[i__]);
	    fact.r = q__1.r, fact.i = q__1.i;
	    i__2 = i__;
	    i__3 = i__;
	    d__[i__2].r = dl[i__3].r, d__[i__2].i = dl[i__3].i;
	    i__2 = i__;
	    dl[i__2].r = fact.r, dl[i__2].i = fact.i;
	    i__2 = i__;
	    temp.r = du[i__2].r, temp.i = du[i__2].i;
	    i__2 = i__;
	    i__3 = i__ + 1;
	    du[i__2].r = d__[i__3].r, du[i__2].i = d__[i__3].i;
	    i__2 = i__ + 1;
	    i__3 = i__ + 1;
	    q__2.r = fact.r * d__[i__3].r - fact.i * d__[i__3].i, q__2.i = 
		    fact.r * d__[i__3].i + fact.i * d__[i__3].r;
	    q__1.r = temp.r - q__2.r, q__1.i = temp.i - q__2.i;
	    d__[i__2].r = q__1.r, d__[i__2].i = q__1.i;
	    i__2 = i__;
	    i__3 = i__ + 1;
	    du2[i__2].r = du[i__3].r, du2[i__2].i = du[i__3].i;
	    i__2 = i__ + 1;
	    q__2.r = -fact.r, q__2.i = -fact.i;
	    i__3 = i__ + 1;
	    q__1.r = q__2.r * du[i__3].r - q__2.i * du[i__3].i, q__1.i = 
		    q__2.r * du[i__3].i + q__2.i * du[i__3].r;
	    du[i__2].r = q__1.r, du[i__2].i = q__1.i;
	    ipiv[i__] = i__ + 1;
	}
/* L30: */
    }
    if (*n > 1) {
	i__ = *n - 1;
	i__1 = i__;
	i__2 = i__;
	if ((r__1 = d__[i__1].r, dabs(r__1)) + (r__2 = r_imag(&d__[i__]), 
		dabs(r__2)) >= (r__3 = dl[i__2].r, dabs(r__3)) + (r__4 = 
		r_imag(&dl[i__]), dabs(r__4))) {
	    i__1 = i__;
	    if ((r__1 = d__[i__1].r, dabs(r__1)) + (r__2 = r_imag(&d__[i__]), 
		    dabs(r__2)) != 0.f) {
		c_div(&q__1, &dl[i__], &d__[i__]);
		fact.r = q__1.r, fact.i = q__1.i;
		i__1 = i__;
		dl[i__1].r = fact.r, dl[i__1].i = fact.i;
		i__1 = i__ + 1;
		i__2 = i__ + 1;
		i__3 = i__;
		q__2.r = fact.r * du[i__3].r - fact.i * du[i__3].i, q__2.i = 
			fact.r * du[i__3].i + fact.i * du[i__3].r;
		q__1.r = d__[i__2].r - q__2.r, q__1.i = d__[i__2].i - q__2.i;
		d__[i__1].r = q__1.r, d__[i__1].i = q__1.i;
	    }
	} else {
	    c_div(&q__1, &d__[i__], &dl[i__]);
	    fact.r = q__1.r, fact.i = q__1.i;
	    i__1 = i__;
	    i__2 = i__;
	    d__[i__1].r = dl[i__2].r, d__[i__1].i = dl[i__2].i;
	    i__1 = i__;
	    dl[i__1].r = fact.r, dl[i__1].i = fact.i;
	    i__1 = i__;
	    temp.r = du[i__1].r, temp.i = du[i__1].i;
	    i__1 = i__;
	    i__2 = i__ + 1;
	    du[i__1].r = d__[i__2].r, du[i__1].i = d__[i__2].i;
	    i__1 = i__ + 1;
	    i__2 = i__ + 1;
	    q__2.r = fact.r * d__[i__2].r - fact.i * d__[i__2].i, q__2.i = 
		    fact.r * d__[i__2].i + fact.i * d__[i__2].r;
	    q__1.r = temp.r - q__2.r, q__1.i = temp.i - q__2.i;
	    d__[i__1].r = q__1.r, d__[i__1].i = q__1.i;
	    ipiv[i__] = i__ + 1;
	}
    }

/*     Check for a zero on the diagonal of U. */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	if ((r__1 = d__[i__2].r, dabs(r__1)) + (r__2 = r_imag(&d__[i__]), 
		dabs(r__2)) == 0.f) {
	    *info = i__;
	    goto L50;
	}
/* L40: */
    }
L50:

    return 0;

/*     End of CGTTRF */

} /* cgttrf_ */
