/**
 * @brief Contains the Fox class and behaviours.
 */

#include "Fox.h"

void Fox::catchRabbit(vector<Rabbit> &rabbits, shared_ptr<RNGController> random)
{
    if(!rabbits.empty())
    {
        unsigned long index = random->i0(rabbits.size() - 1);
        energy += rabbits[index].getEnergy()*0.5;
        rabbits[index].kill();
    }
}

bool Fox::canReproduce()
{
    if(energy > 50)
    {
        energy -= 50;
        return true;
    }
    return false;
}

void Fox::exist()
{
    energy -= 5;
    age += 1;
}

bool Fox::oldAge()
{
    return age > 30;
}
