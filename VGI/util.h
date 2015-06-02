//
//
// util.h
//
// Les methodes static
//
// LE Viet Man
// 14/05/10
//
//

#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QColor>
#include <QPixmap>

const QString igroups = "igroups.data";
const QColor color[36] = {Qt::black, Qt::cyan,
                          Qt::darkCyan, Qt::red,
                          Qt::darkRed, Qt::magenta,
                          Qt::darkMagenta, Qt::green,
                          Qt::darkGreen, Qt::yellow,
                          Qt::darkYellow, Qt::blue,
                          Qt::darkBlue, Qt::gray,
                          Qt::darkGray, Qt::lightGray,
                          Qt::black, Qt::cyan,
                          Qt::darkCyan, Qt::red,
                          Qt::darkRed, Qt::magenta,
                          Qt::darkMagenta, Qt::green,
                          Qt::darkGreen, Qt::yellow,
                          Qt::darkYellow, Qt::blue,
                          Qt::darkBlue, Qt::gray,
                          Qt::darkGray, Qt::lightGray}; // 36 colors
const QString IMAGE_CLUSTER = "images/images.png";
const QString PATH_DIR_IMAGES = "images";
const QString PATH_IMAGE_TRANSPARENT = "images/transparent.png";



class Util
{
public:
    static const int DETECTOR_MS = 0;
    static const int DETECTOR_SA = 1;

    enum VGI_VERSION { VGI10, VGI20 };

    enum TYPE_LEVEL { CENTRE_LEVEL,
                      LEFT_LEVEL,
                      BOTTOM_LEVEL,
                      RIGHT_LEVEL,
                      TOP_LEVEL };

    Util();

    static QString getFilename(const QString &path);
    static QString getNameOfFile(const QString &filename);
    static QString changeExtOfFile(const QString &path, const QString &newExt);
    static QString getPathiGroups(const QString &path);
    static QString getPathDataImage(const QString &path, const QString &filename);
    static QColor getColor(const int id);
    static QPixmap getImageCluster();
    static QString getPathImageCluster();
    static QString getPathDirImages();
    static QString getPathImageTransparent();

    static bool isVGI(const QString path);
    static bool isVXL(const QString path);

    static TYPE_LEVEL changeType(TYPE_LEVEL type);
};

#endif // UTIL_H
