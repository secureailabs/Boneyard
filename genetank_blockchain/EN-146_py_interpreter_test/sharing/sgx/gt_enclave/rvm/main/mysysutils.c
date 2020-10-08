/*
 * mysysutils.c
 *
 *  Created on: Mar 23, 2018
 *      Author: shifa
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h> /* for putenv */
#define R_USE_SIGNALS 1
#include <Defn.h>
#include <Internal.h>
#include <R_ext/Riconv.h>
#include <Rinterface.h>
#include <errno.h>


cetype_t getCharCE(SEXP x)
{
    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "getCharCE");
    return CE_NATIVE;
}

/* This may return a R_alloc-ed result, so the caller has to manage the
   R_alloc stack */
const char *translateChar(SEXP x)
{
    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "translateChar");

    const char *ans = CHAR(x);
    return ans;
}

/* This may return a R_alloc-ed result, so the caller has to manage the
   R_alloc stack */
const char *translateCharUTF8(SEXP x)
{
    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "translateCharUTF8");

    const char *ans = CHAR(x);

    return ans;
}

SEXP installTrChar(SEXP x)
{
    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "installTrChar");

    return installChar(x);
}

/* This may return a R_alloc-ed result, so the caller has to manage the
   R_alloc stack */
const char *translateChar0(SEXP x)
{
    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "translateChar0");
    return CHAR(x);
}

void * Riconv_open (const char* tocode, const char* fromcode)
{
	return (void *)-1;
}

size_t Riconv (void *cd, const char **inbuf, size_t *inbytesleft,
	       char **outbuf, size_t *outbytesleft)
{
    /* here libiconv has const char **, glibc has char ** for inbuf */
    return -1;
}

char * R_tmpnam(const char * prefix, const char * tempdir)
{
    return NULL;
}

int Riconv_close (void *cd)
{
    return 0;
}

/* This may return a R_alloc-ed result, so the caller has to manage the
   R_alloc stack */
attribute_hidden /* but not hidden on Windows, where it was used in tcltk.c */
const wchar_t *wtransChar(SEXP x)
{
    const char *ans = CHAR(x);

    if(TYPEOF(x) != CHARSXP)
	error(_("'%s' must be called on a CHARSXP"), "wtransChar");

    return (const wchar_t *)ans;
}

const char *reEnc(const char *x, cetype_t ce_in, cetype_t ce_out, int subst)
{
	return x;
}

size_t ucstoutf8(char *s, const unsigned int wc)
{
	return 0;
}
/* used in plot.c for non-UTF-8 MBCS */
size_t attribute_hidden
mbtoucs(unsigned int *wc, const char *s, size_t n)
{
    return (size_t) 1;
}
