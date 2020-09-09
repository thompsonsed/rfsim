/**
 * @brief Contains the main Landscape class which controls the simulation.
 */

#include "Landscape.h"

void Landscape::setSeed(unsigned long i)
{
    random->setSeed(i);
}

void Landscape::iterate()
{
    vector<Rabbit> moved_rabbits;
    vector<Fox> moved_foxes;
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            Cell &cell = landscape.get(i, j);
            cell.growGrass(random);
            cell.iterate(random);
            auto tmp_rabbits = cell.moveRabbits(random, landscape.getCols(), landscape.getRows());
            auto tmp_foxes = cell.moveFoxes(random, landscape.getCols(), landscape.getRows());
            //                moved_rabbits.reserve(moved_rabbits.size() + distance(tmp_rabbits.begin(), tmp_rabbits.end()));
            //                moved_foxes.reserve(moved_foxes.size() + distance(tmp_foxes.begin(), tmp_foxes.end()));
            moved_rabbits.insert(moved_rabbits.end(), tmp_rabbits.begin(), tmp_rabbits.end());
            moved_foxes.insert(moved_foxes.end(), tmp_foxes.begin(), tmp_foxes.end());
        }
    }
    // Now move all the moved rabbits and foxes
    for(auto &rabbit: moved_rabbits)
    {
        if(rabbit.survives())
        {
            Coordinates new_location = rabbit.getLocation();
            landscape.get(new_location.y, new_location.x).addRabbit(rabbit);
        }
    }
    for(auto &fox: moved_foxes)
    {
        if(fox.survives())
        {
            Coordinates new_location = fox.getLocation();
            landscape.get(new_location.y, new_location.x).addFox(fox);
        }
    }
}

void Landscape::setLandscapeSize(unsigned long x_size, unsigned long y_size)
{
    landscape.setSize(y_size, x_size);
    for(unsigned long i = 0; i < y_size; i++)
    {
        for(unsigned long j = 0; j < x_size; j++)
        {
            Coordinates tmp_coordinate = Coordinates(j, i);
            landscape.get(i, j).setLocation(tmp_coordinate, random);
        }
    }
}

void Landscape::print()
{
    for(unsigned long i = 0; i < landscape.getRows(); i++)
    {
        for(unsigned long j = 0; j < landscape.getCols(); j++)
        {
            std::cout << "||" << landscape.get(j, i).getNumRabbits() << "|" << landscape.get(j, i).getNumFoxes();
        }
        std::cout << std::endl;
    }
}
