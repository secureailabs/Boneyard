/* stftri.f -- translated by f2c (version 20061008).
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
#include "blaswrap.h"
#include "blas.h"
#include "clapack.h"

/* Table of constant values */

static real c_b13 = -1.f;
static real c_b18 = 1.f;

/* Subroutine */ int stftri_(char *transr, char *uplo, char *diag, integer *n, 
	 real *a, integer *info)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    integer k, n1, n2;
    logical normaltransr;
    extern logical lsame_(char *, char *);
    logical lower;
    extern /* Subroutine */ int strmm_(char *, char *, char *, char *, 
	    integer *, integer *, real *, real *, integer *, real *, integer *
), xerbla_(char *, integer *);
    logical nisodd;
    extern /* Subroutine */ int strtri_(char *, char *, integer *, real *, 
	    integer *, integer *);


/*  -- LAPACK routine (version 3.2)                                    -- */

/*  -- Contributed by Fred Gustavson of the IBM Watson Research Center -- */
/*  -- November 2008                                                   -- */

/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  STFTRI computes the inverse of a triangular matrix A stored in RFP */
/*  format. */

/*  This is a Level 3 BLAS version of the algorithm. */

/*  Arguments */
/*  ========= */

/*  TRANSR    (input) CHARACTER */
/*          = 'N':  The Normal TRANSR of RFP A is stored; */
/*          = 'T':  The Transpose TRANSR of RFP A is stored. */

/*  UPLO    (input) CHARACTER */
/*          = 'U':  A is upper triangular; */
/*          = 'L':  A is lower triangular. */

/*  DIAG    (input) CHARACTER */
/*          = 'N':  A is non-unit triangular; */
/*          = 'U':  A is unit triangular. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input/output) REAL array, dimension (NT); */
/*          NT=N*(N+1)/2. On entry, the triangular factor of a Hermitian */
/*          Positive Definite matrix A in RFP format. RFP format is */
/*          described by TRANSR, UPLO, and N as follows: If TRANSR = 'N' */
/*          then RFP A is (0:N,0:k-1) when N is even; k=N/2. RFP A is */
/*          (0:N-1,0:k) when N is odd; k=N/2. IF TRANSR = 'T' then RFP is */
/*          the transpose of RFP A as defined when */
/*          TRANSR = 'N'. The contents of RFP A are defined by UPLO as */
/*          follows: If UPLO = 'U' the RFP A contains the nt elements of */
/*          upper packed A; If UPLO = 'L' the RFP A contains the nt */
/*          elements of lower packed A. The LDA of RFP A is (N+1)/2 when */
/*          TRANSR = 'T'. When TRANSR is 'N' the LDA is N+1 when N is */
/*          even and N is odd. See the Note below for more details. */

/*          On exit, the (triangular) inverse of the original matrix, in */
/*          the same storage format. */

/*  INFO    (output) INTEGER */
/*          = 0: successful exit */
/*          < 0: if INFO = -i, the i-th argument had an illegal value */
/*          > 0: if INFO = i, A(i,i) is exactly zero.  The triangular */
/*               matrix is singular and its inverse can not be computed. */

/*  Notes */
/*  ===== */

/*  We first consider Rectangular Full Packed (RFP) Format when N is */
/*  even. We give an example where N = 6. */

/*      AP is Upper             AP is Lower */

/*   00 01 02 03 04 05       00 */
/*      11 12 13 14 15       10 11 */
/*         22 23 24 25       20 21 22 */
/*            33 34 35       30 31 32 33 */
/*               44 45       40 41 42 43 44 */
/*                  55       50 51 52 53 54 55 */


/*  Let TRANSR = 'N'. RFP holds AP as follows: */
/*  For UPLO = 'U' the upper trapezoid A(0:5,0:2) consists of the last */
/*  three columns of AP upper. The lower triangle A(4:6,0:2) consists of */
/*  the transpose of the first three columns of AP upper. */
/*  For UPLO = 'L' the lower trapezoid A(1:6,0:2) consists of the first */
/*  three columns of AP lower. The upper triangle A(0:2,0:2) consists of */
/*  the transpose of the last three columns of AP lower. */
/*  This covers the case N even and TRANSR = 'N'. */

/*         RFP A                   RFP A */

/*        03 04 05                33 43 53 */
/*        13 14 15                00 44 54 */
/*        23 24 25                10 11 55 */
/*        33 34 35                20 21 22 */
/*        00 44 45                30 31 32 */
/*        01 11 55                40 41 42 */
/*        02 12 22                50 51 52 */

/*  Now let TRANSR = 'T'. RFP A in both UPLO cases is just the */
/*  transpose of RFP A above. One therefore gets: */


/*           RFP A                   RFP A */

/*     03 13 23 33 00 01 02    33 00 10 20 30 40 50 */
/*     04 14 24 34 44 11 12    43 44 11 21 31 41 51 */
/*     05 15 25 35 45 55 22    53 54 55 22 32 42 52 */


/*  We first consider Rectangular Full Packed (RFP) Format when N is */
/*  odd. We give an example where N = 5. */

/*     AP is Upper                 AP is Lower */

/*   00 01 02 03 04              00 */
/*      11 12 13 14              10 11 */
/*         22 23 24              20 21 22 */
/*            33 34              30 31 32 33 */
/*               44              40 41 42 43 44 */


/*  Let TRANSR = 'N'. RFP holds AP as follows: */
/*  For UPLO = 'U' the upper trapezoid A(0:4,0:2) consists of the last */
/*  three columns of AP upper. The lower triangle A(3:4,0:1) consists of */
/*  the transpose of the first two columns of AP upper. */
/*  For UPLO = 'L' the lower trapezoid A(0:4,0:2) consists of the first */
/*  three columns of AP lower. The upper triangle A(0:1,1:2) consists of */
/*  the transpose of the last two columns of AP lower. */
/*  This covers the case N odd and TRANSR = 'N'. */

/*         RFP A                   RFP A */

/*        02 03 04                00 33 43 */
/*        12 13 14                10 11 44 */
/*        22 23 24                20 21 22 */
/*        00 33 34                30 31 32 */
/*        01 11 44                40 41 42 */

/*  Now let TRANSR = 'T'. RFP A in both UPLO cases is just the */
/*  transpose of RFP A above. One therefore gets: */

/*           RFP A                   RFP A */

/*     02 12 22 00 01             00 10 20 30 40 50 */
/*     03 13 23 33 11             33 11 21 31 41 51 */
/*     04 14 24 34 44             43 44 22 32 42 52 */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    *info = 0;
    normaltransr = lsame_(transr, "N");
    lower = lsame_(uplo, "L");
    if (! normaltransr && ! lsame_(transr, "T")) {
	*info = -1;
    } else if (! lower && ! lsame_(uplo, "U")) {
	*info = -2;
    } else if (! lsame_(diag, "N") && ! lsame_(diag, 
	    "U")) {
	*info = -3;
    } else if (*n < 0) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("STFTRI", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     If N is odd, set NISODD = .TRUE. */
/*     If N is even, set K = N/2 and NISODD = .FALSE. */

    if (*n % 2 == 0) {
	k = *n / 2;
	nisodd = FALSE_;
    } else {
	nisodd = TRUE_;
    }

/*     Set N1 and N2 depending on LOWER */

    if (lower) {
	n2 = *n / 2;
	n1 = *n - n2;
    } else {
	n1 = *n / 2;
	n2 = *n - n1;
    }


/*     start execution: there are eight cases */

    if (nisodd) {

/*        N is odd */

	if (normaltransr) {

/*           N is odd and TRANSR = 'N' */

	    if (lower) {

/*             SRPA for LOWER, NORMAL and N is odd ( a(0:n-1,0:n1-1) ) */
/*             T1 -> a(0,0), T2 -> a(0,1), S -> a(n1,0) */
/*             T1 -> a(0), T2 -> a(n), S -> a(n1) */

		strtri_("L", diag, &n1, a, n, info);
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "L", "N", diag, &n2, &n1, &c_b13, a, n, &a[n1], n);
		strtri_("U", diag, &n2, &a[*n], n, info)
			;
		if (*info > 0) {
		    *info += n1;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "U", "T", diag, &n2, &n1, &c_b18, &a[*n], n, &a[
			n1], n);

	    } else {

/*             SRPA for UPPER, NORMAL and N is odd ( a(0:n-1,0:n2-1) */
/*             T1 -> a(n1+1,0), T2 -> a(n1,0), S -> a(0,0) */
/*             T1 -> a(n2), T2 -> a(n1), S -> a(0) */

		strtri_("L", diag, &n1, &a[n2], n, info)
			;
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "L", "T", diag, &n1, &n2, &c_b13, &a[n2], n, a, n);
		strtri_("U", diag, &n2, &a[n1], n, info)
			;
		if (*info > 0) {
		    *info += n1;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "U", "N", diag, &n1, &n2, &c_b18, &a[n1], n, a, n);

	    }

	} else {

/*           N is odd and TRANSR = 'T' */

	    if (lower) {

/*              SRPA for LOWER, TRANSPOSE and N is odd */
/*              T1 -> a(0), T2 -> a(1), S -> a(0+n1*n1) */

		strtri_("U", diag, &n1, a, &n1, info);
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "U", "N", diag, &n1, &n2, &c_b13, a, &n1, &a[n1 * 
			n1], &n1);
		strtri_("L", diag, &n2, &a[1], &n1, info);
		if (*info > 0) {
		    *info += n1;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "L", "T", diag, &n1, &n2, &c_b18, &a[1], &n1, &a[
			n1 * n1], &n1);

	    } else {

/*              SRPA for UPPER, TRANSPOSE and N is odd */
/*              T1 -> a(0+n2*n2), T2 -> a(0+n1*n2), S -> a(0) */

		strtri_("U", diag, &n1, &a[n2 * n2], &n2, info);
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "U", "T", diag, &n2, &n1, &c_b13, &a[n2 * n2], &
			n2, a, &n2);
		strtri_("L", diag, &n2, &a[n1 * n2], &n2, info);
		if (*info > 0) {
		    *info += n1;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "L", "N", diag, &n2, &n1, &c_b18, &a[n1 * n2], &
			n2, a, &n2);
	    }

	}

    } else {

/*        N is even */

	if (normaltransr) {

/*           N is even and TRANSR = 'N' */

	    if (lower) {

/*              SRPA for LOWER, NORMAL, and N is even ( a(0:n,0:k-1) ) */
/*              T1 -> a(1,0), T2 -> a(0,0), S -> a(k+1,0) */
/*              T1 -> a(1), T2 -> a(0), S -> a(k+1) */

		i__1 = *n + 1;
		strtri_("L", diag, &k, &a[1], &i__1, info);
		if (*info > 0) {
		    return 0;
		}
		i__1 = *n + 1;
		i__2 = *n + 1;
		strmm_("R", "L", "N", diag, &k, &k, &c_b13, &a[1], &i__1, &a[
			k + 1], &i__2);
		i__1 = *n + 1;
		strtri_("U", diag, &k, a, &i__1, info);
		if (*info > 0) {
		    *info += k;
		}
		if (*info > 0) {
		    return 0;
		}
		i__1 = *n + 1;
		i__2 = *n + 1;
		strmm_("L", "U", "T", diag, &k, &k, &c_b18, a, &i__1, &a[k + 
			1], &i__2)
			;

	    } else {

/*              SRPA for UPPER, NORMAL, and N is even ( a(0:n,0:k-1) ) */
/*              T1 -> a(k+1,0) ,  T2 -> a(k,0),   S -> a(0,0) */
/*              T1 -> a(k+1), T2 -> a(k), S -> a(0) */

		i__1 = *n + 1;
		strtri_("L", diag, &k, &a[k + 1], &i__1, info);
		if (*info > 0) {
		    return 0;
		}
		i__1 = *n + 1;
		i__2 = *n + 1;
		strmm_("L", "L", "T", diag, &k, &k, &c_b13, &a[k + 1], &i__1, 
			a, &i__2);
		i__1 = *n + 1;
		strtri_("U", diag, &k, &a[k], &i__1, info);
		if (*info > 0) {
		    *info += k;
		}
		if (*info > 0) {
		    return 0;
		}
		i__1 = *n + 1;
		i__2 = *n + 1;
		strmm_("R", "U", "N", diag, &k, &k, &c_b18, &a[k], &i__1, a, &
			i__2);
	    }
	} else {

/*           N is even and TRANSR = 'T' */

	    if (lower) {

/*              SRPA for LOWER, TRANSPOSE and N is even (see paper) */
/*              T1 -> B(0,1), T2 -> B(0,0), S -> B(0,k+1) */
/*              T1 -> a(0+k), T2 -> a(0+0), S -> a(0+k*(k+1)); lda=k */

		strtri_("U", diag, &k, &a[k], &k, info);
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "U", "N", diag, &k, &k, &c_b13, &a[k], &k, &a[k * 
			(k + 1)], &k);
		strtri_("L", diag, &k, a, &k, info);
		if (*info > 0) {
		    *info += k;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "L", "T", diag, &k, &k, &c_b18, a, &k, &a[k * (k 
			+ 1)], &k)
			;
	    } else {

/*              SRPA for UPPER, TRANSPOSE and N is even (see paper) */
/*              T1 -> B(0,k+1),     T2 -> B(0,k),   S -> B(0,0) */
/*              T1 -> a(0+k*(k+1)), T2 -> a(0+k*k), S -> a(0+0)); lda=k */

		strtri_("U", diag, &k, &a[k * (k + 1)], &k, info);
		if (*info > 0) {
		    return 0;
		}
		strmm_("R", "U", "T", diag, &k, &k, &c_b13, &a[k * (k + 1)], &
			k, a, &k);
		strtri_("L", diag, &k, &a[k * k], &k, info);
		if (*info > 0) {
		    *info += k;
		}
		if (*info > 0) {
		    return 0;
		}
		strmm_("L", "L", "N", diag, &k, &k, &c_b18, &a[k * k], &k, a, 
			&k);
	    }
	}
    }

    return 0;

/*     End of STFTRI */

} /* stftri_ */
