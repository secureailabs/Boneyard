/*                                                     polevl.c
 *                                                     p1evl.c
 *
 *     Evaluate polynomial
 *
 *
 *
 * SYNOPSIS:
 *
 * int N;
 * double x, y, coef[N+1], polevl[];
 *
 * y = polevl( x, coef, N );
 *
 *
 *
 * DESCRIPTION:
 *
 * Evaluates polynomial of degree N:
 *
 *                     2          N
 * y  =  C  + C x + C x  +...+ C x
 *        0    1     2          N
 *
 * Coefficients are stored in reverse order:
 *
 * coef[0] = C  , ..., coef[N] = C  .
 *            N                   0
 *
 *  The function p1evl() assumes that coef[N] = 1.0 and is
 * omitted from the array.  Its calling arguments are
 * otherwise the same as polevl().
 *
 *
 * SPEED:
 *
 * In the interest of speed, there are no checks for out
 * of bounds arithmetic.  This routine is used by most of
 * the functions in the library.  Depending on available
 * equipment features, the user may wish to rewrite the
 * program in microcode or assembly language.
 *
 */


/*
 * Cephes Math Library Release 2.1:  December, 1988
 * Copyright 1984, 1987, 1988 by Stephen L. Moshier
 * Direct inquiries to 30 Frost Street, Cambridge, MA 02140
 */

/* Sources:
 * [1] Holin et. al., "Polynomial and Rational Function Evaluation",
 *     https://www.boost.org/doc/libs/1_61_0/libs/math/doc/html/math_toolkit/roots/rational.html
 */

/* Scipy changes:
 * - 06-23-2016: add code for evaluating rational functions
 */

#ifndef CEPHES_POLEV
#define CEPHES_POLEV

#include "../cephes.h"
#include <numpy/npy_common.h>

static NPY_INLINE double polevl(double x, double coef[], int N)
{
    double ans;
    int i;
    double *p;

    p = coef;
    ans = *p++;
    i = N;

    do
	ans = ans * x + *p++;
    while (--i);

    return (ans);
}

/*                                                     p1evl() */
/*                                          N
 * Evaluate polynomial when coefficient of x  is 1.0.
 * Otherwise same as polevl.
 */

static NPY_INLINE double p1evl(double x, double coef[], int N)
{
    double ans;
    double *p;
    int i;

    p = coef;
    ans = x + *p++;
    i = N - 1;

    do
	ans = ans * x + *p++;
    while (--i);

    return (ans);
}

/* Evaluate a rational function. See [1]. */

static NPY_INLINE double ratevl(double x, double num[], int M, double denom[], int N)
{
    int i, dir;
    double y, num_ans, denom_ans;
    double absx = fabs(x);
    double *p;

    if (absx > 1) {
	/* Evaluate as a polynomial in 1/x. */
	dir = -1;
	p = num + M;
	y = 1 / x;
    } else {
	dir = 1;
	p = num;
	y = x;
    }

    /* Evaluate the numerator */
    num_ans = *p;
    p += dir;
    for (i = 1; i <= M; i++) {
	num_ans = num_ans * y + *p;
	p += dir;
    }

    /* Evaluate the denominator */
    if (absx > 1) {
	p = denom + N;
    } else {
	p = denom;
    }

    denom_ans = *p;
    p += dir;
    for (i = 1; i <= N; i++) {
	denom_ans = denom_ans * y + *p;
	p += dir;
    }

    if (absx > 1) {
	i = N - M;
	return pow(x, i) * num_ans / denom_ans;
    } else {
	return num_ans / denom_ans;
    }
}

#endif
