//
//
// linelevelscene.h
//
// un lien entre deux niveaux dans le scene
//
// LE Viet Man
// 17/06/10
//
//

#ifndef LINELEVELSCENE_H
#define LINELEVELSCENE_H

#include <QGraphicsLineItem>
#include "Scene/levelscene.h"

class LevelScene;

class LineLevelScene : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    LineLevelScene(LevelScene *start, LevelScene *end, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    bool isAdded();
    void setAdded(bool added);

public slots:
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    bool added;

    LevelScene *startLevel;
    LevelScene *endLevel;
};

#endif // LINELEVELSCENE_H
