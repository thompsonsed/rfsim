/**
 * @brief Contains the logic for creating a python module and generating class elements.
 */
#include <Python.h>

#include <vector>
#include <string>


#ifndef CLIB_IMPORT
#define CLIB_IMPORT
using namespace std;
struct module_state
{
	PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

/**
 * @brief A Python error container for all run-time errors.
 */
static PyObject *librfsimError;


// Conditional compilation for Python >= 3.0 (changed how Python integration worked)
#if PY_MAJOR_VERSION >= 3
#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_librfsim(void)
#else
#define INITERROR return

PyMODINIT_FUNC
initlibrfsim(void)
#endif
;
#endif
