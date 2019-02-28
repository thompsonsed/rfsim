#include <iostream>
#include "Landscape.h"

using namespace std;

int main()
{
    Landscape landscape = Landscape();
    landscape.setSeed(1);
    landscape.setLandscapeSize(5, 5);
    for(unsigned long i = 0; i < 100; i ++)
    {
        std::cout << i << std::endl;
        landscape.iterate();
//        if(i > 999)
//        {
            landscape.print();
//        }
    }
    return 0;
}