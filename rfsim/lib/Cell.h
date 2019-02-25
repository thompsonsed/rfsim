/**
 * @brief Contains a single well-mixed cell of grass, rabbits and foxes.
 */

#ifndef LIB_CELL_H
#define LIB_CELL_H

#include <vector>
#include "Rabbit.h"
#include "Fox.h"
#include "Coordinates.h"

class Cell
{
protected:
    double grass_amount;
    std::vector<Rabbit> rabbits;
    std::vector<Fox> foxes;

    Coordinates location;

public:

    Cell();;

    Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes);

    Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes,
         const Coordinates &coordinates);

    void setup();

    void growGrass();

    /**
     * @brief Iterate over the consumption stages (rabbits eating grass and foxes eating rabbits).
     * @param random the random number generator
     */
    void iterate(shared_ptr<RNGController> random);

    /**
     * @brief Allow animals to reproduce
     * @param random the random number generator
     */
    void reproduce(shared_ptr<RNGController> random);

    /**
     * @brief Move rabbits according to a dispersal kernel.
     * @param random the random number generator
     * @param x_max the max x size of the landscape
     * @param y_max the max y size of the landscape
     * @return vector containing the rabbits that have moved.
     */
    vector<Rabbit> moveRabbits(shared_ptr<RNGController> random, unsigned long x_max, unsigned long y_max);

    /**
     * @brief Move foxes according to a dispersal kernel.
     * @param random the random number generator
     * @param x_max the max x size of the landscape
     * @param y_max the max y size of the landscape
     * @return vector containing the foxes that have moved.
     */
    vector<Fox> moveFoxes(shared_ptr<RNGController> random, const unsigned long &x_max, const unsigned long &y_max);

    /**
     * @brief Checks the rabbit carrying capacity, and kills them off as necessary.
     * @param k the carrying capacity
     */
    void checkRabbitCarryingCapacity(const unsigned long &k);

    void checkFoxCarryingCapacity(const unsigned long &k);
    /**
     * @brief Adds a rabbit to the cell
     * @param rabbit the rabbit to add
     */
    void addRabbit(Rabbit &rabbit);

    /**
     * @brief Adds a fox to the cell
     * @param fox the fox to add
     */
    void addFox(Fox &fox);

    /**
     * @brief Set the location of the cell
     * @param coordinates the location in coordinates
     */
    void setLocation(const Coordinates &coordinates);

    /**
     * @brief Get the number of foxes in the cell
     * @return the number of foxes
     */
    unsigned long getNumFoxes();

    /**
     * @brief Get the number of rabbits in the cell
     * @return the number of rabbits
     */
    unsigned long getNumRabbits();

};

#endif //LIB_CELL_H
