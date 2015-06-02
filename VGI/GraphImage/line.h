//
//
// line.h
//
// Un lien dans un graphe
//
// LE Viet Man
// 14/05/10
//
//

#ifndef LINE_H
#define LINE_H

#include <QPen>
#include <QPainter>
#include "GraphImage/image.h"

class Image;

class Line
{
public:
    Line(Image *startItem,
         Image *endItem);

    int getId();
    void setId(const int id);

    void setSize(const int size);
    int getSize();

    void setColor(const QColor &color);
    QColor getColor();

    Image* start();
    Image* end();

private:
    int id;
    int size;
    QColor color;

    Image *startItem;
    Image *endItem;
};

#endif // LINE_H
