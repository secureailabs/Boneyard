/* dspev.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int dspev_(char *jobz, char *uplo, integer *n, doublereal *
	ap, doublereal *w, doublereal *z__, integer *ldz, doublereal *work, 
	integer *info)
{
    /* System generated locals */
    integer z_dim1, z_offset, i__1;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    doublereal eps;
    integer inde;
    doublereal anrm;
    integer imax;
    doublereal rmin, rmax;
    extern /* Subroutine */ int dscal_(integer *, doublereal *, doublereal *, 
	    integer *);
    doublereal sigma;
    extern logical lsame_(char *, char *);
    integer iinfo;
    logical wantz;
    extern doublereal dlamch_(char *);
    integer iscale;
    doublereal safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    doublereal bignum;
    extern doublereal dlansp_(char *, char *, integer *, doublereal *, 
	    doublereal *);
    integer indtau;
    extern /* Subroutine */ int dsterf_(integer *, doublereal *, doublereal *, 
	     integer *);
    integer indwrk;
    extern /* Subroutine */ int dopgtr_(char *, integer *, doublereal *, 
	    doublereal *, doublereal *, integer *, doublereal *, integer *), dsptrd_(char *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *), dsteqr_(char *, 
	    integer *, doublereal *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *);
    doublereal smlnum;


/*  -- LAPACK driver routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DSPEV computes all the eigenvalues and, optionally, eigenvectors of a */
/*  real symmetric matrix A in packed storage. */

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

/*  AP      (input/output) DOUBLE PRECISION array, dimension (N*(N+1)/2) */
/*          On entry, the upper or lower triangle of the symmetric matrix */
/*          A, packed columnwise in a linear array.  The j-th column of A */
/*          is stored in the array AP as follows: */
/*          if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j; */
/*          if UPLO = 'L', AP(i + (j-1)*(2*n-j)/2) = A(i,j) for j<=i<=n. */

/*          On exit, AP is overwritten by values generated during the */
/*          reduction to tridiagonal form.  If UPLO = 'U', the diagonal */
/*          and first superdiagonal of the tridiagonal matrix T overwrite */
/*          the corresponding elements of A, and if UPLO = 'L', the */
/*          diagonal and first subdiagonal of T overwrite the */
/*          corresponding elements of A. */

/*  W       (output) DOUBLE PRECISION array, dimension (N) */
/*          If INFO = 0, the eigenvalues in ascending order. */

/*  Z       (output) DOUBLE PRECISION array, dimension (LDZ, N) */
/*          If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal */
/*          eigenvectors of the matrix A, with the i-th column of Z */
/*          holding the eigenvector associated with W(i). */
/*          If JOBZ = 'N', then Z is not referenced. */

/*  LDZ     (input) INTEGER */
/*          The leading dimension of the array Z.  LDZ >= 1, and if */
/*          JOBZ = 'V', LDZ >= max(1,N). */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (3*N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit. */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
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
    --ap;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;

    /* Function Body */
    wantz = lsame_(jobz, "V");

    *info = 0;
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lsame_(uplo, "U") || lsame_(uplo, 
	    "L"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -7;
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DSPEV ", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	w[1] = ap[1];
	if (wantz) {
	    z__[z_dim1 + 1] = 1.;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = dlamch_("Safe minimum");
    eps = dlamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1. / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = dlansp_("M", uplo, n, &ap[1], &work[1]);
    iscale = 0;
    if (anrm > 0. && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	i__1 = *n * (*n + 1) / 2;
	dscal_(&i__1, &sigma, &ap[1], &c__1);
    }

/*     Call DSPTRD to reduce symmetric packed matrix to tridiagonal form. */

    inde = 1;
    indtau = inde + *n;
    dsptrd_(uplo, n, &ap[1], &w[1], &work[inde], &work[indtau], &iinfo);

/*     For eigenvalues only, call DSTERF.  For eigenvectors, first call */
/*     DOPGTR to generate the orthogonal matrix, then call DSTEQR. */

    if (! wantz) {
	dsterf_(n, &w[1], &work[inde], info);
    } else {
	indwrk = indtau + *n;
	dopgtr_(uplo, n, &ap[1], &work[indtau], &z__[z_offset], ldz, &work[
		indwrk], &iinfo);
	dsteqr_(jobz, n, &w[1], &work[inde], &z__[z_offset], ldz, &work[
		indtau], info);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	if (*info == 0) {
	    imax = *n;
	} else {
	    imax = *info - 1;
	}
	d__1 = 1. / sigma;
	dscal_(&imax, &d__1, &w[1], &c__1);
    }

    return 0;

/*     End of DSPEV */

} /* dspev_ */
