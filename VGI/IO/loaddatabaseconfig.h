//
//
// loaddatabaseconfig.h
//
// Charger des donnees dans le fichier de configuration VGI
//
// LE Viet Man
// 14/05/10
//
//

#ifndef LOADDATABASECONFIG_H
#define LOADDATABASECONFIG_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "util.h"

class LoadDatabaseConfig
{
public:
    LoadDatabaseConfig(const QString path_vgi);

    Util::VGI_VERSION getVGIVersion();

    QString getPathConf();
    QString getPathMS();
    QString getPathSA();

    int getSubCluster();
    int getClusterIndex(int index);
    QString getClusterPathMS(int index);
    QString getClusterPathSA(int index);

    //
    // Charger des donnees dans le fichier de configuration
    //
    bool load();

private:
    Util::VGI_VERSION vgi_version; // version du fichier vgi

    QString path_vgi; // chemin du fichier de configuration VGI
    QString path_conf; // chemin du fichier de configuration de la base d'images
    QString path_MS; // chemin du repertoire MS
    QString path_SA; // chemin du repertoire SA

    int sub_cluster; // nombre des sous clusters
    QList<int> id_cluster; // indice des sous clusters
    QStringList ms_cluster; // chemin du repertoire MS des sous clusters
    QStringList sa_cluster; // chemin du repertoire SA des sous clusters
};

#endif // LOADDATABASECONFIG_H
