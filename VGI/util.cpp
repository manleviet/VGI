//
//
// util.cpp
//
// Implementation de la classe util.h
//
// LE Viet Man
// 14/05/10
//
//

#include "util.h"

Util::Util()
{
}

//
// La fonction sert à prendre le nom de fichier sorti de le fichier entré
// @param :
//	const QString &path : un chemin
// @return : le nom du fichier
//
QString Util::getFilename(const QString &path) // ex : /home/manleviet/12003.jpg
{
    QFileInfo fileinfo(path);
    return fileinfo.fileName(); // 12003.jpg
}

//
// Prendre le nom du fichier sans l'extension
// @param :
//      const QString &filename : le nom du fichier avec l'extension
// @result : le nom sans l'extension
//
QString Util::getNameOfFile(const QString &filename) // ex : 12003.jpg
{
    QStringList fields = filename.split(".");
    return (QString)fields.at(0); // 12003
}

//
// Changer l'extension du nom d'un fichier
// @param :
//      const QString &path : un chemin avec le nom et l'extension d'un fichier
//      const QString &newExt : une nouvelle extension
// @result : un chemin avec l'extension changee
//
QString Util::changeExtOfFile(const QString &path, const QString &newExt)
{
    QFileInfo fileinfo(path);
    return fileinfo.path() + QDir::separator() + getNameOfFile(fileinfo.fileName()) + newExt;
}

//
// La fonction sert a prendre le chemin de fichier igroups.data
// @param :
//      const QString &path : le chemin
// @return : le chemin au fichier igroups.data
//
QString Util::getPathiGroups(const QString &path)
{
    return path + QDir::separator() + igroups;
}

//
// La fonction sert a prendre le chemin de fichier ***.data
// @param :
//      const QString &path : le chemin
// @return : le chemin au fichier ***.data
//
QString Util::getPathDataImage(const QString &path, const QString &filename)
{
    return path + QDir::separator() + filename;
}

//
// Prendre un objet Color
// @param :
//      const int id : indice
// @return : un Color
//
QColor Util::getColor(int id)
{
    return color[id];
}

//
// Prendre un objet Pixmap qui contient l'image images.png
// @result : un objet Pixmap
//
QPixmap Util::getImageCluster()
{
    QPixmap pixmap;
    QString path = getPathImageCluster();
    if (!pixmap.load(QDir::currentPath() + QDir::separator() + IMAGE_CLUSTER))
        return NULL;
    return pixmap;
}

//
// Prendre le chemin de l'image Clusters (images.png dans le repertoire images)
// Maintenant : le modele n'utilise pas cette fonction
// @resultat : le chemin de l'image images.png
//
QString Util::getPathImageCluster()
{
    QString path = QDir::currentPath() + QDir::separator() + IMAGE_CLUSTER;
    return path;
}

//
// Prendre le chemin du repertoire "images"
// @result : le chemin du repertoire "images"
QString Util::getPathDirImages()
{
    QString path = QDir::currentPath() + QDir::separator() + PATH_DIR_IMAGES;
    return path;
}

//
// Prendre le chemin de l'image transparent.png
//
QString Util::getPathImageTransparent()
{
    QString path = QDir::currentPath() + QDir::separator() + PATH_IMAGE_TRANSPARENT;
    return path;
}

//
// Valider si le fichier avec l'extension est .vgi
// @param :
//      const QString path : le chemin avec le nom du fichier
// @result :
//      true : avec l'extension .vgi
//      false : avec l'extension .vml
//
bool Util::isVGI(QString path)
{
    QStringList fields = path.split(".");
    if ((QString)fields.at(fields.length() - 1) == "vgi")
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
// Valider si le fichier avec l'extension est .vml
// @param :
//      const QString path : le chemin avec le nom du fichier
// @result :
//      true : avec l'extension .vml
//      false : avec l'extension .vgi
//
bool Util::isVXL(QString path)
{
    return !isVGI(path);
}

//
// Changer le type de niveau
// @param : un type de niveau
// @result : un autre type de niveau
//
Util::TYPE_LEVEL Util::changeType(Util::TYPE_LEVEL type)
{
    Util::TYPE_LEVEL typetemp;
    switch(type)
    {
    case Util::CENTRE_LEVEL:
        typetemp = LEFT_LEVEL;
        break;
    case Util::LEFT_LEVEL:
        typetemp = Util::BOTTOM_LEVEL;
        break;
    case Util::BOTTOM_LEVEL:
        typetemp = Util::RIGHT_LEVEL;
        break;
    case Util::RIGHT_LEVEL:
        typetemp = Util::TOP_LEVEL;
        break;
    case Util::TOP_LEVEL:
        typetemp = Util::LEFT_LEVEL;
        break;
    }
    return typetemp;
}
