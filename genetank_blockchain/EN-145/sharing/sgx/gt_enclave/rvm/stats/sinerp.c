/* sinerp.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "../../include/f2c.h"

/* Output from Public domain Ratfor, version 1.0 */
/* Subroutine */ int sinerp_(double *abd, int *ld4, int *nk, 
	double *p1ip, double *p2ip, int *ldnk, int *flag__)
{
    /* System generated locals */
    int abd_dim1, abd_offset, p1ip_dim1, p1ip_offset, p2ip_dim1, 
	    p2ip_offset, i__1;
    double d__1, d__2, d__3, d__4;

    /* Local variables */
    static int i__, j, k;
    static double c0, c1, c2, c3, wjm1[1], wjm2[2], wjm3[3];


/* Purpose :  Computes Inner Products between columns of L^{-1} */
/*            where L = abd is a Banded Matrix with 3 subdiagonals */
/* In R, only called in one place, from sslrvg() [ ./sslvrg.f ] with flag = 0 */
/* The algorithm works in two passes: */

/*               Pass 1 computes (cj,ck) k=j,j-1,j-2,j-3 ;  j=nk, .. 1 */
/*               Pass 2 computes (cj,ck) k <= j-4  (If flag == 1 ). */

/*               A refinement of Elden's trick is used. */
/* Args */
/* Locals */

/*     unnecessary initialization of c1 c2 c3 to keep g77 -Wall happy */

    /* Parameter adjustments */
    p1ip_dim1 = *ld4;
    p1ip_offset = 1 + p1ip_dim1;
    p1ip -= p1ip_offset;
    abd_dim1 = *ld4;
    abd_offset = 1 + abd_dim1;
    abd -= abd_offset;
    p2ip_dim1 = *ldnk;
    p2ip_offset = 1 + p2ip_dim1;
    p2ip -= p2ip_offset;

    /* Function Body */
    c1 = 0.;
    c2 = 0.;
    c3 = 0.;

/* Pass 1 */
    wjm3[0] = 0.;
    wjm3[1] = 0.;
    wjm3[2] = 0.;
    wjm2[0] = 0.;
    wjm2[1] = 0.;
    wjm1[0] = 0.;
    i__1 = *nk;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = *nk - i__ + 1;
	c0 = 1. / abd[j * abd_dim1 + 4];
	if (j <= *nk - 3) {
	    c1 = abd[(j + 3) * abd_dim1 + 1] * c0;
	    c2 = abd[(j + 2) * abd_dim1 + 2] * c0;
	    c3 = abd[(j + 1) * abd_dim1 + 3] * c0;
	} else if (j == *nk - 2) {
	    c1 = 0.;
	    c2 = abd[(j + 2) * abd_dim1 + 2] * c0;
	    c3 = abd[(j + 1) * abd_dim1 + 3] * c0;
	} else if (j == *nk - 1) {
	    c1 = 0.;
	    c2 = 0.;
	    c3 = abd[(j + 1) * abd_dim1 + 3] * c0;
	} else if (j == *nk) {
	    c1 = 0.;
	    c2 = 0.;
	    c3 = 0.;
	}
	p1ip[j * p1ip_dim1 + 1] = 0. - (c1 * wjm3[0] + c2 * wjm3[1] + c3 * 
		wjm3[2]);
	p1ip[j * p1ip_dim1 + 2] = 0. - (c1 * wjm3[1] + c2 * wjm2[0] + c3 * 
		wjm2[1]);
	p1ip[j * p1ip_dim1 + 3] = 0. - (c1 * wjm3[2] + c2 * wjm2[1] + c3 * 
		wjm1[0]);
/* Computing 2nd power */
	d__1 = c0;
/* Computing 2nd power */
	d__2 = c1;
/* Computing 2nd power */
	d__3 = c2;
/* Computing 2nd power */
	d__4 = c3;
	p1ip[j * p1ip_dim1 + 4] = d__1 * d__1 + d__2 * d__2 * wjm3[0] + c1 * 
		2. * c2 * wjm3[1] + c1 * 2. * c3 * wjm3[2] + d__3 * d__3 * 
		wjm2[0] + c2 * 2. * c3 * wjm2[1] + d__4 * d__4 * wjm1[0];
	wjm3[0] = wjm2[0];
	wjm3[1] = wjm2[1];
	wjm3[2] = p1ip[j * p1ip_dim1 + 2];
	wjm2[0] = wjm1[0];
	wjm2[1] = p1ip[j * p1ip_dim1 + 3];
	wjm1[0] = p1ip[j * p1ip_dim1 + 4];
/* L100: */
    }
    if (*flag__ != 0) {
/*     ____ Pass 2 _____  Compute p2ip  [never from R's code!] */
	i__1 = *nk;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    j = *nk - i__ + 1;
/*           for(k=1;k<=4 & j+k-1<=nk;k=k+1) { p2ip(.) = .. }: */
	    for (k = 1; k <= 4; ++k) {
		if (j + k - 1 > *nk) {
		    goto L120;
		}
		p2ip[j + (j + k - 1) * p2ip_dim1] = p1ip[5 - k + j * 
			p1ip_dim1];
	    }
L120:
	    ;
	}
	i__1 = *nk;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    j = *nk - i__ + 1;
/*           for(k=j-4;k>=1;k=k-1){ */
	    if (j - 4 >= 1) {
		for (k = j - 4; k >= 1; --k) {
		    c0 = 1. / abd[k * abd_dim1 + 4];
		    c1 = abd[(k + 3) * abd_dim1 + 1] * c0;
		    c2 = abd[(k + 2) * abd_dim1 + 2] * c0;
		    c3 = abd[(k + 1) * abd_dim1 + 3] * c0;
		    p2ip[k + j * p2ip_dim1] = 0. - (c1 * p2ip[k + 3 + j * 
			    p2ip_dim1] + c2 * p2ip[k + 2 + j * p2ip_dim1] + 
			    c3 * p2ip[k + 1 + j * p2ip_dim1]);
		}
	    }
/* L170: */
	}
    }
    return 0;
} /* sinerp_ */

