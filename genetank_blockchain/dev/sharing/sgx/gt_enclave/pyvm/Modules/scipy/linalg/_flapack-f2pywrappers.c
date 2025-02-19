/* _flapack-f2pywrappers.f -- translated by f2c (version 20160102).
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
#include "clapack.h"

/*     -*- fortran -*- */
/*     This file is autogenerated with f2py (version:2) */
/*     It contains Fortran 77 wrappers to fortran functions. */
/* Subroutine */ int f2pywrapdlamch_(doublereal *dlamchf2pywrap, char *cmach, 
	ftnlen cmach_len)
{
    extern doublereal dlamch_(char *, integer);

    *dlamchf2pywrap = dlamch_(cmach, (integer)cmach_len);
    return 0;
} /* f2pywrapdlamch_ */

/* Subroutine */ int f2pywrapslamch_(real *slamchf2pywrap, char *cmach, 
	ftnlen cmach_len)
{
    extern doublereal slamch_(char *, integer);

    *slamchf2pywrap = slamch_(cmach, (integer)cmach_len);
    return 0;
} /* f2pywrapslamch_ */

/* Subroutine */ int f2pywrapslange_(real *slangef2pywrap, char *norm, 
	integer *m, integer *n, real *a, integer *lda, real *work, ftnlen 
	norm_len)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern doublereal slange_(char *, integer *, integer *, real *, integer *,
	     real *, ftnlen);

    /* Parameter adjustments */
    --work;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    *slangef2pywrap = slange_(norm, m, n, &a[a_offset], lda, &work[1], (
	    ftnlen)1);
    return 0;
} /* f2pywrapslange_ */

/* Subroutine */ int f2pywrapdlange_(doublereal *dlangef2pywrap, char *norm, 
	integer *m, integer *n, doublereal *a, integer *lda, doublereal *work,
	 ftnlen norm_len)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern doublereal dlange_(char *, integer *, integer *, doublereal *, 
	    integer *, doublereal *, ftnlen);

    /* Parameter adjustments */
    --work;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    *dlangef2pywrap = dlange_(norm, m, n, &a[a_offset], lda, &work[1], (
	    ftnlen)1);
    return 0;
} /* f2pywrapdlange_ */

/* Subroutine */ int f2pywrapclange_(real *clangef2pywrap, char *norm, 
	integer *m, integer *n, complex *a, integer *lda, real *work, ftnlen 
	norm_len)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern doublereal clange_(char *, integer *, integer *, complex *, 
	    integer *, real *, ftnlen);

    /* Parameter adjustments */
    --work;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    *clangef2pywrap = clange_(norm, m, n, &a[a_offset], lda, &work[1], (
	    ftnlen)1);
    return 0;
} /* f2pywrapclange_ */

/* Subroutine */ int f2pywrapzlange_(doublereal *zlangef2pywrap, char *norm, 
	integer *m, integer *n, doublecomplex *a, integer *lda, doublereal *
	work, ftnlen norm_len)
{
    /* System generated locals */
    integer a_dim1, a_offset;

    /* Local variables */
    extern doublereal zlange_(char *, integer *, integer *, doublecomplex *, 
	    integer *, doublereal *, ftnlen);

    /* Parameter adjustments */
    --work;
    a_dim1 = *m;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    *zlangef2pywrap = zlange_(norm, m, n, &a[a_offset], lda, &work[1], (
	    ftnlen)1);
    return 0;
} /* f2pywrapzlange_ */

