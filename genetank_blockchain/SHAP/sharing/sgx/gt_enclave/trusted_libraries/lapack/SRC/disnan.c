#include "blas.h"
/* disnan.f -- translated by f2c (version 20160102).
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

/* > \brief \b DISNAN tests input for NaN. */

/*  =========== DOCUMENTATION =========== */

/* Online html documentation available at */
/*            http://www.netlib.org/lapack/explore-html/ */

/* > \htmlonly */
/* > Download DISNAN + dependencies */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.tgz?format=tgz&filename=/lapack/lapack_routine/disnan.
f"> */
/* > [TGZ]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.zip?format=zip&filename=/lapack/lapack_routine/disnan.
f"> */
/* > [ZIP]</a> */
/* > <a href="http://www.netlib.org/cgi-bin/netlibfiles.txt?format=txt&filename=/lapack/lapack_routine/disnan.
f"> */
/* > [TXT]</a> */
/* > \endhtmlonly */

/*  Definition: */
/*  =========== */

/*       LOGICAL FUNCTION DISNAN( DIN ) */

/*       .. Scalar Arguments .. */
/*       DOUBLE PRECISION, INTENT(IN) :: DIN */
/*       .. */


/* > \par Purpose: */
/*  ============= */
/* > */
/* > \verbatim */
/* > */
/* > DISNAN returns .TRUE. if its argument is NaN, and .FALSE. */
/* > otherwise.  To be replaced by the Fortran 2003 intrinsic in the */
/* > future. */
/* > \endverbatim */

/*  Arguments: */
/*  ========== */

/* > \param[in] DIN */
/* > \verbatim */
/* >          DIN is DOUBLE PRECISION */
/* >          Input to test for NaN. */
/* > \endverbatim */

/*  Authors: */
/*  ======== */

/* > \author Univ. of Tennessee */
/* > \author Univ. of California Berkeley */
/* > \author Univ. of Colorado Denver */
/* > \author NAG Ltd. */

/* > \date June 2017 */

/* > \ingroup OTHERauxiliary */

/*  ===================================================================== */
logical disnan_(doublereal *din)
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    extern logical dlaisnan_(doublereal *, doublereal *);


/*  -- LAPACK auxiliary routine (version 3.7.1) -- */
/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */
/*     June 2017 */

/*     .. Scalar Arguments .. */
/*     .. */

/*  ===================================================================== */

/*  .. External Functions .. */
/*  .. */
/*  .. Executable Statements .. */
    ret_val = dlaisnan_(din, din);
    return ret_val;
} /* disnan_ */
