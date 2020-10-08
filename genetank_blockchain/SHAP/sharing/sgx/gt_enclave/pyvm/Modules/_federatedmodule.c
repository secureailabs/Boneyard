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

    sgx_status_t ret = setSubEnclaveOutput((uint8_t*)subRes, strlen(subRes));
    if(SGX_SUCCESS!=ret){
    	return NULL;
    }

    return Py_BuildValue("");
}

static PyObject*
collectSubResult(PyObject *self, PyObject *noargs){
   char* res;
   size_t len;

   res = (char*)subResultQueue.deQueue(&len);

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

static PyMethodDef
module_functions[] = {
    { "startAllSub", startAllSub, METH_VARARGS, "" },
	{ "submitResult", submitResult, METH_VARARGS, "" },
	{ "runSingleAlgo", runSingleAlgo, METH_VARARGS, "" },
	{ "collectSubResult", collectSubResult, METH_NOARGS, ""},
    { NULL }
};

PyMODINIT_FUNC
init_federated(void)
{
    Py_InitModule("_federated", module_functions);
}
