/* cheev.f -- translated by f2c (version 20061008).
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
static integer c_n1 = -1;
static integer c__0 = 0;
static real c_b18 = 1.f;

/* Subroutine */ int cheev_(char *jobz, char *uplo, integer *n, complex *a, 
	integer *lda, real *w, complex *work, integer *lwork, real *rwork, 
	integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    integer nb;
    real eps;
    integer inde;
    real anrm;
    integer imax;
    real rmin, rmax, sigma;
    extern logical lsame_(char *, char *);
    integer iinfo;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    logical lower, wantz;
    extern doublereal clanhe_(char *, char *, integer *, complex *, integer *, 
	     real *);
    integer iscale;
    extern /* Subroutine */ int clascl_(char *, integer *, integer *, real *, 
	    real *, integer *, integer *, complex *, integer *, integer *);
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int chetrd_(char *, integer *, complex *, integer 
	    *, real *, real *, complex *, complex *, integer *, integer *);
    real safmin;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *);
    extern /* Subroutine */ int xerbla_(char *, integer *);
    real bignum;
    integer indtau, indwrk;
    extern /* Subroutine */ int csteqr_(char *, integer *, real *, real *, 
	    complex *, integer *, real *, integer *), cungtr_(char *, 
	    integer *, complex *, integer *, complex *, complex *, integer *, 
	    integer *), ssterf_(integer *, real *, real *, integer *);
    integer llwork;
    real smlnum;
    integer lwkopt;
    logical lquery;


/*  -- LAPACK driver routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CHEEV computes all eigenvalues and, optionally, eigenvectors of a */
/*  complex Hermitian matrix A. */

/*  Arguments */
/*  ========= */

/*  JOBZ    (input) CHARACTER*1 */
/*          = 'N':  Compute eigenvalues only; */
/*          = 'V':  Compute eigenvalues and eigenvectors. */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A is stored; */
/*          = 'L':  Lower triangle of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input/output) COMPLEX array, dimension (LDA, N) */
/*          On entry, the Hermitian matrix A.  If UPLO = 'U', the */
/*          leading N-by-N upper triangular part of A contains the */
/*          upper triangular part of the matrix A.  If UPLO = 'L', */
/*          the leading N-by-N lower triangular part of A contains */
/*          the lower triangular part of the matrix A. */
/*          On exit, if JOBZ = 'V', then if INFO = 0, A contains the */
/*          orthonormal eigenvectors of the matrix A. */
/*          If JOBZ = 'N', then on exit the lower triangle (if UPLO='L') */
/*          or the upper triangle (if UPLO='U') of A, including the */
/*          diagonal, is destroyed. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  W       (output) REAL array, dimension (N) */
/*          If INFO = 0, the eigenvalues in ascending order. */

/*  WORK    (workspace/output) COMPLEX array, dimension (MAX(1,LWORK)) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The length of the array WORK.  LWORK >= max(1,2*N-1). */
/*          For optimal efficiency, LWORK >= (NB+1)*N, */
/*          where NB is the blocksize for CHETRD returned by ILAENV. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */

/*  RWORK   (workspace) REAL array, dimension (max(1, 3*N-2)) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, the algorithm failed to converge; i */
/*                off-diagonal elements of an intermediate tridiagonal */
/*                form did not converge to zero. */

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
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --w;
    --work;
    --rwork;

    /* Function Body */
    wantz = lsame_(jobz, "V");
    lower = lsame_(uplo, "L");
    lquery = *lwork == -1;

    *info = 0;
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lower || lsame_(uplo, "U"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*lda < max(1,*n)) {
	*info = -5;
    }

    if (*info == 0) {
	nb = ilaenv_(&c__1, "CHETRD", uplo, n, &c_n1, &c_n1, &c_n1);
/* Computing MAX */
	i__1 = 1, i__2 = (nb + 1) * *n;
	lwkopt = max(i__1,i__2);
	work[1].r = (real) lwkopt, work[1].i = 0.f;

/* Computing MAX */
	i__1 = 1, i__2 = (*n << 1) - 1;
	if (*lwork < max(i__1,i__2) && ! lquery) {
	    *info = -8;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CHEEV ", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	i__1 = a_dim1 + 1;
	w[1] = a[i__1].r;
	work[1].r = 1.f, work[1].i = 0.f;
	if (wantz) {
	    i__1 = a_dim1 + 1;
	    a[i__1].r = 1.f, a[i__1].i = 0.f;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = slamch_("Safe minimum");
    eps = slamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1.f / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = clanhe_("M", uplo, n, &a[a_offset], lda, &rwork[1]);
    iscale = 0;
    if (anrm > 0.f && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	clascl_(uplo, &c__0, &c__0, &c_b18, &sigma, n, n, &a[a_offset], lda, 
		info);
    }

/*     Call CHETRD to reduce Hermitian matrix to tridiagonal form. */

    inde = 1;
    indtau = 1;
    indwrk = indtau + *n;
    llwork = *lwork - indwrk + 1;
    chetrd_(uplo, n, &a[a_offset], lda, &w[1], &rwork[inde], &work[indtau], &
	    work[indwrk], &llwork, &iinfo);

/*     For eigenvalues only, call SSTERF.  For eigenvectors, first call */
/*     CUNGTR to generate the unitary matrix, then call CSTEQR. */

    if (! wantz) {
	ssterf_(n, &w[1], &rwork[inde], info);
    } else {
	cungtr_(uplo, n, &a[a_offset], lda, &work[indtau], &work[indwrk], &
		llwork, &iinfo);
	indwrk = inde + *n;
	csteqr_(jobz, n, &w[1], &rwork[inde], &a[a_offset], lda, &rwork[
		indwrk], info);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	if (*info == 0) {
	    imax = *n;
	} else {
	    imax = *info - 1;
	}
	r__1 = 1.f / sigma;
	sscal_(&imax, &r__1, &w[1], &c__1);
    }

/*     Set WORK(1) to optimal complex workspace size. */

    work[1].r = (real) lwkopt, work[1].i = 0.f;

    return 0;

/*     End of CHEEV */

} /* cheev_ */
