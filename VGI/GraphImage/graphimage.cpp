//
//
// graphimage.cpp
//
// Implementation de la classe graphimage.h
//
// LE Viet Man
// 14/05/10
//
//

#include "iostream"
#include "GraphImage/graphimage.h"

using namespace std;

//
// La construction
//
GraphImage::GraphImage(LoadDatabaseConfig *dbconf)
{
    this->dbconf = dbconf;
}

//
// Prendre la version du fichier vgi
//
Util::VGI_VERSION GraphImage::getVGIVersion()
{
    return dbconf->getVGIVersion();
}

//
// Charger des donnees dans la base d'images
// @return:
//      0 : ne pas avoir de erreur
//      1 : erreur dans la chargement des images
//      2 : erreur dans la chargement des clusters de MS
//      3 : erreur dans la chargement des fichier ***.data de MS
//      4 : erreur dans la chargement des clusters de SA
//      5 : erreur dans la chargement des fichier ***.data de SA
//
int GraphImage::load()
{
    // charger les images
    if (!loadImages(this->dbconf->getPathConf())) return 1;

    // SA
    // lire igroups
    if (!loadClusters(Util::DETECTOR_SA, this->dbconf->getPathSA())) return 4;

    // lire les fichiers data pour mettre les images au cluster
    if (!loadDataImage(Util::DETECTOR_SA, this->dbconf->getPathSA())) return 5;

    // creer des liens entre les clusters et les images
    initLinks();

    if (dbconf->getVGIVersion() == Util::VGI20)
    {
        // lire les sous clusters
        for (int i = 0; i < dbconf->getSubCluster(); i++)
        {
            if (!loadSubClusters(dbconf->getClusterIndex(i), dbconf->getClusterPathSA(i))) return 6;
        }
    }

    return 0;
}

//
// Prendre la quantite des clusters
//
int GraphImage::getQuantityCluster()
{
    return this->clusters.length();
}

//
// Prendre la quantite des sous-clusters
//
int GraphImage::getQuantitySubCluster()
{
    return this->subclusters.length();
}

//
// Prendre la quantite des images
//
int GraphImage::getQuantityImage()
{
    return this->images.length();
}

//
// Prendre la quantite des liens entre des clusters
//
int GraphImage::getQuantityLineClusters()
{
    return this->line_clusters.length();
}

//
// Prendre une image dans le graphe
// @param :
//      int index : l'indice d'image
// @result :
//      NULL : si l'indice est depasse les images dans le graphe
//      Image
//
Image *GraphImage::getImage(int index)
{
    return images.at(index);
}

//
// Prendre un cluster dans le graphe
// @param :
//      int index : l'indice de cluster
// @result :
//      NULL : si l'indice est depasse les clusters dans le graphe
//      Image
//
Image *GraphImage::getCluster(int index)
{
    return clusters.at(index);
}

//
// Prendre un cluster dans le graphe
// @param :
//      int index : l'indice de cluster
// @result :
//      NULL : si l'indice est depasse les clusters dans le graphe
//      Image
//
Image *GraphImage::getSubCluster(int index)
{
    return subclusters.at(index);
}

//
// Prendre l'indice d'un sous-cluster
//
int GraphImage::getIndexSubCluster(Image *subcluster)
{
    return this->subclusters.indexOf(subcluster);
}

//
// Prendre un objet Line
// @param :
//      int index : une indice
// @result : un objet Line
//
Line *GraphImage::getLineCluster(int index)
{
    return this->line_clusters.at(index);
}

//
// Charger des images dans la base d'images
// Initier les objets Image
// @param :
//      const QString &path_conf : le chemin du fichier de configuration
// @return :
//      false : s'il y a des erreur
//      true : si non
//
bool GraphImage::loadImages(const QString &path_conf)
{
    // ouvrir le fichier
    QFile file(path_conf);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);

    // lire la premiere ligne
    QString line = in.readLine();
    int quantity_images = line.toInt();

    int id = -1;
    while (!in.atEnd())
    {
        // lire une ligne
        QString path = in.readLine();

        id++;
        if (id >= quantity_images) return false; // le fichier a plus d'images que la quantite

        Image* img = new Image(Image::IMAGE, id, path);

        images << img; // mettre une image a la liste des images
    }

    return true; // done
}

//
// Lire le fichier igroups.data pour savoir combien de clusters
// Initier les objets Cluster
// @param :
//      const QString &path : le chemin du repertoire
// @return :
//      false : s'il y a des erreur
//      true : si non
//
bool GraphImage::loadClusters(int typeDetector, const QString &path)
{
    if (typeDetector != Util::DETECTOR_SA) return false;

    // creer le chemin au fichier igroups.data
    QString fullpath = Util::getPathiGroups(path);
    // ouvrir le fichier
    QFile file(fullpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);

    // lire la premiere ligne
    QString line = in.readLine();
    int quantity = line.toInt();

    // initier les objets Cluster
    for (int i = 0; i < quantity; i++)
    {
        Image *cluster;

        cluster = new Image(Image::CLUSTER, i, Util::getPathImageCluster());

        clusters << cluster; // mettre un cluster a la liste des clusters
    }

    return true; // done
}

//
// Lire les fichiers ***.data qui est correspondant avec les images
// Initier les objets Cluster
// @param :
//      const QString &path : le chemin du repertoire
// @return :
//      false : s'il y a des erreur
//      true : si non
//
bool GraphImage::loadDataImage(int typeDetector, const QString &path)
{
    if (typeDetector != Util::DETECTOR_SA) return false;
    // traverser les fichiers ***.data
    for (int i = 0; i < this->images.length(); i++)
    {
        Image *image = images.at(i);

        QString filenameImageData = Util::getNameOfFile(image->getFilename()) + ".data";
        QString fullpath = Util::getPathDataImage(path,
                                                  filenameImageData); // chi can lay phan ten, them .data

        // ouvrir le fichier
        QFile file(fullpath);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QTextStream in(&file);

        // lire les lignes inutiles dans le fichier ***.data
        {
            // lire la premiere et deuxieme ligne
            QString line = in.readLine(); // lire la premiere ligne
            line = in.readLine(); // lire la deuxieme ligne
            int num = line.toInt();

            // ignorer ces lignes
            for (int i = 0; i < num; i ++)
                line = in.readLine();
        }

        // lire les derniere ligne
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        int num = QString(fields.at(0)).toInt(); // nombre de clusters

        // Image fait partie de quel Cluster
        for (int j = 0; j < num; j++)
        {
            int id = QString(fields.at(j + 1)).toInt();
            Image *cluster = clusters.at(id);

            cluster->addChildren(image);
            image->addCluster(cluster);
        }
    }

    return true; // done
}

//
// Initier les liens entre les images, entre les clusters
//
void GraphImage::initLinks()
{
    int idLine = 0;
    // les images dans un meme cluster
    for (int i = 0; i < clusters.length(); i++)
    {
        // chaque cluster, traverser les images dans ce cluster
        idLine = clusters.at(i)->initLinks(idLine);
    }

    // entre les clusters
    for (int i = 0; i < clusters.length() - 1; i++)
    {
        for (int j = i + 1; j < clusters.length(); j++)
        {
            // compter les memes images dans ces deux clusters
            int count = clusters.at(i)->countSameChildren(clusters.at(j));

            if (count != 0)
            {
                // creer un lien
                Line *line = new Line(clusters.at(i), clusters.at(j));
                line->setSize(count);

                line->setId(idLine);
                idLine++;

                clusters.at(i)->addNeighbor(clusters.at(j));
                clusters.at(j)->addNeighbor(clusters.at(i));
                clusters.at(i)->addLineNeighbor(line);
                clusters.at(j)->addLineNeighbor(line);

                this->line_clusters << line;
            }
        }
    }
}

//
// Lire les fichiers dans le cluster
// Initier les objets du subcluster
// @param :
//      int index : l'indice du cluster
//      const QString &path : le chemin du repertoire
// @return :
//      false : s'il y a des erreur
//      true : si non
//
bool GraphImage::loadSubClusters(int index, const QString &path)
{
    // LIRE IGROUPS
    // creer le chemin au fichier igroups.data
    QString fullpath = Util::getPathiGroups(path);
    // ouvrir le fichier
    QFile file(fullpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);

    // lire la premiere ligne
    QString line = in.readLine();
    int quantity = line.toInt();

    if (quantity == 0) return true;

    Image *cluster = clusters.at(index);

    // initier les objets sous Cluster
    for (int i = 0; i < quantity; i++)
    {
        Image *subcluster = new Image(Image::SUB_CLUSTER, i, Util::getPathImageCluster());

        subclusters << subcluster; // mettre un cluster a la liste des clusters

        cluster->addSubCluster(subcluster);
        subcluster->addCluster(cluster);
    }

    // LIRE LES FICHIERS DATA
    // traverser les fichiers ***.data
    for (int i = 0; i < cluster->getQuantityChildrens(); i++)
    {
        Image *image = cluster->getChildren(i);

        QString filenameImageData = Util::getNameOfFile(image->getFilename()) + ".data";
        QString fullpath = Util::getPathDataImage(path,
                                                  filenameImageData); // chi can lay phan ten, them .data

        // ouvrir le fichier
        QFile file(fullpath);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QTextStream in(&file);

        // lire les lignes inutiles dans le fichier ***.data
        {
            // lire la premiere et deuxieme ligne
            QString line = in.readLine(); // lire la premiere ligne
            line = in.readLine(); // lire la deuxieme ligne
            int num = line.toInt();

            // ignorer ces lignes
            for (int i = 0; i < num; i ++)
                line = in.readLine();
        }

        // lire les derniere ligne
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        int num = QString(fields.at(0)).toInt(); // nombre de clusters

        // Image fait partie de quel subcluster
        for (int j = 0; j < num; j++)
        {
            int index = QString(fields.at(j + 1)).toInt();
            Image *subcluster = cluster->getSubCluster(index);

            subcluster->addChildren(image);
            image->addSubCluster(subcluster);
        }
    }

    return true; // done
}
