//
//
// graphxml.h
//
// Charger et enregistrer l'object GraphImage sur un fichier XML
//
// LE Viet Man
// 16/05/10
//
//

#ifndef GRAPHXML_H
#define GRAPHXML_H

#include <QDomDocument>
#include <QDomElement>
#include "GraphImage/graphimage.h"

class GraphXML
{
public:
    GraphXML(GraphImage *graph);

    bool saveGraph(const QString &filename);

private:
    GraphImage *graph;
};

#endif // GRAPHXML_H
