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

void Cell::iterate(shared_ptr<RNGController> random)
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

void Cell::reproduce(shared_ptr<RNGController> random)
{
    unsigned long total = 0;
    for(auto &rabbit: rabbits)
    {
        if(rabbit.canReproduce())
        {
            total += 10;
        }
    }
    for(unsigned long i = 0; i < total; i++)
    {
        rabbits.emplace_back(Rabbit(location));
    }
    total = 0;
    for(auto &fox: foxes)
    {
        if(fox.canReproduce())
        {
            total += 1;
        }
    }
    for(unsigned long i = 0; i < total; i++)
    {
        foxes.emplace_back(Fox(location));
    }
}

vector<Rabbit> Cell::moveRabbits(shared_ptr<RNGController> random, unsigned long x_max, unsigned long y_max)
{
    vector<Rabbit> moved_rabbits;
    for(auto &rabbit: rabbits)
    {
        rabbit.move(random);
        rabbit.checkBoundaries(x_max, y_max);
        if(!rabbit.atLocation(location))
        {
            moved_rabbits.push_back(rabbit);
        }
    }
    rabbits.erase(std::remove_if(rabbits.begin(), rabbits.end(),
                                 [this](Rabbit x) { return !x.atLocation(this->location); }),
                  rabbits.end());
    return moved_rabbits;
}

vector<Fox> Cell::moveFoxes(shared_ptr<RNGController> random, const unsigned long &x_max, const unsigned long &y_max)
{
    vector<Fox> moved_foxes;
    for(auto &fox: foxes)
    {
        fox.move(random);
        fox.checkBoundaries(x_max, y_max);
        if(!fox.atLocation(location))
        {
            moved_foxes.push_back(fox);
        }
    }
    foxes.erase(std::remove_if(foxes.begin(), foxes.end(),
                               [this](Fox x) { return !x.atLocation(this->location); }),
                foxes.end());
    return moved_foxes;
}

void Cell::checkRabbitCarryingCapacity(const unsigned long &k)
{
    if(rabbits.size() > k)
    {
        rabbits.erase(rabbits.begin(), rabbits.begin() + k);
    }
}

void Cell::checkFoxCarryingCapacity(const unsigned long &k)
{
    if(foxes.size() > k)
    {
        foxes.erase(foxes.begin(), foxes.begin() + k);
    }
}
void Cell::addRabbit(Rabbit &rabbit)
{
    rabbits.push_back(rabbit);
}

void Cell::addFox(Fox &fox)
{
    foxes.push_back(fox);
}

void Cell::setLocation(const Coordinates &coordinates)
{
    location = coordinates;
    setup();
}

unsigned long Cell::getNumFoxes()
{
    return foxes.size();
}

unsigned long Cell::getNumRabbits()
{
    return rabbits.size();
}
