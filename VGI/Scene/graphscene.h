//
//
// graphscene.h
//
// C'est un scene pour afficher le graphe
//
// LE Viet Man
// 10/06/10
//
//

#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "Scene/linescene.h"
#include "GraphImage/graphimage.h"
#include "Scene/levelscene.h"
#include "Scene/linelevelscene.h"

class LevelScene;
class ImageScene;
class LineLevelScene;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphScene();

    void addGraph(GraphImage *graph);
    void setShowCluster(bool show);

private slots:
    void doubleClickedImage(int id, ImageScene *image, ImageScene *focus);

private:
    GraphImage *graph;
    QList<LevelScene *> levels;
    QList<LineLevelScene *> lines;

    bool showCluster;

    void showLevel();
    void layoutLevels();
    void computePosLevel(LevelScene *level, Util::TYPE_LEVEL type, qreal &width, qreal &height, qreal &x, qreal &y);

    void createLevel(Image *focus, LevelScene *level);
    void createLevel0(Image *focus, LevelScene *level);
    void createLevelSubCluster(Image *focus, LevelScene *level);
    void createLevelImage(Image *focus, LevelScene *level);
    void createLevelBetweenImage(Image *focus, LevelScene *level);
    void createLevelCluster(Image *focus, LevelScene *level);

    ImageScene *searchImage(Image *image, QList<ImageScene*> imagesScene);
};

#endif // GRAPHSCENE_H
