/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1995, 1996  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1998-2015   The R Core Team
 *  Copyright (C) 2002-2005  The R Foundation
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
#include <config.h>
#endif

#include <math.h> /* avoid redefinition of extern in Defn.h */
#include <float.h>
#include <ctype.h>
//--by GT--#include <stdlib.h>
#include <string.h>

#define __MAIN__
#define R_USE_SIGNALS 1
#include "Defn.h"
#include <Internal.h>
//--by GT--#include "Rinterface.h"
#include "IOStuff.h"
//--by GT--#include "Fileio.h"
#include "Parse.h"
#include "Startup.h"

#include "sgx_error.h"

#include "rvm.h"
#include "../../include/vmoutput.h"

#include "../../gt_enclave_t.h"
#include "initpkg.h"

// This creates the interface pointers in this file
#define __SYSTEM__
#define R_INTERFACE_PTRS 1
#include <Rinterface.h>
#undef __SYSTEM__

#include <Rdynpriv.h>
#include "../include/Lapack.h"
#include "../randomForest/rf.h"

VM_Result Result;

/* call pointers to allow interface switching */


void R_Suicide(const char *s) { ptr_R_Suicide(s); }
void R_ShowMessage(const char *s) { ptr_R_ShowMessage(s); }
int R_ReadConsole(const char *prompt, unsigned char *buf, int len, int addtohistory)
{ return ptr_R_ReadConsole(prompt, buf, len, addtohistory); }
//--by GT--void R_WriteConsole(const char *buf, int len) {if (ptr_R_WriteConsole) ptr_R_WriteConsole(buf, len); else ptr_R_WriteConsoleEx(buf, len, 0); }
//--by GT--void R_WriteConsoleEx(const char *buf, int len, int otype) {if (ptr_R_WriteConsole) ptr_R_WriteConsole(buf, len); else ptr_R_WriteConsoleEx(buf, len, otype); }
//--by GT--void R_ResetConsole(void) { ptr_R_ResetConsole(); }
#ifndef HAVE_AQUA
void R_FlushConsole(void) { ptr_R_FlushConsole(); }
#endif
void R_ClearerrConsole(void) { ptr_R_ClearerrConsole(); }
void R_Busy(int which) { ptr_R_Busy(which); }
void R_CleanUp(SA_TYPE saveact, int status, int runLast)
{ ptr_R_CleanUp(saveact, status, runLast); }

attribute_hidden
int R_ShowFiles(int nfile, const char **file, const char **headers,
		const char *wtitle, Rboolean del, const char *pager)
{ return ptr_R_ShowFiles(nfile, file, headers, wtitle, del, pager); }

attribute_hidden
int R_ChooseFile(int _new,  char *buf, int len)
{ return ptr_R_ChooseFile(_new, buf, len); }

const char *R_ExpandFileName(const char *s)
{
#ifdef HAVE_LIBREADLINE
    if(UsingReadline) {
	const char * c = R_ExpandFileName_readline(s, newFileName);
	/* we can return the result only if tilde_expand is not broken */
	if (!c || c[0]!='~' || (c[1]!='\0' && c[1]!='/'))
	    return c;
    }
#endif
    return s;//--by GT--R_ExpandFileName_unix(s, newFileName);
}

//--by GT--#include <locale.h>
#include <R_ext/Print.h>

#include "testR.h"

#ifdef ENABLE_NLS
void attribute_hidden nl_Rdummy(void)
{
    /* force this in as packages use it */
    dgettext("R", "dummy - do not translate");
}
#endif


/* The 'real' main() program is in Rmain.c on Unix-alikes, and
   src/gnuwin/front-ends/graphappmain.c on Windows, unless of course
   R is embedded */

/* Global Variables:  For convenience, all interpeter global symbols
 * ================   are declared in Defn.h as extern -- and defined here.
 *
 * NOTE: This is done by using some preprocessor trickery.  If __MAIN__
 * is defined as above, there is a sneaky
 *     #define extern
 * so that the same code produces both declarations and definitions.
 *
 * This does not include user interface symbols which are included
 * in separate platform dependent modules.
 */



/*
  This is a reorganization of the REPL (Read-Eval-Print Loop) to separate
  the loop from the actions of the body. The motivation is to make the iteration
  code (Rf_ReplIteration) available as a separately callable routine
  to avoid cutting and pasting it when one wants a single iteration
  of the loop. This is needed as we allow different implementations
  of event loops. Currently (summer 2002), we have a package in
  preparation that uses Rf_ReplIteration within either the
  Tcl or Gtk event loop and allows either (or both) loops to
  be used as a replacement for R's loop and take over the event
  handling for the R process.

  The modifications here are intended to leave the semantics of the REPL
  unchanged, just separate into routines. So the variables that maintain
  the state across iterations of the loop are organized into a structure
  and passed to Rf_ReplIteration() from Rf_ReplConsole().
*/


/**
  (local) Structure for maintaining and exchanging the state between
  Rf_ReplConsole and its worker routine Rf_ReplIteration which is the
  implementation of the body of the REPL.

  In the future, we may need to make this accessible to packages
  and so put it into one of the public R header files.
 */
typedef struct {
  ParseStatus    status;
  int            prompt_type;
  int            browselevel;
  unsigned char  buf[CONSOLE_BUFFER_SIZE+1];
  unsigned char *bufp;
} R_ReplState;

void R_SetResultMsg(int code, char result[]) {
	Result.result_code = code; //type of result: -1: error message, 1: return value
	strncpy(Result.result_buf,result,RESULT_BUF_SIZE-1);
}

// Set the result value according to the result expression type
// Support most of simple types and only one level of cascading symbol
void R_SetResultValue(int code, SEXP e){
	SEXP s;
	Result.result_code = code;
	char buf[RESULT_BUF_SIZE];

	R_xlen_t n=XLENGTH(e);

	switch (TYPEOF(e)) {
    case NILSXP:
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","NILSXP");
    	Result.result_buf[0]='\0';
    	break;
    case SYMSXP:
    	s = SYMVALUE(e);
    	if (TYPEOF(s)!=SYMSXP){
    		R_SetResultValue(code,s);
    	}else {
    		strncpy(Result.result_buf,"cascaded symbol",RESULT_BUF_SIZE-1);
    		snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","SYMSXP");
    	}
    	strncpy(buf,Result.result_buf,RESULT_BUF_SIZE-1);
    	s = PRINTNAME(e);
    	if (TYPEOF(s)==CHARSXP) {
    	  snprintf(Result.result_buf,RESULT_BUF_SIZE-1, "%s:%s", "SYMSXP", CHAR(s));
    	} else
      	  snprintf(Result.result_buf,RESULT_BUF_SIZE-1, "%s:No Name", "SYMSXP");
    	strncat(Result.result_buf,buf,RESULT_BUF_SIZE-strlen(Result.result_buf)-1);
    	break;
    case CHARSXP:{
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","CHARSXP");
    	snprintf(Result.result_buf,RESULT_BUF_SIZE-1, "%s", CHAR(e));
    }
    	break;
    case LGLSXP:{
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","LGLSXP");
    	//snprintf(rResult.result_buf,R_RESULT_BUF_SIZE-1, "%d", *LOGICAL(e));
    	for(int i=0,tmpptr=0;i<n;i++){
    	    tmpptr+=snprintf(Result.result_buf+tmpptr, sizeof(Result.result_buf)-tmpptr,"%d  ", ((*LOGICAL)(e))[i]);
    	}
    }
    	break;
    case INTSXP:{
    	snprintf(Result.result_type, 9, "%s","INTSXP");
    	//snprintf(rResult.result_buf,R_RESULT_BUF_SIZE-1, "%d", *INTEGER(e));
    	for(int i=0,tmpptr=0;i<n;i++){
    	    tmpptr+=snprintf(Result.result_buf+tmpptr, sizeof(Result.result_buf)-tmpptr,"%d  ", ((*INTEGER)(e))[i]);
    	}
    }
    	break;
    case REALSXP:{
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","REALSXP");
    	//snprintf(rResult.result_buf,R_RESULT_BUF_SIZE-1, "%f", *REAL(e));
        for(int i=0,tmpptr=0;i<n;i++){
        	tmpptr+=snprintf(Result.result_buf+tmpptr, sizeof(Result.result_buf)-tmpptr,"%f  ", ((*REAL)(e))[i]);
        }
    }
    	break;
    case STRSXP:{
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","STRSXP");
    	//int n = length(e);
    	//n = (n>=R_RESULT_BUF_SIZE-1)?R_RESULT_BUF_SIZE-1:n;
    	char *ptr = (char *)CHAR(STRING_ELT(e,0));
    	snprintf(Result.result_buf,RESULT_BUF_SIZE-1, "%s", ptr);
    	//for (int i = 0; i < n; i++) {
    	//         rResult.result_buf[i] = *CHAR(STRING_ELT(e, i));
    	//}
    	//rResult.result_buf[n] = '\0';
    }
    	break;
    default:
    	snprintf(Result.result_type, RESULT_TYPE_SIZE-1, "%s","NoneSXP");
    	snprintf(Result.result_buf,RESULT_BUF_SIZE-1, "SXP Type %d:Un-printable value", TYPEOF(e));
    	break;
	}

}

void dumm_func(void) {
	return;
}

int Rf_initialize_R()
{
    int i, ioff = 1, j;
    Rboolean useX11 = TRUE, useTk = FALSE;
    char *p, msg[1024], cmdlines[10000], **avv;
    structRstart rstart;
    Rstart Rp = &rstart;
    Rboolean force_interactive = FALSE;


    //--by GT--ptr_R_Suicide = dumm_func; //--by GT--Rstd_Suicide;
    //--by GT--ptr_R_ShowMessage = dumm_func; //--by GT--Rstd_ShowMessage;
    //--by GT--ptr_R_ReadConsole = dumm_func; //--by GT--Rstd_ReadConsole;
    //--by GT--ptr_R_WriteConsole = dumm_func; //--by GT--Rstd_WriteConsole;
    ptr_R_ResetConsole = dumm_func; //--by GT--Rstd_ResetConsole;
    ptr_R_FlushConsole = dumm_func; //--by GT--Rstd_FlushConsole;
    ptr_R_ClearerrConsole = dumm_func; //--by GT--Rstd_ClearerrConsole;
    //--by GT--ptr_R_Busy = dumm_func; //--by GT--Rstd_Busy;
    //--by GT--ptr_R_CleanUp = dumm_func; //--by GT--Rstd_CleanUp;
    //--by GT--ptr_R_ShowFiles = dumm_func; //--by GT--Rstd_ShowFiles;
    //--by GT--ptr_R_ChooseFile = dumm_func; //--by GT--Rstd_ChooseFile;
    //--by GT--ptr_R_loadhistory = dumm_func; //--by GT--Rstd_loadhistory;
    //--by GT--ptr_R_savehistory = dumm_func; //--by GT--Rstd_savehistory;
    //--by GT--ptr_R_addhistory = dumm_func; //--by GT--Rstd_addhistory;
    //--by GT--ptr_R_EditFile = NULL; /* for future expansion */
    //--by GT--R_timeout_handler = NULL;
    //--by GT--R_timeout_val = 0;

    R_GlobalContext = NULL; /* Make R_Suicide less messy... */


    //--by GT--R_setStartTime();
    R_DefParams(Rp);

    //--by GT--if (ifp && Rp->SaveAction != SA_SAVE)
    Rp->SaveAction = SA_NOSAVE;

    R_SetParams(Rp);


	R_Outputfile = NULL;//stdout;
	R_Consolefile = NULL;//stderr;

/*
 *  Since users' expectations for save/no-save will differ, we decided
 *  that they should be forced to specify in the non-interactive case.
 */
    if (!R_Interactive && Rp->SaveAction != SA_SAVE &&
	Rp->SaveAction != SA_NOSAVE)
	R_Suicide(_("you must specify '--save', '--no-save' or '--vanilla'"));

    //--bt GT--R_setupHistory();
    //--bt GT--if (R_RestoreHistory)
    //--bt GT--Rstd_read_history(R_HistoryFile);
    fpu_setup(1);

    return(0);
}

void fpu_setup(Rboolean start)
{
    if (start) {
#ifdef __FreeBSD__
    fpsetmask(0);
#endif

#if (defined(__i386) || defined(__x86_64)) && defined(__INTEL_COMPILER) && __INTEL_COMPILER > 800
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
#endif
    } else {
#ifdef __FreeBSD__
    fpsetmask(~0);
#endif
    }
}

/* Eval an input string expression */
SEXP EvalExpr(char *buf, SEXP rho){
	ParseStatus status;
	int len;
	int savestack;
	IoBuffer iob;
	SEXP thisExpr, value;
    Rboolean wasDisplayed = FALSE;

	if (!R_IoBufferInit(&iob)) {
		REprintf("IO buffer initialization fail.\n");
		//R_SetResultMsg(-1,"IO buffer initialization fail.\n");
		return NULL;
	}

	len = R_IoBufferPuts(buf, &iob);

	if (len != strlen(buf)) {
		REprintf("IO buffer put string fail. \n");
		//R_SetResultMsg(-1,"IO buffer put string fail. \n");
	    R_IoBufferFree(&iob);
		return NULL;
	}

	savestack = R_PPStackTop;

	R_CurrentExpr = R_Parse1Buffer(&iob, 1, &status);

	if (PARSE_OK != status){
	    R_IoBufferFree(&iob);
		return NULL;
	}
	R_Visible=FALSE;
    R_EvalDepth = 0;
    PROTECT(thisExpr = R_CurrentExpr);
    PROTECT(value = eval(thisExpr, rho));
    SET_SYMVALUE(R_LastvalueSymbol, value);
    UNPROTECT(2);
    return value;
}
/* Read-Eval-Print Loop [ =: REPL = repl ] with input from a buffer */

static void R_ReplBuffer(char *buf, SEXP rho)
{
	ParseStatus status;
	int len;
	int savestack;
	IoBuffer iob;
	SEXP thisExpr, value;
    Rboolean wasDisplayed = FALSE;

	if (!R_IoBufferInit(&iob)) {
		REprintf("IO buffer initialization fail.\n");
		//R_SetResultMsg(-1,"IO buffer initialization fail.\n");
		return;
	}

	len = R_IoBufferPuts(buf, &iob);

	if (len != strlen(buf)) {
		REprintf("IO buffer put string fail. \n");
		//R_SetResultMsg(-1,"IO buffer put string fail. \n");
	    R_IoBufferFree(&iob);
		return;
	}

	savestack = R_PPStackTop;

    for(;;) {
		R_PPStackTop = savestack;
		R_CurrentExpr = R_Parse1Buffer(&iob, 1, &status);
		switch (status) {
		case PARSE_NULL:
		    break;
		case PARSE_OK:
			R_Visible=FALSE;
		    R_EvalDepth = 0;
	    	//resetTimeLimits();
		    PROTECT(thisExpr = R_CurrentExpr);
		    //R_Busy(1);
		    PROTECT(value = eval(thisExpr, rho));
		    SET_SYMVALUE(R_LastvalueSymbol, value);
		    //R_SetResultValue(1,value);
		    UNPROTECT(2);
		    if(R_Visible)
		    	PrintValueEnv(value,rho);
		    if(R_CollectWarnings)
		    	PrintWarnings();
		    break;
		case PARSE_ERROR:
		    //R_FinalizeSrcRefState();
			R_IoBufferFree(&iob);
		    parseError(R_NilValue, R_ParseError);
		    break;
		case PARSE_EOF:
		    //R_FinalizeSrcRefState();
			//R_SetResultValue(1,value);
			R_IoBufferFree(&iob);
		    return;
		    break;
		case PARSE_INCOMPLETE:
		    /* can't happen: just here to quieten -Wall */
		    break;
		}
    }
    //R_SetResultValue(1,value);
    R_IoBufferFree(&iob);
}

int R_SignalHandlers = 1;  /* Exposed in R_interface.h */

void setup_Rmainloop()
{
    volatile int doneit;
    volatile SEXP baseEnv;
    SEXP cmd;
    char deferred_warnings[11][250];
    volatile int ndeferred_warnings = 0;

    /* In case this is a silly limit: 2^32 -3 has been seen and
     * casting to intptr_r relies on this being smaller than 2^31 on a
     * 32-bit platform. */
    if(R_CStackLimit > 100000000U)
	R_CStackLimit = (uintptr_t)-1;
    /* make sure we have enough head room to handle errors */
    if(R_CStackLimit != -1)
	R_CStackLimit = (uintptr_t)(0.95 * R_CStackLimit);

    // InitConnections(); /* needed to get any output at all */

    /* Initialize the interpreter's internal structures. */


    /* make sure srand is called before R_tmpnam, PR#14381 */
    //--by GT--srand(TimeToSeed());

    InitArithmetic();
    InitParser();
    //--by GT--InitTempDir(); /* must be before InitEd */
    InitMemory();
    InitStringHash(); /* must be before InitNames */
    InitBaseEnv();
    InitNames(); /* must be after InitBaseEnv to use R_EmptyEnv */
    InitGlobalEnv();
    InitDynload();
    InitOptions();
    //--by GT--InitEd();
    //--by GT--InitGraphics();
    InitTypeTables(); /* must be before InitS3DefaultTypes */
    InitS3DefaultTypes();
    PrintDefaults();

    R_Is_Running = 1;
    //--by GT--R_check_locale();

    /* Initialize the global context for error handling. */
    /* This provides a target for any non-local gotos */
    /* which occur during error handling */

    R_Toplevel.nextcontext = NULL;
    R_Toplevel.callflag = CTXT_TOPLEVEL;
    R_Toplevel.cstacktop = 0;
    R_Toplevel.gcenabled = R_GCEnabled;
    R_Toplevel.promargs = R_NilValue;
    R_Toplevel.callfun = R_NilValue;
    R_Toplevel.call = R_NilValue;
    R_Toplevel.cloenv = R_BaseEnv;
    R_Toplevel.sysparent = R_BaseEnv;
    R_Toplevel.conexit = R_NilValue;
    R_Toplevel.vmax = NULL;
    R_Toplevel.nodestack = R_BCNodeStackTop;
#ifdef BC_INT_STACK
    R_Toplevel.intstack = R_BCIntStackTop;
#endif
    R_Toplevel.cend = NULL;
    R_Toplevel.cenddata = NULL;
    R_Toplevel.intsusp = FALSE;
    R_Toplevel.handlerstack = R_HandlerStack;
    R_Toplevel.restartstack = R_RestartStack;
    R_Toplevel.srcref = R_NilValue;
    R_Toplevel.prstack = NULL;
    R_Toplevel.returnValue = NULL;
    R_Toplevel.evaldepth = 0;
    R_Toplevel.browserfinish = 0;
    R_GlobalContext = R_ToplevelContext = R_SessionContext = &R_Toplevel;
    R_ExitContext = NULL;

    R_Warnings = R_NilValue;

    /* This is the same as R_BaseEnv, but this marks the environment
       of functions as the namespace and not the package. */
    baseEnv = R_BaseNamespace;

    /* Set up some global variables */
    Init_R_Variables(baseEnv);

    R_init_lapack();

    DllInfo* dllrf=AddDLL("randomForest",0,0,NULL);
    R_init_randomForest(dllrf);

    DllInfo* dllstats=AddDLL("stats",0,0,NULL);
    R_init_stats(dllstats);

    /* On initial entry we open the base language package and begin by
       running the repl on it.
       If there is an error we pass on to the repl.
       Perhaps it makes more sense to quit gracefully?
    */

//    const char* base_loc="/home/accuser/R351/library/base/R/base";
//    const char* base_loc="/home/jingwei/genetank_blockchain/doc/R/all.R";
//    size_t buf_len=1024*512*1;
//
//    char* base_buf=(char*)calloc(buf_len, sizeof(char));
//    sgx_status_t retval;
//    ocall_load_base(&retval, base_loc, base_buf, buf_len);
//
//
//    if(SETJMP(R_Toplevel.cjmpbuf)){
//    	    //check_session_exit();
//        Rdummy();
//        R_GlobalContext=R_ToplevelContext=R_SessionContext=&R_Toplevel;
//    }
//    else{
//       	R_ReplBuffer(base_buf, R_GlobalEnv);
//    }
//
//    free(base_buf);

    //DL_FUNC fun1=CEntries[2].fun;
}


VM_Result *run_R_code(const char *r_code, char *result, size_t result_size)
{
	Result.cx=0;
	if (NULL == Result.result_type)
		Result.result_type=(char*)calloc(RESULT_TYPE_SIZE, sizeof(char));
	if (NULL == Result.result_buf)
		Result.result_buf=(char*)calloc(RESULT_BUF_SIZE, sizeof(char));

	if(SETJMP(R_Toplevel.cjmpbuf)){
	    //check_session_exit();
		Rdummy();
        R_GlobalContext=R_ToplevelContext=R_SessionContext=&R_Toplevel;
        //rResult.result_buf[0] = '\0';
	}
	else{
		R_GlobalContext=R_ToplevelContext=R_SessionContext=&R_Toplevel;
   	    R_ReplBuffer((char *)r_code, R_GlobalEnv);
	}
	if (NULL != result){
		snprintf(result, result_size, "%s\n", Result.result_buf);
		free(Result.result_type);
		free(Result.result_buf);
		Result.result_type=NULL;
		Result.result_buf=NULL;
	}

    return &Result;
}


/*this functionality now appears in 3
  places-jump_to_toplevel/profile/here */

//void R_setjmp(){}

//void check_session_exit(){
//    static Rboolean exiting = FALSE;
//    if(exiting)
//    	R_Suicide("error during cleanup\n");
//    else{
//    	exiting = TRUE;
//    	if(GetOption1(install("error"))!=R_NilValue){
//    		exiting = FALSE;
//    		return;
//    	}
//    	//REprint("Excecution halted\n");
//    	//R_CleanUp(SA_NOSAVE,1,0);
//    }
//}

void Rdummy(){
	return;
}
