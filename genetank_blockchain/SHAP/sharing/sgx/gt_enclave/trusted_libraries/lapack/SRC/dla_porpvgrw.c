#include "blas.h"
#include "clapack.h"
/* dla_porpvgrw.f -- translated by f2c (version 20160102).
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

/* > \brief \b DLA_PORPVGRW computes the reciprocal pivot growth factor norm(A)/norm(U) for a symmetric or Her
mitian positive-definite matrix. */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download DLA_PORPVGRW + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/dla_por
pvgrw.f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/dla_por
pvgrw.f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/dla_por
pvgrw.f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       DOUBLE PRECISION FUNCTION DLA_PORPVGRW( UPLO, NCOLS, A, LDA, AF, */
/*                                               LDAF, WORK ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER*1        UPLO */
/*       INTEGER            NCOLS, LDA, LDAF */
/*       .. */
/*       .. Array Arguments .. */
/*       DOUBLE PRECISION   A( LDA, * ), AF( LDAF, * ), WORK( * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > */
/* > DLA_PORPVGRW computes the reciprocal pivot growth factor */
/* > norm(A)/norm(U). The "max absolute element" norm is used. If this is */
/* > much less than 1, the stability of the LU factorization of the */
/* > (equilibrated) matrix A could be poor. This also means that the */
/* > solution X, estimated condition numbers, and error bounds could be */
/* > unreliable. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >       = 'U':  Upper triangle of A is stored; */
/* >       = 'L':  Lower triangle of A is stored. */
/* > \endverbatim */
/* > */
/* > \param[in] NCOLS */
/* > \verbatim */
/* >          NCOLS is INTEGER */
/* >     The number of columns of the matrix A. NCOLS >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* >          A is DOUBLE PRECISION array, dimension (LDA,N) */
/* >     On entry, the N-by-N matrix A. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >     The leading dimension of the array A.  LDA >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] AF */
/* > \verbatim */
/* >          AF is DOUBLE PRECISION array, dimension (LDAF,N) */
/* >     The triangular factor U or L from the Cholesky factorization */
/* >     A = U**T*U or A = L*L**T, as computed by DPOTRF. */
/* > \endverbatim */
/* > */
/* > \param[in] LDAF */
/* > \verbatim */
/* >          LDAF is INTEGER */
/* >     The leading dimension of the array AF.  LDAF >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] WORK */
/* > \verbatim */
/* >          WORK is DOUBLE PRECISION array, dimension (2*N) */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup doublePOcomputational */

/*  ===================================================================== */
doublereal dla_porpvgrw__(char *uplo, integer *ncols, doublereal *a, integer *
	lda, doublereal *af, integer *ldaf, doublereal *work, ftnlen uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, af_dim1, af_offset, i__1, i__2;
    doublereal ret_val, d__1, d__2, d__3;

    /* Local variables */
    static integer i__, j;
    static doublereal amax, umax;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static logical upper;
    static doublereal rpvgrw;


/*  -- LAPACK computational routine (version 3.7.0) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     December 2016 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

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
    upper = lsame_("Upper", uplo, (ftnlen)5, (ftnlen)1);

/*     DPOTRF will have factored only the NCOLSxNCOLS leading minor, so */
/*     we restrict the growth search to that minor and use only the first */
/*     2*NCOLS workspace entries. */

    rpvgrw = 1.;
    i__1 = *ncols << 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	work[i__] = 0.;
    }

/*     Find the max magnitude entry of each column. */

    if (upper) {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
		d__2 = (d__1 = a[i__ + j * a_dim1], abs(d__1)), d__3 = work[*
			ncols + j];
		work[*ncols + j] = max(d__2,d__3);
	    }
	}
    } else {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *ncols;
	    for (i__ = j; i__ <= i__2; ++i__) {
/* Computing MAX */
		d__2 = (d__1 = a[i__ + j * a_dim1], abs(d__1)), d__3 = work[*
			ncols + j];
		work[*ncols + j] = max(d__2,d__3);
	    }
	}
    }

/*     Now find the max magnitude entry of each column of the factor in */
/*     AF.  No pivoting, so no permutations. */

    if (lsame_("Upper", uplo, (ftnlen)5, (ftnlen)1)) {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = j;
	    for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
		d__2 = (d__1 = af[i__ + j * af_dim1], abs(d__1)), d__3 = work[
			j];
		work[j] = max(d__2,d__3);
	    }
	}
    } else {
	i__1 = *ncols;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *ncols;
	    for (i__ = j; i__ <= i__2; ++i__) {
/* Computing MAX */
		d__2 = (d__1 = af[i__ + j * af_dim1], abs(d__1)), d__3 = work[
			j];
		work[j] = max(d__2,d__3);
	    }
	}
    }

/*     Compute the *inverse* of the max element growth factor.  Dividing */
/*     by zero would imply the largest entry of the factor's column is */
/*     zero.  Than can happen when either the column of A is zero or */
/*     massive pivots made the factor underflow to zero.  Neither counts */
/*     as growth in itself, so simply ignore terms with zero */
/*     denominators. */

    if (lsame_("Upper", uplo, (ftnlen)5, (ftnlen)1)) {
	i__1 = *ncols;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    umax = work[i__];
	    amax = work[*ncols + i__];
	    if (umax != 0.) {
/* Computing MIN */
		d__1 = amax / umax;
		rpvgrw = min(d__1,rpvgrw);
	    }
	}
    } else {
	i__1 = *ncols;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    umax = work[i__];
	    amax = work[*ncols + i__];
	    if (umax != 0.) {
/* Computing MIN */
		d__1 = amax / umax;
		rpvgrw = min(d__1,rpvgrw);
	    }
	}
    }
    ret_val = rpvgrw;
    return ret_val;
} /* dla_porpvgrw__ */
