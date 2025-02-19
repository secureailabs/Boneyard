#include "linalg.h"
/* iddr_asvd.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "../../../../../../../include/f2c.h"

/*       this file contains the following user-callable routines: */


/*       routine iddr_aid computes the SVD, to a specified rank, */
/*       of an arbitrary matrix. This routine is randomized. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int iddr_asvd_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *w, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, v_dim1, v_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int iddr_asvd0_(integer *, integer *, doublereal 
	    *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, doublereal *, doublereal *,
	     doublereal *);
    static integer lw, icol, lcol, iproj, ilist, lproj, llist, iwork, lwork, 
	    iwinit, lwinit;


/*       constructs a rank-krank SVD  u diag(s) v^T  approximating a, */
/*       where u is an m x krank matrix whose columns are orthonormal, */
/*       v is an n x krank matrix whose columns are orthonormal, */
/*       and diag(s) is a diagonal krank x krank matrix whose entries */
/*       are all nonnegative. This routine uses a randomized algorithm. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       a -- matrix to be decomposed; the present routine does not */
/*            alter a */
/*       krank -- rank of the SVD being constructed */
/*       w -- initialization array that routine iddr_aidi */
/*            has constructed (for use in the present routine, w must */
/*            be at least (2*krank+28)*m+(6*krank+21)*n+25*krank**2+100 */
/*            real*8 elements long) */

/*       output: */
/*       u -- matrix of orthonormal left singular vectors of a */
/*       v -- matrix of orthonormal right singular vectors of a */
/*       s -- array of singular values of a */
/*       ier -- 0 when the routine terminates successfully; */
/*              nonzero otherwise */

/*       _N.B._: The algorithm used by this routine is randomized. */



/*       Allocate memory in w. */

/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --w;

    /* Function Body */
    lw = 0;

    iwinit = lw + 1;
    lwinit = ((*krank << 1) + 17) * *n + *m * 27 + 100;
    lw += lwinit;

    ilist = lw + 1;
    llist = *n;
    lw += llist;

    iproj = lw + 1;
    lproj = *krank * (*n - *krank);
    lw += lproj;

    icol = lw + 1;
    lcol = *m * *krank;
    lw += lcol;

    iwork = lw + 1;
/* Computing 2nd power */
    i__1 = *krank;
    lwork = (*krank + 1) * (*m + *n * 3) + i__1 * i__1 * 26;
    lw += lwork;


    iddr_asvd0_(m, n, &a[a_offset], krank, &w[iwinit], &u[u_offset], &v[
	    v_offset], &s[1], ier, (integer*)&w[ilist], &w[iproj], &w[icol], &w[iwork]);


    return 0;
} /* iddr_asvd__ */





/* Subroutine */ int iddr_asvd0_(integer *m, integer *n, doublereal *a, 
	integer *krank, doublereal *winit, doublereal *u, doublereal *v, 
	doublereal *s, integer *ier, integer *list, doublereal *proj, 
	doublereal *col, doublereal *work)
{
    /* System generated locals */
    integer a_dim1, a_offset, u_dim1, u_offset, v_dim1, v_offset, proj_dim1, 
	    proj_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int iddr_aid_(integer *, integer *, doublereal *,
	     integer *, doublereal *, integer *, doublereal *), idd_id2svd_(
	    integer *, integer *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     doublereal *), idd_copycols_(integer *, integer *, doublereal *,
	     integer *, integer *, doublereal *);


/*       routine iddr_asvd serves as a memory wrapper */
/*       for the present routine (please see routine iddr_asvd */
/*       for further documentation). */



/*       ID a. */

/* Computing 2nd power */
    i__1 = *krank;
    /* Parameter adjustments */
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --work;
    --col;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;
    --s;
    v_dim1 = *n;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *m;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --winit;

    /* Function Body */
    iddr_aid_(m, n, &a[a_offset], krank, &winit[1], &list[1], &proj[
	    proj_offset]);


/*       Collect together the columns of a indexed by list into col. */

    idd_copycols_(m, n, &a[a_offset], krank, &list[1], &col[1]);


/*       Convert the ID to an SVD. */

    idd_id2svd_(m, krank, &col[1], n, &list[1], &proj[proj_offset], &u[
	    u_offset], &v[v_offset], &s[1], ier, &work[1]);


    return 0;
} /* iddr_asvd0__ */

