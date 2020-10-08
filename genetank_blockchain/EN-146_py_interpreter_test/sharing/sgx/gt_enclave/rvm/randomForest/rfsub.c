/* rfsub.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "./rf.h"

#define min(a,b) (((a)>(b))?(b):(a))

/* Table of constant values */

static int c__53 = 53;

/*     Copyright (C) 2001-7  Leo Breiman and Adele Cutler and Merck & Co, Inc. */
/*     This program is free software; you can redistribute it and/or */
/*     modify it under the terms of the GNU General Public License */
/*     as published by the Free Software Foundation; either version 2 */
/*     of the License, or (at your option) any later version. */
/*     This program is distributed in the hope that it will be useful, */
/*     but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*     GNU General Public License for more details. */

/*     Modified by Andy Liaw and Matt Wiener: */
/*     The main program is re-written as a C function to be called from R. */
/*     All calls to the uniform RNG is replaced with R's RNG.  Some subroutines */
/*     not called are excluded.  Variables and arrays declared as double as */
/*     needed.  Unused variables are deleted. */

/*     SUBROUTINE BUILDTREE */
/* Subroutine */ int buildtree_(int *a, int *b, int *cl, int *
	cat, int *maxcat, int *mdim, int *nsample, int *
	nclass, int *treemap, int *bestvar, int *bestsplit, 
	int *bestsplitnext, double *tgini, int *nodestatus, 
	int *nodepop, int *nodestart, double *classpop, 
	double *tclasspop, double *tclasscat, int *ta, int *
	nrnodes, int *idmove, int *ndsize, int *ncase, int *
	mtry, int *iv, int *nodeclass, int *ndbigtree, double 
	*win, double *wr, double *wl, int *mred, int *nuse, 
	int *mind)
{
    /* System generated locals */
    int a_dim1, a_offset, b_dim1, b_offset, classpop_dim1, 
	    classpop_offset, tclasscat_dim1, tclasscat_offset, i__1, i__2;

    /* Local variables */
    extern /* Subroutine */ int movedata_(int *, int *, int *, 
	    int *, int *, int *, int *, int *, int *, 
	    int *, double *, int *);
    static double decsplit;
    static int j, k, n, nc, kn;
    static double pp, best;
    static int ntie, ncur;
    extern /* Subroutine */ int zerv_(int *, int *);
    static double popt1, popt2;
    static int ndend;
    extern /* Subroutine */ int rrand_(double *);
    static double xrand;
    static int jstat;
    extern /* Subroutine */ int zermr_(double *, int *, int *), 
	    findbestsplit_(int *, int *, int *, int *, 
	    int *, int *, int *, int *, int *, int *, 
	    double *, double *, int *, double *, double *,
	     int *, int *, int *, double *, double *, 
	    double *, int *, int *);
    static int ndendl, kbuild, msplit, ndstart;

/*     Buildtree consists of repeated calls to two subroutines, Findbestsplit */
/*     and Movedata.  Findbestsplit does just that--it finds the best split of */
/*     the current node.  Movedata moves the data in the split node right and */
/*     left so that the data corresponding to each child node is contiguous. */
/*     The buildtree bookkeeping is different from that in Friedman's original */
/*     CART program.  ncur is the total number of nodes to date. */
/*     nodestatus(k)=1 if the kth node has been split.  nodestatus(k)=2 if the */
/*     node exists but has not yet been split, and =-1 of the node is terminal. */
/*     A node is terminal if its size is below a threshold value, or if it is */
/*     all one class, or if all the x-values are equal.  If the current node k */
/*     is split, then its children are numbered ncur+1 (left), and */
/*     ncur+2(right), ncur increases to ncur+2 and the next node to be split is */
/*     numbered k+1.  When no more nodes can be split, buildtree returns to the */
/*     main program. */
    /* Parameter adjustments */
    --tgini;
    --cat;
    --win;
    --ncase;
    --idmove;
    --ta;
    --cl;
    b_dim1 = *mdim;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    a_dim1 = *mdim;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --wl;
    --wr;
    tclasscat_dim1 = *nclass;
    tclasscat_offset = 1 + tclasscat_dim1;
    tclasscat -= tclasscat_offset;
    --tclasspop;
    --nodeclass;
    classpop_dim1 = *nclass;
    classpop_offset = 1 + classpop_dim1;
    classpop -= classpop_offset;
    --nodestart;
    --nodepop;
    --nodestatus;
    --bestsplitnext;
    --bestsplit;
    --bestvar;
    treemap -= 3;
    --mind;
    --iv;

    /* Function Body */
    msplit = 0;
    zerv_(&nodestatus[1], nrnodes);
    zerv_(&nodestart[1], nrnodes);
    zerv_(&nodepop[1], nrnodes);
    zermr_(&classpop[classpop_offset], nclass, nrnodes);
    i__1 = *nclass;
    for (j = 1; j <= i__1; ++j) {
	classpop[j + classpop_dim1] = tclasspop[j];
    }
    ncur = 1;
    nodestart[1] = 1;
    nodepop[1] = *nuse;
    nodestatus[1] = 2;
/*     start main loop */
    i__1 = *nrnodes;
    for (kbuild = 1; kbuild <= i__1; ++kbuild) {
/*         call intpr("kbuild", 6, kbuild, 1) */
/*         call intpr("ncur", 4, ncur, 1) */
	if (kbuild > ncur) {
	    goto L50;
	}
	if (nodestatus[kbuild] != 2) {
	    goto L30;
	}
/*     initialize for next call to findbestsplit */
	ndstart = nodestart[kbuild];
	ndend = ndstart + nodepop[kbuild] - 1;
	i__2 = *nclass;
	for (j = 1; j <= i__2; ++j) {
	    tclasspop[j] = classpop[j + kbuild * classpop_dim1];
	}
	jstat = 0;
	findbestsplit_(&a[a_offset], &b[b_offset], &cl[1], mdim, nsample, 
		nclass, &cat[1], maxcat, &ndstart, &ndend, &tclasspop[1], &
		tclasscat[tclasscat_offset], &msplit, &decsplit, &best, &
		ncase[1], &jstat, mtry, &win[1], &wr[1], &wl[1], mred, &mind[
		1]);
/*         call intpr("jstat", 5, jstat, 1) */
/*         call intpr("msplit", 6, msplit, 1) */
/*     If the node is terminal, move on.  Otherwise, split. */
	if (jstat == -1) {
	    nodestatus[kbuild] = -1;
	    goto L30;
	} else {
	    bestvar[kbuild] = msplit;
	    iv[msplit] = 1;
	    if (decsplit < 0.f) {
		decsplit = 0.f;
	    }
	    tgini[msplit] += decsplit;
	    if (cat[msplit] == 1) {
		bestsplit[kbuild] = (int) ((double) a[msplit + (
			int) best * a_dim1]);
		bestsplitnext[kbuild] = (int) ((double) a[msplit + ((
			int) best + 1) * a_dim1]);
	    } else {
		bestsplit[kbuild] = (int) best;
		bestsplitnext[kbuild] = 0.f;
	    }
	}
	movedata_(&a[a_offset], &ta[1], mdim, nsample, &ndstart, &ndend, &
		idmove[1], &ncase[1], &msplit, &cat[1], &best, &ndendl);
/*         call intpr("ndend", 5, ndend, 1) */
/*         call intpr("ndendl", 6, ndendl, 1) */
/*     leftnode no.= ncur+1, rightnode no. = ncur+2. */
	nodepop[ncur + 1] = ndendl - ndstart + 1;
	nodepop[ncur + 2] = ndend - ndendl;
	nodestart[ncur + 1] = ndstart;
	nodestart[ncur + 2] = ndendl + 1;
/*     find class populations in both nodes */
	i__2 = ndendl;
	for (n = ndstart; n <= i__2; ++n) {
	    nc = ncase[n];
	    j = cl[nc];
	    classpop[j + (ncur + 1) * classpop_dim1] += win[nc];
	}
	i__2 = ndend;
	for (n = ndendl + 1; n <= i__2; ++n) {
	    nc = ncase[n];
	    j = cl[nc];
	    classpop[j + (ncur + 2) * classpop_dim1] += win[nc];
	}
/*         call intpr("nL", 2, nodepop(ncur+1), 1) */
/*         call intpr("nR", 2, nodepop(ncur+2), 1) */
/*     check on nodestatus */
	nodestatus[ncur + 1] = 2;
	nodestatus[ncur + 2] = 2;
	if (nodepop[ncur + 1] <= *ndsize) {
	    nodestatus[ncur + 1] = -1;
	}
	if (nodepop[ncur + 2] <= *ndsize) {
	    nodestatus[ncur + 2] = -1;
	}
	popt1 = 0.;
	popt2 = 0.;
	i__2 = *nclass;
	for (j = 1; j <= i__2; ++j) {
	    popt1 += classpop[j + (ncur + 1) * classpop_dim1];
	    popt2 += classpop[j + (ncur + 2) * classpop_dim1];
	}
	i__2 = *nclass;
	for (j = 1; j <= i__2; ++j) {
	    if (classpop[j + (ncur + 1) * classpop_dim1] == popt1) {
		nodestatus[ncur + 1] = -1;
	    }
	    if (classpop[j + (ncur + 2) * classpop_dim1] == popt2) {
		nodestatus[ncur + 2] = -1;
	    }
	}
	treemap[(kbuild << 1) + 1] = ncur + 1;
	treemap[(kbuild << 1) + 2] = ncur + 2;
	nodestatus[kbuild] = 1;
	ncur += 2;
	if (ncur >= *nrnodes) {
	    goto L50;
	}
L30:
	;
    }
L50:
    *ndbigtree = *nrnodes;
    for (k = *nrnodes; k >= 1; --k) {
	if (nodestatus[k] == 0) {
	    --(*ndbigtree);
	}
	if (nodestatus[k] == 2) {
	    nodestatus[k] = -1;
	}
    }
/*     form prediction in terminal nodes */
    i__1 = *ndbigtree;
    for (kn = 1; kn <= i__1; ++kn) {
	if (nodestatus[kn] == -1) {
	    pp = 0.;
	    ntie = 1;
	    i__2 = *nclass;
	    for (j = 1; j <= i__2; ++j) {
		if (classpop[j + kn * classpop_dim1] > pp) {
		    nodeclass[kn] = j;
		    pp = classpop[j + kn * classpop_dim1];
		    ntie = 1;
		}
/*     Break ties at random: */
		if (classpop[j + kn * classpop_dim1] == pp) {
		    rrand_(&xrand);
		    if (xrand < 1.f / ntie) {
			nodeclass[kn] = j;
			pp = classpop[j + kn * classpop_dim1];
		    }
		    ++ntie;
		}
	    }
	}
/*         call intpr("node", 4, kn, 1) */
/*         call intpr("status", 6, nodestatus(kn), 1) */
/*         call intpr("pred", 4, nodeclass(kn), 1) */
/*         call dblepr("pop1", 4, classpop(1, kn), 1) */
/*         call dblepr("pop2", 4, classpop(2, kn), 1) */
    }
    return 0;
} /* buildtree_ */

/*     SUBROUTINE FINDBESTSPLIT */
/*     For the best split, msplit is the variable split on. decsplit is the */
/*     dec. in impurity.  If msplit is numerical, nsplit is the case number */
/*     of value of msplit split on, and nsplitnext is the case number of the */
/*     next larger value of msplit.  If msplit is categorical, then nsplit is */
/*     the coding into an int of the categories going left. */
/* Subroutine */ int findbestsplit_(int *a, int *b, int *cl, 
	int *mdim, int *nsample, int *nclass, int *cat, 
	int *maxcat, int *ndstart, int *ndend, double *
	tclasspop, double *tclasscat, int *msplit, double *
	decsplit, double *best, int *ncase, int *jstat, int *
	mtry, double *win, double *wr, double *wl, int *mred, 
	int *mind)
{
    /* System generated locals */
    int a_dim1, a_offset, b_dim1, b_offset, tclasscat_dim1, 
	    tclasscat_offset, i__1, i__2, i__3;

    /* Local variables */
    static int i__, j, k, l;
    static double u;
    static int nc;
    static double dn[53];
    static int nn, mt;
    static double su, rld, pdo, rrd, rln, pno;
    static int nsp;
    static double rrn;
    static int nnz, lcat, ntie;
    static double crit;
    static int nhit, mvar;
    static double crit0;
    static int ncmax;
    extern /* Subroutine */ int rrand_(double *);
    static double xrand;
    extern /* Subroutine */ int zermr_(double *, int *, int *), 
	    zervr_(double *, int *), catmax_(double *, double 
	    *, double *, int *, int *, double *, double *,
	     int *, int *, int *, int *), catmaxb_(double 
	    *, double *, double *, int *, int *, double *,
	     double *, int *, double *);
    static double critmax;
    static int ncsplit;

    /* Parameter adjustments */
    --cat;
    --win;
    --ncase;
    --cl;
    b_dim1 = *mdim;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    a_dim1 = *mdim;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --wl;
    --wr;
    tclasscat_dim1 = *nclass;
    tclasscat_offset = 1 + tclasscat_dim1;
    tclasscat -= tclasscat_offset;
    --tclasspop;
    --mind;

    /* Function Body */
    ncmax = 10;
    ncsplit = 512;
/*     compute initial values of numerator and denominator of Gini */
    pno = 0.f;
    pdo = 0.f;
    i__1 = *nclass;
    for (j = 1; j <= i__1; ++j) {
	pno += tclasspop[j] * tclasspop[j];
	pdo += tclasspop[j];
    }
    crit0 = pno / pdo;
    *jstat = 0;
/*     start main loop through variables to find best split */
    critmax = -1e25f;
    i__1 = *mred;
    for (k = 1; k <= i__1; ++k) {
	mind[k] = k;
    }
    nn = *mred;
/*     sampling mtry variables w/o replacement. */
    i__1 = *mtry;
    for (mt = 1; mt <= i__1; ++mt) {
	rrand_(&xrand);
	j = (int) (nn * xrand) + 1;
	mvar = mind[j];
	mind[j] = mind[nn];
	mind[nn] = mvar;
	--nn;
	lcat = cat[mvar];
	if (lcat == 1) {
/*     Split on a numerical predictor. */
	    rrn = pno;
	    rrd = pdo;
	    rln = 0.;
	    rld = 0.;
	    zervr_(&wl[1], nclass);
	    i__2 = *nclass;
	    for (j = 1; j <= i__2; ++j) {
		wr[j] = tclasspop[j];
	    }
	    ntie = 1;
	    i__2 = *ndend - 1;
	    for (nsp = *ndstart; nsp <= i__2; ++nsp) {
		nc = a[mvar + nsp * a_dim1];
		u = win[nc];
		k = cl[nc];
		rln += u * (wl[k] * 2 + u);
		rrn += u * (wr[k] * -2 + u);
		rld += u;
		rrd -= u;
		wl[k] += u;
		wr[k] -= u;
		if (b[mvar + nc * b_dim1] < b[mvar + a[mvar + (nsp + 1) * 
			a_dim1] * b_dim1]) {
/*     If neither nodes is empty, check the split. */
		    if (min(rrd,rld) > 1e-5f) {
			crit = rln / rld + rrn / rrd;
			if (crit > critmax) {
			    *best = (double) nsp;
			    critmax = crit;
			    *msplit = mvar;
			    ntie = 1;
			}
/*     Break ties at random: */
			if (crit == critmax) {
			    rrand_(&xrand);
			    if (xrand < 1.f / ntie) {
				*best = (double) nsp;
				critmax = crit;
				*msplit = mvar;
			    }
			    ++ntie;
			}
		    }
		}
	    }
	} else {
/*     Split on a categorical predictor.  Compute the decrease in impurity. */
	    zermr_(&tclasscat[tclasscat_offset], nclass, &c__53);
	    i__2 = *ndend;
	    for (nsp = *ndstart; nsp <= i__2; ++nsp) {
		nc = ncase[nsp];
		l = a[mvar + ncase[nsp] * a_dim1];
		tclasscat[cl[nc] + l * tclasscat_dim1] += win[nc];
	    }
	    nnz = 0;
	    i__2 = lcat;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		su = 0.;
		i__3 = *nclass;
		for (j = 1; j <= i__3; ++j) {
		    su += tclasscat[j + i__ * tclasscat_dim1];
		}
		dn[i__ - 1] = su;
		if (su > 0.) {
		    ++nnz;
		}
	    }
	    nhit = 0;
	    if (nnz > 1) {
		if (*nclass == 2 && lcat > ncmax) {
		    catmaxb_(&pdo, &tclasscat[tclasscat_offset], &tclasspop[1]
			    , nclass, &lcat, best, &critmax, &nhit, dn);
		} else {
		    catmax_(&pdo, &tclasscat[tclasscat_offset], &tclasspop[1],
			     nclass, &lcat, best, &critmax, &nhit, maxcat, &
			    ncmax, &ncsplit);
		}
		if (nhit == 1) {
		    *msplit = mvar;
		}
/*            else */
/*               critmax = -1.0e25 */
	    }
	}
    }
    if (critmax < -1e10f || *msplit == 0) {
	*jstat = -1;
    }
    *decsplit = critmax - crit0;
    return 0;
} /* findbestsplit_ */

/*     ============================================================== */
/*     SUBROUTINE MOVEDATA */
/*     This subroutine is the heart of the buildtree construction. Based on the */
/*     best split the data in the part of the a matrix corresponding to the */
/*     current node is moved to the left if it belongs to the left child and */
/*     right if it belongs to the right child. */
/* Subroutine */ int movedata_(int *a, int *ta, int *mdim, 
	int *nsample, int *ndstart, int *ndend, int *idmove, 
	int *ncase, int *msplit, int *cat, double *best, 
	int *ndendl)
{
    /* System generated locals */
    int a_dim1, a_offset, i__1, i__2;

    /* Local variables */
    static int k, l, n, nc, ih, ndo, msh, nsp, icat[53];
    extern /* Subroutine */ int unpack_(double *, int *, int *);

/*     compute idmove=indicator of case nos. going left */
    /* Parameter adjustments */
    --cat;
    --ncase;
    --idmove;
    --ta;
    a_dim1 = *mdim;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    if (cat[*msplit] == 1) {
	i__1 = (int) (*best);
	for (nsp = *ndstart; nsp <= i__1; ++nsp) {
	    nc = a[*msplit + nsp * a_dim1];
	    idmove[nc] = 1;
	}
	i__1 = *ndend;
	for (nsp = (int) (*best) + 1; nsp <= i__1; ++nsp) {
	    nc = a[*msplit + nsp * a_dim1];
	    idmove[nc] = 0;
	}
	*ndendl = (int) (*best);
    } else {
	*ndendl = *ndstart - 1;
	l = cat[*msplit];
	unpack_(best, &l, icat);
	i__1 = *ndend;
	for (nsp = *ndstart; nsp <= i__1; ++nsp) {
	    nc = ncase[nsp];
	    if (icat[a[*msplit + nc * a_dim1] - 1] == 1) {
		idmove[nc] = 1;
		++(*ndendl);
	    } else {
		idmove[nc] = 0;
	    }
	}
    }
/*     shift case. nos. right and left for numerical variables. */
    i__1 = *mdim;
    for (msh = 1; msh <= i__1; ++msh) {
	if (cat[msh] == 1) {
	    k = *ndstart - 1;
	    i__2 = *ndend;
	    for (n = *ndstart; n <= i__2; ++n) {
		ih = a[msh + n * a_dim1];
		if (idmove[ih] == 1) {
		    ++k;
		    ta[k] = a[msh + n * a_dim1];
		}
/* L50: */
	    }
	    i__2 = *ndend;
	    for (n = *ndstart; n <= i__2; ++n) {
		ih = a[msh + n * a_dim1];
		if (idmove[ih] == 0) {
		    ++k;
		    ta[k] = a[msh + n * a_dim1];
		}
/* L60: */
	    }
	    i__2 = *ndend;
	    for (k = *ndstart; k <= i__2; ++k) {
		a[msh + k * a_dim1] = ta[k];
/* L70: */
	    }
	}
/* L40: */
    }
    ndo = 0;
    if (ndo == 1) {
	i__1 = *mdim;
	for (msh = 1; msh <= i__1; ++msh) {
	    if (cat[msh] > 1) {
		k = *ndstart - 1;
		i__2 = *ndend;
		for (n = *ndstart; n <= i__2; ++n) {
		    ih = ncase[n];
		    if (idmove[ih] == 1) {
			++k;
			ta[k] = a[msh + ih * a_dim1];
		    }
/* L150: */
		}
		i__2 = *ndend;
		for (n = *ndstart; n <= i__2; ++n) {
		    ih = ncase[n];
		    if (idmove[ih] == 0) {
			++k;
			ta[k] = a[msh + ih * a_dim1];
		    }
/* L160: */
		}
		i__2 = *ndend;
		for (k = *ndstart; k <= i__2; ++k) {
		    a[msh + k * a_dim1] = ta[k];
/* L170: */
		}
	    }
/* L140: */
	}
    }
/*     compute case nos. for right and left nodes. */
    if (cat[*msplit] == 1) {
	i__1 = *ndend;
	for (n = *ndstart; n <= i__1; ++n) {
	    ncase[n] = a[*msplit + n * a_dim1];
/* L80: */
	}
    } else {
	k = *ndstart - 1;
	i__1 = *ndend;
	for (n = *ndstart; n <= i__1; ++n) {
	    if (idmove[ncase[n]] == 1) {
		++k;
		ta[k] = ncase[n];
	    }
/* L90: */
	}
	i__1 = *ndend;
	for (n = *ndstart; n <= i__1; ++n) {
	    if (idmove[ncase[n]] == 0) {
		++k;
		ta[k] = ncase[n];
	    }
/* L100: */
	}
	i__1 = *ndend;
	for (k = *ndstart; k <= i__1; ++k) {
	    ncase[k] = ta[k];
/* L110: */
	}
    }
    return 0;
} /* movedata_ */

/*      subroutine myunpack(l,npack,icat) */

/*     npack is a long int.  The sub. returns icat, an int of zeroes and */
/*     ones corresponding to the coefficients in the binary expansion of npack. */

/*      int icat(53),npack */
/*      do j=1,32 */
/*         icat(j)=0 */
/*      end do */
/*      n=npack */
/*      icat(1)=mod(n,2) */
/*      do k=2,l */
/*         n=(n-icat(k-1))/2 */
/*         icat(k)=mod(n,2) */
/*      end do */
/*      end */
/* Subroutine */ int zerv_(int *ix, int *m1)
{
    /* System generated locals */
    int i__1;

    /* Local variables */
    static int n;

    /* Parameter adjustments */
    --ix;

    /* Function Body */
    i__1 = *m1;
    for (n = 1; n <= i__1; ++n) {
	ix[n] = 0;
    }
    return 0;
} /* zerv_ */

/* Subroutine */ int zervr_(double *rx, int *m1)
{
    /* System generated locals */
    int i__1;

    /* Local variables */
    static int n;

    /* Parameter adjustments */
    --rx;

    /* Function Body */
    i__1 = *m1;
    for (n = 1; n <= i__1; ++n) {
	rx[n] = 0.;
    }
    return 0;
} /* zervr_ */

/* Subroutine */ int zerm_(int *mx, int *m1, int *m2)
{
    /* System generated locals */
    int mx_dim1, mx_offset, i__1, i__2;

    /* Local variables */
    static int i__, j;

    /* Parameter adjustments */
    mx_dim1 = *m1;
    mx_offset = 1 + mx_dim1;
    mx -= mx_offset;

    /* Function Body */
    i__1 = *m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *m2;
	for (j = 1; j <= i__2; ++j) {
	    mx[i__ + j * mx_dim1] = 0;
	}
    }
    return 0;
} /* zerm_ */

/* Subroutine */ int zermr_(double *rx, int *m1, int *m2)
{
    /* System generated locals */
    int rx_dim1, rx_offset, i__1, i__2;

    /* Local variables */
    static int i__, j;

    /* Parameter adjustments */
    rx_dim1 = *m1;
    rx_offset = 1 + rx_dim1;
    rx -= rx_offset;

    /* Function Body */
    i__1 = *m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *m2;
	for (j = 1; j <= i__2; ++j) {
	    rx[i__ + j * rx_dim1] = 0.;
	}
    }
    return 0;
} /* zermr_ */

/* Subroutine */ int zermd_(double *rx, int *m1, int *m2)
{
    /* System generated locals */
    int rx_dim1, rx_offset, i__1, i__2;

    /* Local variables */
    static int i__, j;

    /* Parameter adjustments */
    rx_dim1 = *m1;
    rx_offset = 1 + rx_dim1;
    rx -= rx_offset;

    /* Function Body */
    i__1 = *m1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *m2;
	for (j = 1; j <= i__2; ++j) {
	    rx[i__ + j * rx_dim1] = 0.;
	}
    }
    return 0;
} /* zermd_ */

