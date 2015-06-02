//
//
// graphtreeview.h
//
// C'est une classe interface qui sert a afficher le graphe
// sous forme un arbre.
// Il y a deux arbres :
// - l'arbre d'images
// - l'arbre des clusters
// L'utilisateur a une autre facon pour voir les donnees
//
// LE Viet Man
// 17/06/10
//
//

#ifndef GRAPHTREEVIEW_H
#define GRAPHTREEVIEW_H

#include <QDialog>
#include "GraphImage/graphimage.h"

namespace Ui {
    class GraphTreeView;
}

class GraphTreeView : public QDialog {
    Q_OBJECT
public:
    GraphTreeView(GraphImage *graph, QWidget *parent = 0);
    ~GraphTreeView();

    void addGraph(GraphImage *graph);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GraphTreeView *ui;
    GraphImage *graph;

    void loadTreeGraphImage();
};

#endif // GRAPHTREEVIEW_H
