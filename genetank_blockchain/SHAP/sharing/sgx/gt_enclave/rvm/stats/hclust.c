/* hclust.f -- translated by f2c (version 20160102).
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
#include "misc.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++C */
/*                                                            C */
/*  HIERARCHICAL CLUSTERING using (user-specified) criterion. C */
/*                                                            C */
/*  Parameters:                                               C */
/*                                                            C */
/*  N                 the number of points being clustered    C */
/*  DISS(LEN)         dissimilarities in lower half diagonal  C */
/*                    storage; LEN = N.N-1/2,                 C */
/*  IOPT              clustering criterion to be used,        C */
/*  IA, IB, CRIT      history of agglomerations; dimensions   C */
/*                    N, first N-1 locations only used,       C */
/*  MEMBR, NN, DISNN  vectors of length N, used to store      C */
/*                    cluster cardinalities, current nearest  C */
/*                    neighbour, and the dissimilarity assoc. C */
/*                    with the latter.                        C */
/*                    MEMBR must be initialized by R to the   C */
/*                    default of  rep(1, N)                   C */
/*  FLAG              boolean indicator of agglomerable obj./ C */
/*                    clusters.                               C */
/*                                                            C */
/*  F. Murtagh, ESA/ESO/STECF, Garching, February 1986.       C */
/*  Modifications for R: Ross Ihaka, Dec 1996                 C */
/*                       Fritz Leisch, Jun 2000               C */
/*  all vars declared:   Martin Maechler, Apr 2001            C */
/*                                                            C */
/* - R Bug PR#4195 fixed "along" qclust.c, given in the report C */
/* - Testing: --> "hclust" in ../../../../tests/reg-tests-1b.R C */
/*  "ward.D2" (iOpt = 8): Martin Maechler, Mar 2014           C */
/* ------------------------------------------------------------C */
/* Subroutine */ int hclust_(integer *n, integer *len, integer *iopt, integer 
	*ia, integer *ib, doublereal *crit, doublereal *membr, integer *nn, 
	doublereal *disnn, integer *flag__, doublereal *diss)
{
    /* Initialized data */

    static doublereal inf = 1e300;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    //double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, i2, j2;
    static doublereal d12;
    static integer jj, im, jm, ind, ncl, ind1, ind2;
    static doublereal dmin__;
    static logical isward;
    extern integer ioffst_(integer *, integer *, integer *);

/* Args */
/* Var */
/* External function */

/*     was 1D+20 */
    /* Parameter adjustments */
    --flag__;
    --disnn;
    --nn;
    --membr;
    --crit;
    --ib;
    --ia;
    --diss;

    /* Function Body */

/*     unnecessary initialization of im jj jm to keep g77 -Wall happy */

    im = 0;
    jj = 0;
    jm = 0;

/*  Initializations */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*        We do not initialize MEMBR in order to be able to restart the */
/*        algorithm from a cut. */
/*        MEMBR(I)=1. */
	flag__[i__] = TRUE_;
    }
    ncl = *n;
    if (*iopt == 8) {
/* Ward "D2" ---> using *squared* distances */
	i__1 = *len;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    diss[i__] *= diss[i__];
	}
    }

/*  Carry out an agglomeration - first create list of NNs */
/*  Note NN and DISNN are the nearest neighbour and its distance */
/*  TO THE RIGHT of I. */

    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dmin__ = inf;
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    ind = ioffst_(n, &i__, &j);
	    if (dmin__ > diss[ind]) {
		dmin__ = diss[ind];
		jm = j;
	    }
	}
	nn[i__] = jm;
	disnn[i__] = dmin__;
    }
/* -- Repeat ------------------------------------------------------- */
L400:
/*     Next, determine least diss. using list of NNs */
    dmin__ = inf;
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (flag__[i__] && disnn[i__] < dmin__) {
	    dmin__ = disnn[i__];
	    im = i__;
	    jm = nn[i__];
	}
    }
    --ncl;

/*  This allows an agglomeration to be carried out. */

    i2 = min(im,jm);
    j2 = max(im,jm);
    ia[*n - ncl] = i2;
    ib[*n - ncl] = j2;
/*     WARD'S "D1", or "D2" MINIMUM VARIANCE METHOD -- iOpt = 1 or 8. */
    isward = *iopt == 1 || *iopt == 8;
    if (*iopt == 8) {
	dmin__ = sqrt(dmin__);
    }
    crit[*n - ncl] = dmin__;
    flag__[j2] = FALSE_;

/*  Update dissimilarities from new cluster. */

    dmin__ = inf;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (flag__[k] && k != i2) {
	    if (i2 < k) {
		ind1 = ioffst_(n, &i2, &k);
	    } else {
		ind1 = ioffst_(n, &k, &i2);
	    }
	    if (j2 < k) {
		ind2 = ioffst_(n, &j2, &k);
	    } else {
		ind2 = ioffst_(n, &k, &j2);
	    }
	    d12 = diss[ioffst_(n, &i2, &j2)];

/*     WARD'S "D1", or "D2" MINIMUM VARIANCE METHOD - IOPT=8. */
	    if (isward) {
		diss[ind1] = (membr[i2] + membr[k]) * diss[ind1] + (membr[j2] 
			+ membr[k]) * diss[ind2] - membr[k] * d12;
		diss[ind1] /= membr[i2] + membr[j2] + membr[k];

/*     SINGLE LINK METHOD - IOPT=2. */
	    } else if (*iopt == 2) {
/* Computing MIN */
		d__1 = diss[ind1], d__2 = diss[ind2];
		diss[ind1] = min(d__1,d__2);

/*     COMPLETE LINK METHOD - IOPT=3. */
	    } else if (*iopt == 3) {
/* Computing MAX */
		d__1 = diss[ind1], d__2 = diss[ind2];
		diss[ind1] = max(d__1,d__2);

/*     AVERAGE LINK (OR GROUP AVERAGE) METHOD - IOPT=4. */
	    } else if (*iopt == 4) {
		diss[ind1] = (membr[i2] * diss[ind1] + membr[j2] * diss[ind2])
			 / (membr[i2] + membr[j2]);

/*     MCQUITTY'S METHOD - IOPT=5. */
	    } else if (*iopt == 5) {
		diss[ind1] = (diss[ind1] + diss[ind2]) / 2;

/*     MEDIAN (GOWER'S) METHOD aka "Weighted Centroid" - IOPT=6. */
	    } else if (*iopt == 6) {
		diss[ind1] = (diss[ind1] + diss[ind2] - d12 / 2) / 2;

/*     Unweighted CENTROID METHOD - IOPT=7. */
	    } else if (*iopt == 7) {
		diss[ind1] = (membr[i2] * diss[ind1] + membr[j2] * diss[ind2] 
			- membr[i2] * membr[j2] * d12 / (membr[i2] + membr[j2]
			)) / (membr[i2] + membr[j2]);
	    }

	    if (i2 < k) {
		if (diss[ind1] < dmin__) {
		    dmin__ = diss[ind1];
		    jj = k;
		}
	    } else {
/* 	     FIX: the rest of the else clause is a fix by JB to ensure */
/*            correct nearest neighbours are found when a non-monotone */
/*            clustering method (e.g. the centroid methods) are used */
/* i2 > k */
		if (diss[ind1] < disnn[k]) {
/* find nearest neighbou */
		    disnn[k] = diss[ind1];
		    nn[k] = i2;
		}
	    }
	}
    }
    membr[i2] += membr[j2];
    disnn[i2] = dmin__;
    nn[i2] = jj;

/*  Update list of NNs insofar as this is required. */

    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (flag__[i__] && (nn[i__] == i2 || nn[i__] == j2)) {
/*     (Redetermine NN of I:) */
	    dmin__ = inf;
	    i__2 = *n;
	    for (j = i__ + 1; j <= i__2; ++j) {
		if (flag__[j]) {
		    ind = ioffst_(n, &i__, &j);
		    if (diss[ind] < dmin__) {
			dmin__ = diss[ind];
			jj = j;
		    }
		}
	    }
	    nn[i__] = jj;
	    disnn[i__] = dmin__;
	}
    }

/*  Repeat previous steps until N-1 agglomerations carried out. */

    if (ncl > 1) {
	goto L400;
    }


    return 0;
} /* hclust_ */

/*     of HCLUST() */


integer ioffst_(integer *n, integer *i__, integer *j)
{
    /* System generated locals */
    integer ret_val;

/*  Map row I and column J of upper half diagonal symmetric matrix */
/*  onto vector. */
    ret_val = *j + (*i__ - 1) * *n - *i__ * (*i__ + 1) / 2;
    return ret_val;
} /* ioffst_ */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++C */
/*                                                               C */
/*  Given a HIERARCHIC CLUSTERING, described as a sequence of    C */
/*  agglomerations, prepare the seq. of aggloms. and "horiz."    C */
/*  order of objects for plotting the dendrogram using S routine C */
/*  'plclust'.                                                   C */
/*                                                               C */
/*  Parameters:                                                  C */
/*                                                               C */
/*  IA, IB:       vectors of dimension N defining the agglomer-  C */
/*                 ations.                                       C */
/*  IIA, IIB:     used to store IA and IB values differently     C */
/*                (in form needed for S command 'plclust'        C */
/*  IORDER:       "horiz." order of objects for dendrogram       C */
/*                                                               C */
/*  F. Murtagh, ESA/ESO/STECF, Garching, June 1991               C */
/*                                                               C */
/*  HISTORY                                                      C */
/*                                                               C */
/*  Adapted from routine HCASS, which additionally determines    C */
/*   cluster assignments at all levels, at extra comput. expense C */
/*                                                               C */
/* ---------------------------------------------------------------C */
/* Subroutine */ int hcass2_(integer *n, integer *ia, integer *ib, integer *
	iorder, integer *iia, integer *iib)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, k1, k2, loc;

/* Args */
/* Var */

/*     Following bit is to get seq. of merges into format acceptable to plclust */
/*     I coded clusters as lowest seq. no. of constituents; S's 'hclust' codes */
/*     singletons as -ve numbers, and non-singletons with their seq. nos. */

    /* Parameter adjustments */
    --iib;
    --iia;
    --iorder;
    --ib;
    --ia;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iia[i__] = ia[i__];
	iib[i__] = ib[i__];
    }
    i__1 = *n - 2;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*        In the following, smallest (+ve or -ve) seq. no. wanted */
/* Computing MIN */
	i__2 = ia[i__], i__3 = ib[i__];
	k = min(i__2,i__3);
	i__2 = *n - 1;
	for (j = i__ + 1; j <= i__2; ++j) {
	    if (ia[j] == k) {
		iia[j] = -i__;
	    }
	    if (ib[j] == k) {
		iib[j] = -i__;
	    }
	}
    }
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iia[i__] = -iia[i__];
	iib[i__] = -iib[i__];
    }
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (iia[i__] > 0 && iib[i__] < 0) {
	    k = iia[i__];
	    iia[i__] = iib[i__];
	    iib[i__] = k;
	}
	if (iia[i__] > 0 && iib[i__] > 0) {
/* Computing MIN */
	    i__2 = iia[i__], i__3 = iib[i__];
	    k1 = min(i__2,i__3);
/* Computing MAX */
	    i__2 = iia[i__], i__3 = iib[i__];
	    k2 = max(i__2,i__3);
	    iia[i__] = k1;
	    iib[i__] = k2;
	}
    }


/*     NEW PART FOR 'ORDER' */

    iorder[1] = iia[*n - 1];
    iorder[2] = iib[*n - 1];
    loc = 2;
    for (i__ = *n - 2; i__ >= 1; --i__) {
	i__1 = loc;
	for (j = 1; j <= i__1; ++j) {
	    if (iorder[j] == i__) {
/*      REPLACE IORDER(J) WITH IIA(I) AND IIB(I) */
		iorder[j] = iia[i__];
		if (j == loc) {
		    ++loc;
		    iorder[loc] = iib[i__];
		} else {
		    ++loc;
		    i__2 = j + 2;
		    for (k = loc; k >= i__2; --k) {
			iorder[k] = iorder[k - 1];
		    }
		    iorder[j + 1] = iib[i__];
		}
		goto L171;
	    }
	}
/*     SHOULD NEVER REACH HERE */
L171:
	;
    }


    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iorder[i__] = -iorder[i__];
    }


    return 0;
} /* hcass2_ */

