/* dqrls.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include <R_ext/Linpack.h>
#include <R_ext/Applic.h>
#include "../../include/f2c.h"

/* Table of constant values */

static integer c__1110 = 1110;


/*     dqrfit is a subroutine to compute least squares solutions */
/*     to the system */

/*     (1)               x * b = y */

/*     which may be either under-determined or over-determined. */
/*     the user must supply a tolerance to limit the columns of */
/*     x used in computing the solution.  in effect, a set of */
/*     columns with a condition number approximately bounded by */
/*     1/tol is used, the other components of b being set to zero. */

/*     on entry */

/*        x      double precision(n,p). */
/*               x contains n-by-p coefficient matrix of */
/*               the system (1), x is destroyed by dqrfit. */

/*        n      the number of rows of the matrix x. */

/*        p      the number of columns of the matrix x. */

/*        y      double precision(n,ny) */
/*               y contains the right hand side(s) of the system (1). */

/*        ny     the number of right hand sides of the system (1). */

/*        tol    double precision */
/*               tol is the nonnegative tolerance used to */
/*               determine the subset of columns of x included */
/*               in the solution.  columns are pivoted out of */
/*               decomposition if */

/*        jpvt   integer(p) */
/*               the values in jpvt are permuted in the same */
/*               way as the columns of x.  this can be useful */
/*               in unscrambling coefficients etc. */

/*        work   double precision(2*p) */
/*               work is an array used by dqrdc2 and dqrsl. */

/*     on return */

/*        x      contains the output array from dqrdc2. */
/*               namely the qr decomposition of x stored in */
/*               compact form. */

/*        b      double precision(p,ny) */
/*               b contains the solution vectors with rows permuted */
/*               in the same way as the columns of x.  components */
/*               corresponding to columns not used are set to zero. */

/*        rsd    double precision(n,ny) */
/*               rsd contains the residual vectors y-x*b. */

/*        qty    double precision(n,ny)     t */
/*               qty contains the vectors  q y.   note that */
/*               the initial p elements of this vector are */
/*               permuted in the same way as the columns of x. */

/*        k      integer */
/*               k contains the number of columns used in the */
/*               solution. */

/*        jpvt   has its contents permuted as described above. */

/*        qraux  double precision(p) */
/*               qraux contains auxiliary information on the */
/*               qr decomposition of x. */


/*     on return the arrays x, jpvt and qraux contain the */
/*     usual output from dqrdc, so that the qr decomposition */
/*     of x with pivoting is fully available to the user. */
/*     in particular, columns jpvt(1), jpvt(2),...,jpvt(k) */
/*     were used in the solution, and the condition number */
/*     associated with those columns is estimated by */
/*     abs(x(1,1)/x(k,k)). */

/*     dqrfit uses the linpack routines dqrdc and dqrsl. */

/* Subroutine */ int dqrls_(doublereal *x, integer *n, integer *p, doublereal 
	*y, integer *ny, doublereal *tol, doublereal *b, doublereal *rsd, 
	doublereal *qty, integer *k, integer *jpvt, doublereal *qraux, 
	doublereal *work)
{
    /* System generated locals */
    integer x_dim1, x_offset, y_dim1, y_offset, b_dim1, b_offset, rsd_dim1, 
	    rsd_offset, qty_dim1, qty_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j, jj, kk, info;
    extern /* Subroutine */ int dqrsl_(doublereal *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, integer *, integer *), 
	    dqrdc2_(doublereal *, integer *, integer *, integer *, doublereal 
	    *, integer *, doublereal *, integer *, doublereal *);


/*     internal variables. */


/*     reduce x. */

    /* Parameter adjustments */
    --work;
    --qraux;
    --jpvt;
    x_dim1 = *n;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    qty_dim1 = *n;
    qty_offset = 1 + qty_dim1;
    qty -= qty_offset;
    rsd_dim1 = *n;
    rsd_offset = 1 + rsd_dim1;
    rsd -= rsd_offset;
    b_dim1 = *p;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    y_dim1 = *n;
    y_offset = 1 + y_dim1;
    y -= y_offset;

    /* Function Body */
    dqrdc2_(&x[x_offset], n, n, p, tol, k, &qraux[1], &jpvt[1], &work[1]);

/*     solve the truncated least squares problem for each rhs. */

    if (*k > 0) {
	i__1 = *ny;
	for (jj = 1; jj <= i__1; ++jj) {
	    dqrsl_(&x[x_offset], n, n, k, &qraux[1], &y[jj * y_dim1 + 1], &
		    rsd[jj * rsd_dim1 + 1], &qty[jj * qty_dim1 + 1], &b[jj * 
		    b_dim1 + 1], &rsd[jj * rsd_dim1 + 1], &rsd[jj * rsd_dim1 
		    + 1], &c__1110, &info);
/* L20: */
	}
    } else {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = *ny;
	    for (jj = 1; jj <= i__2; ++jj) {
		rsd[i__ + jj * rsd_dim1] = y[i__ + jj * y_dim1];
/* L30: */
	    }
/* L35: */
	}
    }

/*     set the unused components of b to zero. */

    kk = *k + 1;
    i__1 = *p;
    for (j = kk; j <= i__1; ++j) {
	i__2 = *ny;
	for (jj = 1; jj <= i__2; ++jj) {
	    b[j + jj * b_dim1] = 0.;
/* L40: */
	}
/* L50: */
    }
    return 0;
} /* dqrls_ */

