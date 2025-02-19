#include "blas.h"
#include "clapack.h"
/* cla_syrcond_x.f -- translated by f2c (version 20160102).
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

/* Table of constant values */

static integer c__1 = 1;

/* > \brief \b CLA_SYRCOND_X computes the infinity norm condition number of op(A)*diag(x) for symmetric indefi
nite matrices. */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download CLA_SYRCOND_X + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/cla_syr
cond_x.f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/cla_syr
cond_x.f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/cla_syr
cond_x.f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       REAL FUNCTION CLA_SYRCOND_X( UPLO, N, A, LDA, AF, LDAF, IPIV, X, */
/*                                    INFO, WORK, RWORK ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          UPLO */
/*       INTEGER            N, LDA, LDAF, INFO */
/*       .. */
/*       .. Array Arguments .. */
/*       INTEGER            IPIV( * ) */
/*       COMPLEX            A( LDA, * ), AF( LDAF, * ), WORK( * ), X( * ) */
/*       REAL               RWORK( * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* >    CLA_SYRCOND_X Computes the infinity norm condition number of */
/* >    op(A) * diag(X) where X is a COMPLEX vector. */
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
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >     The number of linear equations, i.e., the order of the */
/* >     matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* >          A is COMPLEX array, dimension (LDA,N) */
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
/* >          AF is COMPLEX array, dimension (LDAF,N) */
/* >     The block diagonal matrix D and the multipliers used to */
/* >     obtain the factor U or L as computed by CSYTRF. */
/* > \endverbatim */
/* > */
/* > \param[in] LDAF */
/* > \verbatim */
/* >          LDAF is INTEGER */
/* >     The leading dimension of the array AF.  LDAF >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] IPIV */
/* > \verbatim */
/* >          IPIV is INTEGER array, dimension (N) */
/* >     Details of the interchanges and the block structure of D */
/* >     as determined by CSYTRF. */
/* > \endverbatim */
/* > */
/* > \param[in] X */
/* > \verbatim */
/* >          X is COMPLEX array, dimension (N) */
/* >     The vector X in the formula op(A) * diag(X). */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* >          INFO is INTEGER */
/* >       = 0:  Successful exit. */
/* >     i > 0:  The ith argument is invalid. */
/* > \endverbatim */
/* > */
/* > \param[in] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX array, dimension (2*N). */
/* >     Workspace. */
/* > \endverbatim */
/* > */
/* > \param[in] RWORK */
/* > \verbatim */
/* >          RWORK is REAL array, dimension (N). */
/* >     Workspace. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complexSYcomputational */

/*  ===================================================================== */
doublereal cla_syrcond_x__(char *uplo, integer *n, complex *a, integer *lda, 
	complex *af, integer *ldaf, integer *ipiv, complex *x, integer *info, 
	complex *work, real *rwork, ftnlen uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, af_dim1, af_offset, i__1, i__2, i__3, i__4;
    real ret_val, r__1, r__2;
    complex q__1, q__2;

    /* Builtin functions */
    double r_imag(complex *);
    void c_div(complex *, complex *, complex *);

    /* Local variables */
    static integer i__, j;
    static logical up;
    static real tmp;
    static integer kase;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer isave[3];
    static real anorm;
    static logical upper;
    extern /* Subroutine */ int clacn2_(integer *, complex *, complex *, real 
	    *, integer *, integer *), xerbla_(char *, integer *, ftnlen);
    static real ainvnm;
    extern /* Subroutine */ int csytrs_(char *, integer *, integer *, complex 
	    *, integer *, integer *, complex *, integer *, integer *, ftnlen);


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
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function Definitions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    af_dim1 = *ldaf;
    af_offset = 1 + af_dim1;
    af -= af_offset;
    --ipiv;
    --x;
    --work;
    --rwork;

    /* Function Body */
    ret_val = 0.f;

    *info = 0;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    } else if (*ldaf < max(1,*n)) {
	*info = -6;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CLA_SYRCOND_X", &i__1, (ftnlen)13);
	return ret_val;
    }
    up = FALSE_;
    if (lsame_(uplo, "U", (ftnlen)1, (ftnlen)1)) {
	up = TRUE_;
    }

/*     Compute norm of op(A)*op2(C). */

    anorm = 0.f;
    if (up) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    tmp = 0.f;
	    i__2 = i__;
	    for (j = 1; j <= i__2; ++j) {
		i__3 = j + i__ * a_dim1;
		i__4 = j;
		q__2.r = a[i__3].r * x[i__4].r - a[i__3].i * x[i__4].i, 
			q__2.i = a[i__3].r * x[i__4].i + a[i__3].i * x[i__4]
			.r;
		q__1.r = q__2.r, q__1.i = q__2.i;
		tmp += (r__1 = q__1.r, dabs(r__1)) + (r__2 = r_imag(&q__1), 
			dabs(r__2));
	    }
	    i__2 = *n;
	    for (j = i__ + 1; j <= i__2; ++j) {
		i__3 = i__ + j * a_dim1;
		i__4 = j;
		q__2.r = a[i__3].r * x[i__4].r - a[i__3].i * x[i__4].i, 
			q__2.i = a[i__3].r * x[i__4].i + a[i__3].i * x[i__4]
			.r;
		q__1.r = q__2.r, q__1.i = q__2.i;
		tmp += (r__1 = q__1.r, dabs(r__1)) + (r__2 = r_imag(&q__1), 
			dabs(r__2));
	    }
	    rwork[i__] = tmp;
	    anorm = dmax(anorm,tmp);
	}
    } else {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    tmp = 0.f;
	    i__2 = i__;
	    for (j = 1; j <= i__2; ++j) {
		i__3 = i__ + j * a_dim1;
		i__4 = j;
		q__2.r = a[i__3].r * x[i__4].r - a[i__3].i * x[i__4].i, 
			q__2.i = a[i__3].r * x[i__4].i + a[i__3].i * x[i__4]
			.r;
		q__1.r = q__2.r, q__1.i = q__2.i;
		tmp += (r__1 = q__1.r, dabs(r__1)) + (r__2 = r_imag(&q__1), 
			dabs(r__2));
	    }
	    i__2 = *n;
	    for (j = i__ + 1; j <= i__2; ++j) {
		i__3 = j + i__ * a_dim1;
		i__4 = j;
		q__2.r = a[i__3].r * x[i__4].r - a[i__3].i * x[i__4].i, 
			q__2.i = a[i__3].r * x[i__4].i + a[i__3].i * x[i__4]
			.r;
		q__1.r = q__2.r, q__1.i = q__2.i;
		tmp += (r__1 = q__1.r, dabs(r__1)) + (r__2 = r_imag(&q__1), 
			dabs(r__2));
	    }
	    rwork[i__] = tmp;
	    anorm = dmax(anorm,tmp);
	}
    }

/*     Quick return if possible. */

    if (*n == 0) {
	ret_val = 1.f;
	return ret_val;
    } else if (anorm == 0.f) {
	return ret_val;
    }

/*     Estimate the norm of inv(op(A)). */

    ainvnm = 0.f;

    kase = 0;
L10:
    clacn2_(n, &work[*n + 1], &work[1], &ainvnm, &kase, isave);
    if (kase != 0) {
	if (kase == 2) {

/*           Multiply by R. */

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		i__3 = i__;
		i__4 = i__;
		q__1.r = rwork[i__4] * work[i__3].r, q__1.i = rwork[i__4] * 
			work[i__3].i;
		work[i__2].r = q__1.r, work[i__2].i = q__1.i;
	    }

	    if (up) {
		csytrs_("U", n, &c__1, &af[af_offset], ldaf, &ipiv[1], &work[
			1], n, info, (ftnlen)1);
	    } else {
		csytrs_("L", n, &c__1, &af[af_offset], ldaf, &ipiv[1], &work[
			1], n, info, (ftnlen)1);
	    }

/*           Multiply by inv(X). */

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		c_div(&q__1, &work[i__], &x[i__]);
		work[i__2].r = q__1.r, work[i__2].i = q__1.i;
	    }
	} else {

/*           Multiply by inv(X**T). */

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		c_div(&q__1, &work[i__], &x[i__]);
		work[i__2].r = q__1.r, work[i__2].i = q__1.i;
	    }

	    if (up) {
		csytrs_("U", n, &c__1, &af[af_offset], ldaf, &ipiv[1], &work[
			1], n, info, (ftnlen)1);
	    } else {
		csytrs_("L", n, &c__1, &af[af_offset], ldaf, &ipiv[1], &work[
			1], n, info, (ftnlen)1);
	    }

/*           Multiply by R. */

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		i__3 = i__;
		i__4 = i__;
		q__1.r = rwork[i__4] * work[i__3].r, q__1.i = rwork[i__4] * 
			work[i__3].i;
		work[i__2].r = q__1.r, work[i__2].i = q__1.i;
	    }
	}
	goto L10;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.f) {
	ret_val = 1.f / ainvnm;
    }

    return ret_val;

} /* cla_syrcond_x__ */
