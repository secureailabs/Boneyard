/* dgbequ.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int dgbequ_(integer *m, integer *n, integer *kl, integer *ku, 
	 doublereal *ab, integer *ldab, doublereal *r__, doublereal *c__, 
	doublereal *rowcnd, doublereal *colcnd, doublereal *amax, integer *
	info)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, i__1, i__2, i__3, i__4;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    integer i__, j, kd;
    doublereal rcmin, rcmax;
    extern doublereal dlamch_(char *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    doublereal bignum, smlnum;


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGBEQU computes row and column scalings intended to equilibrate an */
/*  M-by-N band matrix A and reduce its condition number.  R returns the */
/*  row scale factors and C the column scale factors, chosen to try to */
/*  make the largest element in each row and column of the matrix B with */
/*  elements B(i,j)=R(i)*A(i,j)*C(j) have absolute value 1. */

/*  R(i) and C(j) are restricted to be between SMLNUM = smallest safe */
/*  number and BIGNUM = largest safe number.  Use of these scaling */
/*  factors is not guaranteed to reduce the condition number of A but */
/*  works well in practice. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  KL      (input) INTEGER */
/*          The number of subdiagonals within the band of A.  KL >= 0. */

/*  KU      (input) INTEGER */
/*          The number of superdiagonals within the band of A.  KU >= 0. */

/*  AB      (input) DOUBLE PRECISION array, dimension (LDAB,N) */
/*          The band matrix A, stored in rows 1 to KL+KU+1.  The j-th */
/*          column of A is stored in the j-th column of the array AB as */
/*          follows: */
/*          AB(ku+1+i-j,j) = A(i,j) for max(1,j-ku)<=i<=min(m,j+kl). */

/*  LDAB    (input) INTEGER */
/*          The leading dimension of the array AB.  LDAB >= KL+KU+1. */

/*  R       (output) DOUBLE PRECISION array, dimension (M) */
/*          If INFO = 0, or INFO > M, R contains the row scale factors */
/*          for A. */

/*  C       (output) DOUBLE PRECISION array, dimension (N) */
/*          If INFO = 0, C contains the column scale factors for A. */

/*  ROWCND  (output) DOUBLE PRECISION */
/*          If INFO = 0 or INFO > M, ROWCND contains the ratio of the */
/*          smallest R(i) to the largest R(i).  If ROWCND >= 0.1 and */
/*          AMAX is neither too large nor too small, it is not worth */
/*          scaling by R. */

/*  COLCND  (output) DOUBLE PRECISION */
/*          If INFO = 0, COLCND contains the ratio of the smallest */
/*          C(i) to the largest C(i).  If COLCND >= 0.1, it is not */
/*          worth scaling by C. */

/*  AMAX    (output) DOUBLE PRECISION */
/*          Absolute value of largest matrix element.  If AMAX is very */
/*          close to overflow or very close to underflow, the matrix */
/*          should be scaled. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, and i is */
/*                <= M:  the i-th row of A is exactly zero */
/*                >  M:  the (i-M)-th column of A is exactly zero */

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

/*     Test the input parameters */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --r__;
    --c__;

    /* Function Body */
    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*kl < 0) {
	*info = -3;
    } else if (*ku < 0) {
	*info = -4;
    } else if (*ldab < *kl + *ku + 1) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGBEQU", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*m == 0 || *n == 0) {
	*rowcnd = 1.;
	*colcnd = 1.;
	*amax = 0.;
	return 0;
    }

/*     Get machine constants. */

    smlnum = dlamch_("S");
    bignum = 1. / smlnum;

/*     Compute row scale factors. */

    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	r__[i__] = 0.;
/* L10: */
    }

/*     Find the maximum element in each row. */

    kd = *ku + 1;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	i__2 = j - *ku;
/* Computing MIN */
	i__4 = j + *kl;
	i__3 = min(i__4,*m);
	for (i__ = max(i__2,1); i__ <= i__3; ++i__) {
/* Computing MAX */
	    d__2 = r__[i__], d__3 = (d__1 = ab[kd + i__ - j + j * ab_dim1], 
		    abs(d__1));
	    r__[i__] = max(d__2,d__3);
/* L20: */
	}
/* L30: */
    }

/*     Find the maximum and minimum scale factors. */

    rcmin = bignum;
    rcmax = 0.;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	d__1 = rcmax, d__2 = r__[i__];
	rcmax = max(d__1,d__2);
/* Computing MIN */
	d__1 = rcmin, d__2 = r__[i__];
	rcmin = min(d__1,d__2);
/* L40: */
    }
    *amax = rcmax;

    if (rcmin == 0.) {

/*        Find the first zero scale factor and return an error code. */

	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (r__[i__] == 0.) {
		*info = i__;
		return 0;
	    }
/* L50: */
	}
    } else {

/*        Invert the scale factors. */

	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MIN */
/* Computing MAX */
	    d__2 = r__[i__];
	    d__1 = max(d__2,smlnum);
	    r__[i__] = 1. / min(d__1,bignum);
/* L60: */
	}

/*        Compute ROWCND = min(R(I)) / max(R(I)) */

	*rowcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }

/*     Compute column scale factors */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	c__[j] = 0.;
/* L70: */
    }

/*     Find the maximum element in each column, */
/*     assuming the row scaling computed above. */

    kd = *ku + 1;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	i__3 = j - *ku;
/* Computing MIN */
	i__4 = j + *kl;
	i__2 = min(i__4,*m);
	for (i__ = max(i__3,1); i__ <= i__2; ++i__) {
/* Computing MAX */
	    d__2 = c__[j], d__3 = (d__1 = ab[kd + i__ - j + j * ab_dim1], abs(
		    d__1)) * r__[i__];
	    c__[j] = max(d__2,d__3);
/* L80: */
	}
/* L90: */
    }

/*     Find the maximum and minimum scale factors. */

    rcmin = bignum;
    rcmax = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
	d__1 = rcmin, d__2 = c__[j];
	rcmin = min(d__1,d__2);
/* Computing MAX */
	d__1 = rcmax, d__2 = c__[j];
	rcmax = max(d__1,d__2);
/* L100: */
    }

    if (rcmin == 0.) {

/*        Find the first zero scale factor and return an error code. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    if (c__[j] == 0.) {
		*info = *m + j;
		return 0;
	    }
/* L110: */
	}
    } else {

/*        Invert the scale factors. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
/* Computing MAX */
	    d__2 = c__[j];
	    d__1 = max(d__2,smlnum);
	    c__[j] = 1. / min(d__1,bignum);
/* L120: */
	}

/*        Compute COLCND = min(C(J)) / max(C(J)) */

	*colcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }

    return 0;

/*     End of DGBEQU */

} /* dgbequ_ */
