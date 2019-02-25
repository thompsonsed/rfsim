/**
 * @brief Contains wrapper functions for the C Landscape class, exposing it to Python.
 */
#ifndef PY_WRAPPER
#define PY_WRAPPER

#include <Python.h>
#include <structmember.h>
#include <memory>
#include "Landscape.h"

/**
 * @brief Wrapper for the Landscape C class.
 */
class PyLandscape
{
public:
    PyObject_HEAD

    std::unique_ptr<Landscape> landscape = nullptr;

    virtual ~PyLandscape();

};

/**
 * @brief Traverse the PyLandscape object
 * @param self object to traverse
 */
static int
PyTemplate_traverse(PyLandscape* self, visitproc visit, void* arg)
{
    return 0;
}

/**
 * @brief Clear the PyLandscape object
 * @param self object to clear
 */
static int
PyTemplate_clear(PyLandscape* self)
{
    return 0;
}

/**
 * @brief Deallocate the PyLandscape object
 * @param self object to deallocate
 */
static void
PyTemplate_dealloc(PyLandscape* self)
{
    if(self->landscape != nullptr)
    {
        self->landscape.reset();
        self->landscape = nullptr;
    }
    PyObject_GC_UnTrack(self);
    PyTemplate_clear(self);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

/**
 * @brief Create a new PyLandscape object
 * @param args (empty) arguments for construction
 * @param kwds (empty) keyword arguments for construction
 * @return the new object
 */
static PyObject*
PyLandscape_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    PyLandscape* self;
    self = (PyLandscape*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

/**
 * @brief Initialise the PyLandscape object
 * @param self the object to initialise
 * @param args (empty) arguments for construction
 * @param kwds (empty) keyword arguments for construction
 * @return
 */
static int
PyTemplate_init(PyLandscape* self, PyObject* args, PyObject* kwds)
{
    self->landscape = std::make_unique<Landscape>();
    return 0;
}

/**
 * @brief Sets up the simulation with a particular size and random number seed.
 * @param self the Python self object
 * @param args arguments to parse
 */
static PyObject* setup(PyLandscape* self, PyObject* args)
{
    // Set up the simulation, catch and return any errors.
    unsigned long seed;
    unsigned long x_size, y_size;
    // parse arguments
    if(!PyArg_ParseTuple(args, "kkk", &seed, &x_size, &y_size))
    {
        return nullptr;
    }
    try
    {
        self->landscape->setSeed(seed);
        self->landscape->setLandscapeSize(x_size, y_size);
    }
    catch(exception &e)
    {
        return nullptr;
    }
    Py_RETURN_NONE;
}
/**
 * @brief Get the array of rabbits
 * @param self the landscape to get the rabbits for
 * @param args
 * @return the array of rabbits
 */
static PyObject* getRabbitsArray(PyLandscape* self, PyObject* args)
{
    // Set up the simulation, catch and return any errors.
    try
    {
        return self->landscape->getRabbitNumbers();
    }
    catch(exception &e)
    {
        return nullptr;
    }
    Py_RETURN_NONE;
}

/**
 * @brief Get the array of foxes
 * @param self the landscape to get the foxes for
 * @param args
 * @return the array of foxes
 */
static PyObject* getFoxesArray(PyLandscape* self, PyObject* args)
{
    // Set up the simulation, catch and return any errors.
    try
    {
        return self->landscape->getFoxNumbers();
    }
    catch(exception &e)
    {
        return nullptr;
    }
    Py_RETURN_NONE;
}

/**
 * @brief Iterates the simulation.
 * @param self the Python self object
 * @param args arguments to parse
 * @return Py_RETURN_TRUE if the simulation completes, Py_RETURN_FALSE otherwise.
 */
static PyObject* iterate(PyLandscape* self, PyObject* args)
{
    // Run the program, catch and return any errors.
    try
    {
        int input;
        // parse arguments
        if(!PyArg_ParseTuple(args, "i", &input))
        {
            return nullptr;
        }
        for(int i = 0; i < input; i++)
        {
            self->landscape->iterate();
        }
    }
    catch(exception &e)
    {
        return nullptr;
    }
    Py_RETURN_FALSE;
}

/**
 * @brief Generates the object methods for python.
 * @return the method definition
 */
static PyMethodDef* genPyLandscapeMethods()
{
    static PyMethodDef PyLandscapeMethods[] = {
            {"iterate", (PyCFunction) iterate, METH_VARARGS,
                    "Run the simulation"},
            {"get_rabbits", (PyCFunction) getRabbitsArray, METH_VARARGS,
                    "Get the array of rabbits"},
            {"get_foxes", (PyCFunction) getFoxesArray, METH_VARARGS,
                    "Get the array of foxes"},
            {"setup",   (PyCFunction) setup,   METH_VARARGS,
                    "Set up the simulation, importing the maps and assigning the variables."},
            {nullptr}  /* Sentinel */
    };
    return PyLandscapeMethods;
}

/**
 * @brief Generates the PyLandscape type
 * @param tp_name the type name
 * @param tp_doc the type documentation
 * @return the type object
 */
PyTypeObject genPyLandscapeType(char* tp_name, char* tp_doc)
{
    auto genPyTemplateNew = PyLandscape_new;
    auto genPyTemplateInit = PyTemplate_init;
    auto genPyTemplateDealloc = PyTemplate_dealloc;
    auto genPyTemplateTraverse = PyTemplate_traverse;
    auto genPyTemplateMethods = genPyLandscapeMethods();
    PyTypeObject ret_Simulation_Type = {
            PyVarObject_HEAD_INIT(nullptr, 0)
    };
    ret_Simulation_Type.tp_name = tp_name;
    ret_Simulation_Type.tp_doc = tp_doc;

    ret_Simulation_Type.tp_basicsize = sizeof(PyLandscape);
    ret_Simulation_Type.tp_itemsize = 0;
    ret_Simulation_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC;
    ret_Simulation_Type.tp_new = genPyTemplateNew;
    ret_Simulation_Type.tp_init = (initproc) genPyTemplateInit;
    ret_Simulation_Type.tp_dealloc = (destructor) genPyTemplateDealloc;
    ret_Simulation_Type.tp_traverse = (traverseproc) genPyTemplateTraverse;
    ret_Simulation_Type.tp_methods = genPyTemplateMethods;
    return ret_Simulation_Type;
}

static PyTypeObject C_Landscape_Type = genPyLandscapeType((char*) "librfsim.Landscape",
                                                          (char*) "C class for rabbit and fox simulations.");

#endif // PY_WRAPPER