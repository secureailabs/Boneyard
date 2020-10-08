#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "../../include/data_management.h"
#include "../../include/dir_management.h" // call dmReadFile and dmWriteFile
#include "../../include/dir_cache.h"
#include "../../include/data_inspector.h"
#include "../../include/enclave_view.h"
#define FIX_FORMAT(fmt) (fmt == NULL ? "B" : fmt)

static PyObject *PyFile_AccessError = NULL;
static PyObject *PyPrint_IOError = NULL;

void *fp;

// opens an external file
static PyObject *ext_file_open(PyObject *self, PyObject *args){
	const char *name = "pytest_output.txt";
	const char *mode = "w+";
	sgx_status_t ret;

	ret = oc_fopen(&fp, name, mode);
	if ( SGX_SUCCESS != ret ) {
		PyErr_SetString(PyFile_AccessError, "Error while opening the file.");
		return NULL;
	}

	return PyLong_FromLong(1);
}

// writes to an external file
static PyObject *ext_file_write(PyObject *self, PyObject *args){
	char* message;
	int len = 0;
	size_t rval;
	size_t size = sizeof(char);

	if (!PyArg_ParseTuple(args, "si", &message, &len)) {  // parse arguments
		return NULL;
	}

	if (SGX_SUCCESS != oc_fwrite(&rval, (const void *)message, size * len, size, len, fp)){
		PyErr_SetString(PyFile_AccessError, "Error while writing to the file.");
		return NULL;
	}

	return PyLong_FromLong(1);
}

// closes external file
static PyObject *ext_file_close(PyObject *self, PyObject *args){
	int32_t r;

	if (SGX_SUCCESS != oc_fclose(&r, fp)){
		PyErr_SetString(PyFile_AccessError, "Error while closing the file.");
		return NULL;
	}

	return PyLong_FromLong(1);
}

// reads test paths from /test/file_list.txt
static PyObject *file_read(PyObject *self, PyObject *args) {
	const char* name = "file_list.txt";
	sgx_status_t ret;
	PyObject* data = NULL;
	size_t size = 400000;
	uint8_t *buf = ( uint8_t * )malloc(size);

	ret = dmReadDataOrModel("test.dir", name, buf, &size);

	if ( SGX_SUCCESS != ret ) {
		PyErr_SetString(PyFile_AccessError, "Error while reading from the file.");   // ERROR: file not found error
		return NULL;
	}

	data = Py_BuildValue("s", buf);
	free(buf);
	return data;
}

// lists files in a directory
static PyObject *list_dir(PyObject *self, PyObject *args) {
	const char* name;
	sgx_status_t ret;

	if (!PyArg_ParseTuple(args, "s", &name))
    return NULL;

	ret = dmListDir(name);

	if ( SGX_SUCCESS != ret ) {
		PyErr_SetString(PyFile_AccessError, "Error while reading from the directory.");
		return NULL;
	}

	return PyLong_FromLong(1);
}

static PyMethodDef FileAccessMethods[] = {
	{"file_open", ext_file_open, METH_VARARGS, "Python interface to open and external file."},
	{"file_write", ext_file_write, METH_VARARGS, "Python interface to write to an external file."},
	{"file_close", ext_file_close, METH_VARARGS, "Python interface to close an external file."},
	{"file_read", file_read, METH_VARARGS, "Python interface to read from a file in the SFS."},
	{"list_dir", list_dir, METH_VARARGS, "Python interface to list files in a directory in the SFS."},
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC init_fileaccess(void) {
	PyObject *m;

	m = Py_InitModule("_fileaccess", FileAccessMethods);
	if( m == NULL ) { return; }

	PyFile_AccessError = PyErr_NewException("fileaccess.error", NULL, NULL);
	Py_INCREF(PyFile_AccessError);
	PyModule_AddObject(m, "error", PyFile_AccessError);

	PyPrint_IOError = PyErr_NewException("print.error", NULL, NULL);
	Py_INCREF(PyPrint_IOError);
	PyModule_AddObject(m, "error", PyPrint_IOError);
}
