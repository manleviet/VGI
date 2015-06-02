//
//
// linelevelscene.cpp
//
// Implementation de la classe linelevelscene.h
//
// LE Viet Man
// 17/06/10
//
//

#include "linelevelscene.h"

LineLevelScene::LineLevelScene(LevelScene *start, LevelScene *end, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    this->startLevel = start;
    this->endLevel = end;
}

bool LineLevelScene::isAdded()
{
    return this->added;
}

void LineLevelScene::setAdded(bool added)
{
    this->added = added;
}

void LineLevelScene::updatePosition()
{
    QLineF line(mapFromItem(startLevel, 0, 0), mapFromItem(endLevel, 0, 0));
    setLine(line);
}

void LineLevelScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (startLevel->collidesWithItem(endLevel))
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    // [0]
    Qt::PenStyle style = Qt::SolidLine;
    QColor color = Qt::gray;

    painter->setPen(QPen(color, 4, style, Qt::RoundCap, Qt::RoundJoin));
    // [0]

    // [1] Calculer la nouvelle position des bouts du lien
    QPointF newStart, newEnd;

    QRectF rectStart = startLevel->rect();
    QRectF rectEnd = endLevel->rect();

    newStart.setX(rectStart.x() + (rectStart.width() / 2));
    newStart.setY(rectStart.y() + (rectStart.height() / 2));

    newEnd.setX(rectEnd.x() + (rectEnd.width() / 2));
    newEnd.setY(rectEnd.y() + (rectEnd.height() / 2));

    setLine(QLineF(newStart, newEnd));
    painter->drawLine(this->line());
}
