/* idzr_rsvd_subr_1.f -- translated by f2c (version 20160102).
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

/* Subroutine */ int idzr_rsvd0__(integer *m, integer *n, U_fp matveca, 
	doublecomplex *p1t, doublecomplex *p2t, doublecomplex *p3t, 
	doublecomplex *p4t, U_fp matvec, doublecomplex *p1, doublecomplex *p2,
	 doublecomplex *p3, doublecomplex *p4, integer *krank, doublecomplex *
	u, doublecomplex *v, doublereal *s, integer *ier, integer *list, 
	doublecomplex *proj, doublecomplex *col, doublecomplex *work)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i__1, i__2, i__3;

    /* Local variables */
    extern /* Subroutine */ int idzr_rid__(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *);
    static integer k;
    extern /* Subroutine */ int idz_id2svd__(integer *, integer *, 
	    doublecomplex *, integer *, integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublereal *, integer *, 
	    doublecomplex *), idz_getcols__(integer *, integer *, U_fp, 
	    doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *
	    , integer *, integer *, doublecomplex *, doublecomplex *);


/*       routine idzr_rsvd serves as a memory wrapper */
/*       for the present routine (please see routine idzr_rsvd */
/*       for further documentation). */



/*       ID a. */

/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    --list;
    --work;
    --col;
    --proj;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;

    /* Function Body */
    idzr_rid__(m, n, (U_fp)matveca, p1t, p2t, p3t, p4t, krank, &list[1], &
	    work[1]);


/*       Retrieve proj from work. */

    i__1 = *krank * (*n - *krank);
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	proj[i__2].r = work[i__3].r, proj[i__2].i = work[i__3].i;
    }


/*       Collect together the columns of a indexed by list into col. */

/* k */
    idz_getcols__(m, n, (U_fp)matvec, p1, p2, p3, p4, krank, &list[1], &col[1]
	    , &work[1]);


/*       Convert the ID to an SVD. */

    idz_id2svd__(m, krank, &col[1], n, &list[1], &proj[1], &u[u_offset], &v[
	    v_offset], &s[1], ier, &work[1]);


    return 0;
} /* idzr_rsvd0__ */

