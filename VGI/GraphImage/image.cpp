//
//
// image.cpp
//
// Implementation de la classe image.h
//
// LE Viet Man
// 14/05/10
//
//

#include "GraphImage/image.h"

//
// La consturction
//
Image::Image(TYPE_IMAGE type, int id, QString path)
{
    this->type = type;
    this->id = id;
    this->path = path;
    this->imageLoaded = false;
}

//
// Prendre le type du objet
// @result : trois types : CLUSTER, SOUSCLUSTER ou IMAGE
//
Image::TYPE_IMAGE Image::getType()
{
    return this->type;
}

//
// Mettre l'indice au objet
// @param :
//      int id : une nouvelle indice
//
void Image::setId(int id)
{
    this->id = id;
}

//
// Prendre l'indice du objet
//
int Image::getId()
{
    return this->id;
}

//
// Mettre le chemin de l'image au objet
// @param :
//      QString path : un chemin de l'image
//
void Image::setPath(QString path)
{
    this->path = path;
}

//
// Prendre le chemin de l'image
//
QString Image::getPath()
{
    return this->path;
}

//
// Prendre le nom du fichier de l'image
//
QString Image::getFilename()
{
    return Util::getFilename(this->path);
}

//
// Vouloir savoir si l'objet pixmap est chargee
// @result : true : si chargee, false : si non
//
bool Image::isPixmapLoaded()
{
    return this->imageLoaded;
}

//
// Charger une image
// @param :
//      const QString &filename : le chemin de l'image
//
bool Image::loadPixmap()
{
    if (this->imageLoaded) return true;

    // charger une image au pixmap
    if (!pixmap.load(this->path)) return false;

    this->imageLoaded = true;
    return true;
}

//
// Prendre l'objet pixmap qui stocke l'image chargee
//
QPixmap Image::getPixmap()
{
    return this->pixmap;
}

//
// Prendre la quantite des clusters
//
int Image::getQuantityClusters()
{
    return this->clusters.length();
}

//
// Prendre un cluster
// @param :
//      int id : l'indice du cluster
// @result : un cluster
//
Image *Image::getCluster(int id)
{
    return this->clusters.at(id);
}

//
// Prendre la quantite des sous-clusters
//
int Image::getQuantitySubCluters()
{
    return this->subclusters.length();
}

//
// Prendre un sous-cluster
// @param :
//      int id : l'indice du sous-cluster
// @result : un sous-cluster
//
Image *Image::getSubCluster(int id)
{
    return this->subclusters.at(id);
}

//
// Prendre la quantite des childrens
// childrens : vgi1.0 - childrens : images
//             vgi2.0 - childrens : clusters -> sous-clusters
//                                  sous-clusters -> images
//
int Image::getQuantityChildrens()
{
    return this->childrens.length();
}

//
// Prendre un children
// @param :
//      int id : l'indice de children
// @result : un children
//
Image *Image::getChildren(int id)
{
    return this->childrens.at(id);
}

//
// Prendre la quantite des neighbors
// Neighbors : si le type de l'objet est IMAGE : des images
//                                       CLUSTER : des clusters
//
int Image::getQuantityNeighbors()
{
    return this->neighbors.length();
}

//
// Prendre un neighbor
// @param :
//      int id : l'indice de neighbor
// @result : un neighbor
//
Image *Image::getNeighbor(int id)
{
    return this->neighbors.at(id);
}

//
// Prendre la quantite les liens entre des neighbor
//
int Image::getQuantityLinesNeighbor()
{
    return this->line_neighbor.length();
}

//
// Prendre un lien avec l'autre neighbor
// @param :
//      int id : l'indice de lien
// @result : un lien
//
Line *Image::getLineNeighbor(int id)
{
    return this->line_neighbor.at(id);
}

//
// Prendre la quantite des liens des childrens
//
int Image::getQuantityLinesChildren()
{
    return this->line_children.length();
}

//
// Prendre un lien avec children
// @param :
//      int id : l'indice d'un lien
// @result : un lien
//
Line *Image::getLineChildren(int id)
{
    return this->line_children.at(id);
}

//
// Ajouter un cluster
//
void Image::addCluster(Image *cluster)
{
    this->clusters << cluster;
}

//
// Ajouter un sous-cluster
//
void Image::addSubCluster(Image *subcluster)
{
    this->subclusters << subcluster;
}

//
// Ajouter un children
//
void Image::addChildren(Image *children)
{
    this->childrens << children;
}

//
// Ajouter un neighbor
//
void Image::addNeighbor(Image *neighbor)
{
    this->neighbors << neighbor;
}

//
// Ajouter un lien avec neighbor
//
void Image::addLineNeighbor(Line *line)
{
    this->line_neighbor << line;
}

//
// Ajouter un lien avec children
//
void Image::addLineChildren(Line *line)
{
    this->line_children << line;
}

//
// Compter les memes images dans ces deux clusters
// @param :
//      const Cluster &cluster : comparer avec ce cluster
// @return : nombre de memes images
int Image::countSameChildren(Image *children)
{
    int count = 0;
    for (int i = 0; i < childrens.length(); i++)
    {
        if (children->hasChildren(childrens.at(i))) count++;
    }
    return count;
}

bool Image::hasChildren(Image *children)
{
    return childrens.contains(children);
}

//
// Initier les liens entres les images dans un meme cluster
//
int Image::initLinks(int id)
{
    // traverser n - 1 premieres images
    for (int i = 0; i < childrens.length() - 1; i++)
    {
        // chaque image, lier avec les autres images (de (i + 1)er images)
        for (int j = i + 1; j < childrens.length(); j++)
        {
            // creer un lien entre deux images
            Line *line = new Line(childrens.at(i), childrens.at(j));
            line->setId(id);
            id++; // l'indice

            // la couleur de la lien
            QColor color = Util::getColor(this->id + 1); // id + 1 : eviter le noir
            line->setColor(color);

            this->line_children << line;

            // ajouter les images voisine et les liens
            childrens.at(i)->addNeighbor(childrens.at(j));
            childrens.at(j)->addNeighbor(childrens.at(i));
            childrens.at(i)->addLineNeighbor(line);
            childrens.at(j)->addLineNeighbor(line);
        }
    }
    return id;
}
