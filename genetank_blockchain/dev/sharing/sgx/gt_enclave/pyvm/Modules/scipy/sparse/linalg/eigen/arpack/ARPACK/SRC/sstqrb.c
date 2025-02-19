/* sstqrb.f -- translated by f2c (version 20160102).
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

/* Table of constant values */

static integer c__0 = 0;
static integer c__1 = 1;
static real c_b31 = 1.f;

/* ----------------------------------------------------------------------- */
/* \BeginDoc */

/* \Name: sstqrb */

/* \Description: */
/*  Computes all eigenvalues and the last component of the eigenvectors */
/*  of a symmetric tridiagonal matrix using the implicit QL or QR method. */

/*  This is mostly a modification of the LAPACK routine ssteqr. */
/*  See Remarks. */

/* \Usage: */
/*  call sstqrb */
/*     ( N, D, E, Z, WORK, INFO ) */

/* \Arguments */
/*  N       Integer.  (INPUT) */
/*          The number of rows and columns in the matrix.  N >= 0. */

/*  D       Real array, dimension (N).  (INPUT/OUTPUT) */
/*          On entry, D contains the diagonal elements of the */
/*          tridiagonal matrix. */
/*          On exit, D contains the eigenvalues, in ascending order. */
/*          If an error exit is made, the eigenvalues are correct */
/*          for indices 1,2,...,INFO-1, but they are unordered and */
/*          may not be the smallest eigenvalues of the matrix. */

/*  E       Real array, dimension (N-1).  (INPUT/OUTPUT) */
/*          On entry, E contains the subdiagonal elements of the */
/*          tridiagonal matrix in positions 1 through N-1. */
/*          On exit, E has been destroyed. */

/*  Z       Real array, dimension (N).  (OUTPUT) */
/*          On exit, Z contains the last row of the orthonormal */
/*          eigenvector matrix of the symmetric tridiagonal matrix. */
/*          If an error exit is made, Z contains the last row of the */
/*          eigenvector matrix associated with the stored eigenvalues. */

/*  WORK    Real array, dimension (max(1,2*N-2)).  (WORKSPACE) */
/*          Workspace used in accumulating the transformation for */
/*          computing the last components of the eigenvectors. */

/*  INFO    Integer.  (OUTPUT) */
/*          = 0:  normal return. */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = +i, the i-th eigenvalue has not converged */
/*                              after a total of  30*N  iterations. */

/* \Remarks */
/*  1. None. */

/* ----------------------------------------------------------------------- */

/* \BeginLib */

/* \Local variables: */
/*     xxxxxx  real */

/* \Routines called: */
/*     saxpy   Level 1 BLAS that computes a vector triad. */
/*     scopy   Level 1 BLAS that copies one vector to another. */
/*     sswap   Level 1 BLAS that swaps the contents of two vectors. */
/*     lsame   LAPACK character comparison routine. */
/*     slae2   LAPACK routine that computes the eigenvalues of a 2-by-2 */
/*             symmetric matrix. */
/*     slaev2  LAPACK routine that eigendecomposition of a 2-by-2 symmetric */
/*             matrix. */
/*     slamch  LAPACK routine that determines machine constants. */
/*     slanst  LAPACK routine that computes the norm of a matrix. */
/*     slapy2  LAPACK routine to compute sqrt(x**2+y**2) carefully. */
/*     slartg  LAPACK Givens rotation construction routine. */
/*     slascl  LAPACK routine for careful scaling of a matrix. */
/*     slaset  LAPACK matrix initialization routine. */
/*     slasr   LAPACK routine that applies an orthogonal transformation to */
/*             a matrix. */
/*     slasrt  LAPACK sorting routine. */
/*     ssteqr  LAPACK routine that computes eigenvalues and eigenvectors */
/*             of a symmetric tridiagonal matrix. */
/*     xerbla  LAPACK error handler routine. */

/* \Authors */
/*     Danny Sorensen               Phuong Vu */
/*     Richard Lehoucq              CRPC / Rice University */
/*     Dept. of Computational &     Houston, Texas */
/*     Applied Mathematics */
/*     Rice University */
/*     Houston, Texas */

/* \SCCS Information: @(#) */
/* FILE: stqrb.F   SID: 2.5   DATE OF SID: 8/27/96   RELEASE: 2 */

/* \Remarks */
/*     1. Starting with version 2.5, this routine is a modified version */
/*        of LAPACK version 2.0 subroutine SSTEQR. No lines are deleted, */
/*        only commeted out and new lines inserted. */
/*        All lines commented out have "c$$$" at the beginning. */
/*        Note that the LAPACK version 1.0 subroutine SSTEQR contained */
/*        bugs. */

/* \EndLib */

/* ----------------------------------------------------------------------- */

/* Subroutine */ int sstqrb_(integer *n, real *d__, real *e, real *z__, real *
	work, integer *info)
{
    /* System generated locals */
    integer i__1, i__2;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);
    float r_sign(real *, real *);

    /* Local variables */
    static real b, c__, f, g;
    static integer i__, j, k, l, m;
    static real p, r__, s;
    static integer l1, ii, mm, lm1, mm1, nm1;
    static real rt1, rt2, eps;
    static integer lsv;
    static real tst, eps2;
    static integer lend, jtot;
    extern /* Subroutine */ int slae2_(real *, real *, real *, real *, real *)
	    ;
    static real anorm;
    extern /* Subroutine */ int slasr_(char *, char *, char *, integer *, 
	    integer *, real *, real *, real *, integer *, ftnlen, ftnlen, 
	    ftnlen);
    static integer lendm1, lendp1;
    extern /* Subroutine */ int slaev2_(real *, real *, real *, real *, real *
	    , real *, real *);
    extern doublereal slapy2_(real *, real *);
    static integer iscale;
    extern doublereal slamch_(char *, ftnlen);
    static real safmin, safmax;
    extern /* Subroutine */ int slascl_(char *, integer *, integer *, real *, 
	    real *, integer *, integer *, real *, integer *, integer *, 
	    ftnlen);
    static integer lendsv;
    extern /* Subroutine */ int slartg_(real *, real *, real *, real *, real *
	    );
    static real ssfmin;
    static integer nmaxit, icompz;
    static real ssfmax;
    extern doublereal slanst_(char *, integer *, real *, real *, ftnlen);
    extern /* Subroutine */ int slasrt_(char *, integer *, real *, integer *, 
	    ftnlen);


/*     %------------------% */
/*     | Scalar Arguments | */
/*     %------------------% */


/*     %-----------------% */
/*     | Array Arguments | */
/*     %-----------------% */


/*     .. parameters .. */
/*     .. */
/*     .. local scalars .. */
/*     .. */
/*     .. external functions .. */
/*     .. */
/*     .. external subroutines .. */
/*     .. */
/*     .. intrinsic functions .. */
/*     .. */
/*     .. executable statements .. */

/*     test the input parameters. */

    /* Parameter adjustments */
    --work;
    --z__;
    --e;
    --d__;

    /* Function Body */
    *info = 0;

/* $$$      IF( LSAME( COMPZ, 'N' ) ) THEN */
/* $$$         ICOMPZ = 0 */
/* $$$      ELSE IF( LSAME( COMPZ, 'V' ) ) THEN */
/* $$$         ICOMPZ = 1 */
/* $$$      ELSE IF( LSAME( COMPZ, 'I' ) ) THEN */
/* $$$         ICOMPZ = 2 */
/* $$$      ELSE */
/* $$$         ICOMPZ = -1 */
/* $$$      END IF */
/* $$$      IF( ICOMPZ.LT.0 ) THEN */
/* $$$         INFO = -1 */
/* $$$      ELSE IF( N.LT.0 ) THEN */
/* $$$         INFO = -2 */
/* $$$      ELSE IF( ( LDZ.LT.1 ) .OR. ( ICOMPZ.GT.0 .AND. LDZ.LT.MAX( 1, */
/* $$$     $         N ) ) ) THEN */
/* $$$         INFO = -6 */
/* $$$      END IF */
/* $$$      IF( INFO.NE.0 ) THEN */
/* $$$         CALL XERBLA( 'SSTEQR', -INFO ) */
/* $$$         RETURN */
/* $$$      END IF */

/*    *** New starting with version 2.5 *** */

    icompz = 2;
/*    ************************************* */

/*     quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	if (icompz == 2) {
	    z__[1] = 1.f;
	}
	return 0;
    }

/*     determine the unit roundoff and over/underflow thresholds. */

    eps = slamch_("e", (ftnlen)1);
/* Computing 2nd power */
    r__1 = eps;
    eps2 = r__1 * r__1;
    safmin = slamch_("s", (ftnlen)1);
    safmax = 1.f / safmin;
    ssfmax = sqrt(safmax) / 3.f;
    ssfmin = sqrt(safmin) / eps2;

/*     compute the eigenvalues and eigenvectors of the tridiagonal */
/*     matrix. */

/* $$      if( icompz.eq.2 ) */
/* $$$     $   call slaset( 'full', n, n, zero, one, z, ldz ) */

/*     *** New starting with version 2.5 *** */

    if (icompz == 2) {
	i__1 = *n - 1;
	for (j = 1; j <= i__1; ++j) {
	    z__[j] = 0.f;
/* L5: */
	}
	z__[*n] = 1.f;
    }
/*     ************************************* */

    nmaxit = *n * 30;
    jtot = 0;

/*     determine where the matrix splits and choose ql or qr iteration */
/*     for each block, according to whether top or bottom diagonal */
/*     element is smaller. */

    l1 = 1;
    nm1 = *n - 1;

L10:
    if (l1 > *n) {
	goto L160;
    }
    if (l1 > 1) {
	e[l1 - 1] = 0.f;
    }
    if (l1 <= nm1) {
	i__1 = nm1;
	for (m = l1; m <= i__1; ++m) {
	    tst = (r__1 = e[m], dabs(r__1));
	    if (tst == 0.f) {
		goto L30;
	    }
	    if (tst <= sqrt((r__1 = d__[m], dabs(r__1))) * sqrt((r__2 = d__[m 
		    + 1], dabs(r__2))) * eps) {
		e[m] = 0.f;
		goto L30;
	    }
/* L20: */
	}
    }
    m = *n;

L30:
    l = l1;
    lsv = l;
    lend = m;
    lendsv = lend;
    l1 = m + 1;
    if (lend == l) {
	goto L10;
    }

/*     scale submatrix in rows and columns l to lend */

    i__1 = lend - l + 1;
    anorm = slanst_("i", &i__1, &d__[l], &e[l], (ftnlen)1);
    iscale = 0;
    if (anorm == 0.f) {
	goto L10;
    }
    if (anorm > ssfmax) {
	iscale = 1;
	i__1 = lend - l + 1;
	slascl_("g", &c__0, &c__0, &anorm, &ssfmax, &i__1, &c__1, &d__[l], n, 
		info, (ftnlen)1);
	i__1 = lend - l;
	slascl_("g", &c__0, &c__0, &anorm, &ssfmax, &i__1, &c__1, &e[l], n, 
		info, (ftnlen)1);
    } else if (anorm < ssfmin) {
	iscale = 2;
	i__1 = lend - l + 1;
	slascl_("g", &c__0, &c__0, &anorm, &ssfmin, &i__1, &c__1, &d__[l], n, 
		info, (ftnlen)1);
	i__1 = lend - l;
	slascl_("g", &c__0, &c__0, &anorm, &ssfmin, &i__1, &c__1, &e[l], n, 
		info, (ftnlen)1);
    }

/*     choose between ql and qr iteration */

    if ((r__1 = d__[lend], dabs(r__1)) < (r__2 = d__[l], dabs(r__2))) {
	lend = lsv;
	l = lendsv;
    }

    if (lend > l) {

/*        ql iteration */

/*        look for small subdiagonal element. */

L40:
	if (l != lend) {
	    lendm1 = lend - 1;
	    i__1 = lendm1;
	    for (m = l; m <= i__1; ++m) {
/* Computing 2nd power */
		r__2 = (r__1 = e[m], dabs(r__1));
		tst = r__2 * r__2;
		if (tst <= eps2 * (r__1 = d__[m], dabs(r__1)) * (r__2 = d__[m 
			+ 1], dabs(r__2)) + safmin) {
		    goto L60;
		}
/* L50: */
	    }
	}

	m = lend;

L60:
	if (m < lend) {
	    e[m] = 0.f;
	}
	p = d__[l];
	if (m == l) {
	    goto L80;
	}

/*        if remaining matrix is 2-by-2, use slae2 or slaev2 */
/*        to compute its eigensystem. */

	if (m == l + 1) {
	    if (icompz > 0) {
		slaev2_(&d__[l], &e[l], &d__[l + 1], &rt1, &rt2, &c__, &s);
		work[l] = c__;
		work[*n - 1 + l] = s;
/* $$$               call slasr( 'r', 'v', 'b', n, 2, work( l ), */
/* $$$     $                     work( n-1+l ), z( 1, l ), ldz ) */

/*              *** New starting with version 2.5 *** */

		tst = z__[l + 1];
		z__[l + 1] = c__ * tst - s * z__[l];
		z__[l] = s * tst + c__ * z__[l];
/*              ************************************* */
	    } else {
		slae2_(&d__[l], &e[l], &d__[l + 1], &rt1, &rt2);
	    }
	    d__[l] = rt1;
	    d__[l + 1] = rt2;
	    e[l] = 0.f;
	    l += 2;
	    if (l <= lend) {
		goto L40;
	    }
	    goto L140;
	}

	if (jtot == nmaxit) {
	    goto L140;
	}
	++jtot;

/*        form shift. */

	g = (d__[l + 1] - p) / (e[l] * 2.f);
	r__ = slapy2_(&g, &c_b31);
	g = d__[m] - p + e[l] / (g + r_sign(&r__, &g));

	s = 1.f;
	c__ = 1.f;
	p = 0.f;

/*        inner loop */

	mm1 = m - 1;
	i__1 = l;
	for (i__ = mm1; i__ >= i__1; --i__) {
	    f = s * e[i__];
	    b = c__ * e[i__];
	    slartg_(&g, &f, &c__, &s, &r__);
	    if (i__ != m - 1) {
		e[i__ + 1] = r__;
	    }
	    g = d__[i__ + 1] - p;
	    r__ = (d__[i__] - g) * s + c__ * 2.f * b;
	    p = s * r__;
	    d__[i__ + 1] = g + p;
	    g = c__ * r__ - b;

/*           if eigenvectors are desired, then save rotations. */

	    if (icompz > 0) {
		work[i__] = c__;
		work[*n - 1 + i__] = -s;
	    }

/* L70: */
	}

/*        if eigenvectors are desired, then apply saved rotations. */

	if (icompz > 0) {
	    mm = m - l + 1;
/* $$$            call slasr( 'r', 'v', 'b', n, mm, work( l ), work( n-1+l ), */
/* $$$     $                  z( 1, l ), ldz ) */

/*             *** New starting with version 2.5 *** */

	    slasr_("r", "v", "b", &c__1, &mm, &work[l], &work[*n - 1 + l], &
		    z__[l], &c__1, (ftnlen)1, (ftnlen)1, (ftnlen)1);
/*             ************************************* */
	}

	d__[l] -= p;
	e[l] = g;
	goto L40;

/*        eigenvalue found. */

L80:
	d__[l] = p;

	++l;
	if (l <= lend) {
	    goto L40;
	}
	goto L140;

    } else {

/*        qr iteration */

/*        look for small superdiagonal element. */

L90:
	if (l != lend) {
	    lendp1 = lend + 1;
	    i__1 = lendp1;
	    for (m = l; m >= i__1; --m) {
/* Computing 2nd power */
		r__2 = (r__1 = e[m - 1], dabs(r__1));
		tst = r__2 * r__2;
		if (tst <= eps2 * (r__1 = d__[m], dabs(r__1)) * (r__2 = d__[m 
			- 1], dabs(r__2)) + safmin) {
		    goto L110;
		}
/* L100: */
	    }
	}

	m = lend;

L110:
	if (m > lend) {
	    e[m - 1] = 0.f;
	}
	p = d__[l];
	if (m == l) {
	    goto L130;
	}

/*        if remaining matrix is 2-by-2, use slae2 or slaev2 */
/*        to compute its eigensystem. */

	if (m == l - 1) {
	    if (icompz > 0) {
		slaev2_(&d__[l - 1], &e[l - 1], &d__[l], &rt1, &rt2, &c__, &s)
			;
/* $$$               work( m ) = c */
/* $$$               work( n-1+m ) = s */
/* $$$               call slasr( 'r', 'v', 'f', n, 2, work( m ), */
/* $$$     $                     work( n-1+m ), z( 1, l-1 ), ldz ) */

/*               *** New starting with version 2.5 *** */

		tst = z__[l];
		z__[l] = c__ * tst - s * z__[l - 1];
		z__[l - 1] = s * tst + c__ * z__[l - 1];
/*               ************************************* */
	    } else {
		slae2_(&d__[l - 1], &e[l - 1], &d__[l], &rt1, &rt2);
	    }
	    d__[l - 1] = rt1;
	    d__[l] = rt2;
	    e[l - 1] = 0.f;
	    l += -2;
	    if (l >= lend) {
		goto L90;
	    }
	    goto L140;
	}

	if (jtot == nmaxit) {
	    goto L140;
	}
	++jtot;

/*        form shift. */

	g = (d__[l - 1] - p) / (e[l - 1] * 2.f);
	r__ = slapy2_(&g, &c_b31);
	g = d__[m] - p + e[l - 1] / (g + r_sign(&r__, &g));

	s = 1.f;
	c__ = 1.f;
	p = 0.f;

/*        inner loop */

	lm1 = l - 1;
	i__1 = lm1;
	for (i__ = m; i__ <= i__1; ++i__) {
	    f = s * e[i__];
	    b = c__ * e[i__];
	    slartg_(&g, &f, &c__, &s, &r__);
	    if (i__ != m) {
		e[i__ - 1] = r__;
	    }
	    g = d__[i__] - p;
	    r__ = (d__[i__ + 1] - g) * s + c__ * 2.f * b;
	    p = s * r__;
	    d__[i__] = g + p;
	    g = c__ * r__ - b;

/*           if eigenvectors are desired, then save rotations. */

	    if (icompz > 0) {
		work[i__] = c__;
		work[*n - 1 + i__] = s;
	    }

/* L120: */
	}

/*        if eigenvectors are desired, then apply saved rotations. */

	if (icompz > 0) {
	    mm = l - m + 1;
/* $$$            call slasr( 'r', 'v', 'f', n, mm, work( m ), work( n-1+m ), */
/* $$$     $                  z( 1, m ), ldz ) */

/*           *** New starting with version 2.5 *** */

	    slasr_("r", "v", "f", &c__1, &mm, &work[m], &work[*n - 1 + m], &
		    z__[m], &c__1, (ftnlen)1, (ftnlen)1, (ftnlen)1);
/*           ************************************* */
	}

	d__[l] -= p;
	e[lm1] = g;
	goto L90;

/*        eigenvalue found. */

L130:
	d__[l] = p;

	--l;
	if (l >= lend) {
	    goto L90;
	}
	goto L140;

    }

/*     undo scaling if necessary */

L140:
    if (iscale == 1) {
	i__1 = lendsv - lsv + 1;
	slascl_("g", &c__0, &c__0, &ssfmax, &anorm, &i__1, &c__1, &d__[lsv], 
		n, info, (ftnlen)1);
	i__1 = lendsv - lsv;
	slascl_("g", &c__0, &c__0, &ssfmax, &anorm, &i__1, &c__1, &e[lsv], n, 
		info, (ftnlen)1);
    } else if (iscale == 2) {
	i__1 = lendsv - lsv + 1;
	slascl_("g", &c__0, &c__0, &ssfmin, &anorm, &i__1, &c__1, &d__[lsv], 
		n, info, (ftnlen)1);
	i__1 = lendsv - lsv;
	slascl_("g", &c__0, &c__0, &ssfmin, &anorm, &i__1, &c__1, &e[lsv], n, 
		info, (ftnlen)1);
    }

/*     check for no convergence to an eigenvalue after a total */
/*     of n*maxit iterations. */

    if (jtot < nmaxit) {
	goto L10;
    }
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (e[i__] != 0.f) {
	    ++(*info);
	}
/* L150: */
    }
    goto L190;

/*     order eigenvalues and eigenvectors. */

L160:
    if (icompz == 0) {

/*        use quick sort */

	slasrt_("i", n, &d__[1], info, (ftnlen)1);

    } else {

/*        use selection sort to minimize swaps of eigenvectors */

	i__1 = *n;
	for (ii = 2; ii <= i__1; ++ii) {
	    i__ = ii - 1;
	    k = i__;
	    p = d__[i__];
	    i__2 = *n;
	    for (j = ii; j <= i__2; ++j) {
		if (d__[j] < p) {
		    k = j;
		    p = d__[j];
		}
/* L170: */
	    }
	    if (k != i__) {
		d__[k] = d__[i__];
		d__[i__] = p;
/* $$$               call sswap( n, z( 1, i ), 1, z( 1, k ), 1 ) */
/*           *** New starting with version 2.5 *** */

		p = z__[k];
		z__[k] = z__[i__];
		z__[i__] = p;
/*           ************************************* */
	    }
/* L180: */
	}
    }

L190:
    return 0;

/*     %---------------% */
/*     | End of sstqrb | */
/*     %---------------% */

} /* sstqrb_ */

