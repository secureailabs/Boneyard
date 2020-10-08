/*
 * _httpsendmodule.c
 *
 *  Created on: Jan 14, 2020
 *      Author: accuser
 */

#include "Python.h"
#include "../../include/session.h"

static PyObject *
http_send(PyObject *self, PyObject *args);

static PyObject *HttpSendError;

static PyMethodDef HttpSendMethods[] = {
    {"http_send", http_send, METH_VARARGS, "Call a Rest API"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
init_httpsend(void)
{
    PyObject *m;

    m = Py_InitModule("_httpsend", HttpSendMethods);
    if (m == NULL)
        return;

    HttpSendError = PyErr_NewException("httpsend.error", NULL, NULL);
    Py_INCREF(HttpSendError);
    PyModule_AddObject(m, "error", HttpSendError);
}

static PyObject *
http_send(PyObject *self, PyObject *args)
{
	char *request;
	if (!PyArg_ParseTuple(args, "s", &request))
	        return NULL;
    char *rsp = oc_http_send(request);
    PyObject *ret;
    if (NULL != rsp){
    	ret = Py_BuildValue("s", rsp);
        free(rsp);
    } else
    	ret = Py_BuildValue("s", "{}");
    return ret;
}

