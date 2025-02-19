/* idzp_aid_subr_4.f -- translated by f2c (version 20160102).
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

/* Subroutine */ int idz_estrank0__(doublereal *eps, integer *m, integer *n, 
	doublecomplex *a, doublecomplex *w, integer *n2, integer *krank, 
	doublecomplex *ra, doublecomplex *rat, doublereal *scal)
{
    /* System generated locals */
    integer a_dim1, a_offset, ra_dim1, ra_offset, rat_dim1, rat_offset, i__1, 
	    i__2, i__3;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);
    double sqrt(doublereal), z_abs(doublecomplex *);

    /* Local variables */
    static integer ifrescal;
    static doublecomplex residual;
    static integer j, k;
    extern /* Subroutine */ int idz_house__(integer *, doublecomplex *, 
	    doublecomplex *, doublecomplex *, doublereal *);
    static doublereal ss;
    extern /* Subroutine */ int idz_houseapp__(integer *, doublecomplex *, 
	    doublecomplex *, integer *, doublereal *, doublecomplex *);
    static doublereal ssmax;
    static integer nulls;
    extern /* Subroutine */ int idz_transposer__(integer *, integer *, 
	    doublecomplex *, doublecomplex *), idz_frm__(integer *, integer *,
	     doublecomplex *, doublecomplex *, doublecomplex *);


/*       routine idz_estrank serves as a memory wrapper */
/*       for the present routine. (Please see routine idz_estrank */
/*       for further documentation.) */



/*       Apply the random matrix to every column of a, obtaining ra. */

    /* Parameter adjustments */
    --w;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --scal;
    rat_dim1 = *n;
    rat_offset = 1 + rat_dim1;
    rat -= rat_offset;
    ra_dim1 = *n2;
    ra_offset = 1 + ra_dim1;
    ra -= ra_offset;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	idz_frm__(m, n2, &w[1], &a[k * a_dim1 + 1], &ra[k * ra_dim1 + 1]);
    }


/*       Compute the sum of squares of the entries in each column of ra */
/*       and the maximum of all such sums. */

/* k */
    ssmax = 0.;

    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {

	ss = 0.;
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = j + k * a_dim1;
	    d_cnjg(&z__3, &a[j + k * a_dim1]);
	    z__2.r = a[i__3].r * z__3.r - a[i__3].i * z__3.i, z__2.i = a[i__3]
		    .r * z__3.i + a[i__3].i * z__3.r;
	    z__1.r = ss + z__2.r, z__1.i = z__2.i;
	    ss = z__1.r;
	}

/* j */
	if (ss > ssmax) {
	    ssmax = ss;
	}

    }


/*       Transpose ra to obtain rat. */

/* k */
    idz_transposer__(n2, n, &ra[ra_offset], &rat[rat_offset]);


    *krank = 0;
    nulls = 0;


/*       Loop until nulls = 7, krank+nulls = n2, or krank+nulls = n. */

L1000:


    if (*krank > 0) {

/*           Apply the previous Householder transformations */
/*           to rat(:,krank+1). */

	ifrescal = 0;

	i__1 = *krank;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = *n - k + 1;
	    idz_houseapp__(&i__2, &rat[k * rat_dim1 + 1], &rat[k + (*krank + 
		    1) * rat_dim1], &ifrescal, &scal[k], &rat[k + (*krank + 1)
		     * rat_dim1]);
	}

/* k */
    }


/*         Compute the Householder vector associated */
/*         with rat(krank+1:*,krank+1). */

/* krank .gt. 0 */
    i__1 = *n - *krank;
    idz_house__(&i__1, &rat[*krank + 1 + (*krank + 1) * rat_dim1], &residual, 
	    &rat[(*krank + 1) * rat_dim1 + 1], &scal[*krank + 1]);


    ++(*krank);
    if (z_abs(&residual) <= *eps * sqrt(ssmax)) {
	++nulls;
    }


    if (nulls < 7 && *krank + nulls < *n2 && *krank + nulls < *n) {
	goto L1000;
    }


    if (nulls < 7) {
	*krank = 0;
    }


    return 0;
} /* idz_estrank0__ */

