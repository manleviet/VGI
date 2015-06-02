//
//
// main.cpp
//
// Ce programme traverse les images et les fichiers .data
// pour dessiner les mots visuels
//
// LE Viet Man
// 23/06/10
//
//

#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QImage>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include "regionofword.h"
//#include <sys/dir.h>
//#include <string.h>
//#include <fstream>

using namespace std;

char *Usage = (char*)"Usage: drawvw [option] <chemin de la base d'images>\n\
Traverser les images et les fichiers .data pour dessiner les mots visuels.\n\
Ce programme utilise la librairie Qt4\n\
Arguments:\n\
    -h : aide\n\
    -o <path> : le chemin du fichier sortie\n\
    -d <path> : le chemin du repertoire des fichiers .data\n\
Auteur : LE Viet Man\n\
Date   : 23/06/10\n";

//
// Prototypes des fonctions
//
void showUsage(bool);
void readBagOfWord(QString filename, QList<RegionOfWord> &regions);
void drawBagOfWord(QImage &image, QList<RegionOfWord> regions);

int main(int argc, char *argv[])
{
    if (argc < 2) // manque les paramètres
    {
        showUsage(true); // afficher les informations d'aide
    }

    //char* path_db = 0; // chemin de la base d'images
    //char* path_data = 0; // chemin du repertoire des fichiers .data
    //char* path_fileout = 0; // chemin du fichier sortie

    QString path_db; // chemin de la base d'images
    QString path_data; // chemin du repertoire des fichiers .data
    QString path_fileout; // chemin du fichier sortie

    // cette boucle lit les arguments passés
    int c;
    while ((c = getopt(argc, argv, "ho:d:")) != -1)
    {
        switch (c)
        {
            case 'h':
            {
                showUsage(false);
                break;
            }
            case 'o':
            {
                extern char *optarg;
                path_fileout = optarg;
                break;
            }
            case 'd':
            {
                extern char *optarg;
                path_data = optarg;
            }
            default:
                break;
        }
    }

    // lire le nom de fichier a la fin de la ligne de commande
    extern int optind;
    if (optind>=argc)
    {
        showUsage(true);
    }
    path_db = argv[optind];

    // duyet qua thu muc anh
    QDir dir_db(path_db);
    QDir dir_data(path_data);
    //QDir dir_fileout(path_fileout);

    if (!dir_db.exists())
    {
        QString warning = QString("Cannot find the directory %1").arg(path_db);
        cout << warning.toStdString() << endl;
        return 1;
    }
    if (!dir_data.exists())
    {
        QString warning = QString("Cannot find the directory %1").arg(path_data);
        cout << warning.toStdString() << endl;
        return 1;
    }
    /*if (!dir_fileout.exists())
    {
        QString warning = QString("Cannot find the directory %1").arg(path_fileout);
        cout << warning.toStdString() << endl;
        return 1;
    }*/

    dir_db.setFilter(QDir::Files);
    dir_db.setSorting(QDir::Name);

    QFileInfoList list = dir_db.entryInfoList();
    for (int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        QString filename = fileInfo.filePath();

        cout << "Draw file " << filename.toStdString() << " .";

        // load anh
        QImage image;
        image.load(filename);

        if (image.isNull()) {
            cout << "Cannot load " << filename.toStdString() << endl;
            return 1;
        }
        cout << ".";

        // lay ten file, tao ten file data
        QString filename_data = fileInfo.fileName();
        QStringList fields = filename_data.split(".");
        QString nameOfFile = (QString)fields.at(0); // 12003
        filename_data = nameOfFile + ".data";
        QString path_data = dir_data.path() + QDir::separator() + filename_data;
        // load file data
        QList<RegionOfWord> regions;
        readBagOfWord(path_data, regions);
        cout << ".";

        // ve mots visuels
        drawBagOfWord(image, regions);
        cout << ".";

        // luu file image
        QString filename_out = path_fileout + QDir::separator() + nameOfFile + ".png";
        image.save(filename_out);

        cout << " done" << endl;
    }
    return 0;
}

//
// La fonction sert à afficher les informations d'aide de ce programme
// @param :
//	bool erreur : true, cad il y a une erreur
//                          la fonction exécutera la commande exit(1)
//                    false, cad il n'y a pas d'erreur
//                           la fonction exécutera la commande exit(0)
//
void showUsage(bool erreur)
{
    cerr << Usage << endl;
    if (erreur)
    {
        exit(1);
    }
    else
    {
        exit(0);
    }
}

void readBagOfWord(QString filename, QList<RegionOfWord> &regions)
{
    // ouvrir le fichier
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream in(&file);

    // lire la premiere ligne
    QString line = in.readLine();
    line = in.readLine(); // lire la deuxieme ligne
    int num = line.toInt();

    // lire les bags of word
    for (int i = 0; i < num; i++)
    {
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        double x = QString(fields.at(3)).toDouble();
        double y = QString(fields.at(4)).toDouble();
        double a = QString(fields.at(5)).toDouble();
        double b = QString(fields.at(6)).toDouble();
        double angle = QString(fields.at(7)).toDouble();

        RegionOfWord region(x, y, a, b, angle);

        regions << region;
    }
}

void drawBagOfWord(QImage &image, QList<RegionOfWord> regions)
{
    // paint the ellipse
    QPainter p(&image);

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::yellow);

    for(int i = 0; i < regions.length(); i++)
    {
        RegionOfWord region = regions.at(i);

        p.save();
        p.rotate(region.angle());
        p.drawEllipse(region.x(), region.y(), 5, 3);//region.a(), region.b());
        p.restore();
    }
    p.end();
}
