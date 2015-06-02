//
//
// graphimage.h
//
// Contenir tous les noeuds d'images et de clusters
// Lire des donnees dans la base d'images
//
// LE Viet Man
// 14/05/10
//
//

#ifndef GRAPHIMAGE_H
#define GRAPHIMAGE_H

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QProgressDialog>
#include "GraphImage/image.h"
#include "IO/loaddatabaseconfig.h"
#include "util.h"

class GraphImage
{
public:
    GraphImage(LoadDatabaseConfig *dbconf);

    int load();

    Util::VGI_VERSION getVGIVersion();

    int getQuantityCluster();
    Image *getCluster(const int index);

    int getQuantitySubCluster();
    Image *getSubCluster(const int index);
    int getIndexSubCluster(Image *subcluster);

    int getQuantityImage();
    Image *getImage(const int index);

    int getQuantityLineClusters();
    Line *getLineCluster(const int index);

private:
    LoadDatabaseConfig *dbconf;

    QList<Image *> clusters;
    QList<Image *> subclusters;
    QList<Image *> images;

    QList<Line *> line_clusters;

    bool loadImages(const QString &path_conf);
    bool loadClusters(const int typeDetector, const QString &path);
    bool loadDataImage(const int typeDetector, const QString &path);
    bool loadSubClusters(const int index, const QString &path);
    void initLinks();
};

#endif // GRAPHIMAGE_H
