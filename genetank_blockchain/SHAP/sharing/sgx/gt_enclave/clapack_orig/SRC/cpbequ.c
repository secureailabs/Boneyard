/* cpbequ.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int cpbequ_(char *uplo, integer *n, integer *kd, complex *ab, 
	 integer *ldab, real *s, real *scond, real *amax, integer *info)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, i__1, i__2;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    integer i__, j;
    real smin;
    extern logical lsame_(char *, char *);
    logical upper;
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

/*  CPBEQU computes row and column scalings intended to equilibrate a */
/*  Hermitian positive definite band matrix A and reduce its condition */
/*  number (with respect to the two-norm).  S contains the scale factors, */
/*  S(i) = 1/sqrt(A(i,i)), chosen so that the scaled matrix B with */
/*  elements B(i,j) = S(i)*A(i,j)*S(j) has ones on the diagonal.  This */
/*  choice of S puts the condition number of B within a factor N of the */
/*  smallest possible condition number over all possible diagonal */
/*  scalings. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangular of A is stored; */
/*          = 'L':  Lower triangular of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  KD      (input) INTEGER */
/*          The number of superdiagonals of the matrix A if UPLO = 'U', */
/*          or the number of subdiagonals if UPLO = 'L'.  KD >= 0. */

/*  AB      (input) COMPLEX array, dimension (LDAB,N) */
/*          The upper or lower triangle of the Hermitian band matrix A, */
/*          stored in the first KD+1 rows of the array.  The j-th column */
/*          of A is stored in the j-th column of the array AB as follows: */
/*          if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j; */
/*          if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd). */

/*  LDAB     (input) INTEGER */
/*          The leading dimension of the array A.  LDAB >= KD+1. */

/*  S       (output) REAL array, dimension (N) */
/*          If INFO = 0, S contains the scale factors for A. */

/*  SCOND   (output) REAL */
/*          If INFO = 0, S contains the ratio of the smallest S(i) to */
/*          the largest S(i).  If SCOND >= 0.1 and AMAX is neither too */
/*          large nor too small, it is not worth scaling by S. */

/*  AMAX    (output) REAL */
/*          Absolute value of largest matrix element.  If AMAX is very */
/*          close to overflow or very close to underflow, the matrix */
/*          should be scaled. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = i, the i-th diagonal element is nonpositive. */

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
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --s;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U");
    if (! upper && ! lsame_(uplo, "L")) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*kd < 0) {
	*info = -3;
    } else if (*ldab < *kd + 1) {
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CPBEQU", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	*scond = 1.f;
	*amax = 0.f;
	return 0;
    }

    if (upper) {
	j = *kd + 1;
    } else {
	j = 1;
    }

/*     Initialize SMIN and AMAX. */

    i__1 = j + ab_dim1;
    s[1] = ab[i__1].r;
    smin = s[1];
    *amax = s[1];

/*     Find the minimum and maximum diagonal elements. */

    i__1 = *n;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = j + i__ * ab_dim1;
	s[i__] = ab[i__2].r;
/* Computing MIN */
	r__1 = smin, r__2 = s[i__];
	smin = dmin(r__1,r__2);
/* Computing MAX */
	r__1 = *amax, r__2 = s[i__];
	*amax = dmax(r__1,r__2);
/* L10: */
    }

    if (smin <= 0.f) {

/*        Find the first non-positive diagonal element and return. */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (s[i__] <= 0.f) {
		*info = i__;
		return 0;
	    }
/* L20: */
	}
    } else {

/*        Set the scale factors to the reciprocals */
/*        of the diagonal elements. */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    s[i__] = 1.f / sqrt(s[i__]);
/* L30: */
	}

/*        Compute SCOND = min(S(I)) / max(S(I)) */

	*scond = sqrt(smin) / sqrt(*amax);
    }
    return 0;

/*     End of CPBEQU */

} /* cpbequ_ */
