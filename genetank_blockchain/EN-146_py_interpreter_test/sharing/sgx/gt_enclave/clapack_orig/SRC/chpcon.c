/* chpcon.f -- translated by f2c (version 20061008).
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

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int chpcon_(char *uplo, integer *n, complex *ap, integer *
	ipiv, real *anorm, real *rcond, complex *work, integer *info)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer i__, ip, kase;
    extern logical lsame_(char *, char *);
    integer isave[3];
    logical upper;
    extern /* Subroutine */ int clacn2_(integer *, complex *, complex *, real 
	    *, integer *, integer *), xerbla_(char *, integer *);
    real ainvnm;
    extern /* Subroutine */ int chptrs_(char *, integer *, integer *, complex 
	    *, integer *, complex *, integer *, integer *);


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     Modified to call CLACN2 in place of CLACON, 10 Feb 03, SJH. */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CHPCON estimates the reciprocal of the condition number of a complex */
/*  Hermitian packed matrix A using the factorization A = U*D*U**H or */
/*  A = L*D*L**H computed by CHPTRF. */

/*  An estimate is obtained for norm(inv(A)), and the reciprocal of the */
/*  condition number is computed as RCOND = 1 / (ANORM * norm(inv(A))). */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          Specifies whether the details of the factorization are stored */
/*          as an upper or lower triangular matrix. */
/*          = 'U':  Upper triangular, form is A = U*D*U**H; */
/*          = 'L':  Lower triangular, form is A = L*D*L**H. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  AP      (input) COMPLEX array, dimension (N*(N+1)/2) */
/*          The block diagonal matrix D and the multipliers used to */
/*          obtain the factor U or L as computed by CHPTRF, stored as a */
/*          packed triangular matrix. */

/*  IPIV    (input) INTEGER array, dimension (N) */
/*          Details of the interchanges and the block structure of D */
/*          as determined by CHPTRF. */

/*  ANORM   (input) REAL */
/*          The 1-norm of the original matrix A. */

/*  RCOND   (output) REAL */
/*          The reciprocal of the condition number of the matrix A, */
/*          computed as RCOND = 1/(ANORM * AINVNM), where AINVNM is an */
/*          estimate of the 1-norm of inv(A) computed in this routine. */

/*  WORK    (workspace) COMPLEX array, dimension (2*N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --work;
    --ipiv;
    --ap;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*anorm < 0.f) {
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CHPCON", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.f;
    if (*n == 0) {
	*rcond = 1.f;
	return 0;
    } else if (*anorm <= 0.f) {
	return 0;
    }

/*     Check that the diagonal matrix D is nonsingular. */

    if (upper) {

/*        Upper triangular storage: examine D from bottom to top */

	ip = *n * (*n + 1) / 2;
	for (i__ = *n; i__ >= 1; --i__) {
	    i__1 = ip;
	    if (ipiv[i__] > 0 && (ap[i__1].r == 0.f && ap[i__1].i == 0.f)) {
		return 0;
	    }
	    ip -= i__;
/* L10: */
	}
    } else {

/*        Lower triangular storage: examine D from top to bottom. */

	ip = 1;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = ip;
	    if (ipiv[i__] > 0 && (ap[i__2].r == 0.f && ap[i__2].i == 0.f)) {
		return 0;
	    }
	    ip = ip + *n - i__ + 1;
/* L20: */
	}
    }

/*     Estimate the 1-norm of the inverse. */

    kase = 0;
L30:
    clacn2_(n, &work[*n + 1], &work[1], &ainvnm, &kase, isave);
    if (kase != 0) {

/*        Multiply by inv(L*D*L') or inv(U*D*U'). */

	chptrs_(uplo, n, &c__1, &ap[1], &ipiv[1], &work[1], n, info);
	goto L30;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.f) {
	*rcond = 1.f / ainvnm / *anorm;
    }

    return 0;

/*     End of CHPCON */

} /* chpcon_ */
