/**
 * @brief Contains the logic for creating a python module and generating class elements.
 */
#ifndef LIB_CLIB_H
#define LIB_CLIB_H
//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#define PYTHON_COMPILE
#include <Python.h>
#include <string>
#include <vector>
#include <csignal>
#include "clib.h"
#include "numpy/arrayobject.h"

#include "PyWrapper.h"
using namespace std;
#if PY_MAJOR_VERSION < 3
static PyMethodDef LibMethods[] =
        {
                {NULL, NULL, 0 , NULL}
        };
#endif // PY_MAJOR_VERSION



inline void readyPyTypeObject(PyTypeObject * obj)
{
    if (PyType_Ready(obj) < 0)
    {
        throw runtime_error("Cannot initialise PyTypeObject.");
    }
    Py_INCREF(obj);
}

#if PY_MAJOR_VERSION >= 3
static PyModuleDef genPyModuleDef()
{
	PyModuleDef tmpModuleDef = {
			PyModuleDef_HEAD_INIT,
	};
	tmpModuleDef.m_name = "librfsim";
	tmpModuleDef.m_doc = "Wrapper for c++ library which performs simulations of rabbits and foxes on a landscape.";
	tmpModuleDef.m_size = -1;
	return tmpModuleDef;
}
static PyModuleDef moduledef = genPyModuleDef();


#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_librfsim(void)
#else
#define INITERROR return

PyMODINIT_FUNC
initlibrfsim(void)
#endif
{

    PyObject *module;
#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&moduledef);
#else
    module = Py_InitModule("librfsim", LibMethods);
#endif
    if(module == nullptr)
    {
        INITERROR;
    }
    try
    {
        readyPyTypeObject(&C_Landscape_Type);
    }
    catch(runtime_error &re)
    {
        INITERROR;
    }
    // Threading support
    if(!PyEval_ThreadsInitialized())
    {
        PyEval_InitThreads();

    }
    // This is required for numpy
    import_array1(nullptr);
    librfsimError = PyErr_NewException((char*)"librfsim.librfsimError", nullptr, nullptr);
    Py_INCREF(librfsimError);
    PyModule_AddObject(module, "librfsimError", librfsimError);
    PyModule_AddObject(module, (char*)"CLandscape", (PyObject *) &C_Landscape_Type);
#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}
#endif //LIB_CLIB_H
