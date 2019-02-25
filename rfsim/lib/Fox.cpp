/**
 * @brief Contains the Fox class and behaviours.
 */

#include "Fox.h"

void Fox::catchRabbit(vector<Rabbit> &rabbits, const shared_ptr<RNGController> &random)
{
    if(!rabbits.empty())
    {
        unsigned long index = random->i0(rabbits.size() - 1);
        energy += 25;
        rabbits[index].kill();
    }
}

bool Fox::canReproduce()
{
    if(energy > 500)
    {
        energy -= 400;
        return true;
    }
    return false;
}

void Fox::exist()
{
    energy -= 45;
    age += 1;
}

bool Fox::oldAge()
{
    return age > 10;
}
