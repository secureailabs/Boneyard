#include "blas.h"
#include "clapack.h"
/* chetri2x.f -- translated by f2c (version 20160102).
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

static complex c_b1 = {1.f,0.f};
static complex c_b2 = {0.f,0.f};

/* > \brief \b CHETRI2X */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download CHETRI2X + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/chetri2
x.f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/chetri2
x.f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/chetri2
x.f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE CHETRI2X( UPLO, N, A, LDA, IPIV, WORK, NB, INFO ) */

/*       .. Scalar Arguments .. */
/*       CHARACTER          UPLO */
/*       INTEGER            INFO, LDA, N, NB */
/*       .. */
/*       .. Array Arguments .. */
/*       INTEGER            IPIV( * ) */
/*       COMPLEX            A( LDA, * ), WORK( N+NB+1,* ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > CHETRI2X computes the inverse of a complex Hermitian indefinite matrix */
/* > A using the factorization A = U*D*U**H or A = L*D*L**H computed by */
/* > CHETRF. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* >          Specifies whether the details of the factorization are stored */
/* >          as an upper or lower triangular matrix. */
/* >          = 'U':  Upper triangular, form is A = U*D*U**H; */
/* >          = 'L':  Lower triangular, form is A = L*D*L**H. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >          The order of the matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in,out] A */
/* > \verbatim */
/* >          A is COMPLEX array, dimension (LDA,N) */
/* >          On entry, the NNB diagonal matrix D and the multipliers */
/* >          used to obtain the factor U or L as computed by CHETRF. */
/* > */
/* >          On exit, if INFO = 0, the (symmetric) inverse of the original */
/* >          matrix.  If UPLO = 'U', the upper triangular part of the */
/* >          inverse is formed and the part of A below the diagonal is not */
/* >          referenced; if UPLO = 'L' the lower triangular part of the */
/* >          inverse is formed and the part of A above the diagonal is */
/* >          not referenced. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >          The leading dimension of the array A.  LDA >= max(1,N). */
/* > \endverbatim */
/* > */
/* > \param[in] IPIV */
/* > \verbatim */
/* >          IPIV is INTEGER array, dimension (N) */
/* >          Details of the interchanges and the NNB structure of D */
/* >          as determined by CHETRF. */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX array, dimension (N+NB+1,NB+3) */
/* > \endverbatim */
/* > */
/* > \param[in] NB */
/* > \verbatim */
/* >          NB is INTEGER */
/* >          Block size */
/* > \endverbatim */
/* > */
/* > \param[out] INFO */
/* > \verbatim */
/* >          INFO is INTEGER */
/* >          = 0: successful exit */
/* >          < 0: if INFO = -i, the i-th argument had an illegal value */
/* >          > 0: if INFO = i, D(i,i) = 0; the matrix is singular and its */
/* >               inverse could not be computed. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complexHEcomputational */

/*  ===================================================================== */
/* Subroutine */ int chetri2x_(char *uplo, integer *n, complex *a, integer *
	lda, integer *ipiv, complex *work, integer *nb, integer *info, ftnlen 
	uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, work_dim1, work_offset, i__1, i__2, i__3, i__4, 
	    i__5, i__6;
    real r__1;
    complex q__1, q__2, q__3;

    /* Builtin functions */
    double c_abs(complex *);
    void c_div(complex *, complex *, complex *), r_cnjg(complex *, complex *);

    /* Local variables */
    extern /* Subroutine */ int cheswapr_(char *, integer *, complex *, 
	    integer *, integer *, integer *, ftnlen);
    static complex d__;
    static integer i__, j, k;
    static complex t, ak;
    static integer u11, ip, nnb, cut;
    static complex akp1;
    static integer invd;
    static complex akkp1;
    extern /* Subroutine */ int cgemm_(char *, char *, integer *, integer *, 
	    integer *, complex *, complex *, integer *, complex *, integer *, 
	    complex *, complex *, integer *, ftnlen, ftnlen);
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer iinfo;
    extern /* Subroutine */ int ctrmm_(char *, char *, char *, char *, 
	    integer *, integer *, complex *, complex *, integer *, complex *, 
	    integer *, ftnlen, ftnlen, ftnlen, ftnlen);
    static integer count;
    static logical upper;
    static complex u01_i_j__, u11_i_j__;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen), ctrtri_(
	    char *, char *, integer *, complex *, integer *, integer *, 
	    ftnlen, ftnlen), csyconv_(char *, char *, integer *, complex *, 
	    integer *, integer *, complex *, integer *, ftnlen, ftnlen);
    static complex u01_ip1_j__, u11_ip1_j__;


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
    --ipiv;
    work_dim1 = *n + *nb + 1;
    work_offset = 1 + work_dim1;
    work -= work_offset;

    /* Function Body */
    *info = 0;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (! upper && ! lsame_(uplo, "L", (ftnlen)1, (ftnlen)1)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    }

/*     Quick return if possible */


    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CHETRI2X", &i__1, (ftnlen)8);
	return 0;
    }
    if (*n == 0) {
	return 0;
    }

/*     Convert A */
/*     Workspace got Non-diag elements of D */

    csyconv_(uplo, "C", n, &a[a_offset], lda, &ipiv[1], &work[work_offset], &
	    iinfo, (ftnlen)1, (ftnlen)1);

/*     Check that the diagonal matrix D is nonsingular. */

    if (upper) {

/*        Upper triangular storage: examine D from bottom to top */

	for (*info = *n; *info >= 1; --(*info)) {
	    i__1 = *info + *info * a_dim1;
	    if (ipiv[*info] > 0 && (a[i__1].r == 0.f && a[i__1].i == 0.f)) {
		return 0;
	    }
	}
    } else {

/*        Lower triangular storage: examine D from top to bottom. */

	i__1 = *n;
	for (*info = 1; *info <= i__1; ++(*info)) {
	    i__2 = *info + *info * a_dim1;
	    if (ipiv[*info] > 0 && (a[i__2].r == 0.f && a[i__2].i == 0.f)) {
		return 0;
	    }
	}
    }
    *info = 0;

/*  Splitting Workspace */
/*     U01 is a block (N,NB+1) */
/*     The first element of U01 is in WORK(1,1) */
/*     U11 is a block (NB+1,NB+1) */
/*     The first element of U11 is in WORK(N+1,1) */
    u11 = *n;
/*     INVD is a block (N,2) */
/*     The first element of INVD is in WORK(1,INVD) */
    invd = *nb + 2;
    if (upper) {

/*        invA = P * inv(U**H)*inv(D)*inv(U)*P**H. */

	ctrtri_(uplo, "U", n, &a[a_offset], lda, info, (ftnlen)1, (ftnlen)1);

/*       inv(D) and inv(D)*inv(U) */

	k = 1;
	while(k <= *n) {
	    if (ipiv[k] > 0) {
/*           1 x 1 diagonal NNB */
		i__1 = k + invd * work_dim1;
		i__2 = k + k * a_dim1;
		r__1 = 1.f / a[i__2].r;
		work[i__1].r = r__1, work[i__1].i = 0.f;
		i__1 = k + (invd + 1) * work_dim1;
		work[i__1].r = 0.f, work[i__1].i = 0.f;
		++k;
	    } else {
/*           2 x 2 diagonal NNB */
		r__1 = c_abs(&work[k + 1 + work_dim1]);
		t.r = r__1, t.i = 0.f;
		i__1 = k + k * a_dim1;
		r__1 = a[i__1].r;
		q__2.r = r__1, q__2.i = 0.f;
		c_div(&q__1, &q__2, &t);
		ak.r = q__1.r, ak.i = q__1.i;
		i__1 = k + 1 + (k + 1) * a_dim1;
		r__1 = a[i__1].r;
		q__2.r = r__1, q__2.i = 0.f;
		c_div(&q__1, &q__2, &t);
		akp1.r = q__1.r, akp1.i = q__1.i;
		c_div(&q__1, &work[k + 1 + work_dim1], &t);
		akkp1.r = q__1.r, akkp1.i = q__1.i;
		q__3.r = ak.r * akp1.r - ak.i * akp1.i, q__3.i = ak.r * 
			akp1.i + ak.i * akp1.r;
		q__2.r = q__3.r - 1.f, q__2.i = q__3.i;
		q__1.r = t.r * q__2.r - t.i * q__2.i, q__1.i = t.r * q__2.i + 
			t.i * q__2.r;
		d__.r = q__1.r, d__.i = q__1.i;
		i__1 = k + invd * work_dim1;
		c_div(&q__1, &akp1, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k + 1 + (invd + 1) * work_dim1;
		c_div(&q__1, &ak, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k + (invd + 1) * work_dim1;
		q__2.r = -akkp1.r, q__2.i = -akkp1.i;
		c_div(&q__1, &q__2, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k + 1 + invd * work_dim1;
		r_cnjg(&q__1, &work[k + (invd + 1) * work_dim1]);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		k += 2;
	    }
	}

/*       inv(U**H) = (inv(U))**H */

/*       inv(U**H)*inv(D)*inv(U) */

	cut = *n;
	while(cut > 0) {
	    nnb = *nb;
	    if (cut <= nnb) {
		nnb = cut;
	    } else {
		count = 0;
/*             count negative elements, */
		i__1 = cut;
		for (i__ = cut + 1 - nnb; i__ <= i__1; ++i__) {
		    if (ipiv[i__] < 0) {
			++count;
		    }
		}
/*             need a even number for a clear cut */
		if (count % 2 == 1) {
		    ++nnb;
		}
	    }
	    cut -= nnb;

/*          U01 Block */

	    i__1 = cut;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = nnb;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = i__ + j * work_dim1;
		    i__4 = i__ + (cut + j) * a_dim1;
		    work[i__3].r = a[i__4].r, work[i__3].i = a[i__4].i;
		}
	    }

/*          U11 Block */

	    i__1 = nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = u11 + i__ + i__ * work_dim1;
		work[i__2].r = 1.f, work[i__2].i = 0.f;
		i__2 = i__ - 1;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = u11 + i__ + j * work_dim1;
		    work[i__3].r = 0.f, work[i__3].i = 0.f;
		}
		i__2 = nnb;
		for (j = i__ + 1; j <= i__2; ++j) {
		    i__3 = u11 + i__ + j * work_dim1;
		    i__4 = cut + i__ + (cut + j) * a_dim1;
		    work[i__3].r = a[i__4].r, work[i__3].i = a[i__4].i;
		}
	    }

/*          invD*U01 */

	    i__ = 1;
	    while(i__ <= cut) {
		if (ipiv[i__] > 0) {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = i__ + j * work_dim1;
			i__3 = i__ + invd * work_dim1;
			i__4 = i__ + j * work_dim1;
			q__1.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__1.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    ++i__;
		} else {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = i__ + j * work_dim1;
			u01_i_j__.r = work[i__2].r, u01_i_j__.i = work[i__2]
				.i;
			i__2 = i__ + 1 + j * work_dim1;
			u01_ip1_j__.r = work[i__2].r, u01_ip1_j__.i = work[
				i__2].i;
			i__2 = i__ + j * work_dim1;
			i__3 = i__ + invd * work_dim1;
			q__2.r = work[i__3].r * u01_i_j__.r - work[i__3].i * 
				u01_i_j__.i, q__2.i = work[i__3].r * 
				u01_i_j__.i + work[i__3].i * u01_i_j__.r;
			i__4 = i__ + (invd + 1) * work_dim1;
			q__3.r = work[i__4].r * u01_ip1_j__.r - work[i__4].i *
				 u01_ip1_j__.i, q__3.i = work[i__4].r * 
				u01_ip1_j__.i + work[i__4].i * u01_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
			i__2 = i__ + 1 + j * work_dim1;
			i__3 = i__ + 1 + invd * work_dim1;
			q__2.r = work[i__3].r * u01_i_j__.r - work[i__3].i * 
				u01_i_j__.i, q__2.i = work[i__3].r * 
				u01_i_j__.i + work[i__3].i * u01_i_j__.r;
			i__4 = i__ + 1 + (invd + 1) * work_dim1;
			q__3.r = work[i__4].r * u01_ip1_j__.r - work[i__4].i *
				 u01_ip1_j__.i, q__3.i = work[i__4].r * 
				u01_ip1_j__.i + work[i__4].i * u01_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    i__ += 2;
		}
	    }

/*        invD1*U11 */

	    i__ = 1;
	    while(i__ <= nnb) {
		if (ipiv[cut + i__] > 0) {
		    i__1 = nnb;
		    for (j = i__; j <= i__1; ++j) {
			i__2 = u11 + i__ + j * work_dim1;
			i__3 = cut + i__ + invd * work_dim1;
			i__4 = u11 + i__ + j * work_dim1;
			q__1.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__1.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    ++i__;
		} else {
		    i__1 = nnb;
		    for (j = i__; j <= i__1; ++j) {
			i__2 = u11 + i__ + j * work_dim1;
			u11_i_j__.r = work[i__2].r, u11_i_j__.i = work[i__2]
				.i;
			i__2 = u11 + i__ + 1 + j * work_dim1;
			u11_ip1_j__.r = work[i__2].r, u11_ip1_j__.i = work[
				i__2].i;
			i__2 = u11 + i__ + j * work_dim1;
			i__3 = cut + i__ + invd * work_dim1;
			i__4 = u11 + i__ + j * work_dim1;
			q__2.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__2.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			i__5 = cut + i__ + (invd + 1) * work_dim1;
			i__6 = u11 + i__ + 1 + j * work_dim1;
			q__3.r = work[i__5].r * work[i__6].r - work[i__5].i * 
				work[i__6].i, q__3.i = work[i__5].r * work[
				i__6].i + work[i__5].i * work[i__6].r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
			i__2 = u11 + i__ + 1 + j * work_dim1;
			i__3 = cut + i__ + 1 + invd * work_dim1;
			q__2.r = work[i__3].r * u11_i_j__.r - work[i__3].i * 
				u11_i_j__.i, q__2.i = work[i__3].r * 
				u11_i_j__.i + work[i__3].i * u11_i_j__.r;
			i__4 = cut + i__ + 1 + (invd + 1) * work_dim1;
			q__3.r = work[i__4].r * u11_ip1_j__.r - work[i__4].i *
				 u11_ip1_j__.i, q__3.i = work[i__4].r * 
				u11_ip1_j__.i + work[i__4].i * u11_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    i__ += 2;
		}
	    }

/*       U11**H*invD1*U11->U11 */

	    i__1 = *n + *nb + 1;
	    ctrmm_("L", "U", "C", "U", &nnb, &nnb, &c_b1, &a[cut + 1 + (cut + 
		    1) * a_dim1], lda, &work[u11 + 1 + work_dim1], &i__1, (
		    ftnlen)1, (ftnlen)1, (ftnlen)1, (ftnlen)1);

	    i__1 = nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = nnb;
		for (j = i__; j <= i__2; ++j) {
		    i__3 = cut + i__ + (cut + j) * a_dim1;
		    i__4 = u11 + i__ + j * work_dim1;
		    a[i__3].r = work[i__4].r, a[i__3].i = work[i__4].i;
		}
	    }

/*          U01**H*invD*U01->A(CUT+I,CUT+J) */

	    i__1 = *n + *nb + 1;
	    i__2 = *n + *nb + 1;
	    cgemm_("C", "N", &nnb, &nnb, &cut, &c_b1, &a[(cut + 1) * a_dim1 + 
		    1], lda, &work[work_offset], &i__1, &c_b2, &work[u11 + 1 
		    + work_dim1], &i__2, (ftnlen)1, (ftnlen)1);

/*        U11 =  U11**H*invD1*U11 + U01**H*invD*U01 */

	    i__1 = nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = nnb;
		for (j = i__; j <= i__2; ++j) {
		    i__3 = cut + i__ + (cut + j) * a_dim1;
		    i__4 = cut + i__ + (cut + j) * a_dim1;
		    i__5 = u11 + i__ + j * work_dim1;
		    q__1.r = a[i__4].r + work[i__5].r, q__1.i = a[i__4].i + 
			    work[i__5].i;
		    a[i__3].r = q__1.r, a[i__3].i = q__1.i;
		}
	    }

/*        U01 =  U00**H*invD0*U01 */

	    i__1 = *n + *nb + 1;
	    ctrmm_("L", uplo, "C", "U", &cut, &nnb, &c_b1, &a[a_offset], lda, 
		    &work[work_offset], &i__1, (ftnlen)1, (ftnlen)1, (ftnlen)
		    1, (ftnlen)1);

/*        Update U01 */

	    i__1 = cut;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = nnb;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = i__ + (cut + j) * a_dim1;
		    i__4 = i__ + j * work_dim1;
		    a[i__3].r = work[i__4].r, a[i__3].i = work[i__4].i;
		}
	    }

/*      Next Block */

	}

/*        Apply PERMUTATIONS P and P**H: P * inv(U**H)*inv(D)*inv(U) *P**H */

	i__ = 1;
	while(i__ <= *n) {
	    if (ipiv[i__] > 0) {
		ip = ipiv[i__];
		if (i__ < ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &i__, &ip, (ftnlen)
			    1);
		}
		if (i__ > ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &ip, &i__, (ftnlen)
			    1);
		}
	    } else {
		ip = -ipiv[i__];
		++i__;
		if (i__ - 1 < ip) {
		    i__1 = i__ - 1;
		    cheswapr_(uplo, n, &a[a_offset], lda, &i__1, &ip, (ftnlen)
			    1);
		}
		if (i__ - 1 > ip) {
		    i__1 = i__ - 1;
		    cheswapr_(uplo, n, &a[a_offset], lda, &ip, &i__1, (ftnlen)
			    1);
		}
	    }
	    ++i__;
	}
    } else {

/*        LOWER... */

/*        invA = P * inv(U**H)*inv(D)*inv(U)*P**H. */

	ctrtri_(uplo, "U", n, &a[a_offset], lda, info, (ftnlen)1, (ftnlen)1);

/*       inv(D) and inv(D)*inv(U) */

	k = *n;
	while(k >= 1) {
	    if (ipiv[k] > 0) {
/*           1 x 1 diagonal NNB */
		i__1 = k + invd * work_dim1;
		i__2 = k + k * a_dim1;
		r__1 = 1.f / a[i__2].r;
		work[i__1].r = r__1, work[i__1].i = 0.f;
		i__1 = k + (invd + 1) * work_dim1;
		work[i__1].r = 0.f, work[i__1].i = 0.f;
		--k;
	    } else {
/*           2 x 2 diagonal NNB */
		r__1 = c_abs(&work[k - 1 + work_dim1]);
		t.r = r__1, t.i = 0.f;
		i__1 = k - 1 + (k - 1) * a_dim1;
		r__1 = a[i__1].r;
		q__2.r = r__1, q__2.i = 0.f;
		c_div(&q__1, &q__2, &t);
		ak.r = q__1.r, ak.i = q__1.i;
		i__1 = k + k * a_dim1;
		r__1 = a[i__1].r;
		q__2.r = r__1, q__2.i = 0.f;
		c_div(&q__1, &q__2, &t);
		akp1.r = q__1.r, akp1.i = q__1.i;
		c_div(&q__1, &work[k - 1 + work_dim1], &t);
		akkp1.r = q__1.r, akkp1.i = q__1.i;
		q__3.r = ak.r * akp1.r - ak.i * akp1.i, q__3.i = ak.r * 
			akp1.i + ak.i * akp1.r;
		q__2.r = q__3.r - 1.f, q__2.i = q__3.i;
		q__1.r = t.r * q__2.r - t.i * q__2.i, q__1.i = t.r * q__2.i + 
			t.i * q__2.r;
		d__.r = q__1.r, d__.i = q__1.i;
		i__1 = k - 1 + invd * work_dim1;
		c_div(&q__1, &akp1, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k + invd * work_dim1;
		c_div(&q__1, &ak, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k + (invd + 1) * work_dim1;
		q__2.r = -akkp1.r, q__2.i = -akkp1.i;
		c_div(&q__1, &q__2, &d__);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		i__1 = k - 1 + (invd + 1) * work_dim1;
		r_cnjg(&q__1, &work[k + (invd + 1) * work_dim1]);
		work[i__1].r = q__1.r, work[i__1].i = q__1.i;
		k += -2;
	    }
	}

/*       inv(U**H) = (inv(U))**H */

/*       inv(U**H)*inv(D)*inv(U) */

	cut = 0;
	while(cut < *n) {
	    nnb = *nb;
	    if (cut + nnb >= *n) {
		nnb = *n - cut;
	    } else {
		count = 0;
/*             count negative elements, */
		i__1 = cut + nnb;
		for (i__ = cut + 1; i__ <= i__1; ++i__) {
		    if (ipiv[i__] < 0) {
			++count;
		    }
		}
/*             need a even number for a clear cut */
		if (count % 2 == 1) {
		    ++nnb;
		}
	    }
/*      L21 Block */
	    i__1 = *n - cut - nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = nnb;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = i__ + j * work_dim1;
		    i__4 = cut + nnb + i__ + (cut + j) * a_dim1;
		    work[i__3].r = a[i__4].r, work[i__3].i = a[i__4].i;
		}
	    }
/*     L11 Block */
	    i__1 = nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = u11 + i__ + i__ * work_dim1;
		work[i__2].r = 1.f, work[i__2].i = 0.f;
		i__2 = nnb;
		for (j = i__ + 1; j <= i__2; ++j) {
		    i__3 = u11 + i__ + j * work_dim1;
		    work[i__3].r = 0.f, work[i__3].i = 0.f;
		}
		i__2 = i__ - 1;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = u11 + i__ + j * work_dim1;
		    i__4 = cut + i__ + (cut + j) * a_dim1;
		    work[i__3].r = a[i__4].r, work[i__3].i = a[i__4].i;
		}
	    }

/*          invD*L21 */

	    i__ = *n - cut - nnb;
	    while(i__ >= 1) {
		if (ipiv[cut + nnb + i__] > 0) {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = i__ + j * work_dim1;
			i__3 = cut + nnb + i__ + invd * work_dim1;
			i__4 = i__ + j * work_dim1;
			q__1.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__1.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    --i__;
		} else {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = i__ + j * work_dim1;
			u01_i_j__.r = work[i__2].r, u01_i_j__.i = work[i__2]
				.i;
			i__2 = i__ - 1 + j * work_dim1;
			u01_ip1_j__.r = work[i__2].r, u01_ip1_j__.i = work[
				i__2].i;
			i__2 = i__ + j * work_dim1;
			i__3 = cut + nnb + i__ + invd * work_dim1;
			q__2.r = work[i__3].r * u01_i_j__.r - work[i__3].i * 
				u01_i_j__.i, q__2.i = work[i__3].r * 
				u01_i_j__.i + work[i__3].i * u01_i_j__.r;
			i__4 = cut + nnb + i__ + (invd + 1) * work_dim1;
			q__3.r = work[i__4].r * u01_ip1_j__.r - work[i__4].i *
				 u01_ip1_j__.i, q__3.i = work[i__4].r * 
				u01_ip1_j__.i + work[i__4].i * u01_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
			i__2 = i__ - 1 + j * work_dim1;
			i__3 = cut + nnb + i__ - 1 + (invd + 1) * work_dim1;
			q__2.r = work[i__3].r * u01_i_j__.r - work[i__3].i * 
				u01_i_j__.i, q__2.i = work[i__3].r * 
				u01_i_j__.i + work[i__3].i * u01_i_j__.r;
			i__4 = cut + nnb + i__ - 1 + invd * work_dim1;
			q__3.r = work[i__4].r * u01_ip1_j__.r - work[i__4].i *
				 u01_ip1_j__.i, q__3.i = work[i__4].r * 
				u01_ip1_j__.i + work[i__4].i * u01_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    i__ += -2;
		}
	    }

/*        invD1*L11 */

	    i__ = nnb;
	    while(i__ >= 1) {
		if (ipiv[cut + i__] > 0) {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = u11 + i__ + j * work_dim1;
			i__3 = cut + i__ + invd * work_dim1;
			i__4 = u11 + i__ + j * work_dim1;
			q__1.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__1.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    --i__;
		} else {
		    i__1 = nnb;
		    for (j = 1; j <= i__1; ++j) {
			i__2 = u11 + i__ + j * work_dim1;
			u11_i_j__.r = work[i__2].r, u11_i_j__.i = work[i__2]
				.i;
			i__2 = u11 + i__ - 1 + j * work_dim1;
			u11_ip1_j__.r = work[i__2].r, u11_ip1_j__.i = work[
				i__2].i;
			i__2 = u11 + i__ + j * work_dim1;
			i__3 = cut + i__ + invd * work_dim1;
			i__4 = u11 + i__ + j * work_dim1;
			q__2.r = work[i__3].r * work[i__4].r - work[i__3].i * 
				work[i__4].i, q__2.i = work[i__3].r * work[
				i__4].i + work[i__3].i * work[i__4].r;
			i__5 = cut + i__ + (invd + 1) * work_dim1;
			q__3.r = work[i__5].r * u11_ip1_j__.r - work[i__5].i *
				 u11_ip1_j__.i, q__3.i = work[i__5].r * 
				u11_ip1_j__.i + work[i__5].i * u11_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
			i__2 = u11 + i__ - 1 + j * work_dim1;
			i__3 = cut + i__ - 1 + (invd + 1) * work_dim1;
			q__2.r = work[i__3].r * u11_i_j__.r - work[i__3].i * 
				u11_i_j__.i, q__2.i = work[i__3].r * 
				u11_i_j__.i + work[i__3].i * u11_i_j__.r;
			i__4 = cut + i__ - 1 + invd * work_dim1;
			q__3.r = work[i__4].r * u11_ip1_j__.r - work[i__4].i *
				 u11_ip1_j__.i, q__3.i = work[i__4].r * 
				u11_ip1_j__.i + work[i__4].i * u11_ip1_j__.r;
			q__1.r = q__2.r + q__3.r, q__1.i = q__2.i + q__3.i;
			work[i__2].r = q__1.r, work[i__2].i = q__1.i;
		    }
		    i__ += -2;
		}
	    }

/*       L11**H*invD1*L11->L11 */

	    i__1 = *n + *nb + 1;
	    ctrmm_("L", uplo, "C", "U", &nnb, &nnb, &c_b1, &a[cut + 1 + (cut 
		    + 1) * a_dim1], lda, &work[u11 + 1 + work_dim1], &i__1, (
		    ftnlen)1, (ftnlen)1, (ftnlen)1, (ftnlen)1);

	    i__1 = nnb;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = i__;
		for (j = 1; j <= i__2; ++j) {
		    i__3 = cut + i__ + (cut + j) * a_dim1;
		    i__4 = u11 + i__ + j * work_dim1;
		    a[i__3].r = work[i__4].r, a[i__3].i = work[i__4].i;
		}
	    }

	    if (cut + nnb < *n) {

/*          L21**H*invD2*L21->A(CUT+I,CUT+J) */

		i__1 = *n - nnb - cut;
		i__2 = *n + *nb + 1;
		i__3 = *n + *nb + 1;
		cgemm_("C", "N", &nnb, &nnb, &i__1, &c_b1, &a[cut + nnb + 1 + 
			(cut + 1) * a_dim1], lda, &work[work_offset], &i__2, &
			c_b2, &work[u11 + 1 + work_dim1], &i__3, (ftnlen)1, (
			ftnlen)1);

/*        L11 =  L11**H*invD1*L11 + U01**H*invD*U01 */

		i__1 = nnb;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    i__2 = i__;
		    for (j = 1; j <= i__2; ++j) {
			i__3 = cut + i__ + (cut + j) * a_dim1;
			i__4 = cut + i__ + (cut + j) * a_dim1;
			i__5 = u11 + i__ + j * work_dim1;
			q__1.r = a[i__4].r + work[i__5].r, q__1.i = a[i__4].i 
				+ work[i__5].i;
			a[i__3].r = q__1.r, a[i__3].i = q__1.i;
		    }
		}

/*        L01 =  L22**H*invD2*L21 */

		i__1 = *n - nnb - cut;
		i__2 = *n + *nb + 1;
		ctrmm_("L", uplo, "C", "U", &i__1, &nnb, &c_b1, &a[cut + nnb 
			+ 1 + (cut + nnb + 1) * a_dim1], lda, &work[
			work_offset], &i__2, (ftnlen)1, (ftnlen)1, (ftnlen)1, 
			(ftnlen)1);
/*      Update L21 */
		i__1 = *n - cut - nnb;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    i__2 = nnb;
		    for (j = 1; j <= i__2; ++j) {
			i__3 = cut + nnb + i__ + (cut + j) * a_dim1;
			i__4 = i__ + j * work_dim1;
			a[i__3].r = work[i__4].r, a[i__3].i = work[i__4].i;
		    }
		}
	    } else {

/*        L11 =  L11**H*invD1*L11 */

		i__1 = nnb;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    i__2 = i__;
		    for (j = 1; j <= i__2; ++j) {
			i__3 = cut + i__ + (cut + j) * a_dim1;
			i__4 = u11 + i__ + j * work_dim1;
			a[i__3].r = work[i__4].r, a[i__3].i = work[i__4].i;
		    }
		}
	    }

/*      Next Block */

	    cut += nnb;
	}

/*        Apply PERMUTATIONS P and P**H: P * inv(U**H)*inv(D)*inv(U) *P**H */

	i__ = *n;
	while(i__ >= 1) {
	    if (ipiv[i__] > 0) {
		ip = ipiv[i__];
		if (i__ < ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &i__, &ip, (ftnlen)
			    1);
		}
		if (i__ > ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &ip, &i__, (ftnlen)
			    1);
		}
	    } else {
		ip = -ipiv[i__];
		if (i__ < ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &i__, &ip, (ftnlen)
			    1);
		}
		if (i__ > ip) {
		    cheswapr_(uplo, n, &a[a_offset], lda, &ip, &i__, (ftnlen)
			    1);
		}
		--i__;
	    }
	    --i__;
	}
    }

    return 0;

/*     End of CHETRI2X */

} /* chetri2x_ */
