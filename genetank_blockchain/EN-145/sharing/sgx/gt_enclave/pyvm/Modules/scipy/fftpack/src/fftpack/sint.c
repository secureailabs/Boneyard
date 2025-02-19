/* sint.f -- translated by f2c (version 20160102).
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

/* Subroutine */ int sint_(integer *n, real *x, real *wsave)
{
    static integer np1, iw1, iw2, iw3;
    extern /* Subroutine */ int sint1_(integer *, real *, real *, real *, 
	    real *, real *);

    /* Parameter adjustments */
    --wsave;
    --x;

    /* Function Body */
    np1 = *n + 1;
    iw1 = *n / 2 + 1;
    iw2 = iw1 + np1;
    iw3 = iw2 + np1;
    sint1_(n, &x[1], &wsave[1], &wsave[iw1], &wsave[iw2], &wsave[iw3]);
    return 0;
} /* sint_ */

