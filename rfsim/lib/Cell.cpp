/**
 * @brief Contains a single well-mixed cell of grass, rabbits and foxes.
 */

#include "Cell.h"

Cell::Cell() : Cell(100.0, 10, 1)
{ }

Cell::Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes) :
        Cell(initial_grass, no_rabbits, no_foxes, Coordinates(0, 0))
{

}

Cell::Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes,
           const Coordinates &coordinates) : grass_amount(initial_grass), rabbits(no_rabbits), foxes(no_foxes),
                                             location(coordinates)
{
    setup();
}

void Cell::setup()
{
    for(auto &rabbit: rabbits)
    {
        rabbit.setLocation(location);
    }
    for(auto &fox: foxes)
    {
        fox.setLocation(location);
    }
}

void Cell::growGrass()
{
    grass_amount += 10.0;
}

void Cell::iterate(const shared_ptr<RNGController> &random)
{
    for(auto &rabbit: rabbits)
    {
        if(grass_amount > 1.0)
        {
            rabbit.eatGrass(grass_amount);
            grass_amount -= 10;
        }
        rabbit.exist();
    }
    if(!rabbits.empty())
    {
        for(auto &fox: foxes)
        {
            fox.catchRabbit(rabbits, random);
            fox.exist();
        }

    }
    reproduce(random);
    rabbits.erase(std::remove_if(rabbits.begin(), rabbits.end(),
                                 [this](Rabbit x) { return !x.survives(); }),
                  rabbits.end());
    foxes.erase(std::remove_if(foxes.begin(), foxes.end(),
                               [this](Fox x) { return !x.survives(); }),
                foxes.end());

}
