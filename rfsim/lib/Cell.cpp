/**
 * @brief Contains a single well-mixed cell of grass, rabbits and foxes.
 */

#include "Cell.h"

Cell::Cell() : Cell(100.0, 10, 1){}

Cell::Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes) :
        Cell(initial_grass, no_rabbits, no_foxes, Coordinates(0, 0))
{

}

Cell::Cell(const double &initial_grass, const unsigned long &no_rabbits, const unsigned long &no_foxes,
           const Coordinates &coordinates)
        : grass_amount(initial_grass), rabbits(no_rabbits), foxes(no_foxes),
          location(coordinates)
{
}

void Cell::setup(shared_ptr<RNGController> random)
{
    // Set the ages and randomise their initial ages
    for(auto &rabbit: rabbits)
    {
        rabbit.setLocation(location);
        rabbit.setAge(static_cast<int>(random->i0(3)));
    }
    for(auto &fox: foxes)
    {
        fox.setLocation(location);
        fox.setAge(static_cast<int>(random->i0(9)));
    }

}

void Cell::growGrass(shared_ptr<RNGController> random)
{
    grass_amount += random->i0(500) + 1000;
}

void Cell::iterate(shared_ptr<RNGController> random)
{
    for(auto &rabbit: rabbits)
    {
        if(grass_amount > 1.0)
        {
            rabbit.eatGrass(grass_amount);
            grass_amount -= 30;
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
    reproduce();
    rabbits.erase(std::remove_if(rabbits.begin(), rabbits.end(),
                                 [](Rabbit x){return !x.survives();}),
                  rabbits.end());
    foxes.erase(std::remove_if(foxes.begin(), foxes.end(),
                               [](Fox x){return !x.survives();}),
                foxes.end());
    if(foxes.size() > 10)
    {
        int number = static_cast<int>(foxes.size() - 10);
        foxes.erase(foxes.begin(), foxes.begin() + number);
    }

}

void Cell::reproduce()
{
    unsigned long total = 0;
    for(auto &rabbit: rabbits)
    {
        while(rabbit.canReproduce())
        {
            total ++;
        }
    }
    for(unsigned long i = 0; i < total; i++)
    {
        rabbits.emplace_back(Rabbit(location));
    }
    total = 0;
    for(auto &fox: foxes)
    {
        while(fox.canReproduce())
        {
            total ++;
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
                                 [this](Rabbit x){return !x.atLocation(this->location);}),
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
                               [this](Fox x){return !x.atLocation(this->location);}),
                foxes.end());
    return moved_foxes;
}

void Cell::addRabbit(Rabbit &rabbit)
{
    rabbits.push_back(rabbit);
}

void Cell::addFox(Fox &fox)
{
    foxes.push_back(fox);
}

void Cell::setLocation(const Coordinates &coordinates, shared_ptr<RNGController> random)
{
    location = coordinates;
    setup(random);
}

unsigned long Cell::getNumFoxes()
{
    return foxes.size();
}

unsigned long Cell::getNumRabbits()
{
    return rabbits.size();
}
