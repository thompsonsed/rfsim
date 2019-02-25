/**
 * @brief Contains a simple Coordinates struct.
 */

#ifndef LIB_COORDINATES_H
#define LIB_COORDINATES_H

struct Coordinates
{
    long x = 0;
    long y = 0;

    Coordinates(unsigned long x_in, unsigned long y_in) : x(x_in), y(y_in)
    {

    }

    Coordinates(const Coordinates &coordinates)
    {
        this->x = coordinates.x;
        this->y = coordinates.y;
    }
};

#endif //LIB_COORDINATES_H
