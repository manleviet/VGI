//
//
// mainwindow.h
//
// Le principal interface du logiciel
//
// LE Viet Man
// 12/05/10
//
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QErrorMessage>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "UI/choosefiledialog.h"
#include "UI/savefiledialog.h"
#include "UI/graphtreeview.h"
#include "IO/loaddatabaseconfig.h"
#include "IO/graphtlp.h"
#include "IO/graphxml.h"
#include "GraphImage/graphimage.h"
#include "Scene/graphscene.h"

#include <iostream> // pour tester seulement

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui; // Form
    QString path;
    GraphImage *graph;
    GraphScene *scene;
    GraphTreeView *treegraph;

    bool loadFileVGI(const QString &path);
    bool loadFileVML(const QString &path);

private slots:
    void on_radImages_clicked();
    void on_radClusters_clicked();
    void on_btnViewTreeGraph_clicked();
    void on_btnSave_clicked();
    void on_btnOpen_clicked();
    //void sceneChanged();
};

#endif // MAINWINDOW_H
