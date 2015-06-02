//
//
// levelscene.h
//
// un niveau dans un scene
//
// LE Viet Man
// 10/06/10
//
//

#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "GraphImage/image.h"
#include "Scene/graphscene.h"
#include "Scene/imagescene.h"
#include "Scene/linescene.h"
#include "Scene/linelevelscene.h"
#include "math.h"

#include "iostream"

using namespace std;

class GraphScene;
class ImageScene;
class LineScene;

class LevelScene : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    LevelScene(int id, QGraphicsItem *parent = 0);

    int getId();
    bool isAdded();
    void setAdded(bool added);

    bool canChangeToClusters();
    bool canChangeToImages();

    ImageScene *getFocus();
    void addFocus(int id);
    void setAllVisible();
    void addImage(ImageScene *image);
    void addLine(LineScene *line);
    void addLineLevel(LineLevelScene *lineLevel);

    void showLevel(GraphScene *scene);

    void removeItems(GraphScene *scene);

signals:
    void clickedImage(int id, ImageScene *image);
    void doubleClickedImage(int id, ImageScene *image, ImageScene *focus);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private slots:
    void clickedImage(ImageScene *image);
    void doubleClickedImage(ImageScene *image);

private:
    int id;
    bool added;

    int idFocus;
    QList<ImageScene *> images;
    QList<LineScene *> lines;

    LineLevelScene *lineLevel;

    void doLayout();
    qreal radiiImage(qreal width, qreal height);
    void changeFocus(ImageScene *image);
};

#endif // LEVELSCENE_H
