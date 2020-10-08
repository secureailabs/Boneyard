/*
 * _federatedmodeule.c
 *
 *  Created on: Sep 18, 2019
 *      Author: slowloris
 */

#include "Python.h"
#include "task_manager_enclave.h"
#include "queue.h"

static PyObject*
startAllSub(PyObject *self, PyObject *args){
	char* projectName;
	if (!PyArg_ParseTuple(args, "s", &projectName))
	        return NULL;

	sgx_status_t ret = runSubEnclaveCode(projectName);
	if(SGX_SUCCESS!=ret){
		return NULL;
	}
	return Py_BuildValue("");
}

static PyObject*
submitResult(PyObject *self, PyObject *args){
    char* subRes;

    if (!PyArg_ParseTuple(args, "s", &subRes))
    	return NULL;

    sgx_status_t ret = setSubEnclaveSendQueue((uint8_t*)subRes, strlen(subRes), SIMFL_DATA_UNKNOWN);
    if(SGX_SUCCESS!=ret){
    	return NULL;
    }

    return Py_BuildValue("");
}

static PyObject*
collectSubResult(PyObject *self, PyObject *noargs){
   char* res;
   size_t len;

   res = (char*)subResultQueue.deQueue(&len, NULL);

   if(res == NULL)
       return Py_None;

   return Py_BuildValue("s#", res, len);
}

static PyObject*
runSingleAlgo(PyObject* self, PyObject* args){
	char* sub_enclave;
	char* OPAL_algo;
	char* secret_algo;
	char* dataset;
	char* output = NULL;

	if (!PyArg_ParseTuple(args, "ssss", &secret_algo, &dataset, &sub_enclave, &OPAL_algo)){
    	PyObject* out = Py_BuildValue("s","{\"error\":\"Parse parameter failed.\"}");
	    return out;
	}

    sgx_status_t ret = runSingleCode(sub_enclave, OPAL_algo, secret_algo, dataset, &output);

    if(SGX_SUCCESS!=ret){
    	PyObject* out = Py_BuildValue("s","{\"error\":\"Call runSingleCode() failed.\"}");
	    return out;
    }

    if (NULL != output){
    	PyObject* out = Py_BuildValue("s",output);
    	free(output);
    	return out;
    }
	PyObject* out = Py_BuildValue("s","{\"error\":\"Call runSingleCode() having no outputs.\"}");
    return out;
}

static PyObject*
getNumSubEnclaves(PyObject *self, PyObject *args){
    int ret = numSubEnclaves();
    if(ret <= 0){
    	return NULL;
    }
    return Py_BuildValue("i", ret);
}

static PyObject*
subSendDataToHub(PyObject *self, PyObject *args){
    char* lsh;
    size_t len;
    simfl_data_t data_type;

    if (!PyArg_ParseTuple(args, "si", &lsh, &data_type))
    	return NULL;

    sgx_status_t ret = setSubEnclaveSendQueue((uint8_t*)lsh, strlen(lsh), data_type);

	return Py_BuildValue("");
}

static PyObject*
subCollectHubData(PyObject *self, PyObject *args){
    uint8_t* res;
    size_t len;
    simfl_data_t data_type;

    if (!PyArg_ParseTuple(args, "i", &data_type))
        return NULL;

    res = getSubEnclaveRcvQueue(data_type, &len);

    if(res == NULL)
        return Py_None;

    PyObject *ret = Py_BuildValue("s#", res, len);
    free(res);
    return ret;
}

static PyObject*
hubSendDataToSub(PyObject *self, PyObject *args){
	char* projectName;
    uint8_t* data;
    int enclaveID;
    uint32_t dataLen;
    uint32_t dataType;

	if (!PyArg_ParseTuple(args, "isii", &enclaveID, &data, &dataLen, &dataType))
	        return NULL;

	sgx_status_t ret = sendToSubEnclave(enclaveID, data, dataLen, dataType);
	if(SGX_SUCCESS!=ret) {
		return NULL;
	}

    return Py_BuildValue("");
}

static PyObject*
hubCollectSubData(PyObject *self, PyObject *args){
    int32_t encId;
    uint32_t data_type;
	if (!PyArg_ParseTuple(args, "ii", &encId, &data_type))
	        return NULL;

	sgx_status_t ret = recvFromSubEnclave(encId, data_type);
	if(SGX_SUCCESS!=ret) {
		return NULL;
	}
    char* res;
    size_t len;

    res = (char*)subSimFlLSHQueue.deQueue(&len, NULL);

    PyObject *pyobj = Py_BuildValue("s#", res, len);
    free(res);
    return pyobj;
}

static PyMethodDef
module_functions[] = {
	{ "startAllSub", startAllSub, METH_VARARGS, "" },
	{ "submitResult", submitResult, METH_VARARGS, "" },
	{ "runSingleAlgo", runSingleAlgo, METH_VARARGS, "" },
	{ "collectSubResult", collectSubResult, METH_NOARGS, ""},
	{ "getNumSubEnclaves", getNumSubEnclaves, METH_NOARGS, ""},
	{ "subSendDataToHub", subSendDataToHub, METH_VARARGS, "" },
	{ "subCollectHubData", subCollectHubData, METH_VARARGS, ""},
	{ "hubSendDataToSub", hubSendDataToSub, METH_VARARGS, ""},
	{ "hubCollectSubData", hubCollectSubData, METH_VARARGS, ""},
	{ NULL }
};

PyMODINIT_FUNC
init_federated(void)
{
    Py_InitModule("_federated", module_functions);
}
