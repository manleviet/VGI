
#include <QtGui/QApplication>
#include "UI/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
    w.showMaximized();

    return a.exec();
}
