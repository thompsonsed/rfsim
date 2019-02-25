/**
 * @brief Contains the Animal class for basic inherited behaviours.
 */

#include "Animal.h"

const Coordinates &Animal::getLocation() const
{
    return location;
}

void Animal::setLocation(const Coordinates &location)
{
    Animal::location = location;
}

bool Animal::survives()
{
    return energy > 0.0 && !oldAge();
}

bool Animal::oldAge()
{
    return false;
}

void Animal::move(const std::shared_ptr<RNGController> &random)
{
    location.x += static_cast<long>(random->norm(sigma));
    location.y += static_cast<long>(random->norm(sigma));
    energy -= 10;
}

void Animal::checkBoundaries(const long &x_max, const long &y_max)
{
    location.x = min(x_max-1, location.x);
    location.y = min(y_max-1, location.y);
    location.x = max(( long) 0, location.x);
    location.y = max(( long) 0, location.y);
}

bool Animal::atLocation(const Coordinates &coordinates) const
{
    return coordinates.x == location.x && coordinates.y == location.y;
}
