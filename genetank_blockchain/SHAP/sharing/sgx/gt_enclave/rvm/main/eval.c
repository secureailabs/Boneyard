/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1998--2017	The R Core Team.
 *  Copyright (C) 1995, 1996	Robert Gentleman and Ross Ihaka
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  https://www.R-project.org/Licenses/
 */


#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#define R_USE_SIGNALS 1
#include <Defn.h>
#include <Internal.h>
#include <Rinterface.h>
//--by GT--#include <Fileio.h>
#include <R_ext/Print.h>


static SEXP bcEval(SEXP s, SEXP e, Rboolean b){};

/* BC_PROILFING needs to be enabled at build time. It is not enabled
   by default as enabling it disabled the more efficient threaded code
   implementation of the byte code interpreter. */
#ifdef BC_PROFILING
static Rboolean bc_profiling = FALSE;
#endif

static int R_Profiling = 0;

SEXP do_Rprof(SEXP args)
{
    error(_("R profiling is not available on this system"));
    return R_NilValue;		/* -Wall */
}

/* NEEDED: A fixup is needed in browser, because it can trap errors,
 *	and currently does not reset the limit to the right value. */

void attribute_hidden check_stack_balance(SEXP op, int save)
{
    if(save == R_PPStackTop) return;
    REprintf("Warning: stack imbalance in '%s', %d then %d\n",
	     PRIMNAME(op), save, R_PPStackTop);
}


static SEXP forcePromise(SEXP e)
{
    if (PRVALUE(e) == R_UnboundValue) {
	RPRSTACK prstack;
	SEXP val;
	if(PRSEEN(e)) {
	    if (PRSEEN(e) == 1)
		errorcall(R_GlobalContext->call,
			  _("promise already under evaluation: recursive default argument reference or earlier problems?"));
	    else {
		/* set PRSEEN to 1 to avoid infinite recursion */
		SET_PRSEEN(e, 1);
		warningcall(R_GlobalContext->call,
			     _("restarting interrupted promise evaluation"));
	    }
	}
	/* Mark the promise as under evaluation and push it on a stack
	   that can be used to unmark pending promises if a jump out
	   of the evaluation occurs. */
	SET_PRSEEN(e, 1);
	prstack.promise = e;
	prstack.next = R_PendingPromises;
	R_PendingPromises = &prstack;

	val = eval(PRCODE(e), PRENV(e));

	/* Pop the stack, unmark the promise and set its value field.
	   Also set the environment to R_NilValue to allow GC to
	   reclaim the promise environment; this is also useful for
	   fancy games with delayedAssign() */
	R_PendingPromises = prstack.next;
	SET_PRSEEN(e, 0);
	SET_PRVALUE(e, val);
	SET_NAMED (val, 2);
	SET_PRENV(e, R_NilValue);
    }
    return PRVALUE(e);
}

/* Return value of "e" evaluated in "rho". */

/* some places, e.g. deparse2buff, call this with a promise and rho = NULL */
SEXP eval(SEXP e, SEXP rho)
{
    SEXP op, tmp;
    static int evalcount = 0;

    R_Visible = TRUE;

    /* this is needed even for self-evaluating objects or something like
       'while (TRUE) NULL' will not be interruptable */
    if (++evalcount > 1000) { /* was 100 before 2.8.0 */
	R_CheckUserInterrupt();
#ifndef IMMEDIATE_FINALIZERS
	/* finalizers are run here since this should only be called at
	   points where running arbitrary code should be safe */
	R_RunPendingFinalizers();
#endif
	evalcount = 0 ;
    }

    /* handle self-evaluating objects with minimal overhead */
    switch (TYPEOF(e)) {
    case NILSXP:
    case LISTSXP:
    case LGLSXP:
    case INTSXP:
    case REALSXP:
    case STRSXP:
    case CPLXSXP:
    case RAWSXP:
    case S4SXP:
    case SPECIALSXP:
    case BUILTINSXP:
    case ENVSXP:
    case CLOSXP:
    case VECSXP:
    case EXTPTRSXP:
    case WEAKREFSXP:
    case EXPRSXP:
	/* Make sure constants in expressions are NAMED before being
	   used as values.  Setting NAMED to 2 makes sure weird calls
	   to replacement functions won't modify constants in
	   expressions.  */
	if (NAMED(e) <= 1) SET_NAMED(e, 2);
	return e;
    default: break;
    }

    int bcintactivesave = R_BCIntActive;
    R_BCIntActive = 0;

    if (!rho)
	error("'rho' cannot be C NULL: detected in C-level eval");
    if (!isEnvironment(rho))
	error("'rho' must be an environment not %s: detected in C-level eval",
	      type2char(TYPEOF(rho)));

    /* Save the current srcref context. */

    SEXP srcrefsave = R_Srcref;

    /* The use of depthsave below is necessary because of the
       possibility of non-local returns from evaluation.  Without this
       an "expression too complex error" is quite likely. */

    int depthsave = R_EvalDepth++;

    /* We need to explicit set a NULL call here to circumvent attempts
       to deparse the call in the error-handler */
    if (R_EvalDepth > R_Expressions) {
	R_Expressions = R_Expressions_keep + 500;
	errorcall(R_NilValue,
		  _("evaluation nested too deeply: infinite recursion / options(expressions=)?"));
    }
    R_CheckStack();

    tmp = R_NilValue;		/* -Wall */
#ifdef Win32
    /* This is an inlined version of Rwin_fpreset (src/gnuwin/extra.c)
       and resets the precision, rounding and exception modes of a ix86
       fpu.
     */
    __asm__ ( "fninit" );
#endif

    switch (TYPEOF(e)) {
    case BCODESXP:
	//tmp = bcEval(e, rho, TRUE);
	    break;
    case SYMSXP:
	if (e == R_DotsSymbol)
	    error(_("'...' used in an incorrect context"));
	if( DDVAL(e) )
		tmp = ddfindVar(e,rho);
	else
		tmp = findVar(e, rho);
	if (tmp == R_UnboundValue)
	    error(_("object '%s' not found"), EncodeChar(PRINTNAME(e)));
	/* if ..d is missing then ddfindVar will signal */
	else if (tmp == R_MissingArg && !DDVAL(e) ) {
	    const char *n = CHAR(PRINTNAME(e));
	    if(*n) error(_("argument \"%s\" is missing, with no default"),
			 CHAR(PRINTNAME(e)));
	    else error(_("argument is missing, with no default"));
	}
	else if (TYPEOF(tmp) == PROMSXP) {
	    if (PRVALUE(tmp) == R_UnboundValue) {
		/* not sure the PROTECT is needed here but keep it to
		   be on the safe side. */
		PROTECT(tmp);
		tmp = forcePromise(tmp);
		UNPROTECT(1);
	    }
	    else tmp = PRVALUE(tmp);
	    SET_NAMED(tmp, 2);
	}
	else if (!isNull(tmp) && NAMED(tmp) == 0)
	    SET_NAMED(tmp, 1);
	break;
    case PROMSXP:
	if (PRVALUE(e) == R_UnboundValue)
	    /* We could just unconditionally use the return value from
	       forcePromise; the test avoids the function call if the
	       promise is already evaluated. */
	    forcePromise(e);
	tmp = PRVALUE(e);
	/* This does _not_ change the value of NAMED on the value tmp,
	   in contrast to the handling of promises bound to symbols in
	   the SYMSXP case above.  The reason is that one (typically
	   the only) place promises appear in source code is as
	   wrappers for the RHS value in replacement function calls for
	   complex assignment expression created in applydefine().  If
	   the RHS value is freshly created it will have NAMED = 0 and
	   we want it to stay that way or a BUILTIN or SPECIAL
	   replacement function might have to duplicate the value
	   before inserting it to avoid creating cycles.  (Closure
	   replacement functions will get the value via the SYMSXP case
	   from evaluating their 'value' argument so the value will
	   end up getting duplicated if NAMED = 2.) LT */
	break;
    case LANGSXP:
	if (TYPEOF(CAR(e)) == SYMSXP) {
	    /* This will throw an error if the function is not found */
	    SEXP ecall = e;

	    /* This picks the correct/better error expression for
	       replacement calls running in the AST interpreter. */
	    if (R_GlobalContext != NULL &&
		    (R_GlobalContext->callflag == CTXT_CCODE))
		ecall = R_GlobalContext->call;
	    PROTECT(op = findFun3(CAR(e), rho, ecall));
	} else
	    PROTECT(op = eval(CAR(e), rho));

	if(RTRACE(op) && R_current_trace_state()) {
	    Rprintf("trace: ");
	    PrintValue(e);
	}
	if (TYPEOF(op) == SPECIALSXP) {
	    int save = R_PPStackTop, flag = PRIMPRINT(op);
	    const void *vmax = vmaxget();
	    PROTECT(e);
	    R_Visible = flag != 1;
	    tmp = PRIMFUN(op) (e, op, CDR(e), rho);
#ifdef CHECK_VISIBILITY
	    if(flag < 2 && R_Visible == flag) {
		char *nm = PRIMNAME(op);
		if(strcmp(nm, "for")
		   && strcmp(nm, "repeat") && strcmp(nm, "while")
		   && strcmp(nm, "[[<-") && strcmp(nm, "on.exit"))
		    printf("vis: special %s\n", nm);
	    }
#endif
	    if (flag < 2) R_Visible = flag != 1;
	    UNPROTECT(1);
	    check_stack_balance(op, save);
	    vmaxset(vmax);
	}
	else if (TYPEOF(op) == BUILTINSXP) {
	    int save = R_PPStackTop, flag = PRIMPRINT(op);
	    const void *vmax = vmaxget();
	    RCNTXT cntxt;
	    PROTECT(tmp = evalList(CDR(e), rho, e, 0));
	    if (flag < 2) R_Visible = flag != 1;
	    /* We used to insert a context only if profiling,
	       but helps for tracebacks on .C etc. */
	    if (R_Profiling || (PPINFO(op).kind == PP_FOREIGN)) {
		SEXP oldref = R_Srcref;
		begincontext(&cntxt, CTXT_BUILTIN, e,
			     R_BaseEnv, R_BaseEnv, R_NilValue, R_NilValue);
		R_Srcref = NULL;
		tmp = PRIMFUN(op) (e, op, tmp, rho);
		R_Srcref = oldref;
		endcontext(&cntxt);
	    } else {
		tmp = PRIMFUN(op) (e, op, tmp, rho);
	    }
#ifdef CHECK_VISIBILITY
	    if(flag < 2 && R_Visible == flag) {
		char *nm = PRIMNAME(op);
		printf("vis: builtin %s\n", nm);
	    }
#endif
	    if (flag < 2) R_Visible = flag != 1;
	    UNPROTECT(1);
	    check_stack_balance(op, save);
	    vmaxset(vmax);
	}
	else if (TYPEOF(op) == CLOSXP) {
	    PROTECT(tmp = promiseArgs(CDR(e), rho));
	    tmp = applyClosure(e, op, tmp, rho, R_NilValue);
	    UNPROTECT(1);
	}
	else
	    error(_("attempt to apply non-function"));
	UNPROTECT(1);
	break;
    case DOTSXP:
	error(_("'...' used in an incorrect context"));
    default:
	UNIMPLEMENTED_TYPE("eval", e);
    }
    R_EvalDepth = depthsave;
    R_Srcref = srcrefsave;
    R_BCIntActive = bcintactivesave;
    return (tmp);
}

attribute_hidden
void SrcrefPrompt(const char * prefix, SEXP srcref)
{
    /* If we have a valid srcref, use it */
    if (srcref && srcref != R_NilValue) {
	if (TYPEOF(srcref) == VECSXP) srcref = VECTOR_ELT(srcref, 0);
	SEXP srcfile = getAttrib(srcref, R_SrcfileSymbol);
	if (TYPEOF(srcfile) == ENVSXP) {
	    SEXP filename = findVar(install("filename"), srcfile);
	    if (isString(filename) && length(filename)) {
		Rprintf(_("%s at %s#%d: "), prefix,
			CHAR(STRING_ELT(filename, 0)),
			asInteger(srcref));
		return;
	    }
	}
    }
    /* default: */
    Rprintf("%s: ", prefix);
}

/* JIT support */
typedef unsigned long R_exprhash_t;

static R_exprhash_t hash(unsigned char *str, int n, R_exprhash_t hash)
{
    // djb2 from http://www.cse.yorku.ca/~oz/hash.html
    // (modified for n-byte lengths)

    int i;

    for(i = 0; i < n; i++)
        hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */

    return hash;
}

#define HASH(x, h) hash((unsigned char *) &x, sizeof(x), h)

static R_exprhash_t hashexpr1(SEXP e, R_exprhash_t h)
{
#define SKIP_NONSCALAR 	if (len != 1) break /* non-scalars hashed by address */
    int len = length(e);
    int type = TYPEOF(e);
    h = HASH(type, h);
    h = HASH(len, h);

    switch(type) {
    case LANGSXP:
    case LISTSXP:
	/**** safer to only follow while CDR is LANGSXP/LISTSXP */
	for (; e != R_NilValue; e = CDR(e))
	    h = hashexpr1(CAR(e), h);
	return h;
    case LGLSXP:
	SKIP_NONSCALAR;
	for (int i = 0; i < len; i++) {
	    int ival = LOGICAL(e)[i];
	    h = HASH(ival, h);
	}
	return h;
    case INTSXP:
	SKIP_NONSCALAR;
	for (int i = 0; i < len; i++) {
	    int ival = INTEGER(e)[i];
	    h = HASH(ival, h);
	}
	return h;
    case REALSXP:
	SKIP_NONSCALAR;
	for (int i = 0; i < len; i++) {
	    double dval = REAL(e)[i];
	    h = HASH(dval, h);
	}
	return h;
    case STRSXP:
	SKIP_NONSCALAR;
	for (int i = 0; i < len; i++) {
	    SEXP cval = STRING_ELT(e, i);
	    h = hash((unsigned char *) CHAR(cval), LENGTH(cval), h);
	}
	return h;
    }

    return HASH(e, h);
#undef SKIP_NONSCALAR
}

static R_INLINE SEXP getSrcref(SEXP srcrefs, int ind);
static R_exprhash_t hashsrcref(SEXP e, R_exprhash_t h)
{
    if (TYPEOF(e) == INTSXP && LENGTH(e) >= 6) {
	for(int i = 0; i < 6; i++) {
	    int ival = INTEGER(e)[i];
	    h = HASH(ival, h);
	}
	/* FIXME: update this when deep-comparison of srcref is available */
	SEXP srcfile = getAttrib(e, R_SrcfileSymbol);
	h = HASH(srcfile, h);
    }
    return h;
}
#undef HASH

static R_exprhash_t hashexpr(SEXP e)
{
    return hashexpr1(e, 5381);
}

static R_exprhash_t hashfun(SEXP f)
{
    R_exprhash_t h = hashexpr(BODY(f));
    if (getAttrib(BODY(f), R_SrcrefSymbol) == R_NilValue)
	h = hashsrcref(getAttrib(f, R_SrcrefSymbol), h);
    return h;
}

static void loadCompilerNamespace(void)
{
    SEXP fun, arg, expr;

    PROTECT(fun = install("getNamespace"));
    PROTECT(arg = mkString("compiler"));
    PROTECT(expr = lang2(fun, arg));
    eval(expr, R_GlobalEnv);
    UNPROTECT(3);
}

static void checkCompilerOptions(int jitEnabled)
{
    int old_visible = R_Visible;
    SEXP packsym, funsym, call, fcall, arg;

    packsym = install("compiler");
    funsym = install("checkCompilerOptions");

    PROTECT(arg = ScalarInteger(jitEnabled));
    PROTECT(fcall = lang3(R_TripleColonSymbol, packsym, funsym));
    PROTECT(call = lang2(fcall, arg));
    eval(call, R_GlobalEnv);
    UNPROTECT(3);
    R_Visible = old_visible;
}

static SEXP R_IfSymbol = NULL;
static SEXP R_ForSymbol = NULL;
static SEXP R_WhileSymbol = NULL;
static SEXP R_RepeatSymbol = NULL;

#define JIT_CACHE_SIZE 1024
static SEXP JIT_cache = NULL;
static R_exprhash_t JIT_cache_hashes[JIT_CACHE_SIZE];

/**** allow MIN_JIT_SCORE, or both, to be changed by environment variables? */
static int MIN_JIT_SCORE = 50;
#define LOOP_JIT_SCORE MIN_JIT_SCORE

static struct { unsigned long count, envcount, bdcount; } jit_info = {0, 0, 0};

void attribute_hidden R_init_jit_enabled(void)
{
    /* Need to force the lazy loading promise to avoid recursive
       promise evaluation when JIT is enabled. Might be better to do
       this in baseloader.R. */
    eval(install(".ArgsEnv"), R_BaseEnv);

    int val = 3; /* turn JIT on by default */
    char *enable = getenv("R_ENABLE_JIT");
    if (enable != NULL)
	val = atoi(enable);
    if (val) {
	loadCompilerNamespace();
	checkCompilerOptions(val);
    }
    R_jit_enabled = val;

    if (R_compile_pkgs <= 0) {
	char *compile = getenv("_R_COMPILE_PKGS_");
	if (compile != NULL) {
	    int val = atoi(compile);
	    if (val > 0)
		R_compile_pkgs = TRUE;
	    else
		R_compile_pkgs = FALSE;
	}
    }

    if (R_disable_bytecode <= 0) {
	char *disable = getenv("R_DISABLE_BYTECODE");
	if (disable != NULL) {
	    int val = atoi(disable);
	    if (val > 0)
		R_disable_bytecode = TRUE;
	    else
		R_disable_bytecode = FALSE;
	}
    }

    /* -1 ... duplicate constants on LDCONST and PUSHCONSTARG, no checking
        0 ... no checking (no duplication for >= 0) [DEFAULT]
	1 ... check at error, session exit and reclamation
	2 ... check also at full GC
	3 ... check also at partial GC
	4 ... check also at .Call
	5 ... (very) verbose report on modified constants
    */
    if (R_check_constants <= 1) {
	char *check = getenv("R_CHECK_CONSTANTS");
	if (check != NULL)
	    R_check_constants = atoi(check);
    }

    /* initialize JIT variables */
    R_IfSymbol = install("if");
    R_ForSymbol = install("for");
    R_WhileSymbol = install("while");
    R_RepeatSymbol = install("repeat");

    R_PreserveObject(JIT_cache = allocVector(VECSXP, JIT_CACHE_SIZE));
}

static int JIT_score(SEXP e)
{
    if (TYPEOF(e) == LANGSXP) {
	SEXP fun = CAR(e);
	if (fun == R_IfSymbol) {
	    int cons = JIT_score(CADR(e));
	    int alt =  JIT_score(CADDR(e));
	    return cons > alt ? cons : alt;
	}
	else if (fun == R_ForSymbol ||
		 fun == R_WhileSymbol ||
		 fun == R_RepeatSymbol)
	    return LOOP_JIT_SCORE;
	else {
	    int score = 1;
	    for (SEXP args = CDR(e); args != R_NilValue; args = CDR(args))
		score += JIT_score(CAR(args));
	    return score;
	}
    }
    else return 1;
}

#define STRATEGY_NO_SMALL 0
#define STRATEGY_TOP_SMALL_MAYBE 1
#define STRATEGY_ALL_SMALL_MAYBE 2
#define STRATEGY_NO_SCORE 3
#define STRATEGY_NO_CACHE 4
/* max strategy index is hardcoded in R_CheckJIT */

/*
  NO_CACHE
      functions are compiled 1st time seen
        code is never cached

  NO_SCORE
      functions are compiled 1st time seen
        code is cached
	in case of conflict function may be marked NOJIT

  ALL_SMALL_MAYBE
      functions with small score are compiled 2nd time seen
      function with high score are compiled
          1st time seen if top-level, 2nd time seen otherwise

  TOP_SMALL_MAYBE
      functions with small score compiled
          2nd time seen if top-level, never otherwise
      functions with high score compiled
          1st time seen if top-level, 2nd time seen otherwise
*/

static int jit_strategy = -1;

static R_INLINE Rboolean R_CheckJIT(SEXP fun)
{
    /* to help with testing */
    if (jit_strategy < 0) {
	int dflt = R_jit_enabled == 1 ?
	    STRATEGY_NO_SMALL : STRATEGY_TOP_SMALL_MAYBE;
	int val = dflt;
	char *valstr = getenv("R_JIT_STRATEGY");
	if (valstr != NULL)
	    val = atoi(valstr);
	if (val < 0 || val > 4)
	    jit_strategy = dflt;
	else
	    jit_strategy = val;

	valstr = getenv("R_MIN_JIT_SCORE");
	if (valstr != NULL)
	    MIN_JIT_SCORE = atoi(valstr);
    }

    SEXP body = BODY(fun);

    if (R_jit_enabled > 0 && TYPEOF(body) != BCODESXP &&
	! R_disable_bytecode && ! NOJIT(fun)) {

	if (MAYBEJIT(fun)) {
	    /* function marked as MAYBEJIT the first time now seen
	       twice, so go ahead and compile */
	    UNSET_MAYBEJIT(fun);
	    return TRUE;
	}

	if (jit_strategy == STRATEGY_NO_SCORE ||
	    jit_strategy == STRATEGY_NO_CACHE)
	    return TRUE;

	int score = JIT_score(body);
	if (jit_strategy == STRATEGY_ALL_SMALL_MAYBE)
	    if (score < MIN_JIT_SCORE) { SET_MAYBEJIT(fun); return FALSE; }

	if (CLOENV(fun) == R_GlobalEnv) {
	    /* top level functions are only compiled if score is high enough */
	    if (score < MIN_JIT_SCORE) {
		if (jit_strategy == STRATEGY_TOP_SMALL_MAYBE)
		    SET_MAYBEJIT(fun);
		else
		    SET_NOJIT(fun);
		return FALSE;
	    }
	    else return TRUE;
	}
	else {
	    /* only compile non-top-level function if score is high
	       enough and seen twice */
	    if (score < MIN_JIT_SCORE) {
		SET_NOJIT(fun);
		return FALSE;
	    }
	    else {
		SET_MAYBEJIT(fun);
		return FALSE;
	    }
	}
    }
    return FALSE;
}

#ifdef DEBUG_JIT
# define PRINT_JIT_INFO							\
    REprintf("JIT cache hits: %ld; env: %ld; body %ld\n",		\
	     jit_info.count, jit_info.envcount, jit_info.bdcount)
#else
# define PRINT_JIT_INFO	do { } while(0)
#endif

SEXP topenv(SEXP, SEXP); /**** should be in a header file */

/* FIXME: this should not depend on internals from envir.c but does for now. */
/* copied from envir.c for now */
#define IS_USER_DATABASE(rho)  (OBJECT((rho)) && inherits((rho), "UserDefinedDatabase"))
#define IS_STANDARD_UNHASHED_FRAME(e) (! IS_USER_DATABASE(e) && HASHTAB(e) == R_NilValue)
#define IS_STANDARD_HASHED_FRAME(e) (! IS_USER_DATABASE(e) && HASHTAB(e) != R_NilValue)

/* This makes a snapshot of the local variables in cmpenv and creates
   a new environment with the same top level environment and bindings
   with value R_NilValue for the local variables. This guards against
   the cmpenv changing after being entered in the cache, and also
   allows large values that might be bound to local variables in
   cmpenv to be reclaimed (also, some package tests, e.g. in shiny, test
   when things get reclaimed). Standard local frames are processed directly,
   hashed frames are processed via lsInternal3, which involves extra
   allocations, but should be used rarely. If a local environment is
   of unsupported type, topenv is returned as a valid conservative
   answer.

   Since we compute the local variables at compile
   time we should record them in the byte code object and use the
   recorded value. */
static R_INLINE void cmpenv_enter_frame(SEXP frame, SEXP newenv)
{
    for (; frame != R_NilValue; frame = CDR(frame))
	defineVar(TAG(frame), R_NilValue, newenv);
}

static R_INLINE SEXP make_cached_cmpenv(SEXP fun)
{
    SEXP frmls = FORMALS(fun);
    SEXP cmpenv = CLOENV(fun);
    SEXP top = topenv(R_NilValue, cmpenv);
    if (cmpenv == top && frmls == R_NilValue)
	return cmpenv;
    else {
	SEXP newenv = PROTECT(NewEnvironment(R_NilValue, R_NilValue, top));
	for (; frmls != R_NilValue; frmls = CDR(frmls))
	    defineVar(TAG(frmls), R_NilValue, newenv);
	for (SEXP env = cmpenv; env != top; env = CDR(env)) {
	    if (IS_STANDARD_UNHASHED_FRAME(env))
		cmpenv_enter_frame(FRAME(env), newenv);
	    else if (IS_STANDARD_HASHED_FRAME(env)) {
		SEXP h = HASHTAB(env);
		int n = length(h);
		for (int i = 0; i < n; i++)
		    cmpenv_enter_frame(VECTOR_ELT(h, i), newenv);
	    } else {
		UNPROTECT(1); /* newenv */
		return top;
	    }
		/* topenv is a safe conservative answer; if a closure
		   defines anything, its environment will not match, and
		   it will never be compiled */
		/* FIXME: would it be safe to simply ignore elements of
		   of these environments? */
	}
	UNPROTECT(1); /* newenv */
	return newenv;
    }
}

/* Cache entries are CONS cells with the body in CAR, the environment
   in CDR, and the Srcref in the TAG. */
static R_INLINE void set_jit_cache_entry(R_exprhash_t hash, SEXP val)
{
    int hashidx = hash % JIT_CACHE_SIZE;

    PROTECT(val);
    SEXP entry = CONS(BODY(val), make_cached_cmpenv(val));
    SET_VECTOR_ELT(JIT_cache, hashidx, entry);
    SET_TAG(entry, getAttrib(val, R_SrcrefSymbol));
    UNPROTECT(1); /* val */

    JIT_cache_hashes[hashidx] = hash;
}

static R_INLINE SEXP jit_cache_code(SEXP entry)
{
    return CAR(entry);
}

static R_INLINE SEXP jit_cache_env(SEXP entry)
{
    return CDR(entry);
}

static R_INLINE SEXP jit_cache_srcref(SEXP entry)
{
    return TAG(entry);
}

/* forward declaration */
//-------------- bc start --------------
static SEXP bytecodeExpr(SEXP e){};
SEXP R_PromiseExpr(SEXP p)
{
    return bytecodeExpr(PRCODE(p));
}

SEXP R_ClosureExpr(SEXP p)
{
    return bytecodeExpr(BODY(p));
}

/* Checks if constants of any registered BCODESXP have been modified.
   Returns TRUE if the constants are ok, otherwise returns false or aborts.*/
Rboolean attribute_hidden R_checkConstants(Rboolean abortOnError)
{}

SEXP attribute_hidden R_findBCInterpreterSrcref(RCNTXT *cptr)
{
    //return R_findBCInterpreterLocation(cptr, "srcrefsIndex");
}

static SEXP R_findBCInterpreterExpression()
{
    //return R_findBCInterpreterLocation(NULL, "expressionsIndex");
}
/* Get the current expression being evaluated by the byte-code interpreter. */
SEXP attribute_hidden R_getBCInterpreterExpression()
{}

SEXP attribute_hidden R_getCurrentSrcref()
{

}
//-------------------- bc end -------
static R_INLINE SEXP jit_cache_expr(SEXP entry)
{
    return bytecodeExpr(jit_cache_code(entry));
}

static R_INLINE SEXP get_jit_cache_entry(R_exprhash_t hash)
{
    int hashidx = hash % JIT_CACHE_SIZE;
    if (JIT_cache_hashes[hashidx] == hash) {
	SEXP entry = VECTOR_ELT(JIT_cache, hashidx);
	if (TYPEOF(jit_cache_code(entry)) == BCODESXP)
	    return entry;
	else
	    /* function has been de-compiled; clear the cache entry */
	    SET_VECTOR_ELT(JIT_cache, hashidx, R_NilValue);
    }
    return R_NilValue;
}

static R_INLINE Rboolean jit_expr_match(SEXP expr, SEXP body)
{
    /*** is 16 right here??? does this need to be faster??? */
    return R_compute_identical(expr, body, 16);
}

static R_INLINE SEXP cmpenv_topenv(SEXP cmpenv)
{
    return topenv(R_NilValue, cmpenv);
}

static R_INLINE Rboolean cmpenv_exists_local(SEXP sym, SEXP cmpenv, SEXP top)
{
    if (cmpenv != top)
	for (SEXP frame = FRAME(cmpenv);
	     frame != R_NilValue;
	     frame = CDR(frame))
	    if (TAG(frame) == sym)
		return TRUE;
    return FALSE;
}

static R_INLINE Rboolean jit_env_match(SEXP cmpenv, SEXP fun)
{
    /* Can code compiled for environment cmpenv be used as compiled
       code for environment env?  These tests rely on the assumption
       that compilation is only affected by what variables are bound,
       not their values. So as long as both cmpenv and env have the
       same top level environment and all local bindings present in
       the formals and environment of fun are also present in cmpenv
       the code for cmpenv can be reused, though it might be less
       efficient if a binding in cmpenv prevents an optimization that
       would be possible in env. */

    SEXP env = CLOENV(fun);
    SEXP top = topenv(R_NilValue, env);

    if (top == cmpenv_topenv(cmpenv)) {
	for (SEXP frmls = FORMALS(fun); frmls != R_NilValue; frmls = CDR(frmls))
	    if (! cmpenv_exists_local(TAG(frmls), cmpenv, top))
		return FALSE;
	for (; env != top; env = ENCLOS(env)) {
	    if (IS_STANDARD_UNHASHED_FRAME(env)) {
		/* To keep things simple, for a match this code
		   requires that the local frames be standard unhashed
		   frames. */
		for (SEXP frame = FRAME(env);
		     frame != R_NilValue;
		     frame = CDR(frame))
		    if (! cmpenv_exists_local(TAG(frame), cmpenv, top))
			return FALSE;
	    }
	    else return FALSE;
	}
	return TRUE;
    }
    else return FALSE;
}

static R_INLINE Rboolean jit_srcref_match(SEXP cmpsrcref, SEXP srcref)
{
    return R_compute_identical(cmpsrcref, srcref, 0);
}

SEXP attribute_hidden R_cmpfun1(SEXP fun)
{
    int old_visible = R_Visible;
    SEXP packsym, funsym, call, fcall, val;

    packsym = install("compiler");
    funsym = install("tryCmpfun");

    PROTECT(fcall = lang3(R_TripleColonSymbol, packsym, funsym));
    PROTECT(call = lang2(fcall, fun));
    val = eval(call, R_GlobalEnv);
    UNPROTECT(2);

    R_Visible = old_visible;
    return val;
}

SEXP attribute_hidden R_cmpfun(SEXP fun)
{
    R_exprhash_t hash = 0;
    if (jit_strategy != STRATEGY_NO_CACHE) {
	hash = hashfun(fun);
	SEXP entry = get_jit_cache_entry(hash);

	if (entry != R_NilValue) {
	    jit_info.count++;
	    if (jit_env_match(jit_cache_env(entry), fun)) {
		jit_info.envcount++;
		if (jit_expr_match(jit_cache_expr(entry), BODY(fun))) {
		    jit_info.bdcount++;
		    /* if function body has a srcref, all srcrefs compiled
		       in that function only depend on the body srcref;
		       but, otherwise the srcrefs compiled in are taken
		       from the function (op) */
		    if (getAttrib(BODY(fun), R_SrcrefSymbol) != R_NilValue ||
			jit_srcref_match(jit_cache_srcref(entry),
					 getAttrib(fun, R_SrcrefSymbol))) {
			PRINT_JIT_INFO;
			SET_BODY(fun, jit_cache_code(entry));
			/**** reset the cache here?*/
			return fun;
		    }
		}
		/* The functions probably differ only in source references
		   (for functions with bodies that have no source references
		   we know for sure, for other functions we speculate).
		   Therefore, we allow re-compilation and re-caching. This
		   situation may be caused e.g. by re-sourcing the same source
		   file or re-pasting the same definitions for a function in
		   interactive R session. Note srcref information includes
		   environments (srcfile), which are now compared by address,
		   so it may be we actually have logically identical source
		   references, anyway. */
		/* FIXME: revisit this when deep comparison of environments
			  (and srcrefs) is available */
	    } else {
		SET_NOJIT(fun);
		/**** also mark the cache entry as NOJIT, or as need to see
		      many times? */
		return fun;
	    }
	}
	PRINT_JIT_INFO;
    }

    SEXP val = R_cmpfun1(fun);

    if (TYPEOF(BODY(val)) != BCODESXP)
	SET_NOJIT(fun);
    else if (jit_strategy != STRATEGY_NO_CACHE)
	set_jit_cache_entry(hash, val); /* val is protected by callee */

    return val;
}

static SEXP R_compileExpr(SEXP expr, SEXP rho)
{
    int old_visible = R_Visible;
    SEXP packsym, funsym, quotesym;
    SEXP qexpr, call, fcall, val;

    packsym = install("compiler");
    funsym = install("tryCompile");
    quotesym = install("quote");

    PROTECT(fcall = lang3(R_TripleColonSymbol, packsym, funsym));
    PROTECT(qexpr = lang2(quotesym, expr));
    /* compile(e, env, options, srcref) */
    PROTECT(call = lang5(fcall, qexpr, rho, R_NilValue, R_getCurrentSrcref()));
    val = eval(call, R_GlobalEnv);
    UNPROTECT(3);
    R_Visible = old_visible;
    return val;
}

static Rboolean R_compileAndExecute(SEXP call, SEXP rho)
{
    int old_enabled = R_jit_enabled;
    SEXP code;
    Rboolean ans = FALSE;

    R_jit_enabled = 0;
    PROTECT(call);
    PROTECT(rho);
    PROTECT(code = R_compileExpr(call, rho));
    R_jit_enabled = old_enabled;

    if (TYPEOF(code) == BCODESXP) {
	bcEval(code, rho, TRUE);
	ans = TRUE;
    }

    UNPROTECT(3);
    return ans;
}

SEXP attribute_hidden do_enablejit(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int old = R_jit_enabled, new_v;
    checkArity(op, args);
    new_v = asInteger(CAR(args));
    if (new_v >= 0) {
	if (new_v > 0)
	    loadCompilerNamespace();
	checkCompilerOptions(new_v);
	R_jit_enabled = new_v;
    }
    /* negative 'new' just returns 'old' */
    return ScalarInteger(old);
}

SEXP attribute_hidden do_compilepkgs(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int old = R_compile_pkgs, new_v;
    checkArity(op, args);
    new_v = asLogical(CAR(args));
    if (new_v != NA_LOGICAL && new_v)
	loadCompilerNamespace();
    R_compile_pkgs = new_v;
    return ScalarLogical(old);
}

/* this function gets the srcref attribute from a statement block,
   and confirms it's in the expected format */

static R_INLINE SEXP getBlockSrcrefs(SEXP call)
{
    SEXP srcrefs = getAttrib(call, R_SrcrefSymbol);
    if (TYPEOF(srcrefs) == VECSXP) return srcrefs;
    return R_NilValue;
}

/* this function extracts one srcref, and confirms the format */
/* It assumes srcrefs has already been validated to be a VECSXP or NULL */

static R_INLINE SEXP getSrcref(SEXP srcrefs, int ind)
{
    SEXP result;
    if (!isNull(srcrefs)
	&& length(srcrefs) > ind
	&& !isNull(result = VECTOR_ELT(srcrefs, ind))
	&& TYPEOF(result) == INTSXP
	&& length(result) >= 6)
	return result;
    else
	return R_NilValue;
}

/* There's another copy of this in main.c */
static void PrintCall(SEXP call, SEXP rho)
{
    int old_bl = R_BrowseLines,
        blines = asInteger(GetOption1(install("deparse.max.lines")));
    if(blines != NA_INTEGER && blines > 0)
	R_BrowseLines = blines;
    PrintValueRec(call, rho);
    R_BrowseLines = old_bl;
}

/* Note: GCC will not inline execClosure because it calls setjmp */
static R_INLINE SEXP R_execClosure(SEXP call, SEXP newrho, SEXP sysparent,
                                   SEXP rho, SEXP arglist, SEXP op);

/* Apply SEXP op of type CLOSXP to actuals */
SEXP applyClosure(SEXP call, SEXP op, SEXP arglist, SEXP rho, SEXP suppliedvars)
{
    SEXP formals, actuals, savedrho, newrho;
    SEXP f, a;

    /* formals = list of formal parameters */
    /* actuals = values to be bound to formals */
    /* arglist = the tagged list of arguments */

    /* protection against rho = NULL */
    // these are deliberately not translated
    if (!rho)
	errorcall(call,
		  "'rho' cannot be C NULL: detected in C-level applyClosure");
    if (!isEnvironment(rho))
	errorcall(call, "'rho' must be an environment not %s: detected in C-level applyClosure",
		  type2char(TYPEOF(rho)));

    formals = FORMALS(op);
    savedrho = CLOENV(op);

    /*  Build a list which matches the actual (unevaluated) arguments
	to the formal paramters.  Build a new environment which
	contains the matched pairs.  Ideally this environment sould be
	hashed.  */

    actuals = matchArgs(formals, arglist, call);
    PROTECT(newrho = NewEnvironment(formals, actuals, savedrho));

    /* Turn on reference counting for the binding cells so local
       assignments arguments increment REFCNT values */
    for (a = actuals; a != R_NilValue; a = CDR(a))
	ENABLE_REFCNT(a);

    /*  Use the default code for unbound formals.  FIXME: It looks like
	this code should preceed the building of the environment so that
	this will also go into the hash table.  */

    /* This piece of code is destructively modifying the actuals list,
       which is now also the list of bindings in the frame of newrho.
       This is one place where internal structure of environment
       bindings leaks out of envir.c.  It should be rewritten
       eventually so as not to break encapsulation of the internal
       environment layout.  We can live with it for now since it only
       happens immediately after the environment creation.  LT */

    f = formals;
    a = actuals;
    while (f != R_NilValue) {
	if (CAR(a) == R_MissingArg && CAR(f) != R_MissingArg) {
	    SETCAR(a, mkPROMISE(CAR(f), newrho));
	    SET_MISSING(a, 2);
	}
	f = CDR(f);
	a = CDR(a);
    }

    /*  Fix up any extras that were supplied by usemethod. */

    if (suppliedvars != R_NilValue)
	addMissingVarsToNewEnv(newrho, suppliedvars);

    if (R_envHasNoSpecialSymbols(newrho))
	SET_NO_SPECIAL_SYMBOLS(newrho);

    UNPROTECT(1); /* newrho - below protected via context */

    /*  If we have a generic function we need to use the sysparent of
	the generic as the sysparent of the method because the method
	is a straight substitution of the generic.  */

    return R_execClosure(call, newrho,
	                 (R_GlobalContext->callflag == CTXT_GENERIC) ?
	                     R_GlobalContext->sysparent : rho,
	                 rho, arglist, op);
}

static R_INLINE SEXP R_execClosure(SEXP call, SEXP newrho, SEXP sysparent,
                                   SEXP rho, SEXP arglist, SEXP op)
{
    volatile SEXP body;
    RCNTXT cntxt;
    Rboolean dbg = FALSE;

    begincontext(&cntxt, CTXT_RETURN, call, newrho, sysparent, arglist, op);

    body = BODY(op);
    if (R_CheckJIT(op)) {
	int old_enabled = R_jit_enabled;
	SEXP newop;
	R_jit_enabled = 0;
	newop = R_cmpfun(op);
	body = BODY(newop);
	SET_BODY(op, body);
	R_jit_enabled = old_enabled;
    }

    /* Get the srcref record from the closure object. The old srcref was
       saved in cntxt. */

    R_Srcref = getAttrib(op, R_SrcrefSymbol);

    /* Debugging */

    if ((RDEBUG(op) && R_current_debug_state()) || RSTEP(op)
         || (RDEBUG(rho) && R_BrowserLastCommand == 's')) {

	dbg = TRUE;
	SET_RSTEP(op, 0);
	SET_RDEBUG(newrho, 1);
	cntxt.browserfinish = 0; /* Don't want to inherit the "f" */
	/* switch to interpreted version when debugging compiled code */
	if (TYPEOF(body) == BCODESXP)
	    body = bytecodeExpr(body);
	Rprintf("debugging in: ");
	PrintCall(call, rho);
	SrcrefPrompt("debug", R_Srcref);
	PrintValue(body);
	//--by GT-- do_browser(call, op, R_NilValue, newrho);
    }

    /*  Set a longjmp target which will catch any explicit returns
	from the function body.  */

    if ((SETJMP(cntxt.cjmpbuf))) {
	if (! cntxt.jumptarget /* ignores intermediate jumps for on.exits */
	    && R_ReturnedValue == R_RestartToken) {
	    cntxt.callflag = CTXT_RETURN;  /* turn restart off */
	    R_ReturnedValue = R_NilValue;  /* remove restart token */
	    cntxt.returnValue = eval(body, newrho);
	}
	else
	    cntxt.returnValue = R_ReturnedValue;
    }
    else
	/* make it available to on.exit and implicitly protect */
	cntxt.returnValue = eval(body, newrho);

    R_Srcref = cntxt.srcref;
    endcontext(&cntxt);

    if (dbg) {
	Rprintf("exiting from: ");
	PrintCall(call, rho);
    }
    return cntxt.returnValue;
}

SEXP R_forceAndCall(SEXP e, int n, SEXP rho)
{
    SEXP fun, tmp;
    if (TYPEOF(CAR(e)) == SYMSXP)
	/* This will throw an error if the function is not found */
	PROTECT(fun = findFun(CAR(e), rho));
    else
	PROTECT(fun = eval(CAR(e), rho));

    if (TYPEOF(fun) == SPECIALSXP) {
	int flag = PRIMPRINT(fun);
	PROTECT(e);
	R_Visible = flag != 1;
	tmp = PRIMFUN(fun) (e, fun, CDR(e), rho);
	if (flag < 2) R_Visible = flag != 1;
	UNPROTECT(1);
    }
    else if (TYPEOF(fun) == BUILTINSXP) {
	int flag = PRIMPRINT(fun);
	PROTECT(tmp = evalList(CDR(e), rho, e, 0));
	if (flag < 2) R_Visible = flag != 1;
	/* We used to insert a context only if profiling,
	   but helps for tracebacks on .C etc. */
	if (R_Profiling || (PPINFO(fun).kind == PP_FOREIGN)) {
	    RCNTXT cntxt;
	    SEXP oldref = R_Srcref;
	    begincontext(&cntxt, CTXT_BUILTIN, e,
			 R_BaseEnv, R_BaseEnv, R_NilValue, R_NilValue);
	    R_Srcref = NULL;
	    tmp = PRIMFUN(fun) (e, fun, tmp, rho);
	    R_Srcref = oldref;
	    endcontext(&cntxt);
	} else {
	    tmp = PRIMFUN(fun) (e, fun, tmp, rho);
	}
	if (flag < 2) R_Visible = flag != 1;
	UNPROTECT(1);
    }
    else if (TYPEOF(fun) == CLOSXP) {
	PROTECT(tmp = promiseArgs(CDR(e), rho));
	SEXP a;
	int i;
	for (a = tmp, i = 0; i < n && a != R_NilValue; a = CDR(a), i++) {
	    SEXP p = CAR(a);
	    if (TYPEOF(p) == PROMSXP)
		eval(p, rho);
	    else if (p == R_MissingArg)
		errorcall(e, _("argument %d is empty"), i + 1);
	    else error("something weird happened");
	}
	tmp = applyClosure(e, fun, tmp, rho, R_NilValue);
	UNPROTECT(1);
    }
    else {
	tmp = R_NilValue; /* -Wall */
	error(_("attempt to apply non-function"));
    }

    UNPROTECT(1);
    return tmp;
}

SEXP attribute_hidden do_forceAndCall(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int n = asInteger(eval(CADR(call), rho));
    SEXP e = CDDR(call);

    /* this would not be needed if CDDR(call) was a LANGSXP */
    PROTECT(e = LCONS(CAR(e), CDR(e)));
    SEXP val = R_forceAndCall(e, n, rho);
    UNPROTECT(1);
    return val;
}

/* **** FIXME: Temporary code to execute S4 methods in a way that
   **** preserves lexical scope. */

/* called from methods_list_dispatch.c */
SEXP R_execMethod(SEXP op, SEXP rho)
{
    SEXP call, arglist, callerenv, newrho, next, val;
    RCNTXT *cptr;

    /* create a new environment frame enclosed by the lexical
       environment of the method */
    PROTECT(newrho = Rf_NewEnvironment(R_NilValue, R_NilValue, CLOENV(op)));

    /* copy the bindings for the formal environment from the top frame
       of the internal environment of the generic call to the new
       frame.  need to make sure missingness information is preserved
       and the environments for any default expression promises are
       set to the new environment.  should move this to envir.c where
       it can be done more efficiently. */
    for (next = FORMALS(op); next != R_NilValue; next = CDR(next)) {
	SEXP symbol =  TAG(next);
	R_varloc_t loc;
	int missing;
	loc = R_findVarLocInFrame(rho,symbol);
	if(R_VARLOC_IS_NULL(loc))
	    error(_("could not find symbol \"%s\" in environment of the generic function"),
		  CHAR(PRINTNAME(symbol)));
	missing = R_GetVarLocMISSING(loc);
	val = R_GetVarLocValue(loc);
	SET_FRAME(newrho, CONS(val, FRAME(newrho)));
	SET_TAG(FRAME(newrho), symbol);
	if (missing) {
	    SET_MISSING(FRAME(newrho), missing);
	    if (TYPEOF(val) == PROMSXP && PRENV(val) == rho) {
		SEXP deflt;
		SET_PRENV(val, newrho);
		/* find the symbol in the method, copy its expression
		 * to the promise */
		for(deflt = CAR(op); deflt != R_NilValue; deflt = CDR(deflt)) {
		    if(TAG(deflt) == symbol)
			break;
		}
		if(deflt == R_NilValue)
		    error(_("symbol \"%s\" not in environment of method"),
			  CHAR(PRINTNAME(symbol)));
		SET_PRCODE(val, CAR(deflt));
	    }
	}
    }

    /* copy the bindings of the spacial dispatch variables in the top
       frame of the generic call to the new frame */
    defineVar(R_dot_defined, findVarInFrame(rho, R_dot_defined), newrho);
    defineVar(R_dot_Method, findVarInFrame(rho, R_dot_Method), newrho);
    defineVar(R_dot_target, findVarInFrame(rho, R_dot_target), newrho);

    /* copy the bindings for .Generic and .Methods.  We know (I think)
       that they are in the second frame, so we could use that. */
    defineVar(R_dot_Generic, findVar(R_dot_Generic, rho), newrho);
    defineVar(R_dot_Methods, findVar(R_dot_Methods, rho), newrho);

    /* Find the calling context.  Should be R_GlobalContext unless
       profiling has inserted a CTXT_BUILTIN frame. */
    cptr = R_GlobalContext;
    if (cptr->callflag & CTXT_BUILTIN)
	cptr = cptr->nextcontext;

    /* The calling environment should either be the environment of the
       generic, rho, or the environment of the caller of the generic,
       the current sysparent. */
    callerenv = cptr->sysparent; /* or rho? */

    /* get the rest of the stuff we need from the current context,
       execute the method, and return the result */
    call = cptr->call;
    arglist = cptr->promargs;
    val = R_execClosure(call, newrho, callerenv, callerenv, arglist, op);
    UNPROTECT(1);
    return val;
}

static SEXP EnsureLocal(SEXP symbol, SEXP rho)
{
    SEXP vl;

    if ((vl = findVarInFrame3(rho, symbol, TRUE)) != R_UnboundValue) {
	vl = eval(symbol, rho);	/* for promises */
	if(MAYBE_SHARED(vl)) {
	    PROTECT(vl = shallow_duplicate(vl));
	    defineVar(symbol, vl, rho);
	    UNPROTECT(1);
	    SET_NAMED(vl, 1);
	}
	return vl;
    }

    vl = eval(symbol, ENCLOS(rho));
    if (vl == R_UnboundValue)
	error(_("object '%s' not found"), EncodeChar(PRINTNAME(symbol)));

    PROTECT(vl = shallow_duplicate(vl));
    defineVar(symbol, vl, rho);
    UNPROTECT(1);
    SET_NAMED(vl, 1);
    return vl;
}


/* Note: If val is a language object it must be protected */
/* to prevent evaluation.  As an example consider */
/* e <- quote(f(x=1,y=2); names(e) <- c("","a","b") */

static SEXP R_valueSym = NULL; /* initialized in R_initAssignSymbols below */

static SEXP replaceCall(SEXP fun, SEXP val, SEXP args, SEXP rhs)
{
    SEXP tmp, ptmp;
    PROTECT(fun);
    PROTECT(args);
    PROTECT(rhs);
    PROTECT(val);
    ptmp = tmp = allocList(length(args)+3);
    UNPROTECT(4);
    SETCAR(ptmp, fun); ptmp = CDR(ptmp);
    SETCAR(ptmp, val); ptmp = CDR(ptmp);
    while(args != R_NilValue) {
	SETCAR(ptmp, CAR(args));
	SET_TAG(ptmp, TAG(args));
	ptmp = CDR(ptmp);
	args = CDR(args);
    }
    SETCAR(ptmp, rhs);
    SET_TAG(ptmp, R_valueSym);
    SET_TYPEOF(tmp, LANGSXP);
    return tmp;
}


static SEXP assignCall(SEXP op, SEXP symbol, SEXP fun,
		       SEXP val, SEXP args, SEXP rhs)
{
    PROTECT(op);
    PROTECT(symbol);
    val = replaceCall(fun, val, args, rhs);
    UNPROTECT(2);
    return lang3(op, symbol, val);
}


static R_INLINE Rboolean asLogicalNoNA(SEXP s, SEXP call)
{
    Rboolean cond = NA_LOGICAL;

    if (length(s) > 1) {
	PROTECT(s);	 /* needed as per PR#15990.  call gets protected by warningcall() */
	char *check = getenv("_R_CHECK_LENGTH_1_CONDITION_");
	if((check != NULL) ? StringTrue(check) : FALSE) // warn by default
	    errorcall(call, _("the condition has length > 1"));
        else
	    warningcall(call,
		    _("the condition has length > 1 and only the first element will be used"));
	UNPROTECT(1);
    }
    if (length(s) > 0) {
	/* inline common cases for efficiency */
	switch(TYPEOF(s)) {
	case LGLSXP:
	    cond = LOGICAL(s)[0];
	    break;
	case INTSXP:
	    cond = INTEGER(s)[0]; /* relies on NA_INTEGER == NA_LOGICAL */
	    break;
	default:
	    cond = asLogical(s);
	}
    }

    if (cond == NA_LOGICAL) {
	char *msg = (char *)(length(s) ? (isLogical(s) ?
				 _("missing value where TRUE/FALSE needed") :
				 _("argument is not interpretable as logical")) :
	    _("argument is of length zero"));
	PROTECT(s);	/* Maybe needed in some weird circumstance. */
	errorcall(call, msg);
	UNPROTECT(1);
    }
    return cond;
}


#define BodyHasBraces(body) \
    ((isLanguage(body) && CAR(body) == R_BraceSymbol) ? 1 : 0)

/* Allocate space for the loop variable value the first time through
   (when v == R_NilValue) and when the value has been assigned to
   another variable (NAMED(v) > 1). This should be safe and avoid
   allocation in many cases. */
#define ALLOC_LOOP_VAR(v, val_type, vpi) do { \
	if (v == R_NilValue || MAYBE_SHARED(v)) { \
	    REPROTECT(v = allocVector(val_type, 1), vpi); \
	    SET_NAMED(v, 1); \
	} \
    } while(0)

SEXP attribute_hidden do_if(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP Cond, Stmt=R_NilValue;
    int vis=0;

    PROTECT(Cond = eval(CAR(args), rho));
    if (asLogicalNoNA(Cond, call))
	Stmt = CAR(CDR(args));
    else {
	if (length(args) > 2)
	    Stmt = CAR(CDR(CDR(args)));
	else
	    vis = 1;
    }
    if( !vis && RDEBUG(rho) && !BodyHasBraces(Stmt) && !R_GlobalContext->browserfinish) {
	SrcrefPrompt("debug", R_Srcref);
	PrintValue(Stmt);
	//--by GT--do_browser(call, op, R_NilValue, rho);
    }
    UNPROTECT(1);
    if( vis ) {
	R_Visible = FALSE; /* case of no 'else' so return invisible NULL */
	return Stmt;
    }
    return (eval(Stmt, rho));
}

static R_INLINE SEXP GET_BINDING_CELL(SEXP symbol, SEXP rho)
{
    if (rho == R_BaseEnv || rho == R_BaseNamespace)
	return R_NilValue;
    else {
	R_varloc_t loc = R_findVarLocInFrame(rho, symbol);
	return (! R_VARLOC_IS_NULL(loc)) ? loc.cell : R_NilValue;
    }
}

static R_INLINE Rboolean SET_BINDING_VALUE(SEXP loc, SEXP value) {
    /* This depends on the current implementation of bindings */
    if (loc != R_NilValue &&
	! BINDING_IS_LOCKED(loc) && ! IS_ACTIVE_BINDING(loc)) {
	if (CAR(loc) != value) {
	    SETCAR(loc, value);
	    if (MISSING(loc))
		SET_MISSING(loc, 0);
	}
	return TRUE;
    }
    else
	return FALSE;
}

SEXP attribute_hidden do_for(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    /* Need to declare volatile variables whose values are relied on
       after for_next or for_break longjmps and might change between
       the setjmp and longjmp calls. Theoretically this does not
       include n and bgn, but gcc -O2 -Wclobbered warns about these so
       to be safe we declare them volatile as well. */
    volatile int i = 0, n, bgn;
    volatile SEXP v, val, cell;
    int dbg, val_type;
    SEXP sym, body;
    RCNTXT cntxt;
    PROTECT_INDEX vpi;

    checkArity(op, args);
    sym = CAR(args);
    val = CADR(args);
    body = CADDR(args);

    if ( !isSymbol(sym) ) errorcall(call, _("non-symbol loop variable"));

    dbg = RDEBUG(rho);
    if (R_jit_enabled > 2 && !dbg && !R_disable_bytecode
	    && rho == R_GlobalEnv
	    && isUnmodifiedSpecSym(CAR(call), rho)
	    && R_compileAndExecute(call, rho))
	return R_NilValue;

    PROTECT(args);
    PROTECT(rho);
    PROTECT(val = eval(val, rho));

    /* deal with the case where we are iterating over a factor
       we need to coerce to character - then iterate */

    if ( inherits(val, "factor") ) {
	SEXP tmp = asCharacterFactor(val);
	UNPROTECT(1); /* val from above */
	PROTECT(val = tmp);
    }

    if (isList(val) || isNull(val))
	n = length(val);
    else
	n = LENGTH(val);

    val_type = TYPEOF(val);

    defineVar(sym, R_NilValue, rho);
    PROTECT(cell = GET_BINDING_CELL(sym, rho));
    bgn = BodyHasBraces(body);

    /* bump up NAMED count of sequence to avoid modification by loop code */
    INCREMENT_NAMED(val);
    INCREMENT_REFCNT(val);

    PROTECT_WITH_INDEX(v = R_NilValue, &vpi);

    begincontext(&cntxt, CTXT_LOOP, R_NilValue, rho, R_BaseEnv, R_NilValue,
		 R_NilValue);
    switch (SETJMP(cntxt.cjmpbuf)) {
    case CTXT_BREAK: goto for_break;
    case CTXT_NEXT: goto for_next;
    }
    for (i = 0; i < n; i++) {

	switch (val_type) {

	case EXPRSXP:
	case VECSXP:
	    /* make sure loop variable is not modified via other vars */
	    SET_NAMED(VECTOR_ELT(val, i), 2);
	    /* defineVar is used here and below rather than setVar in
	       case the loop code removes the variable. */
	    defineVar(sym, VECTOR_ELT(val, i), rho);
	    break;

	case LISTSXP:
	    /* make sure loop variable is not modified via other vars */
	    SET_NAMED(CAR(val), 2);
	    defineVar(sym, CAR(val), rho);
	    val = CDR(val);
	    break;

	default:

	    switch (val_type) {
	    case LGLSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		LOGICAL(v)[0] = LOGICAL(val)[i];
		break;
	    case INTSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		INTEGER(v)[0] = INTEGER(val)[i];
		break;
	    case REALSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		REAL(v)[0] = REAL(val)[i];
		break;
	    case CPLXSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		COMPLEX(v)[0] = COMPLEX(val)[i];
		break;
	    case STRSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		SET_STRING_ELT(v, 0, STRING_ELT(val, i));
		break;
	    case RAWSXP:
		ALLOC_LOOP_VAR(v, val_type, vpi);
		RAW(v)[0] = RAW(val)[i];
		break;
	    default:
		errorcall(call, _("invalid for() loop sequence"));
	    }
	    if (CAR(cell) == R_UnboundValue || ! SET_BINDING_VALUE(cell, v))
		defineVar(sym, v, rho);
	}
	if (!bgn && RDEBUG(rho) && !R_GlobalContext->browserfinish) {
	    SrcrefPrompt("debug", R_Srcref);
	    PrintValue(body);
	    //--by GT--do_browser(call, op, R_NilValue, rho);
	}
	eval(body, rho);

    for_next:
	; /* needed for strict ISO C compliance, according to gcc 2.95.2 */
    }
 for_break:
    endcontext(&cntxt);
    DECREMENT_REFCNT(val);
    UNPROTECT(5);
    SET_RDEBUG(rho, dbg);
    return R_NilValue;
}


SEXP attribute_hidden do_while(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int dbg;
    volatile int bgn;
    volatile SEXP body;
    RCNTXT cntxt;

    checkArity(op, args);

    dbg = RDEBUG(rho);
    if (R_jit_enabled > 2 && !dbg && !R_disable_bytecode
	    && rho == R_GlobalEnv
	    && isUnmodifiedSpecSym(CAR(call), rho)
	    && R_compileAndExecute(call, rho))
	return R_NilValue;

    body = CADR(args);
    bgn = BodyHasBraces(body);

    begincontext(&cntxt, CTXT_LOOP, R_NilValue, rho, R_BaseEnv, R_NilValue,
		 R_NilValue);
    if (SETJMP(cntxt.cjmpbuf) != CTXT_BREAK) {
	while (asLogicalNoNA(eval(CAR(args), rho), call)) {
	    if (RDEBUG(rho) && !bgn && !R_GlobalContext->browserfinish) {
		SrcrefPrompt("debug", R_Srcref);
		PrintValue(body);
		//--by GT--do_browser(call, op, R_NilValue, rho);
	    }
	    eval(body, rho);
	    if (RDEBUG(rho) && !R_GlobalContext->browserfinish) {
		SrcrefPrompt("debug", R_Srcref);
		Rprintf("(while) ");
		PrintValue(CAR(args));
		//--by GT--do_browser(call, op, R_NilValue, rho);
	    }
	}
    }
    endcontext(&cntxt);
    SET_RDEBUG(rho, dbg);
    return R_NilValue;
}


SEXP attribute_hidden do_repeat(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int dbg;
    volatile SEXP body;
    RCNTXT cntxt;

    checkArity(op, args);

    dbg = RDEBUG(rho);
    if (R_jit_enabled > 2 && !dbg && !R_disable_bytecode
	    && rho == R_GlobalEnv
	    && isUnmodifiedSpecSym(CAR(call), rho)
	    && R_compileAndExecute(call, rho))
	return R_NilValue;

    body = CAR(args);

    begincontext(&cntxt, CTXT_LOOP, R_NilValue, rho, R_BaseEnv, R_NilValue,
		 R_NilValue);
    if (SETJMP(cntxt.cjmpbuf) != CTXT_BREAK) {
	for (;;) {
	    eval(body, rho);
	}
    }
    endcontext(&cntxt);
    SET_RDEBUG(rho, dbg);
    return R_NilValue;
}


SEXP attribute_hidden NORET do_break(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    checkArity(op, args);
    findcontext(PRIMVAL(op), rho, R_NilValue);
}


SEXP attribute_hidden do_paren(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    checkArity(op, args);
    return CAR(args);
}

SEXP attribute_hidden do_begin(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP s = R_NilValue;
    if (args != R_NilValue) {
	SEXP srcrefs = getBlockSrcrefs(call);
	PROTECT(srcrefs);
	int i = 1;
	while (args != R_NilValue) {
	    PROTECT(R_Srcref = getSrcref(srcrefs, i++));
	    if (RDEBUG(rho) && !R_GlobalContext->browserfinish) {
		SrcrefPrompt("debug", R_Srcref);
		PrintValue(CAR(args));
		//--by GT--do_browser(call, op, R_NilValue, rho);
	    }
	    s = eval(CAR(args), rho);
	    UNPROTECT(1);
	    args = CDR(args);
	}
	R_Srcref = R_NilValue;
	UNPROTECT(1); /* srcrefs */
    }
    return s;
}


SEXP attribute_hidden NORET do_return(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP v;

    if (args == R_NilValue) /* zero arguments provided */
	v = R_NilValue;
    else if (CDR(args) == R_NilValue) /* one argument */
	v = eval(CAR(args), rho);
    else {
	v = R_NilValue; /* to avoid compiler warnings */
	errorcall(call, _("multi-argument returns are not permitted"));
    }

    findcontext(CTXT_BROWSER | CTXT_FUNCTION, rho, v);
}

/* Declared with a variable number of args in names.c */
SEXP attribute_hidden do_function(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP rval, srcref;

    if (TYPEOF(op) == PROMSXP) {
	op = forcePromise(op);
	SET_NAMED(op, 2);
    }
    if (length(args) < 2) WrongArgCount("function");
    CheckFormals(CAR(args));
    rval = mkCLOSXP(CAR(args), CADR(args), rho);
    srcref = CADDR(args);
    if (!isNull(srcref)) setAttrib(rval, R_SrcrefSymbol, srcref);
    return rval;
}


/*
 *  Assignments for complex LVAL specifications. This is the stuff that
 *  nightmares are made of ...	Note that "evalseq" preprocesses the LHS
 *  of an assignment.  Given an expression, it builds a list of partial
 *  values for the exression.  For example, the assignment x$a[3] <- 10
 *  with LHS x$a[3] yields the (improper) list:
 *
 *	 (eval(x$a[3])	eval(x$a)  eval(x)  .  x)
 *
 *  (Note the terminating symbol).  The partial evaluations are carried
 *  out efficiently using previously computed components.
 */

/*
  For complex superassignment  x[y==z]<<-w
  we want x required to be nonlocal, y,z, and w permitted to be local or
  nonlocal.
*/

static SEXP evalseq(SEXP expr, SEXP rho, int forcelocal,  R_varloc_t tmploc)
{
    SEXP val, nval, nexpr;
    if (isNull(expr))
	error(_("invalid (NULL) left side of assignment"));
    if (isSymbol(expr)) {
	PROTECT(expr);
	if(forcelocal) {
	    nval = EnsureLocal(expr, rho);
	}
	else {/* now we are down to the target symbol */
	  nval = eval(expr, ENCLOS(rho));
	}
	if (MAYBE_SHARED(nval))
	    nval = shallow_duplicate(nval);
	UNPROTECT(1);
	return CONS_NR(nval, expr);
    }
    else if (isLanguage(expr)) {
	PROTECT(expr);
	PROTECT(val = evalseq(CADR(expr), rho, forcelocal, tmploc));
	R_SetVarLocValue(tmploc, CAR(val));
	PROTECT(nexpr = LCONS(R_GetVarLocSymbol(tmploc), CDDR(expr)));
	PROTECT(nexpr = LCONS(CAR(expr), nexpr));
	nval = eval(nexpr, rho);
	/* duplicate nval if it might be shared _or_ if the container,
	   CAR(val), has become possibly shared by going through a
	   closure.  This is taken to indicate that the corresponding
	   replacement function might be a closure and will need to
	   see an unmodified LHS value. This heuristic fails if the
	   accessor function called here is not a closure but the
	   replacement function is. */
	if (MAYBE_REFERENCED(nval) &&
	    (MAYBE_SHARED(nval) || MAYBE_SHARED(CAR(val))))
	    nval = shallow_duplicate(nval);
	UNPROTECT(4);
	return CONS_NR(nval, val);
    }
    else error(_("target of assignment expands to non-language object"));
    return R_NilValue;	/*NOTREACHED*/
}

/* Main entry point for complex assignments */
/* We have checked to see that CAR(args) is a LANGSXP */

static const char * const asym[] = {":=", "<-", "<<-", "="};
#define NUM_ASYM (sizeof(asym) / sizeof(char *))
static SEXP asymSymbol[NUM_ASYM];

static SEXP R_ReplaceFunsTable = NULL;
static SEXP R_SubsetSym = NULL;
static SEXP R_SubassignSym = NULL;
static SEXP R_Subset2Sym = NULL;
static SEXP R_Subassign2Sym = NULL;
static SEXP R_DollarGetsSymbol = NULL;
static SEXP R_AssignSym = NULL;

void attribute_hidden R_initAssignSymbols(void)
{
    for (int i = 0; i < NUM_ASYM; i++)
	asymSymbol[i] = install(asym[i]);

    R_ReplaceFunsTable = R_NewHashedEnv(R_EmptyEnv, ScalarInteger(1099));
    R_PreserveObject(R_ReplaceFunsTable);

    R_SubsetSym = install("[");
    R_SubassignSym = install("[<-");
    R_Subset2Sym = install("[[");
    R_Subassign2Sym = install("[[<-");
    R_DollarGetsSymbol = install("$<-");
    R_valueSym = install("value");
    R_AssignSym = install("<-");
}

static R_INLINE SEXP lookupAssignFcnSymbol(SEXP fun)
{
    return findVarInFrame(R_ReplaceFunsTable, fun);
}

static void enterAssignFcnSymbol(SEXP fun, SEXP val)
{
    defineVar(fun, val, R_ReplaceFunsTable);
}

static void tmp_cleanup(void *data)
{
    unbindVar(R_TmpvalSymbol, (SEXP) data);
}

/* This macro stores the current assignment target in the saved
   binding location. It duplicates if necessary to make sure
   replacement functions are always called with a target with NAMED ==
   1. The SET_CAR is intended to protect against possible GC in
   R_SetVarLocValue; this might occur it the binding is an active
   binding. */
#define SET_TEMPVARLOC_FROM_CAR(loc, lhs) do { \
	SEXP __lhs__ = (lhs); \
	SEXP __v__ = CAR(__lhs__); \
	if (MAYBE_SHARED(__v__)) { \
	    __v__ = shallow_duplicate(__v__); \
	    SET_NAMED(__v__, 1); \
	    SETCAR(__lhs__, __v__); \
	} \
	R_SetVarLocValue(loc, __v__); \
    } while(0)

/* This macro makes sure the RHS NAMED value is 0 or 2. This is
   necessary to make sure the RHS value returned by the assignment
   expression is correct when the RHS value is part of the LHS
   object. */
#define FIXUP_RHS_NAMED(r) do { \
	SEXP __rhs__ = (r); \
	if (NAMED(__rhs__) && NAMED(__rhs__) <= 1) \
	    SET_NAMED(__rhs__, 2); \
    } while (0)

#define ASSIGNBUFSIZ 32
static SEXP installAssignFcnSymbol(SEXP fun)
{
    char buf[ASSIGNBUFSIZ];

    /* install the symbol */
    if(strlen(CHAR(PRINTNAME(fun))) + 3 > ASSIGNBUFSIZ)
	error(_("overlong name in '%s'"), EncodeChar(PRINTNAME(fun)));
    snprintf(buf, ASSIGNBUFSIZ-1, "%s<-", CHAR(PRINTNAME(fun)));
    SEXP val = install(buf);

    enterAssignFcnSymbol(fun, val);
    return val;
}

static R_INLINE SEXP getAssignFcnSymbol(SEXP fun)
{
    /* handle [<-, [[<-, and $<- efficiently */
    if (fun == R_SubsetSym)
	return R_SubassignSym;
    else if (fun == R_Subset2Sym)
	return R_Subassign2Sym;
    else if (fun == R_DollarSymbol)
	return R_DollarGetsSymbol;

    /* look up in the replacement functions table */
    SEXP val = lookupAssignFcnSymbol(fun);
    if (val != R_UnboundValue)
	return val;

    /* instal symbol, entern in table,  and return */
    return installAssignFcnSymbol(fun);
}

static R_INLINE SEXP mkRHSPROMISE(SEXP expr, SEXP rhs)
{
    return R_mkEVPROMISE_NR(expr, rhs);
}

static SEXP applydefine(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP expr, lhs, rhs, saverhs, tmp, afun, rhsprom;
    R_varloc_t tmploc;
    RCNTXT cntxt;
    int nprot;

    expr = CAR(args);

    /*  It's important that the rhs get evaluated first because
	assignment is right associative i.e.  a <- b <- c is parsed as
	a <- (b <- c).  */

    PROTECT(saverhs = rhs = eval(CADR(args), rho));
    INCREMENT_REFCNT(saverhs);

    /*  FIXME: We need to ensure that this works for hashed
	environments.  This code only works for unhashed ones.  the
	syntax error here is a deliberate marker so I don't forget that
	this needs to be done.  The code used in "missing" will help
	here.  */

    /*  FIXME: This strategy will not work when we are working in the
	data frame defined by the system hash table.  The structure there
	is different.  Should we special case here?  */

    /*  We need a temporary variable to hold the intermediate values
	in the computation.  For efficiency reasons we record the
	location where this variable is stored.  We need to protect
	the location in case the biding is removed from its
	environment by user code or an assignment within the
	assignment arguments */

    /*  There are two issues with the approach here:

	    A complex assignment within a complex assignment, like
	    f(x, y[] <- 1) <- 3, can cause the value temporary
	    variable for the outer assignment to be overwritten and
	    then removed by the inner one.  This could be addressed by
	    using multiple temporaries or using a promise for this
	    variable as is done for the RHS.  Printing of the
	    replacement function call in error messages might then need
	    to be adjusted.

	    With assignments of the form f(g(x, z), y) <- w the value
	    of 'z' will be computed twice, once for a call to g(x, z)
	    and once for the call to the replacement function g<-.  It
	    might be possible to address this by using promises.
	    Using more temporaries would not work as it would mess up
	    replacement functions that use substitute and/or
	    nonstandard evaluation (and there are packages that do
	    that -- igraph is one).

	    LT */

    FIXUP_RHS_NAMED(rhs);

    if (rho == R_BaseNamespace)
	errorcall(call, _("cannot do complex assignments in base namespace"));
    if (rho == R_BaseEnv)
	errorcall(call, _("cannot do complex assignments in base environment"));
    defineVar(R_TmpvalSymbol, R_NilValue, rho);
    tmploc = R_findVarLocInFrame(rho, R_TmpvalSymbol);
    PROTECT(tmploc.cell);
    DISABLE_REFCNT(tmploc.cell);
    DECREMENT_REFCNT(CDR(tmploc.cell));

    /* Now set up a context to remove it when we are done, even in the
     * case of an error.  This all helps error() provide a better call.
     */
    begincontext(&cntxt, CTXT_CCODE, call, R_BaseEnv, R_BaseEnv,
		 R_NilValue, R_NilValue);
    cntxt.cend = &tmp_cleanup;
    cntxt.cenddata = rho;

    /*  Do a partial evaluation down through the LHS. */
    lhs = evalseq(CADR(expr), rho,
		  PRIMVAL(op)==1 || PRIMVAL(op)==3, tmploc);

    PROTECT(lhs);
    PROTECT(rhsprom = mkRHSPROMISE(CADR(args), rhs));

    while (isLanguage(CADR(expr))) {
	nprot = 1; /* the PROTECT of rhs below from this iteration */
	if (TYPEOF(CAR(expr)) == SYMSXP)
	    tmp = getAssignFcnSymbol(CAR(expr));
	else {
	    /* check for and handle assignments of the form
	       foo::bar(x) <- y or foo:::bar(x) <- y */
	    tmp = R_NilValue; /* avoid uninitialized variable warnings */
	    if (TYPEOF(CAR(expr)) == LANGSXP &&
		(CAR(CAR(expr)) == R_DoubleColonSymbol ||
		 CAR(CAR(expr)) == R_TripleColonSymbol) &&
		length(CAR(expr)) == 3 && TYPEOF(CADDR(CAR(expr))) == SYMSXP) {
		tmp = getAssignFcnSymbol(CADDR(CAR(expr)));
		PROTECT(tmp = lang3(CAAR(expr), CADR(CAR(expr)), tmp));
		nprot++;
	    }
	    else
		error(_("invalid function in complex assignment"));
	}
	SET_TEMPVARLOC_FROM_CAR(tmploc, lhs);
	PROTECT(rhs = replaceCall(tmp, R_TmpvalSymbol, CDDR(expr), rhsprom));
	rhs = eval(rhs, rho);
	SET_PRVALUE(rhsprom, rhs);
	SET_PRCODE(rhsprom, rhs); /* not good but is what we have been doing */
	UNPROTECT(nprot);
	lhs = CDR(lhs);
	expr = CADR(expr);
    }
    nprot = 5; /* the commont case */
    if (TYPEOF(CAR(expr)) == SYMSXP)
	afun = getAssignFcnSymbol(CAR(expr));
    else {
	/* check for and handle assignments of the form
	   foo::bar(x) <- y or foo:::bar(x) <- y */
	afun = R_NilValue; /* avoid uninitialized variable warnings */
	if (TYPEOF(CAR(expr)) == LANGSXP &&
	    (CAR(CAR(expr)) == R_DoubleColonSymbol ||
	     CAR(CAR(expr)) == R_TripleColonSymbol) &&
	    length(CAR(expr)) == 3 && TYPEOF(CADDR(CAR(expr))) == SYMSXP) {
	    afun = getAssignFcnSymbol(CADDR(CAR(expr)));
	    PROTECT(afun = lang3(CAAR(expr), CADR(CAR(expr)), afun));
	    nprot++;
	}
	else
	    error(_("invalid function in complex assignment"));
    }
    SET_TEMPVARLOC_FROM_CAR(tmploc, lhs);
    PROTECT(expr = assignCall(asymSymbol[PRIMVAL(op)], CDR(lhs),
			      afun, R_TmpvalSymbol, CDDR(expr), rhsprom));
    expr = eval(expr, rho);
    endcontext(&cntxt); /* which does not run the remove */
    UNPROTECT(nprot);
    unbindVar(R_TmpvalSymbol, rho);
#ifdef OLD_RHS_NAMED
    /* we do not duplicate the value, so to be conservative mark the
       value as NAMED = 2 */
    SET_NAMED(saverhs, 2);
#else
    INCREMENT_NAMED(saverhs);
#endif
    DECREMENT_REFCNT(saverhs);
    return saverhs;
}

/* Defunct in 1.5.0
SEXP attribute_hidden do_alias(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    checkArity(op,args);
    Rprintf(".Alias is deprecated; there is no replacement \n");
    SET_NAMED(CAR(args), 0);
    return CAR(args);
}
*/

/*  Assignment in its various forms  */

SEXP attribute_hidden do_set(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP lhs, rhs;

    if (args == R_NilValue ||
	CDR(args) == R_NilValue ||
	CDDR(args) != R_NilValue)
	WrongArgCount(asym[PRIMVAL(op)]);

    lhs = CAR(args);

    switch (TYPEOF(lhs)) {
    case STRSXP:
	lhs = installTrChar(STRING_ELT(lhs, 0));
	/* fall through */
    case SYMSXP:
	rhs = eval(CADR(args), rho);
	INCREMENT_NAMED(rhs);
	if (PRIMVAL(op) == 2)                       /* <<- */
	    setVar(lhs, rhs, ENCLOS(rho));
	else                                        /* <-, = */
	    defineVar(lhs, rhs, rho);
	R_Visible = FALSE;
	return rhs;
    case LANGSXP:
	R_Visible = FALSE;
	return applydefine(call, op, args, rho);
    default:
	errorcall(call, _("invalid (do_set) left-hand side to assignment"));
    }

    return R_NilValue;/*NOTREACHED*/
}


/* Evaluate each expression in "el" in the environment "rho".  This is
   a naturally recursive algorithm, but we use the iterative form below
   because it is does not cause growth of the pointer protection stack,
   and because it is a little more efficient.
*/

#define COPY_TAG(to, from) do { \
  SEXP __tag__ = TAG(from); \
  if (__tag__ != R_NilValue) SET_TAG(to, __tag__); \
} while (0)

/* Used in eval and applyMethod (object.c) for builtin primitives,
   do_internal (names.c) for builtin .Internals
   and in evalArgs.

   'n' is the number of arguments already evaluated and hence not
   passed to evalArgs and hence to here.
 */
SEXP attribute_hidden evalList(SEXP el, SEXP rho, SEXP call, int n)
{
    SEXP head, tail, ev, h;

    head = R_NilValue;
    tail = R_NilValue; /* to prevent uninitialized variable warnings */

    while (el != R_NilValue) {
	n++;

	if (CAR(el) == R_DotsSymbol) {
	    /* If we have a ... symbol, we look to see what it is bound to.
	     * If its binding is Null (i.e. zero length)
	     *	we just ignore it and return the cdr with all its expressions evaluated;
	     * if it is bound to a ... list of promises,
	     *	we force all the promises and then splice
	     *	the list of resulting values into the return value.
	     * Anything else bound to a ... symbol is an error
	     */
	    PROTECT(h = findVar(CAR(el), rho));
	    if (TYPEOF(h) == DOTSXP || h == R_NilValue) {
		while (h != R_NilValue) {
		    ev = CONS_NR(eval(CAR(h), rho), R_NilValue);
		    if (head==R_NilValue) {
			UNPROTECT(1); /* h */
			PROTECT(head = ev);
			PROTECT(h); /* put current h on top of protect stack */
		    }
		    else
			SETCDR(tail, ev);
		    COPY_TAG(ev, h);
		    tail = ev;
		    h = CDR(h);
		}
	    }
	    else if (h != R_MissingArg)
		error(_("'...' used in an incorrect context"));
	    UNPROTECT(1); /* h */
	} else if (CAR(el) == R_MissingArg) {
	    /* It was an empty element: most likely get here from evalArgs
	       which may have been called on part of the args. */
	    errorcall(call, _("argument %d is empty"), n);
#ifdef CHECK_IS_MISSING_IN_evalList
	    /* Radford Newl drops this R_isMissing check in pqR in
	       03-zap-isMissing (but it seems to creep in again later
	       with helper thread stuff?)  as it takes quite a bit of
	       time (essentially the equivalent of evaluating the
	       symbol, but maybe not as efficiently as eval) and only
	       serves to change the error message, not always for the
	       better. Also, the byte code interpreter does not do
	       this, so dropping this makes compiled and interreted
	       cod emore consistent. */
	} else if (isSymbol(CAR(el)) && R_isMissing(CAR(el), rho)) {
	    /* It was missing */
	    errorcall_cpy(call,
	                  _("'%s' is missing"),
	                  EncodeChar(PRINTNAME(CAR(el))));
#endif
	} else {
	    ev = CONS_NR(eval(CAR(el), rho), R_NilValue);
	    if (head==R_NilValue)
		PROTECT(head = ev);
	    else
		SETCDR(tail, ev);
	    COPY_TAG(ev, el);
	    tail = ev;
	}
	el = CDR(el);
    }

    if (head!=R_NilValue)
	UNPROTECT(1);

    return head;

} /* evalList() */


/* A slight variation of evaluating each expression in "el" in "rho". */

/* used in evalArgs, arithmetic.c, seq.c */
SEXP attribute_hidden evalListKeepMissing(SEXP el, SEXP rho)
{
    SEXP head, tail, ev, h;

    head = R_NilValue;
    tail = R_NilValue; /* to prevent uninitialized variable warnings */

    while (el != R_NilValue) {

	/* If we have a ... symbol, we look to see what it is bound to.
	 * If its binding is Null (i.e. zero length)
	 *	we just ignore it and return the cdr with all its expressions evaluated;
	 * if it is bound to a ... list of promises,
	 *	we force all the promises and then splice
	 *	the list of resulting values into the return value.
	 * Anything else bound to a ... symbol is an error
	*/
	if (CAR(el) == R_DotsSymbol) {
	    PROTECT(h = findVar(CAR(el), rho));
	    if (TYPEOF(h) == DOTSXP || h == R_NilValue) {
		while (h != R_NilValue) {
		    if (CAR(h) == R_MissingArg)
			ev = CONS_NR(R_MissingArg, R_NilValue);
		    else
			ev = CONS_NR(eval(CAR(h), rho), R_NilValue);
		    if (head == R_NilValue) {
			UNPROTECT(1); /* h */
			PROTECT(head = ev);
			PROTECT(h);
		    } else
			SETCDR(tail, ev);
		    COPY_TAG(ev, h);
		    tail = ev;
		    h = CDR(h);
		}
	    }
	    else if(h != R_MissingArg)
		error(_("'...' used in an incorrect context"));
	    UNPROTECT(1); /* h */
	}
	else {
	    if (CAR(el) == R_MissingArg ||
		(isSymbol(CAR(el)) && R_isMissing(CAR(el), rho)))
		ev = CONS_NR(R_MissingArg, R_NilValue);
	    else
		ev = CONS_NR(eval(CAR(el), rho), R_NilValue);
	    if (head==R_NilValue)
		PROTECT(head = ev);
	    else
		SETCDR(tail, ev);
	    COPY_TAG(ev, el);
	    tail = ev;
	}
	el = CDR(el);
    }

    if (head!=R_NilValue)
	UNPROTECT(1);

    return head;
}


/* Create a promise to evaluate each argument.	Although this is most */
/* naturally attacked with a recursive algorithm, we use the iterative */
/* form below because it is does not cause growth of the pointer */
/* protection stack, and because it is a little more efficient. */

SEXP attribute_hidden promiseArgs(SEXP el, SEXP rho)
{
    SEXP ans, h, tail;

    PROTECT(ans = tail = CONS(R_NilValue, R_NilValue));

    while(el != R_NilValue) {

	/* If we have a ... symbol, we look to see what it is bound to.
	 * If its binding is Null (i.e. zero length)
	 * we just ignore it and return the cdr with all its
	 * expressions promised; if it is bound to a ... list
	 * of promises, we repromise all the promises and then splice
	 * the list of resulting values into the return value.
	 * Anything else bound to a ... symbol is an error
	 */

	/* Is this double promise mechanism really needed? */

	if (CAR(el) == R_DotsSymbol) {
	    PROTECT(h = findVar(CAR(el), rho));
	    if (TYPEOF(h) == DOTSXP || h == R_NilValue) {
		while (h != R_NilValue) {
		    SETCDR(tail, CONS(mkPROMISE(CAR(h), rho), R_NilValue));
		    tail = CDR(tail);
		    COPY_TAG(tail, h);
		    h = CDR(h);
		}
	    }
	    else if (h != R_MissingArg)
		error(_("'...' used in an incorrect context"));
	    UNPROTECT(1); /* h */
	}
	else if (CAR(el) == R_MissingArg) {
	    SETCDR(tail, CONS(R_MissingArg, R_NilValue));
	    tail = CDR(tail);
	    COPY_TAG(tail, el);
	}
	else {
	    SETCDR(tail, CONS(mkPROMISE(CAR(el), rho), R_NilValue));
	    tail = CDR(tail);
	    COPY_TAG(tail, el);
	}
	el = CDR(el);
    }
    UNPROTECT(1);
    return CDR(ans);
}


/* Check that each formal is a symbol */

/* used in coerce.c */
void attribute_hidden CheckFormals(SEXP ls)
{
    if (isList(ls)) {
	for (; ls != R_NilValue; ls = CDR(ls))
	    if (TYPEOF(TAG(ls)) != SYMSXP)
		goto err;
	return;
    }
 err:
    error(_("invalid formal argument list for \"function\""));
}


static SEXP VectorToPairListNamed(SEXP x)
{
    SEXP xptr, xnew, xnames;
    int i, len = 0, named;
    const void *vmax = vmaxget();

    PROTECT(x);
    PROTECT(xnames = getAttrib(x, R_NamesSymbol)); /* isn't this protected via x? */
    named = (xnames != R_NilValue);
    if(named)
	for (i = 0; i < length(x); i++)
	    if (CHAR(STRING_ELT(xnames, i))[0] != '\0') len++;

    if(len) {
	PROTECT(xnew = allocList(len));
	xptr = xnew;
	for (i = 0; i < length(x); i++) {
	    if (CHAR(STRING_ELT(xnames, i))[0] != '\0') {
		SETCAR(xptr, VECTOR_ELT(x, i));
		SET_TAG(xptr, installTrChar(STRING_ELT(xnames, i)));
		xptr = CDR(xptr);
	    }
	}
	UNPROTECT(1);
    } else xnew = allocList(0);
    UNPROTECT(2);
    vmaxset(vmax);
    return xnew;
}

#define simple_as_environment(arg) (IS_S4_OBJECT(arg) && (TYPEOF(arg) == S4SXP) ? R_getS4DataSlot(arg, ENVSXP) : R_NilValue)

/* "eval": Evaluate the first argument
   in the environment specified by the second argument. */

SEXP attribute_hidden do_eval(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP encl, x, xptr;
    volatile SEXP expr, env, tmp;

    int frame;
    RCNTXT cntxt;

    checkArity(op, args);
    expr = CAR(args);
    env = CADR(args);
    encl = CADDR(args);
    SEXPTYPE tEncl = TYPEOF(encl);
    if (isNull(encl)) {
	/* This is supposed to be defunct, but has been kept here
	   (and documented as such) */
	encl = R_BaseEnv;
    } else if ( !isEnvironment(encl) &&
		!isEnvironment((encl = simple_as_environment(encl))) ) {
	error(_("invalid '%s' argument of type '%s'"),
	      "enclos", type2char(tEncl));
    }
    if(IS_S4_OBJECT(env) && (TYPEOF(env) == S4SXP))
	env = R_getS4DataSlot(env, ANYSXP); /* usually an ENVSXP */
    switch(TYPEOF(env)) {
    case NILSXP:
	env = encl;     /* so eval(expr, NULL, encl) works */
	/* falls through */
    case ENVSXP:
	PROTECT(env);	/* so we can unprotect 2 at the end */
	break;
    case LISTSXP:
	/* This usage requires all the pairlist to be named */
	env = NewEnvironment(R_NilValue, duplicate(CADR(args)), encl);
	PROTECT(env);
	break;
    case VECSXP:
	/* PR#14035 */
	x = VectorToPairListNamed(CADR(args));
	for (xptr = x ; xptr != R_NilValue ; xptr = CDR(xptr))
	    SET_NAMED(CAR(xptr) , 2);
	env = NewEnvironment(R_NilValue, x, encl);
	PROTECT(env);
	break;
    case INTSXP:
    case REALSXP:
	if (length(env) != 1)
	    error(_("numeric 'envir' arg not of length one"));
	frame = asInteger(env);
	if (frame == NA_INTEGER)
	    error(_("invalid '%s' argument of type '%s'"),
		  "envir", type2char(TYPEOF(env)));
	PROTECT(env = R_sysframe(frame, R_GlobalContext));
	break;
    default:
	error(_("invalid '%s' argument of type '%s'"),
	      "envir", type2char(TYPEOF(env)));
    }

    /* isLanguage include NILSXP, and that does not need to be
       evaluated
    if (isLanguage(expr) || isSymbol(expr) || isByteCode(expr)) { */
    if (TYPEOF(expr) == LANGSXP || TYPEOF(expr) == SYMSXP || isByteCode(expr)) {
	PROTECT(expr);
	begincontext(&cntxt, CTXT_RETURN, R_GlobalContext->call,
	             env, rho, args, op);
	if (!SETJMP(cntxt.cjmpbuf))
	    expr = eval(expr, env);
	else {
	    expr = R_ReturnedValue;
	    if (expr == R_RestartToken) {
		cntxt.callflag = CTXT_RETURN;  /* turn restart off */
		error(_("restarts not supported in 'eval'"));
	    }
	}
	UNPROTECT(1);
	PROTECT(expr);
	endcontext(&cntxt);
	UNPROTECT(1);
    }
    else if (TYPEOF(expr) == EXPRSXP) {
	int i, n;
	SEXP srcrefs = getBlockSrcrefs(expr);
	PROTECT(expr);
	n = LENGTH(expr);
	tmp = R_NilValue;
	begincontext(&cntxt, CTXT_RETURN, R_GlobalContext->call,
	             env, rho, args, op);
	if (!SETJMP(cntxt.cjmpbuf))
	    for(i = 0 ; i < n ; i++) {
		R_Srcref = getSrcref(srcrefs, i);
		tmp = eval(VECTOR_ELT(expr, i), env);
	    }
	else {
	    tmp = R_ReturnedValue;
	    if (tmp == R_RestartToken) {
		cntxt.callflag = CTXT_RETURN;  /* turn restart off */
		error(_("restarts not supported in 'eval'"));
	    }
	}
	UNPROTECT(1);
	PROTECT(tmp);
	endcontext(&cntxt);
	UNPROTECT(1);
	expr = tmp;
    }
    else if( TYPEOF(expr) == PROMSXP ) {
	expr = eval(expr, rho);
    } /* else expr is returned unchanged */
    UNPROTECT(1);
    return expr;
}

/* This is a special .Internal */
SEXP attribute_hidden do_withVisible(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP x, nm, ret;

    checkArity(op, args);
    x = CAR(args);
    x = eval(x, rho);
    PROTECT(x);
    PROTECT(ret = allocVector(VECSXP, 2));
    PROTECT(nm = allocVector(STRSXP, 2));
    SET_STRING_ELT(nm, 0, mkChar("value"));
    SET_STRING_ELT(nm, 1, mkChar("visible"));
    SET_VECTOR_ELT(ret, 0, x);
    SET_VECTOR_ELT(ret, 1, ScalarLogical(R_Visible));
    setAttrib(ret, R_NamesSymbol, nm);
    UNPROTECT(3);
    return ret;
}

/* This is a special .Internal */
SEXP attribute_hidden do_recall(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    RCNTXT *cptr;
    SEXP s, ans ;
    cptr = R_GlobalContext;
    /* get the args supplied */
    while (cptr != NULL) {
	if (cptr->callflag == CTXT_RETURN && cptr->cloenv == rho)
	    break;
	cptr = cptr->nextcontext;
    }
    if (cptr != NULL) {
	args = cptr->promargs;
    }
    /* get the env recall was called from */
    s = R_GlobalContext->sysparent;
    while (cptr != NULL) {
	if (cptr->callflag == CTXT_RETURN && cptr->cloenv == s)
	    break;
	cptr = cptr->nextcontext;
    }
    if (cptr == NULL)
	error(_("'Recall' called from outside a closure"));

    /* If the function has been recorded in the context, use it
       otherwise search for it by name or evaluate the expression
       originally used to get it.
    */
    if (cptr->callfun != R_NilValue)
	PROTECT(s = cptr->callfun);
    else if( TYPEOF(CAR(cptr->call)) == SYMSXP)
	PROTECT(s = findFun(CAR(cptr->call), cptr->sysparent));
    else
	PROTECT(s = eval(CAR(cptr->call), cptr->sysparent));
    if (TYPEOF(s) != CLOSXP)
	error(_("'Recall' called from outside a closure"));
    ans = applyClosure(cptr->call, s, args, cptr->sysparent, R_NilValue);
    UNPROTECT(1);
    return ans;
}


static SEXP evalArgs(SEXP el, SEXP rho, int dropmissing, SEXP call, int n)
{
    if(dropmissing) return evalList(el, rho, call, n);
    else return evalListKeepMissing(el, rho);
}


/* A version of DispatchOrEval that checks for possible S4 methods for
 * any argument, not just the first.  Used in the code for `c()` in do_c()
 * and previously used in the code for `[` in do_subset.
 * Differs in that all arguments are evaluated
 * immediately, rather than after the call to R_possible_dispatch.
 */
attribute_hidden
int DispatchAnyOrEval(SEXP call, SEXP op, const char *generic, SEXP args,
		      SEXP rho, SEXP *ans, int dropmissing, int argsevald)
{
    if(R_has_methods(op)) {
	SEXP argValue, el,  value;
	/* Rboolean hasS4 = FALSE; */
	int nprotect = 0, dispatch;
	if(!argsevald) {
	    PROTECT(argValue = evalArgs(args, rho, dropmissing, call, 0));
	    nprotect++;
	    argsevald = TRUE;
	}
	else argValue = args;
	for(el = argValue; el != R_NilValue; el = CDR(el)) {
	    if(IS_S4_OBJECT(CAR(el))) {
		value = R_possible_dispatch(call, op, argValue, rho, TRUE);
		if(value) {
		    *ans = value;
		    UNPROTECT(nprotect);
		    return 1;
		}
		else break;
	    }
	}
	 /* else, use the regular DispatchOrEval, but now with evaluated args */
	dispatch = DispatchOrEval(call, op, generic, argValue, rho, ans, dropmissing, argsevald);
	UNPROTECT(nprotect);
	return dispatch;
    }
    return DispatchOrEval(call, op, generic, args, rho, ans, dropmissing, argsevald);
}


/* DispatchOrEval is used in internal functions which dispatch to
 * object methods (e.g. "[" or "[[").  The code either builds promises
 * and dispatches to the appropriate method, or it evaluates the
 * (unevaluated) arguments it comes in with and returns them so that
 * the generic built-in C code can continue.

 * To call this an ugly hack would be to insult all existing ugly hacks
 * at large in the world.
 */
attribute_hidden
int DispatchOrEval(SEXP call, SEXP op, const char *generic, SEXP args,
		   SEXP rho, SEXP *ans, int dropmissing, int argsevald)
{
/* DispatchOrEval is called very frequently, most often in cases where
   no dispatching is needed and the isObject or the string-based
   pre-test fail.  To avoid degrading performance it is therefore
   necessary to avoid creating promises in these cases.  The pre-test
   does require that we look at the first argument, so that needs to
   be evaluated.  The complicating factor is that the first argument
   might come in with a "..." and that there might be other arguments
   in the "..." as well.  LT */

    SEXP x = R_NilValue;
    int dots = FALSE, nprotect = 0;;

    if( argsevald )
	{PROTECT(x = CAR(args)); nprotect++;}
    else {
	/* Find the object to dispatch on, dropping any leading
	   ... arguments with missing or empty values.  If there are no
	   arguments, R_NilValue is used. */
	for (; args != R_NilValue; args = CDR(args)) {
	    if (CAR(args) == R_DotsSymbol) {
		SEXP h = findVar(R_DotsSymbol, rho);
		if (TYPEOF(h) == DOTSXP) {
#ifdef DODO
		    /**** any self-evaluating value should be OK; this
			  is used in byte compiled code. LT */
		    /* just a consistency check */
		    if (TYPEOF(CAR(h)) != PROMSXP)
			error(_("value in '...' is not a promise"));
#endif
		    dots = TRUE;
		    x = eval(CAR(h), rho);
		    break;
		}
		else if (h != R_NilValue && h != R_MissingArg)
		    error(_("'...' used in an incorrect context"));
	    }
	    else {
		dots = FALSE;
		x = eval(CAR(args), rho);
		break;
	    }
	}
	PROTECT(x); nprotect++;
    }
	/* try to dispatch on the object */
    if( isObject(x) ) {
	char *pt;
	/* Try for formal method. */
	if(IS_S4_OBJECT(x) && R_has_methods(op)) {
	    SEXP value, argValue;
	    /* create a promise to pass down to applyClosure  */
	    if(!argsevald) {
		argValue = promiseArgs(args, rho);
		SET_PRVALUE(CAR(argValue), x);
	    } else argValue = args;
	    PROTECT(argValue); nprotect++;
	    /* This means S4 dispatch */
	    value = R_possible_dispatch(call, op, argValue, rho, TRUE);
	    if(value) {
		*ans = value;
		UNPROTECT(nprotect);
		return 1;
	    }
	    else {
		/* go on, with the evaluated args.  Not guaranteed to have
		   the same semantics as if the arguments were not
		   evaluated, in special cases (e.g., arg values that are
		   LANGSXP).
		   The use of the promiseArgs is supposed to prevent
		   multiple evaluation after the call to possible_dispatch.
		*/
		if (dots)
		    PROTECT(argValue = evalArgs(argValue, rho, dropmissing,
						call, 0));
		else {
		    PROTECT(argValue = CONS_NR(x, evalArgs(CDR(argValue), rho,
							   dropmissing, call, 1)));
		    SET_TAG(argValue, CreateTag(TAG(args)));
		}
		nprotect++;
		args = argValue;
		argsevald = 1;
	    }
	}
	if (TYPEOF(CAR(call)) == SYMSXP)
	    pt = Rf_strrchr(CHAR(PRINTNAME(CAR(call))), '.');
	else
	    pt = NULL;

	if (pt == NULL || strcmp(pt,".default")) {
	    RCNTXT cntxt;
	    SEXP pargs, rho1;
	    PROTECT(pargs = promiseArgs(args, rho)); nprotect++;
	    /* The context set up here is needed because of the way
	       usemethod() is written.  DispatchGroup() repeats some
	       internal usemethod() code and avoids the need for a
	       context; perhaps the usemethod() code should be
	       refactored so the contexts around the usemethod() calls
	       in this file can be removed.

	       Using rho for current and calling environment can be
	       confusing for things like sys.parent() calls captured
	       in promises (Gabor G had an example of this).  Also,
	       since the context is established without a SETJMP using
	       an R-accessible environment allows a segfault to be
	       triggered (by something very obscure, but still).
	       Hence here and in the other usemethod() uses below a
	       new environment rho1 is created and used.  LT */
	    PROTECT(rho1 = NewEnvironment(R_NilValue, R_NilValue, rho)); nprotect++;
	    SET_PRVALUE(CAR(pargs), x);
	    begincontext(&cntxt, CTXT_RETURN, call, rho1, rho, pargs, op);
	    if(usemethod(generic, x, call, pargs, rho1, rho, R_BaseEnv, ans))
	    {
		endcontext(&cntxt);
		UNPROTECT(nprotect);
		return 1;
	    }
	    endcontext(&cntxt);
	    DECREMENT_REFCNT(x);
	}
    }
    if(!argsevald) {
	if (dots)
	    /* The first call argument was ... and may contain more than the
	       object, so it needs to be evaluated here.  The object should be
	       in a promise, so evaluating it again should be no problem. */
	    *ans = evalArgs(args, rho, dropmissing, call, 0);
	else {
	    PROTECT(*ans = CONS_NR(x, evalArgs(CDR(args), rho, dropmissing, call, 1)));
	    SET_TAG(*ans, CreateTag(TAG(args)));
	    UNPROTECT(1);
	}
    }
    else *ans = args;
    UNPROTECT(nprotect);
    return 0;
}

static R_INLINE void updateObjFromS4Slot(SEXP objSlot, const char *className) {
    SEXP obj = CAR(objSlot);

    if(IS_S4_OBJECT(obj) && isBasicClass(className)) {
	/* This and the similar test below implement the strategy
	 for S3 methods selected for S4 objects.  See ?Methods */
	if(NAMED(obj)) SET_NAMED(obj, 2);
	obj = R_getS4DataSlot(obj, S4SXP); /* the .S3Class obj. or NULL*/
	if(obj != R_NilValue) /* use the S3Part as the inherited object */
	    SETCAR(objSlot, obj);
    }
}

/* gr needs to be protected on return from this function */
static void findmethod(SEXP Class, const char *group, const char *generic,
		       SEXP *sxp,  SEXP *gr, SEXP *meth, int *which,
		       SEXP objSlot, SEXP rho)
{
    int len, whichclass;
    const void *vmax = vmaxget();

    len = length(Class);

    /* Need to interleave looking for group and generic methods
       e.g. if class(x) is c("foo", "bar)" then x > 3 should invoke
       "Ops.foo" rather than ">.bar"
    */
    for (whichclass = 0 ; whichclass < len ; whichclass++) {
	const char *ss = translateChar(STRING_ELT(Class, whichclass));
	*meth = installS3Signature(generic, ss);
	*sxp = R_LookupMethod(*meth, rho, rho, R_BaseEnv);
	if (isFunction(*sxp)) {
	    *gr = R_BlankScalarString;
	    if (whichclass > 0) updateObjFromS4Slot(objSlot, ss);
	    break;
	}
	*meth = installS3Signature(group, ss);
	*sxp = R_LookupMethod(*meth, rho, rho, R_BaseEnv);
	if (isFunction(*sxp)) {
	    *gr = mkString(group);
	    if (whichclass > 0) updateObjFromS4Slot(objSlot, ss);
	    break;
	}
    }
    vmaxset(vmax);
    *which = whichclass;
}

static SEXP classForGroupDispatch(SEXP obj) {

    return IS_S4_OBJECT(obj) ? R_data_class2(obj)
	    : getAttrib(obj, R_ClassSymbol);
}

attribute_hidden
int DispatchGroup(const char* group, SEXP call, SEXP op, SEXP args, SEXP rho,
		  SEXP *ans)
{
    int i, nargs, lwhich, rwhich;
    SEXP lclass, s, t, m, lmeth, lsxp, lgr, newvars;
    SEXP rclass, rmeth, rgr, rsxp, value;
    char *generic;
    Rboolean useS4 = TRUE, isOps = FALSE;

    /* pre-test to avoid string computations when there is nothing to
       dispatch on because either there is only one argument and it
       isn't an object or there are two or more arguments but neither
       of the first two is an object -- both of these cases would be
       rejected by the code following the string examination code
       below */
    if (args != R_NilValue && ! isObject(CAR(args)) &&
	(CDR(args) == R_NilValue || ! isObject(CADR(args))))
	return 0;

    isOps = strcmp(group, "Ops") == 0;

    /* try for formal method */
    if(length(args) == 1 && !IS_S4_OBJECT(CAR(args))) useS4 = FALSE;
    if(length(args) == 2 &&
       !IS_S4_OBJECT(CAR(args)) && !IS_S4_OBJECT(CADR(args))) useS4 = FALSE;
    if(useS4) {
	/* Remove argument names to ensure positional matching */
	if(isOps)
	    for(s = args; s != R_NilValue; s = CDR(s)) SET_TAG(s, R_NilValue);
	if(R_has_methods(op) &&
	   (value = R_possible_dispatch(call, op, args, rho, FALSE))) {
	       *ans = value;
	       return 1;
	}
	/* else go on to look for S3 methods */
    }

    /* check whether we are processing the default method */
    if ( isSymbol(CAR(call)) ) {
	const char *cstr = strchr(CHAR(PRINTNAME(CAR(call))), '.');
	if (cstr && !strcmp(cstr + 1, "default"))
	    return 0;
    }

    if(isOps)
	nargs = length(args);
    else
	nargs = 1;

    if( nargs == 1 && !isObject(CAR(args)) )
	return 0;

    generic = PRIMNAME(op);

    PROTECT(lclass = classForGroupDispatch(CAR(args)));

    if( nargs == 2 )
	rclass = classForGroupDispatch(CADR(args));
    else
	rclass = R_NilValue;

    PROTECT(rclass);
    lsxp = R_NilValue; lgr = R_NilValue; lmeth = R_NilValue;
    rsxp = R_NilValue; rgr = R_NilValue; rmeth = R_NilValue;

    findmethod(lclass, group, generic, &lsxp, &lgr, &lmeth, &lwhich,
	       args, rho);
    PROTECT(lgr);

    if( nargs == 2 )
	findmethod(rclass, group, generic, &rsxp, &rgr, &rmeth,
		   &rwhich, CDR(args), rho);
    else
	rwhich = 0;

    PROTECT(rgr);

    if( !isFunction(lsxp) && !isFunction(rsxp) ) {
	UNPROTECT(4);
	return 0; /* no generic or group method so use default */
    }

    if( lsxp != rsxp ) {
	if ( isFunction(lsxp) && isFunction(rsxp) ) {
	    /* special-case some methods involving difftime */
	    const char *lname = CHAR(PRINTNAME(lmeth)),
		*rname = CHAR(PRINTNAME(rmeth));
	    if( streql(rname, "Ops.difftime") &&
		(streql(lname, "+.POSIXt") || streql(lname, "-.POSIXt") ||
		 streql(lname, "+.Date") || streql(lname, "-.Date")) )
		rsxp = R_NilValue;
	    else if (streql(lname, "Ops.difftime") &&
		     (streql(rname, "+.POSIXt") || streql(rname, "+.Date")) )
		lsxp = R_NilValue;
	    else {
		warning(_("Incompatible methods (\"%s\", \"%s\") for \"%s\""),
			lname, rname, generic);
		UNPROTECT(4);
		return 0;
	    }
	}
	/* if the right hand side is the one */
	if( !isFunction(lsxp) ) { /* copy over the righthand stuff */
	    lsxp = rsxp;
	    lmeth = rmeth;
	    lgr = rgr;
	    lclass = rclass;
	    lwhich = rwhich;
	}
    }

    /* we either have a group method or a class method */

    PROTECT(m = allocVector(STRSXP,nargs));
    const void *vmax = vmaxget();
    s = args;
    const char *dispatchClassName = translateChar(STRING_ELT(lclass, lwhich));
    for (i = 0 ; i < nargs ; i++) {
	t = classForGroupDispatch(CAR(s));
	if (isString(t) && (stringPositionTr(t, dispatchClassName) >= 0))
	    SET_STRING_ELT(m, i, PRINTNAME(lmeth));
	else
	    SET_STRING_ELT(m, i, R_BlankString);
	s = CDR(s);
    }
    vmaxset(vmax);

    newvars = PROTECT(createS3Vars(
	PROTECT(mkString(generic)),
	lgr,
	PROTECT(stringSuffix(lclass, lwhich)),
	m,
	rho,
	R_BaseEnv
    ));

    PROTECT(t = LCONS(lmeth, CDR(call)));

    /* the arguments have been evaluated; since we are passing them */
    /* out to a closure we need to wrap them in promises so that */
    /* they get duplicated and things like missing/substitute work. */

    PROTECT(s = promiseArgs(CDR(call), rho));
    if (length(s) != length(args))
	error(_("dispatch error in group dispatch"));
    for (m = s ; m != R_NilValue ; m = CDR(m), args = CDR(args) ) {
	SET_PRVALUE(CAR(m), CAR(args));
	/* ensure positional matching for operators */
	if(isOps) SET_TAG(m, R_NilValue);
    }

    *ans = applyClosure(t, lsxp, s, rho, newvars);
    UNPROTECT(10);
    return 1;
}



SEXP attribute_hidden do_setnumthreads(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int old = R_num_math_threads, new_v;
    checkArity(op, args);
    new_v = asInteger(CAR(args));
    if (new_v >= 0 && new_v <= R_max_num_math_threads)
	R_num_math_threads = new_v;
    return ScalarInteger(old);
}

SEXP attribute_hidden do_setmaxnumthreads(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    int old = R_max_num_math_threads, new_v;
    checkArity(op, args);
    new_v = asInteger(CAR(args));
    if (new_v >= 0) {
	R_max_num_math_threads = new_v;
	if (R_num_math_threads > R_max_num_math_threads)
	    R_num_math_threads = R_max_num_math_threads;
    }
    return ScalarInteger(old);
}

SEXP attribute_hidden do_returnValue(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP val;
    checkArity(op, args);
    if (R_ExitContext && (val = R_ExitContext->returnValue)){
	MARK_NOT_MUTABLE(val);
	return val;
    }
    return CAR(args); /* default */
}

#include <Parse.h>
SEXP R_ParseEvalString(const char *str, SEXP env)
{
    SEXP s = PROTECT(mkString(str));

    ParseStatus status;
    SEXP ps = PROTECT(R_ParseVector(s, -1, &status, R_NilValue));
    if (status != PARSE_OK ||
	TYPEOF(ps) != EXPRSXP ||
	LENGTH(ps) != 1)
	error("parse error");

    SEXP val = eval(VECTOR_ELT(ps, 0), env);
    UNPROTECT(2); /* s, ps */
    return val;
}
