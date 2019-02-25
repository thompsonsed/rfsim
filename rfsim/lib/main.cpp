#include <iostream>
#include "Landscape.h"

using namespace std;

int main()
{
    Landscape landscape = Landscape(100);
    landscape.setLandscapeSize(100, 100);
    for(unsigned long i = 0; i < 15; i ++)
    {
        std::cout << i << std::endl;
        landscape.iterate();
        landscape.print();

    }

    return 0;
}