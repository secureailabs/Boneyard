/* sla_porpvgrw.f -- translated by f2c (version 20061008).
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

doublereal sla_porpvgrw__(char *uplo, integer *ncols, real *a, integer *lda, 
	real *af, integer *ldaf, real *work, ftnlen uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, af_dim1, af_offset, i__1, i__2;
    real ret_val, r__1, r__2, r__3;

    /* Local variables */
    integer i__, j;
    real amax, umax;
    extern logical lsame_(char *, char *);
    logical upper;
    real rpvgrw;


/*     -- LAPACK routine (version 3.2.1)                                 -- */
/*     -- Contributed by James Demmel, Deaglan Halligan, Yozo Hida and -- */
/*     -- Jason Riedy of Univ. of California Berkeley.                 -- */
/*     -- April 2009                                                   -- */

/*     -- LAPACK is a software package provided by Univ. of Tennessee, -- */
/*     -- Univ. of California Berkeley and NAG Ltd.                    -- */

/*     .. */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLA_PORPVGRW computes the reciprocal pivot growth factor */
/*  norm(A)/norm(U). The "max absolute element" norm is used. If this is */
/*  much less than 1, the stability of the LU factorization of the */
/*  (equilibrated) matrix A could be poor. This also means that the */
/*  solution X, estimated condition numbers, and error bounds could be */
/*  unreliable. */

/*  Arguments */
/*  ========= */

/*     UPLO    (input) CHARACTER*1 */
/*       = 'U':  Upper triangle of A is stored; */
/*       = 'L':  Lower triangle of A is stored. */

/*     NCOLS   (input) INTEGER */
/*     The number of columns of the matrix A. NCOLS >= 0. */

/*     A       (input) REAL array, dimension (LDA,N) */
/*     On entry, the N-by-N matrix A. */

/*     LDA     (input) INTEGER */
/*     The leading dimension of the array A.  LDA >= max(1,N). */

/*     AF      (input) REAL array, dimension (LDAF,N) */
/*     The triangular factor U or L from the Cholesky factorization */
/*     A = U**T*U or A = L*L**T, as computed by SPOTRF. */

/*     LDAF    (input) INTEGER */
/*     The leading dimension of the array AF.  LDAF >= max(1,N). */

/*     WORK    (input) REAL array, dimension (2*N) */

/*  ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    af_dim1 = *ldaf;
    af_offset = 1 + af_dim1;
    af -= af_offset;
    --work;

    /* Function Body */
    upper = lsame_("Upper", uplo);

/*     SPOTRF will have factored only the NCOLSxNCOLS leading minor, so */
/*     we restrict the growth search to that minor and use only the first */
/*     2*NCOLS workspace entries. */

    rpvgrw = 1.f;
    i__1 = *ncols << 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	work[i__] = 0.f;
    }

/*     Find the max magnitude entry of each column. */

    if (upper) {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
		r__2 = (r__1 = a[i__ + j * a_dim1], dabs(r__1)), r__3 = work[*
			ncols + j];
		work[*ncols + j] = dmax(r__2,r__3);
	    }
	}
    } else {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *ncols;
	    for (i__ = j; i__ <= i__2; ++i__) {
/* Computing MAX */
		r__2 = (r__1 = a[i__ + j * a_dim1], dabs(r__1)), r__3 = work[*
			ncols + j];
		work[*ncols + j] = dmax(r__2,r__3);
	    }
	}
    }

/*     Now find the max magnitude entry of each column of the factor in */
/*     AF.  No pivoting, so no permutations. */

    if (lsame_("Upper", uplo)) {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
		r__2 = (r__1 = af[i__ + j * af_dim1], dabs(r__1)), r__3 = 
			work[j];
		work[j] = dmax(r__2,r__3);
	    }
	}
    } else {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *ncols;
	    for (i__ = j; i__ <= i__2; ++i__) {
/* Computing MAX */
		r__2 = (r__1 = af[i__ + j * af_dim1], dabs(r__1)), r__3 = 
			work[j];
		work[j] = dmax(r__2,r__3);
	    }
	}
    }

/*     Compute the *inverse* of the max element growth factor.  Dividing */
/*     by zero would imply the largest entry of the factor's column is */
/*     zero.  Than can happen when either the column of A is zero or */
/*     massive pivots made the factor underflow to zero.  Neither counts */
/*     as growth in itself, so simply ignore terms with zero */
/*     denominators. */

    if (lsame_("Upper", uplo)) {
	i__1 = *ncols;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    umax = work[i__];
	    amax = work[*ncols + i__];
	    if (umax != 0.f) {
/* Computing MIN */
		r__1 = amax / umax;
		rpvgrw = dmin(r__1,rpvgrw);
	    }
	}
    } else {
	i__1 = *ncols;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    umax = work[i__];
	    amax = work[*ncols + i__];
	    if (umax != 0.f) {
/* Computing MIN */
		r__1 = amax / umax;
		rpvgrw = dmin(r__1,rpvgrw);
	    }
	}
    }
    ret_val = rpvgrw;
    return ret_val;
} /* sla_porpvgrw__ */
