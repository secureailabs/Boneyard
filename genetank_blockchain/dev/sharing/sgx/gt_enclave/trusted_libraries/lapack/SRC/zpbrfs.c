#include "blas.h"
#include "clapack.h"
/* zpbrfs.f -- translated by f2c (version 20160102).
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

static doublecomplex c_b1 = {1.,0.};
static integer c__1 = 1;

/* > \brief \b ZPBRFS */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download ZPBRFS + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/zpbrfs.
f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/zpbrfs.
f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/zpbrfs.
f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE ZPBRFS( UPLO, N, KD, NRHS, AB, LDAB, AFB, LDAFB, B, */
/*                          LDB, X, LDX, FERR, BERR, WORK, RWORK, INFO ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          UPLO */
/*       INTEGER            INFO, KD, LDAB, LDAFB, LDB, LDX, N, NRHS */
/*       .. */
/*       .. Array Arguments .. */
/*       DOUBLE PRECISION   BERR( * ), FERR( * ), RWORK( * ) */
/*       COMPLEX*16         AB( LDAB, * ), AFB( LDAFB, * ), B( LDB, * ), */
/*      $                   WORK( * ), X( LDX, * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZPBRFS improves the computed solution to a system of linear */
/* > equations when the coefficient matrix is Hermitian positive definite */
/* > and banded, and provides error bounds and backward error estimates */
/* > for the solution. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >          = 'U':  Upper triangle of A is stored; */
/* >          = 'L':  Lower triangle of A is stored. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >          The order of the matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] KD */
/* > \verbatim */
/* >          KD is INTEGER */
/* >          The number of superdiagonals of the matrix A if UPLO = 'U', */
/* >          or the number of subdiagonals if UPLO = 'L'.  KD >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] NRHS */
/* > \verbatim */
/* >          NRHS is INTEGER */
/* >          The number of right hand sides, i.e., the number of columns */
/* >          of the matrices B and X.  NRHS >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] AB */
/* > \verbatim */
/* >          AB is COMPLEX*16 array, dimension (LDAB,N) */
/* >          The upper or lower triangle of the Hermitian band matrix A, */
/* >          stored in the first KD+1 rows of the array.  The j-th column */
/* >          of A is stored in the j-th column of the array AB as follows: */
/* >          if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j; */
/* >          if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd). */
/* > \endverbatim */
/* > */
/* > \param[in] LDAB */
/* > \verbatim */
/* >          LDAB is INTEGER */
/* >          The leading dimension of the array AB.  LDAB >= KD+1. */
/* > \endverbatim */
/* > */
/* > \param[in] AFB */
/* > \verbatim */
/* >          AFB is COMPLEX*16 array, dimension (LDAFB,N) */
/* >          The triangular factor U or L from the Cholesky factorization */
/* >          A = U**H*U or A = L*L**H of the band matrix A as computed by */
/* >          ZPBTRF, in the same storage format as A (see AB). */
/* > \endverbatim */
/* > */
/* > \param[in] LDAFB */
/* > \verbatim */
/* >          LDAFB is INTEGER */
/* >          The leading dimension of the array AFB.  LDAFB >= KD+1. */
/* > \endverbatim */
/* > */
/* > \param[in] B */
/* > \verbatim */
/* >          B is COMPLEX*16 array, dimension (LDB,NRHS) */
/* >          The right hand side matrix B. */
/* > \endverbatim */
/* > */
/* > \param[in] LDB */
/* > \verbatim */
/* >          LDB is INTEGER */
/* >          The leading dimension of the array B.  LDB >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in,out] X */
/* > \verbatim */
/* >          X is COMPLEX*16 array, dimension (LDX,NRHS) */
/* >          On entry, the solution matrix X, as computed by ZPBTRS. */
/* >          On exit, the improved solution matrix X. */
/* > \endverbatim */
/* > */
/* > \param[in] LDX */
/* > \verbatim */
/* >          LDX is INTEGER */
/* >          The leading dimension of the array X.  LDX >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[out] FERR */
/* > \verbatim */
/* >          FERR is DOUBLE PRECISION array, dimension (NRHS) */
/* >          The estimated forward error bound for each solution vector */
/* >          X(j) (the j-th column of the solution matrix X). */
/* >          If XTRUE is the true solution corresponding to X(j), FERR(j) */
/* >          is an estimated upper bound for the magnitude of the largest */
/* >          element in (X(j) - XTRUE) divided by the magnitude of the */
/* >          largest element in X(j).  The estimate is as reliable as */
/* >          the estimate for RCOND, and is almost always a slight */
/* >          overestimate of the true error. */
/* > \endverbatim */
/* > */
/* > \param[out] BERR */
/* > \verbatim */
/* >          BERR is DOUBLE PRECISION array, dimension (NRHS) */
/* >          The componentwise relative backward error of each solution */
/* >          vector X(j) (i.e., the smallest relative change in */
/* >          any element of A or B that makes X(j) an exact solution). */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX*16 array, dimension (2*N) */
/* > \endverbatim */
/* > */
/* > \param[out] RWORK */
/* > \verbatim */
/* >          RWORK is DOUBLE PRECISION array, dimension (N) */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* >          INFO is INTEGER */
/* >          = 0:  successful exit */
/* >          < 0:  if INFO = -i, the i-th argument had an illegal value */
/* > \endverbatim */

/* > \par Internal Parameters: */
/*  ========================= */
/* > */
/* > \verbatim */
/* >  ITMAX is the maximum number of steps of iterative refinement. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date June 2016 */

/* > \ingroup complex16OTHERcomputational */

/*  ===================================================================== */
/* Subroutine */ int zpbrfs_(char *uplo, integer *n, integer *kd, integer *
	nrhs, doublecomplex *ab, integer *ldab, doublecomplex *afb, integer *
	ldafb, doublecomplex *b, integer *ldb, doublecomplex *x, integer *ldx,
	 doublereal *ferr, doublereal *berr, doublecomplex *work, doublereal *
	rwork, integer *info, ftnlen uplo_len)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, afb_dim1, afb_offset, b_dim1, b_offset, 
	    x_dim1, x_offset, i__1, i__2, i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4;
    doublecomplex z__1;

    /* Builtin functions */
    double d_imag(doublecomplex *);

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal s, xk;
    static integer nz;
    static doublereal eps;
    static integer kase;
    static doublereal safe1, safe2;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer isave[3];
    extern /* Subroutine */ int zhbmv_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, integer *, doublecomplex *, 
	    integer *, doublecomplex *, doublecomplex *, integer *, ftnlen);
    static integer count;
    static logical upper;
    extern /* Subroutine */ int zcopy_(integer *, doublecomplex *, integer *, 
	    doublecomplex *, integer *), zaxpy_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *, integer *), zlacn2_(
	    integer *, doublecomplex *, doublecomplex *, doublereal *, 
	    integer *, integer *);
    extern doublereal dlamch_(char *, ftnlen);
    static doublereal safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static doublereal lstres;
    extern /* Subroutine */ int zpbtrs_(char *, integer *, integer *, integer 
	    *, doublecomplex *, integer *, doublecomplex *, integer *, 
	    integer *, ftnlen);


/*  -- LAPACK computational routine (version 3.7.0) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     June 2016 */

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
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    afb_dim1 = *ldafb;
    afb_offset = 1 + afb_dim1;
    afb -= afb_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --ferr;
    --berr;
    --work;
    --rwork;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*kd < 0) {
	*info = -3;
    } else if (*nrhs < 0) {
	*info = -4;
    } else if (*ldab < *kd + 1) {
	*info = -6;
    } else if (*ldafb < *kd + 1) {
	*info = -8;
    } else if (*ldb < max(1,*n)) {
	*info = -10;
    } else if (*ldx < max(1,*n)) {
	*info = -12;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("ZPBRFS", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    ferr[j] = 0.;
	    berr[j] = 0.;
/* L10: */
	}
	return 0;
    }

/*     NZ = maximum number of nonzero elements in each row of A, plus 1 */

/* Computing MIN */
    i__1 = *n + 1, i__2 = (*kd << 1) + 2;
    nz = min(i__1,i__2);
    eps = dlamch_("Epsilon", (ftnlen)7);
    safmin = dlamch_("Safe minimum", (ftnlen)12);
    safe1 = nz * safmin;
    safe2 = safe1 / eps;

/*     Do for each right hand side */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {

	count = 1;
	lstres = 3.;
L20:

/*        Loop until stopping criterion is satisfied. */

/*        Compute residual R = B - A * X */

	zcopy_(n, &b[j * b_dim1 + 1], &c__1, &work[1], &c__1);
	z__1.r = -1., z__1.i = -0.;
	zhbmv_(uplo, n, kd, &z__1, &ab[ab_offset], ldab, &x[j * x_dim1 + 1], &
		c__1, &c_b1, &work[1], &c__1, (ftnlen)1);

/*        Compute componentwise relative backward error from formula */

/*        max(i) ( abs(R(i)) / ( abs(A)*abs(X) + abs(B) )(i) ) */

/*        where abs(Z) is the componentwise absolute value of the matrix */
/*        or vector Z.  If the i-th component of the denominator is less */
/*        than SAFE2, then SAFE1 is added to the i-th components of the */
/*        numerator and denominator before dividing. */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * b_dim1;
	    rwork[i__] = (d__1 = b[i__3].r, abs(d__1)) + (d__2 = d_imag(&b[
		    i__ + j * b_dim1]), abs(d__2));
/* L30: */
	}

/*        Compute abs(A)*abs(X) + abs(B). */

	if (upper) {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.;
		i__3 = k + j * x_dim1;
		xk = (d__1 = x[i__3].r, abs(d__1)) + (d__2 = d_imag(&x[k + j *
			 x_dim1]), abs(d__2));
		l = *kd + 1 - k;
/* Computing MAX */
		i__3 = 1, i__4 = k - *kd;
		i__5 = k - 1;
		for (i__ = max(i__3,i__4); i__ <= i__5; ++i__) {
		    i__3 = l + i__ + k * ab_dim1;
		    rwork[i__] += ((d__1 = ab[i__3].r, abs(d__1)) + (d__2 = 
			    d_imag(&ab[l + i__ + k * ab_dim1]), abs(d__2))) * 
			    xk;
		    i__3 = l + i__ + k * ab_dim1;
		    i__4 = i__ + j * x_dim1;
		    s += ((d__1 = ab[i__3].r, abs(d__1)) + (d__2 = d_imag(&ab[
			    l + i__ + k * ab_dim1]), abs(d__2))) * ((d__3 = x[
			    i__4].r, abs(d__3)) + (d__4 = d_imag(&x[i__ + j * 
			    x_dim1]), abs(d__4)));
/* L40: */
		}
		i__5 = *kd + 1 + k * ab_dim1;
		rwork[k] = rwork[k] + (d__1 = ab[i__5].r, abs(d__1)) * xk + s;
/* L50: */
	    }
	} else {
	    i__2 = *n;
	    for (k = 1; k <= i__2; ++k) {
		s = 0.;
		i__5 = k + j * x_dim1;
		xk = (d__1 = x[i__5].r, abs(d__1)) + (d__2 = d_imag(&x[k + j *
			 x_dim1]), abs(d__2));
		i__5 = k * ab_dim1 + 1;
		rwork[k] += (d__1 = ab[i__5].r, abs(d__1)) * xk;
		l = 1 - k;
/* Computing MIN */
		i__3 = *n, i__4 = k + *kd;
		i__5 = min(i__3,i__4);
		for (i__ = k + 1; i__ <= i__5; ++i__) {
		    i__3 = l + i__ + k * ab_dim1;
		    rwork[i__] += ((d__1 = ab[i__3].r, abs(d__1)) + (d__2 = 
			    d_imag(&ab[l + i__ + k * ab_dim1]), abs(d__2))) * 
			    xk;
		    i__3 = l + i__ + k * ab_dim1;
		    i__4 = i__ + j * x_dim1;
		    s += ((d__1 = ab[i__3].r, abs(d__1)) + (d__2 = d_imag(&ab[
			    l + i__ + k * ab_dim1]), abs(d__2))) * ((d__3 = x[
			    i__4].r, abs(d__3)) + (d__4 = d_imag(&x[i__ + j * 
			    x_dim1]), abs(d__4)));
/* L60: */
		}
		rwork[k] += s;
/* L70: */
	    }
	}
	s = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
/* Computing MAX */
		i__5 = i__;
		d__3 = s, d__4 = ((d__1 = work[i__5].r, abs(d__1)) + (d__2 = 
			d_imag(&work[i__]), abs(d__2))) / rwork[i__];
		s = max(d__3,d__4);
	    } else {
/* Computing MAX */
		i__5 = i__;
		d__3 = s, d__4 = ((d__1 = work[i__5].r, abs(d__1)) + (d__2 = 
			d_imag(&work[i__]), abs(d__2)) + safe1) / (rwork[i__] 
			+ safe1);
		s = max(d__3,d__4);
	    }
/* L80: */
	}
	berr[j] = s;

/*        Test stopping criterion. Continue iterating if */
/*           1) The residual BERR(J) is larger than machine epsilon, and */
/*           2) BERR(J) decreased by at least a factor of 2 during the */
/*              last iteration, and */
/*           3) At most ITMAX iterations tried. */

	if (berr[j] > eps && berr[j] * 2. <= lstres && count <= 5) {

/*           Update solution and try again. */

	    zpbtrs_(uplo, n, kd, &c__1, &afb[afb_offset], ldafb, &work[1], n, 
		    info, (ftnlen)1);
	    zaxpy_(n, &c_b1, &work[1], &c__1, &x[j * x_dim1 + 1], &c__1);
	    lstres = berr[j];
	    ++count;
	    goto L20;
	}

/*        Bound error from formula */

/*        norm(X - XTRUE) / norm(X) .le. FERR = */
/*        norm( abs(inv(A))* */
/*           ( abs(R) + NZ*EPS*( abs(A)*abs(X)+abs(B) ))) / norm(X) */

/*        where */
/*          norm(Z) is the magnitude of the largest component of Z */
/*          inv(A) is the inverse of A */
/*          abs(Z) is the componentwise absolute value of the matrix or */
/*             vector Z */
/*          NZ is the maximum number of nonzeros in any row of A, plus 1 */
/*          EPS is machine epsilon */

/*        The i-th component of abs(R)+NZ*EPS*(abs(A)*abs(X)+abs(B)) */
/*        is incremented by SAFE1 if the i-th component of */
/*        abs(A)*abs(X) + abs(B) is less than SAFE2. */

/*        Use ZLACN2 to estimate the infinity-norm of the matrix */
/*           inv(A) * diag(W), */
/*        where W = abs(R) + NZ*EPS*( abs(A)*abs(X)+abs(B) ))) */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
		i__5 = i__;
		rwork[i__] = (d__1 = work[i__5].r, abs(d__1)) + (d__2 = 
			d_imag(&work[i__]), abs(d__2)) + nz * eps * rwork[i__]
			;
	    } else {
		i__5 = i__;
		rwork[i__] = (d__1 = work[i__5].r, abs(d__1)) + (d__2 = 
			d_imag(&work[i__]), abs(d__2)) + nz * eps * rwork[i__]
			 + safe1;
	    }
/* L90: */
	}

	kase = 0;
L100:
	zlacn2_(n, &work[*n + 1], &work[1], &ferr[j], &kase, isave);
	if (kase != 0) {
	    if (kase == 1) {

/*              Multiply by diag(W)*inv(A**H). */

		zpbtrs_(uplo, n, kd, &c__1, &afb[afb_offset], ldafb, &work[1],
			 n, info, (ftnlen)1);
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__5 = i__;
		    i__3 = i__;
		    i__4 = i__;
		    z__1.r = rwork[i__3] * work[i__4].r, z__1.i = rwork[i__3] 
			    * work[i__4].i;
		    work[i__5].r = z__1.r, work[i__5].i = z__1.i;
/* L110: */
		}
	    } else if (kase == 2) {

/*              Multiply by inv(A)*diag(W). */

		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__5 = i__;
		    i__3 = i__;
		    i__4 = i__;
		    z__1.r = rwork[i__3] * work[i__4].r, z__1.i = rwork[i__3] 
			    * work[i__4].i;
		    work[i__5].r = z__1.r, work[i__5].i = z__1.i;
/* L120: */
		}
		zpbtrs_(uplo, n, kd, &c__1, &afb[afb_offset], ldafb, &work[1],
			 n, info, (ftnlen)1);
	    }
	    goto L100;
	}

/*        Normalize error. */

	lstres = 0.;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
	    i__5 = i__ + j * x_dim1;
	    d__3 = lstres, d__4 = (d__1 = x[i__5].r, abs(d__1)) + (d__2 = 
		    d_imag(&x[i__ + j * x_dim1]), abs(d__2));
	    lstres = max(d__3,d__4);
/* L130: */
	}
	if (lstres != 0.) {
	    ferr[j] /= lstres;
	}

/* L140: */
    }

    return 0;

/*     End of ZPBRFS */

} /* zpbrfs_ */
