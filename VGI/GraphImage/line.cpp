//
//
// image.cpp
//
// Implementation de la classe image.cpp
//
// LE Viet Man
// 14/05/10
//
//

#include "GraphImage/line.h"

//
// Construction
//
Line::Line(Image *startItem, Image *endItem)
{
    this->startItem = startItem;
    this->endItem = endItem;

    // la taille de ligne par defaut
    this->size = 1;
    // la couleur par defaut
    this->color = Util::getColor(0);
}

//
// Prendre l'indice du lien
//
int Line::getId()
{
    return this->id;
}

//
// Mettre l'indice au lien
//
void Line::setId(int id)
{
    this->id = id;
}

//
// Mettre la taille au lien
//
void Line::setSize(int size)
{
    this->size = size;
}

//
// Prendre la taille du lien
//
int Line::getSize()
{
    return this->size;
}

//
// Mettre la couleur au lien
//
void Line::setColor(const QColor &color)
{
    this->color = color;
}

//
// Prendre la couleur du lien
//
QColor Line::getColor()
{
    return this->color;
}

//
// Prendre l'image du debut du lien
//
Image* Line::start()
{
    return this->startItem;
}

//
// Prendre l'image du debut du lien
//
Image* Line::end()
{
    return this->endItem;
}
