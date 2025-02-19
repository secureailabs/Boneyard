/* dlaqgb.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int dlaqgb_(integer *m, integer *n, integer *kl, integer *ku, 
	 doublereal *ab, integer *ldab, doublereal *r__, doublereal *c__, 
	doublereal *rowcnd, doublereal *colcnd, doublereal *amax, char *equed)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, i__1, i__2, i__3, i__4, i__5, i__6;

    /* Local variables */
    integer i__, j;
    doublereal cj, large, small;
    extern doublereal dlamch_(char *);


/*  -- LAPACK auxiliary routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DLAQGB equilibrates a general M by N band matrix A with KL */
/*  subdiagonals and KU superdiagonals using the row and scaling factors */
/*  in the vectors R and C. */

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

/*  AB      (input/output) DOUBLE PRECISION array, dimension (LDAB,N) */
/*          On entry, the matrix A in band storage, in rows 1 to KL+KU+1. */
/*          The j-th column of A is stored in the j-th column of the */
/*          array AB as follows: */
/*          AB(ku+1+i-j,j) = A(i,j) for max(1,j-ku)<=i<=min(m,j+kl) */

/*          On exit, the equilibrated matrix, in the same storage format */
/*          as A.  See EQUED for the form of the equilibrated matrix. */

/*  LDAB    (input) INTEGER */
/*          The leading dimension of the array AB.  LDA >= KL+KU+1. */

/*  R       (input) DOUBLE PRECISION array, dimension (M) */
/*          The row scale factors for A. */

/*  C       (input) DOUBLE PRECISION array, dimension (N) */
/*          The column scale factors for A. */

/*  ROWCND  (input) DOUBLE PRECISION */
/*          Ratio of the smallest R(i) to the largest R(i). */

/*  COLCND  (input) DOUBLE PRECISION */
/*          Ratio of the smallest C(i) to the largest C(i). */

/*  AMAX    (input) DOUBLE PRECISION */
/*          Absolute value of largest matrix entry. */

/*  EQUED   (output) CHARACTER*1 */
/*          Specifies the form of equilibration that was done. */
/*          = 'N':  No equilibration */
/*          = 'R':  Row equilibration, i.e., A has been premultiplied by */
/*                  diag(R). */
/*          = 'C':  Column equilibration, i.e., A has been postmultiplied */
/*                  by diag(C). */
/*          = 'B':  Both row and column equilibration, i.e., A has been */
/*                  replaced by diag(R) * A * diag(C). */

/*  Internal Parameters */
/*  =================== */

/*  THRESH is a threshold value used to decide if row or column scaling */
/*  should be done based on the ratio of the row or column scaling */
/*  factors.  If ROWCND < THRESH, row scaling is done, and if */
/*  COLCND < THRESH, column scaling is done. */

/*  LARGE and SMALL are threshold values used to decide if row scaling */
/*  should be done based on the absolute size of the largest matrix */
/*  element.  If AMAX > LARGE or AMAX < SMALL, row scaling is done. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Quick return if possible */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --r__;
    --c__;

    /* Function Body */
    if (*m <= 0 || *n <= 0) {
	*(unsigned char *)equed = 'N';
	return 0;
    }

/*     Initialize LARGE and SMALL. */

    small = dlamch_("Safe minimum") / dlamch_("Precision");
    large = 1. / small;

    if (*rowcnd >= .1 && *amax >= small && *amax <= large) {

/*        No row scaling */

	if (*colcnd >= .1) {

/*           No column scaling */

	    *(unsigned char *)equed = 'N';
	} else {

/*           Column scaling */

	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
		cj = c__[j];
/* Computing MAX */
		i__2 = 1, i__3 = j - *ku;
/* Computing MIN */
		i__5 = *m, i__6 = j + *kl;
		i__4 = min(i__5,i__6);
		for (i__ = max(i__2,i__3); i__ <= i__4; ++i__) {
		    ab[*ku + 1 + i__ - j + j * ab_dim1] = cj * ab[*ku + 1 + 
			    i__ - j + j * ab_dim1];
/* L10: */
		}
/* L20: */
	    }
	    *(unsigned char *)equed = 'C';
	}
    } else if (*colcnd >= .1) {

/*        Row scaling, no column scaling */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	    i__4 = 1, i__2 = j - *ku;
/* Computing MIN */
	    i__5 = *m, i__6 = j + *kl;
	    i__3 = min(i__5,i__6);
	    for (i__ = max(i__4,i__2); i__ <= i__3; ++i__) {
		ab[*ku + 1 + i__ - j + j * ab_dim1] = r__[i__] * ab[*ku + 1 + 
			i__ - j + j * ab_dim1];
/* L30: */
	    }
/* L40: */
	}
	*(unsigned char *)equed = 'R';
    } else {

/*        Row and column scaling */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    cj = c__[j];
/* Computing MAX */
	    i__3 = 1, i__4 = j - *ku;
/* Computing MIN */
	    i__5 = *m, i__6 = j + *kl;
	    i__2 = min(i__5,i__6);
	    for (i__ = max(i__3,i__4); i__ <= i__2; ++i__) {
		ab[*ku + 1 + i__ - j + j * ab_dim1] = cj * r__[i__] * ab[*ku 
			+ 1 + i__ - j + j * ab_dim1];
/* L50: */
	    }
/* L60: */
	}
	*(unsigned char *)equed = 'B';
    }

    return 0;

/*     End of DLAQGB */

} /* dlaqgb_ */
