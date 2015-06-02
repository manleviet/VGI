//
//
// image.h
//
// Un noeud d'image dans un graphe
// J'utilise un meme objet Image pour representer trois types de noeud :
// cluster, sous-cluster et image
//
// LE Viet Man
// 14/05/10
//
//

#ifndef IMAGE_H
#define IMAGE_H

#include <QString>
#include <QList>
#include <QPixmap>
#include "GraphImage/line.h"
#include "util.h"

class Line;

class Image
{
public:
    enum TYPE_IMAGE { CLUSTER, SUB_CLUSTER, IMAGE};

    Image(const TYPE_IMAGE type, const int id, const QString path);

    // Cac ham chung
    TYPE_IMAGE getType();

    void setId(const int id);
    int getId();

    void setPath(const QString path);
    QString getPath();

    QString getFilename();

    bool isPixmapLoaded();
    bool loadPixmap();
    QPixmap getPixmap();

    int getQuantityClusters();
    Image *getCluster(const int id);

    int getQuantitySubCluters();
    Image *getSubCluster(const int id);

    int getQuantityChildrens();
    Image *getChildren(const int id);

    int getQuantityNeighbors();
    Image *getNeighbor(const int id);

    int getQuantityLinesNeighbor();
    Line *getLineNeighbor(const int id);

    int getQuantityLinesChildren();
    Line *getLineChildren(const int id);

    void addCluster(Image *cluster);
    void addSubCluster(Image *subcluster);
    void addChildren(Image *children);
    void addNeighbor(Image *neighbor);
    void addLineNeighbor(Line *line);
    void addLineChildren(Line *line);

    int countSameChildren(Image *children);
    bool hasChildren(Image *children);

    int initLinks(int id);
private:
    // attruibuts generaux du cluster, sous-cluster et image
    TYPE_IMAGE type;
    int id;
    QString path;
    bool imageLoaded;
    QPixmap pixmap;

    QList<Image *> clusters;    //          // souscluster  // image
    QList<Image *> subclusters; // cluster                  // image
    QList<Image *> childrens;   // cluster  // souscluster
    QList<Image *> neighbors;   // cluster                  // image
    QList<Line *> line_neighbor;// cluster                  // image
    QList<Line *> line_children;// cluster
};

#endif // IMAGE_H
