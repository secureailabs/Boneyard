#include "blas.h"
#include "clapack.h"
/* clatrz.f -- translated by f2c (version 20160102).
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

/* > \brief \b CLATRZ factors an upper trapezoidal matrix by means of unitary transformations. */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download CLATRZ + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/clatrz.
f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/clatrz.
f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/clatrz.
f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       SUBROUTINE CLATRZ( M, N, L, A, LDA, TAU, WORK ) */

/*       .. Scalar Arguments .. */
/*       INTEGER            L, LDA, M, N */
/*       .. */
/*       .. Array Arguments .. */
/*       COMPLEX            A( LDA, * ), TAU( * ), WORK( * ) */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > CLATRZ factors the M-by-(M+L) complex upper trapezoidal matrix */
/* > [ A1 A2 ] = [ A(1:M,1:M) A(1:M,N-L+1:N) ] as ( R  0 ) * Z by means */
/* > of unitary transformations, where  Z is an (M+L)-by-(M+L) unitary */
/* > matrix and, R and A1 are M-by-M upper triangular matrices. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] M */
/* > \verbatim */
/* >          M is INTEGER */
/* >          The number of rows of the matrix A.  M >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] N */
/* > \verbatim */
/* >          N is INTEGER */
/* >          The number of columns of the matrix A.  N >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in] L */
/* > \verbatim */
/* >          L is INTEGER */
/* >          The number of columns of the matrix A containing the */
/* >          meaningful part of the Householder vectors. N-M >= L >= 0. */
/* > \endverbatim */
/* > */
/* > \param[in,out] A */
/* > \verbatim */
/* >          A is COMPLEX array, dimension (LDA,N) */
/* >          On entry, the leading M-by-N upper trapezoidal part of the */
/* >          array A must contain the matrix to be factorized. */
/* >          On exit, the leading M-by-M upper triangular part of A */
/* >          contains the upper triangular matrix R, and elements N-L+1 to */
/* >          N of the first M rows of A, with the array TAU, represent the */
/* >          unitary matrix Z as a product of M elementary reflectors. */
/* > \endverbatim */
/* > */
/* > \param[in] LDA */
/* > \verbatim */
/* >          LDA is INTEGER */
/* >          The leading dimension of the array A.  LDA >= max(1,M). */
/* > \endverbatim */
/* > */
/* > \param[out] TAU */
/* > \verbatim */
/* >          TAU is COMPLEX array, dimension (M) */
/* >          The scalar factors of the elementary reflectors. */
/* > \endverbatim */
/* > */
/* > \param[out] WORK */
/* > \verbatim */
/* >          WORK is COMPLEX array, dimension (M) */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date December 2016 */

/* > \ingroup complexOTHERcomputational */

/* > \par Contributors: */
/*  ================== */
/* > */
/* >    A. Petitet, Computer Science Dept., Univ. of Tenn., Knoxville, USA */

/* > \par Further Details: */
/*  ===================== */
/* > */
/* > \verbatim */
/* > */
/* >  The factorization is obtained by Householder's method.  The kth */
/* >  transformation matrix, Z( k ), which is used to introduce zeros into */
/* >  the ( m - k + 1 )th row of A, is given in the form */
/* > */
/* >     Z( k ) = ( I     0   ), */
/* >              ( 0  T( k ) ) */
/* > */
/* >  where */
/* > */
/* >     T( k ) = I - tau*u( k )*u( k )**H,   u( k ) = (   1    ), */
/* >                                                 (   0    ) */
/* >                                                 ( z( k ) ) */
/* > */
/* >  tau is a scalar and z( k ) is an l element vector. tau and z( k ) */
/* >  are chosen to annihilate the elements of the kth row of A2. */
/* > */
/* >  The scalar tau is returned in the kth element of TAU and the vector */
/* >  u( k ) in the kth row of A2, such that the elements of z( k ) are */
/* >  in  a( k, l + 1 ), ..., a( k, n ). The elements of R are returned in */
/* >  the upper triangular part of A1. */
/* > */
/* >  Z is given by */
/* > */
/* >     Z =  Z( 1 ) * Z( 2 ) * ... * Z( m ). */
/* > \endverbatim */
/* > */
/*  ===================================================================== */
/* Subroutine */ int clatrz_(integer *m, integer *n, integer *l, complex *a, 
	integer *lda, complex *tau, complex *work)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    complex q__1;

    /* Builtin functions */
    void r_cnjg(complex *, complex *);

    /* Local variables */
    static integer i__;
    static complex alpha;
    extern /* Subroutine */ int clarz_(char *, integer *, integer *, integer *
	    , complex *, integer *, complex *, complex *, integer *, complex *
	    , ftnlen), clarfg_(integer *, complex *, complex *, integer *, 
	    complex *), clacgv_(integer *, complex *, integer *);


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
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Quick return if possible */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --tau;
    --work;

    /* Function Body */
    if (*m == 0) {
	return 0;
    } else if (*m == *n) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = i__;
	    tau[i__2].r = 0.f, tau[i__2].i = 0.f;
/* L10: */
	}
	return 0;
    }

    for (i__ = *m; i__ >= 1; --i__) {

/*        Generate elementary reflector H(i) to annihilate */
/*        [ A(i,i) A(i,n-l+1:n) ] */

	clacgv_(l, &a[i__ + (*n - *l + 1) * a_dim1], lda);
	r_cnjg(&q__1, &a[i__ + i__ * a_dim1]);
	alpha.r = q__1.r, alpha.i = q__1.i;
	i__1 = *l + 1;
	clarfg_(&i__1, &alpha, &a[i__ + (*n - *l + 1) * a_dim1], lda, &tau[
		i__]);
	i__1 = i__;
	r_cnjg(&q__1, &tau[i__]);
	tau[i__1].r = q__1.r, tau[i__1].i = q__1.i;

/*        Apply H(i) to A(1:i-1,i:n) from the right */

	i__1 = i__ - 1;
	i__2 = *n - i__ + 1;
	r_cnjg(&q__1, &tau[i__]);
	clarz_("Right", &i__1, &i__2, l, &a[i__ + (*n - *l + 1) * a_dim1], 
		lda, &q__1, &a[i__ * a_dim1 + 1], lda, &work[1], (ftnlen)5);
	i__1 = i__ + i__ * a_dim1;
	r_cnjg(&q__1, &alpha);
	a[i__1].r = q__1.r, a[i__1].i = q__1.i;

/* L20: */
    }

    return 0;

/*     End of CLATRZ */

} /* clatrz_ */
