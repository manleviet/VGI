//
//
// graphtreeview.cpp
//
// Implementation de la classe graphtreeview.h
//
// LE Viet Man
// 17/06/10
//
//

#include "UI/graphtreeview.h"
#include "ui_graphtreeview.h"

//
// Construction
//
GraphTreeView::GraphTreeView(GraphImage *graph, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphTreeView)
{
    ui->setupUi(this);
    this->graph = graph;
    loadTreeGraphImage();
}

//
// Destruction
//
GraphTreeView::~GraphTreeView()
{
    delete ui;
}

//
// Mettre a jour un nouveau graphe
//
void GraphTreeView::addGraph(GraphImage *graph)
{
    this->graph = graph;
    loadTreeGraphImage();
}

void GraphTreeView::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//
// Creer les arbres a partir du graphe
//
void GraphTreeView::loadTreeGraphImage()
{
    ui->treeImage->clear();
    ui->treeCluster->clear();

    // charger les donnees a tabImage
    QTreeWidgetItem *treeimage = new QTreeWidgetItem(ui->treeImage);
    treeimage->setText(0, QString("Graph Image"));

    for (int i = 0; i < graph->getQuantityImage(); i++)
    {
        Image *image = graph->getImage(i);
        QTreeWidgetItem *imageItem = new QTreeWidgetItem(treeimage);
        imageItem->setText(0, QString("image %1").arg(image->getFilename()));
        imageItem->setText(1, QString("%1 clusters").arg(image->getQuantityClusters()));
        imageItem->setText(2, QString("%1 subclusters").arg(image->getQuantitySubCluters()));

        if (image->getQuantitySubCluters() != 0)
        {
            // lier au sous-cluster
            for (int j = 0; j < image->getQuantitySubCluters(); j++)
            {
                Image *subcluster = image->getSubCluster(j);
                QTreeWidgetItem *subclusterItem = new QTreeWidgetItem(imageItem);
                subclusterItem->setText(0, QString("subcluster %1").arg(subcluster->getId()));

                // lier au cluster
                for (int k = 0; k < subcluster->getQuantityClusters(); k++)
                {
                    Image *cluster = subcluster->getCluster(k);
                    QTreeWidgetItem *clusterItem = new QTreeWidgetItem(subclusterItem);
                    clusterItem->setText(0, QString("cluster %1").arg(cluster->getId()));
                }
            }
        }
        else
        {
            // lier au cluster
            for (int j = 0; j < image->getQuantityClusters(); j++)
            {
                Image *cluster = image->getCluster(j);
                QTreeWidgetItem *clusterItem = new QTreeWidgetItem(imageItem);
                clusterItem->setText(0, QString("cluster %1").arg(cluster->getId()));
            }
        }
    }

    // charger les donnees a tabCluster
    QTreeWidgetItem *treecluster = new QTreeWidgetItem(ui->treeCluster);
    treecluster->setText(0, QString("Graph Cluster"));

    for (int i = 0; i < graph->getQuantityCluster(); i++)
    {
        Image *cluster = graph->getCluster(i);
        QTreeWidgetItem *clusterItem = new QTreeWidgetItem(treecluster);
        clusterItem->setText(0, QString("cluster %1").arg(cluster->getId()));
        clusterItem->setText(1, QString("%1 subclusters").arg(cluster->getQuantitySubCluters()));
        clusterItem->setText(2, QString("%1 images").arg(cluster->getQuantityChildrens()));

        if (cluster->getQuantitySubCluters() != 0)
        {
            // lier au sous-cluster
            for (int j = 0; j < cluster->getQuantitySubCluters(); j++)
            {
                Image *subcluster = cluster->getSubCluster(j);
                QTreeWidgetItem *subclusterItem = new QTreeWidgetItem(clusterItem);
                subclusterItem->setText(0, QString("subcluster %1").arg(subcluster->getId()));

                // lier a l'image
                for (int k = 0; k < subcluster->getQuantityChildrens(); k++)
                {
                    Image *image = subcluster->getChildren(k);
                    QTreeWidgetItem *imageItem = new QTreeWidgetItem(subclusterItem);
                    imageItem->setText(0, QString("image %1").arg(image->getFilename()));
                }
            }
        }
        else
        {
            // lier a l'image
            for (int j = 0; j < cluster->getQuantityChildrens(); j++)
            {
                Image *image = cluster->getChildren(j);
                QTreeWidgetItem *imageItem = new QTreeWidgetItem(clusterItem);
                imageItem->setText(0, QString("image %1").arg(image->getFilename()));
            }
        }
    }
}
