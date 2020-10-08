#include <stdlib.h> // for NULL
//#include <Defn.h>
#include "rf.h"
#include "../include/initpkg.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

/* .C calls */
static const R_CMethodDef CEntries[5] = {
  {"classForest", (DL_FUNC) classForest, 24},
  {"classRF",     (DL_FUNC) classRF,     39},
  {"regForest",   (DL_FUNC) regForest,   21},
  {"regRF",       (DL_FUNC) regRF,       41},
  {NULL, NULL, 0}
};

void R_init_randomForest(DllInfo *dll)
{
    R_registerRoutines(dll, CEntries, NULL, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
//  R_forceSymbols(dll,TRUE);
//	R_RegisterCCallable("randomForest","classForest", (DL_FUNC) classForest);
//	R_RegisterCCallable("randomForest","classRF",     (DL_FUNC) classRF);
//	R_RegisterCCallable("randomForest","regForest",   (DL_FUNC) regForest);
//	R_RegisterCCallable("randomForest","regRF",       (DL_FUNC) regRF);
}

//#ifdef __cplusplus
//}
//#endif
