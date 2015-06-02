//
//
// graphtlp.cpp
//
// Implementation de la classe graphtlp.h
//
// LE Viet Man
// 18/05/10
//
//

#include "IO/graphtlp.h"

//
// Construction de la classe
//
GraphTLP::GraphTLP(GraphImage *graph)
{
    this->graph = graph;
}

//
// Enregistrer le GraphImage sur le fichier TLP
// @param :
//      bool simple : true - simple, false - complet, utilise seulement pour vgi1.0
//      const QString &filename : le chemin et le nom du fichier TLP
// @result:
//      false : s'il y a des erreur
//      true : si non
//
bool GraphTLP::saveGraph(bool simple, const QString &filename)
{
    if (graph == NULL)
    {
        return false;
    }

    if (filename.isEmpty()) return false;

    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }

    // ecrire les informations sur le fichier
    QTextStream out(&file);

    // header TLP
    out << TLP_HEADER << endl;
    // date
    out << TLP_DATE_BEGIN << QDate::currentDate().toString("dd-MM-yy") << TLP_DATE_END << endl;
    // author
    out << TLP_AUTHOR << endl;
    //comments
    out << TLP_COMMENTS << endl;

    // Il y a trois cas :
    //      - vgi1.0 - complete
    //      - vgi1.0 - simple
    //      - vgi2.0 - simple
    if (graph->getVGIVersion() == Util::VGI10)
    {
        if (simple)
            out << getContentVGI1Simple() << endl;
        else
            out << getContentVGI1Complet() << endl;
    }
    else
        out << getContentVGI2Simple() << endl;

    // tag Attributes
    out << getContentTagAttributes() << endl;

    // tag Scene
    out << TLP_SCENE << endl;

    // enclosing TLP
    out << TLP_ENCLOSING;

    return true;
}

//////////////////////////////////////////
// VGI 1.0 - Complet
//////////////////////////////////////////
QString GraphTLP::getContentVGI1Complet()
{
    QString content;

    // tag nodes
    content = getContentVGI1CompletTagNodes() + "\n";

    // tag edges
    content += getContentVGI1CompletTagEdges() + "\n";

    // tag property clusters : image fait partie de quel cluster
    content += getContentVGI1CompletTagPropertyCluster() + "\n";

    // tag viewColor
    content += getContentVGI1CompletTagColor() + "\n";

    // tag viewLabel
    content += getContentVGI1CompletTagLabel() + "\n";

    // tag viewTexture
    content += getContentVGI1CompletTagTexture();

    return content;
}

QString GraphTLP::getContentVGI1CompletTagNodes()
{
    QString content = ";(nodes <node_id> <node_id> ...)\n"; // ajouter un commentaire

    // ouvrir le tag nodes
    content += "(nodes";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        Image *image = graph->getImage(i);
        content += QString(" %1").arg(image->getId());
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1CompletTagEdges()
{
    QString content = ";(edge <edge_id> <source_id> <target_id>)\n"; // ajouter un commentaire

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        for (int j = 0; j < cluster->getQuantityLinesChildren(); j++)
        {
            Line *line = cluster->getLineChildren(j);
            Image *start = (Image*)line->start();
            Image *end = (Image*)line->end();

            content += QString("(edge %1 %2 %3)\n")
                      .arg(line->getId())
                      .arg(start->getId())
                      .arg(end->getId());
        }
    }

    return content;
}

QString GraphTLP::getContentVGI1CompletTagPropertyCluster()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"clusters\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        QString clusters = "";

        for (int i = 0; i < image->getQuantityClusters(); i++)
        {
            clusters += QString("%1 ").arg(image->getCluster(i)->getId());
        }

        content += QString("%1 \"%2\"")
                   .arg(image->getId())
                   .arg(clusters);
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1CompletTagColor()
{
    // ouvrir le tag property
    QString content = "(property 0 color \"viewColor\"\n";
    content += "(default \"(235,0,23,255)\" \"(0,0,0,0)\")\n";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        for (int j = 0; j < cluster->getQuantityLinesChildren(); j++)
        {
            Line *line = cluster->getLineChildren(j);
            QColor color = line->getColor();

            QString strColor = QString("(%1,%2,%3,%4)")
                       .arg(color.red())
                       .arg(color.green())
                       .arg(color.blue())
                       .arg(color.alpha());

            content += QString("(edge %1 \"%2\")\n")
                      .arg(line->getId())
                      .arg(strColor);
        }
    }

    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1CompletTagLabel()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewLabel\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(image->getId())
                   .arg(Util::getNameOfFile(image->getFilename()));
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1CompletTagTexture()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewTexture\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(image->getId())
                   .arg(image->getPath());
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

//////////////////////////////////////////
// VGI 1.0 - Simple
//////////////////////////////////////////
QString GraphTLP::getContentVGI1Simple()
{
    QString content;

    // tag nodes
    content = getContentVGI1SimpleTagNodes() + "\n";

    // tag edges
    content += getContentVGI1SimpleTagEdges() + "\n";

    // tag property clusters : image fait partie de quel cluster
    content += getContentVGI1SimpleTagPropertyCluster() + "\n";

    // tag viewColor
    content += getContentVGI1SimpleTagColor() + "\n";

    // tag viewLabel
    content += getContentVGI1SimpleTagLabel() + "\n";

    // tag viewTexture
    content += getContentVGI1SimpleTagTexture();

    return content;
}

QString GraphTLP::getContentVGI1SimpleTagNodes()
{
    QString content = ";(nodes <node_id> <node_id> ...)\n"; // ajouter un commentaire

    // ouvrir le tag nodes
    content += "(nodes";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        content += QString(" %1").arg(cluster->getId());
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        Image *image = graph->getImage(i);
        content += QString(" %1").arg(graph->getQuantityCluster() + image->getId());
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1SimpleTagPropertyCluster()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"clusters\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        QString clusters = "";

        for (int i = 0; i < image->getQuantityClusters(); i++)
        {
            clusters += QString("%1 ").arg(image->getCluster(i)->getId());
        }

        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster() + image->getId())
                   .arg(clusters);
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1SimpleTagLabel()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewLabel\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        content += "(node ";
        Image *cluster = graph->getCluster(i);
        content += QString("%1 \"cluster %2\"")
                   .arg(cluster->getId())
                   .arg(cluster->getId());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster() + image->getId())
                   .arg(Util::getNameOfFile(image->getFilename()));
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1SimpleTagTexture()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewTexture\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        content += "(node ";
        Image *cluster = graph->getCluster(i);
        content += QString("%1 \"%2\"")
                   .arg(cluster->getId())
                   .arg(cluster->getPath());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster() + image->getId())
                   .arg(image->getPath());
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI1SimpleTagEdges()
{
    QString content = ";(edge <edge_id> <source_id> <target_id>)\n"; // ajouter un commentaire

    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);
        Image *start = (Image*)line->start();
        Image *end = (Image*)line->end();

        content += QString("(edge %1 %2 %3)\n")
                  .arg(i)
                  .arg(start->getId())
                  .arg(end->getId());
    }

    int index = graph->getQuantityLineClusters();
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        for (int j = 0; j < cluster->getQuantityChildrens(); j++)
        {
            Image *image = cluster->getChildren(j);
            content += QString("(edge %1 %2 %3)\n")
                      .arg(index + j)
                      .arg(cluster->getId())
                      .arg(graph->getQuantityCluster() + image->getId());
        }
        index += cluster->getQuantityChildrens();
    }

    return content;
}

QString GraphTLP::getContentVGI1SimpleTagColor()
{
    // ouvrir le tag property
    QString content = "(property 0 color \"viewColor\"\n";
    content += "(default \"(235,0,23,255)\" \"(0,0,0,0)\")\n";

    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);
        QColor color = line->getColor();

        QString strColor = QString("(%1,%2,%3,%4)")
                   .arg(color.red())
                   .arg(color.green())
                   .arg(color.blue())
                   .arg(color.alpha());

        content += QString("(edge %1 \"%2\")\n")
                  .arg(i)
                  .arg(strColor);
    }

    int index = graph->getQuantityLineClusters();
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        for (int j = 0; j < cluster->getQuantityChildrens(); j++)
        {
            QColor color = Util::getColor(cluster->getId());

            QString strColor = QString("(%1,%2,%3,%4)")
                       .arg(color.red())
                       .arg(color.green())
                       .arg(color.blue())
                       .arg(color.alpha());

            content += QString("(edge %1 \"%2\")\n")
                      .arg(index + j)
                      .arg(strColor);
        }
        index += cluster->getQuantityChildrens();
    }

    content += ")";
    return content;
}

/////////////////////////////////////////
// VGI 2.0 - Simple
/////////////////////////////////////////
QString GraphTLP::getContentVGI2Simple()
{
    QString content;

    // tag nodes
    content = getContentVGI2SimpleTagNodes() + "\n";

    // tag edges
    content += getContentVGI2SimpleTagEdges() + "\n";

    // tag property clusters : image fait partie de quel cluster
    content += getContentVGI2SimpleTagPropertyCluster() + "\n";

    // tag viewColor
    content += getContentVGI2SimpleTagColor() + "\n";

    // tag viewLabel
    content += getContentVGI2SimpleTagLabel() + "\n";

    // tag viewTexture
    content += getContentVGI2SimpleTagTexture();

    return content;
}

QString GraphTLP::getContentVGI2SimpleTagNodes()
{
    QString content = ";(nodes <node_id> <node_id> ...)\n"; // ajouter un commentaire

    // ouvrir le tag nodes
    content += "(nodes";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        content += QString(" %1").arg(cluster->getId());
    }

    for (int i = 0; i < graph->getQuantitySubCluster(); i++)
    {
        content += QString(" %1").arg(graph->getQuantityCluster() + i);
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        Image *image = graph->getImage(i);
        content += QString(" %1").arg(graph->getQuantityCluster()
                                      + graph->getQuantitySubCluster()
                                      + image->getId());
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI2SimpleTagPropertyCluster()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"clusters\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        QString clusters = "";

        for (int i = 0; i < image->getQuantityClusters(); i++)
        {
            clusters += QString("%1 ").arg(image->getCluster(i)->getId());
        }

        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster()
                        + graph->getQuantitySubCluster()
                        + image->getId())
                   .arg(clusters);
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI2SimpleTagLabel()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewLabel\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        content += "(node ";
        Image *cluster = graph->getCluster(i);
        content += QString("%1 \"cluster %2\"")
                   .arg(cluster->getId())
                   .arg(cluster->getId());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantitySubCluster(); i++)
    {
        content += "(node ";
        Image *subcluster = graph->getSubCluster(i);
        content += QString("%1 \"%2 de %3\"")
                   .arg(graph->getQuantityCluster() + i)
                   .arg(subcluster->getId())
                   .arg(subcluster->getCluster(0)->getId());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster()
                        + graph->getQuantitySubCluster()
                        + image->getId())
                   .arg(Util::getNameOfFile(image->getFilename()));
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI2SimpleTagTexture()
{
    // ouvrir le tag property
    QString content = "(property 0 string \"viewTexture\"\n";

    content += "(default \"\" \"\")\n";

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        content += "(node ";
        Image *cluster = graph->getCluster(i);
        content += QString("%1 \"%2\"")
                   .arg(cluster->getId())
                   .arg(cluster->getPath());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantitySubCluster(); i++)
    {
        content += "(node ";
        Image *subcluster = graph->getSubCluster(i);
        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster() + i)
                   .arg(subcluster->getPath());
        content += ")\n";
    }

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        content += "(node ";
        Image *image = graph->getImage(i);
        content += QString("%1 \"%2\"")
                   .arg(graph->getQuantityCluster()
                        + graph->getQuantitySubCluster()
                        + image->getId())
                   .arg(image->getPath());
        content += ")\n";
    }

    // fermer le tag nodes
    content += ")";
    return content;
}

QString GraphTLP::getContentVGI2SimpleTagEdges()
{
    QString content = ";(edge <edge_id> <source_id> <target_id>)\n"; // ajouter un commentaire

    // noi cac clusters
    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);
        Image *start = (Image*)line->start();
        Image *end = (Image*)line->end();

        content += QString("(edge %1 %2 %3)\n")
                  .arg(i)
                  .arg(start->getId())
                  .arg(end->getId());
    }

    int index = graph->getQuantityLineClusters();
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);

        if (cluster->getQuantitySubCluters() != 0)
        {
            // noi cluster voi sous-cluster
            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                Image *subcluster = cluster->getSubCluster(j);
                content += QString("(edge %1 %2 %3)\n")
                          .arg(index + j)
                          .arg(cluster->getId())
                          .arg(graph->getQuantityCluster() + graph->getIndexSubCluster(subcluster));
            }
            index += cluster->getQuantitySubCluters();
            // noi sous-cluster voi image
            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                Image *subcluster = cluster->getSubCluster(j);
                for (int k = 0; k < subcluster->getQuantityChildrens(); k++)
                {
                    Image *image = subcluster->getChildren(k);
                    content += QString("(edge %1 %2 %3)\n")
                              .arg(index + k)
                              .arg(graph->getQuantityCluster() + graph->getIndexSubCluster(subcluster))
                              .arg(graph->getQuantityCluster()
                                   + graph->getQuantitySubCluster()
                                   + image->getId());
                }
                index += subcluster->getQuantityChildrens();
            }
        }
        else
        {
            for (int j = 0; j < cluster->getQuantityChildrens(); j++)
            {
                Image *image = cluster->getChildren(j);
                content += QString("(edge %1 %2 %3)\n")
                          .arg(index + j)
                          .arg(cluster->getId())
                          .arg(graph->getQuantityCluster()
                               + graph->getQuantitySubCluster()
                               + image->getId());
            }
            index += cluster->getQuantityChildrens();
        }
    }

    return content;
}

QString GraphTLP::getContentVGI2SimpleTagColor()
{
    // ouvrir le tag property
    QString content = "(property 0 color \"viewColor\"\n";
    content += "(default \"(235,0,23,255)\" \"(0,0,0,0)\")\n";

    // noi cac clusters
    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);
        QColor color = line->getColor();

        QString strColor = QString("(%1,%2,%3,%4)")
                   .arg(color.red())
                   .arg(color.green())
                   .arg(color.blue())
                   .arg(color.alpha());

        content += QString("(edge %1 \"%2\")\n")
                  .arg(i)
                  .arg(strColor);
    }

    int index = graph->getQuantityLineClusters();
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);

        if (cluster->getQuantitySubCluters() != 0)
        {
            // noi cluster voi sous-cluster
            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                QColor color = Util::getColor(cluster->getId());

                QString strColor = QString("(%1,%2,%3,%4)")
                           .arg(color.red())
                           .arg(color.green())
                           .arg(color.blue())
                           .arg(color.alpha());

                content += QString("(edge %1 \"%2\")\n")
                          .arg(index + j)
                          .arg(strColor);
            }
            index += cluster->getQuantitySubCluters();
            // noi sous-cluster voi image
            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                Image *subcluster = cluster->getSubCluster(j);
                for (int k = 0; k < subcluster->getQuantityChildrens(); k++)
                {
                    QColor color = Util::getColor(subcluster->getId() % 18);

                    QString strColor = QString("(%1,%2,%3,%4)")
                               .arg(color.red())
                               .arg(color.green())
                               .arg(color.blue())
                               .arg(color.alpha());

                    content += QString("(edge %1 \"%2\")\n")
                              .arg(index + k)
                              .arg(strColor);
                }
                index += subcluster->getQuantityChildrens();
            }
        }
        else
        {
            for (int j = 0; j < cluster->getQuantityChildrens(); j++)
            {
                QColor color = Util::getColor(cluster->getId());

                QString strColor = QString("(%1,%2,%3,%4)")
                           .arg(color.red())
                           .arg(color.green())
                           .arg(color.blue())
                           .arg(color.alpha());

                content += QString("(edge %1 \"%2\")\n")
                          .arg(index + j)
                          .arg(strColor);
            }
            index += cluster->getQuantityChildrens();
        }
    }

    content += ")";
    return content;
}

//
// Prendre le contenu du tag Attributes
// @result : le contenu du tag Attributes
//
QString GraphTLP::getContentTagAttributes()
{
    QString content = "(attributes\n(string \"name\" \"graph_images.tlp\")\n)";
    return content;
}
