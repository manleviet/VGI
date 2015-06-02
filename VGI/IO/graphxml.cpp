//
//
// graphxml.cpp
//
// Implementation de la classe graphxml.h
//
// LE Viet Man
// 16/05/10
//
//

#include "IO/graphxml.h"

//
// Construction
//
GraphXML::GraphXML(GraphImage *graph)
{
    this->graph = graph;
}

//
// Enregistrer le GraphImage sur le fichier VML
// @param :
//      const QString &filename : le chemin et le nom du fichier VML
// @result:
//      false : s'il y a des erreur
//      true : si non
//
bool GraphXML::saveGraph(const QString &filename)
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

    // CREER XML
    QDomDocument doc;

    // creer tag <vml1.0>
    QDomElement vml = doc.createElement("vml1.0");
    doc.appendChild(vml);

    // creer tag <graph>
    QDomElement root = doc.createElement("graph");
    doc.appendChild(root);

    // ajouter les tags <image>
    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        Image *image = graph->getImage(i);
        QDomElement imageNode = doc.createElement("image");
        imageNode.setAttribute("id", image->getId());
        imageNode.setAttribute("path", image->getPath());
        root.appendChild(imageNode);
    }

    // ajouter les tags <cluster>
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        QDomElement clusterNode = doc.createElement("cluster");
        clusterNode.setAttribute("id", cluster->getId());

        // ajouter les tags <image> designant ces images qui fait partie de ce cluster
        for (int j = 0; j < cluster->getQuantityChildrens(); j++)
        {
            Image *image = cluster->getChildren(j);
            QDomElement imageNode = doc.createElement("image");
            imageNode.setAttribute("id", image->getId());
            clusterNode.appendChild(imageNode);
        }

        root.appendChild(clusterNode);
    }
    
    if (graph->getVGIVersion() == Util::VGI20)
    {
        // ajouter les tags <cluster> avec <subcluster>
        for (int i = 0; i < graph->getQuantityCluster(); i++)
        {
            Image *cluster = graph->getCluster(i);
            QDomElement clusterNode = doc.createElement("cluster");
            clusterNode.setAttribute("id", cluster->getId());

            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                Image *subcluster = cluster->getSubCluster(j);
                QDomElement subclusterNode = doc.createElement("subcluster");
                subclusterNode.setAttribute("id", subcluster->getId());

                for (int k = 0; k < subcluster->getQuantityChildrens(); k++)
                {
                    Image *image = subcluster->getChildren(k);
                    QDomElement imageNode = doc.createElement("image");
                    imageNode.setAttribute("id", image->getId());
                    subclusterNode.appendChild(imageNode);
                }

                clusterNode.appendChild(subclusterNode);
            }
            root.appendChild(clusterNode);
        }
    }

    // ajouter les tags <link>
    // type="cluster"
    for (int i = 0; i < graph->getQuantityLineClusters(); i++)
    {
        Line *line = graph->getLineCluster(i);
        QDomElement lineNode = doc.createElement("line");
        Image *end = (Image*)line->end();
        lineNode.setAttribute("end", end->getId());
        Image *start = (Image*)line->start();
        lineNode.setAttribute("start", start->getId());
        lineNode.setAttribute("size", line->getSize());
        QColor color = line->getColor();
        lineNode.setAttribute("color", color.rgb());
        root.appendChild(lineNode);
    }
    // type="image"
    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        for (int j = 0; j < cluster->getQuantityLinesChildren(); j++)
        {
            Line *line = cluster->getLineChildren(j);
            QDomElement lineNode = doc.createElement("line");
            lineNode.setAttribute("cluster", cluster->getId());
            Image *end = (Image*)line->end();
            lineNode.setAttribute("end", end->getId());
            Image *start = (Image*)line->start();
            lineNode.setAttribute("start", start->getId());
            lineNode.setAttribute("size", line->getSize());
            QColor color = line->getColor();
            lineNode.setAttribute("color", color.rgb());
            root.appendChild(lineNode);
        }
    }

    vml.appendChild(root);

    // enregistrer sur le fichier VML
    QTextStream out(&file);
    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
    doc.insertBefore(xmlNode, doc.firstChild());
    const int Indent = 4;
    doc.save(out, Indent);

    return true;
}
