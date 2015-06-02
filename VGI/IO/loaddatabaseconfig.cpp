//
//
// loaddatabaseconfig.cpp
//
// Implementation de la clase loaddatabaseconfig.cpp
//
// LE Viet Man
// 14/05/10
//
//

#include "IO/loaddatabaseconfig.h"

//
// Construction
//
LoadDatabaseConfig::LoadDatabaseConfig(QString path_vgi)
{
    this->path_vgi = path_vgi;

    this->vgi_version = Util::VGI10;

    this->path_conf = "";
    this->path_MS = "";
    this->path_SA = "";

    this->sub_cluster = 0;
}

//
// Prendre la version du fichier vgi
//
Util::VGI_VERSION LoadDatabaseConfig::getVGIVersion()
{
    return this->vgi_version;
}

//
// Prendre le chemin du fichier de configuration de la base d'images
//
QString LoadDatabaseConfig::getPathConf()
{
    return this->path_conf;
}

//
// Prendre le chemin du repertoire MS
//
QString LoadDatabaseConfig::getPathMS()
{
    return this->path_MS;
}

//
// Prendre le chemin du repertoire SA
//
QString LoadDatabaseConfig::getPathSA()
{
    return this->path_SA;
}

//
// Prendre la quantite de sous-clusters
//
int LoadDatabaseConfig::getSubCluster()
{
    return this->sub_cluster;
}

//
// Prendre l'indice de sous-cluster avec index
//
int LoadDatabaseConfig::getClusterIndex(int index)
{
    return this->id_cluster.at(index);
}

//
// Prendre le chemin du repertoire MS de sous-cluster avec index
//
QString LoadDatabaseConfig::getClusterPathMS(int index)
{
    return this->ms_cluster.at(index);
}

//
// Prendre le chemin du repertoire SA de sous-cluster avec index
//
QString LoadDatabaseConfig::getClusterPathSA(int index)
{
    return this->sa_cluster.at(index);
}

//
// Charger des donnees dans le fichier de configuration
// @return :
//      false : il y a des erreur
//      true : si non
//
bool LoadDatabaseConfig::load()
{
    // ouvrir le fichier
    QFile file(this->path_vgi);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        // lire une ligne
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        if (fields.at(0) == "vgi1.0")
        {
            this->vgi_version = Util::VGI10;
        }
        if (fields.at(0) == "vgi2.0")
        {
            this->vgi_version = Util::VGI20;
        }
        if (fields.at(0) == "image")
        { // lire le chemin du fichier de configuration
            this->path_conf = fields.at(1);
        }
        if (fields.at(0) == "databaseMS")
        { // lire le chemin du repertoire MS
            this->path_MS = fields.at(1);
        }
        if (fields.at(0) == "databaseSA")
        { // lire le chemin du repertoire SA
            this->path_SA = fields.at(1);
        }
        if (fields.at(0) == "cluster")
        { // lire les informations du sous cluster
            this->sub_cluster++;
            QString id = fields.at(1);
            this->id_cluster << id.toInt();
            this->ms_cluster << fields.at(2);
            this->sa_cluster << fields.at(3);
        }
    }

    return true; // done
}
