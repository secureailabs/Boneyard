/* dpofa.f -- translated by f2c (version 20160102).
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
#include "../../include/f2c.h"
#include "../../include/blas.h"
#include <math.h>

/* Table of constant values */

static integer c__1 = 1;

/*  Modified 2002-05-20 for R to add a tolerance of positive definiteness. */


/*     dpofa factors a double precision symmetric positive definite */
/*     matrix. */

/*     dpofa is usually called by dpoco, but it can be called */
/*     directly with a saving in time if  rcond  is not needed. */
/*     (time for dpoco) = (1 + 18/n)*(time for dpofa) . */

/*     on entry */

/*        a       double precision(lda, n) */
/*                the symmetric matrix to be factored.  only the */
/*                diagonal and upper triangle are used. */

/*        lda     integer */
/*                the leading dimension of the array  a . */

/*        n       integer */
/*                the order of the matrix  a . */

/*     on return */

/*        a       an upper triangular matrix  r  so that  a = trans(r)*r */
/*                where  trans(r)  is the transpose. */
/*                the strict lower triangle is unaltered. */
/*                if  info .ne. 0 , the factorization is not complete. */

/*        info    integer */
/*                = 0  for normal return. */
/*                = k  signals an error condition.  the leading minor */
/*                     of order  k  is not positive definite. */

/*     linpack.  this version dated 08/14/78 . */
/*     cleve moler, university of new mexico, argonne national lab. */

/*     subroutines and functions */

/*     blas ddot */
/*     fortran dsqrt */

/* Subroutine */ int dpofa_(doublereal *a, integer *lda, integer *n, integer *
	info)
{
    /* Initialized data */

    static doublereal eps = 1e-14;

    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer j, k;
    static doublereal s, t;
    static integer jm1;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);


/*     internal variables */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
/*     begin block with ...exits to 40 */


    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	*info = j;
	s = 0.;
	jm1 = j - 1;
	if (jm1 < 1) {
	    goto L20;
	}
	i__2 = jm1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = k - 1;
	    t = a[k + j * a_dim1] - ddot_(&i__3, &a[k * a_dim1 + 1], &c__1, &
		    a[j * a_dim1 + 1], &c__1);
	    t /= a[k + k * a_dim1];
	    a[k + j * a_dim1] = t;
	    s += t * t;
/* L10: */
	}
L20:
	s = a[j + j * a_dim1] - s;
/*     ......exit */
/*            if (s .le. 0.0d0) go to 40 */
	if (s <= eps * (d__1 = a[j + j * a_dim1], abs(d__1))) {
	    goto L40;
	}
	a[j + j * a_dim1] = sqrt(s);
/* L30: */
    }
    *info = 0;
L40:
    return 0;
} /* dpofa_ */

