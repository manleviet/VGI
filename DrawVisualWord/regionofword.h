#ifndef REGIONOFWORD_H
#define REGIONOFWORD_H

class RegionOfWord
{
public:
    RegionOfWord(double x, double y, double a, double b, double angle);

    double x();
    double y();
    double a();
    double b();
    double angle();


private:
    double _x;
    double _y;
    double _a;
    double _b;
    double _angle;
};

#endif // REGIONOFWORD_H
