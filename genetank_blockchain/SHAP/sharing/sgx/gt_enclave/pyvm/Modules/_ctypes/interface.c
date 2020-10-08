#include "interface.h"
#include <string.h>

enum string_code hash_string(const char* str){
    if(strcmp("XGBCallbackSetData", str)==0) return c_XGBCallbackSetData;
	if(strcmp("XGBcallbackDataIterNext", str)==0) return c_XGBcallbackDataIterNext;
	if(strcmp("XGBGetLastError", str)==0) return c_XGBGetLastError;
	if(strcmp("XGBRegisterLogCallback", str)==0) return c_XGBRegisterLogCallback;
	if(strcmp("XGDMatrixCreateFromFile", str)==0) return c_XGDMatrixCreateFromFile;
	if(strcmp("XGDMatrixCreateFromDataIter", str)==0) return c_XGDMatrixCreateFromDataIter;
	if(strcmp("XGDMatrixCreateFromCSREx", str)==0) return c_XGDMatrixCreateFromCSREx;
	if(strcmp("XGDMatrixCreateFromCSCEx", str)==0) return c_XGDMatrixCreateFromCSCEx;
	if(strcmp("XGDMatrixCreateFromMat", str)==0) return c_XGDMatrixCreateFromMat;
	if(strcmp("XGDMatrixCreateFromDT", str)==0) return c_XGDMatrixCreateFromDT;
	if(strcmp("XGDMatrixSliceDMatrix", str)==0) return c_XGDMatrixSliceDMatrix;
	if(strcmp("XGDMatrixSliceDMatrixEx", str)==0) return c_XGDMatrixSliceDMatrixEx;
	if(strcmp("XGDMatrixFree", str)==0) return c_XGDMatrixFree;
	if(strcmp("XGDMatrixSaveBinary", str)==0) return c_XGDMatrixSaveBinary;
	if(strcmp("XGDMatrixSetFloatInfo", str)==0) return c_XGDMatrixSetFloatInfo;
	if(strcmp("XGDMatrixSetUIntInfo", str)==0) return c_XGDMatrixSetUIntInfo;
	if(strcmp("XGDMatrixSetGroup", str)==0) return c_XGDMatrixSetGroup;
	if(strcmp("XGDMatrixGetFloatInfo", str)==0) return c_XGDMatrixGetFloatInfo;
	if(strcmp("XGDMatrixGetUIntInfo", str)==0) return c_XGDMatrixGetUIntInfo;
	if(strcmp("XGDMatrixNumRow", str)==0) return c_XGDMatrixNumRow;
	if(strcmp("XGDMatrixNumCol", str)==0) return c_XGDMatrixNumCol;
	if(strcmp("XGBoosterCreate", str)==0) return c_XGBoosterCreate;
	if(strcmp("XGBoosterFree", str)==0) return c_XGBoosterFree;
	if(strcmp("XGBoosterSetParam", str)==0) return c_XGBoosterSetParam;
	if(strcmp("XGBoosterUpdateOneIter", str)==0) return c_XGBoosterUpdateOneIter;
	if(strcmp("XGBoosterBoostOneIter", str)==0) return c_XGBoosterBoostOneIter;
	if(strcmp("XGBoosterEvalOneIter", str)==0) return c_XGBoosterEvalOneIter;
	if(strcmp("XGBoosterPredict", str)==0) return c_XGBoosterPredict;
	if(strcmp("XGBoosterLoadModel", str)==0) return c_XGBoosterLoadModel;
	if(strcmp("XGBoosterSaveModel", str)==0) return c_XGBoosterSaveModel;
	if(strcmp("XGBoosterLoadModelFromBuffer", str)==0) return c_XGBoosterLoadModelFromBuffer;
	if(strcmp("XGBoosterGetModelRaw", str)==0) return c_XGBoosterGetModelRaw;
	if(strcmp("XGBoosterSerializeToBuffer", str)==0) return c_XGBoosterSerializeToBuffer;
	if(strcmp("XGBoosterUnserializeFromBuffer", str)==0) return c_XGBoosterUnserializeFromBuffer;
	if(strcmp("XGBoosterSaveJsonConfig", str)==0) return c_XGBoosterSaveJsonConfig;
	if(strcmp("XGBoosterLoadJsonConfig", str)==0) return c_XGBoosterLoadJsonConfig;
	if(strcmp("XGBoosterDumpModel", str)==0) return c_XGBoosterDumpModel;
	if(strcmp("XGBoosterDumpModelEx", str)==0) return c_XGBoosterDumpModelEx;
	if(strcmp("XGBoosterDumpModelWithFeatures", str)==0) return c_XGBoosterDumpModelWithFeatures;
	if(strcmp("XGBoosterDumpModelExWithFeatures", str)==0) return c_XGBoosterDumpModelExWithFeatures;
	if(strcmp("XGBoosterGetAttr", str)==0) return c_XGBoosterGetAttr;
	if(strcmp("XGBoosterSetAttr", str)==0) return c_XGBoosterSetAttr;
	if(strcmp("XGBoosterGetAttrNames", str)==0) return c_XGBoosterGetAttrNames;
}

