#include "regionofword.h"

RegionOfWord::RegionOfWord(double x, double y, double a, double b, double angle)
{
    this->_x = x;
    this->_y = y;
    this->_a = a;
    this->_b = b;
    this->_angle = angle;
}

double RegionOfWord::x()
{
    return this->_x;
}

double RegionOfWord::y()
{
    return this->_y;
}

double RegionOfWord::a()
{
    return this->_a;
}

double RegionOfWord::b()
{
    return this->_b;
}

double RegionOfWord::angle()
{
    return this->_angle;
}
