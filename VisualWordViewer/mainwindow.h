#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QScrollBar>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QPainter>
#include <QColorDialog>
#include "regionofword.h"

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
    Ui::MainWindow *ui;

    QImage origine;
    QImage image;
    QColor color;
    QList<RegionOfWord> regions;

    double scaleFactor;

    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    void readBagOfWord(QString filename);
    void drawBagOfWord();

private slots:
    void on_actionChoose_Color_triggered();
    void on_actionFit_to_Window_triggered();
    void on_actionNormal_Size_triggered();
    void on_action_Save_as_triggered();
    void on_actionOpen_BagOfWord_triggered();
    void on_actionZoom_Out_25_triggered();
    void on_actionZoom_In_25_triggered();
    void on_actionOpen_triggered();
};

#endif // MAINWINDOW_H
