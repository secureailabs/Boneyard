/* cung2l.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int cung2l_(integer *m, integer *n, integer *k, complex *a, 
	integer *lda, complex *tau, complex *work, integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    complex q__1;

    /* Local variables */
    integer i__, j, l, ii;
    extern /* Subroutine */ int cscal_(integer *, complex *, complex *, 
	    integer *), clarf_(char *, integer *, integer *, complex *, 
	    integer *, complex *, complex *, integer *, complex *), 
	    xerbla_(char *, integer *);


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CUNG2L generates an m by n complex matrix Q with orthonormal columns, */
/*  which is defined as the last n columns of a product of k elementary */
/*  reflectors of order m */

/*        Q  =  H(k) . . . H(2) H(1) */

/*  as returned by CGEQLF. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix Q. M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix Q. M >= N >= 0. */

/*  K       (input) INTEGER */
/*          The number of elementary reflectors whose product defines the */
/*          matrix Q. N >= K >= 0. */

/*  A       (input/output) COMPLEX array, dimension (LDA,N) */
/*          On entry, the (n-k+i)-th column must contain the vector which */
/*          defines the elementary reflector H(i), for i = 1,2,...,k, as */
/*          returned by CGEQLF in the last k columns of its array */
/*          argument A. */
/*          On exit, the m-by-n matrix Q. */

/*  LDA     (input) INTEGER */
/*          The first dimension of the array A. LDA >= max(1,M). */

/*  TAU     (input) COMPLEX array, dimension (K) */
/*          TAU(i) must contain the scalar factor of the elementary */
/*          reflector H(i), as returned by CGEQLF. */

/*  WORK    (workspace) COMPLEX array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -i, the i-th argument has an illegal value */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --tau;
    --work;

    /* Function Body */
    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0 || *n > *m) {
	*info = -2;
    } else if (*k < 0 || *k > *n) {
	*info = -3;
    } else if (*lda < max(1,*m)) {
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CUNG2L", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n <= 0) {
	return 0;
    }

/*     Initialise columns 1:n-k to columns of the unit matrix */

    i__1 = *n - *k;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (l = 1; l <= i__2; ++l) {
	    i__3 = l + j * a_dim1;
	    a[i__3].r = 0.f, a[i__3].i = 0.f;
/* L10: */
	}
	i__2 = *m - *n + j + j * a_dim1;
	a[i__2].r = 1.f, a[i__2].i = 0.f;
/* L20: */
    }

    i__1 = *k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ii = *n - *k + i__;

/*        Apply H(i) to A(1:m-k+i,1:n-k+i) from the left */

	i__2 = *m - *n + ii + ii * a_dim1;
	a[i__2].r = 1.f, a[i__2].i = 0.f;
	i__2 = *m - *n + ii;
	i__3 = ii - 1;
	clarf_("Left", &i__2, &i__3, &a[ii * a_dim1 + 1], &c__1, &tau[i__], &
		a[a_offset], lda, &work[1]);
	i__2 = *m - *n + ii - 1;
	i__3 = i__;
	q__1.r = -tau[i__3].r, q__1.i = -tau[i__3].i;
	cscal_(&i__2, &q__1, &a[ii * a_dim1 + 1], &c__1);
	i__2 = *m - *n + ii + ii * a_dim1;
	i__3 = i__;
	q__1.r = 1.f - tau[i__3].r, q__1.i = 0.f - tau[i__3].i;
	a[i__2].r = q__1.r, a[i__2].i = q__1.i;

/*        Set A(m-k+i+1:m,n-k+i) to zero */

	i__2 = *m;
	for (l = *m - *n + ii + 1; l <= i__2; ++l) {
	    i__3 = l + ii * a_dim1;
	    a[i__3].r = 0.f, a[i__3].i = 0.f;
/* L30: */
	}
/* L40: */
    }
    return 0;

/*     End of CUNG2L */

} /* cung2l_ */
