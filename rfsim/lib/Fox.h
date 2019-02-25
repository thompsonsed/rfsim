/**
 * @brief Contains the Fox class and behaviours.
 */

#ifndef LIB_FOX_H
#define LIB_FOX_H

#include "Animal.h"
#include "Rabbit.h"

/**
 * Contains the behaviours of a Fox
 */
class Fox : virtual public Animal
{
protected:
public:

    Fox() : Animal(30, 2)
    {}

    Fox(const Coordinates &coordinates) : Animal(100, 4, coordinates)
    { }

    /**
     * @brief Catches a rabbit from the list of rabbits
     * @param rabbits the rabbits list to pick from
     * @param random the random number generator
     */
    void catchRabbit(vector<Rabbit> &rabbits, shared_ptr<RNGController> random);

    /**
     * @brief Checks if this fox can reproduce
     * @return true if the fox has enough energy to reproduce
     */
    bool canReproduce();

    /**
     * @brief The painful process of existence costs energy.
     */
    void exist();

    /**
     * @brief Check if the fox is getting old
     * @return true if the fox dies of old age
     */
    bool oldAge() override;
};

#endif //LIB_FOX_H
