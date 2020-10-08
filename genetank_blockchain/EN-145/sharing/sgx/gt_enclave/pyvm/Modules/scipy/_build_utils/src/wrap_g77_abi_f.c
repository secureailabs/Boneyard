/* wrap_g77_abi_f.f -- translated by f2c (version 20160102).
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

/* Complex */ VOID wcdotc_(complex * ret_val, integer *n, complex *cx, 
	integer *incx, complex *cy, integer *incy)
{
    extern /* Subroutine */ int acc_cdotc_sub__(integer *, complex *, integer 
	    *, complex *, integer *, complex *);
    static complex result;

    /* Parameter adjustments */
    --cy;
    --cx;

    /* Function Body */
    acc_cdotc_sub__(n, &cx[1], incx, &cy[1], incy, &result);
     ret_val->r = result.r,  ret_val->i = result.i;
} /* wcdotc_ */

/* Complex */ VOID wcdotu_(complex * ret_val, integer *n, complex *cx, 
	integer *incx, complex *cy, integer *incy)
{
    extern /* Subroutine */ int acc_cdotu_sub__(integer *, complex *, integer 
	    *, complex *, integer *, complex *);
    static complex result;

    /* Parameter adjustments */
    --cy;
    --cx;

    /* Function Body */
    acc_cdotu_sub__(n, &cx[1], incx, &cy[1], incy, &result);
     ret_val->r = result.r,  ret_val->i = result.i;
} /* wcdotu_ */

/* Double Complex */ VOID wzdotc_(doublecomplex * ret_val, integer *n, 
	doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy)
{
    extern /* Subroutine */ int acc_zdotc_sub__(integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *, doublecomplex *);
    static doublecomplex result;

    /* Parameter adjustments */
    --cy;
    --cx;

    /* Function Body */
    acc_zdotc_sub__(n, &cx[1], incx, &cy[1], incy, &result);
     ret_val->r = result.r,  ret_val->i = result.i;
} /* wzdotc_ */

/* Double Complex */ VOID wzdotu_(doublecomplex * ret_val, integer *n, 
	doublecomplex *cx, integer *incx, doublecomplex *cy, integer *incy)
{
    extern /* Subroutine */ int acc_zdotu_sub__(integer *, doublecomplex *, 
	    integer *, doublecomplex *, integer *, doublecomplex *);
    static doublecomplex result;

    /* Parameter adjustments */
    --cy;
    --cx;

    /* Function Body */
    acc_zdotu_sub__(n, &cx[1], incx, &cy[1], incy, &result);
     ret_val->r = result.r,  ret_val->i = result.i;
} /* wzdotu_ */

/* Complex */ VOID wcladiv_(complex * ret_val, complex *x, complex *y)
{
    static complex z__;
    extern /* Subroutine */ int cladiv_(complex *, complex *, complex *);

    cladiv_(&z__, x, y);
     ret_val->r = z__.r,  ret_val->i = z__.i;
} /* wcladiv_ */

/* Double Complex */ VOID wzladiv_(doublecomplex * ret_val, doublecomplex *x, 
	doublecomplex *y)
{
    static doublecomplex z__;
    extern /* Subroutine */ int zladiv_(doublecomplex *, doublecomplex *, 
	    doublecomplex *);

    zladiv_(&z__, x, y);
     ret_val->r = z__.r,  ret_val->i = z__.i;
} /* wzladiv_ */

