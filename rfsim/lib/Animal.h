/**
 * @brief Contains the Animal class for basic inherited behaviours.
 */


#ifndef LIB_RABBIT_H
#define LIB_RABBIT_H

#include <memory>
#include "Coordinates.h"
#include "RNGController.h"

/**
 * @brief Class containing the basic behaviours of an animal.
 */
class Animal
{
protected:
    double energy;
    double sigma;
    int age;
    Coordinates location;
public:

    Animal() : energy(0), sigma(0), age(0), location(0, 0)
    { }

    Animal(const double &energy, const double &sigma) : Animal(energy, sigma, Coordinates(0, 0))
    { }

    Animal(const double &energy, const double &sigma, const Coordinates &coordinates) : age(0), location(coordinates)
    {
        this->energy = energy;
        this->sigma = sigma;
    }

    /**
     * @brief Gets the location of this animal.
     * @return the x, y coordinates
     */
    const Coordinates &getLocation() const;

    /**
     * @brief Sets the animal's location.
     * @param location the x, y coordinates
     */
    void setLocation(const Coordinates &location);

    /**
     * @brief Checks if the animal has energy to live, and isn't too old.
     * @return true if the animal survives
     */
    bool survives();

    /**
     * @brief Virtual function to be overridden by child classes.
     * @return
     */
    virtual bool oldAge();

    /**
     * @brief Moves a random distance according to a normal distribution.
     * @param random the random number generator
     */
    void move(std::shared_ptr<RNGController> random);

    /**
     * @brief Checks the x, y location of the animal is within the boundaries of the landscape.
     * @param x_max the x location
     * @param y_max the y location
     */
    void checkBoundaries(const long &x_max, const long &y_max);

    /**
     * @brief Checks if the individual is at the given location.
     * @param coordinates the location to check
     * @return true if at coordinates
     */
    bool atLocation(const Coordinates &coordinates) const;
};

#endif //LIB_RABBIT_H
