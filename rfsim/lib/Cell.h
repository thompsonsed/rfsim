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
    void iterate(const shared_ptr<RNGController> &random);

    /**
     * @brief Allow animals to reproduce
     * @param random the random number generator
     */
    void reproduce(const shared_ptr<RNGController> &random)
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

    /**
     * @brief Move rabbits according to a dispersal kernel.
     * @param random the random number generator
     * @param x_max the max x size of the landscape
     * @param y_max the max y size of the landscape
     * @return vector containing the rabbits that have moved.
     */
    vector<Rabbit> moveRabbits(const shared_ptr<RNGController> &random, unsigned long x_max, unsigned long y_max)
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
                                     [this](Rabbit x) { return x.atLocation(this->location); }),
                      rabbits.end());
        return moved_rabbits;
    }

    /**
     * @brief Move foxes according to a dispersal kernel.
     * @param random the random number generator
     * @param x_max the max x size of the landscape
     * @param y_max the max y size of the landscape
     * @return vector containing the foxes that have moved.
     */
    vector<Fox> moveFoxes(const shared_ptr<RNGController> &random, unsigned long x_max, unsigned long y_max)
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
                                   [this](Fox x) { return x.atLocation(this->location); }),
                    foxes.end());
        return moved_foxes;
    }

    /**
     * @brief Checks the rabbit carrying capacity, and kills them off as necessary.
     * @param k the carrying capacity
     */
    void checkRabbitCarryingCapacity(unsigned long k)
    {
        if(rabbits.size() > k)
        {
            rabbits.erase(rabbits.begin(), rabbits.begin() + k);
        }
    }

    /**
     * @brief Adds a rabbit to the cell
     * @param rabbit the rabbit to add
     */
    void addRabbit(Rabbit &rabbit)
    {
        rabbits.push_back(rabbit);
    }

    /**
     * @brief Adds a fox to the cell
     * @param fox the fox to add
     */
    void addFox(Fox &fox)
    {
        foxes.push_back(fox);
    }

    /**
     * @brief Set the location of the cell
     * @param coordinates the location in coordinates
     */
    void setLocation(const Coordinates &coordinates)
    {
        location = coordinates;
        setup();
    }

    /**
     * @brief Get the number of foxes in the cell
     * @return the number of foxes
     */
    unsigned long getNumFoxes()
    {
        return foxes.size();
    }

    /**
     * @brief Get the number of rabbits in the cell
     * @return the number of rabbits
     */
    unsigned long getNumRabbits()
    {
        return rabbits.size();
    }

};

#endif //LIB_CELL_H
