/* ppr.f -- translated by f2c (version 20160102).
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

#include <math.h>


/* Common Block Declarations */

struct pprpar_1_ {
    integer ifl, lf;
    doublereal span, alpha, big;
};

#define pprpar_1 (*(struct pprpar_1_ *) &pprpar_)

struct pprz01_1_ {
    doublereal conv;
    integer maxit, mitone;
    doublereal cutmin, fdel, cjeps;
    integer mitcj;
};

#define pprz01_1 (*(struct pprz01_1_ *) &pprz01_)

struct spsmooth_1_ {
    doublereal df, gcvpen;
    integer ismethod;
    logical trace;
};

#define spsmooth_1 (*(struct spsmooth_1_ *) &spsmooth_)

struct spans_1_ {
    doublereal spans[3];
};

#define spans_1 (*(struct spans_1_ *) &spans_)

struct consts_1_ {
    doublereal big, sml, eps;
};

#define consts_1 (*(struct consts_1_ *) &consts_)

/* Initialized data */

struct {
    doublereal e_1[2];
    integer e_2;
    logical e_3;
    } spsmooth_ = { 4., 1., 0, FALSE_ };

struct {
    integer e_1[2];
    doublereal e_2[3];
    } pprpar_ = { 6, 2, 0., 0., 1e20 };

struct {
    doublereal e_1[3];
    } spans_ = { .05, .2, .5 };

struct {
    doublereal e_1[3];
    } consts_ = { 1e20, 1e-7, .001 };

struct {
    doublereal e_1;
    integer e_2[2];
    doublereal e_3[3];
    integer e_4;
    } pprz01_ = { .005, 20, 20, .1, .02, .001, 1 };


/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static doublereal c_b183 = 0.;
static integer c__4 = 4;
static integer c__18 = 18;


/*     Modified from the SMART package by J.H. Friedman, 10/10/84 */
/*     Main change is to add spline smoothing modified from BRUTO, */
/*     calling code written for smooth.spline in S. */

/*     B.D. Ripley (ripley@stats.ox.ac.uk)  1994-7. */


/* Subroutine */ int smart_(integer *m, integer *mu, integer *p, integer *q, 
	integer *n, doublereal *w, doublereal *x, doublereal *y, doublereal *
	ww, doublereal *smod, integer *nsmod, doublereal *sp, integer *nsp, 
	doublereal *dp, integer *ndp, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, y_dim1, y_offset;

    /* Local variables */
    extern /* Subroutine */ int smart1_(integer *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);

    /* Parameter adjustments */
    --edf;
    --ww;
    y_dim1 = *q;
    y_offset = 1 + y_dim1;
    y -= y_offset;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --w;
    --smod;
    --sp;
    --dp;

    /* Function Body */
    smod[1] = (doublereal) (*m);
    smod[2] = (doublereal) (*p);
    smod[3] = (doublereal) (*q);
    smod[4] = (doublereal) (*n);
    smart1_(m, mu, p, q, n, &w[1], &x[x_offset], &y[y_offset], &ww[1], &smod[
	    6], &smod[*q + 6], &smod[*q + 7], &smod[*q + 7 + *p * *m], &smod[*
	    q + 7 + *m * (*p + *q)], &smod[*q + 7 + *m * (*p + *q + *n)], &
	    smod[*q + 7 + *m * (*p + *q + (*n << 1))], &sp[1], &sp[*q * *n + 
	    1], &sp[*n * (*q + 15) + 1], &sp[*n * (*q + 15) + *q + 1], &dp[1],
	     &smod[5], &edf[1]);
    return 0;
} /* smart_ */

/* Subroutine */ int smart1_(integer *m, integer *mu, integer *p, integer *q, 
	integer *n, doublereal *w, doublereal *x, doublereal *y, doublereal *
	ww, doublereal *yb, doublereal *ys, doublereal *a, doublereal *b, 
	doublereal *f, doublereal *t, doublereal *asr, doublereal *r__, 
	doublereal *sc, doublereal *bt, doublereal *g, doublereal *dp, 
	doublereal *flm, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, a_dim1, a_offset, b_dim1, b_offset, f_dim1, 
	    f_offset, t_dim1, t_offset, r_dim1, r_offset, sc_dim1, sc_offset, 
	    g_dim1, g_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, l;
    static doublereal s;
    static integer lm;
    static doublereal sw, asr1;
    extern /* Subroutine */ int sort_(doublereal *, doublereal *, integer *, 
	    integer *), fulfit_(integer *, integer *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), subfit_(integer *, 
	    integer *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);

/*                        ^^^ really (ndb) of  smart(.) */
/* Common Vars */
    /* Parameter adjustments */
    --edf;
    g_dim1 = *p;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    a_dim1 = *p;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --bt;
    b_dim1 = *q;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --yb;
    --ww;
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    r_dim1 = *q;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    t_dim1 = *n;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    f_dim1 = *n;
    f_offset = 1 + f_dim1;
    f -= f_offset;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --w;
    --y;
    --asr;
    --dp;

    /* Function Body */
    sw = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sw += w[j];
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] = y[*q * (j - 1) + i__];
	}
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    s += w[j] * r__[i__ + j * r_dim1];
	}
	yb[i__] = s / sw;
    }
/*     yb is vector of means */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] -= yb[i__];
	}
    }
    *ys = 0.;
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	    d__1 = r__[i__ + j * r_dim1];
	    s += w[j] * (d__1 * d__1);
	}
	*ys += ww[i__] * s / sw;
    }
    if (*ys > 0.) {
	goto L311;
    }
/*     ys is the overall standard deviation -- quit if zero */
    return 0;
L311:
    *ys = sqrt(*ys);
    s = 1. / *ys;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] *= s;
	}
    }
/*     r is now standardized residuals */
/*     subfit adds up to m  terms one at time; lm is the number fitted. */
    subfit_(m, p, q, n, &w[1], &sw, &x[x_offset], &r__[r_offset], &ww[1], &lm,
	     &a[a_offset], &b[b_offset], &f[f_offset], &t[t_offset], &asr[1], 
	    &sc[sc_offset], &bt[1], &g[g_offset], &dp[1], &edf[1]);
    if (pprpar_1.lf <= 0) {
	goto L9999;
    }
    fulfit_(&lm, &pprpar_1.lf, p, q, n, &w[1], &sw, &x[x_offset], &r__[
	    r_offset], &ww[1], &a[a_offset], &b[b_offset], &f[f_offset], &t[
	    t_offset], &asr[1], &sc[sc_offset], &bt[1], &g[g_offset], &dp[1], 
	    &edf[1]);
/* REPEAT */
L371:
    i__1 = lm;
    for (l = 1; l <= i__1; ++l) {
	sc[l + sc_dim1] = l + .1;
	s = 0.;
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s += ww[i__] * (d__1 = b[i__ + l * b_dim1], abs(d__1));
	}
	sc[l + (sc_dim1 << 1)] = -s;
    }
    sort_(&sc[(sc_dim1 << 1) + 1], &sc[sc_offset], &c__1, &lm);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] = y[*q * (j - 1) + i__];
	}
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    r__[i__ + j * r_dim1] -= yb[i__];
	    s = 0.;
	    i__3 = lm;
	    for (l = 1; l <= i__3; ++l) {
		s += b[i__ + l * b_dim1] * f[j + l * f_dim1];
	    }
	    r__[i__ + j * r_dim1] = r__[i__ + j * r_dim1] / *ys - s;
	}
    }
    if (lm <= *mu) {
	goto L9999;
    }
/* back to integer: */
    l = (integer) sc[lm + sc_dim1];
    asr1 = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] += b[i__ + l * b_dim1] * f[j + l * f_dim1];
/* Computing 2nd power */
	    d__1 = r__[i__ + j * r_dim1];
	    asr1 += w[j] * ww[i__] * (d__1 * d__1);
	}
    }
    asr1 /= sw;
    asr[1] = asr1;
    if (l >= lm) {
	goto L591;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[i__ + l * a_dim1] = a[i__ + lm * a_dim1];
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b[i__ + l * b_dim1] = b[i__ + lm * b_dim1];
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	f[j + l * f_dim1] = f[j + lm * f_dim1];
	t[j + l * t_dim1] = t[j + lm * t_dim1];
    }
L591:
    --lm;
    fulfit_(&lm, &pprpar_1.lf, p, q, n, &w[1], &sw, &x[x_offset], &r__[
	    r_offset], &ww[1], &a[a_offset], &b[b_offset], &f[f_offset], &t[
	    t_offset], &asr[1], &sc[sc_offset], &bt[1], &g[g_offset], &dp[1], 
	    &edf[1]);
    goto L371;
/* END REPEAT */
L9999:
    *flm = (doublereal) lm;
    return 0;
} /* smart1_ */

/* Subroutine */ int subfit_(integer *m, integer *p, integer *q, integer *n, 
	doublereal *w, doublereal *sw, doublereal *x, doublereal *r__, 
	doublereal *ww, integer *lm, doublereal *a, doublereal *b, doublereal 
	*f, doublereal *t, doublereal *asr, doublereal *sc, doublereal *bt, 
	doublereal *g, doublereal *dp, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, r_dim1, r_offset, a_dim1, a_offset, b_dim1, 
	    b_offset, f_dim1, f_offset, t_dim1, t_offset, sc_dim1, sc_offset, 
	    g_dim1, g_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, l;
    extern /* Subroutine */ int newb_(integer *, integer *, doublereal *, 
	    doublereal *);
    static integer iflsv;
    static doublereal asrold;
    extern /* Subroutine */ int fulfit_(integer *, integer *, integer *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), onetrm_(
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), rchkusr_(void);

/* Args */
/* Var */
/* Common Vars */
    /* Parameter adjustments */
    --edf;
    g_dim1 = *p;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    a_dim1 = *p;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --bt;
    b_dim1 = *q;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --ww;
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    t_dim1 = *n;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    f_dim1 = *n;
    f_offset = 1 + f_dim1;
    f -= f_offset;
    r_dim1 = *q;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --w;
    --asr;
    --dp;

    /* Function Body */
    asr[1] = pprpar_1.big;
    *lm = 0;
    i__1 = *m;
    for (l = 1; l <= i__1; ++l) {
	rchkusr_();
	++(*lm);
	asrold = asr[1];
	newb_(lm, q, &ww[1], &b[b_offset]);
/* does 'edf' mean 'edf(1)' or 'edf(l)'? */
	onetrm_(&c__0, p, q, n, &w[1], sw, &x[x_offset], &r__[r_offset], &ww[
		1], &a[*lm * a_dim1 + 1], &b[*lm * b_dim1 + 1], &f[*lm * 
		f_dim1 + 1], &t[*lm * t_dim1 + 1], &asr[1], &sc[sc_offset], &
		g[g_offset], &dp[1], &edf[1]);
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = *q;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		r__[i__ + j * r_dim1] -= b[i__ + *lm * b_dim1] * f[j + *lm * 
			f_dim1];
/* L10: */
	    }
/* L20: */
	}
	if (*lm == 1) {
	    goto L100;
	}
	if (pprpar_1.lf > 0) {
	    if (*lm == *m) {
		return 0;
	    }
	    iflsv = pprpar_1.ifl;
	    pprpar_1.ifl = 0;
	    fulfit_(lm, &c__1, p, q, n, &w[1], sw, &x[x_offset], &r__[
		    r_offset], &ww[1], &a[a_offset], &b[b_offset], &f[
		    f_offset], &t[t_offset], &asr[1], &sc[sc_offset], &bt[1], 
		    &g[g_offset], &dp[1], &edf[1]);
	    pprpar_1.ifl = iflsv;
	}
	if (asr[1] <= 0. || (asrold - asr[1]) / asrold < pprz01_1.conv) {
	    return 0;
	}
L100:
	;
    }
    return 0;
} /* subfit_ */

/* Subroutine */ int fulfit_(integer *lm, integer *lbf, integer *p, integer *
	q, integer *n, doublereal *w, doublereal *sw, doublereal *x, 
	doublereal *r__, doublereal *ww, doublereal *a, doublereal *b, 
	doublereal *f, doublereal *t, doublereal *asr, doublereal *sc, 
	doublereal *bt, doublereal *g, doublereal *dp, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, r_dim1, r_offset, a_dim1, a_offset, b_dim1, 
	    b_offset, f_dim1, f_offset, t_dim1, t_offset, sc_dim1, sc_offset, 
	    g_dim1, g_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, lp;
    static doublereal fsv;
    static integer isv;
    static doublereal asri;
    static integer iter;
    static doublereal asrold;
    extern /* Subroutine */ int onetrm_(integer *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);

/* Args */
/* Var */
/* Common Vars */
    /* Parameter adjustments */
    --edf;
    --asr;
    g_dim1 = *p;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    a_dim1 = *p;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --bt;
    b_dim1 = *q;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --ww;
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    t_dim1 = *n;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    f_dim1 = *n;
    f_offset = 1 + f_dim1;
    f -= f_offset;
    r_dim1 = *q;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --w;
    --dp;

    /* Function Body */
    if (*lbf <= 0) {
	return 0;
    }
    asri = asr[1];
    fsv = pprz01_1.cutmin;
    isv = pprz01_1.mitone;
    if (*lbf < 3) {
	pprz01_1.cutmin = 1.;
	pprz01_1.mitone = *lbf - 1;
    }
    iter = 0;
/* Outer loop: */
L1000:
    asrold = asri;
    ++iter;
    i__1 = *lm;
    for (lp = 1; lp <= i__1; ++lp) {
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    bt[i__] = b[i__ + lp * b_dim1];
/* L10: */
	}
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    g[i__ + g_dim1 * 3] = a[i__ + lp * a_dim1];
/* L20: */
	}
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = *q;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		r__[i__ + j * r_dim1] += bt[i__] * f[j + lp * f_dim1];
/* L30: */
	    }
/* L35: */
	}
	onetrm_(&c__1, p, q, n, &w[1], sw, &x[x_offset], &r__[r_offset], &ww[
		1], &g[g_dim1 * 3 + 1], &bt[1], &sc[sc_dim1 * 14 + 1], &sc[
		sc_dim1 * 15 + 1], &asri, &sc[sc_offset], &g[g_offset], &dp[1]
		, &edf[lp]);
	if (asri < asrold) {
	    i__2 = *q;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		b[i__ + lp * b_dim1] = bt[i__];
/* L40: */
	    }
	    i__2 = *p;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		a[i__ + lp * a_dim1] = g[i__ + g_dim1 * 3];
/* L50: */
	    }
	    i__2 = *n;
	    for (j = 1; j <= i__2; ++j) {
		f[j + lp * f_dim1] = sc[j + sc_dim1 * 14];
		t[j + lp * t_dim1] = sc[j + sc_dim1 * 15];
/* L60: */
	    }
	} else {
	    asri = asrold;
	}
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    i__3 = *q;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		r__[i__ + j * r_dim1] -= b[i__ + lp * b_dim1] * f[j + lp * 
			f_dim1];
/* L80: */
	    }
/* L85: */
	}
/* L100: */
    }
    if (iter <= pprz01_1.maxit && (asri > 0. && (asrold - asri) / asrold >= 
	    pprz01_1.conv)) {
	goto L1000;
    }
    pprz01_1.cutmin = fsv;
    pprz01_1.mitone = isv;
    if (pprpar_1.ifl > 0) {
	asr[*lm + 1] = asri;
	asr[1] = asri;
    }
    return 0;
} /* fulfit_ */

/* Subroutine */ int onetrm_(integer *jfl, integer *p, integer *q, integer *n,
	 doublereal *w, doublereal *sw, doublereal *x, doublereal *y, 
	doublereal *ww, doublereal *a, doublereal *b, doublereal *f, 
	doublereal *t, doublereal *asr, doublereal *sc, doublereal *g, 
	doublereal *dp, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, y_dim1, y_offset, sc_dim1, sc_offset, g_dim1, 
	    g_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Local variables */
    static integer i__, j;
    static doublereal s;
    static integer iter;
    extern /* Subroutine */ int oneone_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal asrold;

/* Args */
/* Var */
/* Common Vars */
    /* Parameter adjustments */
    g_dim1 = *p;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    --a;
    --b;
    --ww;
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --t;
    --f;
    y_dim1 = *q;
    y_offset = 1 + y_dim1;
    y -= y_offset;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --w;
    --dp;

    /* Function Body */
    iter = 0;
    *asr = pprpar_1.big;
/* REPEAT */
L1000:
    ++iter;
    asrold = *asr;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	s = 0.;
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s += ww[i__] * b[i__] * y[i__ + j * y_dim1];
/* L21: */
	}
	sc[j + sc_dim1 * 13] = s;
/* L11: */
    }
/* Computing MAX */
    i__2 = *jfl, i__3 = iter - 1;
    i__1 = max(i__2,i__3);
    oneone_(&i__1, p, n, &w[1], sw, &sc[sc_dim1 * 13 + 1], &x[x_offset], &a[1]
	    , &f[1], &t[1], asr, &sc[sc_offset], &g[g_offset], &dp[1], edf);
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    s += w[j] * y[i__ + j * y_dim1] * f[j];
/* L41: */
	}
	b[i__] = s / *sw;
/* L31: */
    }
    *asr = 0.;
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
/* Computing 2nd power */
	    d__1 = y[i__ + j * y_dim1] - b[i__] * f[j];
	    s += w[j] * (d__1 * d__1);
/* L61: */
	}
	*asr += ww[i__] * s / *sw;
/* L51: */
    }
    if (*q != 1 && iter <= pprz01_1.maxit && *asr > 0. && (asrold - *asr) / 
	    asrold >= pprz01_1.conv) {
	goto L1000;
    }
    return 0;
} /* onetrm_ */

/* Subroutine */ int oneone_(integer *ist, integer *p, integer *n, doublereal 
	*w, doublereal *sw, doublereal *y, doublereal *x, doublereal *a, 
	doublereal *f, doublereal *t, doublereal *asr, doublereal *sc, 
	doublereal *g, doublereal *dp, doublereal *edf)
{
    /* System generated locals */
    integer x_dim1, x_offset, sc_dim1, sc_offset, g_dim1, g_offset, i__1, 
	    i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal s, v, sml, cut;
    static integer iter;
    extern /* Subroutine */ int sort_(doublereal *, doublereal *, integer *, 
	    integer *);
    static doublereal asrold;
    extern /* Subroutine */ int pprder_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *), pprdir_(
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), supsmu_(
	    integer *, doublereal *, doublereal *, doublereal *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);

/* Args */
/* Var */
/* Common Vars */
    /* Parameter adjustments */
    g_dim1 = *p;
    g_offset = 1 + g_dim1;
    g -= g_offset;
    --a;
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --t;
    --f;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --y;
    --w;
    --dp;

    /* Function Body */
    sml = 1. / pprpar_1.big;
    if (*ist <= 0) {
	if (*p <= 1) {
	    a[1] = 1.;
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    sc[j + (sc_dim1 << 1)] = 1.;
/* L10: */
	}
	pprdir_(p, n, &w[1], sw, &y[1], &x[x_offset], &sc[(sc_dim1 << 1) + 1],
		 &a[1], &dp[1]);
    }
    s = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__ + g_dim1] = 0.;
/* Computing 2nd power */
	d__1 = a[i__];
	s += d__1 * d__1;
/* L20: */
    }
    s = 1. / sqrt(s);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[i__] *= s;
/* L30: */
    }
    iter = 0;
    *asr = pprpar_1.big;
    cut = 1.;
/* REPEAT ----------------------------- */
L100:
    ++iter;
    asrold = *asr;
/* REPEAT [inner loop] ----- */
L60:
    s = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__ + (g_dim1 << 1)] = a[i__] + g[i__ + g_dim1];
/* Computing 2nd power */
	d__1 = g[i__ + (g_dim1 << 1)];
	s += d__1 * d__1;
/* L70: */
    }
    s = 1. / sqrt(s);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__ + (g_dim1 << 1)] *= s;
/* L80: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sc[j + sc_dim1] = j + .1;
	s = 0.;
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s += g[i__ + (g_dim1 << 1)] * x[i__ + j * x_dim1];
/* L91: */
	}
	sc[j + sc_dim1 * 11] = s;
/* L90: */
    }
    sort_(&sc[sc_dim1 * 11 + 1], &sc[sc_offset], &c__1, n);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	k = (integer) sc[j + sc_dim1];
	sc[j + (sc_dim1 << 1)] = y[k];
/* Computing MAX */
	d__1 = w[k];
	sc[j + sc_dim1 * 3] = max(d__1,sml);
/* L110: */
    }
    supsmu_(n, &sc[sc_dim1 * 11 + 1], &sc[(sc_dim1 << 1) + 1], &sc[sc_dim1 * 
	    3 + 1], &c__1, &pprpar_1.span, &pprpar_1.alpha, &sc[sc_dim1 * 12 
	    + 1], &sc[(sc_dim1 << 2) + 1], edf);
    s = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing 2nd power */
	d__1 = sc[j + (sc_dim1 << 1)] - sc[j + sc_dim1 * 12];
	s += sc[j + sc_dim1 * 3] * (d__1 * d__1);
/* L120: */
    }
    s /= *sw;
    if (s < *asr) {
	goto L140;
    }
    cut *= .5;
    if (cut < pprz01_1.cutmin) {
	goto L199;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__ + g_dim1] *= cut;
/* L150: */
    }
    goto L60;
/*     -------- */
L140:
    *asr = s;
    cut = 1.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	a[i__] = g[i__ + (g_dim1 << 1)];
/* L160: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	k = (integer) sc[j + sc_dim1];
	t[k] = sc[j + sc_dim1 * 11];
	f[k] = sc[j + sc_dim1 * 12];
/* L170: */
    }
    if (*asr <= 0. || (asrold - *asr) / asrold < pprz01_1.conv) {
	goto L199;
    }
    if (iter > pprz01_1.mitone || *p <= 1) {
	goto L199;
    }
    pprder_(n, &sc[sc_dim1 * 11 + 1], &sc[sc_dim1 * 12 + 1], &sc[sc_dim1 * 3 
	    + 1], &pprz01_1.fdel, &sc[(sc_dim1 << 2) + 1], &sc[sc_dim1 * 5 + 
	    1]);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	k = (integer) sc[j + sc_dim1];
	sc[j + sc_dim1 * 5] = y[j] - f[j];
	sc[k + sc_dim1 * 6] = sc[j + (sc_dim1 << 2)];
/* L180: */
    }
    pprdir_(p, n, &w[1], sw, &sc[sc_dim1 * 5 + 1], &x[x_offset], &sc[sc_dim1 *
	     6 + 1], &g[g_offset], &dp[1]);
    goto L100;
/* -------------- */
L199:
/* -------------- */
    s = 0.;
    v = s;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	s += w[j] * f[j];
/* L210: */
    }
    s /= *sw;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	f[j] -= s;
/* Computing 2nd power */
	d__1 = f[j];
	v += w[j] * (d__1 * d__1);
/* L220: */
    }
    if (v > 0.) {
	v = 1. / sqrt(v / *sw);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    f[j] *= v;
/* L230: */
	}
    }
    return 0;
} /* oneone_ */

/* Subroutine */ int pprdir_(integer *p, integer *n, doublereal *w, 
	doublereal *sw, doublereal *r__, doublereal *x, doublereal *d__, 
	doublereal *e, doublereal *g)
{
    /* System generated locals */
    integer x_dim1, x_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal s;
    static integer m1, m2;
    extern /* Subroutine */ int ppconj_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *, doublereal *);

    /* Parameter adjustments */
    --e;
    --d__;
    x_dim1 = *p;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --r__;
    --w;
    --g;

    /* Function Body */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    s += w[j] * d__[j] * x[i__ + j * x_dim1];
/* L15: */
	}
	e[i__] = s / *sw;
/* L10: */
    }
    k = 0;
    m1 = *p * (*p + 1) / 2;
    m2 = m1 + *p;
    i__1 = *p;
    for (j = 1; j <= i__1; ++j) {
	s = 0.;
	i__2 = *n;
	for (l = 1; l <= i__2; ++l) {
	    s += w[l] * r__[l] * (d__[l] * x[j + l * x_dim1] - e[j]);
/* L22: */
	}
	g[m1 + j] = s / *sw;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s = 0.;
	    i__3 = *n;
	    for (l = 1; l <= i__3; ++l) {
		s += w[l] * (d__[l] * x[i__ + l * x_dim1] - e[i__]) * (d__[l] 
			* x[j + l * x_dim1] - e[j]);
/* L27: */
	    }
	    ++k;
	    g[k] = s / *sw;
/* L25: */
	}
/* L20: */
    }
    ppconj_(p, &g[1], &g[m1 + 1], &g[m2 + 1], &pprz01_1.cjeps, &
	    pprz01_1.mitcj, &g[m2 + *p + 1]);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	e[i__] = g[m2 + i__];
/* L30: */
    }
    return 0;
} /* pprdir_ */

/* Subroutine */ int ppconj_(integer *p, doublereal *g, doublereal *c__, 
	doublereal *x, doublereal *eps, integer *maxit, doublereal *sc)
{
    /* System generated locals */
    integer sc_dim1, sc_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal s, t;
    static integer im1, nit;
    static doublereal beta;
    static integer iter;
    static doublereal alpha;

    /* Parameter adjustments */
    sc_dim1 = *p;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --x;
    --c__;
    --g;

    /* Function Body */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	x[i__] = 0.;
	sc[i__ + (sc_dim1 << 1)] = 0.;
/* L1: */
    }
    nit = 0;
/* REPEAT */
L11321:
    ++nit;
    h__ = 0.;
    beta = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sc[i__ + (sc_dim1 << 2)] = x[i__];
	s = g[i__ * (i__ - 1) / 2 + i__] * x[i__];
	im1 = i__ - 1;
	j = 1;
	goto L11343;
L11341:
	++j;
L11343:
	if (j > im1) {
	    goto L11342;
	}
	s += g[i__ * (i__ - 1) / 2 + j] * x[j];
	goto L11341;
L11342:
	j = i__ + 1;
	goto L11353;
L11351:
	++j;
L11353:
	if (j > *p) {
	    goto L11352;
	}
	s += g[j * (j - 1) / 2 + i__] * x[j];
	goto L11351;
L11352:
	sc[i__ + sc_dim1] = s - c__[i__];
/* Computing 2nd power */
	d__1 = sc[i__ + sc_dim1];
	h__ += d__1 * d__1;
/* L11331: */
    }
    if (h__ <= 0.) {
	goto L11322;
    }
    i__1 = *p;
    for (iter = 1; iter <= i__1; ++iter) {
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sc[i__ + (sc_dim1 << 1)] = beta * sc[i__ + (sc_dim1 << 1)] - sc[
		    i__ + sc_dim1];
/* L11371: */
	}
	t = 0.;
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s = g[i__ * (i__ - 1) / 2 + i__] * sc[i__ + (sc_dim1 << 1)];
	    im1 = i__ - 1;
	    j = 1;
	    goto L11393;
L11391:
	    ++j;
L11393:
	    if (j > im1) {
		goto L11392;
	    }
	    s += g[i__ * (i__ - 1) / 2 + j] * sc[j + (sc_dim1 << 1)];
	    goto L11391;
L11392:
	    j = i__ + 1;
	    goto L11403;
L11401:
	    ++j;
L11403:
	    if (j > *p) {
		goto L11402;
	    }
	    s += g[j * (j - 1) / 2 + i__] * sc[j + (sc_dim1 << 1)];
	    goto L11401;
L11402:
	    sc[i__ + sc_dim1 * 3] = s;
	    t += s * sc[i__ + (sc_dim1 << 1)];
/* L11381: */
	}
	alpha = h__ / t;
	s = 0.;
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    x[i__] += alpha * sc[i__ + (sc_dim1 << 1)];
	    sc[i__ + sc_dim1] += alpha * sc[i__ + sc_dim1 * 3];
/* Computing 2nd power */
	    d__1 = sc[i__ + sc_dim1];
	    s += d__1 * d__1;
/* L11411: */
	}
	if (s <= 0.) {
	    goto L11362;
	}
	beta = s / h__;
	h__ = s;
/* L11361: */
    }
L11362:
    s = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	d__2 = s, d__3 = (d__1 = x[i__] - sc[i__ + (sc_dim1 << 2)], abs(d__1))
		;
	s = max(d__2,d__3);
/* L11421: */
    }
    if (s >= *eps && nit < *maxit) {
	goto L11321;
    }
L11322:
    return 0;
} /* ppconj_ */

/* Subroutine */ int pprder_(integer *n, doublereal *x, doublereal *s, 
	doublereal *w, doublereal *fdel, doublereal *d__, doublereal *sc)
{
    /* System generated locals */
    integer sc_dim1, sc_offset, i__1;

    /* Local variables */
    static integer i__, j, bc, ec, bl, el, br, er;
    static doublereal del;
    extern /* Subroutine */ int pool_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal scale;
    extern /* Subroutine */ int rexit_(char *, ftnlen);


/*     unnecessary initialization of bl el ec to keep g77 -Wall happy */

    /* Parameter adjustments */
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --d__;
    --w;
    --s;
    --x;

    /* Function Body */
    bl = 0;
    el = 0;
    ec = 0;

    if (x[*n] > x[1]) {
	goto L11441;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	d__[j] = 0.;
/* L11451: */
    }
    return 0;
L11441:
    i__ = *n / 4;
    j = i__ * 3;
    scale = x[j] - x[i__];
L11461:
    if (scale > 0.) {
	goto L11462;
    }
    if (j < *n) {
	++j;
    }
    if (i__ > 1) {
	--i__;
    }
    scale = x[j] - x[i__];
    goto L11461;
L11462:
    del = *fdel * scale * 2.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sc[j + sc_dim1] = x[j];
	sc[j + (sc_dim1 << 1)] = s[j];
	sc[j + sc_dim1 * 3] = w[j];
/* L11471: */
    }
    pool_(n, &sc[sc_offset], &sc[(sc_dim1 << 1) + 1], &sc[sc_dim1 * 3 + 1], &
	    del);
    bc = 0;
    br = bc;
    er = br;
L11481:
    br = er + 1;
    er = br;
L11491:
    if (er >= *n) {
	goto L11492;
    }
    if (sc[br + sc_dim1] != sc[er + 1 + sc_dim1]) {
	goto L11511;
    }
    ++er;
    goto L11521;
L11511:
    goto L11492;
L11521:
    goto L11491;
L11492:
    if (br != 1) {
	goto L11541;
    }
    bl = br;
    el = er;
    goto L11481;
L11541:
    if (bc != 0) {
	goto L11561;
    }
    bc = br;
    ec = er;
    i__1 = el;
    for (j = bl; j <= i__1; ++j) {
	d__[j] = (sc[bc + (sc_dim1 << 1)] - sc[bl + (sc_dim1 << 1)]) / (sc[bc 
		+ sc_dim1] - sc[bl + sc_dim1]);
/* L11571: */
    }
    goto L11481;
L11561:
/* sanity check needed for PR#13517 */
    if (br > *n) {
	//rexit_("br is too large", (ftnlen)15);
    }
    i__1 = ec;
    for (j = bc; j <= i__1; ++j) {
	d__[j] = (sc[br + (sc_dim1 << 1)] - sc[bl + (sc_dim1 << 1)]) / (sc[br 
		+ sc_dim1] - sc[bl + sc_dim1]);
/* L11581: */
    }
    if (er != *n) {
	goto L11601;
    }
    i__1 = er;
    for (j = br; j <= i__1; ++j) {
	d__[j] = (sc[br + (sc_dim1 << 1)] - sc[bc + (sc_dim1 << 1)]) / (sc[br 
		+ sc_dim1] - sc[bc + sc_dim1]);
/* L11611: */
    }
    goto L11482;
L11601:
    bl = bc;
    el = ec;
    bc = br;
    ec = er;
    goto L11481;
L11482:
    return 0;
} /* pprder_ */

/* Subroutine */ int pool_(integer *n, doublereal *x, doublereal *y, 
	doublereal *w, doublereal *del)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, bb, eb, bl, el, br, er;
    static doublereal pw, px, py;

    /* Parameter adjustments */
    --w;
    --y;
    --x;

    /* Function Body */
    bb = 0;
    eb = bb;
L11621:
    if (eb >= *n) {
	goto L11622;
    }
    bb = eb + 1;
    eb = bb;
L11631:
    if (eb >= *n) {
	goto L11632;
    }
    if (x[bb] != x[eb + 1]) {
	goto L11651;
    }
    ++eb;
    goto L11661;
L11651:
    goto L11632;
L11661:
    goto L11631;
L11632:
    if (eb >= *n) {
	goto L11681;
    }
    if (x[eb + 1] - x[eb] >= *del) {
	goto L11701;
    }
    br = eb + 1;
    er = br;
L11711:
    if (er >= *n) {
	goto L11712;
    }
    if (x[er + 1] != x[br]) {
	goto L11731;
    }
    ++er;
    goto L11741;
L11731:
    goto L11712;
L11741:
    goto L11711;
L11712:
/* avoid bounds error: this was .and. but order is not guaranteed */
    if (er < *n) {
	if (x[er + 1] - x[er] < x[eb + 1] - x[eb]) {
	    goto L11621;
	}
    }
    eb = er;
    pw = w[bb] + w[eb];
    px = (x[bb] * w[bb] + x[eb] * w[eb]) / pw;
    py = (y[bb] * w[bb] + y[eb] * w[eb]) / pw;
    i__1 = eb;
    for (i__ = bb; i__ <= i__1; ++i__) {
	x[i__] = px;
	y[i__] = py;
	w[i__] = pw;
/* L11751: */
    }
L11701:
L11681:
L11761:
    if (bb <= 1) {
	goto L11762;
    }
    if (x[bb] - x[bb - 1] >= *del) {
	goto L11762;
    }
    bl = bb - 1;
    el = bl;
L11771:
    if (bl <= 1) {
	goto L11772;
    }
    if (x[bl - 1] != x[el]) {
	goto L11791;
    }
    --bl;
    goto L11801;
L11791:
    goto L11772;
L11801:
    goto L11771;
L11772:
    bb = bl;
    pw = w[bb] + w[eb];
    px = (x[bb] * w[bb] + x[eb] * w[eb]) / pw;
    py = (y[bb] * w[bb] + y[eb] * w[eb]) / pw;
    i__1 = eb;
    for (i__ = bb; i__ <= i__1; ++i__) {
	x[i__] = px;
	y[i__] = py;
	w[i__] = pw;
/* L11811: */
    }
    goto L11761;
L11762:
    goto L11621;
L11622:
    return 0;
} /* pool_ */

/* Subroutine */ int newb_(integer *lm, integer *q, doublereal *ww, 
	doublereal *b)
{
    /* System generated locals */
    integer b_dim1, b_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, l;
    static doublereal s, t;
    static integer l1, lm1;
    static doublereal sml;

/* Common */
    /* Parameter adjustments */
    b_dim1 = *q;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --ww;

    /* Function Body */
    sml = 1. / pprpar_1.big;
    if (*q != 1) {
	goto L11831;
    }
    b[*lm * b_dim1 + 1] = 1.;
    return 0;
L11831:
    if (*lm != 1) {
	goto L11851;
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b[i__ + *lm * b_dim1] = (doublereal) i__;
/* L11861: */
    }
    return 0;
L11851:
    lm1 = *lm - 1;
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b[i__ + *lm * b_dim1] = 0.;
/* L11871: */
    }
    t = 0.;
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = 0.;
	i__2 = lm1;
	for (l = 1; l <= i__2; ++l) {
	    s += (d__1 = b[i__ + l * b_dim1], abs(d__1));
/* L11891: */
	}
	b[i__ + *lm * b_dim1] = s;
	t += s;
/* L11881: */
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b[i__ + *lm * b_dim1] = ww[i__] * (t - b[i__ + *lm * b_dim1]);
/* L11901: */
    }
    l1 = 1;
    if (*lm > *q) {
	l1 = *lm - *q + 1;
    }
    i__1 = lm1;
    for (l = l1; l <= i__1; ++l) {
	s = 0.;
	t = s;
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    s += ww[i__] * b[i__ + *lm * b_dim1] * b[i__ + l * b_dim1];
/* Computing 2nd power */
	    d__1 = b[i__ + l * b_dim1];
	    t += ww[i__] * (d__1 * d__1);
/* L11921: */
	}
	s /= sqrt(t);
	i__2 = *q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    b[i__ + *lm * b_dim1] -= s * b[i__ + l * b_dim1];
/* L11931: */
	}
/* L11911: */
    }
    i__1 = *q;
    for (i__ = 2; i__ <= i__1; ++i__) {
	if ((d__1 = b[i__ - 1 + *lm * b_dim1] - b[i__ + *lm * b_dim1], abs(
		d__1)) > sml) {
	    return 0;
	}
/* L11941: */
    }
    i__1 = *q;
    for (i__ = 1; i__ <= i__1; ++i__) {
	b[i__ + *lm * b_dim1] = (doublereal) i__;
/* L11951: */
    }
    return 0;
} /* newb_ */

/* Subroutine */ int bkppr_(void)
{
    return 0;
} /* bkppr_ */

/* Common Vars */

/* Subroutine */ int setppr_(doublereal *span1, doublereal *alpha1, integer *
	optlevel, integer *ism, doublereal *df1, doublereal *gcvpen1)
{
/* Put 'parameters' into Common blocks */
    pprpar_1.span = *span1;
    pprpar_1.lf = *optlevel;
    pprpar_1.alpha = *alpha1;
    if (*ism >= 0) {
	spsmooth_1.ismethod = *ism;
	spsmooth_1.trace = FALSE_;
    } else {
	spsmooth_1.ismethod = -(*ism + 1);
	spsmooth_1.trace = TRUE_;
    }
    spsmooth_1.df = *df1;
    spsmooth_1.gcvpen = *gcvpen1;
    return 0;
} /* setppr_ */

/* Subroutine */ int fsort_(integer *mu, integer *n, doublereal *f, 
	doublereal *t, doublereal *sp)
{
    /* System generated locals */
    integer f_dim1, f_offset, t_dim1, t_offset, sp_dim1, sp_offset, i__1, 
	    i__2;

    /* Local variables */
    static integer j, k, l;
    extern /* Subroutine */ int sort_(doublereal *, doublereal *, integer *, 
	    integer *);



    /* Parameter adjustments */
    sp_dim1 = *n;
    sp_offset = 1 + sp_dim1;
    sp -= sp_offset;
    t_dim1 = *n;
    t_offset = 1 + t_dim1;
    t -= t_offset;
    f_dim1 = *n;
    f_offset = 1 + f_dim1;
    f -= f_offset;

    /* Function Body */
    i__1 = *mu;
    for (l = 1; l <= i__1; ++l) {
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    sp[j + sp_dim1] = j + .1;
	    sp[j + (sp_dim1 << 1)] = f[j + l * f_dim1];
/* L10: */
	}
	sort_(&t[l * t_dim1 + 1], &sp[sp_offset], &c__1, n);
	i__2 = *n;
	for (j = 1; j <= i__2; ++j) {
	    k = (integer) sp[j + sp_dim1];
	    f[j + l * f_dim1] = sp[k + (sp_dim1 << 1)];
/* L20: */
	}
/* L100: */
    }
    return 0;
} /* fsort_ */

/* Subroutine */ int pppred_(integer *np, doublereal *x, doublereal *smod, 
	doublereal *y, doublereal *sc)
{
    /* System generated locals */
    integer x_dim1, x_offset, y_dim1, y_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, l, m, n, p, q;
    static doublereal s, t;
    static integer ja, jb, jf, jt, mu;
    static doublereal ys;
    static integer jfh, jfl, jth, inp, jtl, low, high, place;
    extern /* Subroutine */ int fsort_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *);

    /* Parameter adjustments */
    y_dim1 = *np;
    y_offset = 1 + y_dim1;
    y -= y_offset;
    x_dim1 = *np;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --smod;
    --sc;

    /* Function Body */
    m = (integer) (smod[1] + .1);
    p = (integer) (smod[2] + .1);
    q = (integer) (smod[3] + .1);
    n = (integer) (smod[4] + .1);
    mu = (integer) (smod[5] + .1);
    ys = smod[q + 6];
    ja = q + 6;
    jb = ja + p * m;
    jf = jb + m * q;
    jt = jf + n * m;
    fsort_(&mu, &n, &smod[jf + 1], &smod[jt + 1], &sc[1]);
    i__1 = *np;
    for (inp = 1; inp <= i__1; ++inp) {
	ja = q + 6;
	jb = ja + p * m;
	jf = jb + m * q;
	jt = jf + n * m;
	i__2 = q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    y[inp + i__ * y_dim1] = 0.;
/* L81: */
	}
	i__2 = mu;
	for (l = 1; l <= i__2; ++l) {
	    s = 0.;
	    i__3 = p;
	    for (j = 1; j <= i__3; ++j) {
		s += smod[ja + j] * x[inp + j * x_dim1];
/* L12201: */
	    }
	    if (s > smod[jt + 1]) {
		goto L12221;
	    }
	    place = 1;
	    goto L12230;
L12221:
	    if (s < smod[jt + n]) {
		goto L12251;
	    }
	    place = n;
	    goto L12230;
L12251:
	    low = 0;
	    high = n + 1;
/*        WHILE */
L12261:
	    if (low + 1 >= high) {
		goto L12262;
	    }
	    place = (low + high) / 2;
	    t = smod[jt + place];
	    if (s == t) {
		goto L12230;
	    }
	    if (s < t) {
		high = place;
	    } else {
		low = place;
	    }
	    goto L12261;
/*        END */
L12262:
	    jfl = jf + low;
	    jfh = jf + high;
	    jtl = jt + low;
	    jth = jt + high;
	    t = smod[jfl] + (smod[jfh] - smod[jfl]) * (s - smod[jtl]) / (smod[
		    jth] - smod[jtl]);
	    goto L12300;
L12230:
	    t = smod[jf + place];
L12300:
	    i__3 = q;
	    for (i__ = 1; i__ <= i__3; ++i__) {
		y[inp + i__ * y_dim1] += smod[jb + i__] * t;
/* L12311: */
	    }
	    ja += p;
	    jb += q;
	    jf += n;
	    jt += n;
/* L91: */
	}
	i__2 = q;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    y[inp + i__ * y_dim1] = ys * y[inp + i__ * y_dim1] + smod[i__ + 5]
		    ;
/* L12321: */
	}
/* L100: */
    }
    return 0;
} /* pppred_ */

/* Called from R's supsmu() */
/* Subroutine */ int setsmu_(integer *tr)
{
    spsmooth_1.ismethod = 0;
    spsmooth_1.trace = *tr != 0;
    return 0;
} /* setsmu_ */

/* Subroutine */ int supsmu_(integer *n, doublereal *x, doublereal *y, 
	doublereal *w, integer *iper, doublereal *span, doublereal *alpha, 
	doublereal *smo, doublereal *sc, doublereal *edf)
{
    /* System generated locals */
    integer sc_dim1, sc_offset, i__1;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static doublereal a, f;
    doublereal h__[*n];
    static integer i__, j;
    static doublereal sw, sy;
    static integer jper;
    static doublereal scale;
    extern /* Subroutine */ int spline_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal resmin;
    extern /* Subroutine */ int smooth_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, integer *, doublereal *, doublereal *
	    , doublereal *);
    static doublereal vsmlsq;


/* ------------------------------------------------------------------ */

/* super smoother (Friedman, 1984). */

/* version 10/10/84 */

/* coded  and copywrite (c) 1984 by: */

/*                        Jerome H. Friedman */
/*                     department of statistics */
/*                               and */
/*                stanford linear accelerator center */
/*                        stanford university */

/* all rights reserved. */


/* input: */
/*    n : number of observations (x,y - pairs). */
/*    x(n) : ordered abscissa values. */
/*    y(n) : corresponding ordinate (response) values. */
/*    w(n) : weight for each (x,y) observation. */
/*    iper : periodic variable flag. */
/*       iper=1 => x is ordered interval variable. */
/*       iper=2 => x is a periodic variable with values */
/*                 in the range (0.0,1.0) and period 1.0. */
/*    span : smoother span (fraction of observations in window). */
/*           span=0.0 <=> "cv" : automatic (variable) span selection. */
/*    alpha : controls high frequency (small span) penality */
/*            used with automatic span selection (bass tone control). */
/*            (alpha.le.0.0 or alpha.gt.10.0 => no effect.) */
/* output: */
/*   smo(n) : smoothed ordinate (response) values. */
/* scratch: */
/*   sc(n,7) : internal working storage. */

/* note: */
/*    for small samples (n < 40) or if there are substantial serial */
/*    correlations between observations close in x - value, then */
/*    a prespecified fixed span smoother (span > 0) should be */
/*    used. reasonable span values are 0.2 to 0.4. */

/* ------------------------------------------------------------------ */
/* Args */
/* Var */
/*     Called from R's supsmu(),  ismethod = 0, always (but not when called from ppr) */
    /* Parameter adjustments */
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --smo;
    --w;
    --y;
    --x;

    /* Function Body */
    if (x[*n] > x[1]) {
	goto L30;
    }
/*     x(n) <= x(1) :  boundary case:  smo[.] :=  weighted mean( y ) */
    sy = 0.;
    sw = sy;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sy += w[j] * y[j];
	sw += w[j];
/* L10: */
    }
    a = 0.;
    if (sw > 0.) {
	a = sy / sw;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	smo[j] = a;
/* L20: */
    }
    return 0;
/*     Normal Case */
L30:
    if (spsmooth_1.ismethod != 0) {
/* possible only when called from ppr() */
	spline_(n, &x[1], &y[1], &w[1], &smo[1], edf, &sc[sc_offset]);
    } else {
	i__ = *n / 4;
	j = i__ * 3;
	scale = x[j] - x[i__];
/* = IQR(x) */
L40:
	if (scale > 0.) {
	    goto L50;
	}
	if (j < *n) {
	    ++j;
	}
	if (i__ > 1) {
	    --i__;
	}
	scale = x[j] - x[i__];
	goto L40;
L50:
/* Computing 2nd power */
	d__1 = consts_1.eps * scale;
	vsmlsq = d__1 * d__1;
	jper = *iper;
	if (*iper == 2 && (x[1] < 0. || x[*n] > 1.)) {
	    jper = 1;
	}
	if (jper < 1 || jper > 2) {
	    jper = 1;
	}
	if (*span > 0.) {
	    smooth_(n, &x[1], &y[1], &w[1], span, &jper, &vsmlsq, &smo[1], &
		    sc[sc_offset]);
	    return 0;
	}
/*     else  "cv" (crossvalidation) from  three spans[] */
	for (i__ = 1; i__ <= 3; ++i__) {
	    smooth_(n, &x[1], &y[1], &w[1], &spans_1.spans[i__ - 1], &jper, &
		    vsmlsq, &sc[((i__ << 1) - 1) * sc_dim1 + 1], &sc[sc_dim1 *
		     7 + 1]);
	    i__1 = -jper;
	    smooth_(n, &x[1], &sc[sc_dim1 * 7 + 1], &w[1], &spans_1.spans[1], 
		    &i__1, &vsmlsq, &sc[(i__ << 1) * sc_dim1 + 1], h__);
/* L70: */
	}
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    resmin = consts_1.big;
	    for (i__ = 1; i__ <= 3; ++i__) {
		if (sc[j + (i__ << 1) * sc_dim1] >= resmin) {
		    goto L80;
		}
		resmin = sc[j + (i__ << 1) * sc_dim1];
		sc[j + sc_dim1 * 7] = spans_1.spans[i__ - 1];
L80:
		;
	    }
	    if (*alpha > 0. && *alpha <= 10. && resmin < sc[j + sc_dim1 * 6] 
		    && resmin > 0.) {
/* Computing MAX */
		d__2 = consts_1.sml, d__3 = resmin / sc[j + sc_dim1 * 6];
		d__1 = max(d__2,d__3);
		d__4 = 10. - *alpha;
		sc[j + sc_dim1 * 7] += (spans_1.spans[2] - sc[j + sc_dim1 * 7]
			) * pow_dd(&d__1, &d__4);
	    }
/* L90: */
	}
	i__1 = -jper;
	smooth_(n, &x[1], &sc[sc_dim1 * 7 + 1], &w[1], &spans_1.spans[1], &
		i__1, &vsmlsq, &sc[(sc_dim1 << 1) + 1], h__);
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    if (sc[j + (sc_dim1 << 1)] <= spans_1.spans[0]) {
		sc[j + (sc_dim1 << 1)] = spans_1.spans[0];
	    }
	    if (sc[j + (sc_dim1 << 1)] >= spans_1.spans[2]) {
		sc[j + (sc_dim1 << 1)] = spans_1.spans[2];
	    }
	    f = sc[j + (sc_dim1 << 1)] - spans_1.spans[1];
	    if (f >= 0.) {
		goto L100;
	    }
	    f = -f / (spans_1.spans[1] - spans_1.spans[0]);
	    sc[j + (sc_dim1 << 2)] = (1. - f) * sc[j + sc_dim1 * 3] + f * sc[
		    j + sc_dim1];
	    goto L110;
L100:
	    f /= spans_1.spans[2] - spans_1.spans[1];
	    sc[j + (sc_dim1 << 2)] = (1. - f) * sc[j + sc_dim1 * 3] + f * sc[
		    j + sc_dim1 * 5];
L110:
	    ;
	}
	i__1 = -jper;
	smooth_(n, &x[1], &sc[(sc_dim1 << 2) + 1], &w[1], spans_1.spans, &
		i__1, &vsmlsq, &smo[1], h__);
	*edf = 0.;
    }
    return 0;
} /* supsmu_ */

/* Subroutine */ int smooth_(integer *n, doublereal *x, doublereal *y, 
	doublereal *w, doublereal *span, integer *iper, doublereal *vsmlsq, 
	doublereal *smo, doublereal *acvr)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Local variables */
    static doublereal a, h__;
    static integer i__, j, j0;
    extern /* Subroutine */ int smoothprt_(doublereal *, integer *, 
	    doublereal *, doublereal *);
    static integer in, it;
    static doublereal xm, ym, wt, sy, fbo, fbw;
    static integer ibw;
    static doublereal var, tmp, xti;
    static integer out;
    static doublereal xto, cvar;
    static integer jper;

/* Args */
/* Var */
/* will use 'trace': */
    /* Parameter adjustments */
    --acvr;
    --smo;
    --w;
    --y;
    --x;

    /* Function Body */
    xm = 0.;
    ym = xm;
    var = ym;
    cvar = var;
    fbw = cvar;
    jper = abs(*iper);
    ibw = (integer) (*span * .5 * *n + .5);
    if (ibw < 2) {
	ibw = 2;
    }
    it = (ibw << 1) + 1;
    if (it > *n) {
	it = *n;
    }
    i__1 = it;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = i__;
	if (jper == 2) {
	    j = i__ - ibw - 1;
	}
	if (j >= 1) {
	    xti = x[j];
	} else {
/* if (j.lt.1) then */
	    j = *n + j;
	    xti = x[j] - 1.;
	}
	wt = w[j];
	fbo = fbw;
	fbw += wt;
	if (fbw > 0.) {
	    xm = (fbo * xm + wt * xti) / fbw;
	}
	if (fbw > 0.) {
	    ym = (fbo * ym + wt * y[j]) / fbw;
	}
	tmp = 0.;
	if (fbo > 0.) {
	    tmp = fbw * wt * (xti - xm) / fbo;
	}
	var += tmp * (xti - xm);
	cvar += tmp * (y[j] - ym);
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	out = j - ibw - 1;
	in = j + ibw;
	if (jper != 2 && (out < 1 || in > *n)) {
	    goto L60;
	}
	if (out < 1) {
	    out = *n + out;
	    xto = x[out] - 1.;
	    xti = x[in];
	} else if (in > *n) {
	    in -= *n;
	    xti = x[in] + 1.;
	    xto = x[out];
	} else {
	    xto = x[out];
	    xti = x[in];
	}
	wt = w[out];
	fbo = fbw;
	fbw -= wt;
	tmp = 0.;
	if (fbw > 0.) {
	    tmp = fbo * wt * (xto - xm) / fbw;
	}
	var -= tmp * (xto - xm);
	cvar -= tmp * (y[out] - ym);
	if (fbw > 0.) {
	    xm = (fbo * xm - wt * xto) / fbw;
	}
	if (fbw > 0.) {
	    ym = (fbo * ym - wt * y[out]) / fbw;
	}
	wt = w[in];
	fbo = fbw;
	fbw += wt;
	if (fbw > 0.) {
	    xm = (fbo * xm + wt * xti) / fbw;
	}
	if (fbw > 0.) {
	    ym = (fbo * ym + wt * y[in]) / fbw;
	}
	tmp = 0.;
	if (fbo > 0.) {
	    tmp = fbw * wt * (xti - xm) / fbo;
	}
	var += tmp * (xti - xm);
	cvar += tmp * (y[in] - ym);
L60:
	a = 0.;
	if (var > *vsmlsq) {
	    a = cvar / var;
	}
	smo[j] = a * (x[j] - xm) + ym;
	if (*iper > 0) {
	    h__ = 0.;
	    if (fbw > 0.) {
		h__ = 1. / fbw;
	    }
	    if (var > *vsmlsq) {
/* Computing 2nd power */
		d__1 = x[j] - xm;
		h__ += d__1 * d__1 / var;
	    }
	    acvr[j] = 0.;
	    a = 1. - w[j] * h__;
	    if (a > 0.) {
		acvr[j] = (d__1 = y[j] - smo[j], abs(d__1)) / a;
	    } else if (j > 1) {
		acvr[j] = acvr[j - 1];
	    }
	}
/* L80: */
    }
    if (spsmooth_1.trace) {
	smoothprt_(span, iper, &var, &cvar);
    }
/* -- Recompute fitted values smo(j) as weighted mean for non-unique x(.) values: */
/* -> ./ksmooth.c */
    j = 1;
L90:
    j0 = j;
    sy = smo[j] * w[j];
    fbw = w[j];
    if (j >= *n) {
	goto L110;
    }
L100:
    if (x[j + 1] <= x[j]) {
	++j;
	sy += w[j] * smo[j];
	fbw += w[j];
	if (j < *n) {
	    goto L100;
	}
    }
L110:
    if (j > j0) {
	a = 0.;
	if (fbw > 0.) {
	    a = sy / fbw;
	}
	i__1 = j;
	for (i__ = j0; i__ <= i__1; ++i__) {
	    smo[i__] = a;
	}
    }
    ++j;
    if (j <= *n) {
	goto L90;
    }
    return 0;
} /* smooth_ */

/* Subroutine */ int bksupsmu_(void)
{
    return 0;
} /* bksupsmu_ */


/* --------------------------------------------------------------- */

/* this sets the compile time (default) values for various */
/* internal parameters : */

/* spans : span values for the three running linear smoothers. */
/* spans(1) : tweeter span. */
/* spans(2) : midrange span. */
/* spans(3) : woofer span. */
/*       (these span values should be changed only with care.) */
/* big : a large representable floating point number. */
/* sml : a small number. should be set so that (sml)**(10.0) does */
/*       not cause floating point underflow. */
/* eps : used to numerically stabilize slope calculations for */
/*       running linear fits. */

/* these parameter values can be changed by declaring the */
/* relevant labeled common in the main program and resetting */
/* them with executable statements. */

/* Only for  ppr(*, ismethod != 0):  Compute "smoothing" spline */
/*  (rather, a penalized regression spline with at most 15 (inner) knots): */
/* ----------------------------------------------------------------- */

/* Subroutine */ int spline_(integer *n, doublereal *x, doublereal *y, 
	doublereal *w, doublereal *smo, doublereal *edf, doublereal *sc)
{
    /* System generated locals */
    integer sc_dim1, sc_offset;

    /* Local variables */
    extern /* Subroutine */ int splineaa_(integer *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *);


/* ------------------------------------------------------------------ */

/* input: */
/*    n : number of observations. */
/*    x(n) : ordered abscissa values. */
/*    y(n) : corresponding ordinate (response) values. */
/*    w(n) : weight for each (x,y) observation. */
/* work space: */
/*    sc(n,7) : used for dx(n), dy(n), dw(n), dsmo(n), lev(n) */
/* output: */
/*   smo(n) : smoothed ordinate (response) values. */
/*   edf : equivalent degrees of freedom */

/* ------------------------------------------------------------------ */
/* Args */
    /* Parameter adjustments */
    sc_dim1 = *n;
    sc_offset = 1 + sc_dim1;
    sc -= sc_offset;
    --smo;
    --w;
    --y;
    --x;

    /* Function Body */
    splineaa_(n, &x[1], &y[1], &w[1], &smo[1], edf, &sc[*n + sc_dim1], &sc[*n 
	    + (sc_dim1 << 1)], &sc[*n + sc_dim1 * 3], &sc[*n + (sc_dim1 << 2)]
	    , &sc[*n + sc_dim1 * 5]);
/* dx */
/* dy */
/* dw */
/* dsmo */
/* lev */
    return 0;
} /* spline_ */

/* Subroutine */ int splineaa_(integer *n, doublereal *x, doublereal *y, 
	doublereal *w, doublereal *smo, doublereal *edf, doublereal *dx, 
	doublereal *dy, doublereal *dw, doublereal *dsmo, doublereal *lev)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static doublereal p, s;
    extern /* Subroutine */ int splineprt_(doublereal *, doublereal *, 
	    integer *, doublereal *, doublereal *);
    static integer nk, ip;
    static doublereal df1;
    static integer ier;
    static doublereal coef[25], crit, knot[29], work[1050], param[5];
    extern /* Subroutine */ int rbart_(doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *, doublereal *, doublereal *
	    , integer *, integer *, integer *), intpr_(char *, integer *, 
	    integer *, integer *, ftnlen);
    static doublereal lambda;
    static integer iparms[4];


/*  Workhorse of spline() above */
/* ------------------------------------------------------------------ */

/* Additional input variables (no extra output, work): */
/*     dx : */
/*     dy : */
/*     dw : */
/*     dsmo: */
/*     lev : "leverages", i.e., diagonal entries S_{i,i} of the smoother matrix */

/* ------------------------------------------------------------------ */
/* Args */
/* Var */
/* __no-more__ if (n .gt. 2500) call bdrsplerr() */
    /* Parameter adjustments */
    --lev;
    --dsmo;
    --dw;
    --dy;
    --dx;
    --smo;
    --w;
    --y;
    --x;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dx[i__] = (x[i__] - x[1]) / (x[*n] - x[1]);
	dy[i__] = y[i__];
	dw[i__] = w[i__];
    }
    nk = min(*n,15);
    knot[0] = dx[1];
    knot[1] = dx[1];
    knot[2] = dx[1];
    knot[3] = dx[1];
    knot[nk] = dx[*n];
    knot[nk + 1] = dx[*n];
    knot[nk + 2] = dx[*n];
    knot[nk + 3] = dx[*n];
    i__1 = nk;
    for (i__ = 5; i__ <= i__1; ++i__) {
	p = (*n - 1) * (real) (i__ - 4) / (real) (nk - 3);
	ip = (integer) p;
	p -= ip;
	knot[i__ - 1] = (1 - p) * dx[ip + 1] + p * dx[ip + 2];
    }
/*      call dblepr('knots', 5, knot, nk+4) */
/*     iparms(1:2) := (icrit, ispar)  for ./sbart.c */
    if (spsmooth_1.ismethod == 1) {
	iparms[0] = 3;
	df1 = spsmooth_1.df;
    } else {
	iparms[0] = 1;
	df1 = 0.;
    }

    iparms[1] = 0;
/* ispar := 0 <==> estimate `spar' */
    iparms[2] = 500;
/* maxit = 500 */
    iparms[3] = 0;

/* spar (!= lambda) */
    param[0] = 0.;
/* = lspar : min{spar} */
    param[1] = 1.5;
/*     tol for 'spar' estimation: */
/* = uspar : max{spar} */
    param[2] = .01;
/*     'eps' (~= 2^-12 = sqrt(2^-24) ?= sqrt(machine eps)) in ./sbart.c : */
    param[3] = 2.44e-4f;
    ier = 1;
    rbart_(&spsmooth_1.gcvpen, &df1, &dx[1], &dy[1], &dw[1], &c_b183, n, knot,
	     &nk, coef, &dsmo[1], &lev[1], &crit, iparms, &lambda, param, 
	    work, &c__4, &c__1, &ier);
    if (ier > 0) {
	//intpr_("spline(.) TROUBLE:", &c__18, &ier, &c__1, (ftnlen)18);
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	smo[i__] = dsmo[i__];
    }
/*      call dblepr('smoothed',8, dsmo, n) */
    s = 0.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s += lev[i__];
    }
    *edf = s;
    if (spsmooth_1.trace) {
	splineprt_(&spsmooth_1.df, &spsmooth_1.gcvpen, &spsmooth_1.ismethod, &
		lambda, edf);
    }
    return 0;
} /* splineaa_ */

/* ********************************************************************** */
/* === This was 'sort()' in  gamfit's  mysort.f  [or sortdi() in sortdi.f ] : */

/* ===  FIXME:  Translate to C and add to ../../../main/sort.c <<<<< */

/*     a[] is double precision because the caller reuses a double (sometimes v[] itself!) */
/* Subroutine */ int sort_(doublereal *v, doublereal *a, integer *ii, integer 
	*jj)
{
    static integer i__, j, k, l, m, t, ij, il[20], iu[20], tt;
    static doublereal vt, vtt;


/*     Puts into a the permutation vector which sorts v into */
/*     increasing order.  Only elements from ii to jj are considered. */
/*     Arrays iu(k) and il(k) permit sorting up to 2**(k+1)-1 elements */

/*     This is a modification of CACM algorithm #347 by R. C. Singleton, */
/*     which is a modified Hoare quicksort. */


    /* Parameter adjustments */
    --v;
    --a;

    /* Function Body */
    m = 1;
    i__ = *ii;
    j = *jj;
L10:
    if (i__ >= j) {
	goto L80;
    }
L20:
    k = i__;
    ij = (j + i__) / 2;
    t = (integer) a[ij];
    vt = v[ij];
    if (v[i__] <= vt) {
	goto L30;
    }
    a[ij] = a[i__];
    a[i__] = (doublereal) t;
    t = (integer) a[ij];
    v[ij] = v[i__];
    v[i__] = vt;
    vt = v[ij];
L30:
    l = j;
    if (v[j] >= vt) {
	goto L50;
    }
    a[ij] = a[j];
    a[j] = (doublereal) t;
    t = (integer) a[ij];
    v[ij] = v[j];
    v[j] = vt;
    vt = v[ij];
    if (v[i__] <= vt) {
	goto L50;
    }
    a[ij] = a[i__];
    a[i__] = (doublereal) t;
    t = (integer) a[ij];
    v[ij] = v[i__];
    v[i__] = vt;
    vt = v[ij];
    goto L50;
L40:
    a[l] = a[k];
    a[k] = (doublereal) tt;
    v[l] = v[k];
    v[k] = vtt;
L50:
    --l;
    if (v[l] > vt) {
	goto L50;
    }
    tt = (integer) a[l];
    vtt = v[l];
L60:
    ++k;
    if (v[k] < vt) {
	goto L60;
    }
    if (k <= l) {
	goto L40;
    }
    if (l - i__ <= j - k) {
	goto L70;
    }
    il[m - 1] = i__;
    iu[m - 1] = l;
    i__ = k;
    ++m;
    goto L90;
L70:
    il[m - 1] = k;
    iu[m - 1] = j;
    j = l;
    ++m;
    goto L90;
L80:
    --m;
    if (m == 0) {
	return 0;
    }
    i__ = il[m - 1];
    j = iu[m - 1];
L90:
    if (j - i__ > 10) {
	goto L20;
    }
    if (i__ == *ii) {
	goto L10;
    }
    --i__;
L100:
    ++i__;
    if (i__ == j) {
	goto L80;
    }
    t = (integer) a[i__ + 1];
    vt = v[i__ + 1];
    if (v[i__] <= vt) {
	goto L100;
    }
    k = i__;
L110:
    a[k + 1] = a[k];
    v[k + 1] = v[k];
    --k;
    if (vt < v[k]) {
	goto L110;
    }
    a[k + 1] = (doublereal) t;
    v[k + 1] = vt;
    goto L100;
} /* sort_ */

