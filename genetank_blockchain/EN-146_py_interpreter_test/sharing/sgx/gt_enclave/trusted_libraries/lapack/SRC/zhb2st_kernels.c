#include "blas.h"
#include "clapack.h"
/* zhb2st_kernels.f -- translated by f2c (version 20160102).
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

/* > \brief \b ZHB2ST_KERNELS */

/*  @precisions fortran z -> s d c */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download ZHB2ST_KERNELS + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/zhb2st_
kernels.f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/zhb2st_
kernels.f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/zhb2st_
kernels.f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE  ZHB2ST_KERNELS( UPLO, WANTZ, TTYPE, */
/*                                   ST, ED, SWEEP, N, NB, IB, */
/*                                   A, LDA, V, TAU, LDVT, WORK) */

/*       IMPLICIT NONE */

/*       .. Scalar Arguments .. */
/*       CHARACTER          UPLO */
/*       LOGICAL            WANTZ */
/*       INTEGER            TTYPE, ST, ED, SWEEP, N, NB, IB, LDA, LDVT */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX*16         A( LDA, * ), V( * ), */
/*                          TAU( * ), WORK( * ) */

/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > ZHB2ST_KERNELS is an internal routine used by the ZHETRD_HB2ST */
/* > subroutine. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] UPLO */
/* > \verbatim */
/* >          UPLO is CHARACTER*1 */
/* > \endverbatim */
/* > */
/* > \param[in] WANTZ */
/* > \verbatim */
/* >          WANTZ is LOGICAL which indicate if Eigenvalue are requested or both */
/* >          Eigenvalue/Eigenvectors. */
/* > \endverbatim */
/* > */
/* > \param[in] TTYPE */
/* > \verbatim */
/* >          TTYPE is INTEGER */
/* > \endverbatim */
/* > */
/* > \param[in] ST */
/* > \verbatim */
/* >          ST is INTEGER */
/* >          internal parameter for indices. */
/* > \endverbatim */
/* > */
/* > \param[in] ED */
/* > \verbatim */
/* >          ED is INTEGER */
/* >          internal parameter for indices. */
/* > \endverbatim */
/* > */
/* > \param[in] SWEEP */
/* > \verbatim */
/* >          SWEEP is INTEGER */
/* >          internal parameter for indices. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER. The order of the matrix A. */
/* > \endverbatim */
/* > */
/* > \param[in] NB */
/* > \verbatim */
/* >          NB is INTEGER. The size of the band. */
/* > \endverbatim */
/* > */
/* > \param[in] IB */
/* > \verbatim */
/* >          IB is INTEGER. */
/* > \endverbatim */
/* > */
/* > \param[in, out] A */
/* > \verbatim */
/* >          A is COMPLEX*16 array. A pointer to the matrix A. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER. The leading dimension of the matrix A. */
/* > \endverbatim */
/* > */
/* > \param[out] V */
/* > \verbatim */
/* >          V is COMPLEX*16 array, dimension 2*n if eigenvalues only are */
/* >          requested or to be queried for vectors. */
/* > \endverbatim */
/* > */
/* > \param[out] TAU */
/* > \verbatim */
/* >          TAU is COMPLEX*16 array, dimension (2*n). */
/* >          The scalar factors of the Householder reflectors are stored */
/* >          in this array. */
/* > \endverbatim */
/* > */
/* > \param[in] LDVT */
/* > \verbatim */
/* >          LDVT is INTEGER. */
/* > \endverbatim */
/* > */
/* > \param[in] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX*16 array. Workspace of size nb. */
/* > \endverbatim */
/* > */
/* > \par Further Details: */
/*  ===================== */
/* > */
/* > \verbatim */
/* > */
/* >  Implemented by Azzam Haidar. */
/* > */
/* >  All details are available on technical report, SC11, SC13 papers. */
/* > */
/* >  Azzam Haidar, Hatem Ltaief, and Jack Dongarra. */
/* >  Parallel reduction to condensed forms for symmetric eigenvalue problems */
/* >  using aggregated fine-grained and memory-aware kernels. In Proceedings */
/* >  of 2011 International Conference for High Performance Computing, */
/* >  Networking, Storage and Analysis (SC '11), New York, NY, USA, */
/* >  Article 8 , 11 pages. */
/* >  http://doi.acm.org/10.1145/2063384.2063394 */
/* > */
/* >  A. Haidar, J. Kurzak, P. Luszczek, 2013. */
/* >  An improved parallel singular value algorithm and its implementation */
/* >  for multicore hardware, In Proceedings of 2013 International Conference */
/* >  for High Performance Computing, Networking, Storage and Analysis (SC '13). */
/* >  Denver, Colorado, USA, 2013. */
/* >  Article 90, 12 pages. */
/* >  http://doi.acm.org/10.1145/2503210.2503292 */
/* > */
/* >  A. Haidar, R. Solca, S. Tomov, T. Schulthess and J. Dongarra. */
/* >  A novel hybrid CPU-GPU generalized eigensolver for electronic structure */
/* >  calculations based on fine-grained memory aware tasks. */
/* >  International Journal of High Performance Computing Applications. */
/* >  Volume 28 Issue 2, Pages 196-209, May 2014. */
/* >  http://hpc.sagepub.com/content/28/2/196 */
/* > */
/* > \endverbatim */
/* > */
/*  ===================================================================== */
/* Subroutine */ int zhb2st_kernels__(char *uplo, logical *wantz, integer *
	ttype, integer *st, integer *ed, integer *sweep, integer *n, integer *
	nb, integer *ib, doublecomplex *a, integer *lda, doublecomplex *v, 
	doublecomplex *tau, integer *ldvt, doublecomplex *work, ftnlen 
	uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublecomplex z__1;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    static integer i__, j1, j2, lm, ln;
    static doublecomplex ctmp;
    static integer dpos, vpos;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static logical upper;
    static integer ajeter;
    extern /* Subroutine */ int zlarfg_(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublecomplex *);
    static integer ofdpos;
    extern /* Subroutine */ int zlarfx_(char *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *, integer *, 
	    doublecomplex *, ftnlen), zlarfy_(char *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, doublecomplex *, 
	    integer *, doublecomplex *, ftnlen);
    static integer taupos;



/*  -- LAPACK computational routine (version 3.7.1) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     June 2017 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. External Functions .. */
/*     .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --v;
    --tau;
    --work;

    /* Function Body */
    ajeter = *ib + *ldvt;
    upper = lsame_(uplo, "U", (ftnlen)1, (ftnlen)1);
    if (upper) {
	dpos = (*nb << 1) + 1;
	ofdpos = *nb << 1;
    } else {
	dpos = 1;
	ofdpos = 2;
    }

/*     Upper case */

    if (upper) {

	if (*wantz) {
	    vpos = (*sweep - 1) % 2 * *n + *st;
	    taupos = (*sweep - 1) % 2 * *n + *st;
	} else {
	    vpos = (*sweep - 1) % 2 * *n + *st;
	    taupos = (*sweep - 1) % 2 * *n + *st;
	}

	if (*ttype == 1) {
	    lm = *ed - *st + 1;

	    i__1 = vpos;
	    v[i__1].r = 1., v[i__1].i = 0.;
	    i__1 = lm - 1;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = vpos + i__;
		d_cnjg(&z__1, &a[ofdpos - i__ + (*st + i__) * a_dim1]);
		v[i__2].r = z__1.r, v[i__2].i = z__1.i;
		i__2 = ofdpos - i__ + (*st + i__) * a_dim1;
		a[i__2].r = 0., a[i__2].i = 0.;
/* L10: */
	    }
	    d_cnjg(&z__1, &a[ofdpos + *st * a_dim1]);
	    ctmp.r = z__1.r, ctmp.i = z__1.i;
	    zlarfg_(&lm, &ctmp, &v[vpos + 1], &c__1, &tau[taupos]);
	    i__1 = ofdpos + *st * a_dim1;
	    a[i__1].r = ctmp.r, a[i__1].i = ctmp.i;

	    lm = *ed - *st + 1;
	    d_cnjg(&z__1, &tau[taupos]);
	    i__1 = *lda - 1;
	    zlarfy_(uplo, &lm, &v[vpos], &c__1, &z__1, &a[dpos + *st * a_dim1]
		    , &i__1, &work[1], (ftnlen)1);
	}

	if (*ttype == 3) {

	    lm = *ed - *st + 1;
	    d_cnjg(&z__1, &tau[taupos]);
	    i__1 = *lda - 1;
	    zlarfy_(uplo, &lm, &v[vpos], &c__1, &z__1, &a[dpos + *st * a_dim1]
		    , &i__1, &work[1], (ftnlen)1);
	}

	if (*ttype == 2) {
	    j1 = *ed + 1;
/* Computing MIN */
	    i__1 = *ed + *nb;
	    j2 = min(i__1,*n);
	    ln = *ed - *st + 1;
	    lm = j2 - j1 + 1;
	    if (lm > 0) {
		d_cnjg(&z__1, &tau[taupos]);
		i__1 = *lda - 1;
		zlarfx_("Left", &ln, &lm, &v[vpos], &z__1, &a[dpos - *nb + j1 
			* a_dim1], &i__1, &work[1], (ftnlen)4);

		if (*wantz) {
		    vpos = (*sweep - 1) % 2 * *n + j1;
		    taupos = (*sweep - 1) % 2 * *n + j1;
		} else {
		    vpos = (*sweep - 1) % 2 * *n + j1;
		    taupos = (*sweep - 1) % 2 * *n + j1;
		}

		i__1 = vpos;
		v[i__1].r = 1., v[i__1].i = 0.;
		i__1 = lm - 1;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    i__2 = vpos + i__;
		    d_cnjg(&z__1, &a[dpos - *nb - i__ + (j1 + i__) * a_dim1]);
		    v[i__2].r = z__1.r, v[i__2].i = z__1.i;
		    i__2 = dpos - *nb - i__ + (j1 + i__) * a_dim1;
		    a[i__2].r = 0., a[i__2].i = 0.;
/* L30: */
		}
		d_cnjg(&z__1, &a[dpos - *nb + j1 * a_dim1]);
		ctmp.r = z__1.r, ctmp.i = z__1.i;
		zlarfg_(&lm, &ctmp, &v[vpos + 1], &c__1, &tau[taupos]);
		i__1 = dpos - *nb + j1 * a_dim1;
		a[i__1].r = ctmp.r, a[i__1].i = ctmp.i;

		i__1 = ln - 1;
		i__2 = *lda - 1;
		zlarfx_("Right", &i__1, &lm, &v[vpos], &tau[taupos], &a[dpos 
			- *nb + 1 + j1 * a_dim1], &i__2, &work[1], (ftnlen)5);
	    }
	}

/*     Lower case */

    } else {

	if (*wantz) {
	    vpos = (*sweep - 1) % 2 * *n + *st;
	    taupos = (*sweep - 1) % 2 * *n + *st;
	} else {
	    vpos = (*sweep - 1) % 2 * *n + *st;
	    taupos = (*sweep - 1) % 2 * *n + *st;
	}

	if (*ttype == 1) {
	    lm = *ed - *st + 1;

	    i__1 = vpos;
	    v[i__1].r = 1., v[i__1].i = 0.;
	    i__1 = lm - 1;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		i__2 = vpos + i__;
		i__3 = ofdpos + i__ + (*st - 1) * a_dim1;
		v[i__2].r = a[i__3].r, v[i__2].i = a[i__3].i;
		i__2 = ofdpos + i__ + (*st - 1) * a_dim1;
		a[i__2].r = 0., a[i__2].i = 0.;
/* L20: */
	    }
	    zlarfg_(&lm, &a[ofdpos + (*st - 1) * a_dim1], &v[vpos + 1], &c__1,
		     &tau[taupos]);

	    lm = *ed - *st + 1;

	    d_cnjg(&z__1, &tau[taupos]);
	    i__1 = *lda - 1;
	    zlarfy_(uplo, &lm, &v[vpos], &c__1, &z__1, &a[dpos + *st * a_dim1]
		    , &i__1, &work[1], (ftnlen)1);
	}

	if (*ttype == 3) {
	    lm = *ed - *st + 1;

	    d_cnjg(&z__1, &tau[taupos]);
	    i__1 = *lda - 1;
	    zlarfy_(uplo, &lm, &v[vpos], &c__1, &z__1, &a[dpos + *st * a_dim1]
		    , &i__1, &work[1], (ftnlen)1);
	}

	if (*ttype == 2) {
	    j1 = *ed + 1;
/* Computing MIN */
	    i__1 = *ed + *nb;
	    j2 = min(i__1,*n);
	    ln = *ed - *st + 1;
	    lm = j2 - j1 + 1;

	    if (lm > 0) {
		i__1 = *lda - 1;
		zlarfx_("Right", &lm, &ln, &v[vpos], &tau[taupos], &a[dpos + *
			nb + *st * a_dim1], &i__1, &work[1], (ftnlen)5);

		if (*wantz) {
		    vpos = (*sweep - 1) % 2 * *n + j1;
		    taupos = (*sweep - 1) % 2 * *n + j1;
		} else {
		    vpos = (*sweep - 1) % 2 * *n + j1;
		    taupos = (*sweep - 1) % 2 * *n + j1;
		}

		i__1 = vpos;
		v[i__1].r = 1., v[i__1].i = 0.;
		i__1 = lm - 1;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    i__2 = vpos + i__;
		    i__3 = dpos + *nb + i__ + *st * a_dim1;
		    v[i__2].r = a[i__3].r, v[i__2].i = a[i__3].i;
		    i__2 = dpos + *nb + i__ + *st * a_dim1;
		    a[i__2].r = 0., a[i__2].i = 0.;
/* L40: */
		}
		zlarfg_(&lm, &a[dpos + *nb + *st * a_dim1], &v[vpos + 1], &
			c__1, &tau[taupos]);

		i__1 = ln - 1;
		d_cnjg(&z__1, &tau[taupos]);
		i__2 = *lda - 1;
		zlarfx_("Left", &lm, &i__1, &v[vpos], &z__1, &a[dpos + *nb - 
			1 + (*st + 1) * a_dim1], &i__2, &work[1], (ftnlen)4);
	    }
	}
    }

    return 0;

/*     END OF ZHB2ST_KERNELS */

} /* zhb2st_kernels__ */
