//
//
// mainwindow.cpp
//
// Implementation de la classe mainwindow.h
//
// LE Viet Man
// 12/05/10
//
//

#include "UI/mainwindow.h"
#include "ui_mainwindow.h"

//
// Construction
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new GraphScene();

    ui->setupUi(this);
    // mettre la taille de la fenetre dessinee
    scene->setSceneRect(QRectF(0, 0, ui->graphicsView->width(), ui->graphicsView->height()));
    ui->graphicsView->setScene(scene);
    this->treegraph = 0;
}

//
// Destruction
//
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//
// Ouvrir un graphe d'image
//
void MainWindow::on_btnOpen_clicked()
{
    // ouvrir et prendre le chemin du fichier de configuration
    ChooseFileDialog dialog(this);
    if (dialog.exec())
    {
        path = dialog.getPathVGI();
    }

    if (path.isEmpty()) return;

    if (Util::isVGI(path))
    {
        // charger des donnees au fichier VGI
        if (!loadFileVGI(path))
            return;
    }
    else
    {
        // charger des donnees au fichier VML
        if (!loadFileVML(path))
            return;
    }

    // mettre a jour l'arbre
    if (this->treegraph != 0)
    {
        treegraph->addGraph(graph);
    }
    else
    {
        this->treegraph = new GraphTreeView(this->graph, this);
    }

    //this->treegraph->show();

    ui->btnSave->setEnabled(true);
    ui->btnViewTreeGraph->setEnabled(true);

    // afficher les clusters
    scene->setSceneRect(QRectF(0, 0, ui->graphicsView->width(), ui->graphicsView->height()));
    scene->clear();
    this->scene->addGraph(this->graph);

    // informer tout marche bien
    //QMessageBox::information(this, tr("Information"), tr("Load done."));
}

//
// Enregistrer un graphe d'image sur un fichier VML ou TLP
//
void MainWindow::on_btnSave_clicked()
{
    QString filename;
    SaveFileDialog dialog(graph->getVGIVersion(), this);
    // utiliser le dialog Save
    if (dialog.exec())
    {
        filename = dialog.getPath();
    }

    if (filename.isEmpty())
        return;

    if (dialog.isTLP())
    {
        // enregristrer sous TLP
        GraphTLP tlp(this->graph);

        if (!tlp.saveGraph(dialog.isSimple(), filename))
        {
            // affichier un bulletin d'erreur
            QErrorMessage errorMessageDialog(this);

            errorMessageDialog.showMessage(
                    QObject::tr("Ne pas enregistrer le graphe d'images."
                       "Il posede peut-etre un faux dans le chemin donne, "
                       "ou dans le nom du fichier."));

            return;
        }
    }
    else
    {
        // enregristrer sous VML
        GraphXML vml(this->graph);

        if (!vml.saveGraph(filename))
        {
            // affichier un bulletin d'erreur
            QErrorMessage errorMessageDialog(this);

            errorMessageDialog.showMessage(
                    QObject::tr("Ne pas enregistrer le graphe d'images."
                       "Il posede peut-etre un faux dans le chemin donne, "
                       "ou dans le nom du fichier."));

            return;
        }
    }

    // informer tout marche bien
    QMessageBox::information(this, tr("Information"), tr("Save done."));
}

//void MainWindow::sceneChanged()
//{
//    this->setWindowModified(true);
//}

//
// Charger des donnees dans un fichier VGI
// @param :
//      const QString &path : le chemin et le nom du fichier VGI
// @result :
//      false : s'il y a des erreur
//      true : si non
//
bool MainWindow::loadFileVGI(const QString &path)
{
    // charger des donnees dans ce fichier
    LoadDatabaseConfig *dbconf = new LoadDatabaseConfig(path);
    if (!dbconf->load()) // il y a des erreurs dans la chargement
    {
        // affichier un bulletin d'erreur
        QErrorMessage errorMessageDialog(this);

        errorMessageDialog.showMessage(
                QObject::tr("Ne pas pouvoir ouvrir le fichier. "
                   "Il posede peut-etre un faux dans le chemin donne, "
                   "ou dans le nom du fichier. "));

        return false;
    }

    // charger des donnees dans la base d'images
    graph = new GraphImage(dbconf);

    int result = 0;
    QString error;
    switch (result = graph->load())
    {
    case 1:
        error = QObject::tr("Ne pas pouvoir charger des images dans la base d'images. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    case 2:
        error = QObject::tr("Ne pas pouvoir charger des clusters de MS. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    case 3:
        error = QObject::tr("Ne pas pouvoir charger des fichiers data de l'image de MS. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    case 4:
        error = QObject::tr("Ne pas pouvoir charger des clusters de SA. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    case 5:
        error = QObject::tr("Ne pas pouvoir charger des fichiers data de l'image de SA. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    case 6:
        error = QObject::tr("Ne pas pouvoir charger des fichiers du sous cluster. "
                            "Il posede peut-etre un faux dans les fichiers donne, "
                            "ou dans le nom du fichier. ");
        break;
    }

    if (result != 0)
    {
        // informer des erreur
        QErrorMessage errorMessageDialog(this);
        errorMessageDialog.showMessage(error);

        delete graph;
        return false;
    }

    return true;
}

//
// Charger des donnees dans un fichier VML
// @param :
//      const QString &path : le chemin et le nom du fichier VML
// @result :
//      false : s'il y a des erreurs
//      true : si non
// PAS ENCORE IMPLEMENTER
//
bool MainWindow::loadFileVML(const QString &path)
{
    QMessageBox::information(this, tr("Information"), path);
    return true;
}

void MainWindow::on_btnViewTreeGraph_clicked()
{
    this->treegraph->show();
}

void MainWindow::on_radClusters_clicked()
{
    this->scene->setShowCluster(true);
}

void MainWindow::on_radImages_clicked()
{
    this->scene->setShowCluster(false);
}
