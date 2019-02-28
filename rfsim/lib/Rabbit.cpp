/**
 * @brief Contains the Rabbit class and behaviours.
 */

#include "Rabbit.h"

Rabbit::Rabbit() : Animal(30, 2)
{}

Rabbit::Rabbit(const Coordinates &coordinates) : Animal(10, 2, coordinates)
{}

void Rabbit::eatGrass(const double &grass_amount)
{
    double eaten_amount = min(grass_amount, 30.0);
    energy += eaten_amount;
}

bool Rabbit::canReproduce()
{
    if(energy > 10 && !oldAge())
    {
        energy -= 5;
        return true;
    }
    return false;
}

void Rabbit::exist()
{
    energy -= 5;
    age += 1;
}

bool Rabbit::oldAge()
{
    return age > 10;
}

void Rabbit::kill()
{
    age = 100;
    energy = 0;
}
