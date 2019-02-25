/**
 * @brief Contains the Rabbit class and behaviours.
 */

#ifndef LIB_ANIMAL_H
#define LIB_ANIMAL_H

#include "Animal.h"

/**
 * @brief Contains the behaviours of a rabbit
 */
class Rabbit : virtual public Animal
{
public:

    Rabbit();

    explicit Rabbit(const Coordinates &coordinates);

    /**
     * @brief Gain energy by eating grass
     * @param grass_amount the total amount of grass available
     */
    void eatGrass(const double &grass_amount);

    /**
     * @brief Check if the rabbit can reproduce
     * @return true if the rabbit reproduces
     */
    bool canReproduce();

    /**
     * @brief The painful process of existence costs energy.
     */
    void exist();

    /**
     * @brief Checks if the rabbit is getting old.
     * @return true if the rabbit dies
     */
    bool oldAge() override;

    /**
     * @brief Kills off the rabbit.
     */
    void kill();

};

#endif //LIB_ANIMAL_H
