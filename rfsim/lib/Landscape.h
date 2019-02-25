/**
 * @brief Contains the main Landscape class which controls the simulation.
 */

#ifndef LIB_LANDSCAPE_H
#define LIB_LANDSCAPE_H
//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include "Python.h"
#include "Cell.h"
#include "Matrix.h"
/**
 * @brief Support for numpy returning array
 * @param capsule the python object capsule
 */
void capsuleCleanup(PyObject *capsule);

/**
 * @brief Holds the landscape of foxes and rabbits and controls their behaviours.
 */
class Landscape
{
protected:
    Matrix<Cell> landscape;
    shared_ptr<RNGController> random;
public:

    Landscape() : landscape(), random(make_shared<RNGController>())
    {

    }

    /**
     * @brief Sets the random number seed for the simulation.
     * @param i the random number seed
     */
    void setSeed(unsigned long i);

    /**
     * @brief Perform one iteration of the landscape, including growing the grass, having animals feed and reproduce and
     * moving between cells.
     */
    void iterate();

    /**
     * @brief Set the landscape dimensions.
     * @param x_size the x dimension of the landscape
     * @param y_size the y dimension of the landscape
     */
    void setLandscapeSize(unsigned long x_size, unsigned long y_size);

    /**
     * @brief Print the landscape to the terminal.
     */
    void print();



    /**
     * @brief Get the number of rabbits in the landscape as a numpy array.
     * @return the number of rabbits in the numpy array
     */
    PyObject * getRabbitNumbers();

    /**
    * @brief Get the number of foxes in the landscape as a numpy array.
    * @return the number of foxes in the numpy array
    */
    PyObject * getFoxNumbers();






};

#endif //LIB_LANDSCAPE_H
