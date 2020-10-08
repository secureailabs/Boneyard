#include "../include/xgboost/c_api.h"
#include "../../../Include/Python.h"

static PyObject* Py_XGBGetLastError(PyObject* self, PyObject* no_arg){
    char* ret = XGBGetLastError();
    return Py_BuildValue("s", ret);
}

static PyObject* Py_XGDMatrixCreateFromFile(PyObject* self, PyObject* args){
    char* fname;
    int silent;
    PyObject* tmp;
    
    if(!PyArg_ParseTuple(args, "siO", &fname, &silent, &tmp)){
       return NULL;
    }
    
    void* out=PyCapsule_GetPointer(tmp);
    
    XGDMatrixCreateFromFile(fname, silent, out);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixCreateFromCSREx(PyObject* self, PyObject* args){
    PyObject* oindptr;
    PyObject* oindices;
    PyObject* odata;
    size_t nindptr, nelem, num_col;
    PyObject* tmp;
    
    if(!PyArgParseTuple(args, "OOOkkkO", &oindptr, &oindeces, &odata, &ndindptr, &nelem, &num_col, &tmp)){
        return NULL;
    }
    
    PyArrayObject* aoindptr = PyArray_FROM_OTF(oindptr, NPY_ULONG, NPY_IN_ARRAY);    
    PyArrayObject* aoindices = PyArray_FROM_OTF(oindices, NPY_UINT, NPY_IN_ARRAY);
    PyArrayObject* aodata = PyArray_FROM_OTF(odata, NPY_DOUBLE, NPY_IN_ARRAY);
    
    size_t* indptr = (size_t*)PyArray_DATA(aoindptr);
    unsigned* indices = (unsigned*)PyArray_DATA(aoindices);
    float* data = (float*)PyArray_DATA(aodata);
    
    void* out = PyCapsule_GetPointer(tmp, NULL);
    
    XGDMatrixCreateFromCSREx(indptr, indices, data, nindptr, nelem, num_col, tmp);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixCreateFromCSCEx(PyObject* self, PyObject* args){
    PyObject* ocol_ptr;
    PyObject* oindices;
    PyObject* odata;
    size_t nindptr, nelem, num_row;
    PyObject* tmp;
    
    if(!PyArgParseTuple(args, "OOOkkkO", &ocol_ptr, &oindeces, &odata, &ndindptr, &nelem, &num_row, &tmp)){
        return NULL;
    }
    
    PyArrayObject* aocol_ptr = PyArray_FROM_OTF(ocol_ptr, NPY_ULONG, NPY_IN_ARRAY);    
    PyArrayObject* aoindices = PyArray_FROM_OTF(oindices, NPY_UINT, NPY_IN_ARRAY);
    PyArrayObject* aodata = PyArray_FROM_OTF(odata, NPY_DOUBLE, NPY_IN_ARRAY);
    
    size_t* col_ptr = (size_t*)PyArray_DATA(aocol_ptr);
    unsigned* indices = (unsigned*)PyArray_DATA(aoindices);
    float* data = (float*)PyArray_DATA(aodata);
    
    void* out = PyCapsule_GetPointer(tmp, NULL);
    
    XGDMatrixCreateFromCSREx(col_ptr, indices, data, nindptr, nelem, num_row, tmp);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixCreateFromMat(PyObject* self, PyObject* args){
    PyObject* odata;
    unsigned long nrow, ncol;
    float missing;
    PyObject* tmp;
    
    if(!PyArg_ParseTuple(args, "OkkfO", &odata, &nrow, &ncol, &missing, &tmp)){
        return NULL;
    }
    
    PyArrayObject* aodata = PyArray_FROM_OTF(odata, NPY_DOUBLE, NPY_IN_ARRAY);
    
    float* data = (float*)PyArray_DATA(aodata);
    
    void* out = PyCapsule_GetPointer(tmp, NULL);
    
    XGDMatrixCreateFromMat(data, nrow, ncol, missing, out);
    
    return Py_BuildValue("");    
}

static PyObject* Py_XGDMatrixCreateFromDT(PyObject* self, PyObject* args){
    PyObject* odata;
    PyOjbect* ofeature_types;
    unsigned long nrow, ncol;
    PyObject* tmp;
    int nthread;
    
    if(!PyArg_ParseTuple(args, "OOkkOi", &odata, &ofeature_types, &nrow, &ncol, &tmp, &nthread)){
        return NULL;
    }    
    
    PyArrayObject* aodata = PyArray_FROM_OTF(odata, NPY_DOUBLE, NPY_IN_ARRAY);
    PyArrayObject* aofeature_types = PyArray_FROM_OTF(ofeature_types, NPY_CHAR, NPY_IN_ARRAY);
    
    void* addata = PyArray_DATA(aodata);
    void* data = &addata;
    char* adfeature = (char*)PyArray_DATA(aofeature_types);
    char** feature_types = &adfeature;
    
    void* out = PyCapsule_GetPointer(tmp, NULL);
    
    XGDMatrixCreateFromDT(data, feature_types, nrow, ncol, tmp);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixSliceDMatrix(PyObject* self, PyObject* args){

   PyObject* ohandle;
   PyObject* oidxset;
   unsigned long len;
   PyObject* tmp;
   
   if(!PyArg_ParseTuple(args, "OOkO", &ohandle, &oidxset, &len, &tmp)){
       return NULL;
   }
   
   void* handle = PyCapsule_GetPointer(ohandle, NULL);
   void* out = PyCapsule_GetPointer(tmp, NULL);
   
   PyArrayObject* aoidxset = PyArray_FROM_OTF(oidxset, NPY_INT, NPY_IN_ARRAY);
   int* idxset = PyArray_DATA(aoidxset);
   
   XGDMatrixSliceDMatrix(handle, idxset, len , tmp);
   
   return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixFree(PyObject* self, PyObject* args){
   PyObject* tmp;
   
   if(!PyArg_ParseTuple(args, "O", &tmp)){
       return NULL;
   }
   
   void* out = PyCapsule_GetPointer(tmp, NULL); 
   XGDMatrixFree(out);
   return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixSaveBinary(PyObject* self, PyObject* args){
   PyObject* handle;
   char* fname;
   int silent;
   
   if(!PyArg_ParseTuple(args, "Osi", &handle, &fname, &silent)){
       return NULL;
   }
   
   void* out = PyCapsule_GetPointer(handle, NULL);
   
    XGDMatrixSaveBinary(out, fname, silent);
    
    return Py_BuildValue("");   
}

static PyObject* Py_XGDMatrixSetFloatInfo(PyObject* self, PyObject* args){
    PyObject* handle;
    PyObject* ofield;
    PyObject* oarray;
    unsigned long len;
    
    if(!PyArg_ParseTuple(args, "OOOk", &handle, &ofield, &oarray, &len)){
        return NULL;
    }
    
    PyArrayObject* aofield = PyArray_FROM_OTF(ofield, NPY_CHAR, NPY_IN_ARRAY);
    PyArrayObject* aoarray = PyArray_FROM_OTF(oarray, NPY_DOUBLE, NPY_IN_ARRAY);
    
    char* field = (char*)PyArray_DATA(aofield);
    float* array = (float*)PyArray_DATA(aoarray);
    
    void* out = PyCapsule_GetPointer(handle, NULL);
    
    XGDMatrixSetFloatInfo(handle, field, array, len);
    
    return Py_Buildalue("");
}

static PyObject* Py_XGDMatrixSetUIntInfo(PyObject* self, PyObject* args){
    PyObject* handle;
    PyObject* ofield;
    PyObject* oarray;
    unsigned long len;
    
    if(!PyArg_ParseTuple(args, "OOOk", &handle, &ofield, &oarray, &len)){
        return NULL;
    }
    
    PyArrayObject* aofield = PyArray_FROM_OTF(ofield, NPY_CHAR, NPY_IN_ARRAY);
    PyArrayObject* aoarray = PyArray_FROM_OTF(oarray, NPY_UINT, NPY_IN_ARRAY);
    
    char* field = (char*)PyArray_DATA(aofield);
    unsigned* array = (unsigned*)PyArray_DATA(aoarray);
    
    void* out = PyCapsule_GetPointer(handle, NULL, NULL);
    
    XGDMatrixSetUIntInfo(handle, field, array, len);
    
    return Py_Buildalue("");
    
}

static PyObject* Py_XGDMatrixSetGroup(PyObject* self, PyOjbect* args){
    PyObject* handle;
    PyObject* ogroup;
    unsigned long len;
    
    if(!PyArg_ParseTuple(args, "OOk", &handle, &ogroup, &len)){
        return NULL;
    }
    
    PyArrayObject* aogroup = PyArray_FROM_OTF(ogroup, NPY_UINT, NPY_IN_ARRAY);
    unsigned* group = PyArray_DATA(aogroup);
    
    void* out = PyCapsule_GetPointer(handle, NULL);
    
    XGDMatrixSetGroup(handle, group, len);
    
    Py_BuildValue("");
}

static PyObject* Py_XGDMatrixGetFloatInfo(){
    PyObject* handle;
    PyObject* ofield;
    PyObject* oout_len;
    PyObject* oout_dptr;
    
    if(!PyArg_ParseTuple(args, "OOOO", &handle, &ofield, &oout_len, &oout_dptr)){
        return NULL;
    }
    
    PyArrayObject* aofield = PyArray_FROM_OTF(ofield, NPY_CHAR, NPY_IN_ARRAY);
    PyArrayObject* aoout_len = PyArray_FROM_OTF(oout_len, NPY_ULONG, NPY_IN_ARRAY);
    PyArrayObject* aoout_dptr = PyArray_FROM_OTF(oout_dptr, NPY_DOUBLE, NPY_IN_ARRAY);
    
    char* field = (char*)PyArray_DATA(aofield);
    unsigned long* out_len = (unsigned long*)PyArray_DATA(aoout_len);
    float** out_dptr = &PyArray_DATA(aoout_dptr);
    
    void* out = PyCapsule_GetPointer(handle, NULL);
    
    XGDMatrixGetFloatInfo(handle, field, out_len, out_dptr);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGDMatrixGetUIntInfo(PyObject* self, PyObject* args){
    PyObject* handle;
    PyObject* ofield;
    PyObject* oout_len;
    PyObject* oout_dptr;
    
    if(!PyArg_ParseTuple(args, "OOOO", &handle, &ofield, &oout_len, &oout_dptr)){
        return NULL;
    }
    
    PyArrayObject* aofield = PyArray_FROM_OTF(ofield, NPY_CHAR, NPY_IN_ARRAY);
    PyArrayObject* aoout_len = PyArray_FROM_OTF(oout_len, NPY_ULONG, NPY_IN_ARRAY);
    PyArrayObject* aoout_dptr = PyArray_FROM_OTF(oout_dptr, NPY_UINT, NPY_IN_ARRAY);
    
    char* field = (char*)PyArray_DATA(aofield);
    unsigned* out_len = (unsigned*)PyArray_DATA(aoout_len);
    float** out_dptr = &PyArray_DATA(aoout_dptr);
    
    void* out = PyCapsule_GetPointer(handle, NULL);
    
    XGDMatrixGetUIntInfo(handle, field, out_len, out_dptr);
    
    return Py_BuildValue("");

}

static PyObject* Py_XGDMatrixNumRow(PyObject* self, PyObject* args){
    PyObject* ohandle;
    PyObject* oout;
    
    if(!PyArg_ParseTuple(args, "OO", &ohandle, &oout)){
        return NULL;
    }
    
    PyArrayObject* aoout  = PyArray_FROM_OTF(oout, NPY_ULONG, NPY_IN_ARRAY);
    unsigned long* out = PyArray_DATA(aoout);
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGDMatrixNumRow(handle, out);
    
    return Py_BuildValue("");   
}

static PyObject* Py_XGDMatrixNumCol(PyObject* self, PyObject* args){
    PyObject* ohandle;
    PyObject* oout;
    
    if(!PyArg_ParseTuple(args, "OO", &ohandle, &oout)){
        return NULL;
    }
    
    PyArrayObject* aoout  = PyArray_FROM_OTF(oout, NPY_ULONG, NPY_IN_ARRAY);
    unsigned long* out = PyArray_DATA(aoout);
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGDMatrixNumCol(handle, out);
    
    return Py_BuildValue("");   
}

static PyObject* Py_XGBoosterCreate(PyObject* self, PyObject* args){
    PyObject* ohandle;
    unsigned long len;
    PyObject* oout;
    
    if(!PyArg_ParseTuple(args, "OkO", &ohandle, &len, &oout)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void** out = &PyCapsule_GetPointer(oout, NULL);
    
    XGBoosterCreate(handle, len , out);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterFree(PyObject* self, PyObject* args){

    PyObject* ohandle;
    
    if(!PyArg_ParseTuple(args, "O", &ohandle)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGBoosterFree(handle);
    
    Py_BuildValue("");
}

static PyObject* Py_XGBoosterSetParam(PyObject* self, PyObject* args){
    PyObject* ohandle;
    char* name;
    char* value;
    
    if(!PyArg_ParseTuple(args, "Oss", &ohandle, &name, &value)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGBoosterSetParam(handle, name, value);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterUpdateOneIter(PyObject* self, PyObject* args){
    PyObject* ohandle;
    int iter;
    PyObject* odtrain;
    
    if(!PyArg_ParseTuple(args, "OiO", &ohandle, &iter, &odtrain)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void* dtrain = PyCapsule_GetPointer(odtrain, NULL);
    
    XGBoosterUpdateOneIter(handle, iter, dtrain);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterBoostOneIter(PyObject* self, PyObject* args){
    PyObject* ohandle;
    PyObject* odtrain;
    PyObject* ograd;
    PyObject* ohess;
    unsigned long len;
    
    if(!PyArg_ParseTuple(args, "OOOOk", &ohandle, &odtrain, &ograd, &ohess, &len)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void* dtrain = PyCapsule_GetPointer(odtrain, NULL);
    
    PyArrayObject* aograd = PyArray_FROM_OTF(ograd, NPY_DOUBLE, NPY_IN_ARRAY);
    PyArrayObject* aohess = PyArray_FROM_OTF(ohess, NPY_DOUBLE, NPY_IN_ARRAY);
    
    float* grad = (float*)PyArray_DATA(aograd);
    float* hess = (float*)PyArray_DATA(aohess);
    
    XGBoosterBoostOneIter(handle, dtrain, grad, hess, len);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterEvalOneIter(PyObject* self, PyObject* args){
    PyObject* ohandle;
    int iter;
    PyObject* odmats;
    PyObject* oevnames;
    unsigned long len;
    char* oout_result;
    
    if(!PyArg_ParseTuple(args, "OiOOkO", &ohandle, &iter, &odmats, &oevnames, &len, &oout_result)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void** dmats = &PyCapsule_GetPointer(odmats, NULL);
    
    //....char**
    
    XGBoosterEvalOneIter(handle, iter, dmats, evnames, len, out_result);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterPredict(PyObject* self, PyObject* args){
    PyObject* ohandle;
    PyObject* odmat;
    int option_mask;
    unsigned ntree_limit;
    PyObject* oout_len;
    PyObject* oout_result;
    
    if(!PyArg_ParseTuple(args, "OOikOO", &ohandle, &odmat, &option_mask, &ntree_limit, &oout_len, &oout_result)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void* odmat = PyCapsule_GetPointer(ohandle, NULL);
    
    PyArrayObject* aoout_len = PyArray_FROM_OTF(oout_len, NPY_ULONG, NPY_IN_ARRAY);
    PyArrayObject* aoout_result = PyArray_FROM_OTF(oout_result, NPY_DOUBLE, NPY_IN_ARRAY);
    
    unsigned long* out_len = (unsigned long*)PyArray_DATA(aaout_len);
    float** out_result = &&(float*)PyArray_DATA(aoout_result);    

    XGBoosterPredict(handle, dmat, option_mask, ntree_limit, out_len, out_result);
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterLoadModel(PyObject* self, PyObject* args){
    PyObject* ohandle;
    char* fname;
    
    if(!PyArg_ParseTuple(args, "Os", &ohandle, &fname)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGBoosterLoadModel(handle, fname);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterSaveModel(PyObject* self, PyObject* args){
    PyObject* ohandle;
    char* fname;
    
    if(!PyArg_ParseTuple(args, "Os", &ohandle, &fname)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    
    XGBoosterSaveModel(handle, fname);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterLoadModelFromBuffer(PyObject* self, PyObject* args){
    PyObject* ohandle;
    PyObject* obuf;
    unsigned long len;
    
    if(!PyArg_ParseTuple(args, "OOk", &ohandle, &obuf, &len)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    void* buf = PyCapsule_GetPointer(obuf, NULL);
    
    XGBoosterLoadModelFromBuffer(handle, buf, len);
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterGetModelRaw(PyObject* self, PyObject* args){
   PyObject* ohandle;
   PyObject* oout_len;
   char** out_dptr;
   
   if(!PyArg_ParseTuple(args, "OOs", &ohandle, &oout_len, out_dptr)){
       return NULL;
   }
   
   void* handle = PyCapsule_GetPointer(ohandle, NULL);
   //unsgined long*
   XGBoosterGetModelRaw(handle, out_len, out_dptr);
   
   return Py_BuildValue("");
   
}


static PyObject* Py_XGBoosterDumpModelEx(PyObject* self, PyObject* args){

    PyObject* ohandle;
    char* fmap;
    int with_stats;
    char* format;
    PyObject* oout_len;
    char*** out_dump_array;
    
    ///
    
    return Py_BuildValue("");
}

static PyObject* Py_XGBoosterDumpModelExWithFeatures(PyObject* self, PyObject* args){

    PyObject* ohandle;
    int fnum;
    char* sfname;
    char* sftype;
    int with_stats;
    char* format;
    PyObject* oout_len;
    char*** out_models;
    
    if(!PyArg_ParseTuple(args, "OissisOs", &ohandle, &fnum, &sfname, &sftype, &with_stats, &format, &oout_len, &out_models)){
        return NULL;
    }
    
    void* handle = PyCapsule_GetPointer(ohandle, NULL);
    char** fname = &sfname;
    char** sftype = &
    
    return Py_BuildValue("");
}




static PyMethodDef module_functions={
    {"XGBGetLastError", Py_XGBGetLastError, METH_NOARGS, ""},
    {"XGDMatrixCreateFromFile", Py_XGDMatrixCreateFromFile, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSREx", Py_XGDMatrixCreateFromCSREx, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSCEx", Py_XGDMatrixCreateFromCSCEx, METH_VARARGS, ""},
    {"XGDMatrixCreateFromMat", Py_XGDMatrixCreateFromMat, METH_VARARGS, ""},
    {"XGDMatrixCreateFromDT", Py_XGDMatrixCreateFromDT, METH_VARARGS, ""},
    {"XGDMatrixSliceDMatrix", Py_XGDMatrixSliceDMatrix, METH_VARARGS, ""},
    {"XGDMatrixFree", Py_XGDMatrixFree, METH_VARARGS, ""},
    {NULL}
}

