//
//
// linescene.cpp
//
// Implementation de la classe linescene.h
//
// LE Viet Man
// 10/06/10
//
//

#include "Scene/linescene.h"

//
// Construction
//
LineScene::LineScene(ImageScene *start, ImageScene *end, Line *aline, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    this->startImage = start;
    this->endImage = end;
    this->aline = aline;
    this->added = false;
    this->focus = 0;
}

void LineScene::addFocus(ImageScene *focus)
{
    this->focus = focus;
}

bool LineScene::isAdded()
{
    return this->added;
}

void LineScene::setAdded(bool added)
{
    this->added = added;
}

QRectF LineScene::boundingRect() const
{
    qreal extra = (pen().width() + 10) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                           line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void LineScene::updatePosition()
{
    QLineF line(mapFromItem(startImage, 0, 0), mapFromItem(endImage, 0, 0));
    setLine(line);
}

void LineScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (startImage->collidesWithItem(endImage))
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);

    // [0]
    Qt::PenStyle style = Qt::SolidLine;
    QColor color = Qt::gray;

    switch(focus->getImage()->getType())
    {
    case Image::CLUSTER:
        if ((startImage->getImage()->getType() == Image::CLUSTER)
            && (endImage->getImage()->getType() == Image::CLUSTER))
        { // dessiner le lien du niveau 0
            if ((focus == startImage) || (focus == endImage))
            {
                color = Qt::blue;
            }
        }
        else
        {
            color = aline->getColor();
        }
        break;
    case Image::SUB_CLUSTER:
    case Image::IMAGE:
        color = aline->getColor();
    }

    painter->setPen(QPen(color, aline->getSize() % 10, style, Qt::RoundCap, Qt::RoundJoin));
    // [0]

    // [1] Calculer la nouvelle position des bouts du lien
    QPointF newStart = startImage->pos();
    QPointF newEnd = endImage->pos();

    QPixmap pixStart = startImage->pixmap();
    QPixmap pixEnd = endImage->pixmap();

    newStart.setX(newStart.x() + (pixStart.width() / 2));
    newStart.setY(newStart.y() + (pixStart.height() / 2));

    newEnd.setX(newEnd.x() + (pixEnd.width() / 2));
    newEnd.setY(newEnd.y() + (pixEnd.height() / 2));

    setLine(QLineF(newStart, newEnd));
    painter->drawLine(this->line());
}
