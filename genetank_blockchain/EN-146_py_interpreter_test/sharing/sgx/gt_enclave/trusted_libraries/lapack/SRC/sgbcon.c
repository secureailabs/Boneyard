#include "blas.h"
#include "clapack.h"
/* sgbcon.f -- translated by f2c (version 20160102).
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

/* > \brief \b SGBCON */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download SGBCON + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/sgbcon.
f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/sgbcon.
f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/sgbcon.
f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE SGBCON( NORM, N, KL, KU, AB, LDAB, IPIV, ANORM, RCOND, */
/*                          WORK, IWORK, INFO ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          NORM */
/*       INTEGER            INFO, KL, KU, LDAB, N */
/*       REAL               ANORM, RCOND */
/*       .. */
/*       .. Array Arguments .. */
/*       INTEGER            IPIV( * ), IWORK( * ) */
/*       REAL               AB( LDAB, * ), WORK( * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > SGBCON estimates the reciprocal of the condition number of a real */
/* > general band matrix A, in either the 1-norm or the infinity-norm, */
/* > using the LU factorization computed by SGBTRF. */
/* > */
/* > An estimate is obtained for norm(inv(A)), and the reciprocal of the */
/* > condition number is computed as */
/* >    RCOND = 1 / ( norm(A) * norm(inv(A)) ). */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] NORM */
/* > \verbatim */
/* >          NORM is CHARACTER*1 */
/* >          Specifies whether the 1-norm condition number or the */
/* >          infinity-norm condition number is required: */
/* >          = '1' or 'O':  1-norm; */
/* >          = 'I':         Infinity-norm. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >          The order of the matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] KL */
/* > \verbatim */
/* >          KL is INTEGER */
/* >          The number of subdiagonals within the band of A.  KL >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] KU */
/* > \verbatim */
/* >          KU is INTEGER */
/* >          The number of superdiagonals within the band of A.  KU >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] AB */
/* > \verbatim */
/* >          AB is REAL array, dimension (LDAB,N) */
/* >          Details of the LU factorization of the band matrix A, as */
/* >          computed by SGBTRF.  U is stored as an upper triangular band */
/* >          matrix with KL+KU superdiagonals in rows 1 to KL+KU+1, and */
/* >          the multipliers used during the factorization are stored in */
/* >          rows KL+KU+2 to 2*KL+KU+1. */
/* > \endverbatim */
/* > */
/* > \param[in] LDAB */
/* > \verbatim */
/* >          LDAB is INTEGER */
/* >          The leading dimension of the array AB.  LDAB >= 2*KL+KU+1. */
/* > \endverbatim */
/* > */
/* > \param[in] IPIV */
/* > \verbatim */
/* >          IPIV is INTEGER array, dimension (N) */
/* >          The pivot indices; for 1 <= i <= N, row i of the matrix was */
/* >          interchanged with row IPIV(i). */
/* > \endverbatim */
/* > */
/* > \param[in] ANORM */
/* > \verbatim */
/* >          ANORM is REAL */
/* >          If NORM = '1' or 'O', the 1-norm of the original matrix A. */
/* >          If NORM = 'I', the infinity-norm of the original matrix A. */
/* > \endverbatim */
/* > */
/* > \param[out] RCOND */
/* > \verbatim */
/* >          RCOND is REAL */
/* >          The reciprocal of the condition number of the matrix A, */
/* >          computed as RCOND = 1/(norm(A) * norm(inv(A))). */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* >          WORK is REAL array, dimension (3*N) */
/* > \endverbatim */
/* > */
/* > \param[out] IWORK */
/* > \verbatim */
/* >          IWORK is INTEGER array, dimension (N) */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* >          INFO is INTEGER */
/* >          = 0:  successful exit */
/* >          < 0: if INFO = -i, the i-th argument had an illegal value */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup realGBcomputational */

/*  ===================================================================== */
/* Subroutine */ int sgbcon_(char *norm, integer *n, integer *kl, integer *ku,
	 real *ab, integer *ldab, integer *ipiv, real *anorm, real *rcond, 
	real *work, integer *iwork, integer *info, ftnlen norm_len)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, i__1, i__2, i__3;
    real r__1;

    /* Local variables */
    static integer j;
    static real t;
    static integer kd, lm, jp, ix, kase;
    extern doublereal sdot_(integer *, real *, integer *, real *, integer *);
    static integer kase1;
    static real scale;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer isave[3];
    static logical lnoti;
    extern /* Subroutine */ int srscl_(integer *, real *, real *, integer *), 
	    saxpy_(integer *, real *, real *, integer *, real *, integer *), 
	    slacn2_(integer *, real *, real *, integer *, real *, integer *, 
	    integer *);
    extern doublereal slamch_(char *, ftnlen);
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    extern integer isamax_(integer *, real *, integer *);
    static real ainvnm;
    extern /* Subroutine */ int slatbs_(char *, char *, char *, char *, 
	    integer *, integer *, real *, integer *, real *, real *, real *, 
	    integer *, ftnlen, ftnlen, ftnlen, ftnlen);
    static logical onenrm;
    static char normin[1];
    static real smlnum;


/*  -- LAPACK computational routine (version 3.7.0) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     December 2016 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

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
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --ipiv;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    onenrm = *(unsigned char *)norm == '1' || lsame_(norm, "O", (ftnlen)1, (
	    ftnlen)1);
    if (! onenrm && ! lsame_(norm, "I", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*kl < 0) {
	*info = -3;
    } else if (*ku < 0) {
	*info = -4;
    } else if (*ldab < (*kl << 1) + *ku + 1) {
	*info = -6;
    } else if (*anorm < 0.f) {
	*info = -8;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SGBCON", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.f;
    if (*n == 0) {
	*rcond = 1.f;
	return 0;
    } else if (*anorm == 0.f) {
	return 0;
    }

    smlnum = slamch_("Safe minimum", (ftnlen)12);

/*     Estimate the norm of inv(A). */

    ainvnm = 0.f;
    *(unsigned char *)normin = 'N';
    if (onenrm) {
	kase1 = 1;
    } else {
	kase1 = 2;
    }
    kd = *kl + *ku + 1;
    lnoti = *kl > 0;
    kase = 0;
L10:
    slacn2_(n, &work[*n + 1], &work[1], &iwork[1], &ainvnm, &kase, isave);
    if (kase != 0) {
	if (kase == kase1) {

/*           Multiply by inv(L). */

	    if (lnoti) {
		i__1 = *n - 1;
		for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
		    i__2 = *kl, i__3 = *n - j;
		    lm = min(i__2,i__3);
		    jp = ipiv[j];
		    t = work[jp];
		    if (jp != j) {
			work[jp] = work[j];
			work[j] = t;
		    }
		    r__1 = -t;
		    saxpy_(&lm, &r__1, &ab[kd + 1 + j * ab_dim1], &c__1, &
			    work[j + 1], &c__1);
/* L20: */
		}
	    }

/*           Multiply by inv(U). */

	    i__1 = *kl + *ku;
	    slatbs_("Upper", "No transpose", "Non-unit", normin, n, &i__1, &
		    ab[ab_offset], ldab, &work[1], &scale, &work[(*n << 1) + 
		    1], info, (ftnlen)5, (ftnlen)12, (ftnlen)8, (ftnlen)1);
	} else {

/*           Multiply by inv(U**T). */

	    i__1 = *kl + *ku;
	    slatbs_("Upper", "Transpose", "Non-unit", normin, n, &i__1, &ab[
		    ab_offset], ldab, &work[1], &scale, &work[(*n << 1) + 1], 
		    info, (ftnlen)5, (ftnlen)9, (ftnlen)8, (ftnlen)1);

/*           Multiply by inv(L**T). */

	    if (lnoti) {
		for (j = *n - 1; j >= 1; --j) {
/* Computing MIN */
		    i__1 = *kl, i__2 = *n - j;
		    lm = min(i__1,i__2);
		    work[j] -= sdot_(&lm, &ab[kd + 1 + j * ab_dim1], &c__1, &
			    work[j + 1], &c__1);
		    jp = ipiv[j];
		    if (jp != j) {
			t = work[jp];
			work[jp] = work[j];
			work[j] = t;
		    }
/* L30: */
		}
	    }
	}

/*        Divide X by 1/SCALE if doing so will not cause overflow. */

	*(unsigned char *)normin = 'Y';
	if (scale != 1.f) {
	    ix = isamax_(n, &work[1], &c__1);
	    if (scale < (r__1 = work[ix], dabs(r__1)) * smlnum || scale == 
		    0.f) {
		goto L40;
	    }
	    srscl_(n, &scale, &work[1], &c__1);
	}
	goto L10;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.f) {
	*rcond = 1.f / ainvnm / *anorm;
    }

L40:
    return 0;

/*     End of SGBCON */

} /* sgbcon_ */
