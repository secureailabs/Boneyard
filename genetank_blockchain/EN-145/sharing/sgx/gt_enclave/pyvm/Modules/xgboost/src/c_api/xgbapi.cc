#include "../../include/xgboost/c_api.h"
#include "../../../../Include/Python.h"

/*!
 * \brief get string message of the last error
 *
 *  all function in this file will return 0 when success
 *  and -1 when an error occurred,
 *  XGBGetLastError can be called to retrieve the error
 *
 *  this function is thread safe and can be called by different thread
 * \return const char* error information
 */
static PyObject* Py_XGBGetLastError(PyObject* self, PyObject* noargs){
    const char* s = XGBGetLastError();
    return Py_BuildValue("s", s);
}
/*!
 * \brief register callback function for LOG(INFO) messages -- helpful messages
 *        that are not errors.
 * Note: this function can be called by multiple threads. The callback function
 *       will run on the thread that registered it
 * \return 0 for success, -1 for failure
 */
static PyObject* Py_XGBRegisterLogCallback(PyObject* self, PyObject* args){
    void (*callback)(const char*);
    
    PyArg_ParseTuple(args, "O", &callback);
    
    XGBRegisterLogCallback(callback);
    return Py_BuildValue("");
}
/*!
 * \brief load a data matrix
 * \param fname the name of the file
 * \param silent whether print messages during loading
 * \param out a loaded data matrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromFile(PyObject* self, PyObject* args){
    
    const char* fname;
    int silent;
    DMatrixHandle* out;
    
    PyArg_ParseTuple(args, "OiO", &fname, &silent, &out);
    
    XGDMatrixCreateFromFile(fname, silent, out);
    
    return Py_BuildValue("");
}

/*!
 * \brief Create a DMatrix from a data iterator.
 * \param data_handle The handle to the data.
 * \param callback The callback to get the data.
 * \param cache_info Additional information about cache file, can be null.
 * \param out The created DMatrix
 * \return 0 when success, -1 when failure happens.
 */
 
static PyObject* Py_XGDMatrixCreateFromDataIter(PyObject* self, PyObject* args){

    DataIterHandle data_handle;
    XGBCallbackDataIterNext* callback;
    char* cache_info;
    DMatrixHandle *out;
    
    PyArg_ParseTuple(args, "OOsO", &data_handle, &callback, &cache_info, &out);
    XGDMatrixCreateFromDataIter(data_handle, callback, cache_info, out);
    
    return Py_BuildValue("");
}
/*!
 * \brief create a matrix content from CSR format
 * \param indptr pointer to row headers
 * \param indices findex
 * \param data fvalue
 * \param nindptr number of rows in the matrix + 1
 * \param nelem number of nonzero elements in the matrix
 * \param num_col number of columns; when it's set to 0, then guess from data
 * \param out created dmatrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromCSREx(PyObject* self, PyObject* args){
    size_t* indptr;
    unsigned* indices;
    float* data;
    size_t nindptr;
    size_t nelem;
    size_t num_col;
    DMatrixHandle* out;
    
    PyArg_ParseTuple(args, "OOOkkkO", &indptr, &indices, &data, &nindptr, &nelem, &num_col, &out);

    XGDMatrixCreateFromCSREx(indptr, indices, data, nindptr, nelem, num_col, out);
    
    Py_BuildValue("");
}
/*!
 * \deprecated
 * \brief create a matrix content from CSR format
 * \param indptr pointer to row headers
 * \param indices findex
 * \param data fvalue
 * \param nindptr number of rows in the matrix + 1
 * \param nelem number of nonzero elements in the matrix
 * \param out created dmatrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromCSR(PyObject* self, PyObject* args){
 
    bst_ulong *indptr;
    unsigned* indices;
    float* data;
    bst_ulong nindptr;
    bst_ulong nelem;
    DMatrixHandle* out;
    
    PyArg_ParseTuple(args, "OOOkkO", &indptr, &indices, &data, &nindptr, &nelem, &out);
 
    XGDMatrixCreateFromCSR(indptr, indices, data, nindptr, nelem, out);
    
    Py_BuildValue("");
}
/*!
 * \brief create a matrix content from CSC format
 * \param col_ptr pointer to col headers
 * \param indices findex
 * \param data fvalue
 * \param nindptr number of rows in the matrix + 1
 * \param nelem number of nonzero elements in the matrix
 * \param num_row number of rows; when it's set to 0, then guess from data
 * \param out created dmatrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromCSCEx(PyObject* self, PyObject* args){
    size_t* col_ptr;
    unsigned* indices;
    float* data;
    size_t nindptr;
    size_t nelem;
    size_t num_row;
    DMatrixHandle* out;
    
    PyArg_ParseTuple(args, "OOOkkkO", &col_ptr, &indices, &data, &nindptr, &nelem, &num_row, &out);
 
    XGDMatrixCreateFromCSCEx(col_ptr, indices, data, nindptr, nelem, num_row, out);
    
    Py_BuildValue("");
}

/*!
 * \deprecated
 * \brief create a matrix content from CSC format
 * \param col_ptr pointer to col headers
 * \param indices findex
 * \param data fvalue
 * \param nindptr number of rows in the matrix + 1
 * \param nelem number of nonzero elements in the matrix
 * \param out created dmatrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromCSC(PyObject* self, PyObject* args){
    bst_ulong *col_ptr;
    unsigned *indices;
    float *data;
    bst_ulong nindptr;
    bst_ulong nelem;
    DMatrixHandle *out;
    
    PyArg_ParseTuple(args, "OOOkkO", &col_ptr, &indices, &data, &nindptr, &nelem, &out);
 
    XGDMatrixCreateFromCSC(col_ptr, indices, data, nindptr, nelem, out);
    
    Py_BuildValue("");
}

/*!
 * \brief create matrix content from dense matrix
 * \param data pointer to the data space
 * \param nrow number of rows
 * \param ncol number columns
 * \param missing which value to represent missing value
 * \param out created dmatrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromMat(PyObject* self, PyObject* args){
    float *data;
    bst_ulong nrow;
    bst_ulong ncol;
    float missing;
    DMatrixHandle *out;
    
    PyArg_ParseTuple(args, "OkkfO", &data, &nrow, &ncol, &missing, &out);
 
    XGDMatrixCreateFromMat(data, nrow, ncol, missing, out);
    
    Py_BuildValue("");
}

/*!
 * \brief create matrix content from dense matrix
 * \param data pointer to the data space
 * \param nrow number of rows
 * \param ncol number columns
 * \param missing which value to represent missing value
 * \param out created dmatrix
 * \param nthread number of threads (up to maximum cores available, if <=0 use all cores)
 * \return 0 when success, -1 when failure happens
 */
//static PyObject* Py_XGDMatrixCreateFromCSREx(PyObject* self, PyObject* args){
//    float *data,  // NOLINT
//    bst_ulong nrow, 
//    bst_ulong ncol,
//    float missing, 
//    DMatrixHandle *out,
//    int nthread
// 
// 
// int XGDMatrixCreateFromMat_omp(const float *data,  // NOLINT
//                                       bst_ulong nrow, bst_ulong ncol,
//                                       float missing, DMatrixHandle *out,
//                                       int nthread);
//    
//    Py_BuildValue("");
//}

/*!
 * \brief create matrix content from python data table
 * \param data pointer to pointer to column data
 * \param feature_stypes pointer to strings
 * \param nrow number of rows
 * \param ncol number columns
 * \param out created dmatrix
 * \param nthread number of threads (up to maximum cores available, if <=0 use all cores)
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixCreateFromDT(PyObject* self, PyObject* args){

    void** data;
    const char** feature_types;
    bst_ulong nrow;
    bst_ulong ncol;
    DMatrixHandle* out;
    int nthread;
    
    PyArg_ParseTuple(args, "OOkkOi", &data, &feature_types, &nrow, &ncol, &out, &nthread);

    XGDMatrixCreateFromDT(data, feature_types, nrow, ncol, out, nthread);
    
    Py_BuildValue("");
}

/*!
 * \brief create a new dmatrix from sliced content of existing matrix
 * \param handle instance of data matrix to be sliced
 * \param idxset index set
 * \param len length of index set
 * \param out a sliced new matrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixSliceDMatrix(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    int *idxset;
    bst_ulong len;
    DMatrixHandle *out;
    
    PyArg_ParseTuple(args, "OOkO", &handle, &idxset, &len, &out);
 
    XGDMatrixSliceDMatrix(handle, idxset, len, out);

    Py_BuildValue("");
}
/*!
 * \brief free space in data matrix
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixFree(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    
    PyArg_ParseTuple(args, "O", &handle);
    
    XGDMatrixFree(handle);
    
    Py_BuildValue("");
}
/*!
 * \brief load a data matrix into binary file
 * \param handle a instance of data matrix
 * \param fname file name
 * \param silent print statistics when saving
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixSaveBinary(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    char *fname;
    int silent;
    
    PyArg_ParseTuple(args, "Osi", &handle, &fname, &silent);
 
    XGDMatrixSaveBinary(handle, fname, silent);
    
    Py_BuildValue("");
}

/*!
 * \brief set float vector to a content in info
 * \param handle a instance of data matrix
 * \param field field name, can be label, weight
 * \param array pointer to float vector
 * \param len length of array
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixSetFloatInfo(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    char *field;
    float *array;
    bst_ulong len;
    
    PyArg_ParseTuple(args, "OsOk", &handle, &field, &array, &len);
 
    XGDMatrixSetFloatInfo(handle, field, array, len);
    
    Py_BuildValue("");
}

/*!
 * \brief set uint32 vector to a content in info
 * \param handle a instance of data matrix
 * \param field field name
 * \param array pointer to unsigned int vector
 * \param len length of array
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixSetUIntInfo(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    char *field;
    unsigned *array;
    bst_ulong len;
    
    PyArg_ParseTuple(args, "OsOk", &handle, &field, &array, &len);

    XGDMatrixSetUIntInfo(handle, field, array, len);

    Py_BuildValue("");
}

/*!
 * \brief set label of the training matrix
 * \param handle a instance of data matrix
 * \param group pointer to group size
 * \param len length of array
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixSetGroup(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    unsigned *group;
    bst_ulong len;
    
    PyArg_ParseTuple(args, "OOk", &handle, &group, &len);
    XGDMatrixSetGroup(handle, group, len);
    
    Py_BuildValue("");
}

/*!
 * \brief get float info vector from matrix
 * \param handle a instance of data matrix
 * \param field field name
 * \param out_len used to set result length
 * \param out_dptr pointer to the result
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixGetFloatInfo(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    char *field;
    bst_ulong* out_len;
    const float **out_dptr;
 
    PyArg_ParseTuple(args, "OsOO", &handle, &field, &out_len, &out_dptr);
    
    XGDMatrixGetFloatInfo(handle, field, out_len, out_dptr);
    
    Py_BuildValue("");
}

/*!
 * \brief get uint32 info vector from matrix
 * \param handle a instance of data matrix
 * \param field field name
 * \param out_len The length of the field.
 * \param out_dptr pointer to the result
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixGetUIntInfo(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    char *field;
    bst_ulong* out_len;
    const unsigned **out_dptr;
    
    PyArg_ParseTuple(args, "OsOO", &handle, &field, &out_len, &out_dptr);
 
    XGDMatrixGetUIntInfo(handle, field, out_len, out_dptr);
    
    Py_BuildValue("");
}

/*!
 * \brief get number of rows.
 * \param handle the handle to the DMatrix
 * \param out The address to hold number of rows.
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixNumRow(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    bst_ulong *out;
    
    PyArg_ParseTuple(args, "OO", &handle, &out);
 
    XGDMatrixNumRow(handle, out);
    
    Py_BuildValue("");
}

/*!
 * \brief get number of columns
 * \param handle the handle to the DMatrix
 * \param out The output of number of columns
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGDMatrixNumCol(PyObject* self, PyObject* args){
    DMatrixHandle handle;
    bst_ulong *out;
    
    PyArg_ParseTuple(args, "OO", &handle, &out);
 
    XGDMatrixNumCol(handle, out);
    
    Py_BuildValue("");
}
// --- start XGBoost class
/*!
 * \brief create xgboost learner
 * \param dmats matrices that are set to be cached
 * \param len length of dmats
 * \param out handle to the result booster
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterCreate(PyObject* self, PyObject* args){
    DMatrixHandle* dmats;
    bst_ulong len;
    BoosterHandle *out;
    
    PyArg_ParseTuple(args, "OkO", &dmats, &len, &out); 
 
    XGBoosterCreate(dmats, len, out);
    
    Py_BuildValue("");
}

/*!
 * \brief free obj in handle
 * \param handle handle to be freed
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterFree(PyObject* self, PyObject* args){
    BoosterHandle handle;
    
    PyArg_ParseTuple(args, "O", &handle);
 
    XGBoosterFree(handle);
    
    Py_BuildValue("");
}

/*!
 * \brief set parameters
 * \param handle handle
 * \param name  parameter name
 * \param value value of parameter
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterSetParam(PyObject* self, PyObject* args){
     BoosterHandle handle;
     char *name;
     char *value;
     
     PyArg_ParseTuple(args, "Oss", &handle, &name, &value); 
 
    XGBoosterSetParam(handle, name, value);
    
    Py_BuildValue("");
}

/*!
 * \brief update the model in one round using dtrain
 * \param handle handle
 * \param iter current iteration rounds
 * \param dtrain training data
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterUpdateOneIter(PyObject* self, PyObject* args){
    BoosterHandle handle;
    int iter;
    DMatrixHandle dtrain;
    
    PyArg_ParseTuple(args, "OiO", &handle, &iter, &dtrain);
 
    XGBoosterUpdateOneIter(handle, iter, dtrain);
    
    Py_BuildValue("");
}

/*!
 * \brief update the model, by directly specify gradient and second order gradient,
 *        this can be used to replace UpdateOneIter, to support customized loss function
 * \param handle handle
 * \param dtrain training data
 * \param grad gradient statistics
 * \param hess second order gradient statistics
 * \param len length of grad/hess array
 * \return 0 when success, -1 when failure happens
 */

static PyObject* Py_XGBoosterBoostOneIter(PyObject* self, PyObject* args){
    BoosterHandle handle;
    DMatrixHandle dtrain;
    float *grad;
    float *hess;
    bst_ulong len;
    
    PyArg_ParseTuple(args, "OOOOk", &handle, &dtrain, &grad, &hess, &len);

    XGBoosterBoostOneIter(handle, dtrain, grad, hess, len);
    
    Py_BuildValue("");
}

/*!
 * \brief get evaluation statistics for xgboost
 * \param handle handle
 * \param iter current iteration rounds
 * \param dmats pointers to data to be evaluated
 * \param evnames pointers to names of each data
 * \param len length of dmats
 * \param out_result the string containing evaluation statistics
 * \return 0 when success, -1 when failure happens
 */

static PyObject* Py_XGBoosterEvalOneIter(PyObject* self, PyObject* args){
    BoosterHandle handle;
    int iter;
    DMatrixHandle* dmats;
    const char **evnames;
    bst_ulong len;
    const char **out_result;

    PyArg_ParseTuple(args, "OiOOkO", &handle, &iter, &dmats, &evnames, &len, &out_result);
    
    XGBoosterEvalOneIter(handle, iter, dmats, evnames, len, out_result);
    
    Py_BuildValue("");
}

/*!
 * \brief make prediction based on dmat
 * \param handle handle
 * \param dmat data matrix
 * \param option_mask bit-mask of options taken in prediction, possible values
 *          0:normal prediction
 *          1:output margin instead of transformed value
 *          2:output leaf index of trees instead of leaf value, note leaf index is unique per tree
 *          4:output feature contributions to individual predictions
 * \param ntree_limit limit number of trees used for prediction, this is only valid for boosted trees
 *    when the parameter is set to 0, we will use all the trees
 * \param out_len used to store length of returning result
 * \param out_result used to set a pointer to array
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterPredict(PyObject* self, PyObject* args){
    BoosterHandle handle;
    DMatrixHandle dmat;
    int option_mask;
    unsigned ntree_limit;
    bst_ulong *out_len;
    const float **out_result;
    
    PyArg_ParseTuple(args, "OOiIkO", &handle, &dmat, &option_mask, &ntree_limit, &out_len, &out_result);
 
    XGBoosterPredict(handle, dmat, option_mask, ntree_limit, out_len, out_result);
    
    Py_BuildValue("");
}

/*!
 * \brief load model from existing file
 * \param handle handle
 * \param fname file name
* \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterLoadModel(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char *fname;
    
    PyArg_ParseTuple(args, "Os", &handle, &fname);
 
    XGBoosterLoadModel(handle, fname);
    
    Py_BuildValue("");
}

/*!
 * \brief save model into existing file
 * \param handle handle
 * \param fname file name
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterSaveModel(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char *fname;
    
    PyArg_ParseTuple(args, "Os", &handle, &fname);
 
    XGBoosterSaveModel(handle, fname);
    
    Py_BuildValue("");
}

/*!
 * \brief load model from in memory buffer
 * \param handle handle
 * \param buf pointer to the buffer
 * \param len the length of the buffer
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterLoadModelFromBuffer(PyObject* self, PyObject* args){
    BoosterHandle handle;
    void *buf;
    bst_ulong len;
    
    PyArg_ParseTuple(args, "OOk", &handle, &buf, &len);
    
    XGBoosterLoadModelFromBuffer(handle, buf, len);
    
    Py_BuildValue("");
}

/*!
 * \brief save model into binary raw bytes, return header of the array
 * user must copy the result out, before next xgboost call
 * \param handle handle
 * \param out_len the argument to hold the output length
 * \param out_dptr the argument to hold the output data pointer
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterGetModelRaw(PyObject* self, PyObject* args){
    
    BoosterHandle handle;
    bst_ulong *out_len;
    const char **out_dptr;
    
    PyArg_ParseTuple(args, "OOs", &handle, &out_len, &out_dptr);
    XGBoosterGetModelRaw(handle, out_len, out_dptr);
    
    Py_BuildValue("");
}

/*!
 * \brief dump model, return array of strings representing model dump
 * \param handle handle
 * \param fmap  name to fmap can be empty string
 * \param with_stats whether to dump with statistics
 * \param out_len length of output array
 * \param out_dump_array pointer to hold representing dump of each model
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterDumpModel(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char *fmap;
    int with_stats;
    bst_ulong *out_len;
    const char ***out_dump_array;
    
    PyArg_ParseTuple(args, "OsiOO", &handle, &fmap, &with_stats, &out_len, &out_dump_array);
 
    XGBoosterDumpModel(handle, fmap, with_stats, out_len, out_dump_array);

    
    Py_BuildValue("");
}

/*!
 * \brief dump model, return array of strings representing model dump
 * \param handle handle
 * \param fmap  name to fmap can be empty string
 * \param with_stats whether to dump with statistics
 * \param format the format to dump the model in
 * \param out_len length of output array
 * \param out_dump_array pointer to hold representing dump of each model
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterDumpModelEx(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char *fmap;
    int with_stats;
    char *format;
    bst_ulong *out_len;
    const char ***out_dump_array;
    
    PyArg_ParseTuple(args, "OsisOO", &handle, &fmap, &with_stats, &format, &out_len, &out_dump_array);
 
    XGBoosterDumpModelEx(handle, fmap, with_stats, format, out_len, out_dump_array);

    Py_BuildValue("");
}

/*!
 * \brief dump model, return array of strings representing model dump
 * \param handle handle
 * \param fnum number of features
 * \param fname names of features
 * \param ftype types of features
 * \param with_stats whether to dump with statistics
 * \param out_len length of output array
 * \param out_models pointer to hold representing dump of each model
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterDumpModelWithFeatures(PyObject* self, PyObject* args){

    BoosterHandle handle;
    int fnum;
    const char **fname;
    const char **ftype;
    int with_stats;
    bst_ulong *out_len;
    const char ***out_models;
    
    PyArg_ParseTuple(args, "OiOOkO", &handle, &fnum, &fname, &ftype, &with_stats, &out_len, &out_models);
    
    XGBoosterDumpModelWithFeatures(handle, fnum, fname, ftype, with_stats, out_len, out_models);

    
    Py_BuildValue("");
}

/*!
 * \brief dump model, return array of strings representing model dump
 * \param handle handle
 * \param fnum number of features
 * \param fname names of features
 * \param ftype types of features
 * \param with_stats whether to dump with statistics
 * \param format the format to dump the model in
 * \param out_len length of output array
 * \param out_models pointer to hold representing dump of each model
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterDumpModelExWithFeatures(PyObject* self, PyObject* args){

    BoosterHandle handle;
    int fnum;
    const char **fname;
    const char **ftype;
    int with_stats;
    char* format;
    bst_ulong *out_len;
    const char ***out_models;
    
    PyArg_ParseTuple(args, "OiOOskO", &handle, &fnum, &fname, &ftype, &with_stats, &format, &out_len, &out_models);
    
    XGBoosterDumpModelExWithFeatures(handle, fnum, fname, ftype, with_stats, format, out_len, out_models);
    
    Py_BuildValue("");
}

/*!
 * \brief Get string attribute from Booster.
 * \param handle handle
 * \param key The key of the attribute.
 * \param out The result attribute, can be NULL if the attribute do not exist.
 * \param success Whether the result is contained in out.
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterGetAttr(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char* key;
    const char** out;
    int *success;
    
    PyArg_ParseTuple(args, "OsOO", &handle, &key, &out, &success);
 
    XGBoosterGetAttr(handle, key, out, success);
    
    Py_BuildValue("");
}

/*!
 * \brief Set or delete string attribute.
 *
 * \param handle handle
 * \param key The key of the attribute.
 * \param value The value to be saved. 
 *              If nullptr, the attribute would be deleted.
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterSetAttr(PyObject* self, PyObject* args){
    BoosterHandle handle;
    char* key;
    char* value;
    
    PyArg_ParseTuple(args, "Oss", &handle, &key, &value);
 
    XGBoosterSetAttr(handle, key, value);
    
    Py_BuildValue("");
}

/*!
 * \brief Get the names of all attribute from Booster.
 * \param handle handle
 * \param out_len the argument to hold the output length
 * \param out pointer to hold the output attribute stings
 * \return 0 when success, -1 when failure happens
 */
static PyObject* Py_XGBoosterGetAttrNames(PyObject* self, PyObject* args){
    BoosterHandle handle;
    bst_ulong* out_len;
    const char*** out;
    
    PyArg_ParseTuple(args, "OOO", &handle, &out_len, &out);
 
    XGBoosterGetAttrNames(handle, out_len, out);
    
    Py_BuildValue("");
}

// --- Distributed training API----
// NOTE: functions in rabit/c_api.h will be also available in libxgboost.so
/*!
 * \brief Initialize the booster from rabit checkpoint.
 *  This is used in distributed training API.
 * \param handle handle
 * \param version The output version of the model.
 * \return 0 when success, -1 when failure happens
 */
//static PyObject* Py_XGDMatrixCreateFromCSREx(PyObject* self, PyObject* args){
// int XGBoosterLoadRabitCheckpoint(
//    BoosterHandle handle,
//    int* version);

    
//    Py_BuildValue("");
//}

/*!
 * \brief Save the current checkpoint to rabit.
 * \param handle handle
 * \return 0 when success, -1 when failure happens
 */
//static PyObject* Py_XGDMatrixCreateFromCSREx(PyObject* self, PyObject* args){
// int XGBoosterSaveRabitCheckpoint(BoosterHandle handle);
 
    
//    Py_BuildValue("");
//}

static struct PyMethodDef module_functions[]={
    {"XGBGetLastError", Py_XGBGetLastError, METH_NOARGS, ""},
    {"XGBRegisterLogCallback", Py_XGBRegisterLogCallback, METH_VARARGS, ""},
    {"XGDMatrixCreateFromFile", Py_XGDMatrixCreateFromFile, METH_VARARGS, ""},
    {"XGDMatrixCreateFromDataIter", Py_XGDMatrixCreateFromDataIter, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSREx", Py_XGDMatrixCreateFromCSREx, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSR", Py_XGDMatrixCreateFromCSR, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSCEx", Py_XGDMatrixCreateFromCSCEx, METH_VARARGS, ""},
    {"XGDMatrixCreateFromCSC", Py_XGDMatrixCreateFromCSC, METH_VARARGS, ""},
    {"XGDMatrixCreateFromMat", Py_XGDMatrixCreateFromMat, METH_VARARGS, ""},
    {"XGDMatrixCreateFromDT", Py_XGDMatrixCreateFromDT, METH_VARARGS, ""},
    {"XGDMatrixSliceDMatrix", Py_XGDMatrixSliceDMatrix, METH_VARARGS, ""},
    {"XGDMatrixFree", Py_XGDMatrixFree, METH_VARARGS, ""},
    {"XGDMatrixSaveBinary", Py_XGDMatrixSaveBinary, METH_VARARGS, ""},
    {"XGDMatrixSetFloatInfo", Py_XGDMatrixSetFloatInfo, METH_VARARGS, ""},
    {"XGDMatrixSetUIntInfo", Py_XGDMatrixSetUIntInfo, METH_VARARGS, ""},
    {"XGDMatrixSetGroup", Py_XGDMatrixSetGroup, METH_VARARGS, ""},
    {"XGDMatrixGetFloatInfo", Py_XGDMatrixGetFloatInfo, METH_VARARGS, ""},
    {"XGDMatrixGetUIntInfo", Py_XGDMatrixGetUIntInfo, METH_VARARGS, ""},
    {"XGDMatrixNumRow", Py_XGDMatrixNumRow, METH_VARARGS, ""},
    {"XGDMatrixNumCol", Py_XGDMatrixNumCol, METH_VARARGS, ""},
    {"XGBoosterCreate", Py_XGBoosterCreate, METH_VARARGS, ""},
    {"XGBoosterFree", Py_XGBoosterFree, METH_VARARGS, ""},
    {"XGBoosterSetParam", Py_XGBoosterSetParam, METH_VARARGS, ""},
    {"XGBoosterUpdateOneIter", Py_XGBoosterUpdateOneIter, METH_VARARGS, ""},
    {"XGBoosterBoostOneIter", Py_XGBoosterBoostOneIter, METH_VARARGS, ""},
    {"XGBoosterEvalOneIter", Py_XGBoosterEvalOneIter, METH_VARARGS, ""},
    {"XGBoosterPredict", Py_XGBoosterPredict, METH_VARARGS, ""},
    {"XGBoosterLoadModel", Py_XGBoosterLoadModel, METH_VARARGS, ""},
    {"XGBoosterSaveModel", Py_XGBoosterSaveModel, METH_VARARGS, ""},
    {"XGBoosterLoadModelFromBuffer", Py_XGBoosterLoadModelFromBuffer, METH_VARARGS, ""},
    {"XGBoosterGetModelRaw", Py_XGBoosterGetModelRaw, METH_VARARGS, ""},
    {"XGBoosterDumpModel", Py_XGBoosterDumpModel, METH_VARARGS, ""},
    {"XGBoosterDumpModelEx", Py_XGBoosterDumpModelEx, METH_VARARGS, ""},
    {"XGBoosterDumpModelWithFeatures", Py_XGBoosterDumpModelWithFeatures, METH_VARARGS, ""},
    {"XGBoosterDumpModelExWithFeatures", Py_XGBoosterDumpModelExWithFeatures, METH_VARARGS, ""},
    {"XGBoosterGetAttr", Py_XGBoosterGetAttr, METH_VARARGS, ""},
    {"XGBoosterSetAttr", Py_XGBoosterSetAttr, METH_VARARGS, ""},
    {"XGBoosterGetAttrNames", Py_XGBoosterGetAttrNames, METH_VARARGS, ""},
    {NULL}
};

PyMODINIT_FUNC
init_xgbapi(void)
{
    Py_InitModule("_xgbapi", module_functions);
}
