#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    color = Qt::black;
    ui->setupUi(this);

    ui->lblImage->setBackgroundRole(QPalette::Base);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
}

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

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        origine.load(fileName);
        image = origine;
        if (image.isNull()) {
            QMessageBox::information(this, tr("Visual Word Viewer"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        ui->lblImage->setPixmap(QPixmap::fromImage(image));

        scaleFactor = 1.0;

        ui->actionFit_to_Window->setEnabled(true);
        ui->actionOpen_BagOfWord->setEnabled(true);
        ui->action_Save_as->setEnabled(true);
        updateActions();

        if (!ui->actionFit_to_Window->isChecked())
            ui->lblImage->adjustSize();
    }
}

void MainWindow::on_actionOpen_BagOfWord_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    tr("Open Bag Of Word"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        readBagOfWord(fileName);
        drawBagOfWord();

        ui->actionChoose_Color->setEnabled(true);

        ui->lblImage->setPixmap(QPixmap::fromImage(image));
    }
}

void MainWindow::on_action_Save_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), QDir::currentPath());

    if (!filename.isEmpty())
    {
        image.save(filename, "png");
    }
}

void MainWindow::on_actionZoom_In_25_triggered()
{
    scaleImage(1.25);
}

void MainWindow::on_actionZoom_Out_25_triggered()
{
    scaleImage(0.8);
}

void MainWindow::on_actionNormal_Size_triggered()
{
    ui->lblImage->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::on_actionFit_to_Window_triggered()
{
    bool fitToWindow = ui->actionFit_to_Window->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        on_actionNormal_Size_triggered();
    }
    updateActions();
}

void MainWindow::on_actionChoose_Color_triggered()
{
    color = QColorDialog::getColor(color, this);

    drawBagOfWord();

    ui->lblImage->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::updateActions()
{
    ui->actionZoom_In_25->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionZoom_Out_25->setEnabled(!ui->actionFit_to_Window->isChecked());
    ui->actionNormal_Size->setEnabled(!ui->actionFit_to_Window->isChecked());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->lblImage->pixmap());
    scaleFactor *= factor;
    ui->lblImage->resize(scaleFactor * ui->lblImage->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    ui->actionZoom_In_25->setEnabled(scaleFactor < 3.0);
    ui->actionZoom_Out_25->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::readBagOfWord(QString filename)
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

void MainWindow::drawBagOfWord()
{
    // paint the ellipse
    image = origine;
    QPainter p(&image);

    p.setRenderHint(QPainter::Antialiasing, true);
    //p.setPen(QPen(Qt::black, 2, Qt::DashLine, Qt::SolidPattern));
    //p.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    //p.drawEllipse(80, 80, 400, 240);

    //p.setPen(Qt::SolidPattern);
    //p.rotate(30);
    //QPoint(width,height);

    p.setPen(color);
    for(int i = 0; i < regions.length(); i++)
    {
        RegionOfWord region = regions.at(i);

        p.save();
        p.rotate(region.angle());
        p.drawEllipse(region.x(), region.y(), 5, 3);//region.a(), region.b());
        p.restore();
    }
    //p.rotate(30);
    p.end();
}
