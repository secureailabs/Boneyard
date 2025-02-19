#include "linalg.h"
/* idzr_aid.f -- translated by f2c (version 20160102).
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


/*       routine idzr_aid computes the ID, to a specified rank, */
/*       of an arbitrary matrix. This routine is randomized. */

/*       routine idzr_aidi initializes routine idzr_aid. */


/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */




/* Subroutine */ int idzr_aid_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *w, integer *list, doublecomplex *proj)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern /* Subroutine */ int idzr_aid0_(integer *, integer *, 
	    doublecomplex *, integer *, doublecomplex *, integer *, 
	    doublecomplex *, doublecomplex *);
    static integer ir, lr, iw, lw, lw2;


/*       computes the ID of the matrix a, i.e., lists in list */
/*       the indices of krank columns of a such that */

/*       a(j,list(k))  =  a(j,list(k)) */

/*       for all j = 1, ..., m; k = 1, ..., krank, and */

/*                       min(m,n,krank) */
/*       a(j,list(k))  =     Sigma      a(j,list(l)) * proj(l,k-krank)(*) */
/*                            l=1 */

/*                     +  epsilon(j,k-krank) */

/*       for all j = 1, ..., m; k = krank+1, ..., n, */

/*       for some matrix epsilon, dimensioned epsilon(m,n-krank), */
/*       whose norm is (hopefully) minimized by the pivoting procedure. */

/*       input: */
/*       m -- number of rows in a */
/*       n -- number of columns in a */
/*       a -- matrix to be ID'd; the present routine does not alter a */
/*       krank -- rank of the ID to be constructed */
/*       w -- initialization array that routine idzr_aidi */
/*            has constructed */

/*       output: */
/*       list -- indices of the columns in the ID */
/*       proj -- matrix of coefficients needed to interpolate */
/*               from the selected columns to the other columns */
/*               in the original matrix being ID'd */

/*       _N.B._: The algorithm used by this routine is randomized. */

/*       reference: */
/*       Halko, Martinsson, Tropp, "Finding structure with randomness: */
/*            probabilistic algorithms for constructing approximate */
/*            matrix decompositions," SIAM Review, 53 (2): 217-288, */
/*            2011. */



/*       Allocate memory in w. */

    /* Parameter adjustments */
    --list;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --proj;
    --w;

    /* Function Body */
    lw = 0;

    iw = lw + 1;
    lw2 = *m * 21 + 80 + *n;
    lw += lw2;

    ir = lw + 1;
    lr = (*krank + 8 << 1) * *n;
    lw += lr;


    idzr_aid0_(m, n, &a[a_offset], krank, &w[iw], &list[1], &proj[1], &w[ir])
	    ;


    return 0;
} /* idzr_aid__ */





/* Subroutine */ int idzr_aid0_(integer *m, integer *n, doublecomplex *a, 
	integer *krank, doublecomplex *w, integer *list, doublecomplex *proj, 
	doublecomplex *r__)
{
    /* System generated locals */
    integer a_dim1, a_offset, r_dim1, r_offset, proj_dim1, proj_offset, i__1;

    /* Local variables */
    extern /* Subroutine */ int idz_sfrm_(integer *, integer *, integer *, 
	    doublecomplex *, doublecomplex *, doublecomplex *);
    static integer k, l, n2, mn, lproj;
    extern /* Subroutine */ int idzr_copyzarr_(integer *, doublecomplex *, 
	    doublecomplex *), idzr_id_(integer *, integer *, doublecomplex *,
	     integer *, integer *, doublereal *);


/*       routine idzr_aid serves as a memory wrapper */
/*       for the present routine */
/*       (see idzr_aid for further documentation). */


/*       Please note that the second dimension of r is 2*n */
/*       (instead of n) so that if krank+8 >= m/2, then */
/*       we can copy the whole of a into r. */


/*       Retrieve the number of random test vectors */
/*       and the greatest integer less than m that is */
/*       a positive integer power of two. */

    /* Parameter adjustments */
    --list;
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    r_dim1 = *krank + 8;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    proj_dim1 = *krank;
    proj_offset = 1 + proj_dim1;
    proj -= proj_offset;

    /* Function Body */
    l = (integer) w[1].r;
    n2 = (integer) w[2].r;


    if (l < n2 && l <= *m) {

/*         Apply the random matrix. */

	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    idz_sfrm_(&l, m, &n2, &w[11], &a[k * a_dim1 + 1], &r__[k * 
		    r_dim1 + 1]);
	}

/*         ID r. */

/* k */
	idzr_id_(&l, n, &r__[r_offset], krank, &list[1], (doublereal*)&w[*m * 20 + 81]);

/*         Retrieve proj from r. */

	lproj = *krank * (*n - *krank);
	idzr_copyzarr_(&lproj, &r__[r_offset], &proj[proj_offset]);

    }


    if (l >= n2 || l > *m) {

/*         ID a directly. */

	mn = *m * *n;
	idzr_copyzarr_(&mn, &a[a_offset], &r__[r_offset]);
	idzr_id_(m, n, &r__[r_offset], krank, &list[1], (doublereal*)&w[*m * 20 + 81]);

/*         Retrieve proj from r. */

	lproj = *krank * (*n - *krank);
	idzr_copyzarr_(&lproj, &r__[r_offset], &proj[proj_offset]);

    }


    return 0;
} /* idzr_aid0__ */





/* Subroutine */ int idzr_copyzarr_(integer *n, doublecomplex *a, 
	doublecomplex *b)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer k;


/*       copies a into b. */

/*       input: */
/*       n -- length of a and b */
/*       a -- array to copy into b */

/*       output: */
/*       b -- copy of a */



    /* Parameter adjustments */
    --b;
    --a;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	i__3 = k;
	b[i__2].r = a[i__3].r, b[i__2].i = a[i__3].i;
    }


/* k */
    return 0;
} /* idzr_copyzarr__ */





/* Subroutine */ int idzr_aidi_(integer *m, integer *n, integer *krank, 
	doublecomplex *w)
{
    static integer l, n2;
    extern /* Subroutine */ int idz_sfrmi_(integer *, integer *, integer *, 
	    doublecomplex *);


/*       initializes the array w for using routine idzr_aid. */

/*       input: */
/*       m -- number of rows in the matrix to be ID'd */
/*       n -- number of columns in the matrix to be ID'd */
/*       krank -- rank of the ID to be constructed */

/*       output: */
/*       w -- initialization array for using routine idzr_aid */



/*       Set the number of random test vectors to 8 more than the rank. */

    /* Parameter adjustments */
    --w;

    /* Function Body */
    l = *krank + 8;
    w[1].r = (doublereal) l, w[1].i = 0.;


/*       Initialize the rest of the array w. */

    n2 = 0;
    if (l <= *m) {
	idz_sfrmi_(&l, m, &n2, &w[11]);
    }
    w[2].r = (doublereal) n2, w[2].i = 0.;


    return 0;
} /* idzr_aidi__ */

