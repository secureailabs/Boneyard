#include "blas.h"
#include "clapack.h"
/* ctrrfs.f -- translated by f2c (version 20160102).
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

/* > \brief \b CTRRFS */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download CTRRFS + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/ctrrfs.
f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/ctrrfs.
f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/ctrrfs.
f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE CTRRFS( UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, X, */
/*                          LDX, FERR, BERR, WORK, RWORK, INFO ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          DIAG, TRANS, UPLO */
/*       INTEGER            INFO, LDA, LDB, LDX, N, NRHS */
/*       .. */
/*       .. Array Arguments .. */
/*       REAL               BERR( * ), FERR( * ), RWORK( * ) */
/*       COMPLEX            A( LDA, * ), B( LDB, * ), WORK( * ), */
/*      $                   X( LDX, * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > CTRRFS provides error bounds and backward error estimates for the */
/* > solution to a system of linear equations with a triangular */
/* > coefficient matrix. */
/* > */
/* > The solution matrix X must be computed by CTRTRS or some other */
/* > means before entering this routine.  CTRRFS does not do iterative */
/* > refinement because doing so cannot improve the backward error. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >          = 'U':  A is upper triangular; */
/* >          = 'L':  A is lower triangular. */
/* > \endverbatim */
/* > */
/* > \param[in] TRANS */
/* > \verbatim */
/* >          TRANS is CHARACTER*1 */
/* >          Specifies the form of the system of equations: */
/* >          = 'N':  A * X = B     (No transpose) */
/* >          = 'T':  A**T * X = B  (Transpose) */
/* >          = 'C':  A**H * X = B  (Conjugate transpose) */
/* > \endverbatim */
/* > */
/* > \param[in] DIAG */
/* > \verbatim */
/* >          DIAG is CHARACTER*1 */
/* >          = 'N':  A is non-unit triangular; */
/* >          = 'U':  A is unit triangular. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >          The order of the matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] NRHS */
/* > \verbatim */
/* >          NRHS is INTEGER */
/* >          The number of right hand sides, i.e., the number of columns */
/* >          of the matrices B and X.  NRHS >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] A */
/* > \verbatim */
/* >          A is COMPLEX array, dimension (LDA,N) */
/* >          The triangular matrix A.  If UPLO = 'U', the leading N-by-N */
/* >          upper triangular part of the array A contains the upper */
/* >          triangular matrix, and the strictly lower triangular part of */
/* >          A is not referenced.  If UPLO = 'L', the leading N-by-N lower */
/* >          triangular part of the array A contains the lower triangular */
/* >          matrix, and the strictly upper triangular part of A is not */
/* >          referenced.  If DIAG = 'U', the diagonal elements of A are */
/* >          also not referenced and are assumed to be 1. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >          The leading dimension of the array A.  LDA >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] B */
/* > \verbatim */
/* >          B is COMPLEX array, dimension (LDB,NRHS) */
/* >          The right hand side matrix B. */
/* > \endverbatim */
/* > */
/* > \param[in] LDB */
/* > \verbatim */
/* >          LDB is INTEGER */
/* >          The leading dimension of the array B.  LDB >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] X */
/* > \verbatim */
/* >          X is COMPLEX array, dimension (LDX,NRHS) */
/* >          The solution matrix X. */
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
/* >          FERR is REAL array, dimension (NRHS) */
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
/* >          BERR is REAL array, dimension (NRHS) */
/* >          The componentwise relative backward error of each solution */
/* >          vector X(j) (i.e., the smallest relative change in */
/* >          any element of A or B that makes X(j) an exact solution). */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX array, dimension (2*N) */
/* > \endverbatim */
/* > */
/* > \param[out] RWORK */
/* > \verbatim */
/* >          RWORK is REAL array, dimension (N) */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* >          INFO is INTEGER */
/* >          = 0:  successful exit */
/* >          < 0:  if INFO = -i, the i-th argument had an illegal value */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complexOTHERcomputational */

/*  ===================================================================== */
/* Subroutine */ int ctrrfs_(char *uplo, char *trans, char *diag, integer *n, 
	integer *nrhs, complex *a, integer *lda, complex *b, integer *ldb, 
	complex *x, integer *ldx, real *ferr, real *berr, complex *work, real 
	*rwork, integer *info, ftnlen uplo_len, ftnlen trans_len, ftnlen 
	diag_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, x_dim1, x_offset, i__1, i__2, 
	    i__3, i__4, i__5;
    real r__1, r__2, r__3, r__4;
    complex q__1;

    /* Builtin functions */
    double r_imag(complex *);

    /* Local variables */
    static integer i__, j, k;
    static real s, xk;
    static integer nz;
    static real eps;
    static integer kase;
    static real safe1, safe2;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer isave[3];
    extern /* Subroutine */ int ccopy_(integer *, complex *, integer *, 
	    complex *, integer *), caxpy_(integer *, complex *, complex *, 
	    integer *, complex *, integer *);
    static logical upper;
    extern /* Subroutine */ int ctrmv_(char *, char *, char *, integer *, 
	    complex *, integer *, complex *, integer *, ftnlen, ftnlen, 
	    ftnlen), ctrsv_(char *, char *, char *, integer *, complex *, 
	    integer *, complex *, integer *, ftnlen, ftnlen, ftnlen), clacn2_(
	    integer *, complex *, complex *, real *, integer *, integer *);
    extern doublereal slamch_(char *, ftnlen);
    static real safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static logical notran;
    static char transn[1], transt[1];
    static logical nounit;
    static real lstres;


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
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
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
    notran = lsame_(trans, "N", (ftnlen)1, (ftnlen)1);
    nounit = lsame_(diag, "N", (ftnlen)1, (ftnlen)1);

    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (! notran && ! lsame_(trans, "T", (ftnlen)1, (ftnlen)1) && ! 
	    lsame_(trans, "C", (ftnlen)1, (ftnlen)1)) {
	*info = -2;
    } else if (! nounit && ! lsame_(diag, "U", (ftnlen)1, (ftnlen)1)) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    } else if (*nrhs < 0) {
	*info = -5;
    } else if (*lda < max(1,*n)) {
	*info = -7;
    } else if (*ldb < max(1,*n)) {
	*info = -9;
    } else if (*ldx < max(1,*n)) {
	*info = -11;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CTRRFS", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0 || *nrhs == 0) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    ferr[j] = 0.f;
	    berr[j] = 0.f;
/* L10: */
	}
	return 0;
    }

    if (notran) {
	*(unsigned char *)transn = 'N';
	*(unsigned char *)transt = 'C';
    } else {
	*(unsigned char *)transn = 'C';
	*(unsigned char *)transt = 'N';
    }

/*     NZ = maximum number of nonzero elements in each row of A, plus 1 */

    nz = *n + 1;
    eps = slamch_("Epsilon", (ftnlen)7);
    safmin = slamch_("Safe minimum", (ftnlen)12);
    safe1 = nz * safmin;
    safe2 = safe1 / eps;

/*     Do for each right hand side */

    i__1 = *nrhs;
    for (j = 1; j <= i__1; ++j) {

/*        Compute residual R = B - op(A) * X, */
/*        where op(A) = A, A**T, or A**H, depending on TRANS. */

	ccopy_(n, &x[j * x_dim1 + 1], &c__1, &work[1], &c__1);
	ctrmv_(uplo, trans, diag, n, &a[a_offset], lda, &work[1], &c__1, (
		ftnlen)1, (ftnlen)1, (ftnlen)1);
	q__1.r = -1.f, q__1.i = -0.f;
	caxpy_(n, &q__1, &b[j * b_dim1 + 1], &c__1, &work[1], &c__1);

/*        Compute componentwise relative backward error from formula */

/*        max(i) ( abs(R(i)) / ( abs(op(A))*abs(X) + abs(B) )(i) ) */

/*        where abs(Z) is the componentwise absolute value of the matrix */
/*        or vector Z.  If the i-th component of the denominator is less */
/*        than SAFE2, then SAFE1 is added to the i-th components of the */
/*        numerator and denominator before dividing. */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    i__3 = i__ + j * b_dim1;
	    rwork[i__] = (r__1 = b[i__3].r, dabs(r__1)) + (r__2 = r_imag(&b[
		    i__ + j * b_dim1]), dabs(r__2));
/* L20: */
	}

	if (notran) {

/*           Compute abs(A)*abs(X) + abs(B). */

	    if (upper) {
		if (nounit) {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			xk = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = k;
			for (i__ = 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    rwork[i__] += ((r__1 = a[i__4].r, dabs(r__1)) + (
				    r__2 = r_imag(&a[i__ + k * a_dim1]), dabs(
				    r__2))) * xk;
/* L30: */
			}
/* L40: */
		    }
		} else {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			xk = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = k - 1;
			for (i__ = 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    rwork[i__] += ((r__1 = a[i__4].r, dabs(r__1)) + (
				    r__2 = r_imag(&a[i__ + k * a_dim1]), dabs(
				    r__2))) * xk;
/* L50: */
			}
			rwork[k] += xk;
/* L60: */
		    }
		}
	    } else {
		if (nounit) {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			xk = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = *n;
			for (i__ = k; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    rwork[i__] += ((r__1 = a[i__4].r, dabs(r__1)) + (
				    r__2 = r_imag(&a[i__ + k * a_dim1]), dabs(
				    r__2))) * xk;
/* L70: */
			}
/* L80: */
		    }
		} else {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			xk = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = *n;
			for (i__ = k + 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    rwork[i__] += ((r__1 = a[i__4].r, dabs(r__1)) + (
				    r__2 = r_imag(&a[i__ + k * a_dim1]), dabs(
				    r__2))) * xk;
/* L90: */
			}
			rwork[k] += xk;
/* L100: */
		    }
		}
	    }
	} else {

/*           Compute abs(A**H)*abs(X) + abs(B). */

	    if (upper) {
		if (nounit) {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			s = 0.f;
			i__3 = k;
			for (i__ = 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    i__5 = i__ + j * x_dim1;
			    s += ((r__1 = a[i__4].r, dabs(r__1)) + (r__2 = 
				    r_imag(&a[i__ + k * a_dim1]), dabs(r__2)))
				     * ((r__3 = x[i__5].r, dabs(r__3)) + (
				    r__4 = r_imag(&x[i__ + j * x_dim1]), dabs(
				    r__4)));
/* L110: */
			}
			rwork[k] += s;
/* L120: */
		    }
		} else {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			s = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = k - 1;
			for (i__ = 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    i__5 = i__ + j * x_dim1;
			    s += ((r__1 = a[i__4].r, dabs(r__1)) + (r__2 = 
				    r_imag(&a[i__ + k * a_dim1]), dabs(r__2)))
				     * ((r__3 = x[i__5].r, dabs(r__3)) + (
				    r__4 = r_imag(&x[i__ + j * x_dim1]), dabs(
				    r__4)));
/* L130: */
			}
			rwork[k] += s;
/* L140: */
		    }
		}
	    } else {
		if (nounit) {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			s = 0.f;
			i__3 = *n;
			for (i__ = k; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    i__5 = i__ + j * x_dim1;
			    s += ((r__1 = a[i__4].r, dabs(r__1)) + (r__2 = 
				    r_imag(&a[i__ + k * a_dim1]), dabs(r__2)))
				     * ((r__3 = x[i__5].r, dabs(r__3)) + (
				    r__4 = r_imag(&x[i__ + j * x_dim1]), dabs(
				    r__4)));
/* L150: */
			}
			rwork[k] += s;
/* L160: */
		    }
		} else {
		    i__2 = *n;
		    for (k = 1; k <= i__2; ++k) {
			i__3 = k + j * x_dim1;
			s = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = r_imag(&
				x[k + j * x_dim1]), dabs(r__2));
			i__3 = *n;
			for (i__ = k + 1; i__ <= i__3; ++i__) {
			    i__4 = i__ + k * a_dim1;
			    i__5 = i__ + j * x_dim1;
			    s += ((r__1 = a[i__4].r, dabs(r__1)) + (r__2 = 
				    r_imag(&a[i__ + k * a_dim1]), dabs(r__2)))
				     * ((r__3 = x[i__5].r, dabs(r__3)) + (
				    r__4 = r_imag(&x[i__ + j * x_dim1]), dabs(
				    r__4)));
/* L170: */
			}
			rwork[k] += s;
/* L180: */
		    }
		}
	    }
	}
	s = 0.f;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
/* Computing MAX */
		i__3 = i__;
		r__3 = s, r__4 = ((r__1 = work[i__3].r, dabs(r__1)) + (r__2 = 
			r_imag(&work[i__]), dabs(r__2))) / rwork[i__];
		s = dmax(r__3,r__4);
	    } else {
/* Computing MAX */
		i__3 = i__;
		r__3 = s, r__4 = ((r__1 = work[i__3].r, dabs(r__1)) + (r__2 = 
			r_imag(&work[i__]), dabs(r__2)) + safe1) / (rwork[i__]
			 + safe1);
		s = dmax(r__3,r__4);
	    }
/* L190: */
	}
	berr[j] = s;

/*        Bound error from formula */

/*        norm(X - XTRUE) / norm(X) .le. FERR = */
/*        norm( abs(inv(op(A)))* */
/*           ( abs(R) + NZ*EPS*( abs(op(A))*abs(X)+abs(B) ))) / norm(X) */

/*        where */
/*          norm(Z) is the magnitude of the largest component of Z */
/*          inv(op(A)) is the inverse of op(A) */
/*          abs(Z) is the componentwise absolute value of the matrix or */
/*             vector Z */
/*          NZ is the maximum number of nonzeros in any row of A, plus 1 */
/*          EPS is machine epsilon */

/*        The i-th component of abs(R)+NZ*EPS*(abs(op(A))*abs(X)+abs(B)) */
/*        is incremented by SAFE1 if the i-th component of */
/*        abs(op(A))*abs(X) + abs(B) is less than SAFE2. */

/*        Use CLACN2 to estimate the infinity-norm of the matrix */
/*           inv(op(A)) * diag(W), */
/*        where W = abs(R) + NZ*EPS*( abs(op(A))*abs(X)+abs(B) ))) */

	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    if (rwork[i__] > safe2) {
		i__3 = i__;
		rwork[i__] = (r__1 = work[i__3].r, dabs(r__1)) + (r__2 = 
			r_imag(&work[i__]), dabs(r__2)) + nz * eps * rwork[
			i__];
	    } else {
		i__3 = i__;
		rwork[i__] = (r__1 = work[i__3].r, dabs(r__1)) + (r__2 = 
			r_imag(&work[i__]), dabs(r__2)) + nz * eps * rwork[
			i__] + safe1;
	    }
/* L200: */
	}

	kase = 0;
L210:
	clacn2_(n, &work[*n + 1], &work[1], &ferr[j], &kase, isave);
	if (kase != 0) {
	    if (kase == 1) {

/*              Multiply by diag(W)*inv(op(A)**H). */

		ctrsv_(uplo, transt, diag, n, &a[a_offset], lda, &work[1], &
			c__1, (ftnlen)1, (ftnlen)1, (ftnlen)1);
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__3 = i__;
		    i__4 = i__;
		    i__5 = i__;
		    q__1.r = rwork[i__4] * work[i__5].r, q__1.i = rwork[i__4] 
			    * work[i__5].i;
		    work[i__3].r = q__1.r, work[i__3].i = q__1.i;
/* L220: */
		}
	    } else {

/*              Multiply by inv(op(A))*diag(W). */

		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    i__3 = i__;
		    i__4 = i__;
		    i__5 = i__;
		    q__1.r = rwork[i__4] * work[i__5].r, q__1.i = rwork[i__4] 
			    * work[i__5].i;
		    work[i__3].r = q__1.r, work[i__3].i = q__1.i;
/* L230: */
		}
		ctrsv_(uplo, transn, diag, n, &a[a_offset], lda, &work[1], &
			c__1, (ftnlen)1, (ftnlen)1, (ftnlen)1);
	    }
	    goto L210;
	}

/*        Normalize error. */

	lstres = 0.f;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* Computing MAX */
	    i__3 = i__ + j * x_dim1;
	    r__3 = lstres, r__4 = (r__1 = x[i__3].r, dabs(r__1)) + (r__2 = 
		    r_imag(&x[i__ + j * x_dim1]), dabs(r__2));
	    lstres = dmax(r__3,r__4);
/* L240: */
	}
	if (lstres != 0.f) {
	    ferr[j] /= lstres;
	}

/* L250: */
    }

    return 0;

/*     End of CTRRFS */

} /* ctrrfs_ */
