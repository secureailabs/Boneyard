/* lminfl.f -- translated by f2c (version 20160102).
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
#include <R_ext/Linpack.h>
#include <math.h>

/* Table of constant values */

static integer c__10000 = 10000;
static integer c__1000 = 1000;
static integer c__1 = 1;

/* ----------------------------------------------------------------------- */

/*  R : A Computer Language for Statistical Data Analysis */
/*  Copyright (C) 1996, 1997  Robert Gentleman and Ross Ihaka */
/*  Copyright (C) 2003-5      The R Foundation */

/*  This program is free software; you can redistribute it and/or modify */
/*  it under the terms of the GNU General Public License as published by */
/*  the Free Software Foundation; either version 2 of the License, or */
/*  (at your option) any later version. */

/*  This program is distributed in the hope that it will be useful, */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*  GNU General Public License for more details. */

/*  You should have received a copy of the GNU General Public License */
/*  along with this program; if not, a copy is available at */
/*  https://www.R-project.org/Licenses/ */

/* ----------------------------------------------------------------------- */

/*   lminfl computes basic quantities useful for computing */
/*   regression diagnostics. */

/*   on entry */

/*       x       double precision(ldx,k) */
/*               the qr decomposition as computed by dqrdc or dqrdc2. */

/*       ldx     integer */
/*               the leading dimension of the array x. */

/*       n       integer */
/*               the number of rows of the matrix x. */

/*       k       integer */
/*               the number of columns in the matrix k. */

/*       docoef  integer (logical) indicating if  coef(*,*) should be computed */
/*               Computation of coef(.) is O(n^2 * k) which may be too much. */

/*       qraux   double precision(k) */
/*               auxiliary information about the qr decomposition. */

/*       resid   double precision(k) */
/*               the residuals from the regression. */

/*   on return */

/*       hat     double precision(n) */
/*               the diagonal of the hat matrix. */

/*       coef    double precision(n,p) */
/*               a matrix which has as i-th row the estimated */
/*               regression coefficients when the i-th case is omitted */
/*               from the regression. */

/*       sigma   double precision(n) */
/*               the i-th element of sigma contains an estimate */
/*               of the residual standard deviation for the model with */
/*               the i-th case omitted. */

/*   This version dated Aug 24, 1996. */
/*   Ross Ihaka, University of Auckland. */
/*   `docoef' option added Feb 17, 2003;  Martin Maechler ETH Zurich. */
/*   Handle hat == 1 case, Nov 2005. */
/*   Argument 'tol' was real not double precision, Aug 2007 */
/* Subroutine */ int lminfl_(doublereal *x, integer *ldx, integer *n, integer 
	*k, integer *docoef, doublereal *qraux, doublereal *resid, doublereal 
	*hat, doublereal *coef, doublereal *sigma, doublereal *tol)
{
    /* System generated locals */
    integer x_dim1, x_offset, coef_dim1, coef_offset, i__1, i__2;

    /* Builtin functions */
    //double sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    static doublereal sum;
    static integer info;
    static doublereal denom;
    extern /* Subroutine */ int dqrsl_(doublereal *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, integer *, integer *), 
	    dtrsl_(doublereal *, integer *, integer *, doublereal *, integer *
	    , integer *);
    static doublereal dummy;

/*   coef(.,.) can be dummy(1) when docoef is 0(false) */

/*     hat matrix diagonal */

    /* Parameter adjustments */
    --sigma;
    --hat;
    --resid;
    coef_dim1 = *n;
    coef_offset = 1 + coef_dim1;
    coef -= coef_offset;
    --qraux;
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	hat[i__] = 0.;
/* L10: */
    }
    i__1 = *k;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sigma[i__] = 0.;
/* L20: */
	}
	sigma[j] = 1.;
	dqrsl_(&x[x_offset], ldx, n, k, &qraux[1], &sigma[1], &sigma[1], &
		dummy, &dummy, &dummy, &dummy, &c__10000, &info);
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    hat[i__] += sigma[i__] * sigma[i__];
/* L30: */
	}
/* L40: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (hat[i__] >= 1. - *tol) {
	    hat[i__] = 1.;
	}
/* L45: */
    }

/*     changes in the estimated coefficients */

    if (*docoef != 0) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
		sigma[j] = 0.;
/* L50: */
	    }
/*           if hat is effectively 1, change is zero */
	    if (hat[i__] < 1.) {
		sigma[i__] = resid[i__] / (1. - hat[i__]);
		dqrsl_(&x[x_offset], ldx, n, k, &qraux[1], &sigma[1], &dummy, 
			&sigma[1], &dummy, &dummy, &dummy, &c__1000, &info);
		dtrsl_(&x[x_offset], ldx, k, &sigma[1], &c__1, &info);
	    }
	    i__2 = *k;
	    for (j = 1; j <= i__2; ++j) {
		coef[i__ + j * coef_dim1] = sigma[j];
/* L60: */
	    }
/* L70: */
	}
    }

/*     estimated residual standard deviation */

    denom = (doublereal) (*n - *k - 1);
    sum = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sum += resid[i__] * resid[i__];
/* L80: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (hat[i__] < 1.) {
	    sigma[i__] = sqrt((sum - resid[i__] * resid[i__] / (1. - hat[i__])
		    ) / denom);
	} else {
	    sigma[i__] = sqrt(sum / denom);
	}
/* L90: */
    }
    return 0;
} /* lminfl_ */

