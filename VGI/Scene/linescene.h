//
//
// linescene.h
//
// un lien dans le scene
//
// LE Viet Man
// 10/06/10
//
//

#ifndef LINESCENE_H
#define LINESCENE_H

#include <QGraphicsLineItem>
#include "Scene/imagescene.h"
#include "GraphImage/line.h"

class ImageScene;

class LineScene : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    LineScene(ImageScene *start, ImageScene *end, Line *aline, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void addFocus(ImageScene *focus);
    bool isAdded();
    void setAdded(bool added);

    QRectF boundingRect() const;

public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    bool added;
    Line *aline;

    ImageScene *focus;

    ImageScene *startImage;
    ImageScene *endImage;
};

#endif // LINESCENE_H
