/**
 * @brief Contains the main Landscape class which controls the simulation.
 */

#ifndef LIB_LANDSCAPE_H
#define LIB_LANDSCAPE_H
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include "Python.h"
#include "Cell.h"
#include "Matrix.h"

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
     * @brief Get the number of rows in the landscape
     * @return the number of rows
     */
    unsigned long getRows() const
    {
        return landscape.getRows();
    }

    /**
     * @brief Get the number of columns in the landscape
     * @return the number of columns
     */
    unsigned long getCols() const
    {
        return landscape.getCols();
    }

    /**
     * @brief Gets the cell at the specified location
     * @param i the row
     * @param j the column
     * @return
     */
    Cell get(const unsigned long &i, const unsigned long &j)
    {
        return landscape.get(i, j);
    }

};

#endif //LIB_LANDSCAPE_H
