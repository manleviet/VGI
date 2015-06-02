//
//
// imagescene.h
//
// un noeud d'image dans un scene
//
// LE Viet Man
// 10/06/10
//
//

#ifndef IMAGESCENE_H
#define IMAGESCENE_H

#include <QGraphicsPixmapItem>
#include <QList>
#include <QString>
#include <QPainter>
#include "GraphImage/image.h"
#include "Scene/levelscene.h"
#include "Scene/linescene.h"

class LineScene;

class ImageScene : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT;
public:
    ImageScene(Image *image, QGraphicsItem *parent = 0);

    Image *getImage();
    bool isAdded();
    void setAdded(bool added);
    bool hasImage(Image *image);

    void addLine(LineScene *line);
    void setLineVisible(bool visible);

    QPolygonF polygon() const { return myPolygon; }

signals:
    void clickedImage(ImageScene *image);
    void doubleClickedImage(ImageScene *image);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event); // zoom image
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    int old_zValue;
    qreal old_height;
    qreal old_width;
    qreal deltaX;
    qreal deltaY;
    bool added;

    Image *image;

    QList<LineScene *> lines;

    QPolygonF myPolygon;
    bool hovered;

    void fillEllipse(QPainter *p, int x, int y, int size, const QBrush &brush);

    QString createImageCluster(Image *image);
    void drawImage(QPainter *painter, int x, int y, int angle, const QPixmap& pixmap);
    void drawImageShadow(QPainter *painter, int x, int y, int angle, const QBrush& color);
    void drawShadow(QPainter *painter, int x, int y, int angle, const QBrush& brush);
};

#endif // IMAGESCENE_H
