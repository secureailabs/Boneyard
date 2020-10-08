/* portsrc.f -- translated by f2c (version 20160102).
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

/* Table of constant values */

static integer c__1 = 1;
static doublereal c_b14 = 0.;
static integer c__0 = 0;
static logical c_true = TRUE_;
static logical c_false = FALSE_;
static integer c__2 = 2;
static integer c_n1 = -1;
static doublereal c_b85 = 1.;
static doublereal c_b90 = -1.;
static integer c__3 = 3;
static integer c__6 = 6;
static integer c__5 = 5;
static integer c__4 = 4;
static doublereal c_b325 = .33333333333333331;
static integer c__9 = 9;
static doublereal c_b913 = -.66666666666666663;
static doublereal c_b915 = .66666666666666663;

/* Subroutine */ int drn2g_(doublereal *d__, doublereal *dr, integer *iv, 
	integer *liv, integer *lv, integer *n, integer *nd, integer *n1, 
	integer *n2, integer *p, doublereal *r__, doublereal *rd, doublereal *
	v, doublereal *x)
{
    /* System generated locals */
    integer dr_dim1, dr_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer i__, k, l;
    static doublereal t;
    static integer g1, y1, gi, lh, nn, yi, iv1, qtr1, rmat1, jtol1;
    extern /* Subroutine */ int dq7rad_(integer *, integer *, integer *, 
	    doublereal *, logical *, doublereal *, doublereal *, doublereal *)
	    , dn2lrd_(doublereal *, integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, doublereal 
	    *, doublereal *, doublereal *), dc7vfn_(integer *, doublereal *, 
	    integer *, integer *, integer *, integer *, integer *, doublereal 
	    *), dd7upd_(doublereal *, doublereal *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    doublereal *), dq7apl_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, integer *), dg7lit_(doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, integer 
	    *, doublereal *, doublereal *, doublereal *), dn2cvp_(integer *, 
	    integer *, integer *, integer *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dl7vml_(integer *, doublereal *, doublereal *,
	     doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dv7cpy_(integer *, doublereal *, doublereal *)
	    ;
    static integer ivmode;
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);


/* *** REVISED ITERATION DRIVER FOR NL2SOL (VERSION 2.3) *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* D........ SCALE VECTOR. */
/* DR....... DERIVATIVES OF R AT X. */
/* IV....... INTEGER VALUES ARRAY. */
/* LIV...... LENGTH OF IV... LIV MUST BE AT LEAST P + 82. */
/* LV....... LENGTH OF V...  LV  MUST BE AT LEAST 105 + P*(2*P+16). */
/* N........ TOTAL NUMBER OF RESIDUALS. */
/* ND....... MAX. NO. OF RESIDUALS PASSED ON ONE CALL. */
/* N1....... LOWEST  ROW INDEX FOR RESIDUALS SUPPLIED THIS TIME. */
/* N2....... HIGHEST ROW INDEX FOR RESIDUALS SUPPLIED THIS TIME. */
/* P........ NUMBER OF PARAMETERS (COMPONENTS OF X) BEING ESTIMATED. */
/* R........ RESIDUALS. */
/* RD....... RD(I) = SQRT(G(I)**T * H(I)**-1 * G(I)) ON OUTPUT WHEN */
/*        IV(RDREQ) IS NONZERO.   DRN2G SETS IV(REGD) = 1 IF RD */
/*        IS SUCCESSFULLY COMPUTED, TO 0 IF NO ATTEMPT WAS MADE */
/*        TO COMPUTE IT, AND TO -1 IF H (THE FINITE-DIFFERENCE HESSIAN) */
/*        WAS INDEFINITE.  IF ND .GE. N, THEN RD IS ALSO USED AS */
/*        TEMPORARY STORAGE. */
/* V........ FLOATING-POINT VALUES ARRAY. */
/* X........ PARAMETER VECTOR BEING ESTIMATED (INPUT = INITIAL GUESS, */
/*             OUTPUT = BEST VALUE FOUND). */

/*  ***  DISCUSSION  *** */

/*  NOTE... NL2SOL AND NL2ITR (MENTIONED BELOW) ARE DESCRIBED IN */
/*  ACM TRANS. MATH. SOFTWARE, VOL. 7, PP. 369-383 (AN ADAPTIVE */
/*  NONLINEAR LEAST-SQUARES ALGORITHM, BY J.E. DENNIS, D.M. GAY, */
/*  AND R.E. WELSCH). */

/*     THIS ROUTINE CARRIES OUT ITERATIONS FOR SOLVING NONLINEAR */
/*  LEAST SQUARES PROBLEMS.  WHEN ND = N, IT IS SIMILAR TO NL2ITR */
/*  (WITH J = DR), EXCEPT THAT R(X) AND DR(X) NEED NOT BE INITIALIZED */
/*  WHEN  DRN2G IS CALLED WITH IV(1) = 0 OR 12.   DRN2G ALSO ALLOWS */
/*  R AND DR TO BE SUPPLIED ROW-WISE -- JUST SET ND = 1 AND CALL */
/*   DRN2G ONCE FOR EACH ROW WHEN PROVIDING RESIDUALS AND JACOBIANS. */
/*     ANOTHER NEW FEATURE IS THAT CALLING  DRN2G WITH IV(1) = 13 */
/*  CAUSES STORAGE ALLOCATION ONLY TO BE PERFORMED -- ON RETURN, SUCH */
/*  COMPONENTS AS IV(G) (THE FIRST SUBSCRIPT IN G OF THE GRADIENT) */
/*  AND IV(S) (THE FIRST SUBSCRIPT IN V OF THE S LOWER TRIANGLE OF */
/*  THE S MATRIX) WILL HAVE BEEN SET (UNLESS LIV OR LV IS TOO SMALL), */
/*  AND IV(1) WILL HAVE BEEN SET TO 14. CALLING  DRN2G WITH IV(1) = 14 */
/*  CAUSES EXECUTION OF THE ALGORITHM TO BEGIN UNDER THE ASSUMPTION */
/*  THAT STORAGE HAS BEEN ALLOCATED. */

/* ***  SUPPLYING R AND DR  *** */

/*      DRN2G USES IV AND V IN THE SAME WAY AS NL2SOL, WITH A SMALL */
/*  NUMBER OF OBVIOUS CHANGES.  ONE DIFFERENCE BETWEEN  DRN2G AND */
/*  NL2ITR IS THAT INITIAL FUNCTION AND GRADIENT INFORMATION NEED NOT */
/*  BE SUPPLIED IN THE VERY FIRST CALL ON  DRN2G, THE ONE WITH */
/*  IV(1) = 0 OR 12.  ANOTHER DIFFERENCE IS THAT  DRN2G RETURNS WITH */
/*  IV(1) = -2 WHEN IT WANTS ANOTHER LOOK AT THE OLD JACOBIAN MATRIX */
/*  AND THE CURRENT RESIDUAL -- THE ONE CORRESPONDING TO X AND */
/*  IV(NFGCAL).  IT THEN RETURNS WITH IV(1) = -3 WHEN IT WANTS TO SEE */
/*  BOTH THE NEW RESIDUAL AND THE NEW JACOBIAN MATRIX AT ONCE.  NOTE */
/*  THAT IV(NFGCAL) = IV(7) CONTAINS THE VALUE THAT IV(NFCALL) = IV(6) */
/*  HAD WHEN THE CURRENT RESIDUAL WAS EVALUATED.  ALSO NOTE THAT THE */
/*  VALUE OF X CORRESPONDING TO THE OLD JACOBIAN MATRIX IS STORED IN */
/*  V, STARTING AT V(IV(X0)) = V(IV(43)). */
/*     ANOTHER NEW RETURN...  DRN2G IV(1) = -1 WHEN IT WANTS BOTH THE */
/*  RESIDUAL AND THE JACOBIAN TO BE EVALUATED AT X. */
/*     A NEW RESIDUAL VECTOR MUST BE SUPPLIED WHEN  DRN2G RETURNS WITH */
/*  IV(1) = 1 OR -1.  THIS TAKES THE FORM OF VALUES OF R(I,X) PASSED */
/*  IN R(I-N1+1), I = N1(1)N2.  YOU MAY PASS ALL THESE VALUES AT ONCE */
/*  (I.E., N1 = 1 AND N2 = N) OR IN PIECES BY MAKING SEVERAL CALLS ON */
/*   DRN2G.  EACH TIME  DRN2G RETURNS WITH IV(1) = 1, N1 WILL HAVE */
/*  BEEN SET TO THE INDEX OF THE NEXT RESIDUAL THAT  DRN2G EXPECTS TO */
/*  SEE, AND N2 WILL BE SET TO THE INDEX OF THE HIGHEST RESIDUAL THAT */
/*  COULD BE GIVEN ON THE NEXT CALL, I.E., N2 = N1 + ND - 1.  (THUS */
/*  WHEN  DRN2G FIRST RETURNS WITH IV(1) = 1 FOR A NEW X, IT WILL */
/*  HAVE SET N1 TO 1 AND N2 TO MIN(ND,N).)  THE CALLER MAY PROVIDE */
/*  FEWER THAN N2-N1+1 RESIDUALS ON THE NEXT CALL BY SETTING N2 TO */
/*  A SMALLER VALUE.   DRN2G ASSUMES IT HAS SEEN ALL THE RESIDUALS */
/*  FOR THE CURRENT X WHEN IT IS CALLED WITH N2 .GE. N. */
/*    EXAMPLE... SUPPOSE N = 80 AND THAT R IS TO BE PASSED IN 8 */
/*  BLOCKS OF SIZE 10.  THE FOLLOWING CODE WOULD DO THE JOB. */

/*      N = 80 */
/*      ND = 10 */
/*      ... */
/*      DO 10 K = 1, 8 */
/*           ***  COMPUTE R(I,X) FOR I = 10*K-9 TO 10*K  *** */
/*           ***  AND STORE THEM IN R(1),...,R(10)  *** */
/*           CALL  DRN2G(..., R, ...) */
/*   10      CONTINUE */

/*     THE SITUATION IS SIMILAR WHEN GRADIENT INFORMATION IS */
/*  REQUIRED, I.E., WHEN  DRN2G RETURNS WITH IV(1) = 2, -1, OR -2. */
/*  NOTE THAT  DRN2G OVERWRITES R, BUT THAT IN THE SPECIAL CASE OF */
/*  N1 = 1 AND N2 = N ON PREVIOUS CALLS,  DRN2G NEVER RETURNS WITH */
/*  IV(1) = -2.  IT SHOULD BE CLEAR THAT THE PARTIAL DERIVATIVE OF */
/*  R(I,X) WITH RESPECT TO X(L) IS TO BE STORED IN DR(I-N1+1,L), */
/*  L = 1(1)P, I = N1(1)N2.  IT IS ESSENTIAL THAT R(I) AND DR(I,L) */
/*  ALL CORRESPOND TO THE SAME RESIDUALS WHEN IV(1) = -1 OR -2. */

/*  ***  COVARIANCE MATRIX  *** */

/*     IV(RDREQ) = IV(57) TELLS WHETHER TO COMPUTE A COVARIANCE */
/*  MATRIX AND/OR REGRESSION DIAGNOSTICS... 0 MEANS NEITHER, */
/*  1 MEANS COVARIANCE MATRIX ONLY, 2 MEANS REG. DIAGNOSTICS ONLY, */
/*  3 MEANS BOTH.  AS WITH NL2SOL, IV(COVREQ) = IV(15) TELLS WHAT */
/*  HESSIAN APPROXIMATION TO USE IN THIS COMPUTING. */

/*  ***  REGRESSION DIAGNOSTICS  *** */

/*     SEE THE COMMENTS IN SUBROUTINE   DN2G. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */

/* +++++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++ */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DC7VFN... FINISHES COVARIANCE COMPUTATION. */
/* DIVSET.... PROVIDES DEFAULT IV AND V INPUT COMPONENTS. */
/* DD7TPR... COMPUTES INNER PRODUCT OF TWO VECTORS. */
/* DD7UPD...  UPDATES SCALE VECTOR D. */
/* DG7LIT.... PERFORMS BASIC MINIMIZATION ALGORITHM. */
/* DITSUM.... PRINTS ITERATION SUMMARY, INFO ABOUT INITIAL AND FINAL X. */
/* DL7VML.... COMPUTES L * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DN2CVP... PRINTS COVARIANCE MATRIX. */
/* DN2LRD... COMPUTES REGRESSION DIAGNOSTICS. */
/* DQ7APL... APPLIES QR TRANSFORMATIONS STORED BY DQ7RAD. */
/* DQ7RAD.... ADDS A NEW BLOCK OF ROWS TO QR DECOMPOSITION. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */

/*  ***  LOCAL VARIABLES  *** */



/*  ***  SUBSCRIPTS FOR IV AND V  *** */



/*  ***  IV SUBSCRIPT VALUES  *** */


/*  ***  V SUBSCRIPT VALUES  *** */


/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --rd;
    --r__;
    --x;
    dr_dim1 = *nd;
    dr_offset = 1 + dr_dim1;
    dr -= dr_offset;
    --d__;

    /* Function Body */
    lh = *p * (*p + 1) / 2;
    if (iv[1] == 0) {
	divset_(&c__1, &iv[1], liv, lv, &v[1]);
    }
    iv1 = iv[1];
    if (iv1 > 2) {
	goto L10;
    }
    nn = *n2 - *n1 + 1;
    iv[9] = 0;
    i__ = iv1 + 4;
    if (iv[2] == 0) {
	switch (i__) {
	    case 1:  goto L150;
	    case 2:  goto L130;
	    case 3:  goto L150;
	    case 4:  goto L120;
	    case 5:  goto L120;
	    case 6:  goto L150;
	}
    }
    if (i__ != 5) {
	iv[1] = 2;
    }
    goto L40;

/*  ***  FRESH START OR RESTART -- CHECK INPUT INTEGERS  *** */

L10:
    if (*nd <= 0) {
	goto L210;
    }
    if (*p <= 0) {
	goto L210;
    }
    if (*n <= 0) {
	goto L210;
    }
    if (iv1 == 14) {
	goto L30;
    }
    if (iv1 > 16) {
	goto L300;
    }
    if (iv1 < 12) {
	goto L40;
    }
    if (iv1 == 12) {
	iv[1] = 13;
    }
    if (iv[1] != 13) {
	goto L20;
    }
    iv[3] += *p;
    iv[4] += *p * (*p + 13) / 2;
L20:
    dg7lit_(&d__[1], &x[1], &iv[1], liv, lv, p, p, &v[1], &x[1], &x[1]);
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[76] = iv[46];
    iv[46] = iv[76] + *p;
    iv[48] = iv[47];
    iv[28] = iv[48] + *p;
    iv[66] = iv[28] + *p;
    iv[78] = iv[66] + *p;
    iv[77] = iv[78] + lh;
    iv[59] = iv[77] + *p;
    iv[47] = iv[59] + (*p << 1);
    if (iv1 == 13) {
	goto L999;
    }

L30:
    jtol1 = iv[59];
    if (v[38] >= 0.) {
	dv7scp_(p, &d__[1], &v[38]);
    }
    if (v[39] > 0.) {
	dv7scp_(p, &v[jtol1], &v[39]);
    }
    i__ = jtol1 + *p;
    if (v[40] > 0.) {
	dv7scp_(p, &v[i__], &v[40]);
    }
    iv[68] = 0;
    iv[69] = 0;
    if (*nd >= *n) {
	goto L40;
    }

/*  ***  SPECIAL CASE HANDLING OF FIRST FUNCTION AND GRADIENT EVALUATION */
/*  ***  -- ASK FOR BOTH RESIDUAL AND JACOBIAN AT ONCE */

    g1 = iv[28];
    y1 = iv[48];
    dg7lit_(&d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[y1]);
    if (iv[1] != 1) {
	goto L220;
    }
    v[10] = 0.;
    dv7scp_(p, &v[g1], &c_b14);
    iv[1] = -1;
    qtr1 = iv[77];
    dv7scp_(p, &v[qtr1], &c_b14);
    iv[67] = 0;
    rmat1 = iv[78];
    goto L100;

L40:
    g1 = iv[28];
    y1 = iv[48];
    dg7lit_(&d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[y1]);
    if (iv[1] == 2) {
	goto L60;
    }
    if (iv[1] > 2) {
	goto L220;
    }

    v[10] = 0.;
    if (iv[69] == 0) {
	goto L260;
    }
    if (iv[9] != 2) {
	goto L260;
    }
    iv[68] = iv[69];
    dv7cpy_(n, &rd[1], &r__[1]);
    iv[67] = 0;
    goto L260;

L60:
    dv7scp_(p, &v[g1], &c_b14);
    if (iv[35] > 0) {
	goto L230;
    }
    rmat1 = iv[78];
    qtr1 = iv[77];
    dv7scp_(p, &v[qtr1], &c_b14);
    iv[67] = 0;
    if (*nd < *n) {
	goto L90;
    }
    if (*n1 != 1) {
	goto L90;
    }
    if (iv[35] < 0) {
	goto L100;
    }
    if (iv[69] == iv[7]) {
	goto L70;
    }
    if (iv[68] != iv[7]) {
	goto L90;
    }
    dv7cpy_(n, &r__[1], &rd[1]);
    goto L80;
L70:
    dv7cpy_(n, &rd[1], &r__[1]);
L80:
    dq7apl_(nd, n, p, &dr[dr_offset], &rd[1], &c__0);
    dl7vml_(p, &v[y1], &v[rmat1], &rd[1]);
    goto L110;

L90:
    iv[1] = -2;
    if (iv[35] < 0) {
	iv[1] = -1;
    }
L100:
    dv7scp_(p, &v[y1], &c_b14);
L110:
    dv7scp_(&lh, &v[rmat1], &c_b14);
    goto L260;

/*  ***  COMPUTE F(X)  *** */

L120:
    t = dv2nrm_(&nn, &r__[1]);
    if (t > v[46]) {
	goto L200;
    }
/* Computing 2nd power */
    d__1 = t;
    v[10] += d__1 * d__1 * .5;
    if (*n2 < *n) {
	goto L270;
    }
    if (*n1 == 1) {
	iv[69] = iv[6];
    }
    goto L40;

/*  ***  COMPUTE Y  *** */

L130:
    y1 = iv[48];
    yi = y1;
    i__1 = *p;
    for (l = 1; l <= i__1; ++l) {
	v[yi] += dd7tpr_(&nn, &dr[l * dr_dim1 + 1], &r__[1]);
	++yi;
/* L140: */
    }
    if (*n2 < *n) {
	goto L270;
    }
    iv[1] = 2;
    if (*n1 > 1) {
	iv[1] = -3;
    }
    goto L260;

/*  ***  COMPUTE GRADIENT INFORMATION  *** */

L150:
    if (iv[35] > *p) {
	goto L240;
    }
    g1 = iv[28];
    ivmode = iv[35];
    if (ivmode < 0) {
	goto L170;
    }
    if (ivmode == 0) {
	goto L180;
    }
    iv[1] = 2;

/*  ***  COMPUTE GRADIENT ONLY (FOR USE IN COVARIANCE COMPUTATION)  *** */

    gi = g1;
    i__1 = *p;
    for (l = 1; l <= i__1; ++l) {
	v[gi] += dd7tpr_(&nn, &r__[1], &dr[l * dr_dim1 + 1]);
	++gi;
/* L160: */
    }
    goto L190;

/*  *** COMPUTE INITIAL FUNCTION VALUE WHEN ND .LT. N *** */

L170:
    if (*n <= *nd) {
	goto L180;
    }
    t = dv2nrm_(&nn, &r__[1]);
    if (t > v[46]) {
	goto L200;
    }
/* Computing 2nd power */
    d__1 = t;
    v[10] += d__1 * d__1 * .5;

/*  ***  UPDATE D IF DESIRED  *** */

L180:
    if (iv[16] > 0) {
	dd7upd_(&d__[1], &dr[dr_offset], &iv[1], liv, lv, n, nd, &nn, n2, p, &
		v[1]);
    }

/*  ***  COMPUTE RMAT AND QTR  *** */

    qtr1 = iv[77];
    rmat1 = iv[78];
    dq7rad_(&nn, nd, p, &v[qtr1], &c_true, &v[rmat1], &dr[dr_offset], &r__[1])
	    ;
    iv[69] = 0;

L190:
    if (*n2 < *n) {
	goto L270;
    }
    if (ivmode > 0) {
	goto L40;
    }
    iv[81] = iv[7];

/*  ***  COMPUTE G FROM RMAT AND QTR  *** */

    dl7vml_(p, &v[g1], &v[rmat1], &v[qtr1]);
    iv[1] = 2;
    if (ivmode == 0) {
	goto L40;
    }
    if (*n <= *nd) {
	goto L40;
    }

/*  ***  FINISH SPECIAL CASE HANDLING OF FIRST FUNCTION AND GRADIENT */

    y1 = iv[48];
    iv[1] = 1;
    dg7lit_(&d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[y1]);
    if (iv[1] != 2) {
	goto L220;
    }
    goto L40;

/*  ***  MISC. DETAILS  *** */

/*     ***  X IS OUT OF RANGE (OVERSIZE STEP)  *** */

L200:
    iv[2] = 1;
    goto L40;

/*     ***  BAD N, ND, OR P  *** */

L210:
    iv[1] = 66;
    goto L300;

/*  ***  CONVERGENCE OBTAINED -- SEE WHETHER TO COMPUTE COVARIANCE  *** */

L220:
    if (iv[26] != 0) {
	goto L290;
    }
    if (iv[67] != 0) {
	goto L290;
    }

/*     ***  SEE IF CHOLESKY FACTOR OF HESSIAN IS AVAILABLE  *** */

    k = iv[74];
    if (k <= 0) {
	goto L280;
    }
    if (iv[57] <= 0) {
	goto L290;
    }

/*     ***  COMPUTE REGRESSION DIAGNOSTICS AND DEFAULT COVARIANCE IF */
/*          DESIRED  *** */

    i__ = 0;
    if (iv[57] % 4 >= 2) {
	i__ = 1;
    }
    if (iv[57] % 2 == 1 && abs(iv[15]) <= 1) {
	i__ += 2;
    }
    if (i__ == 0) {
	goto L250;
    }
    iv[35] = *p + i__;
    ++iv[30];
    ++iv[53];
    iv[55] = iv[1];
    if (i__ < 2) {
	goto L230;
    }
    l = abs(iv[56]);
    dv7scp_(&lh, &v[l], &c_b14);
L230:
    ++iv[52];
    ++iv[6];
    iv[7] = iv[6];
    iv[1] = -1;
    goto L260;

L240:
    l = iv[42];
    dn2lrd_(&dr[dr_offset], &iv[1], &v[l], &lh, liv, lv, nd, &nn, p, &r__[1], 
	    &rd[1], &v[1]);
    if (*n2 < *n) {
	goto L270;
    }
    if (*n1 > 1) {
	goto L250;
    }

/*     ***  ENSURE WE CAN RESTART -- AND MAKE RETURN STATE OF DR */
/*     ***  INDEPENDENT OF WHETHER REGRESSION DIAGNOSTICS ARE COMPUTED. */
/*     ***  USE STEP VECTOR (ALLOCATED BY DG7LIT) FOR SCRATCH. */

    rmat1 = iv[78];
    dv7scp_(&lh, &v[rmat1], &c_b14);
    dq7rad_(&nn, nd, p, &r__[1], &c_false, &v[rmat1], &dr[dr_offset], &r__[1])
	    ;
    iv[69] = 0;

/*  ***  FINISH COMPUTING COVARIANCE  *** */

L250:
    l = iv[42];
    dc7vfn_(&iv[1], &v[l], &lh, liv, lv, n, p, &v[1]);
    goto L290;

/*  ***  RETURN FOR MORE FUNCTION OR GRADIENT INFORMATION  *** */

L260:
    *n2 = 0;
L270:
    *n1 = *n2 + 1;
    *n2 += *nd;
    if (*n2 > *n) {
	*n2 = *n;
    }
    goto L999;

/*  ***  COME HERE FOR INDEFINITE FINITE-DIFFERENCE HESSIAN  *** */

L280:
    iv[26] = k;
    iv[67] = k;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L290:
    g1 = iv[28];
L300:
    ditsum_(&d__[1], &v[g1], &iv[1], liv, lv, p, &v[1], &x[1]);
    if (iv[1] <= 6 && iv[57] > 0) {
	dn2cvp_(&iv[1], liv, lv, p, &v[1]);
    }

L999:
    return 0;
/*  ***  LAST LINE OF  DRN2G FOLLOWS  *** */
} /* drn2g_ */

/* Subroutine */ int dl7sqr_(integer *n, doublereal *a, doublereal *l)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    static integer i0, j0, ii, ij, ik, jj, jk, ip1, np1;


/*  ***  COMPUTE  A = LOWER TRIANGLE OF  L*(L**T),  WITH BOTH */
/*  ***  L  AND  A  STORED COMPACTLY BY ROWS.  (BOTH MAY OCCUPY THE */
/*  ***  SAME STORAGE. */

/*  ***  PARAMETERS  *** */

/*     DIMENSION A(N*(N+1)/2), L(N*(N+1)/2) */

/*  ***  LOCAL VARIABLES  *** */


    /* Parameter adjustments */
    --l;
    --a;

    /* Function Body */
    np1 = *n + 1;
    i0 = *n * (*n + 1) / 2;
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = np1 - ii;
	ip1 = i__ + 1;
	i0 -= i__;
	j0 = i__ * (i__ + 1) / 2;
	i__2 = i__;
	for (jj = 1; jj <= i__2; ++jj) {
	    j = ip1 - jj;
	    j0 -= j;
	    t = 0.;
	    i__3 = j;
	    for (k = 1; k <= i__3; ++k) {
		ik = i0 + k;
		jk = j0 + k;
		t += l[ik] * l[jk];
/* L10: */
	    }
	    ij = i0 + j;
	    a[ij] = t;
/* L20: */
	}
/* L30: */
    }
    return 0;
} /* dl7sqr_ */

/* Subroutine */ int drmnhb_(doublereal *b, doublereal *d__, doublereal *fx, 
	doublereal *g, doublereal *h__, integer *iv, integer *lh, integer *
	liv, integer *lv, integer *n, doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal t;
    static integer w1;
    static doublereal gi;
    static integer x01, x11;
    static doublereal xi;
    static integer dg1, td1, tg1, ipi, ipn, nn1o2, temp0, temp1, ipiv2, step0,
	     step1, dummy;
    extern logical stopx_(integer *);
    extern /* Subroutine */ int dd7dup_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *), dg7qsb_(
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int ds7ipr_(integer *, integer *, doublereal *), 
	    dv7ipr_(integer *, integer *, doublereal *), ds7lvm_(integer *, 
	    doublereal *, doublereal *, doublereal *), dv2axy_(integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dv7cpy_(
	    integer *, doublereal *, doublereal *), dv7vmp_(integer *, 
	    doublereal *, doublereal *, doublereal *, integer *), i7pnvr_(
	    integer *, integer *, integer *), dparck_(integer *, doublereal *,
	     integer *, integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);
    static integer lstgst, rstrst;


/*  ***  CARRY OUT  DMNHB (SIMPLY BOUNDED MINIMIZATION) ITERATIONS, */
/*  ***  USING HESSIAN MATRIX PROVIDED BY THE CALLER. */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* D.... SCALE VECTOR. */
/* FX... FUNCTION VALUE. */
/* G.... GRADIENT VECTOR. */
/* H.... LOWER TRIANGLE OF THE HESSIAN, STORED ROWWISE. */
/* IV... INTEGER VALUE ARRAY. */
/* LH... LENGTH OF H = P*(P+1)/2. */
/* LIV.. LENGTH OF IV (AT LEAST 59 + 3*N). */
/* LV... LENGTH OF V (AT LEAST 78 + N*(N+27)/2). */
/* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... PARAMETER VECTOR. */

/*  ***  DISCUSSION  *** */

/*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO  DMNHB (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
/*     THE PART OF V THAT  DMNHB USES FOR STORING G AND H IS NOT NEEDED). */
/*     MOREOVER, COMPARED WITH  DMNHB, IV(1) MAY HAVE THE TWO ADDITIONAL */
/*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
/*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
/*     OUTPUT VALUE FROM  DMNHB, IS NOT REFERENCED BY DRMNHB OR THE */
/*     SUBROUTINES IT CALLS. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
/*             AT X, AND CALL DRMNHB AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
/*             COMPUTED (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN */
/*             BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER */
/*             SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE */
/*             DRMNHB TO IGNORE FX AND TRY A SMALLER STEP.  THE PARA- */
/*             METER NF THAT  DMNH PASSES TO CALCF (FOR POSSIBLE USE BY */
/*             CALCGH) IS A COPY OF IV(NFCALL) = IV(6). */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
/*             X, AND H TO THE LOWER TRIANGLE OF H(X), THE HESSIAN OF F */
/*             AT X, AND CALL DRMNHB AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS EXCEPT PERHAPS THE SCALE VECTOR D. */
/*                  THE PARAMETER NF THAT  DMNHB PASSES TO CALCG IS */
/*             IV(NFGCAL) = IV(7).  IF G(X) AND H(X) CANNOT BE EVALUATED, */
/*             THEN THE CALLER MAY SET IV(NFGCAL) TO 0, IN WHICH CASE */
/*             DRMNHB WILL RETURN WITH IV(1) = 65. */
/*                  NOTE -- DRMNHB OVERWRITES H WITH THE LOWER TRIANGLE */
/*             OF  DIAG(D)**-1 * H(X) * DIAG(D)**-1. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (WINTER, SPRING 1983). */

/*        (SEE  DMNG AND  DMNH FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  NO INTRINSIC FUNCTIONS  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DIVSET.... PROVIDES DEFAULT IV AND V INPUT VALUES. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DD7DUP.... UPDATES SCALE VECTOR D. */
/* DG7QSB... COMPUTES APPROXIMATE OPTIMAL BOUNDED STEP. */
/* I7PNVR... INVERTS PERMUTATION ARRAY. */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* DS7IPR... APPLIES PERMUTATION TO LOWER TRIANG. OF SYM. MATRIX. */
/* DS7LVM... MULTIPLIES SYMMETRIC MATRIX TIMES VECTOR, GIVEN THE LOWER */
/*             TRIANGLE OF THE MATRIX. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7IPR... APPLIES PERMUTATION TO VECTOR. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV7VMP... MULTIPLIES (OR DIVIDES) TWO VECTORS COMPONENTWISE. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */

/*  ***  (NOTE THAT NC AND N0 ARE STORED IN IV(G0) AND IV(STLSTG) RESP.) */


/*  ***  V SUBSCRIPT VALUES  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --h__;
    --iv;
    --v;
    --x;
    --g;
    --d__;
    b -= 3;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L50;
    }
    if (i__ == 2) {
	goto L60;
    }

/*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    if (iv[1] < 12) {
	goto L10;
    }
    if (iv[1] > 13) {
	goto L10;
    }
    iv[4] = iv[4] + *n * (*n + 27) / 2 + 7;
    iv[3] += *n * 3;
L10:
    dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    nn1o2 = *n * (*n + 1) / 2;
    if (*lh >= nn1o2) {
	switch (i__) {
	    case 1:  goto L250;
	    case 2:  goto L250;
	    case 3:  goto L250;
	    case 4:  goto L250;
	    case 5:  goto L250;
	    case 6:  goto L250;
	    case 7:  goto L190;
	    case 8:  goto L150;
	    case 9:  goto L190;
	    case 10:  goto L20;
	    case 11:  goto L20;
	    case 12:  goto L30;
	}
    }
    iv[1] = 81;
    goto L440;

/*  ***  STORAGE ALLOCATION  *** */

L20:
    iv[59] = iv[42] + nn1o2;
    iv[43] = iv[59] + (*n << 1);
    iv[40] = iv[43] + (*n << 1);
    iv[37] = iv[40] + *n * 3;
    iv[34] = iv[37] + (*n << 1);
    iv[47] = iv[34] + (*n << 2) + 7;
    iv[46] = iv[58] + *n * 3;
    if (iv[1] != 13) {
	goto L30;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L30:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[5] = 1;
    iv[11] = 1;
    iv[2] = 0;
    iv[55] = 0;
    iv[8] = 0;
    iv[48] = *n;
    v[9] = 0.;
    v[5] = 0.;
    if (v[38] >= 0.) {
	dv7scp_(n, &d__[1], &v[38]);
    }
    k = iv[59];
    if (v[39] > 0.) {
	dv7scp_(n, &v[k], &v[39]);
    }
    k += *n;
    if (v[40] > 0.) {
	dv7scp_(n, &v[k], &v[40]);
    }

/*  ***  CHECK CONSISTENCY OF B AND INITIALIZE IP ARRAY  *** */

    ipi = iv[58];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iv[ipi] = i__;
	++ipi;
	if (b[(i__ << 1) + 1] > b[(i__ << 1) + 2]) {
	    goto L420;
	}
/* L40: */
    }

/*  ***  GET INITIAL FUNCTION VALUE  *** */

    iv[1] = 1;
    goto L450;

L50:
    v[10] = *fx;
    if (iv[35] >= 0) {
	goto L250;
    }
    v[13] = *fx;
    iv[1] = 2;
    if (iv[2] == 0) {
	goto L999;
    }
    iv[1] = 63;
    goto L440;

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L60:
    if (iv[2] == 0) {
	goto L70;
    }
    iv[1] = 65;
    goto L440;

/*  ***  UPDATE THE SCALE VECTOR D  *** */

L70:
    dg1 = iv[37];
    if (iv[16] <= 0) {
	goto L90;
    }
    k = dg1;
    j = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j += i__;
	v[k] = h__[j];
	++k;
/* L80: */
    }
    dd7dup_(&d__[1], &v[dg1], &iv[1], liv, lv, n, &v[1]);

/*  ***  COMPUTE SCALED GRADIENT AND ITS NORM  *** */

L90:
    dg1 = iv[37];
    dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);

/*  ***  COMPUTE SCALED HESSIAN  *** */

    k = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = 1. / d__[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    h__[k] = t * h__[k] / d__[j];
	    ++k;
/* L100: */
	}
/* L110: */
    }

/*  ***  CHOOSE INITIAL PERMUTATION  *** */

    ipi = iv[58];
    ipn = ipi + *n;
    ipiv2 = ipn - 1;
/*     *** INVERT OLD PERMUTATION ARRAY *** */
    i7pnvr_(n, &iv[ipn], &iv[ipi]);
    k = iv[48];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (b[(i__ << 1) + 1] >= b[(i__ << 1) + 2]) {
	    goto L120;
	}
	xi = x[i__];
	gi = g[i__];
	if (xi <= b[(i__ << 1) + 1] && gi > 0.) {
	    goto L120;
	}
	if (xi >= b[(i__ << 1) + 2] && gi < 0.) {
	    goto L120;
	}
	iv[ipi] = i__;
	++ipi;
	j = ipiv2 + i__;
/*           *** DISALLOW CONVERGENCE IF X(I) HAS JUST BEEN FREED *** */
	if (iv[j] > k) {
	    iv[55] = 0;
	}
	goto L130;
L120:
	--ipn;
	iv[ipn] = i__;
L130:
	;
    }
    iv[48] = ipn - iv[58];

/*  ***  PERMUTE SCALED GRADIENT AND HESSIAN ACCORDINGLY  *** */

    ipi = iv[58];
    ds7ipr_(n, &iv[ipi], &h__[1]);
    dv7ipr_(n, &iv[ipi], &v[dg1]);
    v[1] = 0.;
    if (iv[48] > 0) {
	v[1] = dv2nrm_(&iv[48], &v[dg1]);
    }

    if (iv[55] != 0) {
	goto L430;
    }
    if (iv[35] == 0) {
	goto L380;
    }

/*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

    v[8] = v[35] / (v[21] + 1.);

    iv[35] = 0;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L140:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L150:
    k = iv[31];
    if (k < iv[18]) {
	goto L160;
    }
    iv[1] = 10;
    goto L440;

L160:
    iv[31] = k + 1;

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[33] = -1;

/*     ***  COPY X TO X0  *** */

    dv7cpy_(n, &v[x01], &x[1]);

/*  ***  UPDATE RADIUS  *** */

    if (k == 0) {
	goto L180;
    }
    step1 = iv[40];
    k = step1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = d__[i__] * v[k];
	++k;
/* L170: */
    }
    t = v[16] * dv2nrm_(n, &v[step1]);
    if (v[16] < 1. || t > v[8]) {
	v[8] = t;
    }

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L180:
    if (! stopx_(&dummy)) {
	goto L200;
    }
    iv[1] = 11;
    goto L210;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L190:
    if (v[10] >= v[13]) {
	goto L200;
    }
    v[16] = 1.;
    k = iv[31];
    goto L160;

L200:
    if (iv[6] < iv[17]) {
	goto L220;
    }
    iv[1] = 9;
L210:
    if (v[10] >= v[13]) {
	goto L440;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L370;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L220:
    step1 = iv[40];
    l = iv[42];
    w1 = iv[34];
    ipi = iv[58];
    ipn = ipi + *n;
    ipiv2 = ipn + *n;
    tg1 = iv[37];
    td1 = tg1 + *n;
    x01 = iv[43];
    x11 = x01 + *n;
    dg7qsb_(&b[3], &d__[1], &h__[1], &g[1], &iv[ipi], &iv[ipn], &iv[ipiv2], &
	    iv[33], &v[l], lv, n, &iv[41], &iv[48], &v[step1], &v[td1], &v[
	    tg1], &v[1], &v[w1], &v[x11], &v[x01]);
    if (iv[29] != 6) {
	goto L230;
    }
    if (iv[9] != 2) {
	goto L250;
    }
    rstrst = 2;
    goto L260;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L230:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L250;
    }
    if (iv[29] != 5) {
	goto L240;
    }
    if (v[16] <= 1.) {
	goto L240;
    }
    if (v[7] > v[11] * 1.2) {
	goto L240;
    }
    if (iv[9] != 2) {
	goto L250;
    }
    rstrst = 0;
    goto L260;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L240:
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L450;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L250:
    rstrst = 3;
L260:
    x01 = iv[43];
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = step1 + (*n << 1);
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L300;
	case 2:  goto L270;
	case 3:  goto L280;
	case 4:  goto L290;
    }
L270:
    dv7cpy_(n, &x[1], &v[x01]);
    goto L300;
L280:
    dv7cpy_(n, &v[lstgst], &x[1]);
    goto L300;
L290:
    dv7cpy_(n, &x[1], &v[lstgst]);
    dv2axy_(n, &v[step1], &c_b90, &v[x01], &x[1]);
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

L300:
    k = iv[29];
    switch (k) {
	case 1:  goto L310;
	case 2:  goto L340;
	case 3:  goto L340;
	case 4:  goto L340;
	case 5:  goto L310;
	case 6:  goto L320;
	case 7:  goto L330;
	case 8:  goto L330;
	case 9:  goto L330;
	case 10:  goto L330;
	case 11:  goto L330;
	case 12:  goto L330;
	case 13:  goto L410;
	case 14:  goto L380;
    }

/*     ***  RECOMPUTE STEP WITH NEW RADIUS  *** */

L310:
    v[8] = v[16] * v[2];
    goto L180;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L320:
    v[8] = v[36];
    goto L220;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L330:
    iv[55] = k - 4;
    if (v[10] >= v[13]) {
	goto L430;
    }
    if (iv[13] == 14) {
	goto L430;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L340:
    if (iv[29] != 3) {
	goto L370;
    }
    temp1 = lstgst;

/*     ***  PREPARE FOR GRADIENT TESTS  *** */
/*     ***  SET  TEMP1 = HESSIAN * STEP + G(X0) */
/*     ***             = DIAG(D) * (H * STEP + G(X0)) */

    k = temp1;
    step0 = step1 - 1;
    ipi = iv[58];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = iv[ipi];
	++ipi;
	step1 = step0 + j;
	v[k] = d__[j] * v[step1];
	++k;
/* L350: */
    }
/*        USE X0 VECTOR AS TEMPORARY. */
    ds7lvm_(n, &v[x01], &h__[1], &v[temp1]);
    temp0 = temp1 - 1;
    ipi = iv[58];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = iv[ipi];
	++ipi;
	temp1 = temp0 + j;
	v[temp1] = d__[j] * v[x01] + g[j];
	++x01;
/* L360: */
    }

/*  ***  COMPUTE GRADIENT AND HESSIAN  *** */

L370:
    ++iv[30];
    iv[2] = 0;
    iv[1] = 2;
    goto L450;

L380:
    iv[1] = 2;
    if (iv[29] != 3) {
	goto L140;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

    step1 = iv[40];
/*     *** TEMP1 = STLSTG *** */
    temp1 = step1 + (*n << 1);

/*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

    k = temp1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = (v[k] - g[i__]) / d__[i__];
	++k;
/* L390: */
    }

/*     ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
	goto L400;
    }
    if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L140;
    }
L400:
    v[16] = v[23];
    goto L140;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L410:
    iv[1] = 64;
    goto L440;

/*  ***  INCONSISTENT B  *** */

L420:
    iv[1] = 82;
    goto L440;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L430:
    iv[1] = iv[55];
    iv[55] = 0;
L440:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
    goto L999;

/*  ***  PROJECT X INTO FEASIBLE REGION (PRIOR TO COMPUTING F OR G)  *** */

L450:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (x[i__] < b[(i__ << 1) + 1]) {
	    x[i__] = b[(i__ << 1) + 1];
	}
	if (x[i__] > b[(i__ << 1) + 2]) {
	    x[i__] = b[(i__ << 1) + 2];
	}
/* L460: */
    }

L999:
    return 0;

/*  ***  LAST CARD OF DRMNHB FOLLOWS  *** */
} /* drmnhb_ */

/* Subroutine */ int drmnh_(doublereal *d__, doublereal *fx, doublereal *g, 
	doublereal *h__, integer *iv, integer *lh, integer *liv, integer *lv, 
	integer *n, doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal t;
    static integer w1, x01, dg1, nn1o2, temp1, step1, dummy;
    extern logical stopx_(integer *);
    extern /* Subroutine */ int dd7dup_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dg7qts_(doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *, doublereal *, 
	    doublereal *, doublereal *), ds7lvm_(integer *, doublereal *, 
	    doublereal *, doublereal *), dv2axy_(integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), dv7cpy_(integer *, 
	    doublereal *, doublereal *), dparck_(integer *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);
    static integer lstgst, rstrst;


/*  ***  CARRY OUT  DMNH (UNCONSTRAINED MINIMIZATION) ITERATIONS, USING */
/*  ***  HESSIAN MATRIX PROVIDED BY THE CALLER. */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* D.... SCALE VECTOR. */
/* FX... FUNCTION VALUE. */
/* G.... GRADIENT VECTOR. */
/* H.... LOWER TRIANGLE OF THE HESSIAN, STORED ROWWISE. */
/* IV... INTEGER VALUE ARRAY. */
/* LH... LENGTH OF H = P*(P+1)/2. */
/* LIV.. LENGTH OF IV (AT LEAST 60). */
/* LV... LENGTH OF V (AT LEAST 78 + N*(N+21)/2). */
/* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... PARAMETER VECTOR. */

/*  ***  DISCUSSION  *** */

/*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO  DMNH (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
/*     THE PART OF V THAT  DMNH USES FOR STORING G AND H IS NOT NEEDED). */
/*     MOREOVER, COMPARED WITH  DMNH, IV(1) MAY HAVE THE TWO ADDITIONAL */
/*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
/*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
/*     OUTPUT VALUE FROM  DMNH, IS NOT REFERENCED BY DRMNH OR THE */
/*     SUBROUTINES IT CALLS. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
/*             AT X, AND CALL DRMNH AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
/*             COMPUTED (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN */
/*             BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER */
/*             SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE */
/*             DRMNH TO IGNORE FX AND TRY A SMALLER STEP.  THE PARA- */
/*             METER NF THAT  DMNH PASSES TO CALCF (FOR POSSIBLE USE BY */
/*             CALCGH) IS A COPY OF IV(NFCALL) = IV(6). */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
/*             X, AND H TO THE LOWER TRIANGLE OF H(X), THE HESSIAN OF F */
/*             AT X, AND CALL DRMNH AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS EXCEPT PERHAPS THE SCALE VECTOR D. */
/*                  THE PARAMETER NF THAT  DMNH PASSES TO CALCG IS */
/*             IV(NFGCAL) = IV(7).  IF G(X) AND H(X) CANNOT BE EVALUATED, */
/*             THEN THE CALLER MAY SET IV(TOOBIG) TO 0, IN WHICH CASE */
/*             DRMNH WILL RETURN WITH IV(1) = 65. */
/*                  NOTE -- DRMNH OVERWRITES H WITH THE LOWER TRIANGLE */
/*             OF  DIAG(D)**-1 * H(X) * DIAG(D)**-1. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (WINTER 1980).  REVISED SEPT. 1982. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324 AND MCS-7906671. */

/*        (SEE  DMNG AND  DMNH FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  NO INTRINSIC FUNCTIONS  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DIVSET.... PROVIDES DEFAULT IV AND V INPUT VALUES. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DD7DUP.... UPDATES SCALE VECTOR D. */
/* DG7QTS.... COMPUTES OPTIMALLY LOCALLY CONSTRAINED STEP. */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* DS7LVM... MULTIPLIES SYMMETRIC MATRIX TIMES VECTOR, GIVEN THE LOWER */
/*             TRIANGLE OF THE MATRIX. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */


/*  ***  V SUBSCRIPT VALUES  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --h__;
    --iv;
    --v;
    --x;
    --g;
    --d__;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L30;
    }
    if (i__ == 2) {
	goto L40;
    }

/*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    if (iv[1] == 12 || iv[1] == 13) {
	iv[4] = iv[4] + *n * (*n + 21) / 2 + 7;
    }
    dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    nn1o2 = *n * (*n + 1) / 2;
    if (*lh >= nn1o2) {
	switch (i__) {
	    case 1:  goto L220;
	    case 2:  goto L220;
	    case 3:  goto L220;
	    case 4:  goto L220;
	    case 5:  goto L220;
	    case 6:  goto L220;
	    case 7:  goto L160;
	    case 8:  goto L120;
	    case 9:  goto L160;
	    case 10:  goto L10;
	    case 11:  goto L10;
	    case 12:  goto L20;
	}
    }
    iv[1] = 66;
    goto L400;

/*  ***  STORAGE ALLOCATION  *** */

L10:
    iv[59] = iv[42] + nn1o2;
    iv[43] = iv[59] + (*n << 1);
    iv[40] = iv[43] + *n;
    iv[41] = iv[40] + *n;
    iv[37] = iv[41] + *n;
    iv[34] = iv[37] + *n;
    iv[47] = iv[34] + (*n << 2) + 7;
    if (iv[1] != 13) {
	goto L20;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L20:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[5] = 1;
    iv[11] = 1;
    iv[2] = 0;
    iv[55] = 0;
    iv[8] = 0;
    v[9] = 0.;
    v[5] = 0.;
    if (v[38] >= 0.) {
	dv7scp_(n, &d__[1], &v[38]);
    }
    k = iv[59];
    if (v[39] > 0.) {
	dv7scp_(n, &v[k], &v[39]);
    }
    k += *n;
    if (v[40] > 0.) {
	dv7scp_(n, &v[k], &v[40]);
    }
    iv[1] = 1;
    goto L999;

L30:
    v[10] = *fx;
    if (iv[35] >= 0) {
	goto L220;
    }
    v[13] = *fx;
    iv[1] = 2;
    if (iv[2] == 0) {
	goto L999;
    }
    iv[1] = 63;
    goto L400;

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L40:
    if (iv[2] == 0) {
	goto L50;
    }
    iv[1] = 65;
    goto L400;

/*  ***  UPDATE THE SCALE VECTOR D  *** */

L50:
    dg1 = iv[37];
    if (iv[16] <= 0) {
	goto L70;
    }
    k = dg1;
    j = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j += i__;
	v[k] = h__[j];
	++k;
/* L60: */
    }
    dd7dup_(&d__[1], &v[dg1], &iv[1], liv, lv, n, &v[1]);

/*  ***  COMPUTE SCALED GRADIENT AND ITS NORM  *** */

L70:
    dg1 = iv[37];
    k = dg1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = g[i__] / d__[i__];
	++k;
/* L80: */
    }
    v[1] = dv2nrm_(n, &v[dg1]);

/*  ***  COMPUTE SCALED HESSIAN  *** */

    k = 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = 1. / d__[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    h__[k] = t * h__[k] / d__[j];
	    ++k;
/* L90: */
	}
/* L100: */
    }

    if (iv[55] != 0) {
	goto L390;
    }
    if (iv[35] == 0) {
	goto L350;
    }

/*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

    v[8] = v[35] / (v[21] + 1.);

    iv[35] = 0;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L110:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L120:
    k = iv[31];
    if (k < iv[18]) {
	goto L130;
    }
    iv[1] = 10;
    goto L400;

L130:
    iv[31] = k + 1;

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

    dg1 = iv[37];
    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[33] = -1;

/*     ***  COPY X TO X0  *** */

    dv7cpy_(n, &v[x01], &x[1]);

/*  ***  UPDATE RADIUS  *** */

    if (k == 0) {
	goto L150;
    }
    step1 = iv[40];
    k = step1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = d__[i__] * v[k];
	++k;
/* L140: */
    }
    v[8] = v[16] * dv2nrm_(n, &v[step1]);

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L150:
    if (! stopx_(&dummy)) {
	goto L170;
    }
    iv[1] = 11;
    goto L180;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L160:
    if (v[10] >= v[13]) {
	goto L170;
    }
    v[16] = 1.;
    k = iv[31];
    goto L130;

L170:
    if (iv[6] < iv[17]) {
	goto L190;
    }
    iv[1] = 9;
L180:
    if (v[10] >= v[13]) {
	goto L400;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L340;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L190:
    step1 = iv[40];
    dg1 = iv[37];
    l = iv[42];
    w1 = iv[34];
    dg7qts_(&d__[1], &v[dg1], &h__[1], &iv[33], &v[l], n, &v[step1], &v[1], &
	    v[w1]);
    if (iv[29] != 6) {
	goto L200;
    }
    if (iv[9] != 2) {
	goto L220;
    }
    rstrst = 2;
    goto L230;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L200:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L220;
    }
    if (iv[29] != 5) {
	goto L210;
    }
    if (v[16] <= 1.) {
	goto L210;
    }
    if (v[7] > v[11] * 1.2) {
	goto L210;
    }
    if (iv[9] != 2) {
	goto L220;
    }
    rstrst = 0;
    goto L230;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L210:
    x01 = iv[43];
    step1 = iv[40];
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L999;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L220:
    rstrst = 3;
L230:
    x01 = iv[43];
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = iv[41];
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L270;
	case 2:  goto L240;
	case 3:  goto L250;
	case 4:  goto L260;
    }
L240:
    dv7cpy_(n, &x[1], &v[x01]);
    goto L270;
L250:
    dv7cpy_(n, &v[lstgst], &v[step1]);
    goto L270;
L260:
    dv7cpy_(n, &v[step1], &v[lstgst]);
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

L270:
    k = iv[29];
    switch (k) {
	case 1:  goto L280;
	case 2:  goto L310;
	case 3:  goto L310;
	case 4:  goto L310;
	case 5:  goto L280;
	case 6:  goto L290;
	case 7:  goto L300;
	case 8:  goto L300;
	case 9:  goto L300;
	case 10:  goto L300;
	case 11:  goto L300;
	case 12:  goto L300;
	case 13:  goto L380;
	case 14:  goto L350;
    }

/*     ***  RECOMPUTE STEP WITH NEW RADIUS  *** */

L280:
    v[8] = v[16] * v[2];
    goto L150;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L290:
    v[8] = v[36];
    goto L190;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L300:
    iv[55] = k - 4;
    if (v[10] >= v[13]) {
	goto L390;
    }
    if (iv[13] == 14) {
	goto L390;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L310:
    if (iv[29] != 3) {
	goto L340;
    }
    temp1 = lstgst;

/*     ***  PREPARE FOR GRADIENT TESTS  *** */
/*     ***  SET  TEMP1 = HESSIAN * STEP + G(X0) */
/*     ***             = DIAG(D) * (H * STEP + G(X0)) */

/*        USE X0 VECTOR AS TEMPORARY. */
    k = x01;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = d__[i__] * v[step1];
	++k;
	++step1;
/* L320: */
    }
    ds7lvm_(n, &v[temp1], &h__[1], &v[x01]);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[temp1] = d__[i__] * v[temp1] + g[i__];
	++temp1;
/* L330: */
    }

/*  ***  COMPUTE GRADIENT AND HESSIAN  *** */

L340:
    ++iv[30];
    iv[2] = 0;
    iv[1] = 2;
    goto L999;

L350:
    iv[1] = 2;
    if (iv[29] != 3) {
	goto L110;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

    temp1 = iv[41];
    step1 = iv[40];

/*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

    k = temp1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = (v[k] - g[i__]) / d__[i__];
	++k;
/* L360: */
    }

/*     ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
	goto L370;
    }
    if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L110;
    }
L370:
    v[16] = v[23];
    goto L110;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L380:
    iv[1] = 64;
    goto L400;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L390:
    iv[1] = iv[55];
    iv[55] = 0;
L400:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);

L999:
    return 0;

/*  ***  LAST CARD OF DRMNH FOLLOWS  *** */
} /* drmnh_ */

/* Subroutine */ int dq7rsh_(integer *k, integer *p, logical *havqtr, 
	doublereal *qtr, doublereal *r__, doublereal *w)
{
    /* Initialized data */

    static doublereal zero = 0.;

    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static doublereal a, b;
    static integer i__, j;
    static doublereal t, x, y, z__;
    static integer i1, j1, k1;
    static doublereal wj;
    static integer jm1, km1, jp1, pm1;
    extern /* Subroutine */ int dh2rfa_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *);
    extern doublereal dh2rfg_(doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    extern /* Subroutine */ int dv7cpy_(integer *, doublereal *, doublereal *)
	    ;


/*  ***  PERMUTE COLUMN K OF R TO COLUMN P, MODIFY QTR ACCORDINGLY  *** */

/*     DIMENSION R(P*(P+1)/2) */


/*  ***  LOCAL VARIABLES  *** */


    /* Parameter adjustments */
    --w;
    --qtr;
    --r__;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    if (*k >= *p) {
	goto L999;
    }
    km1 = *k - 1;
    k1 = *k * km1 / 2;
    dv7cpy_(k, &w[1], &r__[k1 + 1]);
    wj = w[*k];
    pm1 = *p - 1;
    j1 = k1 + km1;
    i__1 = pm1;
    for (j = *k; j <= i__1; ++j) {
	jm1 = j - 1;
	jp1 = j + 1;
	if (jm1 > 0) {
	    dv7cpy_(&jm1, &r__[k1 + 1], &r__[j1 + 2]);
	}
	j1 += jp1;
	k1 += j;
	a = r__[j1];
	b = r__[j1 + 1];
	if (b != zero) {
	    goto L10;
	}
	r__[k1] = a;
	x = zero;
	z__ = zero;
	goto L40;
L10:
	r__[k1] = dh2rfg_(&a, &b, &x, &y, &z__);
	if (j == pm1) {
	    goto L30;
	}
	i1 = j1;
	i__2 = pm1;
	for (i__ = jp1; i__ <= i__2; ++i__) {
	    i1 += i__;
	    dh2rfa_(&c__1, &r__[i1], &r__[i1 + 1], &x, &y, &z__);
/* L20: */
	}
L30:
	if (*havqtr) {
	    dh2rfa_(&c__1, &qtr[j], &qtr[jp1], &x, &y, &z__);
	}
L40:
	t = x * wj;
	w[j] = wj + t;
	wj = t * z__;
/* L50: */
    }
    w[*p] = wj;
    dv7cpy_(p, &r__[k1 + 1], &w[1]);
L999:
    return 0;
} /* dq7rsh_ */

/* Subroutine */ int drmnf_(doublereal *d__, doublereal *fx, integer *iv, 
	integer *liv, integer *lv, integer *n, doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, w, g1, iv1, alpha;
    extern /* Subroutine */ int drmng_(doublereal *, doublereal *, doublereal 
	    *, integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *), ds7grd_(doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , divset_(integer *, integer *, integer *, integer *, doublereal *
	    );


/*  ***  ITERATION DRIVER FOR  DMNF... */
/*  ***  MINIMIZE GENERAL UNCONSTRAINED OBJECTIVE FUNCTION USING */
/*  ***  FINITE-DIFFERENCE GRADIENTS AND SECANT HESSIAN APPROXIMATIONS. */

/*     DIMENSION V(77 + N*(N+17)/2) */

/*  ***  PURPOSE  *** */

/*        THIS ROUTINE INTERACTS WITH SUBROUTINE  DRMNG  IN AN ATTEMPT */
/*     TO FIND AN N-VECTOR  X*  THAT MINIMIZES THE (UNCONSTRAINED) */
/*     OBJECTIVE FUNCTION  FX = F(X)  COMPUTED BY THE CALLER.  (OFTEN */
/*     THE  X*  FOUND IS A LOCAL MINIMIZER RATHER THAN A GLOBAL ONE.) */

/*  ***  PARAMETERS  *** */

/*        THE PARAMETERS FOR DRMNF ARE THE SAME AS THOSE FOR  DMNG */
/*     (WHICH SEE), EXCEPT THAT CALCF, CALCG, UIPARM, URPARM, AND UFPARM */
/*     ARE OMITTED, AND A PARAMETER  FX  FOR THE OBJECTIVE FUNCTION */
/*     VALUE AT X IS ADDED.  INSTEAD OF CALLING CALCG TO OBTAIN THE */
/*     GRADIENT OF THE OBJECTIVE FUNCTION AT X, DRMNF CALLS DS7GRD, */
/*     WHICH COMPUTES AN APPROXIMATION TO THE GRADIENT BY FINITE */
/*     (FORWARD AND CENTRAL) DIFFERENCES USING THE METHOD OF REF. 1. */
/*     THE FOLLOWING INPUT COMPONENT IS OF INTEREST IN THIS REGARD */
/*     (AND IS NOT DESCRIBED IN  DMNG). */

/* V(ETA0)..... V(42) IS AN ESTIMATED BOUND ON THE RELATIVE ERROR IN THE */
/*             OBJECTIVE FUNCTION VALUE COMPUTED BY CALCF... */
/*                  (TRUE VALUE) = (COMPUTED VALUE) * (1 + E), */
/*             WHERE ABS(E) .LE. V(ETA0).  DEFAULT = MACHEP * 10**3, */
/*             WHERE MACHEP IS THE UNIT ROUNDOFF. */

/*        THE OUTPUT VALUES IV(NFCALL) AND IV(NGCALL) HAVE DIFFERENT */
/*     MEANINGS FOR  DMNF THAN FOR  DMNG... */

/* IV(NFCALL)... IV(6) IS THE NUMBER OF CALLS SO FAR MADE ON CALCF (I.E., */
/*             FUNCTION EVALUATIONS) EXCLUDING THOSE MADE ONLY FOR */
/*             COMPUTING GRADIENTS.  THE INPUT VALUE IV(MXFCAL) IS A */
/*             LIMIT ON IV(NFCALL). */
/* IV(NGCALL)... IV(30) IS THE NUMBER OF FUNCTION EVALUATIONS MADE ONLY */
/*             FOR COMPUTING GRADIENTS.  THE TOTAL NUMBER OF FUNCTION */
/*             EVALUATIONS IS THUS  IV(NFCALL) + IV(NGCALL). */

/*  ***  REFERENCES  *** */

/* 1. STEWART, G.W. (1967), A MODIFICATION OF DAVIDON*S MINIMIZATION */
/*        METHOD TO ACCEPT DIFFERENCE APPROXIMATIONS OF DERIVATIVES, */
/*        J. ASSOC. COMPUT. MACH. 14, PP. 72-83. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (AUGUST 1982). */

/* ----------------------------  DECLARATIONS  --------------------------- */


/* DIVSET.... SUPPLIES DEFAULT PARAMETER VALUES. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DS7GRD... COMPUTES FINITE-DIFFERENCE GRADIENT APPROXIMATION. */
/* DRMNG.... REVERSE-COMMUNICATION ROUTINE THAT DOES  DMNG ALGORITHM. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */


/*  ***  SUBSCRIPTS FOR IV   *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --d__;

    /* Function Body */
    iv1 = iv[1];
    if (iv1 == 1) {
	goto L10;
    }
    if (iv1 == 2) {
	goto L50;
    }
    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    iv1 = iv[1];
    if (iv1 == 12 || iv1 == 13) {
	iv[4] = iv[4] + (*n << 1) + 6;
    }
    if (iv1 == 14) {
	goto L10;
    }
    if (iv1 > 2 && iv1 < 12) {
	goto L10;
    }
    g1 = 1;
    if (iv1 == 12) {
	iv[1] = 13;
    }
    goto L20;

L10:
    g1 = iv[28];

L20:
    drmng_(&d__[1], fx, &v[g1], &iv[1], liv, lv, n, &v[1], &x[1]);
    if (iv[1] < 2) {
	goto L999;
    }
    if (iv[1] > 2) {
	goto L70;
    }

/*  ***  COMPUTE GRADIENT  *** */

    if (iv[31] == 0) {
	dv7scp_(n, &v[g1], &c_b14);
    }
    j = iv[42];
    k = g1 - *n;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = dd7tpr_(&i__, &v[j], &v[j]);
	++k;
	j += i__;
/* L40: */
    }
/*     ***  UNDO INCREMENT OF IV(NGCALL) DONE BY DRMNG  *** */
    --iv[30];
/*     ***  STORE RETURN CODE FROM DS7GRD IN IV(SGIRC)  *** */
    iv[57] = 0;
/*     ***  X MAY HAVE BEEN RESTORED, SO COPY BACK FX... *** */
    *fx = v[10];
    goto L60;

/*     ***  GRADIENT LOOP  *** */

L50:
    if (iv[2] != 0) {
	goto L10;
    }

L60:
    g1 = iv[28];
    alpha = g1 - *n;
    w = alpha - 6;
    ds7grd_(&v[alpha], &d__[1], &v[42], fx, &v[g1], &iv[57], n, &v[w], &x[1]);
    if (iv[57] == 0) {
	goto L10;
    }
    ++iv[30];
    goto L999;

L70:
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[28] = iv[47] + *n + 6;
    iv[47] = iv[28] + *n;
    if (iv1 != 13) {
	goto L10;
    }

L999:
    return 0;
/*  ***  LAST CARD OF DRMNF FOLLOWS  *** */
} /* drmnf_ */

/* Subroutine */ int dl7vml_(integer *n, doublereal *x, doublereal *l, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j;
    static doublereal t;
    static integer i0, ii, ij, np1;


/*  ***  COMPUTE  X = L*Y, WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
/*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
/*  ***  STORAGE.  *** */

/*     DIMENSION L(N*(N+1)/2) */

    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    np1 = *n + 1;
    i0 = *n * (*n + 1) / 2;
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = np1 - ii;
	i0 -= i__;
	t = 0.;
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    ij = i0 + j;
	    t += l[ij] * y[j];
/* L10: */
	}
	x[i__] = t;
/* L20: */
    }
    return 0;
/*  ***  LAST CARD OF DL7VML FOLLOWS  *** */
} /* dl7vml_ */

/* Subroutine */ int da7sst_(integer *iv, integer *liv, integer *lv, 
	doublereal *v)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Local variables */
    static integer i__, nfc;
    static doublereal gts, emax, xmax, rfac1, emaxs;
    static logical goodx;


/*  ***  ASSESS CANDIDATE STEP (***SOL VERSION 2.3)  *** */


/*  ***  PURPOSE  *** */

/*        THIS SUBROUTINE IS CALLED BY AN UNCONSTRAINED MINIMIZATION */
/*     ROUTINE TO ASSESS THE NEXT CANDIDATE STEP.  IT MAY RECOMMEND ONE */
/*     OF SEVERAL COURSES OF ACTION, SUCH AS ACCEPTING THE STEP, RECOM- */
/*     PUTING IT USING THE SAME OR A NEW QUADRATIC MODEL, OR HALTING DUE */
/*     TO CONVERGENCE OR FALSE CONVERGENCE.  SEE THE RETURN CODE LISTING */
/*     BELOW. */

/* --------------------------  PARAMETER USAGE  -------------------------- */

/*  IV (I/O) INTEGER PARAMETER AND SCRATCH VECTOR -- SEE DESCRIPTION */
/*             BELOW OF IV VALUES REFERENCED. */
/* LIV (IN)  LENGTH OF IV ARRAY. */
/*  LV (IN)  LENGTH OF V ARRAY. */
/*   V (I/O) REAL PARAMETER AND SCRATCH VECTOR -- SEE DESCRIPTION */
/*             BELOW OF V VALUES REFERENCED. */

/*  ***  IV VALUES REFERENCED  *** */

/*    IV(IRC) (I/O) ON INPUT FOR THE FIRST STEP TRIED IN A NEW ITERATION, */
/*             IV(IRC) SHOULD BE SET TO 3 OR 4 (THE VALUE TO WHICH IT IS */
/*             SET WHEN STEP IS DEFINITELY TO BE ACCEPTED).  ON INPUT */
/*             AFTER STEP HAS BEEN RECOMPUTED, IV(IRC) SHOULD BE */
/*             UNCHANGED SINCE THE PREVIOUS RETURN OF DA7SST. */
/*                ON OUTPUT, IV(IRC) IS A RETURN CODE HAVING ONE OF THE */
/*             FOLLOWING VALUES... */
/*                  1 = SWITCH MODELS OR TRY SMALLER STEP. */
/*                  2 = SWITCH MODELS OR ACCEPT STEP. */
/*                  3 = ACCEPT STEP AND DETERMINE V(RADFAC) BY GRADIENT */
/*                       TESTS. */
/*                  4 = ACCEPT STEP, V(RADFAC) HAS BEEN DETERMINED. */
/*                  5 = RECOMPUTE STEP (USING THE SAME MODEL). */
/*                  6 = RECOMPUTE STEP WITH RADIUS = V(LMAXS) BUT DO NOT */
/*                       EVALUATE THE OBJECTIVE FUNCTION. */
/*                  7 = X-CONVERGENCE (SEE V(XCTOL)). */
/*                  8 = RELATIVE FUNCTION CONVERGENCE (SEE V(RFCTOL)). */
/*                  9 = BOTH X- AND RELATIVE FUNCTION CONVERGENCE. */
/*                 10 = ABSOLUTE FUNCTION CONVERGENCE (SEE V(AFCTOL)). */
/*                 11 = SINGULAR CONVERGENCE (SEE V(LMAXS)). */
/*                 12 = FALSE CONVERGENCE (SEE V(XFTOL)). */
/*                 13 = IV(IRC) WAS OUT OF RANGE ON INPUT. */
/*             RETURN CODE I HAS PRECEDENCE OVER I+1 FOR I = 9, 10, 11. */
/* IV(MLSTGD) (I/O) SAVED VALUE OF IV(MODEL). */
/*  IV(MODEL) (I/O) ON INPUT, IV(MODEL) SHOULD BE AN INTEGER IDENTIFYING */
/*             THE CURRENT QUADRATIC MODEL OF THE OBJECTIVE FUNCTION. */
/*             IF A PREVIOUS STEP YIELDED A BETTER FUNCTION REDUCTION, */
/*             THEN IV(MODEL) WILL BE SET TO IV(MLSTGD) ON OUTPUT. */
/* IV(NFCALL) (IN)  INVOCATION COUNT FOR THE OBJECTIVE FUNCTION. */
/* IV(NFGCAL) (I/O) VALUE OF IV(NFCALL) AT STEP THAT GAVE THE BIGGEST */
/*             FUNCTION REDUCTION THIS ITERATION.  IV(NFGCAL) REMAINS */
/*             UNCHANGED UNTIL A FUNCTION REDUCTION IS OBTAINED. */
/* IV(RADINC) (I/O) THE NUMBER OF RADIUS INCREASES (OR MINUS THE NUMBER */
/*             OF DECREASES) SO FAR THIS ITERATION. */
/* IV(RESTOR) (OUT) SET TO 1 IF V(F) HAS BEEN RESTORED AND X SHOULD BE */
/*             RESTORED TO ITS INITIAL VALUE, TO 2 IF X SHOULD BE SAVED, */
/*             TO 3 IF X SHOULD BE RESTORED FROM THE SAVED VALUE, AND TO */
/*             0 OTHERWISE. */
/*  IV(STAGE) (I/O) COUNT OF THE NUMBER OF MODELS TRIED SO FAR IN THE */
/*             CURRENT ITERATION. */
/* IV(STGLIM) (IN)  MAXIMUM NUMBER OF MODELS TO CONSIDER. */
/* IV(SWITCH) (OUT) SET TO 0 UNLESS A NEW MODEL IS BEING TRIED AND IT */
/*             GIVES A SMALLER FUNCTION VALUE THAN THE PREVIOUS MODEL, */
/*             IN WHICH CASE DA7SST SETS IV(SWITCH) = 1. */
/* IV(TOOBIG) (I/O)  IS NONZERO ON INPUT IF STEP WAS TOO BIG (E.G., IF */
/*             IT WOULD CAUSE OVERFLOW).  IT IS SET TO 0 ON RETURN. */
/*   IV(XIRC) (I/O) VALUE THAT IV(IRC) WOULD HAVE IN THE ABSENCE OF */
/*             CONVERGENCE, FALSE CONVERGENCE, AND OVERSIZED STEPS. */

/*  ***  V VALUES REFERENCED  *** */

/* V(AFCTOL) (IN)  ABSOLUTE FUNCTION CONVERGENCE TOLERANCE.  IF THE */
/*             ABSOLUTE VALUE OF THE CURRENT FUNCTION VALUE V(F) IS LESS */
/*             THAN V(AFCTOL) AND DA7SST DOES NOT RETURN WITH */
/*             IV(IRC) = 11, THEN DA7SST RETURNS WITH IV(IRC) = 10. */
/* V(DECFAC) (IN)  FACTOR BY WHICH TO DECREASE RADIUS WHEN IV(TOOBIG) IS */
/*             NONZERO. */
/* V(DSTNRM) (IN)  THE 2-NORM OF D*STEP. */
/* V(DSTSAV) (I/O) VALUE OF V(DSTNRM) ON SAVED STEP. */
/*   V(DST0) (IN)  THE 2-NORM OF D TIMES THE NEWTON STEP (WHEN DEFINED, */
/*             I.E., FOR V(NREDUC) .GE. 0). */
/*      V(F) (I/O) ON BOTH INPUT AND OUTPUT, V(F) IS THE OBJECTIVE FUNC- */
/*             TION VALUE AT X.  IF X IS RESTORED TO A PREVIOUS VALUE, */
/*             THEN V(F) IS RESTORED TO THE CORRESPONDING VALUE. */
/*   V(FDIF) (OUT) THE FUNCTION REDUCTION V(F0) - V(F) (FOR THE OUTPUT */
/*             VALUE OF V(F) IF AN EARLIER STEP GAVE A BIGGER FUNCTION */
/*             DECREASE, AND FOR THE INPUT VALUE OF V(F) OTHERWISE). */
/* V(FLSTGD) (I/O) SAVED VALUE OF V(F). */
/*     V(F0) (IN)  OBJECTIVE FUNCTION VALUE AT START OF ITERATION. */
/* V(GTSLST) (I/O) VALUE OF V(GTSTEP) ON SAVED STEP. */
/* V(GTSTEP) (IN)  INNER PRODUCT BETWEEN STEP AND GRADIENT. */
/* V(INCFAC) (IN)  MINIMUM FACTOR BY WHICH TO INCREASE RADIUS. */
/*  V(LMAXS) (IN)  MAXIMUM REASONABLE STEP SIZE (AND INITIAL STEP BOUND). */
/*             IF THE ACTUAL FUNCTION DECREASE IS NO MORE THAN TWICE */
/*             WHAT WAS PREDICTED, IF A RETURN WITH IV(IRC) = 7, 8, OR 9 */
/*             DOES NOT OCCUR, IF V(DSTNRM) .GT. V(LMAXS) OR THE CURRENT */
/*             STEP IS A NEWTON STEP, AND IF */
/*             V(PREDUC) .LE. V(SCTOL) * ABS(V(F0)), THEN DA7SST RETURNS */
/*             WITH IV(IRC) = 11.  IF SO DOING APPEARS WORTHWHILE, THEN */
/*            DA7SST REPEATS THIS TEST (DISALLOWING A FULL NEWTON STEP) */
/*             WITH V(PREDUC) COMPUTED FOR A STEP OF LENGTH V(LMAXS) */
/*             (BY A RETURN WITH IV(IRC) = 6). */
/* V(NREDUC) (I/O)  FUNCTION REDUCTION PREDICTED BY QUADRATIC MODEL FOR */
/*             NEWTON STEP.  IF DA7SST IS CALLED WITH IV(IRC) = 6, I.E., */
/*             IF V(PREDUC) HAS BEEN COMPUTED WITH RADIUS = V(LMAXS) FOR */
/*             USE IN THE SINGULAR CONVERGENCE TEST, THEN V(NREDUC) IS */
/*             SET TO -V(PREDUC) BEFORE THE LATTER IS RESTORED. */
/* V(PLSTGD) (I/O) VALUE OF V(PREDUC) ON SAVED STEP. */
/* V(PREDUC) (I/O) FUNCTION REDUCTION PREDICTED BY QUADRATIC MODEL FOR */
/*             CURRENT STEP. */
/* V(RADFAC) (OUT) FACTOR TO BE USED IN DETERMINING THE NEW RADIUS, */
/*             WHICH SHOULD BE V(RADFAC)*DST, WHERE  DST  IS EITHER THE */
/*             OUTPUT VALUE OF V(DSTNRM) OR THE 2-NORM OF */
/*             DIAG(NEWD)*STEP  FOR THE OUTPUT VALUE OF STEP AND THE */
/*             UPDATED VERSION, NEWD, OF THE SCALE VECTOR D.  FOR */
/*             IV(IRC) = 3, V(RADFAC) = 1.0 IS RETURNED. */
/* V(RDFCMN) (IN)  MINIMUM VALUE FOR V(RADFAC) IN TERMS OF THE INPUT */
/*             VALUE OF V(DSTNRM) -- SUGGESTED VALUE = 0.1. */
/* V(RDFCMX) (IN)  MAXIMUM VALUE FOR V(RADFAC) -- SUGGESTED VALUE = 4.0. */
/*  V(RELDX) (IN) SCALED RELATIVE CHANGE IN X CAUSED BY STEP, COMPUTED */
/*             (E.G.) BY FUNCTION  DRLDST  AS */
/*                 MAX (D(I)*ABS(X(I)-X0(I)), 1 .LE. I .LE. P) / */
/*                    MAX (D(I)*(ABS(X(I))+ABS(X0(I))), 1 .LE. I .LE. P). */
/* V(RFCTOL) (IN)  RELATIVE FUNCTION CONVERGENCE TOLERANCE.  IF THE */
/*             ACTUAL FUNCTION REDUCTION IS AT MOST TWICE WHAT WAS PRE- */
/*             DICTED AND  V(NREDUC) .LE. V(RFCTOL)*ABS(V(F0)),  THEN */
/*            DA7SST RETURNS WITH IV(IRC) = 8 OR 9. */
/*  V(SCTOL) (IN)  SINGULAR CONVERGENCE TOLERANCE -- SEE V(LMAXS). */
/* V(STPPAR) (IN)  MARQUARDT PARAMETER -- 0 MEANS FULL NEWTON STEP. */
/* V(TUNER1) (IN)  TUNING CONSTANT USED TO DECIDE IF THE FUNCTION */
/*             REDUCTION WAS MUCH LESS THAN EXPECTED.  SUGGESTED */
/*             VALUE = 0.1. */
/* V(TUNER2) (IN)  TUNING CONSTANT USED TO DECIDE IF THE FUNCTION */
/*             REDUCTION WAS LARGE ENOUGH TO ACCEPT STEP.  SUGGESTED */
/*             VALUE = 10**-4. */
/* V(TUNER3) (IN)  TUNING CONSTANT USED TO DECIDE IF THE RADIUS */
/*             SHOULD BE INCREASED.  SUGGESTED VALUE = 0.75. */
/*  V(XCTOL) (IN)  X-CONVERGENCE CRITERION.  IF STEP IS A NEWTON STEP */
/*             (V(STPPAR) = 0) HAVING V(RELDX) .LE. V(XCTOL) AND GIVING */
/*             AT MOST TWICE THE PREDICTED FUNCTION DECREASE, THEN */
/*            DA7SST RETURNS IV(IRC) = 7 OR 9. */
/*  V(XFTOL) (IN)  FALSE CONVERGENCE TOLERANCE.  IF STEP GAVE NO OR ONLY */
/*             A SMALL FUNCTION DECREASE AND V(RELDX) .LE. V(XFTOL), */
/*             THEN DA7SST RETURNS WITH IV(IRC) = 12. */

/* -------------------------------  NOTES  ------------------------------- */

/*  ***  APPLICATION AND USAGE RESTRICTIONS  *** */

/*        THIS ROUTINE IS CALLED AS PART OF THE NL2SOL (NONLINEAR */
/*     LEAST-SQUARES) PACKAGE.  IT MAY BE USED IN ANY UNCONSTRAINED */
/*     MINIMIZATION SOLVER THAT USES DOGLEG, GOLDFELD-QUANDT-TROTTER, */
/*     OR LEVENBERG-MARQUARDT STEPS. */

/*  ***  ALGORITHM NOTES  *** */

/*        SEE (1) FOR FURTHER DISCUSSION OF THE ASSESSING AND MODEL */
/*     SWITCHING STRATEGIES.  WHILE NL2SOL CONSIDERS ONLY TWO MODELS, */
/*    DA7SST IS DESIGNED TO HANDLE ANY NUMBER OF MODELS. */

/*  ***  USAGE NOTES  *** */

/*        ON THE FIRST CALL OF AN ITERATION, ONLY THE I/O VARIABLES */
/*     STEP, X, IV(IRC), IV(MODEL), V(F), V(DSTNRM), V(GTSTEP), AND */
/*     V(PREDUC) NEED HAVE BEEN INITIALIZED.  BETWEEN CALLS, NO I/O */
/*     VALUES EXCEPT STEP, X, IV(MODEL), V(F) AND THE STOPPING TOLER- */
/*     ANCES SHOULD BE CHANGED. */
/*        AFTER A RETURN FOR CONVERGENCE OR FALSE CONVERGENCE, ONE CAN */
/*     CHANGE THE STOPPING TOLERANCES AND CALL DA7SST AGAIN, IN WHICH */
/*     CASE THE STOPPING TESTS WILL BE REPEATED. */

/*  ***  REFERENCES  *** */

/*     (1) DENNIS, J.E., JR., GAY, D.M., AND WELSCH, R.E. (1981), */
/*        AN ADAPTIVE NONLINEAR LEAST-SQUARES ALGORITHM, */
/*        ACM TRANS. MATH. SOFTWARE, VOL. 7, NO. 3. */

/*     (2) POWELL, M.J.D. (1970)  A FORTRAN SUBROUTINE FOR SOLVING */
/*        SYSTEMS OF NONLINEAR ALGEBRAIC EQUATIONS, IN NUMERICAL */
/*        METHODS FOR NONLINEAR ALGEBRAIC EQUATIONS, EDITED BY */
/*        P. RABINOWITZ, GORDON AND BREACH, LONDON. */

/*  ***  HISTORY  *** */

/*        JOHN DENNIS DESIGNED MUCH OF THIS ROUTINE, STARTING WITH */
/*     IDEAS IN (2). ROY WELSCH SUGGESTED THE MODEL SWITCHING STRATEGY. */
/*        DAVID GAY AND STEPHEN PETERS CAST THIS SUBROUTINE INTO A MORE */
/*     PORTABLE FORM (WINTER 1977), AND DAVID GAY CAST IT INTO ITS */
/*     PRESENT FORM (FALL 1978), WITH MINOR CHANGES TO THE SINGULAR */
/*     CONVERGENCE TEST IN MAY, 1984 (TO DEAL WITH FULL NEWTON STEPS). */

/*  ***  GENERAL  *** */

/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, MCS76-11989, AND */
/*     MCS-7906671. */

/* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

/*  ***  NO EXTERNAL FUNCTIONS AND SUBROUTINES  *** */

/* --------------------------  LOCAL VARIABLES  -------------------------- */


/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  DATA INITIALIZATIONS  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;

    /* Function Body */
    nfc = iv[6];
    iv[12] = 0;
    iv[9] = 0;
    rfac1 = 1.;
    goodx = TRUE_;
    i__ = iv[29];
    if (i__ >= 1 && i__ <= 12) {
	switch (i__) {
	    case 1:  goto L20;
	    case 2:  goto L30;
	    case 3:  goto L10;
	    case 4:  goto L10;
	    case 5:  goto L40;
	    case 6:  goto L280;
	    case 7:  goto L220;
	    case 8:  goto L220;
	    case 9:  goto L220;
	    case 10:  goto L220;
	    case 11:  goto L220;
	    case 12:  goto L170;
	}
    }
    iv[29] = 13;
    goto L999;

/*  ***  INITIALIZE FOR NEW ITERATION  *** */

L10:
    iv[10] = 1;
    iv[8] = 0;
    v[12] = v[13];
    if (iv[2] == 0) {
	goto L110;
    }
    iv[10] = -1;
    iv[13] = i__;
    goto L60;

/*  ***  STEP WAS RECOMPUTED WITH NEW MODEL OR SMALLER RADIUS  *** */
/*  ***  FIRST DECIDE WHICH  *** */

L20:
    if (iv[5] != iv[32]) {
	goto L30;
    }
/*        ***  OLD MODEL RETAINED, SMALLER RADIUS TRIED  *** */
/*        ***  DO NOT CONSIDER ANY MORE NEW MODELS THIS ITERATION  *** */
    iv[10] = iv[11];
    iv[8] = -1;
    goto L110;

/*  ***  A NEW MODEL IS BEING TRIED.  DECIDE WHETHER TO KEEP IT.  *** */

L30:
    ++iv[10];

/*     ***  NOW WE ADD THE POSSIBILITY THAT STEP WAS RECOMPUTED WITH  *** */
/*     ***  THE SAME MODEL, PERHAPS BECAUSE OF AN OVERSIZED STEP.     *** */

L40:
    if (iv[10] > 0) {
	goto L50;
    }

/*        ***  STEP WAS RECOMPUTED BECAUSE IT WAS TOO BIG.  *** */

    if (iv[2] != 0) {
	goto L60;
    }

/*        ***  RESTORE IV(STAGE) AND PICK UP WHERE WE LEFT OFF.  *** */

    iv[10] = -iv[10];
    i__ = iv[13];
    switch (i__) {
	case 1:  goto L20;
	case 2:  goto L30;
	case 3:  goto L110;
	case 4:  goto L110;
	case 5:  goto L70;
    }

L50:
    if (iv[2] == 0) {
	goto L70;
    }

/*  ***  HANDLE OVERSIZE STEP  *** */

    iv[2] = 0;
    if (iv[8] > 0) {
	goto L80;
    }
    iv[10] = -iv[10];
    iv[13] = iv[29];

L60:
    iv[2] = 0;
    v[16] = v[22];
    --iv[8];
    iv[29] = 5;
    iv[9] = 1;
    v[10] = v[12];
    goto L999;

L70:
    if (v[10] < v[12]) {
	goto L110;
    }

/*     *** THE NEW STEP IS A LOSER.  RESTORE OLD MODEL.  *** */

    if (iv[5] == iv[32]) {
	goto L80;
    }
    iv[5] = iv[32];
    iv[12] = 1;

/*     ***  RESTORE STEP, ETC. ONLY IF A PREVIOUS STEP DECREASED V(F). */

L80:
    if (v[12] >= v[13]) {
	goto L110;
    }
    if (iv[10] < iv[11]) {
	goodx = FALSE_;
    } else if (nfc < iv[7] + iv[11] + 2) {
	goodx = FALSE_;
    } else if (iv[12] != 0) {
	goodx = FALSE_;
    }
    iv[9] = 3;
    v[10] = v[12];
    v[7] = v[15];
    v[4] = v[14];
    if (iv[12] == 0) {
	rfac1 = v[2] / v[18];
    }
    v[2] = v[18];
    if (goodx) {

/*     ***  ACCEPT PREVIOUS SLIGHTLY REDUCING STEP *** */

	v[11] = v[13] - v[10];
	iv[29] = 4;
	v[16] = rfac1;
	goto L999;
    }
    nfc = iv[7];

L110:
    v[11] = v[13] - v[10];
    if (v[11] > v[27] * v[7]) {
	goto L140;
    }
    if (iv[8] > 0) {
	goto L140;
    }

/*        ***  NO (OR ONLY A TRIVIAL) FUNCTION DECREASE */
/*        ***  -- SO TRY NEW MODEL OR SMALLER RADIUS */

    if (v[10] < v[13]) {
	goto L120;
    }
    iv[32] = iv[5];
    v[12] = v[10];
    v[10] = v[13];
    iv[9] = 1;
    goto L130;
L120:
    iv[7] = nfc;
L130:
    iv[29] = 1;
    if (iv[10] < iv[11]) {
	goto L160;
    }
    iv[29] = 5;
    --iv[8];
    goto L160;

/*  ***  NONTRIVIAL FUNCTION DECREASE ACHIEVED  *** */

L140:
    iv[7] = nfc;
    rfac1 = 1.;
    v[18] = v[2];
    if (v[11] > v[7] * v[26]) {
	goto L190;
    }

/*  ***  DECREASE WAS MUCH LESS THAN PREDICTED -- EITHER CHANGE MODELS */
/*  ***  OR ACCEPT STEP WITH DECREASED RADIUS. */

    if (iv[10] >= iv[11]) {
	goto L150;
    }
/*        ***  CONSIDER SWITCHING MODELS  *** */
    iv[29] = 2;
    goto L160;

/*     ***  ACCEPT STEP WITH DECREASED RADIUS  *** */

L150:
    iv[29] = 4;

/*  ***  SET V(RADFAC) TO FLETCHER*S DECREASE FACTOR  *** */

L160:
    iv[13] = iv[29];
    emax = v[4] + v[11];
    v[16] = rfac1 * .5;
    if (emax < v[4]) {
/* Computing MAX */
	d__1 = v[24], d__2 = v[4] * .5 / emax;
	v[16] = rfac1 * max(d__1,d__2);
    }

/*  ***  DO FALSE CONVERGENCE TEST  *** */

L170:
    if (v[17] <= v[34]) {
	goto L180;
    }
    iv[29] = iv[13];
    if (v[10] < v[13]) {
	goto L200;
    }
    goto L230;

L180:
    iv[29] = 12;
    goto L240;

/*  ***  HANDLE GOOD FUNCTION DECREASE  *** */

L190:
    if (v[11] < -v[28] * v[4]) {
	goto L210;
    }

/*     ***  INCREASING RADIUS LOOKS WORTHWHILE.  SEE IF WE JUST */
/*     ***  RECOMPUTED STEP WITH A DECREASED RADIUS OR RESTORED STEP */
/*     ***  AFTER RECOMPUTING IT WITH A LARGER RADIUS. */

    if (iv[8] < 0) {
	goto L210;
    }
    if (iv[9] == 1) {
	goto L210;
    }
    if (iv[9] == 3) {
	goto L210;
    }

/*        ***  WE DID NOT.  TRY A LONGER STEP UNLESS THIS WAS A NEWTON */
/*        ***  STEP. */

    v[16] = v[25];
    gts = v[4];
    if (v[11] < (.5 / v[16] - 1.) * gts) {
/* Computing MAX */
	d__1 = v[23], d__2 = gts * .5 / (gts + v[11]);
	v[16] = max(d__1,d__2);
    }
    iv[29] = 4;
    if (v[5] == 0.) {
	goto L230;
    }
    if (v[3] >= 0. && (v[3] < v[2] * 2. || v[6] < v[11] * 1.2)) {
	goto L230;
    }
/*             ***  STEP WAS NOT A NEWTON STEP.  RECOMPUTE IT WITH */
/*             ***  A LARGER RADIUS. */
    iv[29] = 5;
    ++iv[8];

/*  ***  SAVE VALUES CORRESPONDING TO GOOD STEP  *** */

L200:
    v[12] = v[10];
    iv[32] = iv[5];
    if (iv[9] == 0) {
	iv[9] = 2;
    }
    v[18] = v[2];
    iv[7] = nfc;
    v[15] = v[7];
    v[14] = v[4];
    goto L230;

/*  ***  ACCEPT STEP WITH RADIUS UNCHANGED  *** */

L210:
    v[16] = 1.;
    iv[29] = 3;
    goto L230;

/*  ***  COME HERE FOR A RESTART AFTER CONVERGENCE  *** */

L220:
    iv[29] = iv[13];
    if (v[18] >= 0.) {
	goto L240;
    }
    iv[29] = 12;
    goto L240;

/*  ***  PERFORM CONVERGENCE TESTS  *** */

L230:
    iv[13] = iv[29];
L240:
    if (iv[9] == 1 && v[12] < v[13]) {
	iv[9] = 3;
    }
    if (abs(v[10]) < v[31]) {
	iv[29] = 10;
    }
    if (v[11] * .5 > v[7]) {
	goto L999;
    }
    emax = v[32] * abs(v[13]);
    emaxs = v[37] * abs(v[13]);
    if (v[7] <= emaxs && (v[2] > v[36] || v[5] == 0.)) {
	iv[29] = 11;
    }
    if (v[3] < 0.) {
	goto L250;
    }
    i__ = 0;
    if (v[6] > 0. && v[6] <= emax || v[6] == 0. && v[7] == 0.) {
	i__ = 2;
    }
    if (v[5] == 0. && v[17] <= v[33] && goodx) {
	++i__;
    }
    if (i__ > 0) {
	iv[29] = i__ + 6;
    }

/*  ***  CONSIDER RECOMPUTING STEP OF LENGTH V(LMAXS) FOR SINGULAR */
/*  ***  CONVERGENCE TEST. */

L250:
    if (iv[29] > 5 && iv[29] != 12) {
	goto L999;
    }
    if (v[5] == 0.) {
	goto L999;
    }
    if (v[2] > v[36]) {
	goto L260;
    }
    if (v[7] >= emaxs) {
	goto L999;
    }
    if (v[3] <= 0.) {
	goto L270;
    }
    if (v[3] * .5 <= v[36]) {
	goto L999;
    }
    goto L270;
L260:
    if (v[2] * .5 <= v[36]) {
	goto L999;
    }
    xmax = v[36] / v[2];
    if (xmax * (2. - xmax) * v[7] >= emaxs) {
	goto L999;
    }
L270:
    if (v[6] < 0.) {
	goto L290;
    }

/*  ***  RECOMPUTE V(PREDUC) FOR USE IN SINGULAR CONVERGENCE TEST  *** */

    v[14] = v[4];
    v[18] = v[2];
    if (iv[29] == 12) {
	v[18] = -v[18];
    }
    v[15] = v[7];
    i__ = iv[9];
    iv[9] = 2;
    if (i__ == 3) {
	iv[9] = 0;
    }
    iv[29] = 6;
    goto L999;

/*  ***  PERFORM SINGULAR CONVERGENCE TEST WITH RECOMPUTED V(PREDUC)  *** */

L280:
    v[4] = v[14];
    v[2] = abs(v[18]);
    iv[29] = iv[13];
    if (v[18] <= 0.) {
	iv[29] = 12;
    }
    v[6] = -v[7];
    v[7] = v[15];
    iv[9] = 3;
L290:
    if (-v[6] <= v[37] * abs(v[13])) {
	iv[29] = 11;
    }

L999:
    return 0;

/*  ***  LAST LINE OF DA7SST FOLLOWS  *** */
} /* da7sst_ */

/* Subroutine */ int i7shft_(integer *n, integer *k, integer *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, t, k1, ii, nm1;


/*  ***  SHIFT X(K),...,X(N) LEFT CIRCULARLY ONE POSITION IF K .GT. 0. */
/*  ***  SHIFT X(-K),...,X(N) RIGHT CIRCULARLY ONE POSITION IF K .LT. 0. */



    /* Parameter adjustments */
    --x;

    /* Function Body */
    if (*k < 0) {
	goto L20;
    }
    if (*k >= *n) {
	goto L999;
    }
    nm1 = *n - 1;
    t = x[*k];
    i__1 = nm1;
    for (i__ = *k; i__ <= i__1; ++i__) {
/* L10: */
	x[i__] = x[i__ + 1];
    }
    x[*n] = t;
    goto L999;

L20:
    k1 = -(*k);
    if (k1 >= *n) {
	goto L999;
    }
    t = x[*n];
    nm1 = *n - k1;
    i__1 = nm1;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = *n - ii;
	x[i__ + 1] = x[i__];
/* L30: */
    }
    x[k1] = t;
L999:
    return 0;
/*  ***  LAST LINE OF I7SHFT FOLLOWS  *** */
} /* i7shft_ */

/* Subroutine */ int s7etr_(integer *m, integer *n, integer *indrow, integer *
	jpntr, integer *indcol, integer *ipntr, integer *iwa)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer l, jp, ir, jpl, jpu, nnz, jcol;

/*     ********** */

/*     SUBROUTINE S7ETR */

/*     GIVEN A COLUMN-ORIENTED DEFINITION OF THE SPARSITY PATTERN */
/*     OF AN M BY N MATRIX A, THIS SUBROUTINE DETERMINES A */
/*     ROW-ORIENTED DEFINITION OF THE SPARSITY PATTERN OF A. */

/*     ON INPUT THE COLUMN-ORIENTED DEFINITION IS SPECIFIED BY */
/*     THE ARRAYS INDROW AND JPNTR. ON OUTPUT THE ROW-ORIENTED */
/*     DEFINITION IS SPECIFIED BY THE ARRAYS INDCOL AND IPNTR. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE S7ETR(M,N,INDROW,JPNTR,INDCOL,IPNTR,IWA) */

/*     WHERE */

/*       M IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF ROWS OF A. */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       INDROW IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE ROW */
/*         INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       JPNTR IS AN INTEGER INPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       INDCOL IS AN INTEGER OUTPUT ARRAY WHICH CONTAINS THE */
/*         COLUMN INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       IPNTR IS AN INTEGER OUTPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(1) IS SET TO 1 AND THAT IPNTR(M+1)-1 IS */
/*         THEN THE NUMBER OF NON-ZERO ELEMENTS OF THE MATRIX A. */

/*       IWA IS AN INTEGER WORK ARRAY OF LENGTH M. */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     DETERMINE THE NUMBER OF NON-ZEROES IN THE ROWS. */

    /* Parameter adjustments */
    --iwa;
    --indrow;
    --jpntr;
    --indcol;
    --ipntr;

    /* Function Body */
    i__1 = *m;
    for (ir = 1; ir <= i__1; ++ir) {
	iwa[ir] = 0;
/* L10: */
    }
    nnz = jpntr[*n + 1] - 1;
    i__1 = nnz;
    for (jp = 1; jp <= i__1; ++jp) {
	ir = indrow[jp];
	++iwa[ir];
/* L20: */
    }

/*     SET POINTERS TO THE START OF THE ROWS IN INDCOL. */

    ipntr[1] = 1;
    i__1 = *m;
    for (ir = 1; ir <= i__1; ++ir) {
	ipntr[ir + 1] = ipntr[ir] + iwa[ir];
	iwa[ir] = ipntr[ir];
/* L30: */
    }

/*     FILL INDCOL. */

    i__1 = *n;
    for (jcol = 1; jcol <= i__1; ++jcol) {
	jpl = jpntr[jcol];
	jpu = jpntr[jcol + 1] - 1;
	if (jpu < jpl) {
	    goto L50;
	}
	i__2 = jpu;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];
	    l = iwa[ir];
	    indcol[l] = jcol;
	    ++iwa[ir];
/* L40: */
	}
L50:
/* L60: */
	;
    }
    return 0;

/*     LAST CARD OF SUBROUTINE S7ETR. */

} /* s7etr_ */

/* Subroutine */ int dg7qsb_(doublereal *b, doublereal *d__, doublereal *
	dihdi, doublereal *g, integer *ipiv, integer *ipiv1, integer *ipiv2, 
	integer *ka, doublereal *l, integer *lv, integer *p, integer *p0, 
	integer *pc, doublereal *step, doublereal *td, doublereal *tg, 
	doublereal *v, doublereal *w, doublereal *x, doublereal *x0)
{
    /* Initialized data */

    static doublereal zero = 0.;

    /* System generated locals */
    integer step_dim1, step_offset;

    /* Local variables */
    static integer k, p1, kb, p10, ns;
    static doublereal ds0, rad, nred, pred;
    static integer kinit;
    extern /* Subroutine */ int ds7bqn_(doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, 
	    doublereal *, integer *, integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , ds7ipr_(integer *, integer *, doublereal *), dg7qts_(doublereal 
	    *, doublereal *, doublereal *, integer *, doublereal *, integer *,
	     doublereal *, doublereal *, doublereal *), dv7ipr_(integer *, 
	    integer *, doublereal *), dv7cpy_(integer *, doublereal *, 
	    doublereal *), dv7vmp_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);


/*  ***  COMPUTE HEURISTIC BOUNDED NEWTON STEP  *** */

/*     DIMENSION DIHDI(P*(P+1)/2), L(P*(P+1)/2) */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  V SUBSCRIPTS  *** */


    /* Parameter adjustments */
    --dihdi;
    --l;
    --v;
    --x0;
    --x;
    --w;
    --tg;
    --td;
    step_dim1 = *p;
    step_offset = 1 + step_dim1;
    step -= step_offset;
    --ipiv2;
    --ipiv1;
    --ipiv;
    --g;
    --d__;
    b -= 3;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    p1 = *pc;
    if (*ka < 0) {
	goto L10;
    }
    nred = v[6];
    ds0 = v[3];
    goto L20;
L10:
    *p0 = 0;
    *ka = -1;

L20:
    kinit = -1;
    if (*p0 == p1) {
	kinit = *ka;
    }
    dv7cpy_(p, &x[1], &x0[1]);
    pred = zero;
    rad = v[8];
    kb = -1;
    v[2] = zero;
    if (p1 > 0) {
	goto L30;
    }
    nred = zero;
    ds0 = zero;
    dv7scp_(p, &step[step_offset], &zero);
    goto L60;

L30:
    dv7cpy_(p, &td[1], &d__[1]);
    dv7ipr_(p, &ipiv[1], &td[1]);
    dv7vmp_(p, &tg[1], &g[1], &d__[1], &c_n1);
    dv7ipr_(p, &ipiv[1], &tg[1]);
L40:
    k = kinit;
    kinit = -1;
    v[8] = rad - v[2];
    dg7qts_(&td[1], &tg[1], &dihdi[1], &k, &l[1], &p1, &step[step_offset], &v[
	    1], &w[1]);
    *p0 = p1;
    if (*ka >= 0) {
	goto L50;
    }
    nred = v[6];
    ds0 = v[3];

L50:
    *ka = k;
    v[8] = rad;
    p10 = p1;
    ds7bqn_(&b[3], &d__[1], &step[(step_dim1 << 1) + 1], &ipiv[1], &ipiv1[1], 
	    &ipiv2[1], &kb, &l[1], lv, &ns, p, &p1, &step[step_offset], &td[1]
	    , &tg[1], &v[1], &w[1], &x[1], &x0[1]);
    if (ns > 0) {
	ds7ipr_(&p10, &ipiv1[1], &dihdi[1]);
    }
    pred += v[7];
    if (ns != 0) {
	*p0 = 0;
    }
    if (kb <= 0) {
	goto L40;
    }

L60:
    v[3] = ds0;
    v[6] = nred;
    v[7] = pred;
    v[4] = dd7tpr_(p, &g[1], &step[step_offset]);

    return 0;
/*  ***  LAST LINE OF DG7QSB FOLLOWS  *** */
} /* dg7qsb_ */

doublereal dl7svx_(integer *p, doublereal *l, doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val, d__1;

    /* Local variables */
    static doublereal b;
    static integer i__, j;
    static doublereal t;
    static integer j0, ji, jj, ix;
    static doublereal yi;
    static integer jm1, pm1, jjj;
    static doublereal blji, splus;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *);
    static integer pplus1;
    static doublereal sminus;


/*  ***  ESTIMATE LARGEST SING. VALUE OF PACKED LOWER TRIANG. MATRIX L */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION L(P*(P+1)/2) */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  PURPOSE  *** */

/*     THIS FUNCTION RETURNS A GOOD UNDER-ESTIMATE OF THE LARGEST */
/*     SINGULAR VALUE OF THE PACKED LOWER TRIANGULAR MATRIX L. */

/*  ***  PARAMETER DESCRIPTION  *** */

/*  P (IN)  = THE ORDER OF L.  L IS A  P X P  LOWER TRIANGULAR MATRIX. */
/*  L (IN)  = ARRAY HOLDING THE ELEMENTS OF  L  IN ROW ORDER, I.E. */
/*             L(1,1), L(2,1), L(2,2), L(3,1), L(3,2), L(3,3), ETC. */
/*  X (OUT) IF DL7SVX RETURNS A POSITIVE VALUE, THEN X = (L**T)*Y IS AN */
/*             (UNNORMALIZED) APPROXIMATE RIGHT SINGULAR VECTOR */
/*             CORRESPONDING TO THE LARGEST SINGULAR VALUE.  THIS */
/*             APPROXIMATION MAY BE CRUDE. */
/*  Y (OUT) IF DL7SVX RETURNS A POSITIVE VALUE, THEN Y = L*X IS A */
/*             NORMALIZED APPROXIMATE LEFT SINGULAR VECTOR CORRESPOND- */
/*             ING TO THE LARGEST SINGULAR VALUE.  THIS APPROXIMATION */
/*             MAY BE VERY CRUDE.  THE CALLER MAY PASS THE SAME VECTOR */
/*             FOR X AND Y (NONSTANDARD FORTRAN USAGE), IN WHICH CASE X */
/*             OVER-WRITES Y. */

/*  ***  ALGORITHM NOTES  *** */

/*     THE ALGORITHM IS BASED ON ANALOGY WITH (1).  IT USES A */
/*     RANDOM NUMBER GENERATOR PROPOSED IN (4), WHICH PASSES THE */
/*     SPECTRAL TEST WITH FLYING COLORS -- SEE (2) AND (3). */

/*  ***  SUBROUTINES AND FUNCTIONS CALLED  *** */

/*        DV2NRM - FUNCTION, RETURNS THE 2-NORM OF A VECTOR. */

/*  ***  REFERENCES  *** */

/*     (1) CLINE, A., MOLER, C., STEWART, G., AND WILKINSON, J.H.(1977), */
/*         AN ESTIMATE FOR THE CONDITION NUMBER OF A MATRIX, REPORT */
/*         TM-310, APPLIED MATH. DIV., ARGONNE NATIONAL LABORATORY. */

/*     (2) HOAGLIN, D.C. (1976), THEORETICAL PROPERTIES OF CONGRUENTIAL */
/*         RANDOM-NUMBER GENERATORS --  AN EMPIRICAL VIEW, */
/*         MEMORANDUM NS-340, DEPT. OF STATISTICS, HARVARD UNIV. */

/*     (3) KNUTH, D.E. (1969), THE ART OF COMPUTER PROGRAMMING, VOL. 2 */
/*         (SEMINUMERICAL ALGORITHMS), ADDISON-WESLEY, READING, MASS. */

/*     (4) SMITH, C.S. (1971), MULTIPLICATIVE PSEUDO-RANDOM NUMBER */
/*         GENERATORS WITH PRIME MODULUS, J. ASSOC. COMPUT. MACH. 18, */
/*         PP. 586-593. */

/*  ***  HISTORY  *** */

/*     DESIGNED AND CODED BY DAVID M. GAY (WINTER 1977/SUMMER 1978). */

/*  ***  GENERAL  *** */

/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, AND MCS76-11989. */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  CONSTANTS  *** */


/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */



/*  ***  BODY  *** */

    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    ix = 2;
    pplus1 = *p + 1;
    pm1 = *p - 1;

/*  ***  FIRST INITIALIZE X TO PARTIAL SUMS  *** */

    j0 = *p * pm1 / 2;
    jj = j0 + *p;
    ix = ix * 3432 % 9973;
    b = ((doublereal) ix / 9973. + 1.) * .5;
    x[*p] = b * l[jj];
    if (*p <= 1) {
	goto L40;
    }
    i__1 = pm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ji = j0 + i__;
	x[i__] = b * l[ji];
/* L10: */
    }

/*  ***  COMPUTE X = (L**T)*B, WHERE THE COMPONENTS OF B HAVE RANDOMLY */
/*  ***  CHOSEN MAGNITUDES IN (.5,1) WITH SIGNS CHOSEN TO MAKE X LARGE. */

/*     DO J = P-1 TO 1 BY -1... */
    i__1 = pm1;
    for (jjj = 1; jjj <= i__1; ++jjj) {
	j = *p - jjj;
/*       ***  DETERMINE X(J) IN THIS ITERATION. NOTE FOR I = 1,2,...,J */
/*       ***  THAT X(I) HOLDS THE CURRENT PARTIAL SUM FOR ROW I. */
	ix = ix * 3432 % 9973;
	b = ((doublereal) ix / 9973. + 1.) * .5;
	jm1 = j - 1;
	j0 = j * jm1 / 2;
	splus = 0.;
	sminus = 0.;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ji = j0 + i__;
	    blji = b * l[ji];
	    splus += (d__1 = blji + x[i__], abs(d__1));
	    sminus += (d__1 = blji - x[i__], abs(d__1));
/* L20: */
	}
	if (sminus > splus) {
	    b = -b;
	}
	x[j] = 0.;
/*        ***  UPDATE PARTIAL SUMS  *** */
	dv2axy_(&j, &x[1], &b, &l[j0 + 1], &x[1]);
/* L30: */
    }

/*  ***  NORMALIZE X  *** */

L40:
    t = dv2nrm_(p, &x[1]);
    if (t <= 0.) {
	goto L80;
    }
    t = 1. / t;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	x[i__] = t * x[i__];
    }

/*  ***  COMPUTE L*X = Y AND RETURN SVMAX = TWONORM(Y)  *** */

    i__1 = *p;
    for (jjj = 1; jjj <= i__1; ++jjj) {
	j = pplus1 - jjj;
	ji = j * (j - 1) / 2 + 1;
	y[j] = dd7tpr_(&j, &l[ji], &x[1]);
/* L60: */
    }

/*  ***  NORMALIZE Y AND SET X = (L**T)*Y  *** */

    t = 1. / dv2nrm_(p, &y[1]);
    ji = 1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	yi = t * y[i__];
	x[i__] = 0.;
	dv2axy_(&i__, &x[1], &yi, &l[ji], &x[1]);
	ji += i__;
/* L70: */
    }
    ret_val = dv2nrm_(p, &x[1]);
    goto L999;

L80:
    ret_val = 0.;

L999:
    return ret_val;
/*  ***  LAST CARD OF DL7SVX FOLLOWS  *** */
} /* dl7svx_ */

/* Subroutine */ int dd7dup_(doublereal *d__, doublereal *hdiag, integer *iv, 
	integer *liv, integer *lv, integer *n, doublereal *v)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal t;
    static integer d0i;
    static doublereal vdfac;
    static integer dtoli;


/*  ***  UPDATE SCALE VECTOR D FOR  DMNH  *** */

/*  ***  PARAMETER DECLARATIONS  *** */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/* -------------------------------  BODY  -------------------------------- */

    /* Parameter adjustments */
    --iv;
    --v;
    --hdiag;
    --d__;

    /* Function Body */
    i__ = iv[16];
    if (i__ == 1) {
	goto L10;
    }
    if (iv[31] > 0) {
	goto L999;
    }

L10:
    dtoli = iv[59];
    d0i = dtoli + *n;
    vdfac = v[41];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	d__2 = sqrt((d__1 = hdiag[i__], abs(d__1))), d__3 = vdfac * d__[i__];
	t = max(d__2,d__3);
	if (t < v[dtoli]) {
/* Computing MAX */
	    d__1 = v[dtoli], d__2 = v[d0i];
	    t = max(d__1,d__2);
	}
	d__[i__] = t;
	++dtoli;
	++d0i;
/* L20: */
    }

L999:
    return 0;
/*  ***  LAST CARD OF DD7DUP FOLLOWS  *** */
} /* dd7dup_ */

/* Subroutine */ int s7rtdt_(integer *n, integer *nnz, integer *indrow, 
	integer *indcol, integer *jpntr, integer *iwa)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, l;

/*     ********** */

/*     SUBROUTINE S7RTDT */

/*     GIVEN THE NON-ZERO ELEMENTS OF AN M BY N MATRIX A IN */
/*     ARBITRARY ORDER AS SPECIFIED BY THEIR ROW AND COLUMN */
/*     INDICES, THIS SUBROUTINE PERMUTES THESE ELEMENTS SO */
/*     THAT THEIR COLUMN INDICES ARE IN NON-DECREASING ORDER. */

/*     ON INPUT IT IS ASSUMED THAT THE ELEMENTS ARE SPECIFIED IN */

/*           INDROW(K),INDCOL(K), K = 1,...,NNZ. */

/*     ON OUTPUT THE ELEMENTS ARE PERMUTED SO THAT INDCOL IS */
/*     IN NON-DECREASING ORDER. IN ADDITION, THE ARRAY JPNTR */
/*     IS SET SO THAT THE ROW INDICES FOR COLUMN J ARE */

/*           INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*     NOTE THAT THE VALUE OF M IS NOT NEEDED BY S7RTDT AND IS */
/*     THEREFORE NOT PRESENT IN THE SUBROUTINE STATEMENT. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE S7RTDT(N,NNZ,INDROW,INDCOL,JPNTR,IWA) */

/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       NNZ IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF NON-ZERO ELEMENTS OF A. */

/*       INDROW IS AN INTEGER ARRAY OF LENGTH NNZ. ON INPUT INDROW */
/*         MUST CONTAIN THE ROW INDICES OF THE NON-ZERO ELEMENTS OF A. */
/*         ON OUTPUT INDROW IS PERMUTED SO THAT THE CORRESPONDING */
/*         COLUMN INDICES OF INDCOL ARE IN NON-DECREASING ORDER. */

/*       INDCOL IS AN INTEGER ARRAY OF LENGTH NNZ. ON INPUT INDCOL */
/*         MUST CONTAIN THE COLUMN INDICES OF THE NON-ZERO ELEMENTS */
/*         OF A. ON OUTPUT INDCOL IS PERMUTED SO THAT THESE INDICES */
/*         ARE IN NON-DECREASING ORDER. */

/*       JPNTR IS AN INTEGER OUTPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN THE OUTPUT */
/*         INDROW. THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(1) IS SET TO 1 AND THAT JPNTR(N+1)-1 */
/*         IS THEN NNZ. */

/*       IWA IS AN INTEGER WORK ARRAY OF LENGTH N. */

/*     SUBPROGRAMS CALLED */

/*       FORTRAN-SUPPLIED ... MAX0 */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     DETERMINE THE NUMBER OF NON-ZEROES IN THE COLUMNS. */

    /* Parameter adjustments */
    --iwa;
    --indcol;
    --indrow;
    --jpntr;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	iwa[j] = 0;
/* L10: */
    }
    i__1 = *nnz;
    for (k = 1; k <= i__1; ++k) {
	j = indcol[k];
	++iwa[j];
/* L20: */
    }

/*     SET POINTERS TO THE START OF THE COLUMNS IN INDROW. */

    jpntr[1] = 1;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	jpntr[j + 1] = jpntr[j] + iwa[j];
	iwa[j] = jpntr[j];
/* L30: */
    }
    k = 1;

/*     BEGIN IN-PLACE SORT. */

L40:
    j = indcol[k];
    if (k < jpntr[j] || k >= jpntr[j + 1]) {
	goto L50;
    }

/*           CURRENT ELEMENT IS IN POSITION. NOW EXAMINE THE */
/*           NEXT ELEMENT OR THE FIRST UN-SORTED ELEMENT IN */
/*           THE J-TH GROUP. */

/* Computing MAX */
    i__1 = k + 1, i__2 = iwa[j];
    k = max(i__1,i__2);
    goto L60;
L50:

/*           CURRENT ELEMENT IS NOT IN POSITION. PLACE ELEMENT */
/*           IN POSITION AND MAKE THE DISPLACED ELEMENT THE */
/*           CURRENT ELEMENT. */

    l = iwa[j];
    ++iwa[j];
    i__ = indrow[k];
    indrow[k] = indrow[l];
    indcol[k] = indcol[l];
    indrow[l] = i__;
    indcol[l] = j;
L60:
    if (k <= *nnz) {
	goto L40;
    }
    return 0;

/*     LAST CARD OF SUBROUTINE S7RTDT. */

} /* s7rtdt_ */

/* Subroutine */ int dl7srt_(integer *n1, integer *n, doublereal *l, 
	doublereal *a, integer *irc)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    static integer i0, j0, ij, ik, jk;
    static doublereal td;
    static integer im1, jm1;


/*  ***  COMPUTE ROWS N1 THROUGH N OF THE CHOLESKY FACTOR  L  OF */
/*  ***  A = L*(L**T),  WHERE  L  AND THE LOWER TRIANGLE OF  A  ARE BOTH */
/*  ***  STORED COMPACTLY BY ROWS (AND MAY OCCUPY THE SAME STORAGE). */
/*  ***  IRC = 0 MEANS ALL WENT WELL.  IRC = J MEANS THE LEADING */
/*  ***  PRINCIPAL  J X J  SUBMATRIX OF  A  IS NOT POSITIVE DEFINITE -- */
/*  ***  AND  L(J*(J+1)/2)  CONTAINS THE (NONPOS.) REDUCED J-TH DIAGONAL. */

/*  ***  PARAMETERS  *** */

/*     DIMENSION L(N*(N+1)/2), A(N*(N+1)/2) */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */

/*  ***  BODY  *** */

    /* Parameter adjustments */
    --a;
    --l;

    /* Function Body */
    i0 = *n1 * (*n1 - 1) / 2;
    i__1 = *n;
    for (i__ = *n1; i__ <= i__1; ++i__) {
	td = 0.;
	if (i__ == 1) {
	    goto L40;
	}
	j0 = 0;
	im1 = i__ - 1;
	i__2 = im1;
	for (j = 1; j <= i__2; ++j) {
	    t = 0.;
	    if (j == 1) {
		goto L20;
	    }
	    jm1 = j - 1;
	    i__3 = jm1;
	    for (k = 1; k <= i__3; ++k) {
		ik = i0 + k;
		jk = j0 + k;
		t += l[ik] * l[jk];
/* L10: */
	    }
L20:
	    ij = i0 + j;
	    j0 += j;
	    t = (a[ij] - t) / l[j0];
	    l[ij] = t;
	    td += t * t;
/* L30: */
	}
L40:
	i0 += i__;
	t = a[i0] - td;
	if (t <= 0.) {
	    goto L60;
	}
	l[i0] = sqrt(t);
/* L50: */
    }

    *irc = 0;
    goto L999;

L60:
    l[i0] = t;
    *irc = i__;

L999:
    return 0;

/*  ***  LAST CARD OF DL7SRT  *** */
} /* dl7srt_ */

doublereal dl7svn_(integer *p, doublereal *l, doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal ret_val, d__1;

    /* Local variables */
    static doublereal b;
    static integer i__, j;
    static doublereal t;
    static integer j0, ii, ji, jj, ix, jm1, pm1, jjj;
    static doublereal splus, xplus;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *);
    static doublereal sminus, xminus;


/*  ***  ESTIMATE SMALLEST SING. VALUE OF PACKED LOWER TRIANG. MATRIX L */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION L(P*(P+1)/2) */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  PURPOSE  *** */

/*     THIS FUNCTION RETURNS A GOOD OVER-ESTIMATE OF THE SMALLEST */
/*     SINGULAR VALUE OF THE PACKED LOWER TRIANGULAR MATRIX L. */

/*  ***  PARAMETER DESCRIPTION  *** */

/*  P (IN)  = THE ORDER OF L.  L IS A  P X P  LOWER TRIANGULAR MATRIX. */
/*  L (IN)  = ARRAY HOLDING THE ELEMENTS OF  L  IN ROW ORDER, I.E. */
/*             L(1,1), L(2,1), L(2,2), L(3,1), L(3,2), L(3,3), ETC. */
/*  X (OUT) IF DL7SVN RETURNS A POSITIVE VALUE, THEN X IS A NORMALIZED */
/*             APPROXIMATE LEFT SINGULAR VECTOR CORRESPONDING TO THE */
/*             SMALLEST SINGULAR VALUE.  THIS APPROXIMATION MAY BE VERY */
/*             CRUDE.  IF DL7SVN RETURNS ZERO, THEN SOME COMPONENTS OF X */
/*             ARE ZERO AND THE REST RETAIN THEIR INPUT VALUES. */
/*  Y (OUT) IF DL7SVN RETURNS A POSITIVE VALUE, THEN Y = (L**-1)*X IS AN */
/*             UNNORMALIZED APPROXIMATE RIGHT SINGULAR VECTOR CORRESPOND- */
/*             ING TO THE SMALLEST SINGULAR VALUE.  THIS APPROXIMATION */
/*             MAY BE CRUDE.  IF DL7SVN RETURNS ZERO, THEN Y RETAINS ITS */
/*             INPUT VALUE.  THE CALLER MAY PASS THE SAME VECTOR FOR X */
/*             AND Y (NONSTANDARD FORTRAN USAGE), IN WHICH CASE Y OVER- */
/*             WRITES X (FOR NONZERO DL7SVN RETURNS). */

/*  ***  ALGORITHM NOTES  *** */

/*     THE ALGORITHM IS BASED ON (1), WITH THE ADDITIONAL PROVISION THAT */
/*     DL7SVN = 0 IS RETURNED IF THE SMALLEST DIAGONAL ELEMENT OF L */
/*     (IN MAGNITUDE) IS NOT MORE THAN THE UNIT ROUNDOFF TIMES THE */
/*     LARGEST.  THE ALGORITHM USES A RANDOM NUMBER GENERATOR PROPOSED */
/*     IN (4), WHICH PASSES THE SPECTRAL TEST WITH FLYING COLORS -- SEE */
/*     (2) AND (3). */

/*  ***  SUBROUTINES AND FUNCTIONS CALLED  *** */

/*        DV2NRM - FUNCTION, RETURNS THE 2-NORM OF A VECTOR. */

/*  ***  REFERENCES  *** */

/*     (1) CLINE, A., MOLER, C., STEWART, G., AND WILKINSON, J.H.(1977), */
/*         AN ESTIMATE FOR THE CONDITION NUMBER OF A MATRIX, REPORT */
/*         TM-310, APPLIED MATH. DIV., ARGONNE NATIONAL LABORATORY. */

/*     (2) HOAGLIN, D.C. (1976), THEORETICAL PROPERTIES OF CONGRUENTIAL */
/*         RANDOM-NUMBER GENERATORS --  AN EMPIRICAL VIEW, */
/*         MEMORANDUM NS-340, DEPT. OF STATISTICS, HARVARD UNIV. */

/*     (3) KNUTH, D.E. (1969), THE ART OF COMPUTER PROGRAMMING, VOL. 2 */
/*         (SEMINUMERICAL ALGORITHMS), ADDISON-WESLEY, READING, MASS. */

/*     (4) SMITH, C.S. (1971), MULTIPLICATIVE PSEUDO-RANDOM NUMBER */
/*         GENERATORS WITH PRIME MODULUS, J. ASSOC. COMPUT. MACH. 18, */
/*         PP. 586-593. */

/*  ***  HISTORY  *** */

/*     DESIGNED AND CODED BY DAVID M. GAY (WINTER 1977/SUMMER 1978). */

/*  ***  GENERAL  *** */

/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, AND MCS76-11989. */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  CONSTANTS  *** */


/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */



/*  ***  BODY  *** */

    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    ix = 2;
    pm1 = *p - 1;

/*  ***  FIRST CHECK WHETHER TO RETURN DL7SVN = 0 AND INITIALIZE X  *** */

    ii = 0;
    j0 = *p * pm1 / 2;
    jj = j0 + *p;
    if (l[jj] == 0.) {
	goto L110;
    }
    ix = ix * 3432 % 9973;
    b = ((doublereal) ix / 9973. + 1.) * .5;
    xplus = b / l[jj];
    x[*p] = xplus;
    if (*p <= 1) {
	goto L60;
    }
    i__1 = pm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ii += i__;
	if (l[ii] == 0.) {
	    goto L110;
	}
	ji = j0 + i__;
	x[i__] = xplus * l[ji];
/* L10: */
    }

/*  ***  SOLVE (L**T)*X = B, WHERE THE COMPONENTS OF B HAVE RANDOMLY */
/*  ***  CHOSEN MAGNITUDES IN (.5,1) WITH SIGNS CHOSEN TO MAKE X LARGE. */

/*     DO J = P-1 TO 1 BY -1... */
    i__1 = pm1;
    for (jjj = 1; jjj <= i__1; ++jjj) {
	j = *p - jjj;
/*       ***  DETERMINE X(J) IN THIS ITERATION. NOTE FOR I = 1,2,...,J */
/*       ***  THAT X(I) HOLDS THE CURRENT PARTIAL SUM FOR ROW I. */
	ix = ix * 3432 % 9973;
	b = ((doublereal) ix / 9973. + 1.) * .5;
	xplus = b - x[j];
	xminus = -b - x[j];
	splus = abs(xplus);
	sminus = abs(xminus);
	jm1 = j - 1;
	j0 = j * jm1 / 2;
	jj = j0 + j;
	xplus /= l[jj];
	xminus /= l[jj];
	if (jm1 == 0) {
	    goto L30;
	}
	i__2 = jm1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ji = j0 + i__;
	    splus += (d__1 = x[i__] + l[ji] * xplus, abs(d__1));
	    sminus += (d__1 = x[i__] + l[ji] * xminus, abs(d__1));
/* L20: */
	}
L30:
	if (sminus > splus) {
	    xplus = xminus;
	}
	x[j] = xplus;
/*       ***  UPDATE PARTIAL SUMS  *** */
	if (jm1 > 0) {
	    dv2axy_(&jm1, &x[1], &xplus, &l[j0 + 1], &x[1]);
	}
/* L50: */
    }

/*  ***  NORMALIZE X  *** */

L60:
    t = 1. / dv2nrm_(p, &x[1]);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L70: */
	x[i__] = t * x[i__];
    }

/*  ***  SOLVE L*Y = X AND RETURN DL7SVN = 1/TWONORM(Y)  *** */

    i__1 = *p;
    for (j = 1; j <= i__1; ++j) {
	jm1 = j - 1;
	j0 = j * jm1 / 2;
	jj = j0 + j;
	t = 0.;
	if (jm1 > 0) {
	    t = dd7tpr_(&jm1, &l[j0 + 1], &y[1]);
	}
	y[j] = (x[j] - t) / l[jj];
/* L100: */
    }

    ret_val = 1. / dv2nrm_(p, &y[1]);
    goto L999;

L110:
    ret_val = 0.;
L999:
    return ret_val;
/*  ***  LAST CARD OF DL7SVN FOLLOWS  *** */
} /* dl7svn_ */

/* Subroutine */ int ds7lvm_(integer *p, doublereal *y, doublereal *s, 
	doublereal *x)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k;
    static doublereal xi;
    static integer im1;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);


/*  ***  SET  Y = S * X,  S = P X P SYMMETRIC MATRIX.  *** */
/*  ***  LOWER TRIANGLE OF  S  STORED ROWWISE.         *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION S(P*(P+1)/2) */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  NO INTRINSIC FUNCTIONS  *** */

/*  ***  EXTERNAL FUNCTION  *** */


/* ----------------------------------------------------------------------- */

    /* Parameter adjustments */
    --x;
    --y;
    --s;

    /* Function Body */
    j = 1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	y[i__] = dd7tpr_(&i__, &s[j], &x[1]);
	j += i__;
/* L10: */
    }

    if (*p <= 1) {
	goto L999;
    }
    j = 1;
    i__1 = *p;
    for (i__ = 2; i__ <= i__1; ++i__) {
	xi = x[i__];
	im1 = i__ - 1;
	++j;
	i__2 = im1;
	for (k = 1; k <= i__2; ++k) {
	    y[k] += s[j] * xi;
	    ++j;
/* L30: */
	}
/* L40: */
    }

L999:
    return 0;
/*  ***  LAST CARD OF DS7LVM FOLLOWS  *** */
} /* ds7lvm_ */

doublereal dh2rfg_(doublereal *a, doublereal *b, doublereal *x, doublereal *y,
	 doublereal *z__)
{
    /* Initialized data */

    static doublereal zero = 0.;

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal c__, t, a1, b1;


/*  ***  DETERMINE X, Y, Z SO  I + (1,Z)**T * (X,Y)  IS A 2X2 */
/*  ***  HOUSEHOLDER REFLECTION SENDING (A,B)**T INTO (C,0)**T, */
/*  ***  WHERE  C = -SIGN(A)*SQRT(A**2 + B**2)  IS THE VALUE DH2RFG */
/*  ***  RETURNS. */


/* /+ */
/* / */

/*  ***  BODY  *** */

    if (*b != zero) {
	goto L10;
    }
    *x = zero;
    *y = zero;
    *z__ = zero;
    ret_val = *a;
    goto L999;
L10:
    t = abs(*a) + abs(*b);
    a1 = *a / t;
    b1 = *b / t;
/* Computing 2nd power */
    d__1 = a1;
/* Computing 2nd power */
    d__2 = b1;
    c__ = sqrt(d__1 * d__1 + d__2 * d__2);
    if (a1 > zero) {
	c__ = -c__;
    }
    a1 -= c__;
    *z__ = b1 / a1;
    *x = a1 / c__;
    *y = b1 / c__;
    ret_val = t * c__;
L999:
    return ret_val;
/*  ***  LAST LINE OF DH2RFG FOLLOWS  *** */
} /* dh2rfg_ */

/* Subroutine */ int dl7nvr_(integer *n, doublereal *lin, doublereal *l)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, k;
    static doublereal t;
    static integer j0, j1, k0, ii, jj, im1, np1;


/*  ***  COMPUTE  LIN = L**-1,  BOTH  N X N  LOWER TRIANG. STORED   *** */
/*  ***  COMPACTLY BY ROWS.  LIN AND L MAY SHARE THE SAME STORAGE.  *** */

/*  ***  PARAMETERS  *** */

/*     DIMENSION L(N*(N+1)/2), LIN(N*(N+1)/2) */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  BODY  *** */

    /* Parameter adjustments */
    --l;
    --lin;

    /* Function Body */
    np1 = *n + 1;
    j0 = *n * np1 / 2;
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = np1 - ii;
	lin[j0] = 1. / l[j0];
	if (i__ <= 1) {
	    goto L999;
	}
	j1 = j0;
	im1 = i__ - 1;
	i__2 = im1;
	for (jj = 1; jj <= i__2; ++jj) {
	    t = 0.;
	    j0 = j1;
	    k0 = j1 - jj;
	    i__3 = jj;
	    for (k = 1; k <= i__3; ++k) {
		t -= l[k0] * lin[j0];
		--j0;
		k0 = k0 + k - i__;
/* L10: */
	    }
	    lin[j0] = t / l[k0];
/* L20: */
	}
	--j0;
/* L30: */
    }
L999:
    return 0;
/*  ***  LAST CARD OF DL7NVR FOLLOWS  *** */
} /* dl7nvr_ */

/* Subroutine */ int dd7dog_(doublereal *dig, integer *lv, integer *n, 
	doublereal *nwtstp, doublereal *step, doublereal *v)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal t, t1, t2, cfact, relax, cnorm, gnorm, rlambd, ghinvg, 
	    femnsq, ctrnwt, nwtnrm;


/*  ***  COMPUTE DOUBLE DOGLEG STEP  *** */

/*  ***  PARAMETER DECLARATIONS  *** */


/*  ***  PURPOSE  *** */

/*        THIS SUBROUTINE COMPUTES A CANDIDATE STEP (FOR USE IN AN UNCON- */
/*     STRAINED MINIMIZATION CODE) BY THE DOUBLE DOGLEG ALGORITHM OF */
/*     DENNIS AND MEI (REF. 1), WHICH IS A VARIATION ON POWELL*S DOGLEG */
/*     SCHEME (REF. 2, P. 95). */

/* --------------------------  PARAMETER USAGE  -------------------------- */

/*    DIG (INPUT) DIAG(D)**-2 * G -- SEE ALGORITHM NOTES. */
/*      G (INPUT) THE CURRENT GRADIENT VECTOR. */
/*     LV (INPUT) LENGTH OF V. */
/*      N (INPUT) NUMBER OF COMPONENTS IN  DIG, G, NWTSTP,  AND  STEP. */
/* NWTSTP (INPUT) NEGATIVE NEWTON STEP -- SEE ALGORITHM NOTES. */
/*   STEP (OUTPUT) THE COMPUTED STEP. */
/*      V (I/O) VALUES ARRAY, THE FOLLOWING COMPONENTS OF WHICH ARE */
/*             USED HERE... */
/* V(BIAS)   (INPUT) BIAS FOR RELAXED NEWTON STEP, WHICH IS V(BIAS) OF */
/*             THE WAY FROM THE FULL NEWTON TO THE FULLY RELAXED NEWTON */
/*             STEP.  RECOMMENDED VALUE = 0.8 . */
/* V(DGNORM) (INPUT) 2-NORM OF DIAG(D)**-1 * G -- SEE ALGORITHM NOTES. */
/* V(DSTNRM) (OUTPUT) 2-NORM OF DIAG(D) * STEP, WHICH IS V(RADIUS) */
/*             UNLESS V(STPPAR) = 0 -- SEE ALGORITHM NOTES. */
/* V(DST0) (INPUT) 2-NORM OF DIAG(D) * NWTSTP -- SEE ALGORITHM NOTES. */
/* V(GRDFAC) (OUTPUT) THE COEFFICIENT OF  DIG  IN THE STEP RETURNED -- */
/*             STEP(I) = V(GRDFAC)*DIG(I) + V(NWTFAC)*NWTSTP(I). */
/* V(GTHG)   (INPUT) SQUARE-ROOT OF (DIG**T) * (HESSIAN) * DIG -- SEE */
/*             ALGORITHM NOTES. */
/* V(GTSTEP) (OUTPUT) INNER PRODUCT BETWEEN G AND STEP. */
/* V(NREDUC) (OUTPUT) FUNCTION REDUCTION PREDICTED FOR THE FULL NEWTON */
/*             STEP. */
/* V(NWTFAC) (OUTPUT) THE COEFFICIENT OF  NWTSTP  IN THE STEP RETURNED -- */
/*             SEE V(GRDFAC) ABOVE. */
/* V(PREDUC) (OUTPUT) FUNCTION REDUCTION PREDICTED FOR THE STEP RETURNED. */
/* V(RADIUS) (INPUT) THE TRUST REGION RADIUS.  D TIMES THE STEP RETURNED */
/*             HAS 2-NORM V(RADIUS) UNLESS V(STPPAR) = 0. */
/* V(STPPAR) (OUTPUT) CODE TELLING HOW STEP WAS COMPUTED... 0 MEANS A */
/*             FULL NEWTON STEP.  BETWEEN 0 AND 1 MEANS V(STPPAR) OF THE */
/*             WAY FROM THE NEWTON TO THE RELAXED NEWTON STEP.  BETWEEN */
/*             1 AND 2 MEANS A TRUE DOUBLE DOGLEG STEP, V(STPPAR) - 1 OF */
/*             THE WAY FROM THE RELAXED NEWTON TO THE CAUCHY STEP. */
/*             GREATER THAN 2 MEANS 1 / (V(STPPAR) - 1) TIMES THE CAUCHY */
/*             STEP. */

/* -------------------------------  NOTES  ------------------------------- */

/*  ***  ALGORITHM NOTES  *** */

/*        LET  G  AND  H  BE THE CURRENT GRADIENT AND HESSIAN APPROXIMA- */
/*     TION RESPECTIVELY AND LET D BE THE CURRENT SCALE VECTOR.  THIS */
/*     ROUTINE ASSUMES DIG = DIAG(D)**-2 * G  AND  NWTSTP = H**-1 * G. */
/*     THE STEP COMPUTED IS THE SAME ONE WOULD GET BY REPLACING G AND H */
/*     BY  DIAG(D)**-1 * G  AND  DIAG(D)**-1 * H * DIAG(D)**-1, */
/*     COMPUTING STEP, AND TRANSLATING STEP BACK TO THE ORIGINAL */
/*     VARIABLES, I.E., PREMULTIPLYING IT BY DIAG(D)**-1. */

/*  ***  REFERENCES  *** */

/* 1.  DENNIS, J.E., AND MEI, H.H.W. (1979), TWO NEW UNCONSTRAINED OPTI- */
/*             MIZATION ALGORITHMS WHICH USE FUNCTION AND GRADIENT */
/*             VALUES, J. OPTIM. THEORY APPLIC. 28, PP. 453-482. */
/* 2. POWELL, M.J.D. (1970), A HYBRID METHOD FOR NON-LINEAR EQUATIONS, */
/*             IN NUMERICAL METHODS FOR NON-LINEAR EQUATIONS, EDITED BY */
/*             P. RABINOWITZ, GORDON AND BREACH, LONDON. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
/*     MCS-7906671. */

/* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/* --------------------------  LOCAL VARIABLES  -------------------------- */


/*  ***  V SUBSCRIPTS  *** */


/*  ***  DATA INITIALIZATIONS  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --v;
    --step;
    --nwtstp;
    --dig;

    /* Function Body */
    nwtnrm = v[3];
    rlambd = 1.;
    if (nwtnrm > 0.) {
	rlambd = v[8] / nwtnrm;
    }
    gnorm = v[1];
    ghinvg = v[6] * 2.;
    v[45] = 0.;
    v[46] = 0.;
    if (rlambd < 1.) {
	goto L30;
    }

/*        ***  THE NEWTON STEP IS INSIDE THE TRUST REGION  *** */

    v[5] = 0.;
    v[2] = nwtnrm;
    v[4] = -ghinvg;
    v[7] = v[6];
    v[46] = -1.;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L20: */
	step[i__] = -nwtstp[i__];
    }
    goto L999;

L30:
    v[2] = v[8];
/* Computing 2nd power */
    d__1 = gnorm / v[44];
    cfact = d__1 * d__1;
/*     ***  CAUCHY STEP = -CFACT * G. */
    cnorm = gnorm * cfact;
    relax = 1. - v[43] * (1. - gnorm * cnorm / ghinvg);
    if (rlambd < relax) {
	goto L50;
    }

/*        ***  STEP IS BETWEEN RELAXED NEWTON AND FULL NEWTON STEPS  *** */

    v[5] = 1. - (rlambd - relax) / (1. - relax);
    t = -rlambd;
    v[4] = t * ghinvg;
    v[7] = rlambd * (1. - rlambd * .5) * ghinvg;
    v[46] = t;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L40: */
	step[i__] = t * nwtstp[i__];
    }
    goto L999;

L50:
    if (cnorm < v[8]) {
	goto L70;
    }

/*        ***  THE CAUCHY STEP LIES OUTSIDE THE TRUST REGION -- */
/*        ***  STEP = SCALED CAUCHY STEP  *** */

    t = -v[8] / gnorm;
    v[45] = t;
    v[5] = cnorm / v[8] + 1.;
    v[4] = -v[8] * gnorm;
/* Computing 2nd power */
    d__1 = v[44] / gnorm;
    v[7] = v[8] * (gnorm - v[8] * .5 * (d__1 * d__1));
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L60: */
	step[i__] = t * dig[i__];
    }
    goto L999;

/*     ***  COMPUTE DOGLEG STEP BETWEEN CAUCHY AND RELAXED NEWTON  *** */
/*     ***  FEMUR = RELAXED NEWTON STEP MINUS CAUCHY STEP  *** */

L70:
    ctrnwt = cfact * relax * ghinvg / gnorm;
/*     *** CTRNWT = INNER PROD. OF CAUCHY AND RELAXED NEWTON STEPS, */
/*     *** SCALED BY GNORM**-1. */
/* Computing 2nd power */
    d__1 = cfact;
    t1 = ctrnwt - gnorm * (d__1 * d__1);
/*     ***  T1 = INNER PROD. OF FEMUR AND CAUCHY STEP, SCALED BY */
/*     ***  GNORM**-1. */
/* Computing 2nd power */
    d__1 = cfact;
    t2 = v[8] * (v[8] / gnorm) - gnorm * (d__1 * d__1);
    t = relax * nwtnrm;
    femnsq = t / gnorm * t - ctrnwt - t1;
/*     ***  FEMNSQ = SQUARE OF 2-NORM OF FEMUR, SCALED BY GNORM**-1. */
/* Computing 2nd power */
    d__1 = t1;
    t = t2 / (t1 + sqrt(d__1 * d__1 + femnsq * t2));
/*     ***  DOGLEG STEP  =  CAUCHY STEP  +  T * FEMUR. */
    t1 = (t - 1.) * cfact;
    v[45] = t1;
    t2 = -t * relax;
    v[46] = t2;
    v[5] = 2. - t;
/* Computing 2nd power */
    d__1 = gnorm;
    v[4] = t1 * (d__1 * d__1) + t2 * ghinvg;
/* Computing 2nd power */
    d__1 = v[44] * t1;
    v[7] = -t1 * gnorm * ((t2 + 1.) * gnorm) - t2 * (t2 * .5 + 1.) * ghinvg - 
	    d__1 * d__1 * .5;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L80: */
	step[i__] = t1 * dig[i__] + t2 * nwtstp[i__];
    }

L999:
    return 0;
/*  ***  LAST LINE OF DD7DOG FOLLOWS  *** */
} /* dd7dog_ */

/* Subroutine */ int ds7ipr_(integer *p, integer *ip, doublereal *h__)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k, l, m;
    static doublereal t;
    static integer j1, k1, kk, jm, km, kmj;


/*  APPLY THE PERMUTATION DEFINED BY IP TO THE ROWS AND COLUMNS OF THE */
/*  P X P SYMMETRIC MATRIX WHOSE LOWER TRIANGLE IS STORED COMPACTLY IN H. */
/*  THUS H.OUTPUT(I,J) = H.INPUT(IP(I), IP(J)). */



/* ***  BODY  *** */

    /* Parameter adjustments */
    --ip;
    --h__;

    /* Function Body */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = ip[i__];
	if (j == i__) {
	    goto L90;
	}
	ip[i__] = abs(j);
	if (j < 0) {
	    goto L90;
	}
	k = i__;
L10:
	j1 = j;
	k1 = k;
	if (j <= k) {
	    goto L20;
	}
	j1 = k;
	k1 = j;
L20:
	kmj = k1 - j1;
	l = j1 - 1;
	jm = j1 * l / 2;
	km = k1 * (k1 - 1) / 2;
	if (l <= 0) {
	    goto L40;
	}
	i__2 = l;
	for (m = 1; m <= i__2; ++m) {
	    ++jm;
	    t = h__[jm];
	    ++km;
	    h__[jm] = h__[km];
	    h__[km] = t;
/* L30: */
	}
L40:
	++km;
	kk = km + kmj;
	++jm;
	t = h__[jm];
	h__[jm] = h__[kk];
	h__[kk] = t;
	j1 = l;
	l = kmj - 1;
	if (l <= 0) {
	    goto L60;
	}
	i__2 = l;
	for (m = 1; m <= i__2; ++m) {
	    jm = jm + j1 + m;
	    t = h__[jm];
	    ++km;
	    h__[jm] = h__[km];
	    h__[km] = t;
/* L50: */
	}
L60:
	if (k1 >= *p) {
	    goto L80;
	}
	l = *p - k1;
	--k1;
	km = kk;
	i__2 = l;
	for (m = 1; m <= i__2; ++m) {
	    km = km + k1 + m;
	    jm = km - kmj;
	    t = h__[jm];
	    h__[jm] = h__[km];
	    h__[km] = t;
/* L70: */
	}
L80:
	k = j;
	j = ip[k];
	ip[k] = -j;
	if (j > i__) {
	    goto L10;
	}
L90:
	;
    }
    return 0;
/*  ***  LAST LINE OF DS7IPR FOLLOWS  *** */
} /* ds7ipr_ */

/* Subroutine */ int dh2rfa_(integer *n, doublereal *a, doublereal *b, 
	doublereal *x, doublereal *y, doublereal *z__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static doublereal t;


/*  ***  APPLY 2X2 HOUSEHOLDER REFLECTION DETERMINED BY X, Y, Z TO */
/*  ***  N-VECTORS A, B  *** */

    /* Parameter adjustments */
    --b;
    --a;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = a[i__] * *x + b[i__] * *y;
	a[i__] += t;
	b[i__] += t * *z__;
/* L10: */
    }
    return 0;
/*  ***  LAST LINE OF DH2RFA FOLLOWS  *** */
} /* dh2rfa_ */

/* Subroutine */ int dparck_(integer *alg, doublereal *d__, integer *iv, 
	integer *liv, integer *lv, integer *n, doublereal *v)
{
    /* Initialized data */

    static doublereal big = 0.;
    static doublereal machep = -1.;
    static doublereal tiny = 1.;
    static doublereal zero = 0.;
    static doublereal vm[34] = { .001,-.99,.001,.01,1.2,.01,1.2,0.,0.,.001,
	    -1.,0.0,0.,0.0,0.,0.,0.0,0.0,0.,-10.,0.,0.,0.,0.0,0.0,0.0,1.01,
	    1e10,0.0,0.,0.,0.,0.0,0. };
    static doublereal vx[34] = { .9,-.001,10.,.8,100.,.8,100.,.5,.5,1.,1.,0.0,
	    0.0,.1,1.,1.,0.0,0.0,1.,0.0,0.0,0.0,1.,1.,1.,1.,1e10,0.0,1.,0.0,
	    1.,1.,1.,1. };
    static char cngd[3][5] = {{"---C"}, {"HANG"}, {"ED V"}};
    static char dflt[3][5] = {{"NOND"}, {"EFAU"}, {"LT V"}};
    static integer ijmp = 33;
    static integer jlim[4] = { 0,24,0,24 };
    static integer ndflt[4] = { 32,25,32,25 };
    static integer miniv[4] = { 82,59,103,103 };

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    ///* Subroutine */ int s_copy(char *, char *, ftnlen, ftnlen);

    /* Local variables */
    static integer i__, j, k, l, m, ii;
    static doublereal vk;
    static integer pu, iv1, alg1, miv1, miv2;
    static char which[4*3];
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dv7dfl_(integer *, integer *, doublereal *), 
	    dv7cpy_(integer *, doublereal *, doublereal *);
    static integer parsv1, ndfalt;
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *);


/*  ***  CHECK ***SOL (VERSION 2.3) PARAMETERS, PRINT CHANGED VALUES  *** */

/*  ***  ALG = 1 FOR REGRESSION, ALG = 2 FOR GENERAL UNCONSTRAINED OPT. */


/* DIVSET  -- SUPPLIES DEFAULT VALUES TO BOTH IV AND V. */
/* DR7MDC -- RETURNS MACHINE-DEPENDENT CONSTANTS. */
/* DV7CPY  -- COPIES ONE VECTOR TO ANOTHER. */
/* DV7DFL  -- SUPPLIES DEFAULT PARAMETER VALUES TO V ALONE. */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  IV AND V SUBSCRIPTS  *** */




    /* Parameter adjustments */
    --iv;
    --v;
    --d__;

    /* Function Body */



/* ...............................  BODY  ................................ */

    pu = 0;
    if (21 <= *liv) {
	pu = iv[21];
    }
    if (51 > *liv) {
	goto L20;
    }
    if (*alg == iv[51]) {
	goto L20;
    }
/*         IF (PU .NE. 0) WRITE(PU,10) ALG, IV(ALGSAV) */
/* 10      FORMAT(/40H THE FIRST PARAMETER TO DIVSET SHOULD BE,I3, */
/*     1          12H RATHER THAN,I3) */
    iv[1] = 67;
    goto L999;
L20:
    if (*alg < 1 || *alg > 4) {
	goto L340;
    }
    miv1 = miniv[*alg - 1];
    if (iv[1] == 15) {
	goto L360;
    }
    alg1 = (*alg - 1) % 2 + 1;
    if (iv[1] == 0) {
	divset_(alg, &iv[1], liv, lv, &v[1]);
    }
    iv1 = iv[1];
    if (iv1 != 13 && iv1 != 12) {
	goto L30;
    }
    if (58 <= *liv) {
/* Computing MAX */
	i__1 = miv1, i__2 = iv[58] - 1;
	miv1 = max(i__1,i__2);
    }
    if (3 <= *liv) {
	miv2 = miv1 + max(iv[3],0);
    }
    if (44 <= *liv) {
	iv[44] = miv2;
    }
    if (*liv < miv1) {
	goto L300;
    }
    iv[3] = 0;
    iv[45] = max(iv[4],0) + iv[42] - 1;
    iv[4] = 0;
    if (*liv < miv2) {
	goto L300;
    }
    if (*lv < iv[45]) {
	goto L320;
    }
L30:
    if (iv1 < 12 || iv1 > 14) {
	goto L60;
    }
    if (*n >= 1) {
	goto L50;
    }
    iv[1] = 81;
    if (pu == 0) {
	goto L999;
    }
/*              WRITE(PU,40) VARNM(ALG1), N */
/* 40           FORMAT(/8H /// BAD,A1,2H =,I5) */
    goto L999;
L50:
    if (iv1 != 14) {
	iv[46] = iv[58];
    }
    if (iv1 != 14) {
	iv[47] = iv[42];
    }
    if (iv1 == 13) {
	goto L999;
    }
    k = iv[49] - 19;
    i__1 = *lv - k;
    dv7dfl_(&alg1, &i__1, &v[k + 1]);
    iv[54] = 2 - alg1;
    iv[38] = *n;
    s_copy(which, dflt[0], (ftnlen)4, (ftnlen)4);
    s_copy(which + 4, dflt[1], (ftnlen)4, (ftnlen)4);
    s_copy(which + 8, dflt[2], (ftnlen)4, (ftnlen)4);
    goto L110;
L60:
    if (*n == iv[38]) {
	goto L80;
    }
    iv[1] = 17;
    if (pu == 0) {
	goto L999;
    }
/*         WRITE(PU,70) VARNM(ALG1), IV(OLDN), N */
/* 70      FORMAT(/5H /// ,1A1,14H CHANGED FROM ,I5,4H TO ,I5) */
    goto L999;

L80:
    if (iv1 <= 11 && iv1 >= 1) {
	goto L100;
    }
    iv[1] = 80;
/*         IF (PU .NE. 0) WRITE(PU,90) IV1 */
/* 90      FORMAT(/13H ///  IV(1) =,I5,28H SHOULD BE BETWEEN 0 AND 14.) */
    goto L999;

L100:
    s_copy(which, cngd[0], (ftnlen)4, (ftnlen)4);
    s_copy(which + 4, cngd[1], (ftnlen)4, (ftnlen)4);
    s_copy(which + 8, cngd[2], (ftnlen)4, (ftnlen)4);

L110:
    if (iv1 == 14) {
	iv1 = 12;
    }
    if (big > tiny) {
	goto L120;
    }
    tiny = dr7mdc_(&c__1);
    machep = dr7mdc_(&c__3);
    big = dr7mdc_(&c__6);
    vm[11] = machep;
    vx[11] = big;
    vx[12] = big;
    vm[13] = machep;
    vm[16] = tiny;
    vx[16] = big;
    vm[17] = tiny;
    vx[17] = big;
    vx[19] = big;
    vx[20] = big;
    vx[21] = big;
    vm[23] = machep;
    vm[24] = machep;
    vm[25] = machep;
    vx[27] = dr7mdc_(&c__5);
    vm[28] = machep;
    vx[29] = big;
    vm[32] = machep;
L120:
    m = 0;
    i__ = 1;
    j = jlim[alg1 - 1];
    k = 19;
    ndfalt = ndflt[alg1 - 1];
    i__1 = ndfalt;
    for (l = 1; l <= i__1; ++l) {
	vk = v[k];
	if (vk >= vm[i__ - 1] && vk <= vx[i__ - 1]) {
	    goto L140;
	}
	m = k;
/*              IF (PU .NE. 0) WRITE(PU,130) VN(1,I), VN(2,I), K, VK, */
/*     1                                    VM(I), VX(I) */
/* 130          FORMAT(/6H ///  ,2A4,5H.. V(,I2,3H) =,D11.3,7H SHOULD, */
/*     1               11H BE BETWEEN,D11.3,4H AND,D11.3) */
L140:
	++k;
	++i__;
	if (i__ == j) {
	    i__ = ijmp;
	}
/* L150: */
    }

    if (iv[50] == ndfalt) {
	goto L170;
    }
    iv[1] = 51;
    if (pu == 0) {
	goto L999;
    }
/*         WRITE(PU,160) IV(NVDFLT), NDFALT */
/* 160     FORMAT(/13H IV(NVDFLT) =,I5,13H RATHER THAN ,I5) */
    goto L999;
L170:
    if ((iv[16] > 0 || v[38] > zero) && iv1 == 12) {
	goto L200;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (d__[i__] > zero) {
	    goto L190;
	}
	m = 18;
/*              IF (PU .NE. 0) WRITE(PU,180) I, D(I) */
/* 180     FORMAT(/8H ///  D(,I3,3H) =,D11.3,19H SHOULD BE POSITIVE) */
L190:
	;
    }
L200:
    if (m == 0) {
	goto L210;
    }
    iv[1] = m;
    goto L999;

L210:
    if (pu == 0 || iv[20] == 0) {
	goto L999;
    }
    if (iv1 != 12 || iv[25] == alg1 - 1) {
	goto L230;
    }
    m = 1;
/*         WRITE(PU,220) SH(ALG1), IV(INITS) */
/* 220     FORMAT(/22H NONDEFAULT VALUES..../5H INIT,A1,14H..... IV(25) =, */
/*     1          I3) */
L230:
    if (iv[16] == iv[54]) {
	goto L250;
    }
/*         IF (M .EQ. 0) WRITE(PU,260) WHICH */
    m = 1;
/*         WRITE(PU,240) IV(DTYPE) */
/* 240     FORMAT(20H DTYPE..... IV(16) =,I3) */
L250:
    i__ = 1;
    j = jlim[alg1 - 1];
    k = 19;
    l = iv[49];
    ndfalt = ndflt[alg1 - 1];
    i__1 = ndfalt;
    for (ii = 1; ii <= i__1; ++ii) {
	if (v[k] == v[l]) {
	    goto L280;
	}
/*              IF (M .EQ. 0) WRITE(PU,260) WHICH */
/* 260          FORMAT(/1H ,3A4,9HALUES..../) */
	m = 1;
/*              WRITE(PU,270) VN(1,I), VN(2,I), K, V(K) */
/* 270          FORMAT(1X,2A4,5H.. V(,I2,3H) =,D15.7) */
L280:
	++k;
	++l;
	++i__;
	if (i__ == j) {
	    i__ = ijmp;
	}
/* L290: */
    }

    iv[54] = iv[16];
    parsv1 = iv[49];
    dv7cpy_(&iv[50], &v[parsv1], &v[19]);
    goto L999;

L300:
    iv[1] = 15;
    if (pu == 0) {
	goto L999;
    }
/*      WRITE(PU,310) LIV, MIV2 */
/* 310  FORMAT(/10H /// LIV =,I5,17H MUST BE AT LEAST,I5) */
    if (*liv < miv1) {
	goto L999;
    }
    if (*lv < iv[45]) {
	goto L320;
    }
    goto L999;

L320:
    iv[1] = 16;
/*      IF (PU .NE. 0) WRITE(PU,330) LV, IV(LASTV) */
/* 330  FORMAT(/9H /// LV =,I5,17H MUST BE AT LEAST,I5) */
    goto L999;

L340:
    iv[1] = 67;
/*      IF (PU .NE. 0) WRITE(PU,350) ALG */
/* 350  FORMAT(/10H /// ALG =,I5,21H MUST BE 1 2, 3, OR 4) */
    goto L999;
L360:
/* 360  IF (PU .NE. 0) WRITE(PU,370) LIV, MIV1 */
/* 370  FORMAT(/10H /// LIV =,I5,17H MUST BE AT LEAST,I5, */
/*     1       37H TO COMPUTE TRUE MIN. LIV AND MIN. LV) */
    if (44 <= *liv) {
	iv[44] = miv1;
    }
    if (45 <= *liv) {
	iv[45] = 0;
    }

L999:
    return 0;
/*  ***  LAST LINE OF DPARCK FOLLOWS  *** */
} /* dparck_ */

/* Subroutine */ int dq7apl_(integer *nn, integer *n, integer *p, doublereal *
	j, doublereal *r__, integer *ierr)
{
    /* System generated locals */
    integer j_dim1, j_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer k, l, nl1;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *);

/*     *****PARAMETERS. */

/*     .................................................................. */
/*     .................................................................. */

/*     *****PURPOSE. */
/*     THIS SUBROUTINE APPLIES TO R THE ORTHOGONAL TRANSFORMATIONS */
/*     STORED IN J BY QRFACT */

/*     *****PARAMETER DESCRIPTION. */
/*     ON INPUT. */

/*        NN IS THE ROW DIMENSION OF THE MATRIX J AS DECLARED IN */
/*             THE CALLING PROGRAM DIMENSION STATEMENT */

/*        N IS THE NUMBER OF ROWS OF J AND THE SIZE OF THE VECTOR R */

/*        P IS THE NUMBER OF COLUMNS OF J AND THE SIZE OF SIGMA */

/*        J CONTAINS ON AND BELOW ITS DIAGONAL THE COLUMN VECTORS */
/*             U WHICH DETERMINE THE HOUSEHOLDER TRANSFORMATIONS */
/*             IDENT - U*U.TRANSPOSE */

/*        R IS THE RIGHT HAND SIDE VECTOR TO WHICH THE ORTHOGONAL */
/*             TRANSFORMATIONS WILL BE APPLIED */

/*        IERR IF NON-ZERO INDICATES THAT NOT ALL THE TRANSFORMATIONS */
/*             WERE SUCCESSFULLY DETERMINED AND ONLY THE FIRST */
/*             ABS(IERR) - 1 TRANSFORMATIONS WILL BE USED */

/*     ON OUTPUT. */

/*        R HAS BEEN OVERWRITTEN BY ITS TRANSFORMED IMAGE */

/*     *****APPLICATION AND USAGE RESTRICTIONS. */
/*     NONE */

/*     *****ALGORITHM NOTES. */
/*     THE VECTORS U WHICH DETERMINE THE HOUSEHOLDER TRANSFORMATIONS */
/*     ARE NORMALIZED SO THAT THEIR 2-NORM SQUARED IS 2.  THE USE OF */
/*     THESE TRANSFORMATIONS HERE IS IN THE SPIRIT OF (1). */

/*     *****SUBROUTINES AND FUNCTIONS CALLED. */

/*     DD7TPR - FUNCTION, RETURNS THE INNER PRODUCT OF VECTORS */

/*     *****REFERENCES. */
/*     (1) BUSINGER, P. A., AND GOLUB, G. H. (1965), LINEAR LEAST SQUARES */
/*        SOLUTIONS BY HOUSEHOLDER TRANSFORMATIONS, NUMER. MATH. 7, */
/*        PP. 269-276. */

/*     *****HISTORY. */
/*     DESIGNED BY DAVID M. GAY, CODED BY STEPHEN C. PETERS (WINTER 1977) */
/*     CALL ON DV2AXY SUBSTITUTED FOR DO LOOP, FALL 1983. */

/*     *****GENERAL. */

/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, AND MCS76-11989. */

/*     .................................................................. */
/*     .................................................................. */

/*     *****LOCAL VARIABLES. */
/*     *****FUNCTIONS. */

/*  ***  BODY  *** */

    /* Parameter adjustments */
    --r__;
    j_dim1 = *nn;
    j_offset = 1 + j_dim1;
    j -= j_offset;

    /* Function Body */
    k = *p;
    if (*ierr != 0) {
	k = abs(*ierr) - 1;
    }
    if (k == 0) {
	goto L999;
    }

    i__1 = k;
    for (l = 1; l <= i__1; ++l) {
	nl1 = *n - l + 1;
	d__1 = -dd7tpr_(&nl1, &j[l + l * j_dim1], &r__[l]);
	dv2axy_(&nl1, &r__[l], &d__1, &j[l + l * j_dim1], &r__[l]);
/* L20: */
    }

L999:
    return 0;
/*  ***  LAST LINE OF DQ7APL FOLLOWS  *** */
} /* dq7apl_ */

/* Subroutine */ int dv7dfl_(integer *alg, integer *lv, doublereal *v)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    extern doublereal dr7mdc_(integer *);
    static doublereal machep, mepcrt, sqteps;


/*  ***  SUPPLY ***SOL (VERSION 2.3) DEFAULT VALUES TO V  *** */

/*  ***  ALG = 1 MEANS REGRESSION CONSTANTS. */
/*  ***  ALG = 2 MEANS GENERAL UNCONSTRAINED OPTIMIZATION CONSTANTS. */


/* DR7MDC... RETURNS MACHINE-DEPENDENT CONSTANTS */


/*  ***  SUBSCRIPTS FOR V  *** */



/*  ***  V SUBSCRIPT VALUES  *** */


/* -------------------------------  BODY  -------------------------------- */

    /* Parameter adjustments */
    --v;

    /* Function Body */
    machep = dr7mdc_(&c__3);
    if (machep > 1e-10) {
/* Computing 2nd power */
	d__1 = machep;
	v[31] = d__1 * d__1;
    } else {
	v[31] = 1e-20;
    }
    v[22] = .5;
    sqteps = dr7mdc_(&c__4);
    v[41] = .6;
    v[39] = 1e-6;
    mepcrt = pow_dd(&machep, &c_b325);
    v[40] = 1.;
    v[19] = .1;
    v[23] = 2.;
    v[35] = 1.;
    v[36] = 1.;
    v[20] = -.1;
    v[21] = .1;
    v[24] = .1;
    v[25] = 4.;
/* Computing MAX */
/* Computing 2nd power */
    d__3 = mepcrt;
    d__1 = 1e-10, d__2 = d__3 * d__3;
    v[32] = max(d__1,d__2);
    v[37] = v[32];
    v[26] = .1;
    v[27] = 1e-4;
    v[28] = .75;
    v[29] = .5;
    v[30] = .75;
    v[33] = sqteps;
    v[34] = machep * 100.;

    if (*alg == 1) {

/*  ***  REGRESSION  VALUES (nls) */

/* Computing MAX */
	d__1 = 1e-6, d__2 = machep * 100.;
	v[47] = max(d__1,d__2);
	v[38] = 0.;
	v[44] = sqteps;
	v[42] = mepcrt;
	v[43] = sqteps;
	v[45] = 1.5;
	v[46] = dr7mdc_(&c__5);
	v[49] = .001;
	v[50] = 1e-4;
    } else {

/*  ***  GENERAL OPTIMIZATION VALUES (nlminb) */

	v[43] = .8;
	v[38] = -1.;
	v[42] = machep * 1e3;

    }
/*  ***  LAST CARD OF DV7DFL FOLLOWS  *** */
    return 0;
} /* dv7dfl_ */

doublereal dr7mdc_(integer *k)
{
    /* Initialized data */

    static doublereal big = 0.;
    static doublereal eta = 0.;
    static doublereal machep = 0.;
    static doublereal zero = 0.;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    extern doublereal d1mach_(integer *);


/*  ***  RETURN MACHINE DEPENDENT CONSTANTS USED BY NL2SOL  *** */


/*  ***  THE CONSTANT RETURNED DEPENDS ON K... */

/*  ***        K = 1... SMALLEST POS. ETA SUCH THAT -ETA EXISTS. */
/*  ***        K = 2... SQUARE ROOT OF ETA. */
/*  ***        K = 3... UNIT ROUNDOFF = SMALLEST POS. NO. MACHEP SUCH */
/*  ***                 THAT 1 + MACHEP .GT. 1 .AND. 1 - MACHEP .LT. 1. */
/*  ***        K = 4... SQUARE ROOT OF MACHEP. */
/*  ***        K = 5... SQUARE ROOT OF BIG (SEE K = 6). */
/*  ***        K = 6... LARGEST MACHINE NO. BIG SUCH THAT -BIG EXISTS. */

/* /+ */
/* / */

    if (big > zero) {
	goto L1;
    }
    big = d1mach_(&c__2);
    eta = d1mach_(&c__1);
    machep = d1mach_(&c__4);
L1:

/* -------------------------------  BODY  -------------------------------- */

    switch (*k) {
	case 1:  goto L10;
	case 2:  goto L20;
	case 3:  goto L30;
	case 4:  goto L40;
	case 5:  goto L50;
	case 6:  goto L60;
    }

L10:
    ret_val = eta;
    goto L999;

L20:
    ret_val = sqrt(eta * 256.) / 16.;
    goto L999;

L30:
    ret_val = machep;
    goto L999;

L40:
    ret_val = sqrt(machep);
    goto L999;

L50:
    ret_val = sqrt(big / 256.) * 16.;
    goto L999;

L60:
    ret_val = big;

L999:
    return ret_val;
/*  ***  LAST CARD OF DR7MDC FOLLOWS  *** */
} /* dr7mdc_ */

/* Subroutine */ int dg7itb_(doublereal *b, doublereal *d__, doublereal *g, 
	integer *iv, integer *liv, integer *lv, integer *p, integer *ps, 
	doublereal *v, doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static doublereal e;
    static integer i__, j, k, l;
    static doublereal t;
    static integer h1, i1, p1, s1;
    static doublereal t1;
    static integer w1, g01;
    static doublereal gi;
    static integer x01;
    static doublereal xi;
    static integer hc1, td1, tg1, pp1, ipi, ipn, dig1, wlm1, qtr1, pp1o2, 
	    lmat1, p1len, rmat1, temp1, ipiv0, ipiv1, ipiv2, temp2, step1;
    static logical havrm;
    static integer dummy;
    extern /* Subroutine */ int df7dhb_(doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, integer 
	    *, doublereal *, doublereal *);
    extern logical stopx_(integer *);
    extern /* Subroutine */ int dg7qsb_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, integer *, 
	    integer *, doublereal *, integer *, integer *, integer *, integer 
	    *, doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), dl7msb_(doublereal *, 
	    doublereal *, doublereal *, integer *, integer *, integer *, 
	    integer *, integer *, doublereal *, integer *, integer *, integer 
	    *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), ds7dmp_(integer *, 
	    doublereal *, doublereal *, doublereal *, integer *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), i7shft_(integer *, integer *, integer *), dl7vml_(
	    integer *, doublereal *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dq7rsh_(integer *, integer *, logical *, 
	    doublereal *, doublereal *, doublereal *), i7copy_(integer *, 
	    integer *, integer *), ds7ipr_(integer *, integer *, doublereal *)
	    , dl7sqr_(integer *, doublereal *, doublereal *), dl7tvm_(integer 
	    *, doublereal *, doublereal *, doublereal *), ds7lup_(doublereal *
	    , doublereal *, integer *, doublereal *, doublereal *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, doublereal *), 
	    ds7lvm_(integer *, doublereal *, doublereal *, doublereal *), 
	    dv2axy_(integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dv7cpy_(integer *, doublereal *, doublereal *), 
	    dv7ipr_(integer *, integer *, doublereal *), dv7scp_(integer *, 
	    doublereal *, doublereal *), dv7vmp_(integer *, doublereal *, 
	    doublereal *, doublereal *, integer *), i7pnvr_(integer *, 
	    integer *, integer *), dparck_(integer *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    static logical havqtr;
    static integer stpmod;
    extern /* Subroutine */ int ditsum_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *, doublereal *);
    static integer lstgst, rstrst;
    static doublereal sttsst;


/*  ***  CARRY OUT NL2SOL-LIKE ITERATIONS FOR GENERALIZED LINEAR   *** */
/*  ***  REGRESSION PROBLEMS (AND OTHERS OF SIMILAR STRUCTURE)     *** */
/*  ***  HAVING SIMPLE BOUNDS ON THE PARAMETERS BEING ESTIMATED.   *** */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* B.... VECTOR OF LOWER AND UPPER BOUNDS ON X. */
/* D.... SCALE VECTOR. */
/* IV... INTEGER VALUE ARRAY. */
/* LIV.. LENGTH OF IV.  MUST BE AT LEAST 80. */
/* LH... LENGTH OF H = P*(P+1)/2. */
/* LV... LENGTH OF V.  MUST BE AT LEAST P*(3*P + 19)/2 + 7. */
/* G.... GRADIENT AT X (WHEN IV(1) = 2). */
/* HC... GAUSS-NEWTON HESSIAN AT X (WHEN IV(1) = 2). */
/* P.... NUMBER OF PARAMETERS (COMPONENTS IN X). */
/* PS... NUMBER OF NONZERO ROWS AND COLUMNS IN S. */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... PARAMETER VECTOR. */
/* Y.... PART OF YIELD VECTOR (WHEN IV(1)= 2, SCRATCH OTHERWISE). */

/*  ***  DISCUSSION  *** */

/*        DG7ITB IS SIMILAR TO DG7LIT, EXCEPT FOR THE EXTRA PARAMETER B */
/*     -- DG7ITB ENFORCES THE BOUNDS  B(1,I) .LE. X(I) .LE. B(2,I), */
/*     I = 1(1)P. */
/*        DG7ITB PERFORMS NL2SOL-LIKE ITERATIONS FOR A VARIETY OF */
/*     REGRESSION PROBLEMS THAT ARE SIMILAR TO NONLINEAR LEAST-SQUARES */
/*     IN THAT THE HESSIAN IS THE SUM OF TWO TERMS, A READILY-COMPUTED */
/*     FIRST-ORDER TERM AND A SECOND-ORDER TERM.  THE CALLER SUPPLIES */
/*     THE FIRST-ORDER TERM OF THE HESSIAN IN HC (LOWER TRIANGLE, STORED */
/*     COMPACTLY BY ROWS), AND DG7ITB BUILDS AN APPROXIMATION, S, TO THE */
/*     SECOND-ORDER TERM.  THE CALLER ALSO PROVIDES THE FUNCTION VALUE, */
/*     GRADIENT, AND PART OF THE YIELD VECTOR USED IN UPDATING S. */
/*     DG7ITB DECIDES DYNAMICALLY WHETHER OR NOT TO USE S WHEN CHOOSING */
/*     THE NEXT STEP TO TRY...  THE HESSIAN APPROXIMATION USED IS EITHER */
/*     HC ALONE (GAUSS-NEWTON MODEL) OR HC + S (AUGMENTED MODEL). */
/*     IF PS .LT. P, THEN ROWS AND COLUMNS PS+1...P OF S ARE KEPT */
/*     CONSTANT.  THEY WILL BE ZERO UNLESS THE CALLER SETS IV(INITS) TO */
/*     1 OR 2 AND SUPPLIES NONZERO VALUES FOR THEM, OR THE CALLER SETS */
/*     IV(INITS) TO 3 OR 4 AND THE FINITE-DIFFERENCE INITIAL S THEN */
/*     COMPUTED HAS NONZERO VALUES IN THESE ROWS. */

/*        IF IV(INITS) IS 3 OR 4, THEN THE INITIAL S IS COMPUTED BY */
/*     FINITE DIFFERENCES.  3 MEANS USE FUNCTION DIFFERENCES, 4 MEANS */
/*     USE GRADIENT DIFFERENCES.  FINITE DIFFERENCING IS DONE THE SAME */
/*     WAY AS IN COMPUTING A COVARIANCE MATRIX (WITH IV(COVREQ) = -1, -2, */
/*     1, OR 2). */

/*        FOR UPDATING S, DG7ITB ASSUMES THAT THE GRADIENT HAS THE FORM */
/*     OF A SUM OVER I OF RHO(I,X)*GRAD(R(I,X)), WHERE GRAD DENOTES THE */
/*     GRADIENT WITH RESPECT TO X.  THE TRUE SECOND-ORDER TERM THEN IS */
/*     THE SUM OVER I OF RHO(I,X)*HESSIAN(R(I,X)).  IF X = X0 + STEP, */
/*     THEN WE WISH TO UPDATE S SO THAT S*STEP IS THE SUM OVER I OF */
/*     RHO(I,X)*(GRAD(R(I,X)) - GRAD(R(I,X0))).  THE CALLER MUST SUPPLY */
/*     PART OF THIS IN Y, NAMELY THE SUM OVER I OF */
/*     RHO(I,X)*GRAD(R(I,X0)), WHEN CALLING DG7ITB WITH IV(1) = 2 AND */
/*     IV(MODE) = 0 (WHERE MODE = 38).  G THEN CONTANS THE OTHER PART, */
/*     SO THAT THE DESIRED YIELD VECTOR IS G - Y.  IF PS .LT. P, THEN */
/*     THE ABOVE DISCUSSION APPLIES ONLY TO THE FIRST PS COMPONENTS OF */
/*     GRAD(R(I,X)), STEP, AND Y. */

/*        PARAMETERS IV, P, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO  DN2GB (AND NL2SOL), EXCEPT THAT V CAN BE SHORTER */
/*     (SINCE THE PART OF V THAT  DN2GB USES FOR STORING D, J, AND R IS */
/*     NOT NEEDED).  MOREOVER, COMPARED WITH  DN2GB (AND NL2SOL), IV(1) */
/*     MAY HAVE THE TWO ADDITIONAL OUTPUT VALUES 1 AND 2, WHICH ARE */
/*     EXPLAINED BELOW, AS IS THE USE OF IV(TOOBIG) AND IV(NFGCAL). */
/*     THE VALUES IV(D), IV(J), AND IV(R), WHICH ARE OUTPUT VALUES FROM */
/*      DN2GB (AND  DN2FB), ARE NOT REFERENCED BY DG7ITB OR THE */
/*     SUBROUTINES IT CALLS. */

/*        WHEN DG7ITB IS FIRST CALLED, I.E., WHEN DG7ITB IS CALLED WITH */
/*     IV(1) = 0 OR 12, V(F), G, AND HC NEED NOT BE INITIALIZED.  TO */
/*     OBTAIN THESE STARTING VALUES, DG7ITB RETURNS FIRST WITH IV(1) = 1, */
/*     THEN WITH IV(1) = 2, WITH IV(MODE) = -1 IN BOTH CASES.  ON */
/*     SUBSEQUENT RETURNS WITH IV(1) = 2, IV(MODE) = 0 IMPLIES THAT */
/*     Y MUST ALSO BE SUPPLIED.  (NOTE THAT Y IS USED FOR SCRATCH -- ITS */
/*     INPUT CONTENTS ARE LOST.  BY CONTRAST, HC IS NEVER CHANGED.) */
/*     ONCE CONVERGENCE HAS BEEN OBTAINED, IV(RDREQ) AND IV(COVREQ) MAY */
/*     IMPLY THAT A FINITE-DIFFERENCE HESSIAN SHOULD BE COMPUTED FOR USE */
/*     IN COMPUTING A COVARIANCE MATRIX.  IN THIS CASE DG7ITB WILL MAKE */
/*     A NUMBER OF RETURNS WITH IV(1) = 1 OR 2 AND IV(MODE) POSITIVE. */
/*     WHEN IV(MODE) IS POSITIVE, Y SHOULD NOT BE CHANGED. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET V(F) (I.E., V(10)) TO F(X), THE */
/*             FUNCTION VALUE AT X, AND CALL DG7ITB AGAIN, HAVING CHANGED */
/*             NONE OF THE OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) */
/*             CANNOT BE EVALUATED (E.G. IF OVERFLOW WOULD OCCUR), WHICH */
/*             MAY HAPPEN BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE */
/*             THE CALLER SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL */
/*             CAUSE DG7ITB TO IGNORE V(F) AND TRY A SMALLER STEP.  NOTE */
/*             THAT THE CURRENT FUNCTION EVALUATION COUNT IS AVAILABLE */
/*             IN IV(NFCALL) = IV(6).  THIS MAY BE USED TO IDENTIFY */
/*             WHICH COPY OF SAVED INFORMATION SHOULD BE USED IN COM- */
/*             PUTING G, HC, AND Y THE NEXT TIME DG7ITB RETURNS WITH */
/*             IV(1) = 2.  SEE MLPIT FOR AN EXAMPLE OF THIS. */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
/*             X.  THE CALLER SHOULD ALSO SET HC TO THE GAUSS-NEWTON */
/*             HESSIAN AT X.  IF IV(MODE) = 0, THEN THE CALLER SHOULD */
/*             ALSO COMPUTE THE PART OF THE YIELD VECTOR DESCRIBED ABOVE. */
/*             THE CALLER SHOULD THEN CALL DG7ITB AGAIN (WITH IV(1) = 2). */
/*             THE CALLER MAY ALSO CHANGE D AT THIS TIME, BUT SHOULD NOT */
/*             CHANGE X.  NOTE THAT IV(NFGCAL) = IV(7) CONTAINS THE */
/*             VALUE THAT IV(NFCALL) HAD DURING THE RETURN WITH */
/*             IV(1) = 1 IN WHICH X HAD THE SAME VALUE AS IT NOW HAS. */
/*             IV(NFGCAL) IS EITHER IV(NFCALL) OR IV(NFCALL) - 1.  MLPIT */
/*             IS AN EXAMPLE WHERE THIS INFORMATION IS USED.  IF G OR HC */
/*             CANNOT BE EVALUATED AT X, THEN THE CALLER MAY SET */
/*             IV(NFGCAL) TO 0, IN WHICH CASE DG7ITB WILL RETURN WITH */
/*             IV(1) = 15. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */

/*        (SEE NL2SOL FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DF7DHB... COMPUTE FINITE-DIFFERENCE HESSIAN (FOR INIT. S MATRIX). */
/* DG7QSB... COMPUTES GOLDFELD-QUANDT-TROTTER STEP (AUGMENTED MODEL). */
/* I7COPY.... COPIES ONE INTEGER VECTOR TO ANOTHER. */
/* I7PNVR... INVERTS PERMUTATION ARRAY. */
/* I7SHFT... SHIFTS AN INTEGER VECTOR. */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DL7MSB... COMPUTES LEVENBERG-MARQUARDT STEP (GAUSS-NEWTON MODEL). */
/* DL7SQR... COMPUTES L * L**T FROM LOWER TRIANGULAR MATRIX L. */
/* DL7TVM... COMPUTES L**T * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DL7VML.... COMPUTES L * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DPARCK.... CHECK VALIDITY OF IV AND V INPUT COMPONENTS. */
/* DQ7RSH... SHIFTS A QR FACTORIZATION. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* DS7DMP... MULTIPLIES A SYM. MATRIX FORE AND AFT BY A DIAG. MATRIX. */
/* DS7IPR... APPLIES PERMUTATION TO (LOWER TRIANG. OF) SYM. MATRIX. */
/* DS7LUP... PERFORMS QUASI-NEWTON UPDATE ON COMPACTLY STORED LOWER TRI- */
/*             ANGLE OF A SYMMETRIC MATRIX. */
/* DS7LVM... MULTIPLIES COMPACTLY STORED SYM. MATRIX TIMES VECTOR. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7IPR... APPLIES A PERMUTATION TO A VECTOR. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV7VMP... MULTIPLIES (DIVIDES) VECTORS COMPONENTWISE. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */

/*  ***  (NOTE THAT P0 AND PC ARE STORED IN IV(G0) AND IV(STLSTG) RESP.) */


/*  ***  V SUBSCRIPT VALUES  *** */




/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --y;
    --x;
    --g;
    --d__;
    b -= 3;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L50;
    }
    if (i__ == 2) {
	goto L60;
    }

    if (i__ < 12) {
	goto L10;
    }
    if (i__ > 13) {
	goto L10;
    }
    iv[4] = iv[4] + *p * (*p * 3 + 25) / 2 + 7;
    iv[3] += *p << 2;
L10:
    dparck_(&c__1, &d__[1], &iv[1], liv, lv, p, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    switch (i__) {
	case 1:  goto L360;
	case 2:  goto L360;
	case 3:  goto L360;
	case 4:  goto L360;
	case 5:  goto L360;
	case 6:  goto L360;
	case 7:  goto L240;
	case 8:  goto L190;
	case 9:  goto L240;
	case 10:  goto L20;
	case 11:  goto L20;
	case 12:  goto L30;
    }

/*  ***  STORAGE ALLOCATION  *** */

L20:
    pp1o2 = *p * (*p + 1) / 2;
    iv[62] = iv[42] + pp1o2;
    iv[43] = iv[62] + pp1o2;
    iv[40] = iv[43] + (*p << 1);
    iv[37] = iv[40] + *p * 3;
    iv[65] = iv[37] + (*p << 1);
    iv[56] = iv[65] + (*p << 2) + 7;
    iv[47] = iv[56] + pp1o2;
    iv[76] = iv[58] + *p * 3;
    iv[46] = iv[76] + *p;
    if (iv[1] != 13) {
	goto L30;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L30:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[11] = 2;
    iv[2] = 0;
    iv[55] = 0;
    iv[26] = 0;
    iv[52] = 0;
    iv[53] = 0;
    iv[8] = 0;
    iv[41] = *p;
    v[9] = 0.;
    v[5] = 0.;
    v[8] = v[35] / (v[21] + 1.);

/*  ***  CHECK CONSISTENCY OF B AND INITIALIZE IP ARRAY  *** */

    ipi = iv[76];
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iv[ipi] = i__;
	++ipi;
	if (b[(i__ << 1) + 1] > b[(i__ << 1) + 2]) {
	    goto L680;
	}
/* L40: */
    }

/*  ***  SET INITIAL MODEL AND S MATRIX  *** */

    iv[5] = 1;
    iv[1] = 1;
    if (iv[62] < 0) {
	goto L710;
    }
    if (iv[25] > 1) {
	iv[5] = 2;
    }
    s1 = iv[62];
    if (iv[25] == 0 || iv[25] > 2) {
	i__1 = *p * (*p + 1) / 2;
	dv7scp_(&i__1, &v[s1], &c_b14);
    }
    goto L710;

/*  ***  NEW FUNCTION VALUE  *** */

L50:
    if (iv[35] == 0) {
	goto L360;
    }
    if (iv[35] > 0) {
	goto L590;
    }

    if (iv[2] == 0) {
	goto L690;
    }
    iv[1] = 63;
    goto L999;

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L60:
    if (iv[2] == 0) {
	goto L70;
    }
    iv[1] = 65;
    goto L999;

/*  ***  NEW GRADIENT  *** */

L70:
    iv[34] = -1;
    iv[33] = -1;
    iv[74] = 0;
    if (iv[35] > 0) {
	goto L590;
    }
    if (iv[71] <= 0 && iv[78] <= 0) {
	goto L670;
    }

/*  ***  CHOOSE INITIAL PERMUTATION  *** */

    ipi = iv[76];
    ipn = ipi + *p - 1;
    ipiv2 = iv[58] - 1;
    k = iv[41];
    p1 = *p;
    pp1 = *p + 1;
    rmat1 = iv[78];
    havrm = rmat1 > 0;
    qtr1 = iv[77];
    havqtr = qtr1 > 0;
/*     *** MAKE SURE V(QTR1) IS LEGAL (EVEN WHEN NOT REFERENCED) *** */
    w1 = iv[65];
    if (! havqtr) {
	qtr1 = w1 + *p;
    }

    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i1 = iv[ipn];
	--ipn;
	if (b[(i1 << 1) + 1] >= b[(i1 << 1) + 2]) {
	    goto L80;
	}
	xi = x[i1];
	gi = g[i1];
	if (xi <= b[(i1 << 1) + 1] && gi > 0.) {
	    goto L80;
	}
	if (xi >= b[(i1 << 1) + 2] && gi < 0.) {
	    goto L80;
	}
/*           *** DISALLOW CONVERGENCE IF X(I1) HAS JUST BEEN FREED *** */
	j = ipiv2 + i1;
	if (iv[j] > k) {
	    iv[55] = 0;
	}
	goto L100;
L80:
	if (i1 >= p1) {
	    goto L90;
	}
	i1 = pp1 - i__;
	i7shft_(&p1, &i1, &iv[ipi]);
	if (havrm) {
	    dq7rsh_(&i1, &p1, &havqtr, &v[qtr1], &v[rmat1], &v[w1]);
	}
L90:
	--p1;
L100:
	;
    }
    iv[41] = p1;

/*  ***  COMPUTE V(DGNORM) (AN OUTPUT VALUE IF WE STOP NOW)  *** */

    v[1] = 0.;
    if (p1 <= 0) {
	goto L110;
    }
    dig1 = iv[37];
    dv7vmp_(p, &v[dig1], &g[1], &d__[1], &c_n1);
    dv7ipr_(p, &iv[ipi], &v[dig1]);
    v[1] = dv2nrm_(&p1, &v[dig1]);
L110:
    if (iv[55] != 0) {
	goto L580;
    }
    if (iv[35] == 0) {
	goto L510;
    }
    iv[35] = 0;
    v[13] = v[10];
    if (iv[25] <= 2) {
	goto L170;
    }

/*  ***  ARRANGE FOR FINITE-DIFFERENCE INITIAL S  *** */

    iv[13] = iv[15];
    iv[15] = -1;
    if (iv[25] > 3) {
	iv[15] = 1;
    }
    iv[55] = 70;
    goto L600;

/*  ***  COME TO NEXT STMT AFTER COMPUTING F.D. HESSIAN FOR INIT. S  *** */

L120:
    h1 = iv[74];
    if (h1 <= 0) {
	goto L660;
    }
    iv[55] = 0;
    iv[35] = 0;
    iv[52] = 0;
    iv[53] = 0;
    iv[15] = iv[13];
    s1 = iv[62];
    pp1o2 = *ps * (*ps + 1) / 2;
    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L130;
    }
    dv2axy_(&pp1o2, &v[s1], &c_b90, &v[hc1], &v[h1]);
    goto L140;
L130:
    rmat1 = iv[78];
    lmat1 = iv[42];
    dl7sqr_(p, &v[lmat1], &v[rmat1]);
    ipi = iv[76];
    ipiv1 = iv[58] + *p;
    i7pnvr_(p, &iv[ipiv1], &iv[ipi]);
    ds7ipr_(p, &iv[ipiv1], &v[lmat1]);
    dv2axy_(&pp1o2, &v[s1], &c_b90, &v[lmat1], &v[h1]);

/*     *** ZERO PORTION OF S CORRESPONDING TO FIXED X COMPONENTS *** */

L140:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	    goto L160;
	}
	k = s1 + i__ * (i__ - 1) / 2;
	dv7scp_(&i__, &v[k], &c_b14);
	if (i__ >= *p) {
	    goto L170;
	}
	k = k + (i__ << 1) - 1;
	i1 = i__ + 1;
	i__2 = *p;
	for (j = i1; j <= i__2; ++j) {
	    v[k] = 0.;
	    k += j;
/* L150: */
	}
L160:
	;
    }

L170:
    iv[1] = 2;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L180:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, p, &v[1], &x[1]);
L190:
    k = iv[31];
    if (k < iv[18]) {
	goto L200;
    }
    iv[1] = 10;
    goto L999;
L200:
    iv[31] = k + 1;

/*  ***  UPDATE RADIUS  *** */

    if (k == 0) {
	goto L220;
    }
    step1 = iv[40];
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[step1] = d__[i__] * v[step1];
	++step1;
/* L210: */
    }
    step1 = iv[40];
    t = v[16] * dv2nrm_(p, &v[step1]);
    if (v[16] < 1. || t > v[8]) {
	v[8] = t;
    }

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

L220:
    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[56] = -abs(iv[56]);
    iv[64] = iv[5];

/*     ***  COPY X TO X0  *** */

    dv7cpy_(p, &v[x01], &x[1]);

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L230:
    if (! stopx_(&dummy)) {
	goto L250;
    }
    iv[1] = 11;
    goto L260;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L240:
    if (v[10] >= v[13]) {
	goto L250;
    }
    v[16] = 1.;
    k = iv[31];
    goto L200;

L250:
    if (iv[6] < iv[17] + iv[52]) {
	goto L270;
    }
    iv[1] = 9;
L260:
    if (v[10] >= v[13]) {
	goto L999;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L500;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L270:
    step1 = iv[40];
    tg1 = iv[37];
    td1 = tg1 + *p;
    x01 = iv[43];
    w1 = iv[65];
    h1 = iv[56];
    p1 = iv[41];
    ipi = iv[58];
    ipiv1 = ipi + *p;
    ipiv2 = ipiv1 + *p;
    ipiv0 = iv[76];
    if (iv[5] == 2) {
	goto L280;
    }

/*        ***  COMPUTE LEVENBERG-MARQUARDT STEP IF POSSIBLE... */

    rmat1 = iv[78];
    if (rmat1 <= 0) {
	goto L280;
    }
    qtr1 = iv[77];
    if (qtr1 <= 0) {
	goto L280;
    }
    lmat1 = iv[42];
    wlm1 = w1 + *p;
    dl7msb_(&b[3], &d__[1], &g[1], &iv[75], &iv[ipiv0], &iv[ipiv1], &iv[ipiv2]
	    , &iv[34], &v[lmat1], lv, p, &iv[48], &iv[41], &v[qtr1], &v[rmat1]
	    , &v[step1], &v[td1], &v[tg1], &v[1], &v[w1], &v[wlm1], &x[1], &v[
	    x01]);
/*        *** H IS STORED IN THE END OF W AND HAS JUST BEEN OVERWRITTEN, */
/*        *** SO WE MARK IT INVALID... */
    iv[56] = -abs(h1);
/*        *** EVEN IF H WERE STORED ELSEWHERE, IT WOULD BE NECESSARY TO */
/*        *** MARK INVALID THE INFORMATION DG7QTS MAY HAVE STORED IN V... */
    iv[33] = -1;
    goto L330;

L280:
    if (h1 > 0) {
	goto L320;
    }

/*     ***  SET H TO  D**-1 * (HC + T1*S) * D**-1.  *** */

    p1len = p1 * (p1 + 1) / 2;
    h1 = -h1;
    iv[56] = h1;
    iv[74] = 0;
    if (p1 <= 0) {
	goto L320;
    }
/*        *** MAKE TEMPORARY PERMUTATION ARRAY *** */
    i7copy_(p, &iv[ipi], &iv[ipiv0]);
    j = iv[71];
    if (j > 0) {
	goto L290;
    }
    j = h1;
    rmat1 = iv[78];
    dl7sqr_(&p1, &v[h1], &v[rmat1]);
    goto L300;
L290:
    i__1 = *p * (*p + 1) / 2;
    dv7cpy_(&i__1, &v[h1], &v[j]);
    ds7ipr_(p, &iv[ipi], &v[h1]);
L300:
    if (iv[5] == 1) {
	goto L310;
    }
    lmat1 = iv[42];
    s1 = iv[62];
    i__1 = *p * (*p + 1) / 2;
    dv7cpy_(&i__1, &v[lmat1], &v[s1]);
    ds7ipr_(p, &iv[ipi], &v[lmat1]);
    dv2axy_(&p1len, &v[h1], &c_b85, &v[lmat1], &v[h1]);
L310:
    dv7cpy_(p, &v[td1], &d__[1]);
    dv7ipr_(p, &iv[ipi], &v[td1]);
    ds7dmp_(&p1, &v[h1], &v[h1], &v[td1], &c_n1);
    iv[33] = -1;

/*  ***  COMPUTE ACTUAL GOLDFELD-QUANDT-TROTTER STEP  *** */

L320:
    lmat1 = iv[42];
    dg7qsb_(&b[3], &d__[1], &v[h1], &g[1], &iv[ipi], &iv[ipiv1], &iv[ipiv2], &
	    iv[33], &v[lmat1], lv, p, &iv[48], &p1, &v[step1], &v[td1], &v[
	    tg1], &v[1], &v[w1], &x[1], &v[x01]);
    if (iv[34] > 0) {
	iv[34] = 0;
    }

L330:
    if (iv[29] != 6) {
	goto L340;
    }
    if (iv[9] != 2) {
	goto L360;
    }
    rstrst = 2;
    goto L370;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L340:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L360;
    }
    if (iv[29] != 5) {
	goto L350;
    }
    if (v[16] <= 1.) {
	goto L350;
    }
    if (v[7] > v[11] * 1.2) {
	goto L350;
    }
    step1 = iv[40];
    x01 = iv[43];
    dv2axy_(p, &v[step1], &c_b90, &v[x01], &x[1]);
    if (iv[9] != 2) {
	goto L360;
    }
    rstrst = 0;
    goto L370;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L350:
    x01 = iv[43];
    step1 = iv[40];
    dv2axy_(p, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L710;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L360:
    rstrst = 3;
L370:
    x01 = iv[43];
    v[17] = drldst_(p, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = x01 + *p;
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L410;
	case 2:  goto L380;
	case 3:  goto L390;
	case 4:  goto L400;
    }
L380:
    dv7cpy_(p, &x[1], &v[x01]);
    goto L410;
L390:
    dv7cpy_(p, &v[lstgst], &v[step1]);
    goto L410;
L400:
    dv7cpy_(p, &v[step1], &v[lstgst]);
    dv2axy_(p, &x[1], &c_b85, &v[step1], &v[x01]);
    v[17] = drldst_(p, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

/*  ***  IF NECESSARY, SWITCH MODELS  *** */

L410:
    if (iv[12] == 0) {
	goto L420;
    }
    iv[56] = -abs(iv[56]);
    iv[64] += 2;
    l = iv[60];
    dv7cpy_(&c__9, &v[1], &v[l]);
L420:
    l = iv[29] - 4;
    stpmod = iv[5];
    if (l > 0) {
	switch (l) {
	    case 1:  goto L440;
	    case 2:  goto L450;
	    case 3:  goto L460;
	    case 4:  goto L460;
	    case 5:  goto L460;
	    case 6:  goto L460;
	    case 7:  goto L460;
	    case 8:  goto L460;
	    case 9:  goto L570;
	    case 10:  goto L510;
	}
    }

/*  ***  DECIDE WHETHER TO CHANGE MODELS  *** */

    e = v[7] - v[11];
    s1 = iv[62];
    ds7lvm_(ps, &y[1], &v[s1], &v[step1]);
    sttsst = dd7tpr_(ps, &v[step1], &y[1]) * .5;
    if (iv[5] == 1) {
	sttsst = -sttsst;
    }
    if ((d__1 = e + sttsst, abs(d__1)) * v[45] >= abs(e)) {
	goto L430;
    }

/*     ***  SWITCH MODELS  *** */

    iv[5] = 3 - iv[5];
    if (-2 < l) {
	goto L470;
    }
    iv[56] = -abs(iv[56]);
    iv[64] += 2;
    l = iv[60];
    dv7cpy_(&c__9, &v[l], &v[1]);
    goto L230;

L430:
    if (-3 < l) {
	goto L470;
    }

/*     ***  RECOMPUTE STEP WITH DIFFERENT RADIUS  *** */

L440:
    v[8] = v[16] * v[2];
    goto L230;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST */

L450:
    v[8] = v[36];
    goto L270;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L460:
    iv[55] = l;
    if (v[10] >= v[13]) {
	goto L580;
    }
    if (iv[13] == 14) {
	goto L580;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L470:
    iv[26] = 0;
    iv[67] = 0;

/*  ***  SEE WHETHER TO SET V(RADFAC) BY GRADIENT TESTS  *** */

    if (iv[29] != 3) {
	goto L500;
    }
    step1 = iv[40];
    temp1 = step1 + *p;
    temp2 = iv[43];

/*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L480;
    }
    ds7lvm_(p, &v[temp1], &v[hc1], &v[step1]);
    goto L490;
L480:
    rmat1 = iv[78];
    ipiv0 = iv[76];
    dv7cpy_(p, &v[temp1], &v[step1]);
    dv7ipr_(p, &iv[ipiv0], &v[temp1]);
    dl7tvm_(p, &v[temp1], &v[rmat1], &v[temp1]);
    dl7vml_(p, &v[temp1], &v[rmat1], &v[temp1]);
    ipiv1 = iv[58] + *p;
    i7pnvr_(p, &iv[ipiv1], &iv[ipiv0]);
    dv7ipr_(p, &iv[ipiv1], &v[temp1]);

L490:
    if (stpmod == 1) {
	goto L500;
    }
    s1 = iv[62];
    ds7lvm_(ps, &v[temp2], &v[s1], &v[step1]);
    dv2axy_(ps, &v[temp1], &c_b85, &v[temp2], &v[temp1]);

/*  ***  SAVE OLD GRADIENT AND COMPUTE NEW ONE  *** */

L500:
    ++iv[30];
    g01 = iv[65];
    dv7cpy_(p, &v[g01], &g[1]);
    goto L690;

/*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L510:
    g01 = iv[65];
    dv2axy_(p, &v[g01], &c_b90, &v[g01], &g[1]);
    step1 = iv[40];
    temp1 = step1 + *p;
    temp2 = iv[43];
    if (iv[29] != 3) {
	goto L540;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

/*     ***  SET  TEMP1 = D**-1 * (HESSIAN * STEP  +  (G(X0) - G(X)))  *** */

    k = temp1;
    l = g01;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = (v[k] - v[l]) / d__[i__];
	++k;
	++l;
/* L520: */
    }

/*        ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(p, &v[temp1]) <= v[1] * v[29]) {
	goto L530;
    }
    if (dd7tpr_(p, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L540;
    }
L530:
    v[16] = v[23];

/*  ***  COMPUTE Y VECTOR NEEDED FOR UPDATING S  *** */

L540:
    dv2axy_(ps, &y[1], &c_b90, &y[1], &g[1]);

/*  ***  DETERMINE SIZING FACTOR V(SIZE)  *** */

/*     ***  SET TEMP1 = S * STEP  *** */
    s1 = iv[62];
    ds7lvm_(ps, &v[temp1], &v[s1], &v[step1]);

    t1 = (d__1 = dd7tpr_(ps, &v[step1], &v[temp1]), abs(d__1));
    t = (d__1 = dd7tpr_(ps, &v[step1], &y[1]), abs(d__1));
    v[55] = 1.;
    if (t < t1) {
	v[55] = t / t1;
    }

/*  ***  SET G0 TO WCHMTD CHOICE OF FLETCHER AND AL-BAALI  *** */

    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L550;
    }
    ds7lvm_(ps, &v[g01], &v[hc1], &v[step1]);
    goto L560;

L550:
    rmat1 = iv[78];
    ipiv0 = iv[76];
    dv7cpy_(p, &v[g01], &v[step1]);
    i__ = g01 + *ps;
    if (*ps < *p) {
	i__1 = *p - *ps;
	dv7scp_(&i__1, &v[i__], &c_b14);
    }
    dv7ipr_(p, &iv[ipiv0], &v[g01]);
    dl7tvm_(p, &v[g01], &v[rmat1], &v[g01]);
    dl7vml_(p, &v[g01], &v[rmat1], &v[g01]);
    ipiv1 = iv[58] + *p;
    i7pnvr_(p, &iv[ipiv1], &iv[ipiv0]);
    dv7ipr_(p, &iv[ipiv1], &v[g01]);

L560:
    dv2axy_(ps, &v[g01], &c_b85, &y[1], &v[g01]);

/*  ***  UPDATE S  *** */

    ds7lup_(&v[s1], &v[47], ps, &v[55], &v[step1], &v[temp1], &v[temp2], &v[
	    g01], &v[56], &y[1]);
    iv[1] = 2;
    goto L180;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L570:
    iv[1] = 64;
    goto L999;


/*  ***  CONVERGENCE OBTAINED -- SEE WHETHER TO COMPUTE COVARIANCE  *** */

L580:
    if (iv[57] == 0) {
	goto L660;
    }
    if (iv[74] != 0) {
	goto L660;
    }
    if (iv[55] >= 7) {
	goto L660;
    }
    if (iv[67] > 0) {
	goto L660;
    }
    if (iv[26] > 0) {
	goto L660;
    }
    if (abs(iv[15]) >= 3) {
	goto L640;
    }
    if (iv[9] == 0) {
	iv[9] = 2;
    }
    goto L600;

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN FOR COMPUTING COVARIANCE  *** */

L590:
    iv[9] = 0;
L600:
    df7dhb_(&b[3], &d__[1], &g[1], &i__, &iv[1], liv, lv, p, &v[1], &x[1]);
    switch (i__) {
	case 1:  goto L610;
	case 2:  goto L620;
	case 3:  goto L630;
    }
L610:
    ++iv[52];
    ++iv[6];
    iv[1] = 1;
    goto L710;

L620:
    ++iv[53];
    ++iv[30];
    iv[7] = iv[6] + iv[53];
    goto L690;

L630:
    if (iv[55] == 70) {
	goto L120;
    }
    goto L660;

L640:
    h1 = abs(iv[56]);
    iv[74] = h1;
    iv[56] = -h1;
    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L650;
    }
    i__1 = *p * (*p + 1) / 2;
    dv7cpy_(&i__1, &v[h1], &v[hc1]);
    goto L660;
L650:
    rmat1 = iv[78];
    dl7sqr_(p, &v[h1], &v[rmat1]);

L660:
    iv[35] = 0;
    iv[1] = iv[55];
    iv[55] = 0;
    goto L999;

/*  ***  SPECIAL RETURN FOR MISSING HESSIAN INFORMATION -- BOTH */
/*  ***  IV(HC) .LE. 0 AND IV(RMAT) .LE. 0 */

L670:
    iv[1] = 1400;
    goto L999;

/*  ***  INCONSISTENT B  *** */

L680:
    iv[1] = 82;
    goto L999;

/*  *** SAVE, THEN INITIALIZE IPIVOT ARRAY BEFORE COMPUTING G *** */

L690:
    iv[1] = 2;
    j = iv[76];
    ipi = iv[58];
    i7pnvr_(p, &iv[ipi], &iv[j]);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iv[j] = i__;
	++j;
/* L700: */
    }

/*  ***  PROJECT X INTO FEASIBLE REGION (PRIOR TO COMPUTING F OR G)  *** */

L710:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (x[i__] < b[(i__ << 1) + 1]) {
	    x[i__] = b[(i__ << 1) + 1];
	}
	if (x[i__] > b[(i__ << 1) + 2]) {
	    x[i__] = b[(i__ << 1) + 2];
	}
/* L720: */
    }
    iv[2] = 0;

L999:
    return 0;

/*  ***  LAST LINE OF DG7ITB FOLLOWS  *** */
} /* dg7itb_ */

/* Subroutine */ int drnsgb_(doublereal *a, doublereal *alf, doublereal *b, 
	doublereal *c__, doublereal *da, integer *in, integer *iv, integer *l,
	 integer *l1, integer *la, integer *liv, integer *lv, integer *n, 
	integer *nda, integer *p, doublereal *v, doublereal *y)
{
    /* Initialized data */

    static doublereal machep = -1.;
    static doublereal negone = -1.;
    static doublereal sngfac = 100.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer a_dim1, a_offset, da_dim1, da_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer i__, k;
    static doublereal t;
    static integer d1, i1, j1, n1, n2, r1, md, ar1, dr1, rd1, r1l, iv1, ier, 
	    nml, dr1l, jlen, ll1o2, nran, ipiv1;
    extern /* Subroutine */ int drn2gb_(doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, integer 
	    *, integer *, integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    extern doublereal dr7mdc_(integer *);
    static integer csave1;
    extern /* Subroutine */ int dq7apl_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, integer *), dq7rfh_(integer *, 
	    integer *, integer *, integer *, integer *, integer *, doublereal 
	    *, doublereal *, integer *, doublereal *), ds7cpr_(doublereal *, 
	    integer *, integer *, integer *), dv7scp_(integer *, doublereal *,
	     doublereal *), dl7itv_(integer *, doublereal *, doublereal *, 
	    doublereal *), dv7cpy_(integer *, doublereal *, doublereal *);
    extern doublereal dl7svn_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *), dv7prm_(integer *, integer *, 
	    doublereal *);
    extern doublereal dl7svx_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *);
    static doublereal singtl;
    extern /* Subroutine */ int ditsum_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *, doublereal *);


/*  ***  ITERATION DRIVER FOR SEPARABLE NONLINEAR LEAST SQUARES, */
/*  ***  WITH SIMPLE BOUNDS ON THE NONLINEAR VARIABLES. */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION UIPARM(*) */

/*  ***  PURPOSE  *** */

/* GIVEN A SET OF N OBSERVATIONS Y(1)....Y(N) OF A DEPENDENT VARIABLE */
/* T(1)...T(N), DRNSGB ATTEMPTS TO COMPUTE A LEAST SQUARES FIT */
/* TO A FUNCTION  ETA  (THE MODEL) WHICH IS A LINEAR COMBINATION */

/*                  L */
/* ETA(C,ALF,T) =  SUM C * PHI(ALF,T) +PHI   (ALF,T) */
/*                 J=1  J     J           L+1 */

/* OF NONLINEAR FUNCTIONS PHI(J) DEPENDENT ON T AND ALF(1),...,ALF(P) */
/* (.E.G. A SUM OF EXPONENTIALS OR GAUSSIANS).  THAT IS, IT DETERMINES */
/* NONLINEAR PARAMETERS ALF WHICH MINIMIZE */

/*                   2    N                      2 */
/*     NORM(RESIDUAL)  = SUM  (Y - ETA(C,ALF,T )) , */
/*                       I=1    I             I */

/* SUBJECT TO THE SIMPLE BOUND CONSTRAINTS */
/* B(1,I) .LE. ALF(I) .LE. B(2,I), I = 1(1)P. */

/* THE (L+1)ST TERM IS OPTIONAL. */


/*  ***  PARAMETERS  *** */

/*      A (IN)  MATRIX PHI(ALF,T) OF THE MODEL. */
/*    ALF (I/O) NONLINEAR PARAMETERS. */
/*                 INPUT = INITIAL GUESS, */
/*                 OUTPUT = BEST ESTIMATE FOUND. */
/*      C (OUT) LINEAR PARAMETERS (ESTIMATED). */
/*     DA (IN)  DERIVATIVES OF COLUMNS OF A WITH RESPECT TO COMPONENTS */
/*                 OF ALF, AS SPECIFIED BY THE IN ARRAY... */
/*     IN (IN)  WHEN DRNSGB IS CALLED WITH IV(1) = 2 OR -2, THEN FOR */
/*                 I = 1(1)NDA, COLUMN I OF DA IS THE PARTIAL */
/*                 DERIVATIVE WITH RESPECT TO ALF(IN(1,I)) OF COLUMN */
/*                 IN(2,I) OF A, UNLESS IV(1,I) IS NOT POSITIVE (IN */
/*                 WHICH CASE COLUMN I OF DA IS IGNORED.  IV(1) = -2 */
/*                 MEANS THERE ARE MORE COLUMNS OF DA TO COME AND */
/*                 DRNSGB SHOULD RETURN FOR THEM. */
/*     IV (I/O) INTEGER PARAMETER AND SCRATCH VECTOR.  DRNSGB RETURNS */
/*                 WITH IV(1) = 1 WHEN IT WANTS A TO BE EVALUATED AT */
/*                 ALF AND WITH IV(1) = 2 WHEN IT WANTS DA TO BE */
/*                 EVALUATED AT ALF.  WHEN CALLED WITH IV(1) = -2 */
/*                 (AFTER A RETURN WITH IV(1) = 2), DRNSGB RETURNS */
/*                 WITH IV(1) = -2 TO GET MORE COLUMNS OF DA. */
/*      L (IN)  NUMBER OF LINEAR PARAMETERS TO BE ESTIMATED. */
/*     L1 (IN)  L+1 IF PHI(L+1) IS IN THE MODEL, L IF NOT. */
/*     LA (IN)  LEAD DIMENSION OF A.  MUST BE AT LEAST N. */
/*    LIV (IN)  LENGTH OF IV.  MUST BE AT LEAST 110 + L + 4*P. */
/*     LV (IN)  LENGTH OF V.  MUST BE AT LEAST */
/*                 105 + 2*N + L*(L+3)/2 + P*(2*P + 21 + N). */
/*      N (IN)  NUMBER OF OBSERVATIONS. */
/*    NDA (IN)  NUMBER OF COLUMNS IN DA AND IN. */
/*      P (IN)  NUMBER OF NONLINEAR PARAMETERS TO BE ESTIMATED. */
/*      V (I/O) FLOATING-POINT PARAMETER AND SCRATCH VECTOR. */
/*      Y (IN)  RIGHT-HAND SIDE VECTOR. */


/*  ***  EXTERNAL SUBROUTINES  *** */


/* DIVSET.... SUPPLIES DEFAULT PARAMETER VALUES. */
/* DITSUM.... PRINTS ITERATION SUMMARY, INITIAL AND FINAL ALF. */
/* DL7ITV... APPLIES INVERSE-TRANSPOSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DL7SVX... ESTIMATES LARGEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DL7SVN... ESTIMATES SMALLEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DRN2GB... UNDERLYING NONLINEAR LEAST-SQUARES SOLVER. */
/* DQ7APL... APPLIES HOUSEHOLDER TRANSFORMS STORED BY DQ7RFH. */
/* DQ7RFH.... COMPUTES QR FACT. VIA HOUSEHOLDER TRANSFORMS WITH PIVOTING. */
/* DR7MDC... RETURNS MACHINE-DEP. CONSTANTS. */
/* DS7CPR... PRINTS LINEAR PARAMETERS AT SOLUTION. */
/* DV2AXY.... ADDS MULTIPLE OF ONE VECTOR TO ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7PRM.... PERMUTES VECTOR. */
/* DV7SCL... SCALES AND COPIES ONE VECTOR TO ANOTHER. */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */

    /* Parameter adjustments */
    --c__;
    a_dim1 = *la;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --iv;
    --v;
    --y;
    in -= 3;
    da_dim1 = *la;
    da_offset = 1 + da_dim1;
    da -= da_offset;
    b -= 3;
    --alf;

    /* Function Body */

/* ++++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++ */


    if (iv[1] == 0) {
	divset_(&c__1, &iv[1], liv, lv, &v[1]);
    }
    n1 = 1;
    nml = *n;
    iv1 = iv[1];
    if (iv1 <= 2) {
	goto L20;
    }

/*  ***  CHECK INPUT INTEGERS  *** */

    if (*p <= 0) {
	goto L240;
    }
    if (*l < 0) {
	goto L240;
    }
    if (*n <= *l) {
	goto L240;
    }
    if (*la < *n) {
	goto L240;
    }
    if (iv1 < 12) {
	goto L20;
    }
    if (iv1 == 14) {
	goto L20;
    }
    if (iv1 == 12) {
	iv[1] = 13;
    }

/*  ***  FRESH START -- COMPUTE STORAGE REQUIREMENTS  *** */

    if (iv[1] > 16) {
	goto L240;
    }
    ll1o2 = *l * (*l + 1) / 2;
    jlen = *n * *p;
    i__ = *l + *p;
    if (iv[1] != 13) {
	goto L10;
    }
    iv[3] += *l;
    iv[4] = iv[4] + *p + (*n << 1) + jlen + ll1o2 + *l;
L10:
    if (iv[58] <= 110) {
	iv[58] = 111;
    }
    drn2gb_(&b[3], &v[1], &v[1], &iv[1], liv, lv, n, n, &n1, &nml, p, &v[1], &
	    v[1], &v[1], &alf[1]);
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[109] = iv[46];
    iv[46] += *l;
    iv[27] = iv[47];
    iv[82] = iv[27] + *p;
    iv[110] = iv[82] + *n;
    iv[105] = iv[110] + ll1o2;
    iv[70] = iv[105] + *l;
    iv[61] = iv[70] + jlen;
    iv[47] = iv[61] + *n;
    iv[108] = 0;
    if (iv1 == 13) {
	goto L999;
    }

/*  ***  SET POINTERS INTO IV AND V  *** */

L20:
    ar1 = iv[110];
    d1 = iv[27];
    dr1 = iv[70];
    dr1l = dr1 + *l;
    r1 = iv[61];
    r1l = r1 + *l;
    rd1 = iv[82];
    csave1 = iv[105];
    nml = *n - *l;
    if (iv1 <= 2) {
	goto L50;
    }

L30:
    n2 = nml;
    drn2gb_(&b[3], &v[d1], &v[dr1l], &iv[1], liv, lv, &nml, n, &n1, &n2, p, &
	    v[r1l], &v[rd1], &v[1], &alf[1]);
    if ((i__1 = iv[9] - 2, abs(i__1)) == 1 && *l > 0) {
	dv7cpy_(l, &c__[1], &v[csave1]);
    }
    iv1 = iv[1];
    if (iv1 == 2) {
	goto L150;
    }
    if (iv1 > 2) {
	goto L230;
    }

/*  ***  NEW FUNCTION VALUE (RESIDUAL) NEEDED  *** */

    iv[104] = iv[1];
    iv[1] = abs(iv1);
    if (iv[9] == 2 && *l > 0) {
	dv7cpy_(l, &v[csave1], &c__[1]);
    }
    goto L999;

/*  ***  COMPUTE NEW RESIDUAL OR GRADIENT  *** */

L50:
    iv[1] = iv[104];
    md = iv[35];
    if (md <= 0) {
	goto L60;
    }
    nml = *n;
    dr1l = dr1;
    r1l = r1;
L60:
    if (iv[2] != 0) {
	goto L30;
    }
    if (abs(iv1) == 2) {
	goto L170;
    }

/*  ***  COMPUTE NEW RESIDUAL  *** */

    if (*l1 <= *l) {
	dv7cpy_(n, &v[r1], &y[1]);
    }
    if (*l1 > *l) {
	dv2axy_(n, &v[r1], &negone, &a[*l1 * a_dim1 + 1], &y[1]);
    }
    if (md > 0) {
	goto L120;
    }
    ier = 0;
    if (*l <= 0) {
	goto L110;
    }
    ll1o2 = *l * (*l + 1) / 2;
    ipiv1 = iv[109];
    dq7rfh_(&ier, &iv[ipiv1], n, la, &c__0, l, &a[a_offset], &v[ar1], &ll1o2, 
	    &c__[1]);

/* *** DETERMINE NUMERICAL RANK OF A *** */

    if (machep <= zero) {
	machep = dr7mdc_(&c__3);
    }
    singtl = sngfac * (doublereal) max(*l,*n) * machep;
    k = *l;
    if (ier != 0) {
	k = ier - 1;
    }
L70:
    if (k <= 0) {
	goto L90;
    }
    t = dl7svx_(&k, &v[ar1], &c__[1], &c__[1]);
    if (t > zero) {
	t = dl7svn_(&k, &v[ar1], &c__[1], &c__[1]) / t;
    }
    if (t > singtl) {
	goto L80;
    }
    --k;
    goto L70;

/* *** RECORD RANK IN IV(IERS)... IV(IERS) = 0 MEANS FULL RANK, */
/* *** IV(IERS) .GT. 0 MEANS RANK IV(IERS) - 1. */

L80:
    if (k >= *l) {
	goto L100;
    }
L90:
    ier = k + 1;
    i__1 = *l - k;
    dv7scp_(&i__1, &c__[k + 1], &zero);
L100:
    iv[108] = ier;
    if (k <= 0) {
	goto L110;
    }

/* *** APPLY HOUSEHOLDER TRANSFORMATONS TO RESIDUALS... */

    dq7apl_(la, n, &k, &a[a_offset], &v[r1], &ier);

/* *** COMPUTING C NOW MAY SAVE A FUNCTION EVALUATION AT */
/* *** THE LAST ITERATION. */

    dl7itv_(&k, &c__[1], &v[ar1], &v[r1]);
    dv7prm_(l, &iv[ipiv1], &c__[1]);

L110:
    if (iv[1] < 2) {
	goto L220;
    }
    goto L999;


/*  ***  RESIDUAL COMPUTATION FOR F.D. HESSIAN  *** */

L120:
    if (*l <= 0) {
	goto L140;
    }
    i__1 = *l;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L130: */
	d__1 = -c__[i__];
	dv2axy_(n, &v[r1], &d__1, &a[i__ * a_dim1 + 1], &v[r1]);
    }
L140:
    if (iv[1] > 0) {
	goto L30;
    }
    iv[1] = 2;
    goto L160;

/*  ***  NEW GRADIENT (JACOBIAN) NEEDED  *** */

L150:
    iv[104] = iv1;
    if (iv[7] != iv[6]) {
	iv[1] = 1;
    }
L160:
    i__1 = *n * *p;
    dv7scp_(&i__1, &v[dr1], &zero);
    goto L999;

/*  ***  COMPUTE NEW JACOBIAN  *** */

L170:
    if (*nda <= 0) {
	goto L240;
    }
    i__1 = *nda;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i1 = in[(i__ << 1) + 1] - 1;
	if (i1 < 0) {
	    goto L180;
	}
	j1 = in[(i__ << 1) + 2];
	k = dr1 + i1 * *n;
	t = negone;
	if (j1 <= *l) {
	    t = -c__[j1];
	}
	dv2axy_(n, &v[k], &t, &da[i__ * da_dim1 + 1], &v[k]);
L180:
	;
    }
    if (iv1 == 2) {
	goto L190;
    }
    iv[1] = iv1;
    goto L999;
L190:
    if (*l <= 0) {
	goto L30;
    }
    if (md > 0) {
	goto L30;
    }
    k = dr1;
    ier = iv[108];
    nran = *l;
    if (ier > 0) {
	nran = ier - 1;
    }
    if (nran <= 0) {
	goto L210;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dq7apl_(la, n, &nran, &a[a_offset], &v[k], &ier);
	k += *n;
/* L200: */
    }
L210:
    dv7cpy_(l, &v[csave1], &c__[1]);
L220:
    if (ier == 0) {
	goto L30;
    }

/*     *** ADJUST SUBSCRIPTS DESCRIBING R AND DR... */

    nran = ier - 1;
    dr1l = dr1 + nran;
    nml = *n - nran;
    r1l = r1 + nran;
    goto L30;

/*  ***  CONVERGENCE OR LIMIT REACHED  *** */

L230:
    if (iv[67] == 1) {
	iv[67] = rd1;
    }
    if (iv[1] <= 11) {
	ds7cpr_(&c__[1], &iv[1], l, liv);
    }
    goto L999;

L240:
    iv[1] = 66;
    ditsum_(&v[1], &v[1], &iv[1], liv, lv, p, &v[1], &alf[1]);

L999:
    return 0;

/*  ***  LAST CARD OF DRNSGB FOLLOWS  *** */
} /* drnsgb_ */

/* Subroutine */ int ds7lup_(doublereal *a, doublereal *cosmin, integer *p, 
	doublereal *size, doublereal *step, doublereal *u, doublereal *w, 
	doublereal *wchmtd, doublereal *wscale, doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t, ui, wi;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int ds7lvm_(integer *, doublereal *, doublereal *,
	     doublereal *);
    static doublereal denmin, sdotwm;


/*  ***  UPDATE SYMMETRIC  A  SO THAT  A * STEP = Y  *** */
/*  ***  (LOWER TRIANGLE OF  A  STORED ROWWISE       *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION A(P*(P+1)/2) */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */



/* ----------------------------------------------------------------------- */

    /* Parameter adjustments */
    --a;
    --y;
    --wchmtd;
    --w;
    --u;
    --step;

    /* Function Body */
    sdotwm = dd7tpr_(p, &step[1], &wchmtd[1]);
    denmin = *cosmin * dv2nrm_(p, &step[1]) * dv2nrm_(p, &wchmtd[1]);
    *wscale = 1.;
    if (denmin != 0.) {
/* Computing MIN */
	d__2 = 1., d__3 = (d__1 = sdotwm / denmin, abs(d__1));
	*wscale = min(d__2,d__3);
    }
    t = 0.;
    if (sdotwm != 0.) {
	t = *wscale / sdotwm;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	w[i__] = t * wchmtd[i__];
    }
    ds7lvm_(p, &u[1], &a[1], &step[1]);
    t = (*size * dd7tpr_(p, &step[1], &u[1]) - dd7tpr_(p, &step[1], &y[1])) * 
	    .5;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L20: */
	u[i__] = t * w[i__] + y[i__] - *size * u[i__];
    }

/*  ***  SET  A = A + U*(W**T) + W*(U**T)  *** */

    k = 1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ui = u[i__];
	wi = w[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    a[k] = *size * a[k] + ui * w[j] + wi * u[j];
	    ++k;
/* L30: */
	}
/* L40: */
    }

    return 0;
/*  ***  LAST CARD OF DS7LUP FOLLOWS  *** */
} /* ds7lup_ */

/* Subroutine */ int dl7mst_(doublereal *d__, doublereal *g, integer *ierr, 
	integer *ipivot, integer *ka, integer *p, doublereal *qtr, doublereal 
	*r__, doublereal *step, doublereal *v, doublereal *w)
{
    /* Initialized data */

    static doublereal big = 0.;

    /* System generated locals */
    integer i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal a, b;
    static integer i__, k, l;
    static doublereal t, d1, d2;
    static integer i1, j1;
    static doublereal lk, si, sj, uk, wl;
    static integer lk0, ip1, uk0;
    static doublereal adi, rad, phi;
    static integer res;
    static doublereal dst;
    static integer res0, pp1o2, rmat;
    static doublereal dtol;
    static integer rmat0, kalim;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dl7ivm_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
	     doublereal *), dv7cpy_(integer *, doublereal *, doublereal *);
    extern doublereal dl7svn_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    static doublereal alphak, dfacsq, psifac, oldphi, phimin, phimax;
    static integer phipin, dstsav;
    static doublereal sqrtak, twopsi;


/*  ***  COMPUTE LEVENBERG-MARQUARDT STEP USING MORE-HEBDEN TECHNIQUE  ** */
/*  ***  NL2SOL VERSION 2.2.  *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION W(P*(P+5)/2 + 4) */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  PURPOSE  *** */

/*        GIVEN THE R MATRIX FROM THE QR DECOMPOSITION OF A JACOBIAN */
/*     MATRIX, J, AS WELL AS Q-TRANSPOSE TIMES THE CORRESPONDING */
/*     RESIDUAL VECTOR, RESID, THIS SUBROUTINE COMPUTES A LEVENBERG- */
/*     MARQUARDT STEP OF APPROXIMATE LENGTH V(RADIUS) BY THE MORE- */
/*     TECHNIQUE. */

/*  ***  PARAMETER DESCRIPTION  *** */

/*      D (IN)  = THE SCALE VECTOR. */
/*      G (IN)  = THE GRADIENT VECTOR (J**T)*R. */
/*   IERR (I/O) = RETURN CODE FROM QRFACT OR DQ7RGS -- 0 MEANS R HAS */
/*             FULL RANK. */
/* IPIVOT (I/O) = PERMUTATION ARRAY FROM QRFACT OR DQ7RGS, WHICH COMPUTE */
/*             QR DECOMPOSITIONS WITH COLUMN PIVOTING. */
/*     KA (I/O).  KA .LT. 0 ON INPUT MEANS THIS IS THE FIRST CALL ON */
/*             DL7MST FOR THE CURRENT R AND QTR.  ON OUTPUT KA CON- */
/*             TAINS THE NUMBER OF HEBDEN ITERATIONS NEEDED TO DETERMINE */
/*             STEP.  KA = 0 MEANS A GAUSS-NEWTON STEP. */
/*      P (IN)  = NUMBER OF PARAMETERS. */
/*    QTR (IN)  = (Q**T)*RESID = Q-TRANSPOSE TIMES THE RESIDUAL VECTOR. */
/*      R (IN)  = THE R MATRIX, STORED COMPACTLY BY COLUMNS. */
/*   STEP (OUT) = THE LEVENBERG-MARQUARDT STEP COMPUTED. */
/*      V (I/O) CONTAINS VARIOUS CONSTANTS AND VARIABLES DESCRIBED BELOW. */
/*      W (I/O) = WORKSPACE OF LENGTH P*(P+5)/2 + 4. */

/*  ***  ENTRIES IN V  *** */

/* V(DGNORM) (I/O) = 2-NORM OF (D**-1)*G. */
/* V(DSTNRM) (I/O) = 2-NORM OF D*STEP. */
/* V(DST0)   (I/O) = 2-NORM OF GAUSS-NEWTON STEP (FOR NONSING. J). */
/* V(EPSLON) (IN) = MAX. REL. ERROR ALLOWED IN TWONORM(R)**2 MINUS */
/*             TWONORM(R - J*STEP)**2.  (SEE ALGORITHM NOTES BELOW.) */
/* V(GTSTEP) (OUT) = INNER PRODUCT BETWEEN G AND STEP. */
/* V(NREDUC) (OUT) = HALF THE REDUCTION IN THE SUM OF SQUARES PREDICTED */
/*             FOR A GAUSS-NEWTON STEP. */
/* V(PHMNFC) (IN)  = TOL. (TOGETHER WITH V(PHMXFC)) FOR ACCEPTING STEP */
/*             (MORE*S SIGMA).  THE ERROR V(DSTNRM) - V(RADIUS) MUST LIE */
/*             BETWEEN V(PHMNFC)*V(RADIUS) AND V(PHMXFC)*V(RADIUS). */
/* V(PHMXFC) (IN)  (SEE V(PHMNFC).) */
/* V(PREDUC) (OUT) = HALF THE REDUCTION IN THE SUM OF SQUARES PREDICTED */
/*             BY THE STEP RETURNED. */
/* V(RADIUS) (IN)  = RADIUS OF CURRENT (SCALED) TRUST REGION. */
/* V(RAD0)   (I/O) = VALUE OF V(RADIUS) FROM PREVIOUS CALL. */
/* V(STPPAR) (I/O) = MARQUARDT PARAMETER (OR ITS NEGATIVE IF THE SPECIAL */
/*             CASE MENTIONED BELOW IN THE ALGORITHM NOTES OCCURS). */

/* NOTE -- SEE DATA STATEMENT BELOW FOR VALUES OF ABOVE SUBSCRIPTS. */

/*  ***  USAGE NOTES  *** */

/*     IF IT IS DESIRED TO RECOMPUTE STEP USING A DIFFERENT VALUE OF */
/*     V(RADIUS), THEN THIS ROUTINE MAY BE RESTARTED BY CALLING IT */
/*     WITH ALL PARAMETERS UNCHANGED EXCEPT V(RADIUS).  (THIS EXPLAINS */
/*     WHY MANY PARAMETERS ARE LISTED AS I/O).  ON AN INTIIAL CALL (ONE */
/*     WITH KA = -1), THE CALLER NEED ONLY HAVE INITIALIZED D, G, KA, P, */
/*     QTR, R, V(EPSLON), V(PHMNFC), V(PHMXFC), V(RADIUS), AND V(RAD0). */

/*  ***  APPLICATION AND USAGE RESTRICTIONS  *** */

/*     THIS ROUTINE IS CALLED AS PART OF THE NL2SOL (NONLINEAR LEAST- */
/*     SQUARES) PACKAGE (REF. 1). */

/*  ***  ALGORITHM NOTES  *** */

/*     THIS CODE IMPLEMENTS THE STEP COMPUTATION SCHEME DESCRIBED IN */
/*     REFS. 2 AND 4.  FAST GIVENS TRANSFORMATIONS (SEE REF. 3, PP. 60- */
/*     62) ARE USED TO COMPUTE STEP WITH A NONZERO MARQUARDT PARAMETER. */
/*        A SPECIAL CASE OCCURS IF J IS (NEARLY) SINGULAR AND V(RADIUS) */
/*     IS SUFFICIENTLY LARGE.  IN THIS CASE THE STEP RETURNED IS SUCH */
/*     THAT  TWONORM(R)**2 - TWONORM(R - J*STEP)**2  DIFFERS FROM ITS */
/*     OPTIMAL VALUE BY LESS THAN V(EPSLON) TIMES THIS OPTIMAL VALUE, */
/*     WHERE J AND R DENOTE THE ORIGINAL JACOBIAN AND RESIDUAL.  (SEE */
/*     REF. 2 FOR MORE DETAILS.) */

/*  ***  FUNCTIONS AND SUBROUTINES CALLED  *** */

/* DD7TPR - RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DL7ITV - APPLY INVERSE-TRANSPOSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DL7IVM - APPLY INVERSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DV7CPY  - COPIES ONE VECTOR TO ANOTHER. */
/* DV2NRM - RETURNS 2-NORM OF A VECTOR. */

/*  ***  REFERENCES  *** */

/* 1.  DENNIS, J.E., GAY, D.M., AND WELSCH, R.E. (1981), AN ADAPTIVE */
/*             NONLINEAR LEAST-SQUARES ALGORITHM, ACM TRANS. MATH. */
/*             SOFTWARE, VOL. 7, NO. 3. */
/* 2.  GAY, D.M. (1981), COMPUTING OPTIMAL LOCALLY CONSTRAINED STEPS, */
/*             SIAM J. SCI. STATIST. COMPUTING, VOL. 2, NO. 2, PP. */
/*             186-197. */
/* 3.  LAWSON, C.L., AND HANSON, R.J. (1974), SOLVING LEAST SQUARES */
/*             PROBLEMS, PRENTICE-HALL, ENGLEWOOD CLIFFS, N.J. */
/* 4.  MORE, J.J. (1978), THE LEVENBERG-MARQUARDT ALGORITHM, IMPLEMEN- */
/*             TATION AND THEORY, PP.105-116 OF SPRINGER LECTURE NOTES */
/*             IN MATHEMATICS NO. 630, EDITED BY G.A. WATSON, SPRINGER- */
/*             VERLAG, BERLIN AND NEW YORK. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, MCS76-11989, AND */
/*     MCS-7906671. */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/*  ***  SUBSCRIPTS FOR V  *** */


    /* Parameter adjustments */
    --step;
    --qtr;
    --ipivot;
    --g;
    --d__;
    --r__;
    --v;
    --w;

    /* Function Body */

/*  ***  BODY  *** */

/*     ***  FOR USE IN RECOMPUTING STEP, THE FINAL VALUES OF LK AND UK, */
/*     ***  THE INVERSE DERIVATIVE OF MORE*S PHI AT 0 (FOR NONSING. J) */
/*     ***  AND THE VALUE RETURNED AS V(DSTNRM) ARE STORED AT W(LK0), */
/*     ***  W(UK0), W(PHIPIN), AND W(DSTSAV) RESPECTIVELY. */
    lk0 = *p + 1;
    phipin = lk0 + 1;
    uk0 = phipin + 1;
    dstsav = uk0 + 1;
    rmat0 = dstsav;
/*     ***  A COPY OF THE R-MATRIX FROM THE QR DECOMPOSITION OF J IS */
/*     ***  STORED IN W STARTING AT W(RMAT), AND A COPY OF THE RESIDUAL */
/*     ***  VECTOR IS STORED IN W STARTING AT W(RES).  THE LOOPS BELOW */
/*     ***  THAT UPDATE THE QR DECOMP. FOR A NONZERO MARQUARDT PARAMETER */
/*     ***  WORK ON THESE COPIES. */
    rmat = rmat0 + 1;
    pp1o2 = *p * (*p + 1) / 2;
    res0 = pp1o2 + rmat0;
    res = res0 + 1;
    rad = v[8];
    if (rad > 0.) {
/* Computing 2nd power */
	d__1 = rad;
	psifac = v[19] / (((v[20] + 1.) * 8. + 3.) * (d__1 * d__1));
    }
    if (big <= 0.) {
	big = dr7mdc_(&c__6);
    }
    phimax = v[21] * rad;
    phimin = v[20] * rad;
/*     ***  DTOL, DFAC, AND DFACSQ ARE USED IN RESCALING THE FAST GIVENS */
/*     ***  REPRESENTATION OF THE UPDATED QR DECOMPOSITION. */
    dtol = .00390625;
    dfacsq = 65536.;
/*     ***  OLDPHI IS USED TO DETECT LIMITS OF NUMERICAL ACCURACY.  IF */
/*     ***  WE RECOMPUTE STEP AND IT DOES NOT CHANGE, THEN WE ACCEPT IT. */
    oldphi = 0.;
    lk = 0.;
    uk = 0.;
    kalim = *ka + 12;

/*  ***  START OR RESTART, DEPENDING ON KA  *** */

    if (*ka == 0) {
	goto L20;
    }
    if (*ka > 0) {
	goto L370;
    }

/*  ***  FRESH START -- COMPUTE V(NREDUC)  *** */

    *ka = 0;
    kalim = 12;
    k = *p;
    if (*ierr != 0) {
	k = abs(*ierr) - 1;
    }
    v[6] = dd7tpr_(&k, &qtr[1], &qtr[1]) * .5;

/*  ***  SET UP TO TRY INITIAL GAUSS-NEWTON STEP  *** */

L20:
    v[3] = -1.;
    if (*ierr != 0) {
	goto L90;
    }
    t = dl7svn_(p, &r__[1], &step[1], &w[res]);
    if (t >= 1.) {
	goto L30;
    }
    if (dv2nrm_(p, &qtr[1]) >= big * t) {
	goto L90;
    }

/*  ***  COMPUTE GAUSS-NEWTON STEP  *** */

/*     ***  NOTE -- THE R-MATRIX IS STORED COMPACTLY BY COLUMNS IN */
/*     ***  R(1), R(2), R(3), ...  IT IS THE TRANSPOSE OF A */
/*     ***  LOWER TRIANGULAR MATRIX STORED COMPACTLY BY ROWS, AND WE */
/*     ***  TREAT IT AS SUCH WHEN USING DL7ITV AND DL7IVM. */
L30:
    dl7itv_(p, &w[1], &r__[1], &qtr[1]);
/*     ***  TEMPORARILY STORE PERMUTED -D*STEP IN STEP. */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j1 = ipivot[i__];
	step[i__] = d__[j1] * w[i__];
/* L60: */
    }
    dst = dv2nrm_(p, &step[1]);
    v[3] = dst;
    phi = dst - rad;
    if (phi <= phimax) {
	goto L410;
    }
/*     ***  IF THIS IS A RESTART, GO TO 110  *** */
    if (*ka > 0) {
	goto L110;
    }

/*  ***  GAUSS-NEWTON STEP WAS UNACCEPTABLE.  COMPUTE L0  *** */

    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j1 = ipivot[i__];
	step[i__] = d__[j1] * (step[i__] / dst);
/* L70: */
    }
    dl7ivm_(p, &step[1], &r__[1], &step[1]);
    t = 1. / dv2nrm_(p, &step[1]);
    w[phipin] = t / rad * t;
    lk = phi * w[phipin];

/*  ***  COMPUTE U0  *** */

L90:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L100: */
	w[i__] = g[i__] / d__[i__];
    }
    v[1] = dv2nrm_(p, &w[1]);
    uk = v[1] / rad;
    if (uk <= 0.) {
	goto L390;
    }

/*     ***  ALPHAK WILL BE USED AS THE CURRENT MARQUARDT PARAMETER.  WE */
/*     ***  USE MORE*S SCHEME FOR INITIALIZING IT. */

    alphak = abs(v[5]) * v[9] / rad;
/* Computing MIN */
    d__1 = uk, d__2 = max(alphak,lk);
    alphak = min(d__1,d__2);


/*  ***  TOP OF LOOP -- INCREMENT KA, COPY R TO RMAT, QTR TO RES  *** */

L110:
    ++(*ka);
    dv7cpy_(&pp1o2, &w[rmat], &r__[1]);
    dv7cpy_(p, &w[res], &qtr[1]);

/*  ***  SAFEGUARD ALPHAK AND INITIALIZE FAST GIVENS SCALE VECTOR.  *** */

    if (alphak <= 0. || alphak < lk || alphak >= uk) {
/* Computing MAX */
	d__1 = .001, d__2 = sqrt(lk / uk);
	alphak = uk * max(d__1,d__2);
    }
    if (alphak <= 0.) {
	alphak = uk * .5;
    }
    sqrtak = sqrt(alphak);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L120: */
	w[i__] = 1.;
    }

/*  ***  ADD ALPHAK*D AND UPDATE QR DECOMP. USING FAST GIVENS TRANS.  *** */

    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*        ***  GENERATE, APPLY 1ST GIVENS TRANS. FOR ROW I OF ALPHAK*D. */
/*        ***  (USE STEP TO STORE TEMPORARY ROW)  *** */
	l = i__ * (i__ + 1) / 2 + rmat0;
	wl = w[l];
	d2 = 1.;
	d1 = w[i__];
	j1 = ipivot[i__];
	adi = sqrtak * d__[j1];
	if (adi >= abs(wl)) {
	    goto L150;
	}
L130:
	a = adi / wl;
	b = d2 * a / d1;
	t = a * b + 1.;
	if (t > 2.5) {
	    goto L150;
	}
	w[i__] = d1 / t;
	d2 /= t;
	w[l] = t * wl;
	a = -a;
	i__2 = *p;
	for (j1 = i__; j1 <= i__2; ++j1) {
	    l += j1;
	    step[j1] = a * w[l];
/* L140: */
	}
	goto L170;

L150:
	b = wl / adi;
	a = d1 * b / d2;
	t = a * b + 1.;
	if (t > 2.5) {
	    goto L130;
	}
	w[i__] = d2 / t;
	d2 = d1 / t;
	w[l] = t * adi;
	i__2 = *p;
	for (j1 = i__; j1 <= i__2; ++j1) {
	    l += j1;
	    wl = w[l];
	    step[j1] = -wl;
	    w[l] = a * wl;
/* L160: */
	}

L170:
	if (i__ == *p) {
	    goto L280;
	}

/*        ***  NOW USE GIVENS TRANS. TO ZERO ELEMENTS OF TEMP. ROW  *** */

	ip1 = i__ + 1;
	i__2 = *p;
	for (i1 = ip1; i1 <= i__2; ++i1) {
	    si = step[i1 - 1];
	    if (si == 0.) {
		goto L260;
	    }
	    l = i1 * (i1 + 1) / 2 + rmat0;
	    wl = w[l];
	    d1 = w[i1];

/*             ***  RESCALE ROW I1 IF NECESSARY  *** */

	    if (d1 >= dtol) {
		goto L190;
	    }
	    d1 *= dfacsq;
	    wl /= 256.;
	    k = l;
	    i__3 = *p;
	    for (j1 = i1; j1 <= i__3; ++j1) {
		k += j1;
		w[k] /= 256.;
/* L180: */
	    }

/*             ***  USE GIVENS TRANS. TO ZERO NEXT ELEMENT OF TEMP. ROW */

L190:
	    if (abs(si) > abs(wl)) {
		goto L220;
	    }
L200:
	    a = si / wl;
	    b = d2 * a / d1;
	    t = a * b + 1.;
	    if (t > 2.5) {
		goto L220;
	    }
	    w[l] = t * wl;
	    w[i1] = d1 / t;
	    d2 /= t;
	    i__3 = *p;
	    for (j1 = i1; j1 <= i__3; ++j1) {
		l += j1;
		wl = w[l];
		sj = step[j1];
		w[l] = wl + b * sj;
		step[j1] = sj - a * wl;
/* L210: */
	    }
	    goto L240;

L220:
	    b = wl / si;
	    a = d1 * b / d2;
	    t = a * b + 1.;
	    if (t > 2.5) {
		goto L200;
	    }
	    w[i1] = d2 / t;
	    d2 = d1 / t;
	    w[l] = t * si;
	    i__3 = *p;
	    for (j1 = i1; j1 <= i__3; ++j1) {
		l += j1;
		wl = w[l];
		sj = step[j1];
		w[l] = a * wl + sj;
		step[j1] = b * sj - wl;
/* L230: */
	    }

/*             ***  RESCALE TEMP. ROW IF NECESSARY  *** */

L240:
	    if (d2 >= dtol) {
		goto L260;
	    }
	    d2 *= dfacsq;
	    i__3 = *p;
	    for (k = i1; k <= i__3; ++k) {
/* L250: */
		step[k] /= 256.;
	    }
L260:
	    ;
	}
/* L270: */
    }

/*  ***  COMPUTE STEP  *** */

L280:
    dl7itv_(p, &w[res], &w[rmat], &w[res]);
/*     ***  RECOVER STEP AND STORE PERMUTED -D*STEP AT W(RES)  *** */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j1 = ipivot[i__];
	k = res0 + i__;
	t = w[k];
	step[j1] = -t;
	w[k] = t * d__[j1];
/* L290: */
    }
    dst = dv2nrm_(p, &w[res]);
    phi = dst - rad;
    if (phi <= phimax && phi >= phimin) {
	goto L430;
    }
    if (oldphi == phi) {
	goto L430;
    }
    oldphi = phi;

/*  ***  CHECK FOR (AND HANDLE) SPECIAL CASE  *** */

    if (phi > 0.) {
	goto L310;
    }
    if (*ka >= kalim) {
	goto L430;
    }
    twopsi = alphak * dst * dst - dd7tpr_(p, &step[1], &g[1]);
    if (alphak >= twopsi * psifac) {
	goto L310;
    }
    v[5] = -alphak;
    goto L440;

/*  ***  UNACCEPTABLE STEP -- UPDATE LK, UK, ALPHAK, AND TRY AGAIN  *** */

L300:
    if (phi < 0.) {
	uk = min(uk,alphak);
    }
    goto L320;
L310:
    if (phi < 0.) {
	uk = alphak;
    }
L320:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j1 = ipivot[i__];
	k = res0 + i__;
	step[i__] = d__[j1] * (w[k] / dst);
/* L330: */
    }
    dl7ivm_(p, &step[1], &w[rmat], &step[1]);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L340: */
	step[i__] /= sqrt(w[i__]);
    }
    t = 1. / dv2nrm_(p, &step[1]);
    alphak += t * phi * t / rad;
    lk = max(lk,alphak);
    alphak = lk;
    goto L110;

/*  ***  RESTART  *** */

L370:
    lk = w[lk0];
    uk = w[uk0];
    if (v[3] > 0. && v[3] - rad <= phimax) {
	goto L20;
    }
    alphak = abs(v[5]);
    dst = w[dstsav];
    phi = dst - rad;
    t = v[1] / rad;
    if (rad > v[9]) {
	goto L380;
    }

/*        ***  SMALLER RADIUS  *** */
    uk = t;
    if (alphak <= 0.) {
	lk = 0.;
    }
    if (v[3] > 0.) {
/* Computing MAX */
	d__1 = lk, d__2 = (v[3] - rad) * w[phipin];
	lk = max(d__1,d__2);
    }
    goto L300;

/*     ***  BIGGER RADIUS  *** */
L380:
    if (alphak <= 0. || uk > t) {
	uk = t;
    }
    lk = 0.;
    if (v[3] > 0.) {
/* Computing MAX */
	d__1 = lk, d__2 = (v[3] - rad) * w[phipin];
	lk = max(d__1,d__2);
    }
    goto L300;

/*  ***  SPECIAL CASE -- RAD .LE. 0 OR (G = 0 AND J IS SINGULAR)  *** */

L390:
    v[5] = 0.;
    dst = 0.;
    lk = 0.;
    uk = 0.;
    v[4] = 0.;
    v[7] = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L400: */
	step[i__] = 0.;
    }
    goto L450;

/*  ***  ACCEPTABLE GAUSS-NEWTON STEP -- RECOVER STEP FROM W  *** */

L410:
    alphak = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j1 = ipivot[i__];
	step[j1] = -w[i__];
/* L420: */
    }

/*  ***  SAVE VALUES FOR USE IN A POSSIBLE RESTART  *** */

L430:
    v[5] = alphak;
L440:
/* Computing MIN */
    d__1 = dd7tpr_(p, &step[1], &g[1]);
    v[4] = min(d__1,0.);
    v[7] = (alphak * dst * dst - v[4]) * .5;
L450:
    v[2] = dst;
    w[dstsav] = dst;
    w[lk0] = lk;
    w[uk0] = uk;
    v[9] = rad;

    return 0;

/*  ***  LAST CARD OF DL7MST FOLLOWS  *** */
} /* dl7mst_ */

/* Subroutine */ int drmnfb_(doublereal *b, doublereal *d__, doublereal *fx, 
	integer *iv, integer *liv, integer *lv, integer *p, doublereal *v, 
	doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, w, g1, iv1, ipi, alpha, alpha0;
    extern /* Subroutine */ int ds3grd_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , drmngb_(doublereal *, doublereal *, doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *), divset_(integer *, integer *, integer *, integer *,
	     doublereal *);


/*  ***  ITERATION DRIVER FOR  DMNF... */
/*  ***  MINIMIZE GENERAL UNCONSTRAINED OBJECTIVE FUNCTION USING */
/*  ***  FINITE-DIFFERENCE GRADIENTS AND SECANT HESSIAN APPROXIMATIONS. */

/*     DIMENSION IV(59 + P), V(77 + P*(P+23)/2) */

/*  ***  PURPOSE  *** */

/*        THIS ROUTINE INTERACTS WITH SUBROUTINE  DRMNGB  IN AN ATTEMPT */
/*     TO FIND AN P-VECTOR  X*  THAT MINIMIZES THE (UNCONSTRAINED) */
/*     OBJECTIVE FUNCTION  FX = F(X)  COMPUTED BY THE CALLER.  (OFTEN */
/*     THE  X*  FOUND IS A LOCAL MINIMIZER RATHER THAN A GLOBAL ONE.) */

/*  ***  PARAMETERS  *** */

/*        THE PARAMETERS FOR DRMNFB ARE THE SAME AS THOSE FOR  DMNG */
/*     (WHICH SEE), EXCEPT THAT CALCF, CALCG, UIPARM, URPARM, AND UFPARM */
/*     ARE OMITTED, AND A PARAMETER  FX  FOR THE OBJECTIVE FUNCTION */
/*     VALUE AT X IS ADDED.  INSTEAD OF CALLING CALCG TO OBTAIN THE */
/*     GRADIENT OF THE OBJECTIVE FUNCTION AT X, DRMNFB CALLS DS3GRD, */
/*     WHICH COMPUTES AN APPROXIMATION TO THE GRADIENT BY FINITE */
/*     (FORWARD AND CENTRAL) DIFFERENCES USING THE METHOD OF REF. 1. */
/*     THE FOLLOWING INPUT COMPONENT IS OF INTEREST IN THIS REGARD */
/*     (AND IS NOT DESCRIBED IN  DMNG). */

/* V(ETA0)..... V(42) IS AN ESTIMATED BOUND ON THE RELATIVE ERROR IN THE */
/*             OBJECTIVE FUNCTION VALUE COMPUTED BY CALCF... */
/*                  (TRUE VALUE) = (COMPUTED VALUE) * (1 + E), */
/*             WHERE ABS(E) .LE. V(ETA0).  DEFAULT = MACHEP * 10**3, */
/*             WHERE MACHEP IS THE UNIT ROUNDOFF. */

/*        THE OUTPUT VALUES IV(NFCALL) AND IV(NGCALL) HAVE DIFFERENT */
/*     MEANINGS FOR  DMNF THAN FOR  DMNG... */

/* IV(NFCALL)... IV(6) IS THE NUMBER OF CALLS SO FAR MADE ON CALCF (I.E., */
/*             FUNCTION EVALUATIONS) EXCLUDING THOSE MADE ONLY FOR */
/*             COMPUTING GRADIENTS.  THE INPUT VALUE IV(MXFCAL) IS A */
/*             LIMIT ON IV(NFCALL). */
/* IV(NGCALL)... IV(30) IS THE NUMBER OF FUNCTION EVALUATIONS MADE ONLY */
/*             FOR COMPUTING GRADIENTS.  THE TOTAL NUMBER OF FUNCTION */
/*             EVALUATIONS IS THUS  IV(NFCALL) + IV(NGCALL). */

/*  ***  REFERENCES  *** */

/* 1. STEWART, G.W. (1967), A MODIFICATION OF DAVIDON*S MINIMIZATION */
/*        METHOD TO ACCEPT DIFFERENCE APPROXIMATIONS OF DERIVATIVES, */
/*        J. ASSOC. COMPUT. MACH. 14, PP. 72-83. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (AUGUST 1982). */

/* ----------------------------  DECLARATIONS  --------------------------- */


/* DIVSET.... SUPPLIES DEFAULT PARAMETER VALUES. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DS3GRD... COMPUTES FINITE-DIFFERENCE GRADIENT APPROXIMATION. */
/* DRMNGB... REVERSE-COMMUNICATION ROUTINE THAT DOES  DMNGB ALGORITHM. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */


/*  ***  SUBSCRIPTS FOR IV   *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --d__;
    b -= 3;

    /* Function Body */
    iv1 = iv[1];
    if (iv1 == 1) {
	goto L10;
    }
    if (iv1 == 2) {
	goto L50;
    }
    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    iv1 = iv[1];
    if (iv1 == 12 || iv1 == 13) {
	iv[4] = iv[4] + (*p << 1) + 6;
    }
    if (iv1 == 14) {
	goto L10;
    }
    if (iv1 > 2 && iv1 < 12) {
	goto L10;
    }
    g1 = 1;
    if (iv1 == 12) {
	iv[1] = 13;
    }
    goto L20;

L10:
    g1 = iv[28];

L20:
    drmngb_(&b[3], &d__[1], fx, &v[g1], &iv[1], liv, lv, p, &v[1], &x[1]);
    if (iv[1] < 2) {
	goto L999;
    }
    if (iv[1] > 2) {
	goto L80;
    }

/*  ***  COMPUTE GRADIENT  *** */

    if (iv[31] == 0) {
	dv7scp_(p, &v[g1], &c_b14);
    }
    j = iv[42];
    alpha0 = g1 - *p - 1;
    ipi = iv[58];
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = alpha0 + iv[ipi];
	v[k] = dd7tpr_(&i__, &v[j], &v[j]);
	++ipi;
	j += i__;
/* L40: */
    }
/*     ***  UNDO INCREMENT OF IV(NGCALL) DONE BY DRMNGB  *** */
    --iv[30];
/*     ***  STORE RETURN CODE FROM DS3GRD IN IV(SGIRC)  *** */
    iv[57] = 0;
/*     ***  X MAY HAVE BEEN RESTORED, SO COPY BACK FX... *** */
    *fx = v[10];
    goto L60;

/*     ***  GRADIENT LOOP  *** */

L50:
    if (iv[2] != 0) {
	goto L10;
    }

L60:
    g1 = iv[28];
    alpha = g1 - *p;
    w = alpha - 6;
    ds3grd_(&v[alpha], &b[3], &d__[1], &v[42], fx, &v[g1], &iv[57], p, &v[w], 
	    &x[1]);
    i__ = iv[57];
    if (i__ == 0) {
	goto L10;
    }
    if (i__ <= *p) {
	goto L70;
    }
    iv[2] = 1;
    goto L10;

L70:
    ++iv[30];
    goto L999;

L80:
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[28] = iv[47] + *p + 6;
    iv[47] = iv[28] + *p;
    if (iv1 != 13) {
	goto L10;
    }

L999:
    return 0;
/*  ***  LAST CARD OF DRMNFB FOLLOWS  *** */
} /* drmnfb_ */

/* Subroutine */ int d7egr_(integer *n, integer *indrow, integer *jpntr, 
	integer *indcol, integer *ipntr, integer *ndeg, integer *iwa, logical 
	*bwa)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer ic, ip, jp, ir, deg, ipl, jpl, ipu, jpu, jcol;

/*     ********** */

/*     SUBROUTINE D7EGR */

/*     GIVEN THE SPARSITY PATTERN OF AN M BY N MATRIX A, */
/*     THIS SUBROUTINE DETERMINES THE DEGREE SEQUENCE FOR */
/*     THE INTERSECTION GRAPH OF THE COLUMNS OF A. */

/*     IN GRAPH-THEORY TERMINOLOGY, THE INTERSECTION GRAPH OF */
/*     THE COLUMNS OF A IS THE LOOPLESS GRAPH G WITH VERTICES */
/*     A(J), J = 1,2,...,N WHERE A(J) IS THE J-TH COLUMN OF A */
/*     AND WITH EDGE (A(I),A(J)) IF AND ONLY IF COLUMNS I AND J */
/*     HAVE A NON-ZERO IN THE SAME ROW POSITION. */

/*     NOTE THAT THE VALUE OF M IS NOT NEEDED BY D7EGR AND IS */
/*     THEREFORE NOT PRESENT IN THE SUBROUTINE STATEMENT. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE D7EGR(N,INDROW,JPNTR,INDCOL,IPNTR,NDEG,IWA,BWA) */

/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       INDROW IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE ROW */
/*         INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       JPNTR IS AN INTEGER INPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       INDCOL IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE */
/*         COLUMN INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       IPNTR IS AN INTEGER INPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(M+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       NDEG IS AN INTEGER OUTPUT ARRAY OF LENGTH N WHICH */
/*         SPECIFIES THE DEGREE SEQUENCE. THE DEGREE OF THE */
/*         J-TH COLUMN OF A IS NDEG(J). */

/*       IWA IS AN INTEGER WORK ARRAY OF LENGTH N. */

/*       BWA IS A LOGICAL WORK ARRAY OF LENGTH N. */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     INITIALIZATION BLOCK. */

    /* Parameter adjustments */
    --bwa;
    --iwa;
    --ndeg;
    --indrow;
    --jpntr;
    --indcol;
    --ipntr;

    /* Function Body */
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	ndeg[jp] = 0;
	bwa[jp] = FALSE_;
/* L10: */
    }

/*     COMPUTE THE DEGREE SEQUENCE BY DETERMINING THE CONTRIBUTIONS */
/*     TO THE DEGREES FROM THE CURRENT(JCOL) COLUMN AND FURTHER */
/*     COLUMNS WHICH HAVE NOT YET BEEN CONSIDERED. */

    if (*n < 2) {
	goto L90;
    }
    i__1 = *n;
    for (jcol = 2; jcol <= i__1; ++jcol) {
	bwa[jcol] = TRUE_;
	deg = 0;

/*        DETERMINE ALL POSITIONS (IR,JCOL) WHICH CORRESPOND */
/*        TO NON-ZEROES IN THE MATRIX. */

	jpl = jpntr[jcol];
	jpu = jpntr[jcol + 1] - 1;
	if (jpu < jpl) {
	    goto L50;
	}
	i__2 = jpu;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];

/*           FOR EACH ROW IR, DETERMINE ALL POSITIONS (IR,IC) */
/*           WHICH CORRESPOND TO NON-ZEROES IN THE MATRIX. */

	    ipl = ipntr[ir];
	    ipu = ipntr[ir + 1] - 1;
	    i__3 = ipu;
	    for (ip = ipl; ip <= i__3; ++ip) {
		ic = indcol[ip];

/*              ARRAY BWA MARKS COLUMNS WHICH HAVE CONTRIBUTED TO */
/*              THE DEGREE COUNT OF COLUMN JCOL. UPDATE THE DEGREE */
/*              COUNTS OF THESE COLUMNS. ARRAY IWA RECORDS THE */
/*              MARKED COLUMNS. */

		if (bwa[ic]) {
		    goto L20;
		}
		bwa[ic] = TRUE_;
		++ndeg[ic];
		++deg;
		iwa[deg] = ic;
L20:
/* L30: */
		;
	    }
/* L40: */
	}
L50:

/*        UN-MARK THE COLUMNS RECORDED BY IWA AND FINALIZE THE */
/*        DEGREE COUNT OF COLUMN JCOL. */

	if (deg < 1) {
	    goto L70;
	}
	i__2 = deg;
	for (jp = 1; jp <= i__2; ++jp) {
	    ic = iwa[jp];
	    bwa[ic] = FALSE_;
/* L60: */
	}
	ndeg[jcol] += deg;
L70:
/* L80: */
	;
    }
L90:
    return 0;

/*     LAST CARD OF SUBROUTINE D7EGR. */

} /* d7egr_ */

/* Subroutine */ int drmng_(doublereal *d__, doublereal *fx, doublereal *g, 
	integer *iv, integer *liv, integer *lv, integer *n, doublereal *v, 
	doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, k, l;
    static doublereal t;
    static integer w, z__, g01, x01, dg1, temp1, step1, dummy;
    extern /* Subroutine */ int dd7dog_(doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *);
    extern logical stopx_(integer *);
    extern /* Subroutine */ int dl7upd_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *), dl7ivm_(integer *, doublereal *, doublereal *, 
	    doublereal *), dw7zbf_(doublereal *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), dl7vml_(integer *, doublereal *, doublereal *, 
	    doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
	     doublereal *), dv7cpy_(integer *, doublereal *, doublereal *), 
	    dl7tvm_(integer *, doublereal *, doublereal *, doublereal *), 
	    dv2axy_(integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dv7vmp_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    static integer nwtst1;
    extern /* Subroutine */ int dparck_(integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);
    static integer lstgst, rstrst;


/*  ***  CARRY OUT  DMNG (UNCONSTRAINED MINIMIZATION) ITERATIONS, USING */
/*  ***  DOUBLE-DOGLEG/BFGS STEPS. */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* D.... SCALE VECTOR. */
/* FX... FUNCTION VALUE. */
/* G.... GRADIENT VECTOR. */
/* IV... INTEGER VALUE ARRAY. */
/* LIV.. LENGTH OF IV (AT LEAST 60). */
/* LV... LENGTH OF V (AT LEAST 71 + N*(N+13)/2). */
/* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... VECTOR OF PARAMETERS TO BE OPTIMIZED. */

/*  ***  DISCUSSION  *** */

/*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO  DMNG (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
/*     THE PART OF V THAT  DMNG USES FOR STORING G IS NOT NEEDED). */
/*     MOREOVER, COMPARED WITH  DMNG, IV(1) MAY HAVE THE TWO ADDITIONAL */
/*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
/*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
/*     OUTPUT VALUE FROM  DMNG (AND  DMNF), IS NOT REFERENCED BY */
/*     DRMNG OR THE SUBROUTINES IT CALLS. */
/*        FX AND G NEED NOT HAVE BEEN INITIALIZED WHEN DRMNG IS CALLED */
/*     WITH IV(1) = 12, 13, OR 14. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
/*             AT X, AND CALL DRMNG AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
/*             (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN BECAUSE */
/*             OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER SHOULD SET */
/*             IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE DRMNG TO IG- */
/*             NORE FX AND TRY A SMALLER STEP.  THE PARAMETER NF THAT */
/*              DMNG PASSES TO CALCF (FOR POSSIBLE USE BY CALCG) IS A */
/*             COPY OF IV(NFCALL) = IV(6). */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT VECTOR */
/*             OF F AT X, AND CALL DRMNG AGAIN, HAVING CHANGED NONE OF */
/*             THE OTHER PARAMETERS EXCEPT POSSIBLY THE SCALE VECTOR D */
/*             WHEN IV(DTYPE) = 0.  THE PARAMETER NF THAT  DMNG PASSES */
/*             TO CALCG IS IV(NFGCAL) = IV(7).  IF G(X) CANNOT BE */
/*             EVALUATED, THEN THE CALLER MAY SET IV(TOOBIG) TO 0, IN */
/*             WHICH CASE DRMNG WILL RETURN WITH IV(1) = 65. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (DECEMBER 1979).  REVISED SEPT. 1982. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324 AND MCS-7906671. */

/*        (SEE  DMNG FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  NO INTRINSIC FUNCTIONS  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DD7DOG.... COMPUTES DOUBLE-DOGLEG (CANDIDATE) STEP. */
/* DIVSET.... SUPPLIES DEFAULT IV AND V INPUT COMPONENTS. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DL7ITV... MULTIPLIES INVERSE TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
/* DL7IVM... MULTIPLIES INVERSE OF LOWER TRIANGLE TIMES VECTOR. */
/* DL7TVM... MULTIPLIES TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
/* LUPDT.... UPDATES CHOLESKY FACTOR OF HESSIAN APPROXIMATION. */
/* DL7VML.... MULTIPLIES LOWER TRIANGLE TIMES VECTOR. */
/* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV7VMP... MULTIPLIES VECTOR BY VECTOR RAISED TO POWER (COMPONENTWISE). */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
/* DW7ZBF... COMPUTES W AND Z FOR DL7UPD CORRESPONDING TO BFGS UPDATE. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */


/*  ***  V SUBSCRIPT VALUES  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --g;
    --d__;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L50;
    }
    if (i__ == 2) {
	goto L60;
    }

/*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    if (iv[1] == 12 || iv[1] == 13) {
	iv[4] += *n * (*n + 13) / 2;
    }
    dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    switch (i__) {
	case 1:  goto L190;
	case 2:  goto L190;
	case 3:  goto L190;
	case 4:  goto L190;
	case 5:  goto L190;
	case 6:  goto L190;
	case 7:  goto L120;
	case 8:  goto L90;
	case 9:  goto L120;
	case 10:  goto L10;
	case 11:  goto L10;
	case 12:  goto L20;
    }

/*  ***  STORAGE ALLOCATION  *** */

L10:
    l = iv[42];
    iv[43] = l + *n * (*n + 1) / 2;
    iv[40] = iv[43] + *n;
    iv[41] = iv[40] + *n;
    iv[48] = iv[41] + *n;
    iv[34] = iv[48] + *n;
    iv[37] = iv[34] + *n;
    iv[47] = iv[37] + *n;
    if (iv[1] != 13) {
	goto L20;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L20:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[5] = 1;
    iv[11] = 1;
    iv[2] = 0;
    iv[55] = 0;
    iv[8] = 0;
    v[9] = 0.;
    if (v[38] >= 0.) {
	dv7scp_(n, &d__[1], &v[38]);
    }
    if (iv[25] != 1) {
	goto L40;
    }

/*     ***  SET THE INITIAL HESSIAN APPROXIMATION TO DIAG(D)**-2  *** */

    l = iv[42];
    i__1 = *n * (*n + 1) / 2;
    dv7scp_(&i__1, &v[l], &c_b14);
    k = l - 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k += i__;
	t = d__[i__];
	if (t <= 0.) {
	    t = 1.;
	}
	v[k] = t;
/* L30: */
    }

/*  ***  COMPUTE INITIAL FUNCTION VALUE  *** */

L40:
    iv[1] = 1;
    goto L999;

L50:
    v[10] = *fx;
    if (iv[35] >= 0) {
	goto L190;
    }
    v[13] = *fx;
    iv[1] = 2;
    if (iv[2] == 0) {
	goto L999;
    }
    iv[1] = 63;
    goto L350;

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L60:
    if (iv[2] == 0) {
	goto L70;
    }
    iv[1] = 65;
    goto L350;

L70:
    dg1 = iv[37];
    dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);
    v[1] = dv2nrm_(n, &v[dg1]);

    if (iv[55] != 0) {
	goto L340;
    }
    if (iv[35] == 0) {
	goto L300;
    }

/*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

    v[8] = v[35];

    iv[35] = 0;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L80:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L90:
    k = iv[31];
    if (k < iv[18]) {
	goto L100;
    }
    iv[1] = 10;
    goto L350;

/*  ***  UPDATE RADIUS  *** */

L100:
    iv[31] = k + 1;
    if (k > 0) {
	v[8] = v[16] * v[2];
    }

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

    g01 = iv[48];
    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[33] = -1;

/*     ***  COPY X TO X0, G TO G0  *** */

    dv7cpy_(n, &v[x01], &x[1]);
    dv7cpy_(n, &v[g01], &g[1]);

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L110:
    if (! stopx_(&dummy)) {
	goto L130;
    }
    iv[1] = 11;
    goto L140;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L120:
    if (v[10] >= v[13]) {
	goto L130;
    }
    v[16] = 1.;
    k = iv[31];
    goto L100;

L130:
    if (iv[6] < iv[17]) {
	goto L150;
    }
    iv[1] = 9;
L140:
    if (v[10] >= v[13]) {
	goto L350;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L290;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L150:
    step1 = iv[40];
    dg1 = iv[37];
    nwtst1 = iv[34];
    if (iv[33] >= 0) {
	goto L160;
    }
    l = iv[42];
    dl7ivm_(n, &v[nwtst1], &v[l], &g[1]);
    v[6] = dd7tpr_(n, &v[nwtst1], &v[nwtst1]) * .5;
    dl7itv_(n, &v[nwtst1], &v[l], &v[nwtst1]);
    dv7vmp_(n, &v[step1], &v[nwtst1], &d__[1], &c__1);
    v[3] = dv2nrm_(n, &v[step1]);
    dv7vmp_(n, &v[dg1], &v[dg1], &d__[1], &c_n1);
    dl7tvm_(n, &v[step1], &v[l], &v[dg1]);
    v[44] = dv2nrm_(n, &v[step1]);
    iv[33] = 0;
L160:
    dd7dog_(&v[dg1], lv, n, &v[nwtst1], &v[step1], &v[1]);
    if (iv[29] != 6) {
	goto L170;
    }
    if (iv[9] != 2) {
	goto L190;
    }
    rstrst = 2;
    goto L200;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L170:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L190;
    }
    if (iv[29] != 5) {
	goto L180;
    }
    if (v[16] <= 1.) {
	goto L180;
    }
    if (v[7] > v[11] * 1.2) {
	goto L180;
    }
    if (iv[9] != 2) {
	goto L190;
    }
    rstrst = 0;
    goto L200;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L180:
    x01 = iv[43];
    step1 = iv[40];
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L999;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L190:
    rstrst = 3;
L200:
    x01 = iv[43];
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = iv[41];
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L240;
	case 2:  goto L210;
	case 3:  goto L220;
	case 4:  goto L230;
    }
L210:
    dv7cpy_(n, &x[1], &v[x01]);
    goto L240;
L220:
    dv7cpy_(n, &v[lstgst], &v[step1]);
    goto L240;
L230:
    dv7cpy_(n, &v[step1], &v[lstgst]);
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

L240:
    k = iv[29];
    switch (k) {
	case 1:  goto L250;
	case 2:  goto L280;
	case 3:  goto L280;
	case 4:  goto L280;
	case 5:  goto L250;
	case 6:  goto L260;
	case 7:  goto L270;
	case 8:  goto L270;
	case 9:  goto L270;
	case 10:  goto L270;
	case 11:  goto L270;
	case 12:  goto L270;
	case 13:  goto L330;
	case 14:  goto L300;
    }

/*     ***  RECOMPUTE STEP WITH CHANGED RADIUS  *** */

L250:
    v[8] = v[16] * v[2];
    goto L110;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L260:
    v[8] = v[36];
    goto L150;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L270:
    iv[55] = k - 4;
    if (v[10] >= v[13]) {
	goto L340;
    }
    if (iv[13] == 14) {
	goto L340;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L280:
    if (iv[29] != 3) {
	goto L290;
    }
    step1 = iv[40];
    temp1 = iv[41];

/*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

    l = iv[42];
    dl7tvm_(n, &v[temp1], &v[l], &v[step1]);
    dl7vml_(n, &v[temp1], &v[l], &v[temp1]);

/*  ***  COMPUTE GRADIENT  *** */

L290:
    ++iv[30];
    iv[1] = 2;
    goto L999;

/*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L300:
    g01 = iv[48];
    dv2axy_(n, &v[g01], &c_b90, &v[g01], &g[1]);
    step1 = iv[40];
    temp1 = iv[41];
    if (iv[29] != 3) {
	goto L320;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

/*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

    dv2axy_(n, &v[temp1], &c_b90, &v[g01], &v[temp1]);
    dv7vmp_(n, &v[temp1], &v[temp1], &d__[1], &c_n1);

/*        ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
	goto L310;
    }
    if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L320;
    }
L310:
    v[16] = v[23];

/*  ***  UPDATE H, LOOP  *** */

L320:
    w = iv[34];
    z__ = iv[43];
    l = iv[42];
    dw7zbf_(&v[l], n, &v[step1], &v[w], &v[g01], &v[z__]);

/*     ** USE THE N-VECTORS STARTING AT V(STEP1) AND V(G01) FOR SCRATCH.. */
    dl7upd_(&v[temp1], &v[step1], &v[l], &v[g01], &v[l], n, &v[w], &v[z__]);
    iv[1] = 2;
    goto L80;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L330:
    iv[1] = 64;
    goto L350;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L340:
    iv[1] = iv[55];
    iv[55] = 0;
L350:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);

L999:
    return 0;

/*  ***  LAST LINE OF DRMNG FOLLOWS  *** */
} /* drmng_ */

/* Subroutine */ int i7do_(integer *m, integer *n, integer *indrow, integer *
	jpntr, integer *indcol, integer *ipntr, integer *ndeg, integer *list, 
	integer *maxclq, integer *iwa1, integer *iwa2, integer *iwa3, integer 
	*iwa4, logical *bwa)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer l, ic, ip, jp, ir, deg, ipl, jpl, ipu, jpu, head, jcol, 
	    ncomp;
    extern /* Subroutine */ int n7msrt_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *);
    static integer maxinc, numinc, numord, maxlst, numwgt, numlst;

/*     ********** */

/*     SUBROUTINE I7DO */

/*     GIVEN THE SPARSITY PATTERN OF AN M BY N MATRIX A, THIS */
/*     SUBROUTINE DETERMINES AN INCIDENCE-DEGREE ORDERING OF THE */
/*     COLUMNS OF A. */

/*     THE INCIDENCE-DEGREE ORDERING IS DEFINED FOR THE LOOPLESS */
/*     GRAPH G WITH VERTICES A(J), J = 1,2,...,N WHERE A(J) IS THE */
/*     J-TH COLUMN OF A AND WITH EDGE (A(I),A(J)) IF AND ONLY IF */
/*     COLUMNS I AND J HAVE A NON-ZERO IN THE SAME ROW POSITION. */

/*     AT EACH STAGE OF I7DO, A COLUMN OF MAXIMAL INCIDENCE IS */
/*     CHOSEN AND ORDERED. IF JCOL IS AN UN-ORDERED COLUMN, THEN */
/*     THE INCIDENCE OF JCOL IS THE NUMBER OF ORDERED COLUMNS */
/*     ADJACENT TO JCOL IN THE GRAPH G. AMONG ALL THE COLUMNS OF */
/*     MAXIMAL INCIDENCE,I7DO CHOOSES A COLUMN OF MAXIMAL DEGREE. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE I7DO(M,N,INDROW,JPNTR,INDCOL,IPNTR,NDEG,LIST, */
/*                      MAXCLQ,IWA1,IWA2,IWA3,IWA4,BWA) */

/*     WHERE */

/*       M IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF ROWS OF A. */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       INDROW IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE ROW */
/*         INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       JPNTR IS AN INTEGER INPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       INDCOL IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE */
/*         COLUMN INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       IPNTR IS AN INTEGER INPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(M+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       NDEG IS AN INTEGER INPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE DEGREE SEQUENCE. THE DEGREE OF THE J-TH COLUMN */
/*         OF A IS NDEG(J). */

/*       LIST IS AN INTEGER OUTPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE INCIDENCE-DEGREE ORDERING OF THE COLUMNS OF A. THE J-TH */
/*         COLUMN IN THIS ORDER IS LIST(J). */

/*       MAXCLQ IS AN INTEGER OUTPUT VARIABLE SET TO THE SIZE */
/*         OF THE LARGEST CLIQUE FOUND DURING THE ORDERING. */

/*       IWA1,IWA2,IWA3, AND IWA4 ARE INTEGER WORK ARRAYS OF LENGTH N. */

/*       BWA IS A LOGICAL WORK ARRAY OF LENGTH N. */

/*     SUBPROGRAMS CALLED */

/*       MINPACK-SUPPLIED ... N7MSRT */

/*       FORTRAN-SUPPLIED ... MAX0 */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     SORT THE DEGREE SEQUENCE. */

    /* Parameter adjustments */
    --bwa;
    --iwa4;
    --iwa3;
    --iwa2;
    --iwa1;
    --list;
    --ndeg;
    --indrow;
    --jpntr;
    --indcol;
    --ipntr;

    /* Function Body */
    i__1 = *n - 1;
    n7msrt_(n, &i__1, &ndeg[1], &c_n1, &iwa4[1], &iwa1[1], &iwa3[1]);

/*     INITIALIZATION BLOCK. */

/*     CREATE A DOUBLY-LINKED LIST TO ACCESS THE INCIDENCES OF THE */
/*     COLUMNS. THE POINTERS FOR THE LINKED LIST ARE AS FOLLOWS. */

/*     EACH UN-ORDERED COLUMN JCOL IS IN A LIST (THE INCIDENCE LIST) */
/*     OF COLUMNS WITH THE SAME INCIDENCE. */

/*     IWA1(NUMINC+1) IS THE FIRST COLUMN IN THE NUMINC LIST */
/*     UNLESS IWA1(NUMINC+1) = 0. IN THIS CASE THERE ARE */
/*     NO COLUMNS IN THE NUMINC LIST. */

/*     IWA2(JCOL) IS THE COLUMN BEFORE JCOL IN THE INCIDENCE LIST */
/*     UNLESS IWA2(JCOL) = 0. IN THIS CASE JCOL IS THE FIRST */
/*     COLUMN IN THIS INCIDENCE LIST. */

/*     IWA3(JCOL) IS THE COLUMN AFTER JCOL IN THE INCIDENCE LIST */
/*     UNLESS IWA3(JCOL) = 0. IN THIS CASE JCOL IS THE LAST */
/*     COLUMN IN THIS INCIDENCE LIST. */

/*     IF JCOL IS AN UN-ORDERED COLUMN, THEN LIST(JCOL) IS THE */
/*     INCIDENCE OF JCOL IN THE GRAPH. IF JCOL IS AN ORDERED COLUMN, */
/*     THEN LIST(JCOL) IS THE INCIDENCE-DEGREE ORDER OF COLUMN JCOL. */

    maxinc = 0;
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	list[jp] = 0;
	bwa[jp] = FALSE_;
	iwa1[jp] = 0;
	l = iwa4[jp];
	if (jp != 1) {
	    iwa2[l] = iwa4[jp - 1];
	}
	if (jp != *n) {
	    iwa3[l] = iwa4[jp + 1];
	}
/* L10: */
    }
    iwa1[1] = iwa4[1];
    l = iwa4[1];
    iwa2[l] = 0;
    l = iwa4[*n];
    iwa3[l] = 0;

/*     DETERMINE THE MAXIMAL SEARCH LENGTH FOR THE LIST */
/*     OF COLUMNS OF MAXIMAL INCIDENCE. */

    maxlst = 0;
    i__1 = *m;
    for (ir = 1; ir <= i__1; ++ir) {
/* Computing 2nd power */
	i__2 = ipntr[ir + 1] - ipntr[ir];
	maxlst += i__2 * i__2;
/* L20: */
    }
    maxlst /= *n;
    *maxclq = 1;

/*     BEGINNING OF ITERATION LOOP. */

    i__1 = *n;
    for (numord = 1; numord <= i__1; ++numord) {

/*        CHOOSE A COLUMN JCOL OF MAXIMAL DEGREE AMONG THE */
/*        COLUMNS OF MAXIMAL INCIDENCE. */

	jp = iwa1[maxinc + 1];
	numlst = 1;
	numwgt = -1;
L30:
	if (ndeg[jp] <= numwgt) {
	    goto L40;
	}
	numwgt = ndeg[jp];
	jcol = jp;
L40:
	jp = iwa3[jp];
	++numlst;
	if (jp > 0 && numlst <= maxlst) {
	    goto L30;
	}
	list[jcol] = numord;

/*        DELETE COLUMN JCOL FROM THE LIST OF COLUMNS OF */
/*        MAXIMAL INCIDENCE. */

	l = iwa2[jcol];
	if (l == 0) {
	    iwa1[maxinc + 1] = iwa3[jcol];
	}
	if (l > 0) {
	    iwa3[l] = iwa3[jcol];
	}
	l = iwa3[jcol];
	if (l > 0) {
	    iwa2[l] = iwa2[jcol];
	}

/*        UPDATE THE SIZE OF THE LARGEST CLIQUE */
/*        FOUND DURING THE ORDERING. */

	if (maxinc == 0) {
	    ncomp = 0;
	}
	++ncomp;
	if (maxinc + 1 == ncomp) {
	    *maxclq = max(*maxclq,ncomp);
	}

/*        UPDATE THE MAXIMAL INCIDENCE COUNT. */

L50:
	if (iwa1[maxinc + 1] > 0) {
	    goto L60;
	}
	--maxinc;
	if (maxinc >= 0) {
	    goto L50;
	}
L60:

/*        FIND ALL COLUMNS ADJACENT TO COLUMN JCOL. */

	bwa[jcol] = TRUE_;
	deg = 0;

/*        DETERMINE ALL POSITIONS (IR,JCOL) WHICH CORRESPOND */
/*        TO NON-ZEROES IN THE MATRIX. */

	jpl = jpntr[jcol];
	jpu = jpntr[jcol + 1] - 1;
	if (jpu < jpl) {
	    goto L100;
	}
	i__2 = jpu;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];

/*           FOR EACH ROW IR, DETERMINE ALL POSITIONS (IR,IC) */
/*           WHICH CORRESPOND TO NON-ZEROES IN THE MATRIX. */

	    ipl = ipntr[ir];
	    ipu = ipntr[ir + 1] - 1;
	    i__3 = ipu;
	    for (ip = ipl; ip <= i__3; ++ip) {
		ic = indcol[ip];

/*              ARRAY BWA MARKS COLUMNS WHICH ARE ADJACENT TO */
/*              COLUMN JCOL. ARRAY IWA4 RECORDS THE MARKED COLUMNS. */

		if (bwa[ic]) {
		    goto L70;
		}
		bwa[ic] = TRUE_;
		++deg;
		iwa4[deg] = ic;
L70:
/* L80: */
		;
	    }
/* L90: */
	}
L100:

/*        UPDATE THE POINTERS TO THE INCIDENCE LISTS. */

	if (deg < 1) {
	    goto L130;
	}
	i__2 = deg;
	for (jp = 1; jp <= i__2; ++jp) {
	    ic = iwa4[jp];
	    if (list[ic] > 0) {
		goto L110;
	    }
	    numinc = -list[ic] + 1;
	    list[ic] = -numinc;
	    maxinc = max(maxinc,numinc);

/*           DELETE COLUMN IC FROM THE NUMINC-1 LIST. */

	    l = iwa2[ic];
	    if (l == 0) {
		iwa1[numinc] = iwa3[ic];
	    }
	    if (l > 0) {
		iwa3[l] = iwa3[ic];
	    }
	    l = iwa3[ic];
	    if (l > 0) {
		iwa2[l] = iwa2[ic];
	    }

/*           ADD COLUMN IC TO THE NUMINC LIST. */

	    head = iwa1[numinc + 1];
	    iwa1[numinc + 1] = ic;
	    iwa2[ic] = 0;
	    iwa3[ic] = head;
	    if (head > 0) {
		iwa2[head] = ic;
	    }
L110:

/*           UN-MARK COLUMN IC IN THE ARRAY BWA. */

	    bwa[ic] = FALSE_;
/* L120: */
	}
L130:
	bwa[jcol] = FALSE_;

/*        END OF ITERATION LOOP. */

/* L140: */
    }

/*     INVERT THE ARRAY LIST. */

    i__1 = *n;
    for (jcol = 1; jcol <= i__1; ++jcol) {
	numord = list[jcol];
	iwa1[numord] = jcol;
/* L150: */
    }
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	list[jp] = iwa1[jp];
/* L160: */
    }
    return 0;

/*     LAST CARD OF SUBROUTINE I7DO. */

} /* i7do_ */

/* Subroutine */ int m7slo_(integer *n, integer *indrow, integer *jpntr, 
	integer *indcol, integer *ipntr, integer *ndeg, integer *list, 
	integer *maxclq, integer *iwa1, integer *iwa2, integer *iwa3, integer 
	*iwa4, logical *bwa)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer l, ic, ip, jp, ir, deg, ipl, jpl, ipu, jpu, head, jcol, 
	    mindeg, numdeg, numord;

/*     ********** */

/*     SUBROUTINE M7SLO */

/*     GIVEN THE SPARSITY PATTERN OF AN M BY N MATRIX A, THIS */
/*     SUBROUTINE DETERMINES THE SMALLEST-LAST ORDERING OF THE */
/*     COLUMNS OF A. */

/*     THE SMALLEST-LAST ORDERING IS DEFINED FOR THE LOOPLESS */
/*     GRAPH G WITH VERTICES A(J), J = 1,2,...,N WHERE A(J) IS THE */
/*     J-TH COLUMN OF A AND WITH EDGE (A(I),A(J)) IF AND ONLY IF */
/*     COLUMNS I AND J HAVE A NON-ZERO IN THE SAME ROW POSITION. */

/*     THE SMALLEST-LAST ORDERING IS DETERMINED RECURSIVELY BY */
/*     LETTING LIST(K), K = N,...,1 BE A COLUMN WITH LEAST DEGREE */
/*     IN THE SUBGRAPH SPANNED BY THE UN-ORDERED COLUMNS. */

/*     NOTE THAT THE VALUE OF M IS NOT NEEDED BY M7SLO AND IS */
/*     THEREFORE NOT PRESENT IN THE SUBROUTINE STATEMENT. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE M7SLO(N,INDROW,JPNTR,INDCOL,IPNTR,NDEG,LIST, */
/*                      MAXCLQ,IWA1,IWA2,IWA3,IWA4,BWA) */

/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       INDROW IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE ROW */
/*         INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       JPNTR IS AN INTEGER INPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       INDCOL IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE */
/*         COLUMN INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       IPNTR IS AN INTEGER INPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(M+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       NDEG IS AN INTEGER INPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE DEGREE SEQUENCE. THE DEGREE OF THE J-TH COLUMN */
/*         OF A IS NDEG(J). */

/*       LIST IS AN INTEGER OUTPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE SMALLEST-LAST ORDERING OF THE COLUMNS OF A. THE J-TH */
/*         COLUMN IN THIS ORDER IS LIST(J). */

/*       MAXCLQ IS AN INTEGER OUTPUT VARIABLE SET TO THE SIZE */
/*         OF THE LARGEST CLIQUE FOUND DURING THE ORDERING. */

/*       IWA1,IWA2,IWA3, AND IWA4 ARE INTEGER WORK ARRAYS OF LENGTH N. */

/*       BWA IS A LOGICAL WORK ARRAY OF LENGTH N. */

/*     SUBPROGRAMS CALLED */

/*       FORTRAN-SUPPLIED ... MIN0 */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     INITIALIZATION BLOCK. */

    /* Parameter adjustments */
    --bwa;
    --iwa4;
    --iwa3;
    --iwa2;
    --iwa1;
    --list;
    --ndeg;
    --indrow;
    --jpntr;
    --indcol;
    --ipntr;

    /* Function Body */
    mindeg = *n;
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	iwa1[jp] = 0;
	bwa[jp] = FALSE_;
	list[jp] = ndeg[jp];
/* Computing MIN */
	i__2 = mindeg, i__3 = ndeg[jp];
	mindeg = min(i__2,i__3);
/* L10: */
    }

/*     CREATE A DOUBLY-LINKED LIST TO ACCESS THE DEGREES OF THE */
/*     COLUMNS. THE POINTERS FOR THE LINKED LIST ARE AS FOLLOWS. */

/*     EACH UN-ORDERED COLUMN JCOL IS IN A LIST (THE DEGREE */
/*     LIST) OF COLUMNS WITH THE SAME DEGREE. */

/*     IWA1(NUMDEG+1) IS THE FIRST COLUMN IN THE NUMDEG LIST */
/*     UNLESS IWA1(NUMDEG+1) = 0. IN THIS CASE THERE ARE */
/*     NO COLUMNS IN THE NUMDEG LIST. */

/*     IWA2(JCOL) IS THE COLUMN BEFORE JCOL IN THE DEGREE LIST */
/*     UNLESS IWA2(JCOL) = 0. IN THIS CASE JCOL IS THE FIRST */
/*     COLUMN IN THIS DEGREE LIST. */

/*     IWA3(JCOL) IS THE COLUMN AFTER JCOL IN THE DEGREE LIST */
/*     UNLESS IWA3(JCOL) = 0. IN THIS CASE JCOL IS THE LAST */
/*     COLUMN IN THIS DEGREE LIST. */

/*     IF JCOL IS AN UN-ORDERED COLUMN, THEN LIST(JCOL) IS THE */
/*     DEGREE OF JCOL IN THE GRAPH INDUCED BY THE UN-ORDERED */
/*     COLUMNS. IF JCOL IS AN ORDERED COLUMN, THEN LIST(JCOL) */
/*     IS THE SMALLEST-LAST ORDER OF COLUMN JCOL. */

    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	numdeg = ndeg[jp];
	head = iwa1[numdeg + 1];
	iwa1[numdeg + 1] = jp;
	iwa2[jp] = 0;
	iwa3[jp] = head;
	if (head > 0) {
	    iwa2[head] = jp;
	}
/* L20: */
    }
    *maxclq = 0;
    numord = *n;

/*     BEGINNING OF ITERATION LOOP. */

L30:

/*        MARK THE SIZE OF THE LARGEST CLIQUE */
/*        FOUND DURING THE ORDERING. */

    if (mindeg + 1 == numord && *maxclq == 0) {
	*maxclq = numord;
    }

/*        CHOOSE A COLUMN JCOL OF MINIMAL DEGREE MINDEG. */

L40:
    jcol = iwa1[mindeg + 1];
    if (jcol > 0) {
	goto L50;
    }
    ++mindeg;
    goto L40;
L50:
    list[jcol] = numord;
    --numord;

/*        TERMINATION TEST. */

    if (numord == 0) {
	goto L120;
    }

/*        DELETE COLUMN JCOL FROM THE MINDEG LIST. */

    l = iwa3[jcol];
    iwa1[mindeg + 1] = l;
    if (l > 0) {
	iwa2[l] = 0;
    }

/*        FIND ALL COLUMNS ADJACENT TO COLUMN JCOL. */

    bwa[jcol] = TRUE_;
    deg = 0;

/*        DETERMINE ALL POSITIONS (IR,JCOL) WHICH CORRESPOND */
/*        TO NON-ZEROES IN THE MATRIX. */

    jpl = jpntr[jcol];
    jpu = jpntr[jcol + 1] - 1;
    if (jpu < jpl) {
	goto L90;
    }
    i__1 = jpu;
    for (jp = jpl; jp <= i__1; ++jp) {
	ir = indrow[jp];

/*           FOR EACH ROW IR, DETERMINE ALL POSITIONS (IR,IC) */
/*           WHICH CORRESPOND TO NON-ZEROES IN THE MATRIX. */

	ipl = ipntr[ir];
	ipu = ipntr[ir + 1] - 1;
	i__2 = ipu;
	for (ip = ipl; ip <= i__2; ++ip) {
	    ic = indcol[ip];

/*              ARRAY BWA MARKS COLUMNS WHICH ARE ADJACENT TO */
/*              COLUMN JCOL. ARRAY IWA4 RECORDS THE MARKED COLUMNS. */

	    if (bwa[ic]) {
		goto L60;
	    }
	    bwa[ic] = TRUE_;
	    ++deg;
	    iwa4[deg] = ic;
L60:
/* L70: */
	    ;
	}
/* L80: */
    }
L90:

/*        UPDATE THE POINTERS TO THE CURRENT DEGREE LISTS. */

    if (deg < 1) {
	goto L110;
    }
    i__1 = deg;
    for (jp = 1; jp <= i__1; ++jp) {
	ic = iwa4[jp];
	numdeg = list[ic];
	--list[ic];
/* Computing MIN */
	i__2 = mindeg, i__3 = list[ic];
	mindeg = min(i__2,i__3);

/*           DELETE COLUMN IC FROM THE NUMDEG LIST. */

	l = iwa2[ic];
	if (l == 0) {
	    iwa1[numdeg + 1] = iwa3[ic];
	}
	if (l > 0) {
	    iwa3[l] = iwa3[ic];
	}
	l = iwa3[ic];
	if (l > 0) {
	    iwa2[l] = iwa2[ic];
	}

/*           ADD COLUMN IC TO THE NUMDEG-1 LIST. */

	head = iwa1[numdeg];
	iwa1[numdeg] = ic;
	iwa2[ic] = 0;
	iwa3[ic] = head;
	if (head > 0) {
	    iwa2[head] = ic;
	}

/*           UN-MARK COLUMN IC IN THE ARRAY BWA. */

	bwa[ic] = FALSE_;
/* L100: */
    }
L110:

/*        END OF ITERATION LOOP. */

    goto L30;
L120:

/*     INVERT THE ARRAY LIST. */

    i__1 = *n;
    for (jcol = 1; jcol <= i__1; ++jcol) {
	numord = list[jcol];
	iwa1[numord] = jcol;
/* L130: */
    }
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	list[jp] = iwa1[jp];
/* L140: */
    }
    return 0;

/*     LAST CARD OF SUBROUTINE M7SLO. */

} /* m7slo_ */

/* Subroutine */ int ds7dmp_(integer *n, doublereal *x, doublereal *y, 
	doublereal *z__, integer *k)
{
    /* Initialized data */

    static doublereal one = 1.;

    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, l;
    static doublereal t;


/* ***  SET X = DIAG(Z)**K * Y * DIAG(Z)**K */
/* ***  FOR X, Y = COMPACTLY STORED LOWER TRIANG. MATRICES */
/* ***  K = 1 OR -1. */

    /* Parameter adjustments */
    --z__;
    --x;
    --y;

    /* Function Body */

    l = 1;
    if (*k >= 0) {
	goto L30;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = one / z__[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    x[l] = t * y[l] / z__[j];
	    ++l;
/* L10: */
	}
/* L20: */
    }
    goto L999;

L30:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = z__[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    x[l] = t * y[l] * z__[j];
	    ++l;
/* L40: */
	}
/* L50: */
    }
L999:
    return 0;
/*  ***  LAST CARD OF DS7DMP FOLLOWS  *** */
} /* ds7dmp_ */

/* Subroutine */ int ds7bqn_(doublereal *b, doublereal *d__, doublereal *dst, 
	integer *ipiv, integer *ipiv1, integer *ipiv2, integer *kb, 
	doublereal *l, integer *lv, integer *ns, integer *p, integer *p1, 
	doublereal *step, doublereal *td, doublereal *tg, doublereal *v, 
	doublereal *w, doublereal *x, doublereal *x0)
{
    /* Initialized data */

    static doublereal fudge = 1.0001;
    static doublereal half = .5;
    static doublereal meps2 = 0.;
    static doublereal one = 1.;
    static doublereal two = 2.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    static integer p0;
    static doublereal t1, dx, ti, xi;
    static integer p1m1;
    static doublereal gts, dst0, dst1, alpha;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dv7shf_(integer *, integer *, doublereal *), 
	    dl7ivm_(integer *, doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int i7shft_(integer *, integer *, integer *), 
	    dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
	     doublereal *), dq7rsh_(integer *, integer *, logical *, 
	    doublereal *, doublereal *, doublereal *), dv7ipr_(integer *, 
	    integer *, doublereal *), dv7cpy_(integer *, doublereal *, 
	    doublereal *), dv2axy_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal dstmin, dstmax;


/*  ***  COMPUTE BOUNDED MODIFIED NEWTON STEP  *** */

/*     DIMENSION L(P*(P+1)/2) */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  V SUBSCRIPTS  *** */



    /* Parameter adjustments */
    --l;
    --v;
    --x0;
    --x;
    --w;
    --tg;
    --td;
    --step;
    --ipiv2;
    --ipiv1;
    --ipiv;
    --dst;
    --d__;
    b -= 3;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    dstmax = fudge * (one + v[21]) * v[8];
    dstmin = (one + v[20]) * v[8];
    dst1 = zero;
    if (meps2 <= zero) {
	meps2 = two * dr7mdc_(&c__3);
    }
    p0 = *p1;
    *ns = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ipiv1[i__] = i__;
	ipiv2[i__] = i__;
/* L10: */
    }
    i__1 = *p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L20: */
	w[i__] = -step[i__] * td[i__];
    }
    alpha = abs(v[5]);
    v[7] = zero;
    gts = -v[4];
    if (*kb < 0) {
	dv7scp_(p, &dst[1], &zero);
    }
    *kb = 1;

/*     ***  -W = D TIMES RESTRICTED NEWTON STEP FROM X + DST/D. */

/*     ***  FIND T SUCH THAT X - T*W IS STILL FEASIBLE. */

L30:
    t = one;
    k = 0;
    i__1 = *p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = ipiv[i__];
	dx = w[i__] / d__[j];
	xi = x[j] - dx;
	if (xi < b[(j << 1) + 1]) {
	    goto L40;
	}
	if (xi <= b[(j << 1) + 2]) {
	    goto L60;
	}
	ti = (x[j] - b[(j << 1) + 2]) / dx;
	k = i__;
	goto L50;
L40:
	ti = (x[j] - b[(j << 1) + 1]) / dx;
	k = -i__;
L50:
	if (t <= ti) {
	    goto L60;
	}
	t = ti;
L60:
	;
    }

    if (*p > *p1) {
	i__1 = *p - *p1;
	dv7cpy_(&i__1, &step[*p1 + 1], &dst[*p1 + 1]);
    }
    d__1 = -t;
    dv2axy_(p1, &step[1], &d__1, &w[1], &dst[1]);
    dst0 = dst1;
    dst1 = dv2nrm_(p, &step[1]);

/*  ***  CHECK FOR OVERSIZE STEP  *** */

    if (dst1 <= dstmax) {
	goto L80;
    }
    if (*p1 >= p0) {
	goto L70;
    }
    if (dst0 < dstmin) {
	*kb = 0;
    }
    goto L110;

L70:
    k = 0;

/*  ***  UPDATE DST, TG, AND V(PREDUC)  *** */

L80:
    v[2] = dst1;
    dv7cpy_(p1, &dst[1], &step[1]);
    t1 = one - t;
    i__1 = *p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L90: */
	tg[i__] = t1 * tg[i__];
    }
    if (alpha > zero) {
	d__1 = t * alpha;
	dv2axy_(p1, &tg[1], &d__1, &w[1], &tg[1]);
    }
    v[7] += t * ((one - half * t) * gts + half * alpha * t * dd7tpr_(p1, &w[1]
	    , &w[1]));
    if (k == 0) {
	goto L110;
    }

/*     ***  PERMUTE L, ETC. IF NECESSARY  *** */

    p1m1 = *p1 - 1;
    j = abs(k);
    if (j == *p1) {
	goto L100;
    }
    ++(*ns);
    ipiv2[*p1] = j;
    dq7rsh_(&j, p1, &c_false, &tg[1], &l[1], &w[1]);
    i7shft_(p1, &j, &ipiv[1]);
    i7shft_(p1, &j, &ipiv1[1]);
    dv7shf_(p1, &j, &tg[1]);
    dv7shf_(p1, &j, &dst[1]);
L100:
    if (k < 0) {
	ipiv[*p1] = -ipiv[*p1];
    }
    *p1 = p1m1;
    if (*p1 <= 0) {
	goto L110;
    }
    dl7ivm_(p1, &w[1], &l[1], &tg[1]);
    gts = dd7tpr_(p1, &w[1], &w[1]);
    dl7itv_(p1, &w[1], &l[1], &w[1]);
    goto L30;

/*     ***  UNSCALE STEP  *** */

L110:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = (i__2 = ipiv[i__], abs(i__2));
	step[j] = dst[i__] / d__[j];
/* L120: */
    }

/*  ***  FUDGE STEP TO ENSURE THAT IT FORCES APPROPRIATE COMPONENTS */
/*  ***  TO THEIR BOUNDS  *** */

    if (*p1 >= p0) {
	goto L150;
    }
    k = *p1 + 1;
    i__1 = p0;
    for (i__ = k; i__ <= i__1; ++i__) {
	j = ipiv[i__];
	t = meps2;
	if (j > 0) {
	    goto L130;
	}
	t = -t;
	j = -j;
	ipiv[i__] = j;
L130:
/* Computing MAX */
	d__3 = (d__1 = x[j], abs(d__1)), d__4 = (d__2 = x0[j], abs(d__2));
	t *= max(d__3,d__4);
	step[j] += t;
/* L140: */
    }

L150:
    dv2axy_(p, &x[1], &one, &step[1], &x0[1]);
    if (*ns > 0) {
	dv7ipr_(&p0, &ipiv1[1], &td[1]);
    }
    return 0;
/*  ***  LAST LINE OF DS7BQN FOLLOWS  *** */
} /* ds7bqn_ */

/* Subroutine */ int n7msrt_(integer *n, integer *nmax, integer *num, integer 
	*mode, integer *index, integer *last, integer *next)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, jp, nmaxp1, nmaxp2;

/*     **********. */

/*     SUBROUTINE N7MSRT */

/*     GIVEN A SEQUENCE OF INTEGERS, THIS SUBROUTINE GROUPS */
/*     TOGETHER THOSE INDICES WITH THE SAME SEQUENCE VALUE */
/*     AND, OPTIONALLY, SORTS THE SEQUENCE INTO EITHER */
/*     ASCENDING OR DESCENDING ORDER. */

/*     THE SEQUENCE OF INTEGERS IS DEFINED BY THE ARRAY NUM, */
/*     AND IT IS ASSUMED THAT THE INTEGERS ARE EACH FROM THE SET */
/*     0,1,...,NMAX. ON OUTPUT THE INDICES K SUCH THAT NUM(K) = L */
/*     FOR ANY L = 0,1,...,NMAX CAN BE OBTAINED FROM THE ARRAYS */
/*     LAST AND NEXT AS FOLLOWS. */

/*           K = LAST(L+1) */
/*           WHILE (K .NE. 0) K = NEXT(K) */

/*     OPTIONALLY, THE SUBROUTINE PRODUCES AN ARRAY INDEX SO THAT */
/*     THE SEQUENCE NUM(INDEX(I)), I = 1,2,...,N IS SORTED. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE N7MSRT(N,NMAX,NUM,MODE,INDEX,LAST,NEXT) */

/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE. */

/*       NMAX IS A POSITIVE INTEGER INPUT VARIABLE. */

/*       NUM IS AN INPUT ARRAY OF LENGTH N WHICH CONTAINS THE */
/*         SEQUENCE OF INTEGERS TO BE GROUPED AND SORTED. IT */
/*         IS ASSUMED THAT THE INTEGERS ARE EACH FROM THE SET */
/*         0,1,...,NMAX. */

/*       MODE IS AN INTEGER INPUT VARIABLE. THE SEQUENCE NUM IS */
/*         SORTED IN ASCENDING ORDER IF MODE IS POSITIVE AND IN */
/*         DESCENDING ORDER IF MODE IS NEGATIVE. IF MODE IS 0, */
/*         NO SORTING IS DONE. */

/*       INDEX IS AN INTEGER OUTPUT ARRAY OF LENGTH N SET SO */
/*         THAT THE SEQUENCE */

/*               NUM(INDEX(I)), I = 1,2,...,N */

/*         IS SORTED ACCORDING TO THE SETTING OF MODE. IF MODE */
/*         IS 0, INDEX IS NOT REFERENCED. */

/*       LAST IS AN INTEGER OUTPUT ARRAY OF LENGTH NMAX + 1. THE */
/*         INDEX OF NUM FOR THE LAST OCCURRENCE OF L IS LAST(L+1) */
/*         FOR ANY L = 0,1,...,NMAX UNLESS LAST(L+1) = 0. IN */
/*         THIS CASE L DOES NOT APPEAR IN NUM. */

/*       NEXT IS AN INTEGER OUTPUT ARRAY OF LENGTH N. IF */
/*         NUM(K) = L, THEN THE INDEX OF NUM FOR THE PREVIOUS */
/*         OCCURRENCE OF L IS NEXT(K) FOR ANY L = 0,1,...,NMAX */
/*         UNLESS NEXT(K) = 0. IN THIS CASE THERE IS NO PREVIOUS */
/*         OCCURRENCE OF L IN NUM. */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     DETERMINE THE ARRAYS NEXT AND LAST. */

    /* Parameter adjustments */
    --next;
    --index;
    --num;
    --last;

    /* Function Body */
    nmaxp1 = *nmax + 1;
    i__1 = nmaxp1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	last[i__] = 0;
/* L10: */
    }
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	l = num[k];
	next[k] = last[l + 1];
	last[l + 1] = k;
/* L20: */
    }
    if (*mode == 0) {
	goto L60;
    }

/*     STORE THE POINTERS TO THE SORTED ARRAY IN INDEX. */

    i__ = 1;
    nmaxp2 = nmaxp1 + 1;
    i__1 = nmaxp1;
    for (j = 1; j <= i__1; ++j) {
	jp = j;
	if (*mode < 0) {
	    jp = nmaxp2 - j;
	}
	k = last[jp];
L30:
	if (k == 0) {
	    goto L40;
	}
	index[i__] = k;
	++i__;
	k = next[k];
	goto L30;
L40:
/* L50: */
	;
    }
L60:
    return 0;

/*     LAST CARD OF SUBROUTINE N7MSRT. */

} /* n7msrt_ */

/* Subroutine */ int dg7lit_(doublereal *d__, doublereal *g, integer *iv, 
	integer *liv, integer *lv, integer *p, integer *ps, doublereal *v, 
	doublereal *x, doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static doublereal e;
    static integer i__, j, k, l;
    static doublereal t;
    static integer h1, s1;
    static doublereal t1;
    static integer w1, g01, x01, hc1, dig1, qtr1, pp1o2, lmat1, rmat1, temp1, 
	    temp2, ipiv1, step1, dummy;
    extern logical stopx_(integer *);
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int df7hes_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), dl7vml_(integer *, doublereal *, doublereal *, 
	    doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dg7qts_(doublereal *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *, doublereal *, 
	    doublereal *, doublereal *), dl7mst_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dl7sqr_(
	    integer *, doublereal *, doublereal *);
    extern doublereal dl7svn_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int dl7tvm_(integer *, doublereal *, doublereal *,
	     doublereal *), dl7srt_(integer *, integer *, doublereal *, 
	    doublereal *, integer *), ds7lup_(doublereal *, doublereal *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *), ds7lvm_(integer *, 
	    doublereal *, doublereal *, doublereal *), dv2axy_(integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dv7cpy_(
	    integer *, doublereal *, doublereal *);
    extern doublereal dl7svx_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int dparck_(integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int ditsum_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *, doublereal *);
    static integer stpmod, lstgst, rstrst;
    static doublereal sttsst;


/*  ***  CARRY OUT NL2SOL-LIKE ITERATIONS FOR GENERALIZED LINEAR   *** */
/*  ***  REGRESSION PROBLEMS (AND OTHERS OF SIMILAR STRUCTURE)     *** */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* D.... SCALE VECTOR. */
/* IV... INTEGER VALUE ARRAY. */
/* LIV.. LENGTH OF IV.  MUST BE AT LEAST 82. */
/* LH... LENGTH OF H = P*(P+1)/2. */
/* LV... LENGTH OF V.  MUST BE AT LEAST P*(3*P + 19)/2 + 7. */
/* G.... GRADIENT AT X (WHEN IV(1) = 2). */
/* P.... NUMBER OF PARAMETERS (COMPONENTS IN X). */
/* PS... NUMBER OF NONZERO ROWS AND COLUMNS IN S. */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... PARAMETER VECTOR. */
/* Y.... PART OF YIELD VECTOR (WHEN IV(1)= 2, SCRATCH OTHERWISE). */

/*  ***  DISCUSSION  *** */

/*       DG7LIT PERFORMS NL2SOL-LIKE ITERATIONS FOR A VARIETY OF */
/*     REGRESSION PROBLEMS THAT ARE SIMILAR TO NONLINEAR LEAST-SQUARES */
/*     IN THAT THE HESSIAN IS THE SUM OF TWO TERMS, A READILY-COMPUTED */
/*     FIRST-ORDER TERM AND A SECOND-ORDER TERM.  THE CALLER SUPPLIES */
/*     THE FIRST-ORDER TERM OF THE HESSIAN IN HC (LOWER TRIANGLE, STORED */
/*     COMPACTLY BY ROWS IN V, STARTING AT IV(HC)), AND DG7LIT BUILDS AN */
/*     APPROXIMATION, S, TO THE SECOND-ORDER TERM.  THE CALLER ALSO */
/*     PROVIDES THE FUNCTION VALUE, GRADIENT, AND PART OF THE YIELD */
/*     VECTOR USED IN UPDATING S. DG7LIT DECIDES DYNAMICALLY WHETHER OR */
/*     NOT TO USE S WHEN CHOOSING THE NEXT STEP TO TRY...  THE HESSIAN */
/*     APPROXIMATION USED IS EITHER HC ALONE (GAUSS-NEWTON MODEL) OR */
/*     HC + S (AUGMENTED MODEL). */

/*        IF PS .LT. P, THEN ROWS AND COLUMNS PS+1...P OF S ARE KEPT */
/*     CONSTANT.  THEY WILL BE ZERO UNLESS THE CALLER SETS IV(INITS) TO */
/*     1 OR 2 AND SUPPLIES NONZERO VALUES FOR THEM, OR THE CALLER SETS */
/*     IV(INITS) TO 3 OR 4 AND THE FINITE-DIFFERENCE INITIAL S THEN */
/*     COMPUTED HAS NONZERO VALUES IN THESE ROWS. */

/*        IF IV(INITS) IS 3 OR 4, THEN THE INITIAL S IS COMPUTED BY */
/*     FINITE DIFFERENCES.  3 MEANS USE FUNCTION DIFFERENCES, 4 MEANS */
/*     USE GRADIENT DIFFERENCES.  FINITE DIFFERENCING IS DONE THE SAME */
/*     WAY AS IN COMPUTING A COVARIANCE MATRIX (WITH IV(COVREQ) = -1, -2, */
/*     1, OR 2). */

/*        FOR UPDATING S,DG7LIT ASSUMES THAT THE GRADIENT HAS THE FORM */
/*     OF A SUM OVER I OF RHO(I,X)*GRAD(R(I,X)), WHERE GRAD DENOTES THE */
/*     GRADIENT WITH RESPECT TO X.  THE TRUE SECOND-ORDER TERM THEN IS */
/*     THE SUM OVER I OF RHO(I,X)*HESSIAN(R(I,X)).  IF X = X0 + STEP, */
/*     THEN WE WISH TO UPDATE S SO THAT S*STEP IS THE SUM OVER I OF */
/*     RHO(I,X)*(GRAD(R(I,X)) - GRAD(R(I,X0))).  THE CALLER MUST SUPPLY */
/*     PART OF THIS IN Y, NAMELY THE SUM OVER I OF */
/*     RHO(I,X)*GRAD(R(I,X0)), WHEN CALLING DG7LIT WITH IV(1) = 2 AND */
/*     IV(MODE) = 0 (WHERE MODE = 38).  G THEN CONTANS THE OTHER PART, */
/*     SO THAT THE DESIRED YIELD VECTOR IS G - Y.  IF PS .LT. P, THEN */
/*     THE ABOVE DISCUSSION APPLIES ONLY TO THE FIRST PS COMPONENTS OF */
/*     GRAD(R(I,X)), STEP, AND Y. */

/*        PARAMETERS IV, P, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO NL2SOL (WHICH SEE), EXCEPT THAT V CAN BE SHORTER */
/*     (SINCE THE PART OF V THAT NL2SOL USES FOR STORING D, J, AND R IS */
/*     NOT NEEDED).  MOREOVER, COMPARED WITH NL2SOL, IV(1) MAY HAVE THE */
/*     TWO ADDITIONAL OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, */
/*     AS IS THE USE OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUES IV(D), */
/*     IV(J), AND IV(R), WHICH ARE OUTPUT VALUES FROM NL2SOL (AND */
/*     NL2SNO), ARE NOT REFERENCED BY DG7LIT OR THE SUBROUTINES IT CALLS. */

/*        WHEN DG7LIT IS FIRST CALLED, I.E., WHEN DG7LIT IS CALLED WITH */
/*     IV(1) = 0 OR 12, V(F), G, AND HC NEED NOT BE INITIALIZED.  TO */
/*     OBTAIN THESE STARTING VALUES,DG7LIT RETURNS FIRST WITH IV(1) = 1, */
/*     THEN WITH IV(1) = 2, WITH IV(MODE) = -1 IN BOTH CASES.  ON */
/*     SUBSEQUENT RETURNS WITH IV(1) = 2, IV(MODE) = 0 IMPLIES THAT */
/*     Y MUST ALSO BE SUPPLIED.  (NOTE THAT Y IS USED FOR SCRATCH -- ITS */
/*     INPUT CONTENTS ARE LOST.  BY CONTRAST, HC IS NEVER CHANGED.) */
/*     ONCE CONVERGENCE HAS BEEN OBTAINED, IV(RDREQ) AND IV(COVREQ) MAY */
/*     IMPLY THAT A FINITE-DIFFERENCE HESSIAN SHOULD BE COMPUTED FOR USE */
/*     IN COMPUTING A COVARIANCE MATRIX.  IN THIS CASE DG7LIT WILL MAKE A */
/*     NUMBER OF RETURNS WITH IV(1) = 1 OR 2 AND IV(MODE) POSITIVE. */
/*     WHEN IV(MODE) IS POSITIVE, Y SHOULD NOT BE CHANGED. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET V(F) (I.E., V(10)) TO F(X), THE */
/*             FUNCTION VALUE AT X, AND CALL DG7LIT AGAIN, HAVING CHANGED */
/*             NONE OF THE OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) */
/*             CANNOT BE EVALUATED (E.G. IF OVERFLOW WOULD OCCUR), WHICH */
/*             MAY HAPPEN BECAUSE OF AN OVERSIZED STEP.  IN THIS CASE */
/*             THE CALLER SHOULD SET IV(TOOBIG) = IV(2) TO 1, WHICH WILL */
/*             CAUSE DG7LIT TO IGNORE V(F) AND TRY A SMALLER STEP.  NOTE */
/*             THAT THE CURRENT FUNCTION EVALUATION COUNT IS AVAILABLE */
/*             IN IV(NFCALL) = IV(6).  THIS MAY BE USED TO IDENTIFY */
/*             WHICH COPY OF SAVED INFORMATION SHOULD BE USED IN COM- */
/*             PUTING G, HC, AND Y THE NEXT TIME DG7LIT RETURNS WITH */
/*             IV(1) = 2.  SEE MLPIT FOR AN EXAMPLE OF THIS. */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT OF F AT */
/*             X.  THE CALLER SHOULD ALSO SET HC TO THE GAUSS-NEWTON */
/*             HESSIAN AT X.  IF IV(MODE) = 0, THEN THE CALLER SHOULD */
/*             ALSO COMPUTE THE PART OF THE YIELD VECTOR DESCRIBED ABOVE. */
/*             THE CALLER SHOULD THEN CALL DG7LIT AGAIN (WITH IV(1) = 2). */
/*             THE CALLER MAY ALSO CHANGE D AT THIS TIME, BUT SHOULD NOT */
/*             CHANGE X.  NOTE THAT IV(NFGCAL) = IV(7) CONTAINS THE */
/*             VALUE THAT IV(NFCALL) HAD DURING THE RETURN WITH */
/*             IV(1) = 1 IN WHICH X HAD THE SAME VALUE AS IT NOW HAS. */
/*             IV(NFGCAL) IS EITHER IV(NFCALL) OR IV(NFCALL) - 1.  MLPIT */
/*             IS AN EXAMPLE WHERE THIS INFORMATION IS USED.  IF G OR HC */
/*             CANNOT BE EVALUATED AT X, THEN THE CALLER MAY SET */
/*             IV(TOOBIG) TO 1, IN WHICH CASE DG7LIT WILL RETURN WITH */
/*             IV(1) = 15. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED IN PART BY D.O.E. GRANT EX-76-A-01-2295 TO MIT/CCREMS. */

/*        (SEE NL2SOL FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DF7HES.... COMPUTE FINITE-DIFFERENCE HESSIAN (FOR COVARIANCE). */
/* DG7QTS.... COMPUTES GOLDFELD-QUANDT-TROTTER STEP (AUGMENTED MODEL). */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DL7MST... COMPUTES LEVENBERG-MARQUARDT STEP (GAUSS-NEWTON MODEL). */
/* DL7SRT.... COMPUTES CHOLESKY FACTOR OF (LOWER TRIANG. OF) SYM. MATRIX. */
/* DL7SQR... COMPUTES L * L**T FROM LOWER TRIANGULAR MATRIX L. */
/* DL7TVM... COMPUTES L**T * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DL7SVX... ESTIMATES LARGEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DL7SVN... ESTIMATES SMALLEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DL7VML.... COMPUTES L * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DPARCK.... CHECK VALIDITY OF IV AND V INPUT COMPONENTS. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* DR7MDC... RETURNS MACHINE-DEPENDENT CONSTANTS. */
/* DS7LUP... PERFORMS QUASI-NEWTON UPDATE ON COMPACTLY STORED LOWER TRI- */
/*             ANGLE OF A SYMMETRIC MATRIX. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */


/*  ***  V SUBSCRIPT VALUES  *** */




/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --y;
    --x;
    --g;
    --d__;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L40;
    }
    if (i__ == 2) {
	goto L50;
    }

    if (i__ == 12 || i__ == 13) {
	iv[4] = iv[4] + *p * (*p * 3 + 19) / 2 + 7;
    }
    dparck_(&c__1, &d__[1], &iv[1], liv, lv, p, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    switch (i__) {
	case 1:  goto L290;
	case 2:  goto L290;
	case 3:  goto L290;
	case 4:  goto L290;
	case 5:  goto L290;
	case 6:  goto L290;
	case 7:  goto L170;
	case 8:  goto L120;
	case 9:  goto L170;
	case 10:  goto L10;
	case 11:  goto L10;
	case 12:  goto L20;
    }

/*  ***  STORAGE ALLOCATION  *** */

L10:
    pp1o2 = *p * (*p + 1) / 2;
    iv[62] = iv[42] + pp1o2;
    iv[43] = iv[62] + pp1o2;
    iv[40] = iv[43] + *p;
    iv[41] = iv[40] + *p;
    iv[37] = iv[41] + *p;
    iv[65] = iv[37] + *p;
    iv[56] = iv[65] + (*p << 2) + 7;
    iv[47] = iv[56] + pp1o2;
    if (iv[1] != 13) {
	goto L20;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L20:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[11] = 2;
    iv[2] = 0;
    iv[55] = 0;
    iv[26] = 0;
    iv[52] = 0;
    iv[53] = 0;
    iv[8] = 0;
    iv[9] = 0;
    iv[74] = 0;
    v[9] = 0.;
    v[5] = 0.;
    v[8] = v[35] / (v[21] + 1.);

/*  ***  SET INITIAL MODEL AND S MATRIX  *** */

    iv[5] = 1;
    if (iv[62] < 0) {
	goto L999;
    }
    if (iv[25] > 1) {
	iv[5] = 2;
    }
    s1 = iv[62];
    if (iv[25] == 0 || iv[25] > 2) {
	i__1 = *p * (*p + 1) / 2;
	dv7scp_(&i__1, &v[s1], &c_b14);
    }
    iv[1] = 1;
    j = iv[76];
    if (j <= 0) {
	goto L999;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iv[j] = i__;
	++j;
/* L30: */
    }
    goto L999;

/*  ***  NEW FUNCTION VALUE  *** */

L40:
    if (iv[35] == 0) {
	goto L290;
    }
    if (iv[35] > 0) {
	goto L520;
    }

    iv[1] = 2;
    if (iv[2] == 0) {
	goto L999;
    }
    iv[1] = 63;
    goto L999;

/*  ***  NEW GRADIENT  *** */

L50:
    iv[34] = -1;
    iv[33] = -1;
    iv[74] = 0;
    if (iv[35] > 0) {
	goto L520;
    }

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

    if (iv[2] == 0) {
	goto L60;
    }
    iv[1] = 65;
    goto L999;
L60:
    if (iv[71] <= 0 && iv[78] <= 0) {
	goto L610;
    }

/*  ***  COMPUTE  D**-1 * GRADIENT  *** */

    dig1 = iv[37];
    k = dig1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = g[i__] / d__[i__];
	++k;
/* L70: */
    }
    v[1] = dv2nrm_(p, &v[dig1]);

    if (iv[55] != 0) {
	goto L510;
    }
    if (iv[35] == 0) {
	goto L440;
    }
    iv[35] = 0;
    v[13] = v[10];
    if (iv[25] <= 2) {
	goto L100;
    }

/*  ***  ARRANGE FOR FINITE-DIFFERENCE INITIAL S  *** */

    iv[13] = iv[15];
    iv[15] = -1;
    if (iv[25] > 3) {
	iv[15] = 1;
    }
    iv[55] = 70;
    goto L530;

/*  ***  COME TO NEXT STMT AFTER COMPUTING F.D. HESSIAN FOR INIT. S  *** */

L80:
    iv[55] = 0;
    iv[35] = 0;
    iv[52] = 0;
    iv[53] = 0;
    iv[15] = iv[13];
    s1 = iv[62];
    pp1o2 = *ps * (*ps + 1) / 2;
    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L90;
    }
    dv2axy_(&pp1o2, &v[s1], &c_b90, &v[hc1], &v[h1]);
    goto L100;
L90:
    rmat1 = iv[78];
    dl7sqr_(ps, &v[s1], &v[rmat1]);
    dv2axy_(&pp1o2, &v[s1], &c_b90, &v[s1], &v[h1]);
L100:
    iv[1] = 2;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L110:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, p, &v[1], &x[1]);
L120:
    k = iv[31];
    if (k < iv[18]) {
	goto L130;
    }
    iv[1] = 10;
    goto L999;
L130:
    iv[31] = k + 1;

/*  ***  UPDATE RADIUS  *** */

    if (k == 0) {
	goto L150;
    }
    step1 = iv[40];
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[step1] = d__[i__] * v[step1];
	++step1;
/* L140: */
    }
    step1 = iv[40];
    t = v[16] * dv2nrm_(p, &v[step1]);
    if (v[16] < 1. || t > v[8]) {
	v[8] = t;
    }

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

L150:
    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[56] = -abs(iv[56]);
    iv[64] = iv[5];

/*     ***  COPY X TO X0  *** */

    dv7cpy_(p, &v[x01], &x[1]);

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L160:
    if (! stopx_(&dummy)) {
	goto L180;
    }
    iv[1] = 11;
    goto L190;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L170:
    if (v[10] >= v[13]) {
	goto L180;
    }
    v[16] = 1.;
    k = iv[31];
    goto L130;

L180:
    if (iv[6] < iv[17] + iv[52]) {
	goto L200;
    }
    iv[1] = 9;
L190:
    if (v[10] >= v[13]) {
	goto L999;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L430;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L200:
    step1 = iv[40];
    w1 = iv[65];
    h1 = iv[56];
    t1 = 1.;
    if (iv[5] == 2) {
	goto L210;
    }
    t1 = 0.;

/*        ***  COMPUTE LEVENBERG-MARQUARDT STEP IF POSSIBLE... */

    rmat1 = iv[78];
    if (rmat1 <= 0) {
	goto L210;
    }
    qtr1 = iv[77];
    if (qtr1 <= 0) {
	goto L210;
    }
    ipiv1 = iv[76];
    dl7mst_(&d__[1], &g[1], &iv[75], &iv[ipiv1], &iv[34], p, &v[qtr1], &v[
	    rmat1], &v[step1], &v[1], &v[w1]);
/*        *** H IS STORED IN THE END OF W AND HAS JUST BEEN OVERWRITTEN, */
/*        *** SO WE MARK IT INVALID... */
    iv[56] = -abs(h1);
/*        *** EVEN IF H WERE STORED ELSEWHERE, IT WOULD BE NECESSARY TO */
/*        *** MARK INVALID THE INFORMATION DG7QTS MAY HAVE STORED IN V... */
    iv[33] = -1;
    goto L260;

L210:
    if (h1 > 0) {
	goto L250;
    }

/*     ***  SET H TO  D**-1 * (HC + T1*S) * D**-1.  *** */

    h1 = -h1;
    iv[56] = h1;
    iv[74] = 0;
    j = iv[71];
    if (j > 0) {
	goto L220;
    }
    j = h1;
    rmat1 = iv[78];
    dl7sqr_(p, &v[h1], &v[rmat1]);
L220:
    s1 = iv[62];
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = 1. / d__[i__];
	i__2 = i__;
	for (k = 1; k <= i__2; ++k) {
	    v[h1] = t * (v[j] + t1 * v[s1]) / d__[k];
	    ++j;
	    ++h1;
	    ++s1;
/* L230: */
	}
/* L240: */
    }
    h1 = iv[56];
    iv[33] = -1;

/*  ***  COMPUTE ACTUAL GOLDFELD-QUANDT-TROTTER STEP  *** */

L250:
    dig1 = iv[37];
    lmat1 = iv[42];
    dg7qts_(&d__[1], &v[dig1], &v[h1], &iv[33], &v[lmat1], p, &v[step1], &v[1]
	    , &v[w1]);
    if (iv[34] > 0) {
	iv[34] = 0;
    }

L260:
    if (iv[29] != 6) {
	goto L270;
    }
    if (iv[9] != 2) {
	goto L290;
    }
    rstrst = 2;
    goto L300;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L270:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L290;
    }
    if (iv[29] != 5) {
	goto L280;
    }
    if (v[16] <= 1.) {
	goto L280;
    }
    if (v[7] > v[11] * 1.2) {
	goto L280;
    }
    step1 = iv[40];
    x01 = iv[43];
    dv2axy_(p, &v[step1], &c_b90, &v[x01], &x[1]);
    if (iv[9] != 2) {
	goto L290;
    }
    rstrst = 0;
    goto L300;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L280:
    x01 = iv[43];
    step1 = iv[40];
    dv2axy_(p, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L999;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L290:
    rstrst = 3;
L300:
    x01 = iv[43];
    v[17] = drldst_(p, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = iv[41];
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L340;
	case 2:  goto L310;
	case 3:  goto L320;
	case 4:  goto L330;
    }
L310:
    dv7cpy_(p, &x[1], &v[x01]);
    goto L340;
L320:
    dv7cpy_(p, &v[lstgst], &v[step1]);
    goto L340;
L330:
    dv7cpy_(p, &v[step1], &v[lstgst]);
    dv2axy_(p, &x[1], &c_b85, &v[step1], &v[x01]);
    v[17] = drldst_(p, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

/*  ***  IF NECESSARY, SWITCH MODELS  *** */

L340:
    if (iv[12] == 0) {
	goto L350;
    }
    iv[56] = -abs(iv[56]);
    iv[64] += 2;
    l = iv[60];
    dv7cpy_(&c__9, &v[1], &v[l]);
L350:
    l = iv[29] - 4;
    stpmod = iv[5];
    if (l > 0) {
	switch (l) {
	    case 1:  goto L370;
	    case 2:  goto L380;
	    case 3:  goto L390;
	    case 4:  goto L390;
	    case 5:  goto L390;
	    case 6:  goto L390;
	    case 7:  goto L390;
	    case 8:  goto L390;
	    case 9:  goto L500;
	    case 10:  goto L440;
	}
    }

/*  ***  DECIDE WHETHER TO CHANGE MODELS  *** */

    e = v[7] - v[11];
    s1 = iv[62];
    ds7lvm_(ps, &y[1], &v[s1], &v[step1]);
    sttsst = dd7tpr_(ps, &v[step1], &y[1]) * .5;
    if (iv[5] == 1) {
	sttsst = -sttsst;
    }
    if ((d__1 = e + sttsst, abs(d__1)) * v[45] >= abs(e)) {
	goto L360;
    }

/*     ***  SWITCH MODELS  *** */

    iv[5] = 3 - iv[5];
    if (-2 < l) {
	goto L400;
    }
    iv[56] = -abs(iv[56]);
    iv[64] += 2;
    l = iv[60];
    dv7cpy_(&c__9, &v[l], &v[1]);
    goto L160;

L360:
    if (-3 < l) {
	goto L400;
    }

/*  ***  RECOMPUTE STEP WITH NEW RADIUS  *** */

L370:
    v[8] = v[16] * v[2];
    goto L160;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST */

L380:
    v[8] = v[36];
    goto L200;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L390:
    iv[55] = l;
    if (v[10] >= v[13]) {
	goto L510;
    }
    if (iv[13] == 14) {
	goto L510;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L400:
    iv[26] = 0;
    iv[67] = 0;

/*  ***  SEE WHETHER TO SET V(RADFAC) BY GRADIENT TESTS  *** */

    if (iv[29] != 3) {
	goto L430;
    }
    step1 = iv[40];
    temp1 = iv[41];
    temp2 = iv[65];

/*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L410;
    }
    ds7lvm_(p, &v[temp1], &v[hc1], &v[step1]);
    goto L420;
L410:
    rmat1 = iv[78];
    dl7tvm_(p, &v[temp1], &v[rmat1], &v[step1]);
    dl7vml_(p, &v[temp1], &v[rmat1], &v[temp1]);

L420:
    if (stpmod == 1) {
	goto L430;
    }
    s1 = iv[62];
    ds7lvm_(ps, &v[temp2], &v[s1], &v[step1]);
    dv2axy_(ps, &v[temp1], &c_b85, &v[temp2], &v[temp1]);

/*  ***  SAVE OLD GRADIENT AND COMPUTE NEW ONE  *** */

L430:
    ++iv[30];
    g01 = iv[65];
    dv7cpy_(p, &v[g01], &g[1]);
    iv[1] = 2;
    iv[2] = 0;
    goto L999;

/*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L440:
    g01 = iv[65];
    dv2axy_(p, &v[g01], &c_b90, &v[g01], &g[1]);
    step1 = iv[40];
    temp1 = iv[41];
    temp2 = iv[65];
    if (iv[29] != 3) {
	goto L470;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

/*     ***  SET  TEMP1 = D**-1 * (HESSIAN * STEP  +  (G(X0) - G(X)))  *** */

    k = temp1;
    l = g01;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = (v[k] - v[l]) / d__[i__];
	++k;
	++l;
/* L450: */
    }

/*        ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(p, &v[temp1]) <= v[1] * v[29]) {
	goto L460;
    }
    if (dd7tpr_(p, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L470;
    }
L460:
    v[16] = v[23];

/*  ***  COMPUTE Y VECTOR NEEDED FOR UPDATING S  *** */

L470:
    dv2axy_(ps, &y[1], &c_b90, &y[1], &g[1]);

/*  ***  DETERMINE SIZING FACTOR V(SIZE)  *** */

/*     ***  SET TEMP1 = S * STEP  *** */
    s1 = iv[62];
    ds7lvm_(ps, &v[temp1], &v[s1], &v[step1]);

    t1 = (d__1 = dd7tpr_(ps, &v[step1], &v[temp1]), abs(d__1));
    t = (d__1 = dd7tpr_(ps, &v[step1], &y[1]), abs(d__1));
    v[55] = 1.;
    if (t < t1) {
	v[55] = t / t1;
    }

/*  ***  SET G0 TO WCHMTD CHOICE OF FLETCHER AND AL-BAALI  *** */

    hc1 = iv[71];
    if (hc1 <= 0) {
	goto L480;
    }
    ds7lvm_(ps, &v[g01], &v[hc1], &v[step1]);
    goto L490;

L480:
    rmat1 = iv[78];
    dl7tvm_(ps, &v[g01], &v[rmat1], &v[step1]);
    dl7vml_(ps, &v[g01], &v[rmat1], &v[g01]);

L490:
    dv2axy_(ps, &v[g01], &c_b85, &y[1], &v[g01]);

/*  ***  UPDATE S  *** */

    ds7lup_(&v[s1], &v[47], ps, &v[55], &v[step1], &v[temp1], &v[temp2], &v[
	    g01], &v[56], &y[1]);
    iv[1] = 2;
    goto L110;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L500:
    iv[1] = 64;
    goto L999;


/*  ***  CONVERGENCE OBTAINED -- SEE WHETHER TO COMPUTE COVARIANCE  *** */

L510:
    if (iv[57] == 0) {
	goto L600;
    }
    if (iv[74] != 0) {
	goto L600;
    }
    if (iv[55] >= 7) {
	goto L600;
    }
    if (iv[67] > 0) {
	goto L600;
    }
    if (iv[26] > 0) {
	goto L600;
    }
    if (abs(iv[15]) >= 3) {
	goto L560;
    }
    if (iv[9] == 0) {
	iv[9] = 2;
    }
    goto L530;

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN FOR COMPUTING COVARIANCE  *** */

L520:
    iv[9] = 0;
L530:
    df7hes_(&d__[1], &g[1], &i__, &iv[1], liv, lv, p, &v[1], &x[1]);
    switch (i__) {
	case 1:  goto L540;
	case 2:  goto L550;
	case 3:  goto L580;
    }
L540:
    ++iv[52];
    ++iv[6];
    iv[1] = 1;
    goto L999;

L550:
    ++iv[53];
    ++iv[30];
    iv[7] = iv[6] + iv[53];
    iv[1] = 2;
    goto L999;

L560:
    h1 = abs(iv[56]);
    iv[56] = -h1;
    pp1o2 = *p * (*p + 1) / 2;
    rmat1 = iv[78];
    if (rmat1 <= 0) {
	goto L570;
    }
    lmat1 = iv[42];
    dv7cpy_(&pp1o2, &v[lmat1], &v[rmat1]);
    v[53] = 0.;
    goto L590;
L570:
    hc1 = iv[71];
    iv[74] = h1;
    i__1 = *p * (*p + 1) / 2;
    dv7cpy_(&i__1, &v[h1], &v[hc1]);

/*  ***  COMPUTE CHOLESKY FACTOR OF FINITE-DIFFERENCE HESSIAN */
/*  ***  FOR USE IN CALLER*S COVARIANCE CALCULATION... */

L580:
    lmat1 = iv[42];
    h1 = iv[74];
    if (h1 <= 0) {
	goto L600;
    }
    if (iv[55] == 70) {
	goto L80;
    }
    dl7srt_(&c__1, p, &v[lmat1], &v[h1], &i__);
    iv[74] = -1;
    v[53] = 0.;
    if (i__ != 0) {
	goto L600;
    }

L590:
    iv[74] = -1;
    step1 = iv[40];
    t = dl7svn_(p, &v[lmat1], &v[step1], &v[step1]);
    if (t <= 0.) {
	goto L600;
    }
    t /= dl7svx_(p, &v[lmat1], &v[step1], &v[step1]);
    if (t > dr7mdc_(&c__4)) {
	iv[74] = h1;
    }
    v[53] = t;

L600:
    iv[35] = 0;
    iv[1] = iv[55];
    iv[55] = 0;
    goto L999;

/*  ***  SPECIAL RETURN FOR MISSING HESSIAN INFORMATION -- BOTH */
/*  ***  IV(HC) .LE. 0 AND IV(RMAT) .LE. 0 */

L610:
    iv[1] = 1400;

L999:
    return 0;

/*  ***  LAST LINE OF DG7LIT FOLLOWS  *** */
} /* dg7lit_ */

/* Subroutine */ int dl7msb_(doublereal *b, doublereal *d__, doublereal *g, 
	integer *ierr, integer *ipiv, integer *ipiv1, integer *ipiv2, integer 
	*ka, doublereal *lmat, integer *lv, integer *p, integer *p0, integer *
	pc, doublereal *qtr, doublereal *rmat, doublereal *step, doublereal *
	td, doublereal *tg, doublereal *v, doublereal *w, doublereal *wlm, 
	doublereal *x, doublereal *x0)
{
    /* Initialized data */

    static doublereal one = 1.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer step_dim1, step_offset, i__1;

    /* Local variables */
    static integer i__, j, k, l, k0, p1, kb, p10, p11, ns;
    static doublereal ds0, rad, nred, pred;
    static integer kinit;
    extern /* Subroutine */ int dd7mlp_(integer *, doublereal *, doublereal *,
	     doublereal *, integer *), ds7bqn_(doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, 
	    doublereal *, integer *, integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , dq7rsh_(integer *, integer *, logical *, doublereal *, 
	    doublereal *, doublereal *), dl7mst_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dv7ipr_(
	    integer *, integer *, doublereal *), dv7cpy_(integer *, 
	    doublereal *, doublereal *), dl7tvm_(integer *, doublereal *, 
	    doublereal *, doublereal *), dv2axy_(integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), dv7vmp_(integer *, 
	    doublereal *, doublereal *, doublereal *, integer *);


/*  ***  COMPUTE HEURISTIC BOUNDED NEWTON STEP  *** */

/*     DIMENSION LMAT(P*(P+1)/2), RMAT(P*(P+1)/2), WLM(P*(P+5)/2 + 4) */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  V SUBSCRIPTS  *** */


    /* Parameter adjustments */
    --lmat;
    --v;
    --x0;
    --x;
    --w;
    --tg;
    --td;
    step_dim1 = *p;
    step_offset = 1 + step_dim1;
    step -= step_offset;
    --qtr;
    --ipiv2;
    --ipiv1;
    --ipiv;
    --g;
    --d__;
    b -= 3;
    --rmat;
    --wlm;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    p1 = *pc;
    if (*ka < 0) {
	goto L10;
    }
    nred = v[6];
    ds0 = v[3];
    goto L20;
L10:
    *p0 = 0;
    *ka = -1;

L20:
    kinit = -1;
    if (*p0 == p1) {
	kinit = *ka;
    }
    dv7cpy_(p, &x[1], &x0[1]);
    dv7cpy_(p, &td[1], &d__[1]);
/*     *** USE STEP(1,3) AS TEMP. COPY OF QTR *** */
    dv7cpy_(p, &step[step_dim1 * 3 + 1], &qtr[1]);
    dv7ipr_(p, &ipiv[1], &td[1]);
    pred = zero;
    rad = v[8];
    kb = -1;
    v[2] = zero;
    if (p1 > 0) {
	goto L30;
    }
    nred = zero;
    ds0 = zero;
    dv7scp_(p, &step[step_offset], &zero);
    goto L90;

L30:
    dv7vmp_(p, &tg[1], &g[1], &d__[1], &c_n1);
    dv7ipr_(p, &ipiv[1], &tg[1]);
    p10 = p1;
L40:
    k = kinit;
    kinit = -1;
    v[8] = rad - v[2];
    dv7vmp_(&p1, &tg[1], &tg[1], &td[1], &c__1);
    i__1 = p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L50: */
	ipiv1[i__] = i__;
    }
    k0 = max(0,k);
    dl7mst_(&td[1], &tg[1], ierr, &ipiv1[1], &k, &p1, &step[step_dim1 * 3 + 1]
	    , &rmat[1], &step[step_offset], &v[1], &wlm[1]);
    dv7vmp_(&p1, &tg[1], &tg[1], &td[1], &c_n1);
    *p0 = p1;
    if (*ka >= 0) {
	goto L60;
    }
    nred = v[6];
    ds0 = v[3];

L60:
    *ka = k;
    v[8] = rad;
    l = p1 + 5;
    if (k <= k0) {
	dd7mlp_(&p1, &lmat[1], &td[1], &rmat[1], &c_n1);
    }
    if (k > k0) {
	dd7mlp_(&p1, &lmat[1], &td[1], &wlm[l], &c_n1);
    }
    ds7bqn_(&b[3], &d__[1], &step[(step_dim1 << 1) + 1], &ipiv[1], &ipiv1[1], 
	    &ipiv2[1], &kb, &lmat[1], lv, &ns, p, &p1, &step[step_offset], &
	    td[1], &tg[1], &v[1], &w[1], &x[1], &x0[1]);
    pred += v[7];
    if (ns == 0) {
	goto L80;
    }
    *p0 = 0;

/*  ***  UPDATE RMAT AND QTR  *** */

    p11 = p1 + 1;
    l = p10 + p11;
    i__1 = p10;
    for (k = p11; k <= i__1; ++k) {
	j = l - k;
	i__ = ipiv2[j];
	if (i__ < j) {
	    dq7rsh_(&i__, &j, &c_true, &qtr[1], &rmat[1], &w[1]);
	}
/* L70: */
    }

L80:
    if (kb > 0) {
	goto L90;
    }

/*  ***  UPDATE LOCAL COPY OF QTR  *** */

    dv7vmp_(&p10, &w[1], &step[(step_dim1 << 1) + 1], &td[1], &c_n1);
    dl7tvm_(&p10, &w[1], &lmat[1], &w[1]);
    dv2axy_(&p10, &step[step_dim1 * 3 + 1], &one, &w[1], &qtr[1]);
    goto L40;

L90:
    v[3] = ds0;
    v[6] = nred;
    v[7] = pred;
    v[4] = dd7tpr_(p, &g[1], &step[step_offset]);

    return 0;
/*  ***  LAST LINE OF DL7MSB FOLLOWS  *** */
} /* dl7msb_ */

/* Subroutine */ int dn2lrd_(doublereal *dr, integer *iv, doublereal *l, 
	integer *lh, integer *liv, integer *lv, integer *nd, integer *nn, 
	integer *p, doublereal *r__, doublereal *rd, doublereal *v)
{
    /* Initialized data */

    static doublereal onev[1] = { 1. };

    /* System generated locals */
    integer dr_dim1, dr_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal a;
    static integer i__, j, m;
    static doublereal s, t, ff;
    static integer cov, step1;
    extern /* Subroutine */ int do7prd_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *), dl7ivm_(
	    integer *, doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , dl7itv_(integer *, doublereal *, doublereal *, doublereal *);


/*  ***  COMPUTE REGRESSION DIAGNOSTIC AND DEFAULT COVARIANCE MATRIX FOR */
/*        DRN2G  *** */

/*  ***  PARAMETERS  *** */


/*  ***  CODED BY DAVID M. GAY (WINTER 1982, FALL 1983)  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  CONSTANTS  *** */


/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */

/*  ***  IV AND V SUBSCRIPTS  *** */

    /* Parameter adjustments */
    --l;
    --iv;
    --v;
    --rd;
    --r__;
    dr_dim1 = *nd;
    dr_offset = 1 + dr_dim1;
    dr -= dr_offset;

    /* Function Body */

/* ++++++++++++++++++++++++++++++++  BODY  +++++++++++++++++++++++++++++++ */

    step1 = iv[40];
    i__ = iv[57];
    if (i__ <= 0) {
	goto L999;
    }
    if (i__ % 4 < 2) {
	goto L30;
    }
    ff = 1.;
    if (v[10] != 0.) {
	ff = 1. / sqrt((abs(v[10])));
    }
    dv7scp_(nn, &rd[1], &c_b90);
    i__1 = *nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing 2nd power */
	d__1 = r__[i__];
	a = d__1 * d__1;
	m = step1;
	i__2 = *p;
	for (j = 1; j <= i__2; ++j) {
	    v[m] = dr[i__ + j * dr_dim1];
	    ++m;
/* L10: */
	}
	dl7ivm_(p, &v[step1], &l[1], &v[step1]);
	s = dd7tpr_(p, &v[step1], &v[step1]);
	t = 1. - s;
	if (t <= 0.) {
	    goto L20;
	}
	a = a * s / t;
	rd[i__] = sqrt(a) * ff;
L20:
	;
    }

L30:
    if (iv[35] - *p < 2) {
	goto L999;
    }

/*  ***  COMPUTE DEFAULT COVARIANCE MATRIX  *** */

    cov = abs(iv[56]);
    i__1 = *nn;
    for (i__ = 1; i__ <= i__1; ++i__) {
	m = step1;
	i__2 = *p;
	for (j = 1; j <= i__2; ++j) {
	    v[m] = dr[i__ + j * dr_dim1];
	    ++m;
/* L40: */
	}
	dl7ivm_(p, &v[step1], &l[1], &v[step1]);
	dl7itv_(p, &v[step1], &l[1], &v[step1]);
	do7prd_(&c__1, lh, p, &v[cov], onev, &v[step1], &v[step1]);
/* L50: */
    }

L999:
    return 0;
/*  ***  LAST LINE OF DN2LRD FOLLOWS  *** */
} /* dn2lrd_ */

/* Subroutine */ int dr7tvm_(integer *n, integer *p, doublereal *y, 
	doublereal *d__, doublereal *u, doublereal *x)
{
    /* System generated locals */
    integer u_dim1, u_offset, i__1, i__2;

    /* Local variables */
    static integer i__;
    static doublereal t;
    static integer ii, pl, pp1;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);


/*  ***  SET Y TO R*X, WHERE R IS THE UPPER TRIANGULAR MATRIX WHOSE */
/*  ***  DIAGONAL IS IN D AND WHOSE STRICT UPPER TRIANGLE IS IN U. */

/*  ***  X AND Y MAY SHARE STORAGE. */



/*  ***  LOCAL VARIABLES  *** */


/*  ***  BODY  *** */

    /* Parameter adjustments */
    --x;
    u_dim1 = *n;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    --d__;
    --y;

    /* Function Body */
    pl = min(*n,*p);
    pp1 = pl + 1;
    i__1 = pl;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = pp1 - ii;
	t = x[i__] * d__[i__];
	if (i__ > 1) {
	    i__2 = i__ - 1;
	    t += dd7tpr_(&i__2, &u[i__ * u_dim1 + 1], &x[1]);
	}
	y[i__] = t;
/* L10: */
    }
    return 0;
/*  ***  LAST LINE OF DR7TVM FOLLOWS  *** */
} /* dr7tvm_ */

/* Subroutine */ int dq7rad_(integer *n, integer *nn, integer *p, doublereal *
	qtr, logical *qtrset, doublereal *rmat, doublereal *w, doublereal *y)
{
    /* Initialized data */

    static doublereal big = -1.;
    static doublereal bigrt = -1.;
    static doublereal one = 1.;
    static doublereal tiny = 0.;
    static doublereal tinyrt = 0.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer w_dim1, w_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal s, t;
    static integer ii, ij, nk;
    static doublereal ri, wi;
    static integer ip1;
    static doublereal ari, qri;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dv7scl_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *);


/*  ***  ADD ROWS W TO QR FACTORIZATION WITH R MATRIX RMAT AND */
/*  ***  Q**T * RESIDUAL = QTR.  Y = NEW COMPONENTS OF RESIDUAL */
/*  ***  CORRESPONDING TO W.  QTR, Y REFERENCED ONLY IF QTRSET = .TRUE. */

/*     DIMENSION RMAT(P*(P+1)/2) */
/* /+ */
/* / */

/*  ***  LOCAL VARIABLES  *** */

    /* Parameter adjustments */
    --y;
    w_dim1 = *nn;
    w_offset = 1 + w_dim1;
    w -= w_offset;
    --qtr;
    --rmat;

    /* Function Body */

/* ------------------------------ BODY ----------------------------------- */

    if (tiny > zero) {
	goto L10;
    }
    tiny = dr7mdc_(&c__1);
    big = dr7mdc_(&c__6);
    if (tiny * big < one) {
	tiny = one / big;
    }
L10:
    k = 1;
    nk = *n;
    ii = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ii += i__;
	ip1 = i__ + 1;
	ij = ii + i__;
	if (nk <= 1) {
	    t = (d__1 = w[k + i__ * w_dim1], abs(d__1));
	}
	if (nk > 1) {
	    t = dv2nrm_(&nk, &w[k + i__ * w_dim1]);
	}
	if (t < tiny) {
	    goto L180;
	}
	ri = rmat[ii];
	if (ri != zero) {
	    goto L100;
	}
	if (nk > 1) {
	    goto L30;
	}
	ij = ii;
	i__2 = *p;
	for (j = i__; j <= i__2; ++j) {
	    rmat[ij] = w[k + j * w_dim1];
	    ij += j;
/* L20: */
	}
	if (*qtrset) {
	    qtr[i__] = y[k];
	}
	w[k + i__ * w_dim1] = zero;
	goto L999;
L30:
	wi = w[k + i__ * w_dim1];
	if (bigrt > zero) {
	    goto L40;
	}
	bigrt = dr7mdc_(&c__5);
	tinyrt = dr7mdc_(&c__2);
L40:
	if (t <= tinyrt) {
	    goto L50;
	}
	if (t >= bigrt) {
	    goto L50;
	}
	if (wi < zero) {
	    t = -t;
	}
	wi += t;
	s = sqrt(t * wi);
	goto L70;
L50:
	s = sqrt(t);
	if (wi < zero) {
	    goto L60;
	}
	wi += t;
	s *= sqrt(wi);
	goto L70;
L60:
	t = -t;
	wi += t;
	s *= sqrt(-wi);
L70:
	w[k + i__ * w_dim1] = wi;
	d__1 = one / s;
	dv7scl_(&nk, &w[k + i__ * w_dim1], &d__1, &w[k + i__ * w_dim1]);
	rmat[ii] = -t;
	if (! (*qtrset)) {
	    goto L80;
	}
	d__1 = -dd7tpr_(&nk, &y[k], &w[k + i__ * w_dim1]);
	dv2axy_(&nk, &y[k], &d__1, &w[k + i__ * w_dim1], &y[k]);
	qtr[i__] = y[k];
L80:
	if (ip1 > *p) {
	    goto L999;
	}
	i__2 = *p;
	for (j = ip1; j <= i__2; ++j) {
	    d__1 = -dd7tpr_(&nk, &w[k + j * w_dim1], &w[k + i__ * w_dim1]);
	    dv2axy_(&nk, &w[k + j * w_dim1], &d__1, &w[k + i__ * w_dim1], &w[
		    k + j * w_dim1]);
	    rmat[ij] = w[k + j * w_dim1];
	    ij += j;
/* L90: */
	}
	if (nk <= 1) {
	    goto L999;
	}
	++k;
	--nk;
	goto L180;

L100:
	ari = abs(ri);
	if (ari > t) {
	    goto L110;
	}
/* Computing 2nd power */
	d__1 = ari / t;
	t *= sqrt(one + d__1 * d__1);
	goto L120;
L110:
/* Computing 2nd power */
	d__1 = t / ari;
	t = ari * sqrt(one + d__1 * d__1);
L120:
	if (ri < zero) {
	    t = -t;
	}
	ri += t;
	rmat[ii] = -t;
	s = -ri / t;
	if (nk <= 1) {
	    goto L150;
	}
	d__1 = one / ri;
	dv7scl_(&nk, &w[k + i__ * w_dim1], &d__1, &w[k + i__ * w_dim1]);
	if (! (*qtrset)) {
	    goto L130;
	}
	qri = qtr[i__];
	t = s * (qri + dd7tpr_(&nk, &y[k], &w[k + i__ * w_dim1]));
	qtr[i__] = qri + t;
L130:
	if (ip1 > *p) {
	    goto L999;
	}
	if (*qtrset) {
	    dv2axy_(&nk, &y[k], &t, &w[k + i__ * w_dim1], &y[k]);
	}
	i__2 = *p;
	for (j = ip1; j <= i__2; ++j) {
	    ri = rmat[ij];
	    t = s * (ri + dd7tpr_(&nk, &w[k + j * w_dim1], &w[k + i__ * 
		    w_dim1]));
	    dv2axy_(&nk, &w[k + j * w_dim1], &t, &w[k + i__ * w_dim1], &w[k + 
		    j * w_dim1]);
	    rmat[ij] = ri + t;
	    ij += j;
/* L140: */
	}
	goto L180;

L150:
	wi = w[k + i__ * w_dim1] / ri;
	w[k + i__ * w_dim1] = wi;
	if (! (*qtrset)) {
	    goto L160;
	}
	qri = qtr[i__];
	t = s * (qri + y[k] * wi);
	qtr[i__] = qri + t;
L160:
	if (ip1 > *p) {
	    goto L999;
	}
	if (*qtrset) {
	    y[k] = t * wi + y[k];
	}
	i__2 = *p;
	for (j = ip1; j <= i__2; ++j) {
	    ri = rmat[ij];
	    t = s * (ri + w[k + j * w_dim1] * wi);
	    w[k + j * w_dim1] += t * wi;
	    rmat[ij] = ri + t;
	    ij += j;
/* L170: */
	}
L180:
	;
    }

L999:
    return 0;
/*  ***  LAST LINE OF DQ7RAD FOLLOWS  *** */
} /* dq7rad_ */

/* Subroutine */ int df7hes_(doublereal *d__, doublereal *g, integer *irt, 
	integer *iv, integer *liv, integer *lv, integer *p, doublereal *v, 
	doublereal *x)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static integer i__, k, l, m, mm1;
    static doublereal del;
    static integer hmi, hes, hpi, hpm, stp0, kind, mm1o2, pp1o2, stpi, stpm, 
	    gsave1;
    extern /* Subroutine */ int dv7cpy_(integer *, doublereal *, doublereal *)
	    ;


/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN, STORE IT IN V STARTING */
/*  ***  AT V(IV(FDH)) = V(-IV(H)). */

/*  ***  IF IV(COVREQ) .GE. 0 THEN DF7HES USES GRADIENT DIFFERENCES, */
/*  ***  OTHERWISE FUNCTION DIFFERENCES.  STORAGE IN V IS AS IN DG7LIT. */

/* IRT VALUES... */
/*     1 = COMPUTE FUNCTION VALUE, I.E., V(F). */
/*     2 = COMPUTE G. */
/*     3 = DONE. */


/*  ***  PARAMETER DECLARATIONS  *** */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  EXTERNAL SUBROUTINES  *** */


/* DV7CPY.... COPY ONE VECTOR TO ANOTHER. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */




/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --g;
    --d__;

    /* Function Body */
    *irt = 4;
    kind = iv[15];
    m = iv[35];
    if (m > 0) {
	goto L10;
    }
    iv[56] = -abs(iv[56]);
    iv[74] = 0;
    iv[33] = -1;
    v[53] = v[10];
L10:
    if (m > *p) {
	goto L999;
    }
    if (kind < 0) {
	goto L110;
    }

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN USING BOTH FUNCTION AND */
/*  ***  GRADIENT VALUES. */

    gsave1 = iv[65] + *p;
    if (m > 0) {
	goto L20;
    }
/*        ***  FIRST CALL ON DF7HES.  SET GSAVE = G, TAKE FIRST STEP  *** */
    dv7cpy_(p, &v[gsave1], &g[1]);
    iv[12] = iv[7];
    goto L90;

L20:
    del = v[52];
    x[m] = v[51];
    if (iv[2] == 0) {
	goto L40;
    }

/*     ***  HANDLE OVERSIZE V(DELTA)  *** */

    if (del * x[m] > 0.) {
	goto L30;
    }
/*             ***  WE ALREADY TRIED SHRINKING V(DELTA), SO QUIT  *** */
    iv[74] = -2;
    goto L220;

/*        ***  TRY SHRINKING V(DELTA)  *** */
L30:
    del *= -.5;
    goto L100;

L40:
    hes = -iv[56];

/*  ***  SET  G = (G - GSAVE)/DEL  *** */

    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__] = (g[i__] - v[gsave1]) / del;
	++gsave1;
/* L50: */
    }

/*  ***  ADD G AS NEW COL. TO FINITE-DIFF. HESSIAN MATRIX  *** */

    k = hes + m * (m - 1) / 2;
    l = k + m - 2;
    if (m == 1) {
	goto L70;
    }

/*  ***  SET  H(I,M) = 0.5 * (H(I,M) + G(I))  FOR I = 1 TO M-1  *** */

    mm1 = m - 1;
    i__1 = mm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[k] = (v[k] + g[i__]) * .5;
	++k;
/* L60: */
    }

/*  ***  ADD  H(I,M) = G(I)  FOR I = M TO P  *** */

L70:
    ++l;
    i__1 = *p;
    for (i__ = m; i__ <= i__1; ++i__) {
	v[l] = g[i__];
	l += i__;
/* L80: */
    }

L90:
    ++m;
    iv[35] = m;
    if (m > *p) {
	goto L210;
    }

/*  ***  CHOOSE NEXT FINITE-DIFFERENCE STEP, RETURN TO GET G THERE  *** */

/* Computing MAX */
    d__2 = 1. / d__[m], d__3 = (d__1 = x[m], abs(d__1));
    del = v[44] * max(d__2,d__3);
    if (x[m] < 0.) {
	del = -del;
    }
    v[51] = x[m];
L100:
    x[m] += del;
    v[52] = del;
    *irt = 2;
    goto L999;

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN USING FUNCTION VALUES ONLY. */

L110:
    stp0 = iv[65] + *p - 1;
    mm1 = m - 1;
    mm1o2 = m * mm1 / 2;
    if (m > 0) {
	goto L120;
    }
/*        ***  FIRST CALL ON DF7HES.  *** */
    iv[63] = 0;
    goto L200;

L120:
    i__ = iv[63];
    hes = -iv[56];
    if (i__ > 0) {
	goto L180;
    }
    if (iv[2] == 0) {
	goto L140;
    }

/*     ***  HANDLE OVERSIZE STEP  *** */

    stpm = stp0 + m;
    del = v[stpm];
    if (del * x[51] > 0.) {
	goto L130;
    }
/*             ***  WE ALREADY TRIED SHRINKING THE STEP, SO QUIT  *** */
    iv[74] = -2;
    goto L220;

/*        ***  TRY SHRINKING THE STEP  *** */
L130:
    del *= -.5;
    x[m] = x[51] + del;
    v[stpm] = del;
    *irt = 1;
    goto L999;

/*  ***  SAVE F(X + STP(M)*E(M)) IN H(P,M)  *** */

L140:
    pp1o2 = *p * (*p - 1) / 2;
    hpm = hes + pp1o2 + mm1;
    v[hpm] = v[10];

/*  ***  START COMPUTING ROW M OF THE FINITE-DIFFERENCE HESSIAN H.  *** */

    hmi = hes + mm1o2;
    if (mm1 == 0) {
	goto L160;
    }
    hpi = hes + pp1o2;
    i__1 = mm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	v[hmi] = v[53] - (v[10] + v[hpi]);
	++hmi;
	++hpi;
/* L150: */
    }
L160:
    v[hmi] = v[10] - v[53] * 2.;

/*  ***  COMPUTE FUNCTION VALUES NEEDED TO COMPLETE ROW M OF H.  *** */

    i__ = 1;

L170:
    iv[63] = i__;
    stpi = stp0 + i__;
    v[52] = x[i__];
    x[i__] += v[stpi];
    if (i__ == m) {
	x[i__] = v[51] - v[stpi];
    }
    *irt = 1;
    goto L999;

L180:
    x[i__] = v[52];
    if (iv[2] == 0) {
	goto L190;
    }
/*        ***  PUNT IN THE EVENT OF AN OVERSIZE STEP  *** */
    iv[74] = -2;
    goto L220;

/*  ***  FINISH COMPUTING H(M,I)  *** */

L190:
    stpi = stp0 + i__;
    hmi = hes + mm1o2 + i__ - 1;
    stpm = stp0 + m;
    v[hmi] = (v[hmi] + v[10]) / (v[stpi] * v[stpm]);
    ++i__;
    if (i__ <= m) {
	goto L170;
    }
    iv[63] = 0;
    x[m] = v[51];

L200:
    ++m;
    iv[35] = m;
    if (m > *p) {
	goto L210;
    }

/*  ***  PREPARE TO COMPUTE ROW M OF THE FINITE-DIFFERENCE HESSIAN H. */
/*  ***  COMPUTE M-TH STEP SIZE STP(M), THEN RETURN TO OBTAIN */
/*  ***  F(X + STP(M)*E(M)), WHERE E(M) = M-TH STD. UNIT VECTOR. */

/* Computing MAX */
    d__2 = 1. / d__[m], d__3 = (d__1 = x[m], abs(d__1));
    del = v[42] * max(d__2,d__3);
    if (x[m] < 0.) {
	del = -del;
    }
    v[51] = x[m];
    x[m] += del;
    stpm = stp0 + m;
    v[stpm] = del;
    *irt = 1;
    goto L999;

/*  ***  RESTORE V(F), ETC.  *** */

L210:
    iv[74] = hes;
L220:
    v[10] = v[53];
    *irt = 3;
    if (kind < 0) {
	goto L999;
    }
    iv[7] = iv[12];
    gsave1 = iv[65] + *p;
    dv7cpy_(p, &g[1], &v[gsave1]);
    goto L999;

L999:
    return 0;
/*  ***  LAST CARD OF DF7HES FOLLOWS  *** */
} /* df7hes_ */

/* Subroutine */ int drnsg_(doublereal *a, doublereal *alf, doublereal *c__, 
	doublereal *da, integer *in, integer *iv, integer *l, integer *l1, 
	integer *la, integer *liv, integer *lv, integer *n, integer *nda, 
	integer *p, doublereal *v, doublereal *y)
{
    /* Initialized data */

    static doublereal machep = -1.;
    static doublereal negone = -1.;
    static doublereal sngfac = 100.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer a_dim1, a_offset, da_dim1, da_offset, i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static integer i__, k;
    static doublereal t;
    static integer d1, i1, j1, n1, n2, r1, md, lh, li, pp, ar1, dr1, rd1, r1l,
	     iv1, pp1, dri, ier, nml, fdh0, dri1, dr1l, jlen, ll1o2, nran;
    extern /* Subroutine */ int drn2g_(doublereal *, doublereal *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *)
	    ;
    static integer temp1, ipiv1, hsave;
    static logical nocov;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dq7rad_(integer *, integer *, integer *, 
	    doublereal *, logical *, doublereal *, doublereal *, doublereal *)
	    ;
    static integer csave1;
    extern /* Subroutine */ int dn2lrd_(doublereal *, integer *, doublereal *,
	     integer *, integer *, integer *, integer *, integer *, integer *,
	     doublereal *, doublereal *, doublereal *), dc7vfn_(integer *, 
	    doublereal *, integer *, integer *, integer *, integer *, integer 
	    *, doublereal *), dq7apl_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, integer *), dn2rdp_(integer *, 
	    integer *, integer *, integer *, doublereal *, doublereal *), 
	    dq7rfh_(integer *, integer *, integer *, integer *, integer *, 
	    integer *, doublereal *, doublereal *, integer *, doublereal *), 
	    dn2cvp_(integer *, integer *, integer *, integer *, doublereal *),
	     ds7cpr_(doublereal *, integer *, integer *, integer *), dv7scl_(
	    integer *, doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , dl7itv_(integer *, doublereal *, doublereal *, doublereal *), 
	    dv7cpy_(integer *, doublereal *, doublereal *);
    extern doublereal dl7svn_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *), dl7srt_(integer *, integer *, 
	    doublereal *, doublereal *, integer *), dv7prm_(integer *, 
	    integer *, doublereal *);
    extern doublereal dl7svx_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *);
    static doublereal singtl;
    extern /* Subroutine */ int ditsum_(doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *, doublereal *, doublereal *);


/*  ***  ITERATION DRIVER FOR SEPARABLE NONLINEAR LEAST SQUARES. */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION UIPARM(*) */

/*  ***  PURPOSE  *** */

/* GIVEN A SET OF N OBSERVATIONS Y(1)....Y(N) OF A DEPENDENT VARIABLE */
/* T(1)...T(N),  DRNSG ATTEMPTS TO COMPUTE A LEAST SQUARES FIT */
/* TO A FUNCTION  ETA  (THE MODEL) WHICH IS A LINEAR COMBINATION */

/*                  L */
/* ETA(C,ALF,T) =  SUM C * PHI(ALF,T) +PHI   (ALF,T) */
/*                 J=1  J     J           L+1 */

/* OF NONLINEAR FUNCTIONS PHI(J) DEPENDENT ON T AND ALF(1),...,ALF(P) */
/* (.E.G. A SUM OF EXPONENTIALS OR GAUSSIANS).  THAT IS, IT DETERMINES */
/* NONLINEAR PARAMETERS ALF WHICH MINIMIZE */

/*                   2    N                      2 */
/*     NORM(RESIDUAL)  = SUM  (Y - ETA(C,ALF,T )). */
/*                       I=1    I             I */

/* THE (L+1)ST TERM IS OPTIONAL. */


/*  ***  PARAMETERS  *** */

/*      A (IN)  MATRIX PHI(ALF,T) OF THE MODEL. */
/*    ALF (I/O) NONLINEAR PARAMETERS. */
/*                 INPUT = INITIAL GUESS, */
/*                 OUTPUT = BEST ESTIMATE FOUND. */
/*      C (OUT) LINEAR PARAMETERS (ESTIMATED). */
/*     DA (IN)  DERIVATIVES OF COLUMNS OF A WITH RESPECT TO COMPONENTS */
/*                 OF ALF, AS SPECIFIED BY THE IN ARRAY... */
/*     IN (IN)  WHEN  DRNSG IS CALLED WITH IV(1) = 2 OR -2, THEN FOR */
/*                 I = 1(1)NDA, COLUMN I OF DA IS THE PARTIAL */
/*                 DERIVATIVE WITH RESPECT TO ALF(IN(1,I)) OF COLUMN */
/*                 IN(2,I) OF A, UNLESS IV(1,I) IS NOT POSITIVE (IN */
/*                 WHICH CASE COLUMN I OF DA IS IGNORED.  IV(1) = -2 */
/*                 MEANS THERE ARE MORE COLUMNS OF DA TO COME AND */
/*                  DRNSG SHOULD RETURN FOR THEM. */
/*     IV (I/O) INTEGER PARAMETER AND SCRATCH VECTOR.   DRNSG RETURNS */
/*                 WITH IV(1) = 1 WHEN IT WANTS A TO BE EVALUATED AT */
/*                 ALF AND WITH IV(1) = 2 WHEN IT WANTS DA TO BE */
/*                 EVALUATED AT ALF.  WHEN CALLED WITH IV(1) = -2 */
/*                 (AFTER A RETURN WITH IV(1) = 2),  DRNSG RETURNS */
/*                 WITH IV(1) = -2 TO GET MORE COLUMNS OF DA. */
/*      L (IN)  NUMBER OF LINEAR PARAMETERS TO BE ESTIMATED. */
/*     L1 (IN)  L+1 IF PHI(L+1) IS IN THE MODEL, L IF NOT. */
/*     LA (IN)  LEAD DIMENSION OF A.  MUST BE AT LEAST N. */
/*    LIV (IN)  LENGTH OF IV.  MUST BE AT LEAST 110 + L + P. */
/*     LV (IN)  LENGTH OF V.  MUST BE AT LEAST */
/*                 105 + 2*N + JLEN + L*(L+3)/2 + P*(2*P + 17), */
/*                 WHERE  JLEN = (L+P)*(N+L+P+1),  UNLESS NEITHER A */
/*                 COVARIANCE MATRIX NOR REGRESSION DIAGNOSTICS ARE */
/*                 REQUESTED, IN WHICH CASE  JLEN = N*P. */
/*      N (IN)  NUMBER OF OBSERVATIONS. */
/*    NDA (IN)  NUMBER OF COLUMNS IN DA AND IN. */
/*      P (IN)  NUMBER OF NONLINEAR PARAMETERS TO BE ESTIMATED. */
/*      V (I/O) FLOATING-POINT PARAMETER AND SCRATCH VECTOR. */
/*              IF A COVARIANCE ESTIMATE IS REQUESTED, IT IS FOR */
/*              (ALF,C) -- NONLINEAR PARAMETERS ORDERED FIRST, */
/*              FOLLOWED BY LINEAR PARAMETERS. */
/*      Y (IN)  RIGHT-HAND SIDE VECTOR. */


/*  ***  EXTERNAL SUBROUTINES  *** */


/* DC7VFN... FINISHES COVARIANCE COMPUTATION. */
/* DIVSET.... SUPPLIES DEFAULT PARAMETER VALUES. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DITSUM.... PRINTS ITERATION SUMMARY, INITIAL AND FINAL ALF. */
/* DL7ITV... APPLIES INVERSE-TRANSPOSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DL7SRT.... COMPUTES (PARTIAL) CHOLESKY FACTORIZATION. */
/* DL7SVX... ESTIMATES LARGEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DL7SVN... ESTIMATES SMALLEST SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DN2CVP... PRINTS COVARIANCE MATRIX. */
/* DN2LRD... COMPUTES COVARIANCE AND REGRESSION DIAGNOSTICS. */
/* DN2RDP... PRINTS REGRESSION DIAGNOSTICS. */
/*  DRN2G... UNDERLYING NONLINEAR LEAST-SQUARES SOLVER. */
/* DQ7APL... APPLIES HOUSEHOLDER TRANSFORMS STORED BY DQ7RFH. */
/* DQ7RFH.... COMPUTES QR FACT. VIA HOUSEHOLDER TRANSFORMS WITH PIVOTING. */
/* DQ7RAD.... QR FACT., NO PIVOTING. */
/* DR7MDC... RETURNS MACHINE-DEP. CONSTANTS. */
/* DS7CPR... PRINTS LINEAR PARAMETERS AT SOLUTION. */
/* DV2AXY.... ADDS MULTIPLE OF ONE VECTOR TO ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7PRM.... PERMUTES A VECTOR. */
/* DV7SCL... SCALES AND COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL COMPONENTS OF A VECTOR TO A SCALAR. */

/*  ***  LOCAL VARIABLES  *** */


/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */

    /* Parameter adjustments */
    --c__;
    a_dim1 = *la;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --iv;
    --v;
    --y;
    in -= 3;
    da_dim1 = *la;
    da_offset = 1 + da_dim1;
    da -= da_offset;
    --alf;

    /* Function Body */

/* ++++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++ */


    if (iv[1] == 0) {
	divset_(&c__1, &iv[1], liv, lv, &v[1]);
    }
    n1 = 1;
    nml = *n;
    iv1 = iv[1];
    if (iv1 <= 2) {
	goto L20;
    }

/*  ***  CHECK INPUT INTEGERS  *** */

    if (*p <= 0) {
	goto L370;
    }
    if (*l < 0) {
	goto L370;
    }
    if (*n <= *l) {
	goto L370;
    }
    if (*la < *n) {
	goto L370;
    }
    if (iv1 < 12) {
	goto L20;
    }
    if (iv1 == 14) {
	goto L20;
    }
    if (iv1 == 12) {
	iv[1] = 13;
    }

/*  ***  FRESH START -- COMPUTE STORAGE REQUIREMENTS  *** */

    if (iv[1] > 16) {
	goto L370;
    }
    ll1o2 = *l * (*l + 1) / 2;
    jlen = *n * *p;
    i__ = *l + *p;
    if (iv[57] > 0 && iv[15] != 0) {
	jlen = i__ * (*n + i__ + 1);
    }
    if (iv[1] != 13) {
	goto L10;
    }
    iv[3] += *l;
    iv[4] = iv[4] + *p + (*n << 1) + jlen + ll1o2 + *l;
L10:
    if (iv[58] <= 110) {
	iv[58] = 111;
    }
    drn2g_(&v[1], &v[1], &iv[1], liv, lv, n, n, &n1, &nml, p, &v[1], &v[1], &
	    v[1], &alf[1]);
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[109] = iv[46];
    iv[46] += *l;
    iv[27] = iv[47];
    iv[82] = iv[27] + *p;
    iv[110] = iv[82] + *n;
    iv[105] = iv[110] + ll1o2;
    iv[70] = iv[105] + *l;
    iv[61] = iv[70] + jlen;
    iv[47] = iv[61] + *n;
    iv[108] = 0;
    if (iv1 == 13) {
	goto L999;
    }

/*  ***  SET POINTERS INTO IV AND V  *** */

L20:
    ar1 = iv[110];
    d1 = iv[27];
    dr1 = iv[70];
    dr1l = dr1 + *l;
    r1 = iv[61];
    r1l = r1 + *l;
    rd1 = iv[82];
    csave1 = iv[105];
    nml = *n - *l;
    if (iv1 <= 2) {
	goto L50;
    }

/*  ***  IF F.D. HESSIAN WILL BE NEEDED (FOR COVARIANCE OR REG. */
/*  ***  DIAGNOSTICS), HAVE  DRN2G COMPUTE ONLY THE PART CORRESP. */
/*  ***  TO ALF WITH C FIXED... */

    if (*l <= 0) {
	goto L30;
    }
    iv[106] = iv[15];
    if (abs(iv[15]) >= 3) {
	iv[15] = 0;
    }
    iv[107] = iv[57];
    if (iv[57] > 0) {
	iv[57] = -1;
    }

L30:
    n2 = nml;
    drn2g_(&v[d1], &v[dr1l], &iv[1], liv, lv, &nml, n, &n1, &n2, p, &v[r1l], &
	    v[rd1], &v[1], &alf[1]);
    if ((i__1 = iv[9] - 2, abs(i__1)) == 1 && *l > 0) {
	dv7cpy_(l, &c__[1], &v[csave1]);
    }
    iv1 = iv[1];
    if (iv1 == 2) {
	goto L150;
    }
    if (iv1 > 2) {
	goto L230;
    }

/*  ***  NEW FUNCTION VALUE (RESIDUAL) NEEDED  *** */

    iv[104] = iv[1];
    iv[1] = abs(iv1);
    if (iv[9] == 2 && *l > 0) {
	dv7cpy_(l, &v[csave1], &c__[1]);
    }
    goto L999;

/*  ***  COMPUTE NEW RESIDUAL OR GRADIENT  *** */

L50:
    iv[1] = iv[104];
    md = iv[35];
    if (md <= 0) {
	goto L60;
    }
    nml = *n;
    dr1l = dr1;
    r1l = r1;
L60:
    if (iv[2] != 0) {
	goto L30;
    }
    if (abs(iv1) == 2) {
	goto L170;
    }

/*  ***  COMPUTE NEW RESIDUAL  *** */

    if (*l1 <= *l) {
	dv7cpy_(n, &v[r1], &y[1]);
    }
    if (*l1 > *l) {
	dv2axy_(n, &v[r1], &negone, &a[*l1 * a_dim1 + 1], &y[1]);
    }
    if (md > 0) {
	goto L120;
    }
    ier = 0;
    if (*l <= 0) {
	goto L110;
    }
    ll1o2 = *l * (*l + 1) / 2;
    ipiv1 = iv[109];
    dq7rfh_(&ier, &iv[ipiv1], n, la, &c__0, l, &a[a_offset], &v[ar1], &ll1o2, 
	    &c__[1]);

/* *** DETERMINE NUMERICAL RANK OF A *** */

    if (machep <= zero) {
	machep = dr7mdc_(&c__3);
    }
    singtl = sngfac * (doublereal) max(*l,*n) * machep;
    k = *l;
    if (ier != 0) {
	k = ier - 1;
    }
L70:
    if (k <= 0) {
	goto L90;
    }
    t = dl7svx_(&k, &v[ar1], &c__[1], &c__[1]);
    if (t > zero) {
	t = dl7svn_(&k, &v[ar1], &c__[1], &c__[1]) / t;
    }
    if (t > singtl) {
	goto L80;
    }
    --k;
    goto L70;

/* *** RECORD RANK IN IV(IERS)... IV(IERS) = 0 MEANS FULL RANK, */
/* *** IV(IERS) .GT. 0 MEANS RANK IV(IERS) - 1. */

L80:
    if (k >= *l) {
	goto L100;
    }
L90:
    ier = k + 1;
    i__1 = *l - k;
    dv7scp_(&i__1, &c__[k + 1], &zero);
L100:
    iv[108] = ier;
    if (k <= 0) {
	goto L110;
    }

/* *** APPLY HOUSEHOLDER TRANSFORMATONS TO RESIDUALS... */

    dq7apl_(la, n, &k, &a[a_offset], &v[r1], &ier);

/* *** COMPUTING C NOW MAY SAVE A FUNCTION EVALUATION AT */
/* *** THE LAST ITERATION. */

    dl7itv_(&k, &c__[1], &v[ar1], &v[r1]);
    dv7prm_(l, &iv[ipiv1], &c__[1]);

L110:
    if (iv[1] < 2) {
	goto L220;
    }
    goto L999;


/*  ***  RESIDUAL COMPUTATION FOR F.D. HESSIAN  *** */

L120:
    if (*l <= 0) {
	goto L140;
    }
    i__1 = *l;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L130: */
	d__1 = -c__[i__];
	dv2axy_(n, &v[r1], &d__1, &a[i__ * a_dim1 + 1], &v[r1]);
    }
L140:
    if (iv[1] > 0) {
	goto L30;
    }
    iv[1] = 2;
    goto L160;

/*  ***  NEW GRADIENT (JACOBIAN) NEEDED  *** */

L150:
    iv[104] = iv1;
    if (iv[7] != iv[6]) {
	iv[1] = 1;
    }
L160:
    i__1 = *n * *p;
    dv7scp_(&i__1, &v[dr1], &zero);
    goto L999;

/*  ***  COMPUTE NEW JACOBIAN  *** */

L170:
    nocov = md <= *p || abs(iv[15]) >= 3;
    fdh0 = dr1 + *n * (*p + *l);
    if (*nda <= 0) {
	goto L370;
    }
    i__1 = *nda;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i1 = in[(i__ << 1) + 1] - 1;
	if (i1 < 0) {
	    goto L180;
	}
	j1 = in[(i__ << 1) + 2];
	k = dr1 + i1 * *n;
	t = negone;
	if (j1 <= *l) {
	    t = -c__[j1];
	}
	dv2axy_(n, &v[k], &t, &da[i__ * da_dim1 + 1], &v[k]);
	if (nocov) {
	    goto L180;
	}
	if (j1 > *l) {
	    goto L180;
	}
/*        ***  ADD IN (L,P) PORTION OF SECOND-ORDER PART OF HESSIAN */
/*        ***  FOR COVARIANCE OR REG. DIAG. COMPUTATIONS... */
	j1 += *p;
	k = fdh0 + j1 * (j1 - 1) / 2 + i1;
	v[k] -= dd7tpr_(n, &v[r1], &da[i__ * da_dim1 + 1]);
L180:
	;
    }
    if (iv1 == 2) {
	goto L190;
    }
    iv[1] = iv1;
    goto L999;
L190:
    if (*l <= 0) {
	goto L30;
    }
    if (md > *p) {
	goto L240;
    }
    if (md > 0) {
	goto L30;
    }
    k = dr1;
    ier = iv[108];
    nran = *l;
    if (ier > 0) {
	nran = ier - 1;
    }
    if (nran <= 0) {
	goto L210;
    }
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dq7apl_(la, n, &nran, &a[a_offset], &v[k], &ier);
	k += *n;
/* L200: */
    }
L210:
    dv7cpy_(l, &v[csave1], &c__[1]);
L220:
    if (ier == 0) {
	goto L30;
    }

/*     *** ADJUST SUBSCRIPTS DESCRIBING R AND DR... */

    nran = ier - 1;
    dr1l = dr1 + nran;
    nml = *n - nran;
    r1l = r1 + nran;
    goto L30;

/*  ***  CONVERGENCE OR LIMIT REACHED  *** */

L230:
    if (*l <= 0) {
	goto L350;
    }
    iv[15] = iv[106];
    iv[57] = iv[107];
    if (iv[1] > 6) {
	goto L360;
    }
    if (iv[57] % 4 == 0) {
	goto L360;
    }
    if (iv[74] <= 0 && abs(iv[15]) < 3) {
	goto L360;
    }
    if (iv[67] > 0) {
	goto L360;
    }
    if (iv[26] > 0) {
	goto L360;
    }

/*  *** PREPARE TO FINISH COMPUTING COVARIANCE MATRIX AND REG. DIAG. *** */

    pp = *l + *p;
    i__ = 0;
    if (iv[57] % 4 >= 2) {
	i__ = 1;
    }
    if (iv[57] % 2 == 1 && abs(iv[15]) == 1) {
	i__ += 2;
    }
    iv[35] = pp + i__;
    i__ = dr1 + *n * pp;
    k = *p * (*p + 1) / 2;
    i1 = iv[42];
    dv7cpy_(&k, &v[i__], &v[i1]);
    i__ += k;
    i__1 = pp * (pp + 1) / 2 - k;
    dv7scp_(&i__1, &v[i__], &zero);
    ++iv[52];
    ++iv[6];
    iv[7] = iv[6];
    iv[55] = iv[1];
    iv[104] = -1;
    iv[1] = 1;
    ++iv[30];
    ++iv[53];
    goto L999;

/*  ***  FINISH COVARIANCE COMPUTATION  *** */

L240:
    i__ = dr1 + *n * *p;
    i__1 = *l;
    for (i1 = 1; i1 <= i__1; ++i1) {
	dv7scl_(n, &v[i__], &negone, &a[i1 * a_dim1 + 1]);
	i__ += *n;
/* L250: */
    }
    pp = *l + *p;
    hsave = iv[56];
    k = dr1 + *n * pp;
    lh = pp * (pp + 1) / 2;
    if (abs(iv[15]) < 3) {
	goto L270;
    }
    i__ = iv[35] - 4;
    if (i__ >= pp) {
	goto L260;
    }
    dv7scp_(&lh, &v[k], &zero);
    dq7rad_(n, n, &pp, &v[1], &c_false, &v[k], &v[dr1], &v[1]);
    iv[35] = i__ + 8;
    iv[1] = 2;
    ++iv[30];
    ++iv[53];
    goto L160;

L260:
    iv[35] = i__;
    goto L300;

L270:
    pp1 = *p + 1;
    dri = dr1 + *n * *p;
    li = k + *p * pp1 / 2;
    i__1 = pp;
    for (i__ = pp1; i__ <= i__1; ++i__) {
	dri1 = dr1;
	i__2 = i__;
	for (i1 = 1; i1 <= i__2; ++i1) {
	    v[li] += dd7tpr_(n, &v[dri], &v[dri1]);
	    ++li;
	    dri1 += *n;
/* L280: */
	}
	dri += *n;
/* L290: */
    }
    dl7srt_(&pp1, &pp, &v[k], &v[k], &i__);
    if (i__ != 0) {
	goto L310;
    }
L300:
    temp1 = k + lh;
    t = dl7svn_(&pp, &v[k], &v[temp1], &v[temp1]);
    if (t <= zero) {
	goto L310;
    }
    t /= dl7svx_(&pp, &v[k], &v[temp1], &v[temp1]);
    v[53] = t;
    if (t > dr7mdc_(&c__4)) {
	goto L320;
    }
L310:
    iv[67] = -1;
    iv[26] = -1;
    iv[74] = -1;
    goto L340;
L320:
    iv[56] = temp1;
    iv[74] = abs(hsave);
    if (iv[35] - pp < 2) {
	goto L330;
    }
    i__ = iv[56];
    dv7scp_(&lh, &v[i__], &zero);
L330:
    dn2lrd_(&v[dr1], &iv[1], &v[k], &lh, liv, lv, n, n, &pp, &v[r1], &v[rd1], 
	    &v[1]);
L340:
    dc7vfn_(&iv[1], &v[k], &lh, liv, lv, n, &pp, &v[1]);
    iv[56] = hsave;

L350:
    if (iv[67] == 1) {
	iv[67] = rd1;
    }
L360:
    if (iv[1] <= 11) {
	ds7cpr_(&c__[1], &iv[1], l, liv);
    }
    if (iv[1] > 6) {
	goto L999;
    }
    i__1 = *p + *l;
    dn2cvp_(&iv[1], liv, lv, &i__1, &v[1]);
    dn2rdp_(&iv[1], liv, lv, n, &v[rd1], &v[1]);
    goto L999;

L370:
    iv[1] = 66;
    ditsum_(&v[1], &v[1], &iv[1], liv, lv, p, &v[1], &alf[1]);

L999:
    return 0;

/*  ***  LAST CARD OF  DRNSG FOLLOWS  *** */
} /* drnsg_ */

/* Subroutine */ int dl7tvm_(integer *n, doublereal *x, doublereal *l, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, i0, ij;
    static doublereal yi;


/*  ***  COMPUTE  X = (L**T)*Y, WHERE  L  IS AN  N X N  LOWER */
/*  ***  TRIANGULAR MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY */
/*  ***  OCCUPY THE SAME STORAGE.  *** */

/*     DIMENSION L(N*(N+1)/2) */

    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    i0 = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	yi = y[i__];
	x[i__] = 0.;
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    ij = i0 + j;
	    x[j] += yi * l[ij];
/* L10: */
	}
	i0 += i__;
/* L20: */
    }
    return 0;
/*  ***  LAST CARD OF DL7TVM FOLLOWS  *** */
} /* dl7tvm_ */

/* Subroutine */ int dl7itv_(integer *n, doublereal *x, doublereal *l, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, i0, ii, ij;
    static doublereal xi;
    static integer im1, np1;


/*  ***  SOLVE  (L**T)*X = Y,  WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
/*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
/*  ***  STORAGE.  *** */


    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	x[i__] = y[i__];
    }
    np1 = *n + 1;
    i0 = *n * (*n + 1) / 2;
    i__1 = *n;
    for (ii = 1; ii <= i__1; ++ii) {
	i__ = np1 - ii;
	xi = x[i__] / l[i0];
	x[i__] = xi;
	if (i__ <= 1) {
	    goto L999;
	}
	i0 -= i__;
	if (xi == 0.) {
	    goto L30;
	}
	im1 = i__ - 1;
	i__2 = im1;
	for (j = 1; j <= i__2; ++j) {
	    ij = i0 + j;
	    x[j] -= xi * l[ij];
/* L20: */
	}
L30:
	;
    }
L999:
    return 0;
/*  ***  LAST CARD OF DL7ITV FOLLOWS  *** */
} /* dl7itv_ */

/* Subroutine */ int drmngb_(doublereal *b, doublereal *d__, doublereal *fx, 
	doublereal *g, integer *iv, integer *liv, integer *lv, integer *n, 
	doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal t;
    static integer z__, i1, n1, w1, g01;
    static doublereal gi;
    static integer x01;
    static doublereal xi;
    static integer dg1, td1, tg1, np1, ipi, ipn, temp0, temp1, step1, dummy;
    extern /* Subroutine */ int dd7dgb_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    extern logical stopx_(integer *);
    extern /* Subroutine */ int dl7upd_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *);
    static integer dstep1;
    extern /* Subroutine */ int dw7zbf_(doublereal *, integer *, doublereal *,
	     doublereal *, doublereal *, doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int da7sst_(integer *, integer *, integer *, 
	    doublereal *), i7shft_(integer *, integer *, integer *), dl7vml_(
	    integer *, doublereal *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dq7rsh_(integer *, integer *, logical *, 
	    doublereal *, doublereal *, doublereal *), dv7scp_(integer *, 
	    doublereal *, doublereal *), dv7ipr_(integer *, integer *, 
	    doublereal *), dv7cpy_(integer *, doublereal *, doublereal *), 
	    dl7tvm_(integer *, doublereal *, doublereal *, doublereal *), 
	    dv2axy_(integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *), dv7vmp_(integer *, doublereal *, doublereal *, 
	    doublereal *, integer *);
    static integer nwtst1;
    extern /* Subroutine */ int dparck_(integer *, doublereal *, integer *, 
	    integer *, integer *, integer *, doublereal *);
    extern doublereal drldst_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);
    static integer lstgst, rstrst;


/*  ***  CARRY OUT  DMNGB (SIMPLY BOUNDED MINIMIZATION) ITERATIONS, */
/*  ***  USING DOUBLE-DOGLEG/BFGS STEPS. */

/*  ***  PARAMETER DECLARATIONS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* B.... VECTOR OF LOWER AND UPPER BOUNDS ON X. */
/* D.... SCALE VECTOR. */
/* FX... FUNCTION VALUE. */
/* G.... GRADIENT VECTOR. */
/* IV... INTEGER VALUE ARRAY. */
/* LIV.. LENGTH OF IV (AT LEAST 59) + N. */
/* LV... LENGTH OF V (AT LEAST 71 + N*(N+19)/2). */
/* N.... NUMBER OF VARIABLES (COMPONENTS IN X AND G). */
/* V.... FLOATING-POINT VALUE ARRAY. */
/* X.... VECTOR OF PARAMETERS TO BE OPTIMIZED. */

/*  ***  DISCUSSION  *** */

/*        PARAMETERS IV, N, V, AND X ARE THE SAME AS THE CORRESPONDING */
/*     ONES TO  DMNGB (WHICH SEE), EXCEPT THAT V CAN BE SHORTER (SINCE */
/*     THE PART OF V THAT  DMNGB USES FOR STORING G IS NOT NEEDED). */
/*     MOREOVER, COMPARED WITH  DMNGB, IV(1) MAY HAVE THE TWO ADDITIONAL */
/*     OUTPUT VALUES 1 AND 2, WHICH ARE EXPLAINED BELOW, AS IS THE USE */
/*     OF IV(TOOBIG) AND IV(NFGCAL).  THE VALUE IV(G), WHICH IS AN */
/*     OUTPUT VALUE FROM  DMNGB (AND SMSNOB), IS NOT REFERENCED BY */
/*     DRMNGB OR THE SUBROUTINES IT CALLS. */
/*        FX AND G NEED NOT HAVE BEEN INITIALIZED WHEN DRMNGB IS CALLED */
/*     WITH IV(1) = 12, 13, OR 14. */

/* IV(1) = 1 MEANS THE CALLER SHOULD SET FX TO F(X), THE FUNCTION VALUE */
/*             AT X, AND CALL DRMNGB AGAIN, HAVING CHANGED NONE OF THE */
/*             OTHER PARAMETERS.  AN EXCEPTION OCCURS IF F(X) CANNOT BE */
/*             (E.G. IF OVERFLOW WOULD OCCUR), WHICH MAY HAPPEN BECAUSE */
/*             OF AN OVERSIZED STEP.  IN THIS CASE THE CALLER SHOULD SET */
/*             IV(TOOBIG) = IV(2) TO 1, WHICH WILL CAUSE DRMNGB TO IG- */
/*             NORE FX AND TRY A SMALLER STEP.  THE PARAMETER NF THAT */
/*              DMNGB PASSES TO CALCF (FOR POSSIBLE USE BY CALCG) IS A */
/*             COPY OF IV(NFCALL) = IV(6). */
/* IV(1) = 2 MEANS THE CALLER SHOULD SET G TO G(X), THE GRADIENT VECTOR */
/*             OF F AT X, AND CALL DRMNGB AGAIN, HAVING CHANGED NONE OF */
/*             THE OTHER PARAMETERS EXCEPT POSSIBLY THE SCALE VECTOR D */
/*             WHEN IV(DTYPE) = 0.  THE PARAMETER NF THAT  DMNGB PASSES */
/*             TO CALCG IS IV(NFGCAL) = IV(7).  IF G(X) CANNOT BE */
/*             EVALUATED, THEN THE CALLER MAY SET IV(NFGCAL) TO 0, IN */
/*             WHICH CASE DRMNGB WILL RETURN WITH IV(1) = 65. */
/* . */
/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (DECEMBER 1979).  REVISED SEPT. 1982. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     IN PART BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324 AND MCS-7906671. */

/*        (SEE  DMNG FOR REFERENCES.) */

/* +++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  NO INTRINSIC FUNCTIONS  *** */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DA7SST.... ASSESSES CANDIDATE STEP. */
/* DD7DGB... COMPUTES SIMPLY BOUNDED DOUBLE-DOGLEG (CANDIDATE) STEP. */
/* DIVSET.... SUPPLIES DEFAULT IV AND V INPUT COMPONENTS. */
/* DD7TPR... RETURNS INNER PRODUCT OF TWO VECTORS. */
/* I7SHFT... CYCLICALLLY SHIFTS AN ARRAY OF INTEGERS. */
/* DITSUM.... PRINTS ITERATION SUMMARY AND INFO ON INITIAL AND FINAL X. */
/* DL7TVM... MULTIPLIES TRANSPOSE OF LOWER TRIANGLE TIMES VECTOR. */
/* LUPDT.... UPDATES CHOLESKY FACTOR OF HESSIAN APPROXIMATION. */
/* DL7VML.... MULTIPLIES LOWER TRIANGLE TIMES VECTOR. */
/* DPARCK.... CHECKS VALIDITY OF INPUT IV AND V VALUES. */
/* DQ7RSH... CYCLICALLY SHIFTS CHOLESKY FACTOR. */
/* DRLDST... COMPUTES V(RELDX) = RELATIVE STEP SIZE. */
/* STOPX.... RETURNS .TRUE. IF THE BREAK KEY HAS BEEN PRESSED. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */
/* DV2AXY.... COMPUTES SCALAR TIMES ONE VECTOR PLUS ANOTHER. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7IPR... CYCLICALLY SHIFTS A FLOATING-POINT ARRAY. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV7VMP... MULTIPLIES VECTOR BY VECTOR RAISED TO POWER (COMPONENTWISE). */
/* DW7ZBF... COMPUTES W AND Z FOR DL7UPD CORRESPONDING TO BFGS UPDATE. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */

/*  ***  (NOTE THAT NC IS STORED IN IV(G0)) *** */


/*  ***  V SUBSCRIPT VALUES  *** */



/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --g;
    --d__;
    b -= 3;

    /* Function Body */
    i__ = iv[1];
    if (i__ == 1) {
	goto L70;
    }
    if (i__ == 2) {
	goto L80;
    }

/*  ***  CHECK VALIDITY OF IV AND V INPUT VALUES  *** */

    if (iv[1] == 0) {
	divset_(&c__2, &iv[1], liv, lv, &v[1]);
    }
    if (iv[1] < 12) {
	goto L10;
    }
    if (iv[1] > 13) {
	goto L10;
    }
    iv[4] += *n * (*n + 19) / 2;
    iv[3] += *n;
L10:
    dparck_(&c__2, &d__[1], &iv[1], liv, lv, n, &v[1]);
    i__ = iv[1] - 2;
    if (i__ > 12) {
	goto L999;
    }
    switch (i__) {
	case 1:  goto L250;
	case 2:  goto L250;
	case 3:  goto L250;
	case 4:  goto L250;
	case 5:  goto L250;
	case 6:  goto L250;
	case 7:  goto L190;
	case 8:  goto L150;
	case 9:  goto L190;
	case 10:  goto L20;
	case 11:  goto L20;
	case 12:  goto L30;
    }

/*  ***  STORAGE ALLOCATION  *** */

L20:
    l = iv[42];
    iv[43] = l + *n * (*n + 1) / 2;
    iv[40] = iv[43] + (*n << 1);
    iv[41] = iv[40] + (*n << 1);
    iv[34] = iv[41] + *n;
    iv[37] = iv[34] + (*n << 1);
    iv[47] = iv[37] + (*n << 1);
    iv[46] = iv[58] + *n;
    if (iv[1] != 13) {
	goto L30;
    }
    iv[1] = 14;
    goto L999;

/*  ***  INITIALIZATION  *** */

L30:
    iv[31] = 0;
    iv[6] = 1;
    iv[30] = 1;
    iv[7] = 1;
    iv[35] = -1;
    iv[5] = 1;
    iv[11] = 1;
    iv[2] = 0;
    iv[55] = 0;
    iv[8] = 0;
    iv[48] = *n;
    v[9] = 0.;

/*  ***  CHECK CONSISTENCY OF B AND INITIALIZE IP ARRAY  *** */

    ipi = iv[58];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iv[ipi] = i__;
	++ipi;
	if (b[(i__ << 1) + 1] > b[(i__ << 1) + 2]) {
	    goto L410;
	}
/* L40: */
    }

    if (v[38] >= 0.) {
	dv7scp_(n, &d__[1], &v[38]);
    }
    if (iv[25] != 1) {
	goto L60;
    }

/*     ***  SET THE INITIAL HESSIAN APPROXIMATION TO DIAG(D)**-2  *** */

    l = iv[42];
    i__1 = *n * (*n + 1) / 2;
    dv7scp_(&i__1, &v[l], &c_b14);
    k = l - 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k += i__;
	t = d__[i__];
	if (t <= 0.) {
	    t = 1.;
	}
	v[k] = t;
/* L50: */
    }

/*  ***  GET INITIAL FUNCTION VALUE  *** */

L60:
    iv[1] = 1;
    goto L440;

L70:
    v[10] = *fx;
    if (iv[35] >= 0) {
	goto L250;
    }
    v[13] = *fx;
    iv[1] = 2;
    if (iv[2] == 0) {
	goto L999;
    }
    iv[1] = 63;
    goto L430;

/*  ***  MAKE SURE GRADIENT COULD BE COMPUTED  *** */

L80:
    if (iv[2] == 0) {
	goto L90;
    }
    iv[1] = 65;
    goto L430;

/*  ***  CHOOSE INITIAL PERMUTATION  *** */

L90:
    ipi = iv[58];
    ipn = ipi + *n;
    n1 = *n;
    np1 = *n + 1;
    l = iv[42];
    w1 = iv[34] + *n;
    k = *n - iv[48];
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	--ipn;
	j = iv[ipn];
	if (b[(j << 1) + 1] >= b[(j << 1) + 2]) {
	    goto L100;
	}
	xi = x[j];
	gi = g[j];
	if (xi <= b[(j << 1) + 1] && gi > 0.) {
	    goto L100;
	}
	if (xi >= b[(j << 1) + 2] && gi < 0.) {
	    goto L100;
	}
/*           *** DISALLOW CONVERGENCE IF X(J) HAS JUST BEEN FREED *** */
	if (i__ <= k) {
	    iv[55] = 0;
	}
	goto L120;
L100:
	i1 = np1 - i__;
	if (i1 >= n1) {
	    goto L110;
	}
	i7shft_(&n1, &i1, &iv[ipi]);
	dq7rsh_(&i1, &n1, &c_false, &g[1], &v[l], &v[w1]);
L110:
	--n1;
L120:
	;
    }

    iv[48] = n1;
    v[1] = 0.;
    if (n1 <= 0) {
	goto L130;
    }
    dg1 = iv[37];
    dv7vmp_(n, &v[dg1], &g[1], &d__[1], &c_n1);
    dv7ipr_(n, &iv[ipi], &v[dg1]);
    v[1] = dv2nrm_(&n1, &v[dg1]);
L130:
    if (iv[55] != 0) {
	goto L420;
    }
    if (iv[35] == 0) {
	goto L370;
    }

/*  ***  ALLOW FIRST STEP TO HAVE SCALED 2-NORM AT MOST V(LMAX0)  *** */

    v[8] = v[35];

    iv[35] = 0;


/* -----------------------------  MAIN LOOP  ----------------------------- */


/*  ***  PRINT ITERATION SUMMARY, CHECK ITERATION LIMIT  *** */

L140:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
L150:
    k = iv[31];
    if (k < iv[18]) {
	goto L160;
    }
    iv[1] = 10;
    goto L430;

/*  ***  UPDATE RADIUS  *** */

L160:
    iv[31] = k + 1;
    if (k == 0) {
	goto L170;
    }
    t = v[16] * v[2];
    if (v[16] < 1. || t > v[8]) {
	v[8] = t;
    }

/*  ***  INITIALIZE FOR START OF NEXT ITERATION  *** */

L170:
    x01 = iv[43];
    v[13] = v[10];
    iv[29] = 4;
    iv[33] = -1;

/*     ***  COPY X TO X0  *** */

    dv7cpy_(n, &v[x01], &x[1]);

/*  ***  CHECK STOPX AND FUNCTION EVALUATION LIMIT  *** */

L180:
    if (! stopx_(&dummy)) {
	goto L200;
    }
    iv[1] = 11;
    goto L210;

/*     ***  COME HERE WHEN RESTARTING AFTER FUNC. EVAL. LIMIT OR STOPX. */

L190:
    if (v[10] >= v[13]) {
	goto L200;
    }
    v[16] = 1.;
    k = iv[31];
    goto L160;

L200:
    if (iv[6] < iv[17]) {
	goto L220;
    }
    iv[1] = 9;
L210:
    if (v[10] >= v[13]) {
	goto L430;
    }

/*        ***  IN CASE OF STOPX OR FUNCTION EVALUATION LIMIT WITH */
/*        ***  IMPROVED V(F), EVALUATE THE GRADIENT AT X. */

    iv[55] = iv[1];
    goto L360;

/* . . . . . . . . . . . . .  COMPUTE CANDIDATE STEP  . . . . . . . . . . */

L220:
    step1 = iv[40];
    dg1 = iv[37];
    nwtst1 = iv[34];
    w1 = nwtst1 + *n;
    dstep1 = step1 + *n;
    ipi = iv[58];
    l = iv[42];
    tg1 = dg1 + *n;
    x01 = iv[43];
    td1 = x01 + *n;
    dd7dgb_(&b[3], &d__[1], &v[dg1], &v[dstep1], &g[1], &iv[ipi], &iv[33], &v[
	    l], lv, n, &iv[48], &v[nwtst1], &v[step1], &v[td1], &v[tg1], &v[1]
	    , &v[w1], &v[x01]);
    if (iv[29] != 6) {
	goto L230;
    }
    if (iv[9] != 2) {
	goto L250;
    }
    rstrst = 2;
    goto L260;

/*  ***  CHECK WHETHER EVALUATING F(X0 + STEP) LOOKS WORTHWHILE  *** */

L230:
    iv[2] = 0;
    if (v[2] <= 0.) {
	goto L250;
    }
    if (iv[29] != 5) {
	goto L240;
    }
    if (v[16] <= 1.) {
	goto L240;
    }
    if (v[7] > v[11] * 1.2) {
	goto L240;
    }
    if (iv[9] != 2) {
	goto L250;
    }
    rstrst = 0;
    goto L260;

/*  ***  COMPUTE F(X0 + STEP)  *** */

L240:
    dv2axy_(n, &x[1], &c_b85, &v[step1], &v[x01]);
    ++iv[6];
    iv[1] = 1;
    goto L440;

/* . . . . . . . . . . . . .  ASSESS CANDIDATE STEP  . . . . . . . . . . . */

L250:
    rstrst = 3;
L260:
    x01 = iv[43];
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    da7sst_(&iv[1], liv, lv, &v[1]);
    step1 = iv[40];
    lstgst = iv[41];
    i__ = iv[9] + 1;
    switch (i__) {
	case 1:  goto L300;
	case 2:  goto L270;
	case 3:  goto L280;
	case 4:  goto L290;
    }
L270:
    dv7cpy_(n, &x[1], &v[x01]);
    goto L300;
L280:
    dv7cpy_(n, &v[lstgst], &x[1]);
    goto L300;
L290:
    dv7cpy_(n, &x[1], &v[lstgst]);
    dv2axy_(n, &v[step1], &c_b90, &v[x01], &x[1]);
    v[17] = drldst_(n, &d__[1], &x[1], &v[x01]);
    iv[9] = rstrst;

L300:
    k = iv[29];
    switch (k) {
	case 1:  goto L310;
	case 2:  goto L340;
	case 3:  goto L340;
	case 4:  goto L340;
	case 5:  goto L310;
	case 6:  goto L320;
	case 7:  goto L330;
	case 8:  goto L330;
	case 9:  goto L330;
	case 10:  goto L330;
	case 11:  goto L330;
	case 12:  goto L330;
	case 13:  goto L400;
	case 14:  goto L370;
    }

/*     ***  RECOMPUTE STEP WITH CHANGED RADIUS  *** */

L310:
    v[8] = v[16] * v[2];
    goto L180;

/*  ***  COMPUTE STEP OF LENGTH V(LMAXS) FOR SINGULAR CONVERGENCE TEST. */

L320:
    v[8] = v[36];
    goto L220;

/*  ***  CONVERGENCE OR FALSE CONVERGENCE  *** */

L330:
    iv[55] = k - 4;
    if (v[10] >= v[13]) {
	goto L420;
    }
    if (iv[13] == 14) {
	goto L420;
    }
    iv[13] = 14;

/* . . . . . . . . . . . .  PROCESS ACCEPTABLE STEP  . . . . . . . . . . . */

L340:
    x01 = iv[43];
    step1 = iv[40];
    dv2axy_(n, &v[step1], &c_b90, &v[x01], &x[1]);
    if (iv[29] != 3) {
	goto L360;
    }

/*     ***  SET  TEMP1 = HESSIAN * STEP  FOR USE IN GRADIENT TESTS  *** */

/*     ***  USE X0 AS TEMPORARY... */

    ipi = iv[58];
    dv7cpy_(n, &v[x01], &v[step1]);
    dv7ipr_(n, &iv[ipi], &v[x01]);
    l = iv[42];
    dl7tvm_(n, &v[x01], &v[l], &v[x01]);
    dl7vml_(n, &v[x01], &v[l], &v[x01]);

/*        *** UNPERMUTE X0 INTO TEMP1 *** */

    temp1 = iv[41];
    temp0 = temp1 - 1;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = iv[ipi];
	++ipi;
	k = temp0 + j;
	v[k] = v[x01];
	++x01;
/* L350: */
    }

/*  ***  SAVE OLD GRADIENT, COMPUTE NEW ONE  *** */

L360:
    g01 = iv[34] + *n;
    dv7cpy_(n, &v[g01], &g[1]);
    ++iv[30];
    iv[2] = 0;
    iv[1] = 2;
    goto L999;

/*  ***  INITIALIZATIONS -- G0 = G - G0, ETC.  *** */

L370:
    g01 = iv[34] + *n;
    dv2axy_(n, &v[g01], &c_b90, &v[g01], &g[1]);
    step1 = iv[40];
    temp1 = iv[41];
    if (iv[29] != 3) {
	goto L390;
    }

/*  ***  SET V(RADFAC) BY GRADIENT TESTS  *** */

/*     ***  SET  TEMP1 = DIAG(D)**-1 * (HESSIAN*STEP + (G(X0)-G(X)))  *** */

    dv2axy_(n, &v[temp1], &c_b90, &v[g01], &v[temp1]);
    dv7vmp_(n, &v[temp1], &v[temp1], &d__[1], &c_n1);

/*        ***  DO GRADIENT TESTS  *** */

    if (dv2nrm_(n, &v[temp1]) <= v[1] * v[29]) {
	goto L380;
    }
    if (dd7tpr_(n, &g[1], &v[step1]) >= v[4] * v[30]) {
	goto L390;
    }
L380:
    v[16] = v[23];

/*  ***  UPDATE H, LOOP  *** */

L390:
    w1 = iv[34];
    z__ = iv[43];
    l = iv[42];
    ipi = iv[58];
    dv7ipr_(n, &iv[ipi], &v[step1]);
    dv7ipr_(n, &iv[ipi], &v[g01]);
    dw7zbf_(&v[l], n, &v[step1], &v[w1], &v[g01], &v[z__]);

/*     ** USE THE N-VECTORS STARTING AT V(STEP1) AND V(G01) FOR SCRATCH.. */
    dl7upd_(&v[temp1], &v[step1], &v[l], &v[g01], &v[l], n, &v[w1], &v[z__]);
    iv[1] = 2;
    goto L140;

/* . . . . . . . . . . . . . .  MISC. DETAILS  . . . . . . . . . . . . . . */

/*  ***  BAD PARAMETERS TO ASSESS  *** */

L400:
    iv[1] = 64;
    goto L430;

/*  ***  INCONSISTENT B  *** */

L410:
    iv[1] = 82;
    goto L430;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L420:
    iv[1] = iv[55];
    iv[55] = 0;
L430:
    ditsum_(&d__[1], &g[1], &iv[1], liv, lv, n, &v[1], &x[1]);
    goto L999;

/*  ***  PROJECT X INTO FEASIBLE REGION (PRIOR TO COMPUTING F OR G)  *** */

L440:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (x[i__] < b[(i__ << 1) + 1]) {
	    x[i__] = b[(i__ << 1) + 1];
	}
	if (x[i__] > b[(i__ << 1) + 2]) {
	    x[i__] = b[(i__ << 1) + 2];
	}
/* L450: */
    }

L999:
    return 0;

/*  ***  LAST CARD OF DRMNGB FOLLOWS  *** */
} /* drmngb_ */

/* Subroutine */ int ds7grd_(doublereal *alpha, doublereal *d__, doublereal *
	eta0, doublereal *fx, doublereal *g, integer *irc, integer *n, 
	doublereal *w, doublereal *x)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static doublereal h__;
    static integer i__;
    static doublereal h0, gi, aai, agi, eta, afx, axi, hmin;
    extern doublereal dr7mdc_(integer *);
    static doublereal machep, alphai, axibar, afxeta, discon;


/*  ***  COMPUTE FINITE DIFFERENCE GRADIENT BY STWEART*S SCHEME  *** */

/*     ***  PARAMETERS  *** */


/* ....................................................................... */

/*     ***  PURPOSE  *** */

/*        THIS SUBROUTINE USES AN EMBELLISHED FORM OF THE FINITE-DIFFER- */
/*     ENCE SCHEME PROPOSED BY STEWART (REF. 1) TO APPROXIMATE THE */
/*     GRADIENT OF THE FUNCTION F(X), WHOSE VALUES ARE SUPPLIED BY */
/*     REVERSE COMMUNICATION. */

/*     ***  PARAMETER DESCRIPTION  *** */

/*  ALPHA IN  (APPROXIMATE) DIAGONAL ELEMENTS OF THE HESSIAN OF F(X). */
/*      D IN  SCALE VECTOR SUCH THAT D(I)*X(I), I = 1,...,N, ARE IN */
/*             COMPARABLE UNITS. */
/*   ETA0 IN  ESTIMATED BOUND ON RELATIVE ERROR IN THE FUNCTION VALUE... */
/*             (TRUE VALUE) = (COMPUTED VALUE)*(1+E),   WHERE */
/*             ABS(E) .LE. ETA0. */
/*     FX I/O ON INPUT,  FX  MUST BE THE COMPUTED VALUE OF F(X).  ON */
/*             OUTPUT WITH IRC = 0, FX HAS BEEN RESTORED TO ITS ORIGINAL */
/*             VALUE, THE ONE IT HAD WHEN DS7GRD WAS LAST CALLED WITH */
/*             IRC = 0. */
/*      G I/O ON INPUT WITH IRC = 0, G SHOULD CONTAIN AN APPROXIMATION */
/*             TO THE GRADIENT OF F NEAR X, E.G., THE GRADIENT AT THE */
/*             PREVIOUS ITERATE.  WHEN DS7GRD RETURNS WITH IRC = 0, G IS */
/*             THE DESIRED FINITE-DIFFERENCE APPROXIMATION TO THE */
/*             GRADIENT AT X. */
/*    IRC I/O INPUT/RETURN CODE... BEFORE THE VERY FIRST CALL ON DS7GRD, */
/*             THE CALLER MUST SET IRC TO 0.  WHENEVER DS7GRD RETURNS A */
/*             NONZERO VALUE FOR IRC, IT HAS PERTURBED SOME COMPONENT OF */
/*             X... THE CALLER SHOULD EVALUATE F(X) AND CALL DS7GRD */
/*             AGAIN WITH FX = F(X). */
/*      N IN  THE NUMBER OF VARIABLES (COMPONENTS OF X) ON WHICH F */
/*             DEPENDS. */
/*      X I/O ON INPUT WITH IRC = 0, X IS THE POINT AT WHICH THE */
/*             GRADIENT OF F IS DESIRED.  ON OUTPUT WITH IRC NONZERO, X */
/*             IS THE POINT AT WHICH F SHOULD BE EVALUATED.  ON OUTPUT */
/*             WITH IRC = 0, X HAS BEEN RESTORED TO ITS ORIGINAL VALUE */
/*             (THE ONE IT HAD WHEN DS7GRD WAS LAST CALLED WITH IRC = 0) */
/*             AND G CONTAINS THE DESIRED GRADIENT APPROXIMATION. */
/*      W I/O WORK VECTOR OF LENGTH 6 IN WHICH DS7GRD SAVES CERTAIN */
/*             QUANTITIES WHILE THE CALLER IS EVALUATING F(X) AT A */
/*             PERTURBED X. */

/*     ***  APPLICATION AND USAGE RESTRICTIONS  *** */

/*        THIS ROUTINE IS INTENDED FOR USE WITH QUASI-NEWTON ROUTINES */
/*     FOR UNCONSTRAINED MINIMIZATION (IN WHICH CASE  ALPHA  COMES FROM */
/*     THE DIAGONAL OF THE QUASI-NEWTON HESSIAN APPROXIMATION). */

/*     ***  ALGORITHM NOTES  *** */

/*        THIS CODE DEPARTS FROM THE SCHEME PROPOSED BY STEWART (REF. 1) */
/*     IN ITS GUARDING AGAINST OVERLY LARGE OR SMALL STEP SIZES AND ITS */
/*     HANDLING OF SPECIAL CASES (SUCH AS ZERO COMPONENTS OF ALPHA OR G). */

/*     ***  REFERENCES  *** */

/* 1. STEWART, G.W. (1967), A MODIFICATION OF DAVIDON*S MINIMIZATION */
/*        METHOD TO ACCEPT DIFFERENCE APPROXIMATIONS OF DERIVATIVES, */
/*        J. ASSOC. COMPUT. MACH. 14, PP. 72-83. */

/*     ***  HISTORY  *** */

/*     DESIGNED AND CODED BY DAVID M. GAY (SUMMER 1977/SUMMER 1980). */

/*     ***  GENERAL  *** */

/*        THIS ROUTINE WAS PREPARED IN CONNECTION WITH WORK SUPPORTED BY */
/*     THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS76-00324 AND */
/*     MCS-7906671. */

/* ....................................................................... */

/*     *****  EXTERNAL FUNCTION  ***** */

/* DR7MDC... RETURNS MACHINE-DEPENDENT CONSTANTS. */

/*     ***** INTRINSIC FUNCTIONS ***** */
/* /+ */
/* / */
/*     ***** LOCAL VARIABLES ***** */



/* ---------------------------------  BODY  ------------------------------ */

    /* Parameter adjustments */
    --x;
    --g;
    --d__;
    --alpha;
    --w;

    /* Function Body */
    if (*irc < 0) {
	goto L140;
    }
    if (*irc > 0) {
	goto L210;
    }

/*     ***  FRESH START -- GET MACHINE-DEPENDENT CONSTANTS  *** */

/*     STORE MACHEP IN W(1) AND H0 IN W(2), WHERE MACHEP IS THE UNIT */
/*     ROUNDOFF (THE SMALLEST POSITIVE NUMBER SUCH THAT */
/*     1 + MACHEP .GT. 1  AND  1 - MACHEP .LT. 1),  AND  H0 IS THE */
/*     SQUARE-ROOT OF MACHEP. */

    w[1] = dr7mdc_(&c__3);
    w[2] = sqrt(w[1]);

    w[4] = *fx;

/*     ***  INCREMENT  I  AND START COMPUTING  G(I)  *** */

L110:
    i__ = abs(*irc) + 1;
    if (i__ > *n) {
	goto L300;
    }
    *irc = i__;
    afx = abs(w[4]);
    machep = w[1];
    h0 = w[2];
    hmin = machep * 50.;
    w[6] = x[i__];
    axi = (d__1 = x[i__], abs(d__1));
/* Computing MAX */
    d__1 = axi, d__2 = 1. / d__[i__];
    axibar = max(d__1,d__2);
    gi = g[i__];
    agi = abs(gi);
    eta = abs(*eta0);
    if (afx > 0.) {
/* Computing MAX */
	d__1 = eta, d__2 = agi * axi * machep / afx;
	eta = max(d__1,d__2);
    }
    alphai = alpha[i__];
    if (alphai == 0.) {
	goto L170;
    }
    if (gi == 0. || *fx == 0.) {
	goto L180;
    }
    afxeta = afx * eta;
    aai = abs(alphai);

/*        *** COMPUTE H = STEWART*S FORWARD-DIFFERENCE STEP SIZE. */

/* Computing 2nd power */
    d__1 = gi;
    if (d__1 * d__1 <= afxeta * aai) {
	goto L120;
    }
    h__ = sqrt(afxeta / aai) * 2.;
    h__ *= 1. - aai * h__ / (aai * 3. * h__ + agi * 4.);
    goto L130;
/* 120     H = TWO*(AFXETA*AGI/(AAI**2))**(ONE/THREE) */
L120:
    d__1 = afxeta * agi;
    h__ = pow_dd(&d__1, &c_b325) * 2. * pow_dd(&aai, &c_b913);
    h__ *= 1. - agi * 2. / (aai * 3. * h__ + agi * 4.);

/*        ***  ENSURE THAT  H  IS NOT INSIGNIFICANTLY SMALL  *** */

L130:
/* Computing MAX */
    d__1 = h__, d__2 = hmin * axibar;
    h__ = max(d__1,d__2);

/*        *** USE FORWARD DIFFERENCE IF BOUND ON TRUNCATION ERROR IS AT */
/*        *** MOST 10**-3. */

    if (aai * h__ <= agi * .002) {
	goto L160;
    }

/*        *** COMPUTE H = STEWART*S STEP FOR CENTRAL DIFFERENCE. */

    discon = afxeta * 2e3;
/* Computing 2nd power */
    d__1 = gi;
    h__ = discon / (agi + sqrt(d__1 * d__1 + aai * discon));

/*        ***  ENSURE THAT  H  IS NEITHER TOO SMALL NOR TOO BIG  *** */

/* Computing MAX */
    d__1 = h__, d__2 = hmin * axibar;
    h__ = max(d__1,d__2);
    if (h__ >= axibar * .02) {
	h__ = axibar * pow_dd(&h0, &c_b915);
    }

/*        ***  COMPUTE CENTRAL DIFFERENCE  *** */

    *irc = -i__;
    goto L200;

L140:
    h__ = -w[5];
    i__ = abs(*irc);
    if (h__ > 0.) {
	goto L150;
    }
    w[3] = *fx;
    goto L200;

L150:
    g[i__] = (w[3] - *fx) / (h__ * 2.);
    x[i__] = w[6];
    goto L110;

/*     ***  COMPUTE FORWARD DIFFERENCES IN VARIOUS CASES  *** */

L160:
    if (h__ >= axibar * .02) {
	h__ = h0 * axibar;
    }
    if (alphai * gi < 0.) {
	h__ = -h__;
    }
    goto L200;
L170:
    h__ = axibar;
    goto L200;
L180:
    h__ = h0 * axibar;

L200:
    x[i__] = w[6] + h__;
    w[5] = h__;
    goto L999;

/*     ***  COMPUTE ACTUAL FORWARD DIFFERENCE  *** */

L210:
    g[*irc] = (*fx - w[4]) / w[5];
    x[*irc] = w[6];
    goto L110;

/*  ***  RESTORE FX AND INDICATE THAT G HAS BEEN COMPUTED  *** */

L300:
    *fx = w[4];
    *irc = 0;

L999:
    return 0;
/*  ***  LAST CARD OF DS7GRD FOLLOWS  *** */
} /* ds7grd_ */

/* Subroutine */ int dg7qts_(doublereal *d__, doublereal *dig, doublereal *
	dihdi, integer *ka, doublereal *l, integer *p, doublereal *step, 
	doublereal *v, doublereal *w)
{
    /* Initialized data */

    static doublereal big = 0.;
    static doublereal dgxfac = 0.;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, q;
    static doublereal t;
    static integer x, k1, q0;
    static doublereal t1, t2, lk, si, sk, uk, wi, sw;
    static integer im1, lk0, uk0;
    static doublereal aki, akk, rad;
    static integer inc, irc;
    static doublereal phi, eps, dst;
    static integer diag, emin, emax;
    static doublereal root;
    static integer diag0;
    static doublereal delta;
    static integer kalim, kamin;
    static doublereal radsq, gtsta;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dl7ivm_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *), dv2nrm_(
	    integer *, doublereal *);
    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dl7svn_(integer *, doublereal *, doublereal *, 
	    doublereal *);
    extern /* Subroutine */ int dl7srt_(integer *, integer *, doublereal *, 
	    doublereal *, integer *);
    static doublereal alphak, psifac;
    static integer dggdmx;
    static doublereal oldphi, phimin, phimax;
    static integer phipin, dstsav;
    static logical restrt;
    static doublereal twopsi;


/*  *** COMPUTE GOLDFELD-QUANDT-TROTTER STEP BY MORE-HEBDEN TECHNIQUE *** */
/*  ***  (NL2SOL VERSION 2.2), MODIFIED A LA MORE AND SORENSEN  *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION DIHDI(P*(P+1)/2), L(P*(P+1)/2), W(4*P+7) */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  PURPOSE  *** */

/*        GIVEN THE (COMPACTLY STORED) LOWER TRIANGLE OF A SCALED */
/*     HESSIAN (APPROXIMATION) AND A NONZERO SCALED GRADIENT VECTOR, */
/*     THIS SUBROUTINE COMPUTES A GOLDFELD-QUANDT-TROTTER STEP OF */
/*     APPROXIMATE LENGTH V(RADIUS) BY THE MORE-HEBDEN TECHNIQUE.  IN */
/*     OTHER WORDS, STEP IS COMPUTED TO (APPROXIMATELY) MINIMIZE */
/*     PSI(STEP) = (G**T)*STEP + 0.5*(STEP**T)*H*STEP  SUCH THAT THE */
/*     2-NORM OF D*STEP IS AT MOST (APPROXIMATELY) V(RADIUS), WHERE */
/*     G  IS THE GRADIENT,  H  IS THE HESSIAN, AND  D  IS A DIAGONAL */
/*     SCALE MATRIX WHOSE DIAGONAL IS STORED IN THE PARAMETER D. */
/*     (DG7QTS ASSUMES  DIG = D**-1 * G  AND  DIHDI = D**-1 * H * D**-1.) */

/*  ***  PARAMETER DESCRIPTION  *** */

/*     D (IN)  = THE SCALE VECTOR, I.E. THE DIAGONAL OF THE SCALE */
/*              MATRIX  D  MENTIONED ABOVE UNDER PURPOSE. */
/*   DIG (IN)  = THE SCALED GRADIENT VECTOR, D**-1 * G.  IF G = 0, THEN */
/*              STEP = 0  AND  V(STPPAR) = 0  ARE RETURNED. */
/* DIHDI (IN)  = LOWER TRIANGLE OF THE SCALED HESSIAN (APPROXIMATION), */
/*              I.E., D**-1 * H * D**-1, STORED COMPACTLY BY ROWS., I.E., */
/*              IN THE ORDER (1,1), (2,1), (2,2), (3,1), (3,2), ETC. */
/*    KA (I/O) = THE NUMBER OF HEBDEN ITERATIONS (SO FAR) TAKEN TO DETER- */
/*              MINE STEP.  KA .LT. 0 ON INPUT MEANS THIS IS THE FIRST */
/*              ATTEMPT TO DETERMINE STEP (FOR THE PRESENT DIG AND DIHDI) */
/*              -- KA IS INITIALIZED TO 0 IN THIS CASE.  OUTPUT WITH */
/*              KA = 0  (OR V(STPPAR) = 0)  MEANS  STEP = -(H**-1)*G. */
/*     L (I/O) = WORKSPACE OF LENGTH P*(P+1)/2 FOR CHOLESKY FACTORS. */
/*     P (IN)  = NUMBER OF PARAMETERS -- THE HESSIAN IS A  P X P  MATRIX. */
/*  STEP (I/O) = THE STEP COMPUTED. */
/*     V (I/O) CONTAINS VARIOUS CONSTANTS AND VARIABLES DESCRIBED BELOW. */
/*     W (I/O) = WORKSPACE OF LENGTH 4*P + 6. */

/*  ***  ENTRIES IN V  *** */

/* V(DGNORM) (I/O) = 2-NORM OF (D**-1)*G. */
/* V(DSTNRM) (OUTPUT) = 2-NORM OF D*STEP. */
/* V(DST0)   (I/O) = 2-NORM OF D*(H**-1)*G (FOR POS. DEF. H ONLY), OR */
/*             OVERESTIMATE OF SMALLEST EIGENVALUE OF (D**-1)*H*(D**-1). */
/* V(EPSLON) (IN)  = MAX. REL. ERROR ALLOWED FOR PSI(STEP).  FOR THE */
/*             STEP RETURNED, PSI(STEP) WILL EXCEED ITS OPTIMAL VALUE */
/*             BY LESS THAN -V(EPSLON)*PSI(STEP).  SUGGESTED VALUE = 0.1. */
/* V(GTSTEP) (OUT) = INNER PRODUCT BETWEEN G AND STEP. */
/* V(NREDUC) (OUT) = PSI(-(H**-1)*G) = PSI(NEWTON STEP)  (FOR POS. DEF. */
/*             H ONLY -- V(NREDUC) IS SET TO ZERO OTHERWISE). */
/* V(PHMNFC) (IN)  = TOL. (TOGETHER WITH V(PHMXFC)) FOR ACCEPTING STEP */
/*             (MORE*S SIGMA).  THE ERROR V(DSTNRM) - V(RADIUS) MUST LIE */
/*             BETWEEN V(PHMNFC)*V(RADIUS) AND V(PHMXFC)*V(RADIUS). */
/* V(PHMXFC) (IN)  (SEE V(PHMNFC).) */
/*             SUGGESTED VALUES -- V(PHMNFC) = -0.25, V(PHMXFC) = 0.5. */
/* V(PREDUC) (OUT) = PSI(STEP) = PREDICTED OBJ. FUNC. REDUCTION FOR STEP. */
/* V(RADIUS) (IN)  = RADIUS OF CURRENT (SCALED) TRUST REGION. */
/* V(RAD0)   (I/O) = VALUE OF V(RADIUS) FROM PREVIOUS CALL. */
/* V(STPPAR) (I/O) IS NORMALLY THE MARQUARDT PARAMETER, I.E. THE ALPHA */
/*             DESCRIBED BELOW UNDER ALGORITHM NOTES.  IF H + ALPHA*D**2 */
/*             (SEE ALGORITHM NOTES) IS (NEARLY) SINGULAR, HOWEVER, */
/*             THEN V(STPPAR) = -ALPHA. */

/*  ***  USAGE NOTES  *** */

/*     IF IT IS DESIRED TO RECOMPUTE STEP USING A DIFFERENT VALUE OF */
/*     V(RADIUS), THEN THIS ROUTINE MAY BE RESTARTED BY CALLING IT */
/*     WITH ALL PARAMETERS UNCHANGED EXCEPT V(RADIUS).  (THIS EXPLAINS */
/*     WHY STEP AND W ARE LISTED AS I/O).  ON AN INITIAL CALL (ONE WITH */
/*     KA .LT. 0), STEP AND W NEED NOT BE INITIALIZED AND ONLY COMPO- */
/*     NENTS V(EPSLON), V(STPPAR), V(PHMNFC), V(PHMXFC), V(RADIUS), AND */
/*     V(RAD0) OF V MUST BE INITIALIZED. */

/*  ***  ALGORITHM NOTES  *** */

/*        THE DESIRED G-Q-T STEP (REF. 2, 3, 4, 6) SATISFIES */
/*     (H + ALPHA*D**2)*STEP = -G  FOR SOME NONNEGATIVE ALPHA SUCH THAT */
/*     H + ALPHA*D**2 IS POSITIVE SEMIDEFINITE.  ALPHA AND STEP ARE */
/*     COMPUTED BY A SCHEME ANALOGOUS TO THE ONE DESCRIBED IN REF. 5. */
/*     ESTIMATES OF THE SMALLEST AND LARGEST EIGENVALUES OF THE HESSIAN */
/*     ARE OBTAINED FROM THE GERSCHGORIN CIRCLE THEOREM ENHANCED BY A */
/*     SIMPLE FORM OF THE SCALING DESCRIBED IN REF. 7.  CASES IN WHICH */
/*     H + ALPHA*D**2 IS NEARLY (OR EXACTLY) SINGULAR ARE HANDLED BY */
/*     THE TECHNIQUE DISCUSSED IN REF. 2.  IN THESE CASES, A STEP OF */
/*     (EXACT) LENGTH V(RADIUS) IS RETURNED FOR WHICH PSI(STEP) EXCEEDS */
/*     ITS OPTIMAL VALUE BY LESS THAN -V(EPSLON)*PSI(STEP).  THE TEST */
/*     SUGGESTED IN REF. 6 FOR DETECTING THE SPECIAL CASE IS PERFORMED */
/*     ONCE TWO MATRIX FACTORIZATIONS HAVE BEEN DONE -- DOING SO SOONER */
/*     SEEMS TO DEGRADE THE PERFORMANCE OF OPTIMIZATION ROUTINES THAT */
/*     CALL THIS ROUTINE. */

/*  ***  FUNCTIONS AND SUBROUTINES CALLED  *** */

/* DD7TPR - RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DL7ITV - APPLIES INVERSE-TRANSPOSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DL7IVM - APPLIES INVERSE OF COMPACT LOWER TRIANG. MATRIX. */
/* DL7SRT  - FINDS CHOLESKY FACTOR (OF COMPACTLY STORED LOWER TRIANG.). */
/* DL7SVN - RETURNS APPROX. TO MIN. SING. VALUE OF LOWER TRIANG. MATRIX. */
/* DR7MDC - RETURNS MACHINE-DEPENDENT CONSTANTS. */
/* DV2NRM - RETURNS 2-NORM OF A VECTOR. */

/*  ***  REFERENCES  *** */

/* 1.  DENNIS, J.E., GAY, D.M., AND WELSCH, R.E. (1981), AN ADAPTIVE */
/*             NONLINEAR LEAST-SQUARES ALGORITHM, ACM TRANS. MATH. */
/*             SOFTWARE, VOL. 7, NO. 3. */
/* 2.  GAY, D.M. (1981), COMPUTING OPTIMAL LOCALLY CONSTRAINED STEPS, */
/*             SIAM J. SCI. STATIST. COMPUTING, VOL. 2, NO. 2, PP. */
/*             186-197. */
/* 3.  GOLDFELD, S.M., QUANDT, R.E., AND TROTTER, H.F. (1966), */
/*             MAXIMIZATION BY QUADRATIC HILL-CLIMBING, ECONOMETRICA 34, */
/*             PP. 541-551. */
/* 4.  HEBDEN, M.D. (1973), AN ALGORITHM FOR MINIMIZATION USING EXACT */
/*             SECOND DERIVATIVES, REPORT T.P. 515, THEORETICAL PHYSICS */
/*             DIV., A.E.R.E. HARWELL, OXON., ENGLAND. */
/* 5.  MORE, J.J. (1978), THE LEVENBERG-MARQUARDT ALGORITHM, IMPLEMEN- */
/*             TATION AND THEORY, PP.105-116 OF SPRINGER LECTURE NOTES */
/*             IN MATHEMATICS NO. 630, EDITED BY G.A. WATSON, SPRINGER- */
/*             VERLAG, BERLIN AND NEW YORK. */
/* 6.  MORE, J.J., AND SORENSEN, D.C. (1981), COMPUTING A TRUST REGION */
/*             STEP, TECHNICAL REPORT ANL-81-83, ARGONNE NATIONAL LAB. */
/* 7.  VARGA, R.S. (1965), MINIMAL GERSCHGORIN SETS, PACIFIC J. MATH. 15, */
/*             PP. 719-729. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH */
/*     SUPPORTED BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS */
/*     MCS-7600324, DCR75-10143, 76-14311DSS, MCS76-11989, AND */
/*     MCS-7906671. */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/*  ***  SUBSCRIPTS FOR V  *** */


    /* Parameter adjustments */
    --dihdi;
    --l;
    --step;
    --dig;
    --d__;
    --v;
    --w;

    /* Function Body */

/*  ***  BODY  *** */

    if (big <= 0.) {
	big = dr7mdc_(&c__6);
    }

/*     ***  STORE LARGEST ABS. ENTRY IN (D**-1)*H*(D**-1) AT W(DGGDMX). */
    dggdmx = *p + 1;
/*     ***  STORE GERSCHGORIN OVER- AND UNDERESTIMATES OF THE LARGEST */
/*     ***  AND SMALLEST EIGENVALUES OF (D**-1)*H*(D**-1) AT W(EMAX) */
/*     ***  AND W(EMIN) RESPECTIVELY. */
    emax = dggdmx + 1;
    emin = emax + 1;
/*     ***  FOR USE IN RECOMPUTING STEP, THE FINAL VALUES OF LK, UK, DST, */
/*     ***  AND THE INVERSE DERIVATIVE OF MORE*S PHI AT 0 (FOR POS. DEF. */
/*     ***  H) ARE STORED IN W(LK0), W(UK0), W(DSTSAV), AND W(PHIPIN) */
/*     ***  RESPECTIVELY. */
    lk0 = emin + 1;
    phipin = lk0 + 1;
    uk0 = phipin + 1;
    dstsav = uk0 + 1;
/*     ***  STORE DIAG OF (D**-1)*H*(D**-1) IN W(DIAG),...,W(DIAG0+P). */
    diag0 = dstsav;
    diag = diag0 + 1;
/*     ***  STORE -D*STEP IN W(Q),...,W(Q0+P). */
    q0 = diag0 + *p;
    q = q0 + 1;
/*     ***  ALLOCATE STORAGE FOR SCRATCH VECTOR X  *** */
    x = q + *p;
    rad = v[8];
/* Computing 2nd power */
    d__1 = rad;
    radsq = d__1 * d__1;
/*     ***  PHITOL = MAX. ERROR ALLOWED IN DST = V(DSTNRM) = 2-NORM OF */
/*     ***  D*STEP. */
    phimax = v[21] * rad;
    phimin = v[20] * rad;
    psifac = big;
    t1 = v[19] * 2. / (((v[20] + 1.) * 4. * 3. + 2. + 2.) * 3. * rad);
    if (t1 < big * min(rad,1.)) {
	psifac = t1 / rad;
    }
/*     ***  OLDPHI IS USED TO DETECT LIMITS OF NUMERICAL ACCURACY.  IF */
/*     ***  WE RECOMPUTE STEP AND IT DOES NOT CHANGE, THEN WE ACCEPT IT. */
    oldphi = 0.;
    eps = v[19];
    irc = 0;
    restrt = FALSE_;
    kalim = *ka + 50;

/*  ***  START OR RESTART, DEPENDING ON KA  *** */

    if (*ka >= 0) {
	goto L290;
    }

/*  ***  FRESH START  *** */

    k = 0;
    uk = -1.;
    *ka = 0;
    kalim = 50;
    v[1] = dv2nrm_(p, &dig[1]);
    v[6] = 0.;
    v[3] = 0.;
    kamin = 3;
    if (v[1] == 0.) {
	kamin = 0;
    }

/*     ***  STORE DIAG(DIHDI) IN W(DIAG0+1),...,W(DIAG0+P)  *** */

    j = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j += i__;
	k1 = diag0 + i__;
	w[k1] = dihdi[j];
/* L10: */
    }

/*     ***  DETERMINE W(DGGDMX), THE LARGEST ELEMENT OF DIHDI  *** */

    t1 = 0.;
    j = *p * (*p + 1) / 2;
    i__1 = j;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = (d__1 = dihdi[i__], abs(d__1));
	if (t1 < t) {
	    t1 = t;
	}
/* L20: */
    }
    w[dggdmx] = t1;

/*  ***  TRY ALPHA = 0  *** */

L30:
    dl7srt_(&c__1, p, &l[1], &dihdi[1], &irc);
    if (irc == 0) {
	goto L50;
    }
/*        ***  INDEF. H -- UNDERESTIMATE SMALLEST EIGENVALUE, USE THIS */
/*        ***  ESTIMATE TO INITIALIZE LOWER BOUND LK ON ALPHA. */
    j = irc * (irc + 1) / 2;
    t = l[j];
    l[j] = 1.;
    i__1 = irc;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L40: */
	w[i__] = 0.;
    }
    w[irc] = 1.;
    dl7itv_(&irc, &w[1], &l[1], &w[1]);
    t1 = dv2nrm_(&irc, &w[1]);
    lk = -t / t1 / t1;
    v[3] = -lk;
    if (restrt) {
	goto L210;
    }
    goto L70;

/*     ***  POSITIVE DEFINITE H -- COMPUTE UNMODIFIED NEWTON STEP.  *** */
L50:
    lk = 0.;
    t = dl7svn_(p, &l[1], &w[q], &w[q]);
    if (t >= 1.) {
	goto L60;
    }
    if (v[1] >= t * t * big) {
	goto L70;
    }
L60:
    dl7ivm_(p, &w[q], &l[1], &dig[1]);
    gtsta = dd7tpr_(p, &w[q], &w[q]);
    v[6] = gtsta * .5;
    dl7itv_(p, &w[q], &l[1], &w[q]);
    dst = dv2nrm_(p, &w[q]);
    v[3] = dst;
    phi = dst - rad;
    if (phi <= phimax) {
	goto L260;
    }
    if (restrt) {
	goto L210;
    }

/*  ***  PREPARE TO COMPUTE GERSCHGORIN ESTIMATES OF LARGEST (AND */
/*  ***  SMALLEST) EIGENVALUES.  *** */

L70:
    k = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wi = 0.;
	if (i__ == 1) {
	    goto L90;
	}
	im1 = i__ - 1;
	i__2 = im1;
	for (j = 1; j <= i__2; ++j) {
	    ++k;
	    t = (d__1 = dihdi[k], abs(d__1));
	    wi += t;
	    w[j] += t;
/* L80: */
	}
L90:
	w[i__] = wi;
	++k;
/* L100: */
    }

/*  ***  (UNDER-)ESTIMATE SMALLEST EIGENVALUE OF (D**-1)*H*(D**-1)  *** */

    k = 1;
    t1 = w[diag] - w[1];
    if (*p <= 1) {
	goto L120;
    }
    i__1 = *p;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = diag0 + i__;
	t = w[j] - w[i__];
	if (t >= t1) {
	    goto L110;
	}
	t1 = t;
	k = i__;
L110:
	;
    }

L120:
    sk = w[k];
    j = diag0 + k;
    akk = w[j];
    k1 = k * (k - 1) / 2 + 1;
    inc = 1;
    t = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (i__ == k) {
	    goto L130;
	}
	aki = (d__1 = dihdi[k1], abs(d__1));
	si = w[i__];
	j = diag0 + i__;
	t1 = (akk - w[j] + si - aki) * .5;
	t1 += sqrt(t1 * t1 + sk * aki);
	if (t < t1) {
	    t = t1;
	}
	if (i__ < k) {
	    goto L140;
	}
L130:
	inc = i__;
L140:
	k1 += inc;
/* L150: */
    }

    w[emin] = akk - t;
    uk = v[1] / rad - w[emin];
    if (v[1] == 0.) {
	uk = uk + .001 + uk * .001;
    }
    if (uk <= 0.) {
	uk = .001;
    }

/*  ***  COMPUTE GERSCHGORIN (OVER-)ESTIMATE OF LARGEST EIGENVALUE  *** */

    k = 1;
    t1 = w[diag] + w[1];
    if (*p <= 1) {
	goto L170;
    }
    i__1 = *p;
    for (i__ = 2; i__ <= i__1; ++i__) {
	j = diag0 + i__;
	t = w[j] + w[i__];
	if (t <= t1) {
	    goto L160;
	}
	t1 = t;
	k = i__;
L160:
	;
    }

L170:
    sk = w[k];
    j = diag0 + k;
    akk = w[j];
    k1 = k * (k - 1) / 2 + 1;
    inc = 1;
    t = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (i__ == k) {
	    goto L180;
	}
	aki = (d__1 = dihdi[k1], abs(d__1));
	si = w[i__];
	j = diag0 + i__;
	t1 = (w[j] + si - aki - akk) * .5;
	t1 += sqrt(t1 * t1 + sk * aki);
	if (t < t1) {
	    t = t1;
	}
	if (i__ < k) {
	    goto L190;
	}
L180:
	inc = i__;
L190:
	k1 += inc;
/* L200: */
    }

    w[emax] = akk + t;
/* Computing MAX */
    d__1 = lk, d__2 = v[1] / rad - w[emax];
    lk = max(d__1,d__2);

/*     ***  ALPHAK = CURRENT VALUE OF ALPHA (SEE ALG. NOTES ABOVE).  WE */
/*     ***  USE MORE*S SCHEME FOR INITIALIZING IT. */
    alphak = abs(v[5]) * v[9] / rad;
/* Computing MIN */
    d__1 = uk, d__2 = max(alphak,lk);
    alphak = min(d__1,d__2);

    if (irc != 0) {
	goto L210;
    }

/*  ***  COMPUTE L0 FOR POSITIVE DEFINITE H  *** */

    dl7ivm_(p, &w[1], &l[1], &w[q]);
    t = dv2nrm_(p, &w[1]);
    w[phipin] = rad / t / t;
/* Computing MAX */
    d__1 = lk, d__2 = phi * w[phipin];
    lk = max(d__1,d__2);

/*  ***  SAFEGUARD ALPHAK AND ADD ALPHAK*I TO (D**-1)*H*(D**-1)  *** */

L210:
    ++(*ka);
    if (-v[3] >= alphak || alphak < lk || alphak >= uk) {
/* Computing MAX */
	d__1 = .001, d__2 = sqrt(lk / uk);
	alphak = uk * max(d__1,d__2);
    }
    if (alphak <= 0.) {
	alphak = uk * .5;
    }
    if (alphak <= 0.) {
	alphak = uk;
    }
    k = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k += i__;
	j = diag0 + i__;
	dihdi[k] = w[j] + alphak;
/* L220: */
    }

/*  ***  TRY COMPUTING CHOLESKY DECOMPOSITION  *** */

    dl7srt_(&c__1, p, &l[1], &dihdi[1], &irc);
    if (irc == 0) {
	goto L240;
    }

/*  ***  (D**-1)*H*(D**-1) + ALPHAK*I  IS INDEFINITE -- OVERESTIMATE */
/*  ***  SMALLEST EIGENVALUE FOR USE IN UPDATING LK  *** */

    j = irc * (irc + 1) / 2;
    t = l[j];
    l[j] = 1.;
    i__1 = irc;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L230: */
	w[i__] = 0.;
    }
    w[irc] = 1.;
    dl7itv_(&irc, &w[1], &l[1], &w[1]);
    t1 = dv2nrm_(&irc, &w[1]);
    lk = alphak - t / t1 / t1;
    v[3] = -lk;
    if (uk < lk) {
	uk = lk;
    }
    if (alphak < lk) {
	goto L210;
    }

/*  ***  NASTY CASE -- EXACT GERSCHGORIN BOUNDS.  FUDGE LK, UK... */

    t = alphak * .001;
    if (t <= 0.) {
	t = .001;
    }
    lk = alphak + t;
    if (uk <= lk) {
	uk = lk + t;
    }
    goto L210;

/*  ***  ALPHAK MAKES (D**-1)*H*(D**-1) POSITIVE DEFINITE. */
/*  ***  COMPUTE Q = -D*STEP, CHECK FOR CONVERGENCE.  *** */

L240:
    dl7ivm_(p, &w[q], &l[1], &dig[1]);
    gtsta = dd7tpr_(p, &w[q], &w[q]);
    dl7itv_(p, &w[q], &l[1], &w[q]);
    dst = dv2nrm_(p, &w[q]);
    phi = dst - rad;
    if (phi <= phimax && phi >= phimin) {
	goto L270;
    }
    if (phi == oldphi) {
	goto L270;
    }
    oldphi = phi;
    if (phi < 0.) {
	goto L330;
    }

/*  ***  UNACCEPTABLE ALPHAK -- UPDATE LK, UK, ALPHAK  *** */

L250:
    if (*ka >= kalim) {
	goto L270;
    }
/*     ***  THE FOLLOWING DMIN1 IS NECESSARY BECAUSE OF RESTARTS  *** */
    if (phi < 0.) {
	uk = min(uk,alphak);
    }
/*     *** KAMIN = 0 ONLY IFF THE GRADIENT VANISHES  *** */
    if (kamin == 0) {
	goto L210;
    }
    dl7ivm_(p, &w[1], &l[1], &w[q]);
/*     *** THE FOLLOWING, COMMENTED CALCULATION OF ALPHAK IS SOMETIMES */
/*     *** SAFER BUT WORSE IN PERFORMANCE... */
/*     T1 = DST / DV2NRM(P, W) */
/*     ALPHAK = ALPHAK  +  T1 * (PHI/RAD) * T1 */
    t1 = dv2nrm_(p, &w[1]);
    alphak += phi / t1 * (dst / t1) * (dst / rad);
    lk = max(lk,alphak);
    alphak = lk;
    goto L210;

/*  ***  ACCEPTABLE STEP ON FIRST TRY  *** */

L260:
    alphak = 0.;

/*  ***  SUCCESSFUL STEP IN GENERAL.  COMPUTE STEP = -(D**-1)*Q  *** */

L270:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = q0 + i__;
	step[i__] = -w[j] / d__[i__];
/* L280: */
    }
    v[4] = -gtsta;
    v[7] = (abs(alphak) * dst * dst + gtsta) * .5;
    goto L410;


/*  ***  RESTART WITH NEW RADIUS  *** */

L290:
    if (v[3] <= 0. || v[3] - rad > phimax) {
	goto L310;
    }

/*     ***  PREPARE TO RETURN NEWTON STEP  *** */

    restrt = TRUE_;
    ++(*ka);
    k = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k += i__;
	j = diag0 + i__;
	dihdi[k] = w[j];
/* L300: */
    }
    uk = -1.;
    goto L30;

L310:
    kamin = *ka + 3;
    if (v[1] == 0.) {
	kamin = 0;
    }
    if (*ka == 0) {
	goto L50;
    }

    dst = w[dstsav];
    alphak = abs(v[5]);
    phi = dst - rad;
    t = v[1] / rad;
    uk = t - w[emin];
    if (v[1] == 0.) {
	uk = uk + .001 + uk * .001;
    }
    if (uk <= 0.) {
	uk = .001;
    }
    if (rad > v[9]) {
	goto L320;
    }

/*        ***  SMALLER RADIUS  *** */
    lk = 0.;
    if (alphak > 0.) {
	lk = w[lk0];
    }
/* Computing MAX */
    d__1 = lk, d__2 = t - w[emax];
    lk = max(d__1,d__2);
    if (v[3] > 0.) {
/* Computing MAX */
	d__1 = lk, d__2 = (v[3] - rad) * w[phipin];
	lk = max(d__1,d__2);
    }
    goto L250;

/*     ***  BIGGER RADIUS  *** */
L320:
    if (alphak > 0.) {
/* Computing MIN */
	d__1 = uk, d__2 = w[uk0];
	uk = min(d__1,d__2);
    }
/* Computing MAX */
    d__1 = 0., d__2 = -v[3], d__1 = max(d__1,d__2), d__2 = t - w[emax];
    lk = max(d__1,d__2);
    if (v[3] > 0.) {
/* Computing MAX */
	d__1 = lk, d__2 = (v[3] - rad) * w[phipin];
	lk = max(d__1,d__2);
    }
    goto L250;

/*  ***  DECIDE WHETHER TO CHECK FOR SPECIAL CASE... IN PRACTICE (FROM */
/*  ***  THE STANDPOINT OF THE CALLING OPTIMIZATION CODE) IT SEEMS BEST */
/*  ***  NOT TO CHECK UNTIL A FEW ITERATIONS HAVE FAILED -- HENCE THE */
/*  ***  TEST ON KAMIN BELOW. */

L330:
    delta = alphak + min(0.,v[3]);
    twopsi = alphak * dst * dst + gtsta;
    if (*ka >= kamin) {
	goto L340;
    }
/*     *** IF THE TEST IN REF. 2 IS SATISFIED, FALL THROUGH TO HANDLE */
/*     *** THE SPECIAL CASE (AS SOON AS THE MORE-SORENSEN TEST DETECTS */
/*     *** IT). */
    if (psifac >= big) {
	goto L340;
    }
    if (delta >= psifac * twopsi) {
	goto L370;
    }

/*  ***  CHECK FOR THE SPECIAL CASE OF  H + ALPHA*D**2  (NEARLY) */
/*  ***  SINGULAR.  USE ONE STEP OF INVERSE POWER METHOD WITH START */
/*  ***  FROM DL7SVN TO OBTAIN APPROXIMATE EIGENVECTOR CORRESPONDING */
/*  ***  TO SMALLEST EIGENVALUE OF (D**-1)*H*(D**-1).  DL7SVN RETURNS */
/*  ***  X AND W WITH  L*W = X. */

L340:
    t = dl7svn_(p, &l[1], &w[x], &w[1]);

/*     ***  NORMALIZE W  *** */
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L350: */
	w[i__] = t * w[i__];
    }
/*     ***  COMPLETE CURRENT INV. POWER ITER. -- REPLACE W BY (L**-T)*W. */
    dl7itv_(p, &w[1], &l[1], &w[1]);
    t2 = 1. / dv2nrm_(p, &w[1]);
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L360: */
	w[i__] = t2 * w[i__];
    }
    t = t2 * t;

/*  ***  NOW W IS THE DESIRED APPROXIMATE (UNIT) EIGENVECTOR AND */
/*  ***  T*X = ((D**-1)*H*(D**-1) + ALPHAK*I)*W. */

    sw = dd7tpr_(p, &w[q], &w[1]);
    t1 = (rad + dst) * (rad - dst);
    root = sqrt(sw * sw + t1);
    if (sw < 0.) {
	root = -root;
    }
    si = t1 / (sw + root);

/*  ***  THE ACTUAL TEST FOR THE SPECIAL CASE... */

/* Computing 2nd power */
    d__1 = t2 * si;
/* Computing 2nd power */
    d__2 = dst;
    if (d__1 * d__1 <= eps * (d__2 * d__2 + alphak * radsq)) {
	goto L380;
    }

/*  ***  UPDATE UPPER BOUND ON SMALLEST EIGENVALUE (WHEN NOT POSITIVE) */
/*  ***  (AS RECOMMENDED BY MORE AND SORENSEN) AND CONTINUE... */

    if (v[3] <= 0.) {
/* Computing MIN */
/* Computing 2nd power */
	d__3 = t2;
	d__1 = v[3], d__2 = d__3 * d__3 - alphak;
	v[3] = min(d__1,d__2);
    }
/* Computing MAX */
    d__1 = lk, d__2 = -v[3];
    lk = max(d__1,d__2);

/*  ***  CHECK WHETHER WE CAN HOPE TO DETECT THE SPECIAL CASE IN */
/*  ***  THE AVAILABLE ARITHMETIC.  ACCEPT STEP AS IT IS IF NOT. */

/*     ***  IF NOT YET AVAILABLE, OBTAIN MACHINE DEPENDENT VALUE DGXFAC. */
L370:
    if (dgxfac == 0.) {
	dgxfac = dr7mdc_(&c__3) * 50.;
    }

    if (delta > dgxfac * w[dggdmx]) {
	goto L250;
    }
    goto L270;

/*  ***  SPECIAL CASE DETECTED... NEGATE ALPHAK TO INDICATE SPECIAL CASE */

L380:
    alphak = -alphak;
    v[7] = twopsi * .5;

/*  ***  ACCEPT CURRENT STEP IF ADDING SI*W WOULD LEAD TO A */
/*  ***  FURTHER RELATIVE REDUCTION IN PSI OF LESS THAN V(EPSLON)/3. */

    t1 = 0.;
    t = si * (alphak * sw - si * .5 * (alphak + t * dd7tpr_(p, &w[x], &w[1])))
	    ;
    if (t < eps * twopsi / 6.) {
	goto L390;
    }
    v[7] += t;
    dst = rad;
    t1 = -si;
L390:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = q0 + i__;
	w[j] = t1 * w[i__] - w[j];
	step[i__] = w[j] / d__[i__];
/* L400: */
    }
    v[4] = dd7tpr_(p, &dig[1], &w[q]);

/*  ***  SAVE VALUES FOR USE IN A POSSIBLE RESTART  *** */

L410:
    v[2] = dst;
    v[5] = alphak;
    w[lk0] = lk;
    w[uk0] = uk;
    v[9] = rad;
    w[dstsav] = dst;

/*     ***  RESTORE DIAGONAL OF DIHDI  *** */

    j = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j += i__;
	k = diag0 + i__;
	dihdi[j] = w[k];
/* L420: */
    }

    return 0;

/*  ***  LAST CARD OF DG7QTS FOLLOWS  *** */
} /* dg7qts_ */

/* Subroutine */ int dw7zbf_(doublereal *l, integer *n, doublereal *s, 
	doublereal *w, doublereal *y, doublereal *z__)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal cs, cy, ys, shs, theta, epsrt;
    extern /* Subroutine */ int dl7ivm_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dl7tvm_(integer *, doublereal *, doublereal *,
	     doublereal *);


/*  ***  COMPUTE  Y  AND  Z  FOR  DL7UPD  CORRESPONDING TO BFGS UPDATE. */

/*     DIMENSION L(N*(N+1)/2) */

/* --------------------------  PARAMETER USAGE  -------------------------- */

/* L (I/O) CHOLESKY FACTOR OF HESSIAN, A LOWER TRIANG. MATRIX STORED */
/*             COMPACTLY BY ROWS. */
/* N (INPUT) ORDER OF  L  AND LENGTH OF  S,  W,  Y,  Z. */
/* S (INPUT) THE STEP JUST TAKEN. */
/* W (OUTPUT) RIGHT SINGULAR VECTOR OF RANK 1 CORRECTION TO L. */
/* Y (INPUT) CHANGE IN GRADIENTS CORRESPONDING TO S. */
/* Z (OUTPUT) LEFT SINGULAR VECTOR OF RANK 1 CORRECTION TO L. */

/* -------------------------------  NOTES  ------------------------------- */

/*  ***  ALGORITHM NOTES  *** */

/*        WHEN  S  IS COMPUTED IN CERTAIN WAYS, E.G. BY  GQTSTP  OR */
/*     DBLDOG,  IT IS POSSIBLE TO SAVE N**2/2 OPERATIONS SINCE  (L**T)*S */
/*     OR  L*(L**T)*S IS THEN KNOWN. */
/*        IF THE BFGS UPDATE TO L*(L**T) WOULD REDUCE ITS DETERMINANT TO */
/*     LESS THAN EPS TIMES ITS OLD VALUE, THEN THIS ROUTINE IN EFFECT */
/*     REPLACES  Y  BY  THETA*Y + (1 - THETA)*L*(L**T)*S,  WHERE  THETA */
/*     (BETWEEN 0 AND 1) IS CHOSEN TO MAKE THE REDUCTION FACTOR = EPS. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (FALL 1979). */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
/*     MCS-7906671. */

/* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

/*  ***  FUNCTIONS AND SUBROUTINES CALLED  *** */

/* DD7TPR RETURNS INNER PRODUCT OF TWO VECTORS. */
/* DL7IVM MULTIPLIES L**-1 TIMES A VECTOR. */
/* DL7TVM MULTIPLIES L**T TIMES A VECTOR. */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/* --------------------------  LOCAL VARIABLES  -------------------------- */


/*  ***  DATA INITIALIZATIONS  *** */


/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --l;
    --z__;
    --y;
    --w;
    --s;

    /* Function Body */
    dl7tvm_(n, &w[1], &l[1], &s[1]);
    shs = dd7tpr_(n, &w[1], &w[1]);
    ys = dd7tpr_(n, &y[1], &s[1]);
    if (ys >= shs * .1) {
	goto L10;
    }
    theta = shs * .90000000000000002 / (shs - ys);
    epsrt = sqrt(.1);
    cy = theta / (shs * epsrt);
    cs = ((theta - 1.) / epsrt + 1.) / shs;
    goto L20;
L10:
    cy = 1. / (sqrt(ys) * sqrt(shs));
    cs = 1. / shs;
L20:
    dl7ivm_(n, &z__[1], &l[1], &y[1]);
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	z__[i__] = cy * z__[i__] - cs * w[i__];
    }

    return 0;
/*  ***  LAST CARD OF DW7ZBF FOLLOWS  *** */
} /* dw7zbf_ */

/* Subroutine */ int dc7vfn_(integer *iv, doublereal *l, integer *lh, integer 
	*liv, integer *lv, integer *n, integer *p, doublereal *v)
{
    /* Initialized data */

    static doublereal half = .5;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static integer i__, cov;
    extern /* Subroutine */ int dv7scl_(integer *, doublereal *, doublereal *,
	     doublereal *), dl7nvr_(integer *, doublereal *, doublereal *), 
	    dl7tsq_(integer *, doublereal *, doublereal *);


/*  ***  FINISH COVARIANCE COMPUTATION FOR  DRN2G,  DRNSG  *** */



/*  ***  LOCAL VARIABLES  *** */


/*  ***  SUBSCRIPTS FOR IV AND V  *** */


    /* Parameter adjustments */
    --l;
    --iv;
    --v;

    /* Function Body */

/*  ***  BODY  *** */

    iv[1] = iv[55];
    i__ = iv[35] - *p;
    iv[35] = 0;
    iv[55] = 0;
    if (iv[74] <= 0) {
	goto L999;
    }
/* Computing 2nd power */
    i__1 = i__ - 2;
    if (i__1 * i__1 == 1) {
	iv[67] = 1;
    }
    if (iv[57] % 2 != 1) {
	goto L999;
    }

/*     ***  FINISH COMPUTING COVARIANCE MATRIX = INVERSE OF F.D. HESSIAN. */

    cov = abs(iv[56]);
    iv[74] = 0;

    if (iv[26] != 0) {
	goto L999;
    }
    if (i__ >= 2) {
	goto L10;
    }
    dl7nvr_(p, &v[cov], &l[1]);
    dl7tsq_(p, &v[cov], &v[cov]);

L10:
/* Computing MAX */
    i__1 = 1, i__2 = *n - *p;
    d__1 = v[10] / (half * (doublereal) max(i__1,i__2));
    dv7scl_(lh, &v[cov], &d__1, &v[cov]);
    iv[26] = cov;

L999:
    return 0;
/*  ***  LAST LINE OF DC7VFN FOLLOWS  *** */
} /* dc7vfn_ */

/* Subroutine */ int dd7mlp_(integer *n, doublereal *x, doublereal *y, 
	doublereal *z__, integer *k)
{
    /* Initialized data */

    static doublereal one = 1.;

    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, l;
    static doublereal t;


/* ***  SET X = DIAG(Y)**K * Z */
/* ***  FOR X, Z = LOWER TRIANG. MATRICES STORED COMPACTLY BY ROW */
/* ***  K = 1 OR -1. */

    /* Parameter adjustments */
    --y;
    --x;
    --z__;

    /* Function Body */

    l = 1;
    if (*k >= 0) {
	goto L30;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = one / y[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    x[l] = t * z__[l];
	    ++l;
/* L10: */
	}
/* L20: */
    }
    goto L999;

L30:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = y[i__];
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    x[l] = t * z__[l];
	    ++l;
/* L40: */
	}
/* L50: */
    }
L999:
    return 0;
/*  ***  LAST CARD OF DD7MLP FOLLOWS  *** */
} /* dd7mlp_ */

/* Subroutine */ int dl7ivm_(integer *n, doublereal *x, doublereal *l, 
	doublereal *y)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);


/*  ***  SOLVE  L*X = Y, WHERE  L  IS AN  N X N  LOWER TRIANGULAR */
/*  ***  MATRIX STORED COMPACTLY BY ROWS.  X AND Y MAY OCCUPY THE SAME */
/*  ***  STORAGE.  *** */


    /* Parameter adjustments */
    --y;
    --x;
    --l;

    /* Function Body */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (y[k] != 0.) {
	    goto L20;
	}
	x[k] = 0.;
/* L10: */
    }
    goto L999;
L20:
    j = k * (k + 1) / 2;
    x[k] = y[k] / l[j];
    if (k >= *n) {
	goto L999;
    }
    ++k;
    i__1 = *n;
    for (i__ = k; i__ <= i__1; ++i__) {
	i__2 = i__ - 1;
	t = dd7tpr_(&i__2, &l[j + 1], &x[1]);
	j += i__;
	x[i__] = (y[i__] - t) / l[j];
/* L30: */
    }
L999:
    return 0;
/*  ***  LAST CARD OF DL7IVM FOLLOWS  *** */
} /* dl7ivm_ */

/* Subroutine */ int dd7upd_(doublereal *d__, doublereal *dr, integer *iv, 
	integer *liv, integer *lv, integer *n, integer *nd, integer *nn, 
	integer *n2, integer *p, doublereal *v)
{
    /* System generated locals */
    integer dr_dim1, dr_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, k;
    static doublereal t;
    static integer d0, sii, jcn0, jcn1, jcni, jtol0;
    static doublereal vdfac;
    static integer jtoli;
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    ;


/*  ***  UPDATE SCALE VECTOR D FOR NL2IT  *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION V(*) */

/*  ***  LOCAL VARIABLES  *** */


/*     ***  CONSTANTS  *** */


/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/*  ***  EXTERNAL SUBROUTINE  *** */


/* DV7SCP... SETS ALL COMPONENTS OF A VECTOR TO A SCALAR. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */



/* -------------------------------  BODY  -------------------------------- */

    /* Parameter adjustments */
    --iv;
    --v;
    dr_dim1 = *nd;
    dr_offset = 1 + dr_dim1;
    dr -= dr_offset;
    --d__;

    /* Function Body */
    if (iv[16] != 1 && iv[31] > 0) {
	goto L999;
    }
    jcn1 = iv[66];
    jcn0 = abs(jcn1) - 1;
    if (jcn1 < 0) {
	goto L10;
    }
    iv[66] = -jcn1;
    dv7scp_(p, &v[jcn1], &c_b14);
L10:
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	jcni = jcn0 + i__;
	t = v[jcni];
	i__2 = *nn;
	for (k = 1; k <= i__2; ++k) {
/* L20: */
/* Computing MAX */
	    d__2 = t, d__3 = (d__1 = dr[k + i__ * dr_dim1], abs(d__1));
	    t = max(d__2,d__3);
	}
	v[jcni] = t;
/* L30: */
    }
    if (*n2 < *n) {
	goto L999;
    }
    vdfac = v[41];
    jtol0 = iv[59] - 1;
    d0 = jtol0 + *p;
    sii = iv[62] - 1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	sii += i__;
	jcni = jcn0 + i__;
	t = v[jcni];
	if (v[sii] > 0.) {
/* Computing MAX */
	    d__1 = sqrt(v[sii]);
	    t = max(d__1,t);
	}
	jtoli = jtol0 + i__;
	++d0;
	if (t < v[jtoli]) {
/* Computing MAX */
	    d__1 = v[d0], d__2 = v[jtoli];
	    t = max(d__1,d__2);
	}
/* Computing MAX */
	d__1 = vdfac * d__[i__];
	d__[i__] = max(d__1,t);
/* L50: */
    }

L999:
    return 0;
/*  ***  LAST CARD OF DD7UPD FOLLOWS  *** */
} /* dd7upd_ */

/* Subroutine */ int dv7shf_(integer *n, integer *k, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;
    static doublereal t;
    static integer nm1;


/*  ***  SHIFT X(K),...,X(N) LEFT CIRCULARLY ONE POSITION  *** */



    /* Parameter adjustments */
    --x;

    /* Function Body */
    if (*k >= *n) {
	goto L999;
    }
    nm1 = *n - 1;
    t = x[*k];
    i__1 = nm1;
    for (i__ = *k; i__ <= i__1; ++i__) {
/* L10: */
	x[i__] = x[i__ + 1];
    }
    x[*n] = t;
L999:
    return 0;
} /* dv7shf_ */

/* Subroutine */ int ds3grd_(doublereal *alpha, doublereal *b, doublereal *
	d__, doublereal *eta0, doublereal *fx, doublereal *g, integer *irc, 
	integer *p, doublereal *w, doublereal *x)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal), pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static doublereal h__;
    static integer i__;
    static doublereal h0, gi, xi, aai, agi, eta, afx, axi;
    static logical hit;
    static doublereal xih, hmin;
    extern doublereal dr7mdc_(integer *);
    static doublereal machep, alphai, axibar, afxeta, discon;


/*  ***  COMPUTE FINITE DIFFERENCE GRADIENT BY STWEART*S SCHEME  *** */

/*     ***  PARAMETERS  *** */


/* ....................................................................... */

/*     ***  PURPOSE  *** */

/*        THIS SUBROUTINE USES AN EMBELLISHED FORM OF THE FINITE-DIFFER- */
/*     ENCE SCHEME PROPOSED BY STEWART (REF. 1) TO APPROXIMATE THE */
/*     GRADIENT OF THE FUNCTION F(X), WHOSE VALUES ARE SUPPLIED BY */
/*     REVERSE COMMUNICATION. */

/*     ***  PARAMETER DESCRIPTION  *** */

/*  ALPHA IN  (APPROXIMATE) DIAGONAL ELEMENTS OF THE HESSIAN OF F(X). */
/*      B IN  ARRAY OF SIMPLE LOWER AND UPPER BOUNDS ON X.  X MUST */
/*             SATISFY B(1,I) .LE. X(I) .LE. B(2,I), I = 1(1)P. */
/*             FOR ALL I WITH B(1,I) .GE. B(2,I), DS3GRD SIMPLY */
/*             SETS G(I) TO 0. */
/*      D IN  SCALE VECTOR SUCH THAT D(I)*X(I), I = 1,...,P, ARE IN */
/*             COMPARABLE UNITS. */
/*   ETA0 IN  ESTIMATED BOUND ON RELATIVE ERROR IN THE FUNCTION VALUE... */
/*             (TRUE VALUE) = (COMPUTED VALUE)*(1+E),   WHERE */
/*             ABS(E) .LE. ETA0. */
/*     FX I/O ON INPUT,  FX  MUST BE THE COMPUTED VALUE OF F(X).  ON */
/*             OUTPUT WITH IRC = 0, FX HAS BEEN RESTORED TO ITS ORIGINAL */
/*             VALUE, THE ONE IT HAD WHEN DS3GRD WAS LAST CALLED WITH */
/*             IRC = 0. */
/*      G I/O ON INPUT WITH IRC = 0, G SHOULD CONTAIN AN APPROXIMATION */
/*             TO THE GRADIENT OF F NEAR X, E.G., THE GRADIENT AT THE */
/*             PREVIOUS ITERATE.  WHEN DS3GRD RETURNS WITH IRC = 0, G IS */
/*             THE DESIRED FINITE-DIFFERENCE APPROXIMATION TO THE */
/*             GRADIENT AT X. */
/*    IRC I/O INPUT/RETURN CODE... BEFORE THE VERY FIRST CALL ON DS3GRD, */
/*             THE CALLER MUST SET IRC TO 0.  WHENEVER DS3GRD RETURNS A */
/*             NONZERO VALUE (OF AT MOST P) FOR IRC, IT HAS PERTURBED */
/*             SOME COMPONENT OF X... THE CALLER SHOULD EVALUATE F(X) */
/*             AND CALL DS3GRD AGAIN WITH FX = F(X).  IF B PREVENTS */
/*             ESTIMATING G(I) I.E., IF THERE IS AN I WITH */
/*             B(1,I) .LT. B(2,I) BUT WITH B(1,I) SO CLOSE TO B(2,I) */
/*             THAT THE FINITE-DIFFERENCING STEPS CANNOT BE CHOSEN, */
/*             THEN DS3GRD RETURNS WITH IRC .GT. P. */
/*      P IN  THE NUMBER OF VARIABLES (COMPONENTS OF X) ON WHICH F */
/*             DEPENDS. */
/*      X I/O ON INPUT WITH IRC = 0, X IS THE POINT AT WHICH THE */
/*             GRADIENT OF F IS DESIRED.  ON OUTPUT WITH IRC NONZERO, X */
/*             IS THE POINT AT WHICH F SHOULD BE EVALUATED.  ON OUTPUT */
/*             WITH IRC = 0, X HAS BEEN RESTORED TO ITS ORIGINAL VALUE */
/*             (THE ONE IT HAD WHEN DS3GRD WAS LAST CALLED WITH IRC = 0) */
/*             AND G CONTAINS THE DESIRED GRADIENT APPROXIMATION. */
/*      W I/O WORK VECTOR OF LENGTH 6 IN WHICH DS3GRD SAVES CERTAIN */
/*             QUANTITIES WHILE THE CALLER IS EVALUATING F(X) AT A */
/*             PERTURBED X. */

/*     ***  APPLICATION AND USAGE RESTRICTIONS  *** */

/*        THIS ROUTINE IS INTENDED FOR USE WITH QUASI-NEWTON ROUTINES */
/*     FOR UNCONSTRAINED MINIMIZATION (IN WHICH CASE  ALPHA  COMES FROM */
/*     THE DIAGONAL OF THE QUASI-NEWTON HESSIAN APPROXIMATION). */

/*     ***  ALGORITHM NOTES  *** */

/*        THIS CODE DEPARTS FROM THE SCHEME PROPOSED BY STEWART (REF. 1) */
/*     IN ITS GUARDING AGAINST OVERLY LARGE OR SMALL STEP SIZES AND ITS */
/*     HANDLING OF SPECIAL CASES (SUCH AS ZERO COMPONENTS OF ALPHA OR G). */

/*     ***  REFERENCES  *** */

/* 1. STEWART, G.W. (1967), A MODIFICATION OF DAVIDON*S MINIMIZATION */
/*        METHOD TO ACCEPT DIFFERENCE APPROXIMATIONS OF DERIVATIVES, */
/*        J. ASSOC. COMPUT. MACH. 14, PP. 72-83. */

/*     ***  HISTORY  *** */

/*     DESIGNED AND CODED BY DAVID M. GAY (SUMMER 1977/SUMMER 1980). */

/*     ***  GENERAL  *** */

/*        THIS ROUTINE WAS PREPARED IN CONNECTION WITH WORK SUPPORTED BY */
/*     THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS76-00324 AND */
/*     MCS-7906671. */

/* ....................................................................... */

/*     *****  EXTERNAL FUNCTION  ***** */

/* DR7MDC... RETURNS MACHINE-DEPENDENT CONSTANTS. */

/*     ***** INTRINSIC FUNCTIONS ***** */
/* /+ */
/* / */
/*     ***** LOCAL VARIABLES ***** */



/* ---------------------------------  BODY  ------------------------------ */

    /* Parameter adjustments */
    --x;
    --g;
    --d__;
    b -= 3;
    --alpha;
    --w;

    /* Function Body */
    if (*irc < 0) {
	goto L80;
    }
    if (*irc > 0) {
	goto L210;
    }

/*     ***  FRESH START -- GET MACHINE-DEPENDENT CONSTANTS  *** */

/*     STORE MACHEP IN W(1) AND H0 IN W(2), WHERE MACHEP IS THE UNIT */
/*     ROUNDOFF (THE SMALLEST POSITIVE NUMBER SUCH THAT */
/*     1 + MACHEP .GT. 1  AND  1 - MACHEP .LT. 1),  AND  H0 IS THE */
/*     SQUARE-ROOT OF MACHEP. */

    w[1] = dr7mdc_(&c__3);
    w[2] = sqrt(w[1]);

    w[4] = *fx;

/*     ***  INCREMENT  I  AND START COMPUTING  G(I)  *** */

L20:
    i__ = abs(*irc) + 1;
    if (i__ > *p) {
	goto L220;
    }
    *irc = i__;
    if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	goto L30;
    }
    g[i__] = 0.;
    goto L20;
L30:
    afx = abs(w[4]);
    machep = w[1];
    h0 = w[2];
    hmin = machep * 50.;
    xi = x[i__];
    w[6] = xi;
    axi = abs(xi);
/* Computing MAX */
    d__1 = axi, d__2 = 1. / d__[i__];
    axibar = max(d__1,d__2);
    gi = g[i__];
    agi = abs(gi);
    eta = abs(*eta0);
    if (afx > 0.) {
/* Computing MAX */
	d__1 = eta, d__2 = agi * axi * machep / afx;
	eta = max(d__1,d__2);
    }
    alphai = alpha[i__];
    if (alphai == 0.) {
	goto L130;
    }
    if (gi == 0. || *fx == 0.) {
	goto L140;
    }
    afxeta = afx * eta;
    aai = abs(alphai);

/*        *** COMPUTE H = STEWART*S FORWARD-DIFFERENCE STEP SIZE. */

/* Computing 2nd power */
    d__1 = gi;
    if (d__1 * d__1 <= afxeta * aai) {
	goto L40;
    }
    h__ = sqrt(afxeta / aai) * 2.;
    h__ *= 1. - aai * h__ / (aai * 3. * h__ + agi * 4.);
    goto L50;
/* 40      H = TWO*(AFXETA*AGI/(AAI**2))**(ONE/THREE) */
L40:
    d__1 = afxeta * agi;
    h__ = pow_dd(&d__1, &c_b325) * 2. * pow_dd(&aai, &c_b913);
    h__ *= 1. - agi * 2. / (aai * 3. * h__ + agi * 4.);

/*        ***  ENSURE THAT  H  IS NOT INSIGNIFICANTLY SMALL  *** */

L50:
/* Computing MAX */
    d__1 = h__, d__2 = hmin * axibar;
    h__ = max(d__1,d__2);

/*        *** USE FORWARD DIFFERENCE IF BOUND ON TRUNCATION ERROR IS AT */
/*        *** MOST 10**-3. */

    if (aai * h__ <= agi * .002) {
	goto L120;
    }

/*        *** COMPUTE H = STEWART*S STEP FOR CENTRAL DIFFERENCE. */

    discon = afxeta * 2e3;
/* Computing 2nd power */
    d__1 = gi;
    h__ = discon / (agi + sqrt(d__1 * d__1 + aai * discon));

/*        ***  ENSURE THAT  H  IS NEITHER TOO SMALL NOR TOO BIG  *** */

/* Computing MAX */
    d__1 = h__, d__2 = hmin * axibar;
    h__ = max(d__1,d__2);
    if (h__ >= axibar * .02) {
	h__ = axibar * pow_dd(&h0, &c_b915);
    }

/*        ***  COMPUTE CENTRAL DIFFERENCE  *** */

    xih = xi + h__;
    if (xi - h__ < b[(i__ << 1) + 1]) {
	goto L60;
    }
    *irc = -i__;
    if (xih <= b[(i__ << 1) + 2]) {
	goto L200;
    }
    h__ = -h__;
    xih = xi + h__;
    if (xi + h__ * 2. < b[(i__ << 1) + 1]) {
	goto L190;
    }
    goto L70;
L60:
    if (xi + h__ * 2. > b[(i__ << 1) + 2]) {
	goto L190;
    }
/*        *** MUST DO OFF-SIDE CENTRAL DIFFERENCE *** */
L70:
    *irc = -(i__ + *p);
    goto L200;

L80:
    i__ = -(*irc);
    if (i__ <= *p) {
	goto L100;
    }
    i__ -= *p;
    if (i__ > *p) {
	goto L90;
    }
    w[3] = *fx;
    h__ = w[5] * 2.;
    xih = w[6] + h__;
    *irc -= *p;
    goto L200;

/*    *** FINISH OFF-SIDE CENTRAL DIFFERENCE *** */

L90:
    i__ -= *p;
    g[i__] = (w[3] * 4. - *fx - w[4] * 3.) / w[5];
    *irc = i__;
    x[i__] = w[6];
    goto L20;

L100:
    h__ = -w[5];
    if (h__ > 0.) {
	goto L110;
    }
    w[3] = *fx;
    xih = w[6] + h__;
    goto L200;

L110:
    g[i__] = (w[3] - *fx) / (h__ * 2.);
    x[i__] = w[6];
    goto L20;

/*     ***  COMPUTE FORWARD DIFFERENCES IN VARIOUS CASES  *** */

L120:
    if (h__ >= axibar * .02) {
	h__ = h0 * axibar;
    }
    if (alphai * gi < 0.) {
	h__ = -h__;
    }
    goto L150;
L130:
    h__ = axibar;
    goto L150;
L140:
    h__ = h0 * axibar;

L150:
    hit = FALSE_;
L160:
    xih = xi + h__;
    if (h__ > 0.) {
	goto L170;
    }
    if (xih >= b[(i__ << 1) + 1]) {
	goto L200;
    }
    goto L180;
L170:
    if (xih <= b[(i__ << 1) + 2]) {
	goto L200;
    }
L180:
    if (hit) {
	goto L190;
    }
    hit = TRUE_;
    h__ = -h__;
    goto L160;

/*        *** ERROR RETURN... */
L190:
    *irc = i__ + *p;
    goto L230;

/*        *** RETURN FOR NEW FUNCTION VALUE... */
L200:
    x[i__] = xih;
    w[5] = h__;
    goto L999;

/*     ***  COMPUTE ACTUAL FORWARD DIFFERENCE  *** */

L210:
    g[*irc] = (*fx - w[4]) / w[5];
    x[*irc] = w[6];
    goto L20;

/*  ***  RESTORE FX AND INDICATE THAT G HAS BEEN COMPUTED  *** */

L220:
    *irc = 0;
L230:
    *fx = w[4];

L999:
    return 0;
/*  ***  LAST LINE OF DS3GRD FOLLOWS  *** */
} /* ds3grd_ */

/* Subroutine */ int dl7upd_(doublereal *beta, doublereal *gamma, doublereal *
	l, doublereal *lambda, doublereal *lplus, integer *n, doublereal *w, 
	doublereal *z__)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal a, b;
    static integer i__, j, k;
    static doublereal s, bj, gj;
    static integer ij, jj;
    static doublereal lj, wj, nu, zj;
    static integer jp1, nm1, np1;
    static doublereal eta, lij, ljj, theta;


/*  ***  COMPUTE LPLUS = SECANT UPDATE OF L  *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION L(N*(N+1)/2), LPLUS(N*(N+1)/2) */

/* --------------------------  PARAMETER USAGE  -------------------------- */

/*   BETA = SCRATCH VECTOR. */
/*  GAMMA = SCRATCH VECTOR. */
/*      L (INPUT) LOWER TRIANGULAR MATRIX, STORED ROWWISE. */
/* LAMBDA = SCRATCH VECTOR. */
/*  LPLUS (OUTPUT) LOWER TRIANGULAR MATRIX, STORED ROWWISE, WHICH MAY */
/*             OCCUPY THE SAME STORAGE AS  L. */
/*      N (INPUT) LENGTH OF VECTOR PARAMETERS AND ORDER OF MATRICES. */
/*      W (INPUT, DESTROYED ON OUTPUT) RIGHT SINGULAR VECTOR OF RANK 1 */
/*             CORRECTION TO  L. */
/*      Z (INPUT, DESTROYED ON OUTPUT) LEFT SINGULAR VECTOR OF RANK 1 */
/*             CORRECTION TO  L. */

/* -------------------------------  NOTES  ------------------------------- */

/*  ***  APPLICATION AND USAGE RESTRICTIONS  *** */

/*        THIS ROUTINE UPDATES THE CHOLESKY FACTOR  L  OF A SYMMETRIC */
/*     POSITIVE DEFINITE MATRIX TO WHICH A SECANT UPDATE IS BEING */
/*     APPLIED -- IT COMPUTES A CHOLESKY FACTOR  LPLUS  OF */
/*     L * (I + Z*W**T) * (I + W*Z**T) * L**T.  IT IS ASSUMED THAT  W */
/*     AND  Z  HAVE BEEN CHOSEN SO THAT THE UPDATED MATRIX IS STRICTLY */
/*     POSITIVE DEFINITE. */

/*  ***  ALGORITHM NOTES  *** */

/*        THIS CODE USES RECURRENCE 3 OF REF. 1 (WITH D(J) = 1 FOR ALL J) */
/*     TO COMPUTE  LPLUS  OF THE FORM  L * (I + Z*W**T) * Q,  WHERE  Q */
/*     IS AN ORTHOGONAL MATRIX THAT MAKES THE RESULT LOWER TRIANGULAR. */
/*        LPLUS MAY HAVE SOME NEGATIVE DIAGONAL ELEMENTS. */

/*  ***  REFERENCES  *** */

/* 1.  GOLDFARB, D. (1976), FACTORIZED VARIABLE METRIC METHODS FOR UNCON- */
/*             STRAINED OPTIMIZATION, MATH. COMPUT. 30, PP. 796-811. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY (FALL 1979). */
/*     THIS SUBROUTINE WAS WRITTEN IN CONNECTION WITH RESEARCH SUPPORTED */
/*     BY THE NATIONAL SCIENCE FOUNDATION UNDER GRANTS MCS-7600324 AND */
/*     MCS-7906671. */

/* ------------------------  EXTERNAL QUANTITIES  ------------------------ */

/*  ***  INTRINSIC FUNCTIONS  *** */
/* /+ */
/* / */
/* --------------------------  LOCAL VARIABLES  -------------------------- */


/*  ***  DATA INITIALIZATIONS  *** */


/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --l;
    --lplus;
    --z__;
    --w;
    --lambda;
    --gamma;
    --beta;

    /* Function Body */
    nu = 1.;
    eta = 0.;
    if (*n <= 1) {
	goto L30;
    }
    nm1 = *n - 1;

/*  ***  TEMPORARILY STORE S(J) = SUM OVER K = J+1 TO N OF W(K)**2 IN */
/*  ***  LAMBDA(J). */

    s = 0.;
    i__1 = nm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = *n - i__;
/* Computing 2nd power */
	d__1 = w[j + 1];
	s += d__1 * d__1;
	lambda[j] = s;
/* L10: */
    }

/*  ***  COMPUTE LAMBDA, GAMMA, AND BETA BY GOLDFARB*S RECURRENCE 3. */

    i__1 = nm1;
    for (j = 1; j <= i__1; ++j) {
	wj = w[j];
	a = nu * z__[j] - eta * wj;
	theta = a * wj + 1.;
	s = a * lambda[j];
/* Computing 2nd power */
	d__1 = theta;
	lj = sqrt(d__1 * d__1 + a * s);
	if (theta > 0.) {
	    lj = -lj;
	}
	lambda[j] = lj;
	b = theta * wj + s;
	gamma[j] = b * nu / lj;
	beta[j] = (a - b * eta) / lj;
	nu = -nu / lj;
/* Computing 2nd power */
	d__1 = a;
	eta = -(eta + d__1 * d__1 / (theta - lj)) / lj;
/* L20: */
    }
L30:
    lambda[*n] = (nu * z__[*n] - eta * w[*n]) * w[*n] + 1.;

/*  ***  UPDATE L, GRADUALLY OVERWRITING  W  AND  Z  WITH  L*W  AND  L*Z. */

    np1 = *n + 1;
    jj = *n * (*n + 1) / 2;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	j = np1 - k;
	lj = lambda[j];
	ljj = l[jj];
	lplus[jj] = lj * ljj;
	wj = w[j];
	w[j] = ljj * wj;
	zj = z__[j];
	z__[j] = ljj * zj;
	if (k == 1) {
	    goto L50;
	}
	bj = beta[j];
	gj = gamma[j];
	ij = jj + j;
	jp1 = j + 1;
	i__2 = *n;
	for (i__ = jp1; i__ <= i__2; ++i__) {
	    lij = l[ij];
	    lplus[ij] = lj * lij + bj * w[i__] + gj * z__[i__];
	    w[i__] += lij * wj;
	    z__[i__] += lij * zj;
	    ij += i__;
/* L40: */
	}
L50:
	jj -= j;
/* L60: */
    }

    return 0;
/*  ***  LAST CARD OF DL7UPD FOLLOWS  *** */
} /* dl7upd_ */

/* Subroutine */ int do7prd_(integer *l, integer *ls, integer *p, doublereal *
	s, doublereal *w, doublereal *y, doublereal *z__)
{
    /* Initialized data */

    static doublereal zero = 0.;

    /* System generated locals */
    integer y_dim1, y_offset, z_dim1, z_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, m;
    static doublereal wk, yi;


/*  ***  FOR I = 1..L, SET S = S + W(I)*Y(.,I)*(Z(.,I)**T), I.E., */
/*  ***        ADD W(I) TIMES THE OUTER PRODUCT OF Y(.,I) AND Z(.,I). */

/*     DIMENSION S(P*(P+1)/2) */

    /* Parameter adjustments */
    --w;
    --s;
    z_dim1 = *p;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    y_dim1 = *p;
    y_offset = 1 + y_dim1;
    y -= y_offset;

    /* Function Body */

    i__1 = *l;
    for (k = 1; k <= i__1; ++k) {
	wk = w[k];
	if (wk == zero) {
	    goto L30;
	}
	m = 1;
	i__2 = *p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    yi = wk * y[i__ + k * y_dim1];
	    i__3 = i__;
	    for (j = 1; j <= i__3; ++j) {
		s[m] += yi * z__[j + k * z_dim1];
		++m;
/* L10: */
	    }
/* L20: */
	}
L30:
	;
    }

    return 0;
/*  ***  LAST CARD OF DO7PRD FOLLOWS  *** */
} /* do7prd_ */

/* Subroutine */ int dv7vmp_(integer *n, doublereal *x, doublereal *y, 
	doublereal *z__, integer *k)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__;


/* ***  SET X(I) = Y(I) * Z(I)**K, 1 .LE. I .LE. N (FOR K = 1 OR -1)  *** */


    /* Parameter adjustments */
    --z__;
    --y;
    --x;

    /* Function Body */
    if (*k >= 0) {
	goto L20;
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	x[i__] = y[i__] / z__[i__];
    }
    goto L999;

L20:
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L30: */
	x[i__] = y[i__] * z__[i__];
    }
L999:
    return 0;
/*  ***  LAST CARD OF DV7VMP FOLLOWS  *** */
} /* dv7vmp_ */

/* Subroutine */ int dsm_(integer *m, integer *n, integer *npairs, integer *
	indrow, integer *indcol, integer *ngrp, integer *maxgrp, integer *
	mingrp, integer *info, integer *ipntr, integer *jpntr, integer *iwa, 
	integer *liwa, logical *bwa)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, jp, ir, jpl, jpu, nnz;
    extern /* Subroutine */ int i7do_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, logical *), d7egr_(
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, logical *), m7seq_(integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    logical *), m7slo_(integer *, integer *, integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, logical *), s7etr_(integer *, integer *, 
	    integer *, integer *, integer *, integer *, integer *), s7rtdt_(
	    integer *, integer *, integer *, integer *, integer *, integer *),
	     n7msrt_(integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *);
    static integer maxclq, numgrp;

/*     ********** */

/*     SUBROUTINE DSM */

/*     THE PURPOSE OF DSM IS TO DETERMINE AN OPTIMAL OR NEAR- */
/*     OPTIMAL CONSISTENT PARTITION OF THE COLUMNS OF A SPARSE */
/*     M BY N MATRIX A. */

/*     THE SPARSITY PATTERN OF THE MATRIX A IS SPECIFIED BY */
/*     THE ARRAYS INDROW AND INDCOL. ON INPUT THE INDICES */
/*     FOR THE NON-ZERO ELEMENTS OF A ARE */

/*           INDROW(K),INDCOL(K), K = 1,2,...,NPAIRS. */

/*     THE (INDROW,INDCOL) PAIRS MAY BE SPECIFIED IN ANY ORDER. */
/*     DUPLICATE INPUT PAIRS ARE PERMITTED, BUT THE SUBROUTINE */
/*     ELIMINATES THEM. */

/*     THE SUBROUTINE PARTITIONS THE COLUMNS OF A INTO GROUPS */
/*     SUCH THAT COLUMNS IN THE SAME GROUP DO NOT HAVE A */
/*     NON-ZERO IN THE SAME ROW POSITION. A PARTITION OF THE */
/*     COLUMNS OF A WITH THIS PROPERTY IS CONSISTENT WITH THE */
/*     DIRECT DETERMINATION OF A. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE DSM(M,N,NPAIRS,INDROW,INDCOL,NGRP,MAXGRP,MINGRP, */
/*                      INFO,IPNTR,JPNTR,IWA,LIWA,BWA) */

/*     WHERE */

/*       M IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF ROWS OF A. */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       NPAIRS IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE */
/*         NUMBER OF (INDROW,INDCOL) PAIRS USED TO DESCRIBE THE */
/*         SPARSITY PATTERN OF A. */

/*       INDROW IS AN INTEGER ARRAY OF LENGTH NPAIRS. ON INPUT INDROW */
/*         MUST CONTAIN THE ROW INDICES OF THE NON-ZERO ELEMENTS OF A. */
/*         ON OUTPUT INDROW IS PERMUTED SO THAT THE CORRESPONDING */
/*         COLUMN INDICES ARE IN NON-DECREASING ORDER. THE COLUMN */
/*         INDICES CAN BE RECOVERED FROM THE ARRAY JPNTR. */

/*       INDCOL IS AN INTEGER ARRAY OF LENGTH NPAIRS. ON INPUT INDCOL */
/*         MUST CONTAIN THE COLUMN INDICES OF THE NON-ZERO ELEMENTS OF */
/*         A. ON OUTPUT INDCOL IS PERMUTED SO THAT THE CORRESPONDING */
/*         ROW INDICES ARE IN NON-DECREASING ORDER. THE ROW INDICES */
/*         CAN BE RECOVERED FROM THE ARRAY IPNTR. */

/*       NGRP IS AN INTEGER OUTPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE PARTITION OF THE COLUMNS OF A. COLUMN JCOL BELONGS */
/*         TO GROUP NGRP(JCOL). */

/*       MAXGRP IS AN INTEGER OUTPUT VARIABLE WHICH SPECIFIES THE */
/*         NUMBER OF GROUPS IN THE PARTITION OF THE COLUMNS OF A. */

/*       MINGRP IS AN INTEGER OUTPUT VARIABLE WHICH SPECIFIES A LOWER */
/*         BOUND FOR THE NUMBER OF GROUPS IN ANY CONSISTENT PARTITION */
/*         OF THE COLUMNS OF A. */

/*       INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS. FOR */
/*         NORMAL TERMINATION INFO = 1. IF M, N, OR NPAIRS IS NOT */
/*         POSITIVE OR LIWA IS LESS THAN MAX(M,6*N), THEN INFO = 0. */
/*         IF THE K-TH ELEMENT OF INDROW IS NOT AN INTEGER BETWEEN */
/*         1 AND M OR THE K-TH ELEMENT OF INDCOL IS NOT AN INTEGER */
/*         BETWEEN 1 AND N, THEN INFO = -K. */

/*       IPNTR IS AN INTEGER OUTPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(M+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       JPNTR IS AN INTEGER OUTPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       IWA IS AN INTEGER WORK ARRAY OF LENGTH LIWA. */

/*       LIWA IS A POSITIVE INTEGER INPUT VARIABLE NOT LESS THAN */
/*         MAX(M,6*N). */

/*       BWA IS A LOGICAL WORK ARRAY OF LENGTH N. */

/*     SUBPROGRAMS CALLED */

/*       MINPACK-SUPPLIED ...D7EGR,I7DO,N7MSRT,M7SEQ,S7ETR,M7SLO,S7RTDT */

/*       FORTRAN-SUPPLIED ... MAX0 */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     CHECK THE INPUT DATA. */

    /* Parameter adjustments */
    --bwa;
    --ngrp;
    --indcol;
    --indrow;
    --ipntr;
    --jpntr;
    --iwa;

    /* Function Body */
    *info = 0;
/* Computing MAX */
    i__1 = *m, i__2 = *n * 6;
    if (*m < 1 || *n < 1 || *npairs < 1 || *liwa < max(i__1,i__2)) {
	goto L130;
    }
    i__1 = *npairs;
    for (k = 1; k <= i__1; ++k) {
	*info = -k;
	if (indrow[k] < 1 || indrow[k] > *m || indcol[k] < 1 || indcol[k] > *
		n) {
	    goto L130;
	}
/* L10: */
    }
    *info = 1;

/*     SORT THE DATA STRUCTURE BY COLUMNS. */

    s7rtdt_(n, npairs, &indrow[1], &indcol[1], &jpntr[1], &iwa[1]);

/*     COMPRESS THE DATA AND DETERMINE THE NUMBER OF */
/*     NON-ZERO ELEMENTS OF A. */

    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	iwa[i__] = 0;
/* L20: */
    }
    nnz = 0;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	jpl = jpntr[j];
	jpu = jpntr[j + 1] - 1;
	jpntr[j] = nnz + 1;
	if (jpu < jpl) {
	    goto L60;
	}
	i__2 = jpu;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];
	    if (iwa[ir] != 0) {
		goto L30;
	    }
	    ++nnz;
	    indrow[nnz] = ir;
	    iwa[ir] = 1;
L30:
/* L40: */
	    ;
	}
	jpl = jpntr[j];
	i__2 = nnz;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];
	    iwa[ir] = 0;
/* L50: */
	}
L60:
/* L70: */
	;
    }
    jpntr[*n + 1] = nnz + 1;

/*     EXTEND THE DATA STRUCTURE TO ROWS. */

    s7etr_(m, n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[1]);

/*     DETERMINE A LOWER BOUND FOR THE NUMBER OF GROUPS. */

    *mingrp = 0;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* Computing MAX */
	i__2 = *mingrp, i__3 = ipntr[i__ + 1] - ipntr[i__];
	*mingrp = max(i__2,i__3);
/* L80: */
    }

/*     DETERMINE THE DEGREE SEQUENCE FOR THE INTERSECTION */
/*     GRAPH OF THE COLUMNS OF A. */

    d7egr_(n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[*n * 5 + 1], 
	    &iwa[*n + 1], &bwa[1]);

/*     COLOR THE INTERSECTION GRAPH OF THE COLUMNS OF A */
/*     WITH THE SMALLEST-LAST (SL) ORDERING. */

    m7slo_(n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[*n * 5 + 1], 
	    &iwa[(*n << 2) + 1], &maxclq, &iwa[1], &iwa[*n + 1], &iwa[(*n << 
	    1) + 1], &iwa[*n * 3 + 1], &bwa[1]);
    m7seq_(n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[(*n << 2) + 
	    1], &ngrp[1], maxgrp, &iwa[*n + 1], &bwa[1]);
    *mingrp = max(*mingrp,maxclq);
    if (*maxgrp == *mingrp) {
	goto L130;
    }

/*     COLOR THE INTERSECTION GRAPH OF THE COLUMNS OF A */
/*     WITH THE INCIDENCE-DEGREE (ID) ORDERING. */

    i7do_(m, n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[*n * 5 + 1]
	    , &iwa[(*n << 2) + 1], &maxclq, &iwa[1], &iwa[*n + 1], &iwa[(*n <<
	     1) + 1], &iwa[*n * 3 + 1], &bwa[1]);
    m7seq_(n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[(*n << 2) + 
	    1], &iwa[1], &numgrp, &iwa[*n + 1], &bwa[1]);
    *mingrp = max(*mingrp,maxclq);
    if (numgrp >= *maxgrp) {
	goto L100;
    }
    *maxgrp = numgrp;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ngrp[j] = iwa[j];
/* L90: */
    }
    if (*maxgrp == *mingrp) {
	goto L130;
    }
L100:

/*     COLOR THE INTERSECTION GRAPH OF THE COLUMNS OF A */
/*     WITH THE LARGEST-FIRST (LF) ORDERING. */

    i__1 = *n - 1;
    n7msrt_(n, &i__1, &iwa[*n * 5 + 1], &c_n1, &iwa[(*n << 2) + 1], &iwa[(*n 
	    << 1) + 1], &iwa[*n + 1]);
    m7seq_(n, &indrow[1], &jpntr[1], &indcol[1], &ipntr[1], &iwa[(*n << 2) + 
	    1], &iwa[1], &numgrp, &iwa[*n + 1], &bwa[1]);
    if (numgrp >= *maxgrp) {
	goto L120;
    }
    *maxgrp = numgrp;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ngrp[j] = iwa[j];
/* L110: */
    }
L120:

/*     EXIT FROM PROGRAM. */

L130:
    return 0;

/*     LAST CARD OF SUBROUTINE DSM. */

} /* dsm_ */

/* Subroutine */ int m7seq_(integer *n, integer *indrow, integer *jpntr, 
	integer *indcol, integer *ipntr, integer *list, integer *ngrp, 
	integer *maxgrp, integer *iwa, logical *bwa)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer j, l, ic, ip, jp, ir, deg, ipl, jpl, ipu, jpu, jcol, 
	    numgrp;

/*     ********** */

/*     SUBROUTINE M7SEQ */

/*     GIVEN THE SPARSITY PATTERN OF AN M BY N MATRIX A, THIS */
/*     SUBROUTINE DETERMINES A CONSISTENT PARTITION OF THE */
/*     COLUMNS OF A BY A SEQUENTIAL ALGORITHM. */

/*     A CONSISTENT PARTITION IS DEFINED IN TERMS OF THE LOOPLESS */
/*     GRAPH G WITH VERTICES A(J), J = 1,2,...,N WHERE A(J) IS THE */
/*     J-TH COLUMN OF A AND WITH EDGE (A(I),A(J)) IF AND ONLY IF */
/*     COLUMNS I AND J HAVE A NON-ZERO IN THE SAME ROW POSITION. */

/*     A PARTITION OF THE COLUMNS OF A INTO GROUPS IS CONSISTENT */
/*     IF THE COLUMNS IN ANY GROUP ARE NOT ADJACENT IN THE GRAPH G. */
/*     IN GRAPH-THEORY TERMINOLOGY, A CONSISTENT PARTITION OF THE */
/*     COLUMNS OF A CORRESPONDS TO A COLORING OF THE GRAPH G. */

/*     THE SUBROUTINE EXAMINES THE COLUMNS IN THE ORDER SPECIFIED */
/*     BY THE ARRAY LIST, AND ASSIGNS THE CURRENT COLUMN TO THE */
/*     GROUP WITH THE SMALLEST POSSIBLE NUMBER. */

/*     NOTE THAT THE VALUE OF M IS NOT NEEDED BY M7SEQ AND IS */
/*     THEREFORE NOT PRESENT IN THE SUBROUTINE STATEMENT. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE M7SEQ(N,INDROW,JPNTR,INDCOL,IPNTR,LIST,NGRP,MAXGRP, */
/*                      IWA,BWA) */

/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF COLUMNS OF A. */

/*       INDROW IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE ROW */
/*         INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       JPNTR IS AN INTEGER INPUT ARRAY OF LENGTH N + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE ROW INDICES IN INDROW. */
/*         THE ROW INDICES FOR COLUMN J ARE */

/*               INDROW(K), K = JPNTR(J),...,JPNTR(J+1)-1. */

/*         NOTE THAT JPNTR(N+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       INDCOL IS AN INTEGER INPUT ARRAY WHICH CONTAINS THE */
/*         COLUMN INDICES FOR THE NON-ZEROES IN THE MATRIX A. */

/*       IPNTR IS AN INTEGER INPUT ARRAY OF LENGTH M + 1 WHICH */
/*         SPECIFIES THE LOCATIONS OF THE COLUMN INDICES IN INDCOL. */
/*         THE COLUMN INDICES FOR ROW I ARE */

/*               INDCOL(K), K = IPNTR(I),...,IPNTR(I+1)-1. */

/*         NOTE THAT IPNTR(M+1)-1 IS THEN THE NUMBER OF NON-ZERO */
/*         ELEMENTS OF THE MATRIX A. */

/*       LIST IS AN INTEGER INPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE ORDER TO BE USED BY THE SEQUENTIAL ALGORITHM. */
/*         THE J-TH COLUMN IN THIS ORDER IS LIST(J). */

/*       NGRP IS AN INTEGER OUTPUT ARRAY OF LENGTH N WHICH SPECIFIES */
/*         THE PARTITION OF THE COLUMNS OF A. COLUMN JCOL BELONGS */
/*         TO GROUP NGRP(JCOL). */

/*       MAXGRP IS AN INTEGER OUTPUT VARIABLE WHICH SPECIFIES THE */
/*         NUMBER OF GROUPS IN THE PARTITION OF THE COLUMNS OF A. */

/*       IWA IS AN INTEGER WORK ARRAY OF LENGTH N. */

/*       BWA IS A LOGICAL WORK ARRAY OF LENGTH N. */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1982. */
/*     THOMAS F. COLEMAN, BURTON S. GARBOW, JORGE J. MORE */

/*     ********** */

/*     INITIALIZATION BLOCK. */

    /* Parameter adjustments */
    --bwa;
    --iwa;
    --ngrp;
    --list;
    --indrow;
    --jpntr;
    --indcol;
    --ipntr;

    /* Function Body */
    *maxgrp = 0;
    i__1 = *n;
    for (jp = 1; jp <= i__1; ++jp) {
	ngrp[jp] = *n;
	bwa[jp] = FALSE_;
/* L10: */
    }
    bwa[*n] = TRUE_;

/*     BEGINNING OF ITERATION LOOP. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	jcol = list[j];

/*        FIND ALL COLUMNS ADJACENT TO COLUMN JCOL. */

	deg = 0;

/*        DETERMINE ALL POSITIONS (IR,JCOL) WHICH CORRESPOND */
/*        TO NON-ZEROES IN THE MATRIX. */

	jpl = jpntr[jcol];
	jpu = jpntr[jcol + 1] - 1;
	if (jpu < jpl) {
	    goto L50;
	}
	i__2 = jpu;
	for (jp = jpl; jp <= i__2; ++jp) {
	    ir = indrow[jp];

/*           FOR EACH ROW IR, DETERMINE ALL POSITIONS (IR,IC) */
/*           WHICH CORRESPOND TO NON-ZEROES IN THE MATRIX. */

	    ipl = ipntr[ir];
	    ipu = ipntr[ir + 1] - 1;
	    i__3 = ipu;
	    for (ip = ipl; ip <= i__3; ++ip) {
		ic = indcol[ip];
		l = ngrp[ic];

/*              ARRAY BWA MARKS THE GROUP NUMBERS OF THE */
/*              COLUMNS WHICH ARE ADJACENT TO COLUMN JCOL. */
/*              ARRAY IWA RECORDS THE MARKED GROUP NUMBERS. */

		if (bwa[l]) {
		    goto L20;
		}
		bwa[l] = TRUE_;
		++deg;
		iwa[deg] = l;
L20:
/* L30: */
		;
	    }
/* L40: */
	}
L50:

/*        ASSIGN THE SMALLEST UN-MARKED GROUP NUMBER TO JCOL. */

	i__2 = *n;
	for (jp = 1; jp <= i__2; ++jp) {
	    numgrp = jp;
	    if (! bwa[jp]) {
		goto L70;
	    }
/* L60: */
	}
L70:
	ngrp[jcol] = numgrp;
	*maxgrp = max(*maxgrp,numgrp);

/*        UN-MARK THE GROUP NUMBERS. */

	if (deg < 1) {
	    goto L90;
	}
	i__2 = deg;
	for (jp = 1; jp <= i__2; ++jp) {
	    l = iwa[jp];
	    bwa[l] = FALSE_;
/* L80: */
	}
L90:
/* L100: */
	;
    }

/*        END OF ITERATION LOOP. */

    return 0;

/*     LAST CARD OF SUBROUTINE M7SEQ. */

} /* m7seq_ */

/* Subroutine */ int dl7tsq_(integer *n, doublereal *a, doublereal *l)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, m, i1, ii;
    static doublereal lj, lii;
    static integer iim1;


/*  ***  SET A TO LOWER TRIANGLE OF (L**T) * L  *** */

/*  ***  L = N X N LOWER TRIANG. MATRIX STORED ROWWISE.  *** */
/*  ***  A IS ALSO STORED ROWWISE AND MAY SHARE STORAGE WITH L.  *** */

/*     DIMENSION A(N*(N+1)/2), L(N*(N+1)/2) */


    /* Parameter adjustments */
    --l;
    --a;

    /* Function Body */
    ii = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i1 = ii + 1;
	ii += i__;
	m = 1;
	if (i__ == 1) {
	    goto L30;
	}
	iim1 = ii - 1;
	i__2 = iim1;
	for (j = i1; j <= i__2; ++j) {
	    lj = l[j];
	    i__3 = j;
	    for (k = i1; k <= i__3; ++k) {
		a[m] += lj * l[k];
		++m;
/* L10: */
	    }
/* L20: */
	}
L30:
	lii = l[ii];
	i__2 = ii;
	for (j = i1; j <= i__2; ++j) {
/* L40: */
	    a[j] = lii * l[j];
	}
/* L50: */
    }

    return 0;
/*  ***  LAST CARD OF DL7TSQ FOLLOWS  *** */
} /* dl7tsq_ */

doublereal drldst_(integer *p, doublereal *d__, doublereal *x, doublereal *x0)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1, d__2;

    /* Local variables */
    static integer i__;
    static doublereal t, emax, xmax;


/*  ***  COMPUTE AND RETURN RELATIVE DIFFERENCE BETWEEN X AND X0  *** */
/*  ***  NL2SOL VERSION 2.2  *** */



/*  ***  BODY  *** */

    /* Parameter adjustments */
    --x0;
    --x;
    --d__;

    /* Function Body */
    emax = 0.;
    xmax = 0.;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = (d__1 = d__[i__] * (x[i__] - x0[i__]), abs(d__1));
	if (emax < t) {
	    emax = t;
	}
	t = d__[i__] * ((d__1 = x[i__], abs(d__1)) + (d__2 = x0[i__], abs(
		d__2)));
	if (xmax < t) {
	    xmax = t;
	}
/* L10: */
    }
    ret_val = 0.;
    if (xmax > 0.) {
	ret_val = emax / xmax;
    }
    return ret_val;
/*  ***  LAST CARD OF DRLDST FOLLOWS  *** */
} /* drldst_ */

/* Subroutine */ int drn2gb_(doublereal *b, doublereal *d__, doublereal *dr, 
	integer *iv, integer *liv, integer *lv, integer *n, integer *nd, 
	integer *n1, integer *n2, integer *p, doublereal *r__, doublereal *rd,
	 doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer dr_dim1, dr_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer i__, l;
    static doublereal t;
    static integer g1, y1, gi, lh, nn, yi, rd1, iv1, qtr1, rmat1, jtol1;
    extern /* Subroutine */ int dg7itb_(doublereal *, doublereal *, 
	    doublereal *, integer *, integer *, integer *, integer *, integer 
	    *, doublereal *, doublereal *, doublereal *), dq7rad_(integer *, 
	    integer *, integer *, doublereal *, logical *, doublereal *, 
	    doublereal *, doublereal *), dd7upd_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, integer *, integer *, 
	    integer *, integer *, doublereal *), dq7apl_(integer *, integer *,
	     integer *, doublereal *, doublereal *, integer *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dl7vml_(integer *, doublereal *, doublereal *,
	     doublereal *), dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dv7cpy_(integer *, doublereal *, doublereal *)
	    , dr7tvm_(integer *, integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *);
    static integer ivmode;
    extern /* Subroutine */ int divset_(integer *, integer *, integer *, 
	    integer *, doublereal *), ditsum_(doublereal *, doublereal *, 
	    integer *, integer *, integer *, integer *, doublereal *, 
	    doublereal *);


/*  ***  REVISED ITERATION DRIVER FOR NL2SOL WITH SIMPLE BOUNDS  *** */


/* --------------------------  PARAMETER USAGE  -------------------------- */

/* B........ BOUNDS ON X. */
/* D........ SCALE VECTOR. */
/* DR....... DERIVATIVES OF R AT X. */
/* IV....... INTEGER VALUES ARRAY. */
/* LIV...... LENGTH OF IV... LIV MUST BE AT LEAST 4*P + 82. */
/* LV....... LENGTH OF V...  LV  MUST BE AT LEAST 105 + P*(2*P+20). */
/* N........ TOTAL NUMBER OF RESIDUALS. */
/* ND....... MAX. NO. OF RESIDUALS PASSED ON ONE CALL. */
/* N1....... LOWEST  ROW INDEX FOR RESIDUALS SUPPLIED THIS TIME. */
/* N2....... HIGHEST ROW INDEX FOR RESIDUALS SUPPLIED THIS TIME. */
/* P........ NUMBER OF PARAMETERS (COMPONENTS OF X) BEING ESTIMATED. */
/* R........ RESIDUALS. */
/* V........ FLOATING-POINT VALUES ARRAY. */
/* X........ PARAMETER VECTOR BEING ESTIMATED (INPUT = INITIAL GUESS, */
/*             OUTPUT = BEST VALUE FOUND). */

/*  ***  DISCUSSION  *** */

/*     THIS ROUTINE CARRIES OUT ITERATIONS FOR SOLVING NONLINEAR */
/*  LEAST SQUARES PROBLEMS.  IT IS SIMILAR TO  DRN2G, EXCEPT THAT */
/*  THIS ROUTINE ENFORCES THE BOUNDS  B(1,I) .LE. X(I) .LE. B(2,I), */
/*  I = 1(1)P. */

/*  ***  GENERAL  *** */

/*     CODED BY DAVID M. GAY. */

/* +++++++++++++++++++++++++++++  DECLARATIONS  ++++++++++++++++++++++++++ */

/*  ***  EXTERNAL FUNCTIONS AND SUBROUTINES  *** */


/* DIVSET.... PROVIDES DEFAULT IV AND V INPUT COMPONENTS. */
/* DD7TPR... COMPUTES INNER PRODUCT OF TWO VECTORS. */
/* DD7UPD...  UPDATES SCALE VECTOR D. */
/* DG7ITB... PERFORMS BASIC MINIMIZATION ALGORITHM. */
/* DITSUM.... PRINTS ITERATION SUMMARY, INFO ABOUT INITIAL AND FINAL X. */
/* DL7VML.... COMPUTES L * V, V = VECTOR, L = LOWER TRIANGULAR MATRIX. */
/* DQ7APL... APPLIES QR TRANSFORMATIONS STORED BY DQ7RAD. */
/* DQ7RAD.... ADDS A NEW BLOCK OF ROWS TO QR DECOMPOSITION. */
/* DR7TVM... MULT. VECTOR BY TRANS. OF UPPER TRIANG. MATRIX FROM QR FACT. */
/* DV7CPY.... COPIES ONE VECTOR TO ANOTHER. */
/* DV7SCP... SETS ALL ELEMENTS OF A VECTOR TO A SCALAR. */
/* DV2NRM... RETURNS THE 2-NORM OF A VECTOR. */


/*  ***  LOCAL VARIABLES  *** */



/*  ***  SUBSCRIPTS FOR IV AND V  *** */


/*  ***  IV SUBSCRIPT VALUES  *** */


/*  ***  V SUBSCRIPT VALUES  *** */


/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --rd;
    --r__;
    --x;
    dr_dim1 = *nd;
    dr_offset = 1 + dr_dim1;
    dr -= dr_offset;
    --d__;
    b -= 3;

    /* Function Body */
    lh = *p * (*p + 1) / 2;
    if (iv[1] == 0) {
	divset_(&c__1, &iv[1], liv, lv, &v[1]);
    }
    iv1 = iv[1];
    if (iv1 > 2) {
	goto L10;
    }
    nn = *n2 - *n1 + 1;
    iv[9] = 0;
    i__ = iv1 + 4;
    if (iv[2] == 0) {
	switch (i__) {
	    case 1:  goto L150;
	    case 2:  goto L130;
	    case 3:  goto L150;
	    case 4:  goto L120;
	    case 5:  goto L120;
	    case 6:  goto L150;
	}
    }
    if (i__ != 5) {
	iv[1] = 2;
    }
    goto L40;

/*  ***  FRESH START OR RESTART -- CHECK INPUT INTEGERS  *** */

L10:
    if (*nd <= 0) {
	goto L220;
    }
    if (*p <= 0) {
	goto L220;
    }
    if (*n <= 0) {
	goto L220;
    }
    if (iv1 == 14) {
	goto L30;
    }
    if (iv1 > 16) {
	goto L270;
    }
    if (iv1 < 12) {
	goto L40;
    }
    if (iv1 == 12) {
	iv[1] = 13;
    }
    if (iv[1] != 13) {
	goto L20;
    }
    iv[4] += *p * (*p + 15) / 2;
L20:
    dg7itb_(&b[3], &d__[1], &x[1], &iv[1], liv, lv, p, p, &v[1], &x[1], &x[1])
	    ;
    if (iv[1] != 14) {
	goto L999;
    }

/*  ***  STORAGE ALLOCATION  *** */

    iv[28] = iv[47];
    iv[66] = iv[28] + (*p << 1);
    iv[78] = iv[66] + *p;
    iv[77] = iv[78] + lh;
    iv[59] = iv[77] + (*p << 1);
    iv[47] = iv[59] + (*p << 1);
/*  ***  TURN OFF COVARIANCE COMPUTATION  *** */
    iv[57] = 0;
    if (iv1 == 13) {
	goto L999;
    }

L30:
    jtol1 = iv[59];
    if (v[38] >= 0.) {
	dv7scp_(p, &d__[1], &v[38]);
    }
    if (v[39] > 0.) {
	dv7scp_(p, &v[jtol1], &v[39]);
    }
    i__ = jtol1 + *p;
    if (v[40] > 0.) {
	dv7scp_(p, &v[i__], &v[40]);
    }
    iv[68] = 0;
    iv[69] = 0;
    if (*nd >= *n) {
	goto L40;
    }

/*  ***  SPECIAL CASE HANDLING OF FIRST FUNCTION AND GRADIENT EVALUATION */
/*  ***  -- ASK FOR BOTH RESIDUAL AND JACOBIAN AT ONCE */

    g1 = iv[28];
    y1 = g1 + *p;
    dg7itb_(&b[3], &d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[
	    y1]);
    if (iv[1] != 1) {
	goto L260;
    }
    v[10] = 0.;
    dv7scp_(p, &v[g1], &c_b14);
    iv[1] = -1;
    qtr1 = iv[77];
    dv7scp_(p, &v[qtr1], &c_b14);
    iv[67] = 0;
    rmat1 = iv[78];
    goto L100;

L40:
    g1 = iv[28];
    y1 = g1 + *p;
    dg7itb_(&b[3], &d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[
	    y1]);
    if (iv[1] == 2) {
	goto L60;
    }
    if (iv[1] > 2) {
	goto L260;
    }

    v[10] = 0.;
    if (iv[69] == 0) {
	goto L240;
    }
    if (iv[9] != 2) {
	goto L240;
    }
    iv[68] = iv[69];
    dv7cpy_(n, &rd[1], &r__[1]);
    iv[67] = 0;
    goto L240;

L60:
    dv7scp_(p, &v[g1], &c_b14);
    if (iv[35] > 0) {
	goto L230;
    }
    rmat1 = iv[78];
    qtr1 = iv[77];
    rd1 = qtr1 + *p;
    dv7scp_(p, &v[qtr1], &c_b14);
    iv[67] = 0;
    if (*nd < *n) {
	goto L90;
    }
    if (*n1 != 1) {
	goto L90;
    }
    if (iv[35] < 0) {
	goto L100;
    }
    if (iv[69] == iv[7]) {
	goto L70;
    }
    if (iv[68] != iv[7]) {
	goto L90;
    }
    dv7cpy_(n, &r__[1], &rd[1]);
    goto L80;
L70:
    dv7cpy_(n, &rd[1], &r__[1]);
L80:
    dq7apl_(nd, n, p, &dr[dr_offset], &rd[1], &c__0);
    i__1 = min(*n,*p);
    dr7tvm_(nd, &i__1, &v[y1], &v[rd1], &dr[dr_offset], &rd[1]);
    iv[67] = 0;
    goto L110;

L90:
    iv[1] = -2;
    if (iv[35] < 0) {
	iv[1] = -3;
    }
L100:
    dv7scp_(p, &v[y1], &c_b14);
L110:
    dv7scp_(&lh, &v[rmat1], &c_b14);
    goto L240;

/*  ***  COMPUTE F(X)  *** */

L120:
    t = dv2nrm_(&nn, &r__[1]);
    if (t > v[46]) {
	goto L210;
    }
/* Computing 2nd power */
    d__1 = t;
    v[10] += d__1 * d__1 * .5;
    if (*n2 < *n) {
	goto L250;
    }
    if (*n1 == 1) {
	iv[69] = iv[6];
    }
    goto L40;

/*  ***  COMPUTE Y  *** */

L130:
    y1 = iv[28] + *p;
    yi = y1;
    i__1 = *p;
    for (l = 1; l <= i__1; ++l) {
	v[yi] += dd7tpr_(&nn, &dr[l * dr_dim1 + 1], &r__[1]);
	++yi;
/* L140: */
    }
    if (*n2 < *n) {
	goto L250;
    }
    iv[1] = 2;
    if (*n1 > 1) {
	iv[1] = -3;
    }
    goto L240;

/*  ***  COMPUTE GRADIENT INFORMATION  *** */

L150:
    g1 = iv[28];
    ivmode = iv[35];
    if (ivmode < 0) {
	goto L170;
    }
    if (ivmode == 0) {
	goto L180;
    }
    iv[1] = 2;

/*  ***  COMPUTE GRADIENT ONLY (FOR USE IN COVARIANCE COMPUTATION)  *** */

    gi = g1;
    i__1 = *p;
    for (l = 1; l <= i__1; ++l) {
	v[gi] += dd7tpr_(&nn, &r__[1], &dr[l * dr_dim1 + 1]);
	++gi;
/* L160: */
    }
    goto L200;

/*  *** COMPUTE INITIAL FUNCTION VALUE WHEN ND .LT. N *** */

L170:
    if (*n <= *nd) {
	goto L180;
    }
    t = dv2nrm_(&nn, &r__[1]);
    if (t > v[46]) {
	goto L210;
    }
/* Computing 2nd power */
    d__1 = t;
    v[10] += d__1 * d__1 * .5;

/*  ***  UPDATE D IF DESIRED  *** */

L180:
    if (iv[16] > 0) {
	dd7upd_(&d__[1], &dr[dr_offset], &iv[1], liv, lv, n, nd, &nn, n2, p, &
		v[1]);
    }

/*  ***  COMPUTE RMAT AND QTR  *** */

    qtr1 = iv[77];
    rmat1 = iv[78];
    dq7rad_(&nn, nd, p, &v[qtr1], &c_true, &v[rmat1], &dr[dr_offset], &r__[1])
	    ;
    iv[69] = 0;
    if (*n1 > 1) {
	goto L200;
    }
    if (*n2 < *n) {
	goto L250;
    }

/*  ***  SAVE DIAGONAL OF R FOR COMPUTING Y LATER  *** */

    rd1 = qtr1 + *p;
    l = rmat1 - 1;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	l += i__;
	v[rd1] = v[l];
	++rd1;
/* L190: */
    }

L200:
    if (*n2 < *n) {
	goto L250;
    }
    if (ivmode > 0) {
	goto L40;
    }
    iv[81] = iv[7];

/*  ***  COMPUTE G FROM RMAT AND QTR  *** */

    dl7vml_(p, &v[g1], &v[rmat1], &v[qtr1]);
    iv[1] = 2;
    if (ivmode == 0) {
	goto L40;
    }
    if (*n <= *nd) {
	goto L40;
    }

/*  ***  FINISH SPECIAL CASE HANDLING OF FIRST FUNCTION AND GRADIENT */

    y1 = g1 + *p;
    iv[1] = 1;
    dg7itb_(&b[3], &d__[1], &v[g1], &iv[1], liv, lv, p, p, &v[1], &x[1], &v[
	    y1]);
    if (iv[1] != 2) {
	goto L260;
    }
    goto L40;

/*  ***  MISC. DETAILS  *** */

/*     ***  X IS OUT OF RANGE (OVERSIZE STEP)  *** */

L210:
    iv[2] = 1;
    goto L40;

/*     ***  BAD N, ND, OR P  *** */

L220:
    iv[1] = 66;
    goto L270;

/*  ***  RECORD EXTRA EVALUATIONS FOR FINITE-DIFFERENCE HESSIAN  *** */

L230:
    ++iv[52];
    ++iv[6];
    iv[7] = iv[6];
    iv[1] = -1;

/*  ***  RETURN FOR MORE FUNCTION OR GRADIENT INFORMATION  *** */

L240:
    *n2 = 0;
L250:
    *n1 = *n2 + 1;
    *n2 += *nd;
    if (*n2 > *n) {
	*n2 = *n;
    }
    goto L999;

/*  ***  PRINT SUMMARY OF FINAL ITERATION AND OTHER REQUESTED ITEMS  *** */

L260:
    g1 = iv[28];
L270:
    ditsum_(&d__[1], &v[g1], &iv[1], liv, lv, p, &v[1], &x[1]);

L999:
    return 0;
/*  ***  LAST CARD OF DRN2GB FOLLOWS  *** */
} /* drn2gb_ */

/* Subroutine */ int dd7dgb_(doublereal *b, doublereal *d__, doublereal *dig, 
	doublereal *dst, doublereal *g, integer *ipiv, integer *ka, 
	doublereal *l, integer *lv, integer *p, integer *pc, doublereal *
	nwtst, doublereal *step, doublereal *td, doublereal *tg, doublereal *
	v, doublereal *w, doublereal *x0)
{
    /* Initialized data */

    static doublereal meps2 = 0.;

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2, d__3, d__4;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    static integer p1;
    static doublereal t1, t2, ti, xi, x0i, rad;
    static integer p1m1;
    static doublereal nred, pred, gnorm;
    extern /* Subroutine */ int dd7dog_(doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *);
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dv7shf_(integer *, integer *, doublereal *), 
	    dl7ivm_(integer *, doublereal *, doublereal *, doublereal *);
    static doublereal gnorm0;
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int i7shft_(integer *, integer *, integer *), 
	    dl7vml_(integer *, doublereal *, doublereal *, doublereal *), 
	    dv7scp_(integer *, doublereal *, doublereal *);
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dl7itv_(integer *, doublereal *, doublereal *,
	     doublereal *), dq7rsh_(integer *, integer *, logical *, 
	    doublereal *, doublereal *, doublereal *), dv7ipr_(integer *, 
	    integer *, doublereal *), dv7cpy_(integer *, doublereal *, 
	    doublereal *), dl7tvm_(integer *, doublereal *, doublereal *, 
	    doublereal *), dv2axy_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *), dv7vmp_(integer *, doublereal *, 
	    doublereal *, doublereal *, integer *);
    static doublereal ghinvg, dnwtst;


/*  ***  COMPUTE DOUBLE-DOGLEG STEP, SUBJECT TO SIMPLE BOUNDS ON X  *** */


/*     DIMENSION L(P*(P+1)/2) */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  V SUBSCRIPTS  *** */


    /* Parameter adjustments */
    --l;
    --v;
    --x0;
    --w;
    --tg;
    --td;
    --step;
    --nwtst;
    --ipiv;
    --g;
    --dst;
    --dig;
    --d__;
    b -= 3;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    if (meps2 <= 0.) {
	meps2 = 2. * dr7mdc_(&c__3);
    }
    gnorm0 = v[1];
    v[2] = 0.;
    if (*ka < 0) {
	goto L10;
    }
    dnwtst = v[3];
    nred = v[6];
L10:
    pred = 0.;
    v[5] = 0.;
    rad = v[8];
    if (*pc > 0) {
	goto L20;
    }
    dnwtst = 0.;
    dv7scp_(p, &step[1], &c_b14);
    goto L140;

L20:
    p1 = *pc;
    dv7cpy_(p, &td[1], &d__[1]);
    dv7ipr_(p, &ipiv[1], &td[1]);
    dv7scp_(pc, &dst[1], &c_b14);
    dv7cpy_(p, &tg[1], &g[1]);
    dv7ipr_(p, &ipiv[1], &tg[1]);

L30:
    dl7ivm_(&p1, &nwtst[1], &l[1], &tg[1]);
    ghinvg = dd7tpr_(&p1, &nwtst[1], &nwtst[1]);
    v[6] = ghinvg * .5;
    dl7itv_(&p1, &nwtst[1], &l[1], &nwtst[1]);
    dv7vmp_(&p1, &step[1], &nwtst[1], &td[1], &c__1);
    v[3] = dv2nrm_(pc, &step[1]);
    if (*ka >= 0) {
	goto L40;
    }
    *ka = 0;
    dnwtst = v[3];
    nred = v[6];
L40:
    v[8] = rad - v[2];
    if (v[8] <= 0.) {
	goto L100;
    }
    dv7vmp_(&p1, &dig[1], &tg[1], &td[1], &c_n1);
    gnorm = dv2nrm_(&p1, &dig[1]);
    if (gnorm <= 0.) {
	goto L100;
    }
    v[1] = gnorm;
    dv7vmp_(&p1, &dig[1], &dig[1], &td[1], &c_n1);
    dl7tvm_(&p1, &w[1], &l[1], &dig[1]);
    v[44] = dv2nrm_(&p1, &w[1]);
    ++(*ka);
    dd7dog_(&dig[1], lv, &p1, &nwtst[1], &step[1], &v[1]);

/*     ***  FIND T SUCH THAT X - T*STEP IS STILL FEASIBLE. */

    t = 1.;
    k = 0;
    i__1 = p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = ipiv[i__];
	x0i = x0[j] + dst[i__] / td[i__];
	xi = x0i + step[i__];
	if (xi < b[(j << 1) + 1]) {
	    goto L50;
	}
	if (xi <= b[(j << 1) + 2]) {
	    goto L70;
	}
	ti = (b[(j << 1) + 2] - x0i) / step[i__];
	j = i__;
	goto L60;
L50:
	ti = (b[(j << 1) + 1] - x0i) / step[i__];
	j = -i__;
L60:
	if (t <= ti) {
	    goto L70;
	}
	k = j;
	t = ti;
L70:
	;
    }

/*  ***  UPDATE DST, TG, AND PRED  *** */

    dv7vmp_(&p1, &step[1], &step[1], &td[1], &c__1);
    dv2axy_(&p1, &dst[1], &t, &step[1], &dst[1]);
    v[2] = dv2nrm_(pc, &dst[1]);
    t1 = t * v[45];
    t2 = t * v[46];
/* Computing 2nd power */
    d__1 = v[44] * t1;
    pred = pred - t1 * gnorm * ((t2 + 1.) * gnorm) - t2 * (t2 * .5 + 1.) * 
	    ghinvg - d__1 * d__1 * .5;
    if (k == 0) {
	goto L100;
    }
    dl7vml_(&p1, &w[1], &l[1], &w[1]);
    t2 = 1. - t2;
    i__1 = p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L80: */
	tg[i__] = t2 * tg[i__] - t1 * w[i__];
    }

/*     ***  PERMUTE L, ETC. IF NECESSARY  *** */

    p1m1 = p1 - 1;
    j = abs(k);
    if (j == p1) {
	goto L90;
    }
    dq7rsh_(&j, &p1, &c_false, &tg[1], &l[1], &w[1]);
    i7shft_(&p1, &j, &ipiv[1]);
    dv7shf_(&p1, &j, &tg[1]);
    dv7shf_(&p1, &j, &td[1]);
    dv7shf_(&p1, &j, &dst[1]);
L90:
    if (k < 0) {
	ipiv[p1] = -ipiv[p1];
    }
    p1 = p1m1;
    if (p1 > 0) {
	goto L30;
    }

/*     ***  UNSCALE STEP, UPDATE X AND DIHDI  *** */

L100:
    dv7scp_(p, &step[1], &c_b14);
    i__1 = *pc;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = (i__2 = ipiv[i__], abs(i__2));
	step[j] = dst[i__] / td[i__];
/* L110: */
    }

/*  ***  FUDGE STEP TO ENSURE THAT IT FORCES APPROPRIATE COMPONENTS */
/*  ***  TO THEIR BOUNDS  *** */

    if (p1 >= *pc) {
	goto L140;
    }
    dv2axy_(p, &td[1], &c_b85, &step[1], &x0[1]);
    k = p1 + 1;
    i__1 = *pc;
    for (i__ = k; i__ <= i__1; ++i__) {
	j = ipiv[i__];
	t = meps2;
	if (j > 0) {
	    goto L120;
	}
	t = -t;
	j = -j;
	ipiv[i__] = j;
L120:
/* Computing MAX */
	d__3 = (d__1 = td[j], abs(d__1)), d__4 = (d__2 = x0[j], abs(d__2));
	t *= max(d__3,d__4);
	step[j] += t;
/* L130: */
    }

L140:
    v[1] = gnorm0;
    v[6] = nred;
    v[7] = pred;
    v[8] = rad;
    v[3] = dnwtst;
    v[4] = dd7tpr_(p, &step[1], &g[1]);

    return 0;
/*  ***  LAST LINE OF DD7DGB FOLLOWS  *** */
} /* dd7dgb_ */

/* Subroutine */ int dq7rfh_(integer *ierr, integer *ipivot, integer *n, 
	integer *nn, integer *nopivk, integer *p, doublereal *q, doublereal *
	r__, integer *rlen, doublereal *w)
{
    /* Initialized data */

    static doublereal bigrt = 0.;
    static doublereal meps10 = 0.;
    static doublereal tiny = 0.;
    static doublereal tinyrt = 0.;

    /* System generated locals */
    integer q_dim1, q_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k;
    static doublereal s, t, t1, ak;
    static integer ii, kk;
    static doublereal wk;
    static integer km1, nk1, kp1;
    static doublereal big, qkk;
    extern doublereal dr7mdc_(integer *);
    extern /* Subroutine */ int dv7scl_(integer *, doublereal *, doublereal *,
	     doublereal *);
    extern doublereal dd7tpr_(integer *, doublereal *, doublereal *);
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    ;
    extern doublereal dv2nrm_(integer *, doublereal *);
    extern /* Subroutine */ int dv2axy_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *), dv7swp_(integer *, doublereal *, 
	    doublereal *);
    static doublereal singtl;


/*  ***  COMPUTE QR FACTORIZATION VIA HOUSEHOLDER TRANSFORMATIONS */
/*  ***  WITH COLUMN PIVOTING  *** */

/*  ***  PARAMETER DECLARATIONS  *** */

/*     DIMENSION R(P*(P+1)/2) */

/* ----------------------------  DESCRIPTION  ---------------------------- */

/*    THIS ROUTINE COMPUTES A QR FACTORIZATION (VIA HOUSEHOLDER TRANS- */
/* FORMATIONS) OF THE MATRIX  A  THAT ON INPUT IS STORED IN Q. */
/* IF  NOPIVK  ALLOWS IT, THIS ROUTINE DOES COLUMN PIVOTING -- IF */
/* K .GT. NOPIVK,  THEN ORIGINAL COLUMN  K  IS ELIGIBLE FOR PIVOTING. */
/* THE  Q  AND  R  RETURNED ARE SUCH THAT COLUMN  I  OF  Q*R  EQUALS */
/* COLUMN  IPIVOT(I)  OF THE ORIGINAL MATRIX  A.  THE UPPER TRIANGULAR */
/* MATRIX  R  IS STORED COMPACTLY BY COLUMNS, I.E., THE OUTPUT VECTOR  R */
/* CONTAINS  R(1,1), R(1,2), R(2,2), R(1,3), R(2,3), ..., R(P,P) (IN */
/* THAT ORDER).  IF ALL GOES WELL, THEN THIS ROUTINE SETS  IERR = 0. */
/* BUT IF (PERMUTED) COLUMN  K  OF  A  IS LINEARLY DEPENDENT ON */
/* (PERMUTED) COLUMNS 1,2,...,K-1, THEN  IERR  IS SET TO  K AND THE R */
/* MATRIX RETURNED HAS  R(I,J) = 0  FOR  I .GE. K  AND  J .GE. K. */
/*    THE ORIGINAL MATRIX  A  IS AN N BY P MATRIX.  NN  IS THE LEAD */
/* DIMENSION OF THE ARRAY  Q  AND MUST SATISFY  NN .GE. N.  NO */
/* PARAMETER CHECKING IS DONE. */
/*    PIVOTING IS DONE AS THOUGH ALL COLUMNS OF Q WERE FIRST */
/* SCALED TO HAVE THE SAME NORM.  IF COLUMN K IS ELIGIBLE FOR */
/* PIVOTING AND ITS (SCALED) NORM**2 LOSS IS MORE THAN THE */
/* MINIMUM SUCH LOSS (OVER COLUMNS K THRU P), THEN COLUMN K IS */
/* SWAPPED WITH THE COLUMN OF LEAST NORM**2 LOSS. */

/*        CODED BY DAVID M. GAY (FALL 1979, SPRING 1984). */

/* --------------------------  LOCAL VARIABLES  -------------------------- */

/* /+ */
/* / */
    /* Parameter adjustments */
    --w;
    q_dim1 = *nn;
    q_offset = 1 + q_dim1;
    q -= q_offset;
    --ipivot;
    --r__;

    /* Function Body */

/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    *ierr = 0;
    if (meps10 > 0.) {
	goto L10;
    }
    bigrt = dr7mdc_(&c__5);
    meps10 = dr7mdc_(&c__3) * 10.;
    tinyrt = dr7mdc_(&c__2);
    tiny = dr7mdc_(&c__1);
    big = dr7mdc_(&c__6);
    if (tiny * big < 1.) {
	tiny = 1. / big;
    }
L10:
    singtl = (doublereal) max(*n,*p) * meps10;

/*  ***  INITIALIZE W, IPIVOT, AND DIAG(R)  *** */

    j = 0;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ipivot[i__] = i__;
	t = dv2nrm_(n, &q[i__ * q_dim1 + 1]);
	if (t > 0.) {
	    goto L20;
	}
	w[i__] = 1.;
	goto L30;
L20:
	w[i__] = 0.;
L30:
	j += i__;
	r__[j] = t;
/* L40: */
    }

/*  ***  MAIN LOOP  *** */

    kk = 0;
    nk1 = *n + 1;
    i__1 = *p;
    for (k = 1; k <= i__1; ++k) {
	if (nk1 <= 1) {
	    goto L999;
	}
	--nk1;
	kk += k;
	kp1 = k + 1;
	if (k <= *nopivk) {
	    goto L60;
	}
	if (k >= *p) {
	    goto L60;
	}

/*        ***  FIND COLUMN WITH MINIMUM WEIGHT LOSS  *** */

	t = w[k];
	if (t <= 0.) {
	    goto L60;
	}
	j = k;
	i__2 = *p;
	for (i__ = kp1; i__ <= i__2; ++i__) {
	    if (w[i__] >= t) {
		goto L50;
	    }
	    t = w[i__];
	    j = i__;
L50:
	    ;
	}
	if (j == k) {
	    goto L60;
	}

/*             ***  INTERCHANGE COLUMNS K AND J  *** */

	i__ = ipivot[k];
	ipivot[k] = ipivot[j];
	ipivot[j] = i__;
	w[j] = w[k];
	w[k] = t;
	i__ = j * (j + 1) / 2;
	t1 = r__[i__];
	r__[i__] = r__[kk];
	r__[kk] = t1;
	dv7swp_(n, &q[k * q_dim1 + 1], &q[j * q_dim1 + 1]);
	if (k <= 1) {
	    goto L60;
	}
	i__ = i__ - j + 1;
	j = kk - k + 1;
	i__2 = k - 1;
	dv7swp_(&i__2, &r__[i__], &r__[j]);

/*        ***  COLUMN K OF Q SHOULD BE NEARLY ORTHOGONAL TO THE PREVIOUS */
/*        ***  COLUMNS.  NORMALIZE IT, TEST FOR SINGULARITY, AND DECIDE */
/*        ***  WHETHER TO REORTHOGONALIZE IT. */

L60:
	ak = r__[kk];
	if (ak <= 0.) {
	    goto L140;
	}
	wk = w[k];

/*        *** SET T TO THE NORM OF (Q(K,K),...,Q(N,K)) */
/*        *** AND CHECK FOR SINGULARITY. */

	if (wk < .75) {
	    goto L70;
	}
	t = dv2nrm_(&nk1, &q[k + k * q_dim1]);
	if (t / ak <= singtl) {
	    goto L140;
	}
	goto L80;
L70:
	t = sqrt(1. - wk);
	if (t <= singtl) {
	    goto L140;
	}
	t *= ak;

/*        *** DETERMINE HOUSEHOLDER TRANSFORMATION *** */

L80:
	qkk = q[k + k * q_dim1];
	if (t <= tinyrt) {
	    goto L90;
	}
	if (t >= bigrt) {
	    goto L90;
	}
	if (qkk < 0.) {
	    t = -t;
	}
	qkk += t;
	s = sqrt(t * qkk);
	goto L110;
L90:
	s = sqrt(t);
	if (qkk < 0.) {
	    goto L100;
	}
	qkk += t;
	s *= sqrt(qkk);
	goto L110;
L100:
	t = -t;
	qkk += t;
	s *= sqrt(-qkk);
L110:
	q[k + k * q_dim1] = qkk;

/*         ***  SCALE (Q(K,K),...,Q(N,K)) TO HAVE NORM SQRT(2)  *** */

	if (s <= tiny) {
	    goto L140;
	}
	d__1 = 1. / s;
	dv7scl_(&nk1, &q[k + k * q_dim1], &d__1, &q[k + k * q_dim1]);

	r__[kk] = -t;

/*        ***  COMPUTE R(K,I) FOR I = K+1,...,P AND UPDATE Q  *** */

	if (k >= *p) {
	    goto L999;
	}
	j = kk + k;
	ii = kk;
	i__2 = *p;
	for (i__ = kp1; i__ <= i__2; ++i__) {
	    ii += i__;
	    d__1 = -dd7tpr_(&nk1, &q[k + k * q_dim1], &q[k + i__ * q_dim1]);
	    dv2axy_(&nk1, &q[k + i__ * q_dim1], &d__1, &q[k + k * q_dim1], &q[
		    k + i__ * q_dim1]);
	    t = q[k + i__ * q_dim1];
	    r__[j] = t;
	    j += i__;
	    t1 = r__[ii];
	    if (t1 > 0.) {
/* Computing 2nd power */
		d__1 = t / t1;
		w[i__] += d__1 * d__1;
	    }
/* L120: */
	}
/* L130: */
    }

/*  ***  SINGULAR Q  *** */

L140:
    *ierr = k;
    km1 = k - 1;
    j = kk;
    i__1 = *p;
    for (i__ = k; i__ <= i__1; ++i__) {
	i__2 = i__ - km1;
	dv7scp_(&i__2, &r__[j], &c_b14);
	j += i__;
/* L150: */
    }

L999:
    return 0;
/*  ***  LAST CARD OF DQ7RFH FOLLOWS  *** */
} /* dq7rfh_ */

/* Subroutine */ int df7dhb_(doublereal *b, doublereal *d__, doublereal *g, 
	integer *irt, integer *iv, integer *liv, integer *lv, integer *p, 
	doublereal *v, doublereal *x)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static integer i__, k, l, m;
    static doublereal t, xm;
    static integer mm1;
    static doublereal xm1, del;
    static integer hmi, hes, hpi, hpm;
    static doublereal del0;
    static integer stp0, kind, mm1o2, pp1o2, stpi, stpm, newm1, gsave1;
    extern /* Subroutine */ int dv7scp_(integer *, doublereal *, doublereal *)
	    , dv7cpy_(integer *, doublereal *, doublereal *);
    static logical offsid;


/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN, STORE IT IN V STARTING */
/*  ***  AT V(IV(FDH)) = V(-IV(H)).  HONOR SIMPLE BOUNDS IN B. */

/*  ***  IF IV(COVREQ) .GE. 0 THEN DF7DHB USES GRADIENT DIFFERENCES, */
/*  ***  OTHERWISE FUNCTION DIFFERENCES.  STORAGE IN V IS AS IN DG7LIT. */

/* IRT VALUES... */
/*     1 = COMPUTE FUNCTION VALUE, I.E., V(F). */
/*     2 = COMPUTE G. */
/*     3 = DONE. */


/*  ***  PARAMETER DECLARATIONS  *** */


/*  ***  LOCAL VARIABLES  *** */


/*  ***  EXTERNAL SUBROUTINES  *** */


/* DV7CPY.... COPY ONE VECTOR TO ANOTHER. */
/* DV7SCP... COPY SCALAR TO ALL COMPONENTS OF A VECTOR. */

/*  ***  SUBSCRIPTS FOR IV AND V  *** */




/* +++++++++++++++++++++++++++++++  BODY  ++++++++++++++++++++++++++++++++ */

    /* Parameter adjustments */
    --iv;
    --v;
    --x;
    --g;
    --d__;
    b -= 3;

    /* Function Body */
    *irt = 4;
    kind = iv[15];
    m = iv[35];
    if (m > 0) {
	goto L10;
    }
    hes = abs(iv[56]);
    iv[56] = -hes;
    iv[74] = 0;
    iv[33] = -1;
    v[53] = v[10];
/*        *** SUPPLY ZEROS IN CASE B(1,I) = B(2,I) FOR SOME I *** */
    i__1 = *p * (*p + 1) / 2;
    dv7scp_(&i__1, &v[hes], &c_b14);
L10:
    if (m > *p) {
	goto L999;
    }
    if (kind < 0) {
	goto L120;
    }

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN USING BOTH FUNCTION AND */
/*  ***  GRADIENT VALUES. */

    gsave1 = iv[65] + *p;
    if (m > 0) {
	goto L20;
    }
/*        ***  FIRST CALL ON DF7DHB.  SET GSAVE = G, TAKE FIRST STEP  *** */
    dv7cpy_(p, &v[gsave1], &g[1]);
    iv[12] = iv[7];
    goto L80;

L20:
    del = v[52];
    x[m] = v[51];
    if (iv[2] == 0) {
	goto L30;
    }

/*     ***  HANDLE OVERSIZE V(DELTA)  *** */

/* Computing MAX */
    d__2 = 1. / d__[m], d__3 = (d__1 = x[m], abs(d__1));
    del0 = v[44] * max(d__2,d__3);
    del *= .5;
    if ((d__1 = del / del0, abs(d__1)) <= .1) {
	goto L140;
    }

L30:
    hes = -iv[56];

/*  ***  SET  G = (G - GSAVE)/DEL  *** */

    del = 1. / del;
    i__1 = *p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	g[i__] = del * (g[i__] - v[gsave1]);
	++gsave1;
/* L40: */
    }

/*  ***  ADD G AS NEW COL. TO FINITE-DIFF. HESSIAN MATRIX  *** */

    k = hes + m * (m - 1) / 2;
    l = k + m - 2;
    if (m == 1) {
	goto L60;
    }

/*  ***  SET  H(I,M) = 0.5 * (H(I,M) + G(I))  FOR I = 1 TO M-1  *** */

    mm1 = m - 1;
    i__1 = mm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	    v[k] = (v[k] + g[i__]) * .5;
	}
	++k;
/* L50: */
    }

/*  ***  ADD  H(I,M) = G(I)  FOR I = M TO P  *** */

L60:
    ++l;
    i__1 = *p;
    for (i__ = m; i__ <= i__1; ++i__) {
	if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	    v[l] = g[i__];
	}
	l += i__;
/* L70: */
    }

L80:
    ++m;
    iv[35] = m;
    if (m > *p) {
	goto L340;
    }
    if (b[(m << 1) + 1] >= b[(m << 1) + 2]) {
	goto L80;
    }

/*  ***  CHOOSE NEXT FINITE-DIFFERENCE STEP, RETURN TO GET G THERE  *** */

/* Computing MAX */
    d__2 = 1. / d__[m], d__3 = (d__1 = x[m], abs(d__1));
    del = v[44] * max(d__2,d__3);
    xm = x[m];
    if (xm < 0.) {
	goto L90;
    }
    xm1 = xm + del;
    if (xm1 <= b[(m << 1) + 2]) {
	goto L110;
    }
    xm1 = xm - del;
    if (xm1 >= b[(m << 1) + 1]) {
	goto L100;
    }
    goto L280;
L90:
    xm1 = xm - del;
    if (xm1 >= b[(m << 1) + 1]) {
	goto L100;
    }
    xm1 = xm + del;
    if (xm1 <= b[(m << 1) + 2]) {
	goto L110;
    }
    goto L280;

L100:
    del = -del;
L110:
    v[51] = xm;
    x[m] = xm1;
    v[52] = del;
    *irt = 2;
    goto L999;

/*  ***  COMPUTE FINITE-DIFFERENCE HESSIAN USING FUNCTION VALUES ONLY. */

L120:
    stp0 = iv[65] + *p - 1;
    mm1 = m - 1;
    mm1o2 = m * mm1 / 2;
    hes = -iv[56];
    if (m > 0) {
	goto L130;
    }
/*        ***  FIRST CALL ON DF7DHB.  *** */
    iv[63] = 0;
    goto L240;

L130:
    if (iv[2] == 0) {
	goto L150;
    }
/*        ***  PUNT IN THE EVENT OF AN OVERSIZE STEP  *** */
L140:
    iv[74] = -2;
    goto L350;
L150:
    i__ = iv[63];
    if (i__ > 0) {
	goto L190;
    }

/*  ***  SAVE F(X + STP(M)*E(M)) IN H(P,M)  *** */

    pp1o2 = *p * (*p - 1) / 2;
    hpm = hes + pp1o2 + mm1;
    v[hpm] = v[10];

/*  ***  START COMPUTING ROW M OF THE FINITE-DIFFERENCE HESSIAN H.  *** */

    newm1 = 1;
    goto L260;
L160:
    hmi = hes + mm1o2;
    if (mm1 == 0) {
	goto L180;
    }
    hpi = hes + pp1o2;
    i__1 = mm1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	t = 0.;
	if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	    t = v[53] - (v[10] + v[hpi]);
	}
	v[hmi] = t;
	++hmi;
	++hpi;
/* L170: */
    }
L180:
    v[hmi] = v[10] - v[53] * 2.;
    if (offsid) {
	v[hmi] = v[53] - v[10] * 2.;
    }

/*  ***  COMPUTE FUNCTION VALUES NEEDED TO COMPLETE ROW M OF H.  *** */

    i__ = 0;
    goto L200;

L190:
    x[i__] = v[52];

/*  ***  FINISH COMPUTING H(M,I)  *** */

    stpi = stp0 + i__;
    hmi = hes + mm1o2 + i__ - 1;
    stpm = stp0 + m;
    v[hmi] = (v[hmi] + v[10]) / (v[stpi] * v[stpm]);
L200:
    ++i__;
    if (i__ > m) {
	goto L230;
    }
    if (b[(i__ << 1) + 1] < b[(i__ << 1) + 2]) {
	goto L210;
    }
    goto L200;

L210:
    iv[63] = i__;
    stpi = stp0 + i__;
    v[52] = x[i__];
    x[i__] += v[stpi];
    *irt = 1;
    if (i__ < m) {
	goto L999;
    }
    newm1 = 2;
    goto L260;
L220:
    x[m] = v[51] - del;
    if (offsid) {
	x[m] = v[51] + del * 2.;
    }
    goto L999;

L230:
    iv[63] = 0;
    x[m] = v[51];

L240:
    ++m;
    iv[35] = m;
    if (m > *p) {
	goto L330;
    }
    if (b[(m << 1) + 1] < b[(m << 1) + 2]) {
	goto L250;
    }
    goto L240;

/*  ***  PREPARE TO COMPUTE ROW M OF THE FINITE-DIFFERENCE HESSIAN H. */
/*  ***  COMPUTE M-TH STEP SIZE STP(M), THEN RETURN TO OBTAIN */
/*  ***  F(X + STP(M)*E(M)), WHERE E(M) = M-TH STD. UNIT VECTOR. */

L250:
    v[51] = x[m];
    newm1 = 3;
L260:
    xm = v[51];
/* Computing MAX */
    d__1 = 1. / d__[m], d__2 = abs(xm);
    del = v[42] * max(d__1,d__2);
    xm1 = xm + del;
    offsid = FALSE_;
    if (xm1 <= b[(m << 1) + 2]) {
	goto L270;
    }
    offsid = TRUE_;
    xm1 = xm - del;
    if (xm - del * 2. >= b[(m << 1) + 1]) {
	goto L300;
    }
    goto L280;
L270:
    if (xm - del >= b[(m << 1) + 1]) {
	goto L290;
    }
    offsid = TRUE_;
    if (xm + del * 2. <= b[(m << 1) + 2]) {
	goto L310;
    }

L280:
    iv[74] = -2;
    goto L350;

L290:
    if (xm >= 0.) {
	goto L310;
    }
    xm1 = xm - del;
L300:
    del = -del;
L310:
    switch (newm1) {
	case 1:  goto L160;
	case 2:  goto L220;
	case 3:  goto L320;
    }
L320:
    x[m] = xm1;
    stpm = stp0 + m;
    v[stpm] = del;
    *irt = 1;
    goto L999;

/*  ***  HANDLE SPECIAL CASE OF B(1,P) = B(2,P) -- CLEAR SCRATCH VALUES */
/*  ***  FROM LAST ROW OF FDH... */

L330:
    if (b[(*p << 1) + 1] < b[(*p << 1) + 2]) {
	goto L340;
    }
    i__ = hes + *p * (*p - 1) / 2;
    dv7scp_(p, &v[i__], &c_b14);

/*  ***  RESTORE V(F), ETC.  *** */

L340:
    iv[74] = hes;
L350:
    v[10] = v[53];
    *irt = 3;
    if (kind < 0) {
	goto L999;
    }
    iv[7] = iv[12];
    gsave1 = iv[65] + *p;
    dv7cpy_(p, &g[1], &v[gsave1]);
    goto L999;

L999:
    return 0;
/*  ***  LAST LINE OF DF7DHB FOLLOWS  *** */
} /* df7dhb_ */

