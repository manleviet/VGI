/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jun 22 23:13:54 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionOpen_BagOfWord;
    QAction *action_Save_as;
    QAction *actionE_xit;
    QAction *actionZoom_In_25;
    QAction *actionZoom_Out_25;
    QAction *actionNormal_Size;
    QAction *actionFit_to_Window;
    QAction *actionChoose_Color;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *lblImage;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpen->setShortcutContext(Qt::WidgetShortcut);
        actionOpen_BagOfWord = new QAction(MainWindow);
        actionOpen_BagOfWord->setObjectName(QString::fromUtf8("actionOpen_BagOfWord"));
        actionOpen_BagOfWord->setEnabled(false);
        action_Save_as = new QAction(MainWindow);
        action_Save_as->setObjectName(QString::fromUtf8("action_Save_as"));
        action_Save_as->setEnabled(false);
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        actionZoom_In_25 = new QAction(MainWindow);
        actionZoom_In_25->setObjectName(QString::fromUtf8("actionZoom_In_25"));
        actionZoom_In_25->setEnabled(false);
        actionZoom_Out_25 = new QAction(MainWindow);
        actionZoom_Out_25->setObjectName(QString::fromUtf8("actionZoom_Out_25"));
        actionZoom_Out_25->setEnabled(false);
        actionNormal_Size = new QAction(MainWindow);
        actionNormal_Size->setObjectName(QString::fromUtf8("actionNormal_Size"));
        actionNormal_Size->setEnabled(false);
        actionFit_to_Window = new QAction(MainWindow);
        actionFit_to_Window->setObjectName(QString::fromUtf8("actionFit_to_Window"));
        actionFit_to_Window->setCheckable(true);
        actionFit_to_Window->setEnabled(false);
        actionChoose_Color = new QAction(MainWindow);
        actionChoose_Color->setObjectName(QString::fromUtf8("actionChoose_Color"));
        actionChoose_Color->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 580, 355));
        lblImage = new QLabel(scrollAreaWidgetContents);
        lblImage->setObjectName(QString::fromUtf8("lblImage"));
        lblImage->setGeometry(QRect(0, 0, 7, 16));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblImage->sizePolicy().hasHeightForWidth());
        lblImage->setSizePolicy(sizePolicy);
        lblImage->setScaledContents(true);
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_BagOfWord);
        menuFile->addAction(action_Save_as);
        menuFile->addSeparator();
        menuFile->addAction(actionE_xit);
        menuView->addAction(actionChoose_Color);
        menuView->addSeparator();
        menuView->addAction(actionZoom_In_25);
        menuView->addAction(actionZoom_Out_25);
        menuView->addAction(actionNormal_Size);
        menuView->addSeparator();
        menuView->addAction(actionFit_to_Window);

        retranslateUi(MainWindow);
        QObject::connect(actionE_xit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Visual Word Viewer", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "&Open Image...", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionOpen_BagOfWord->setText(QApplication::translate("MainWindow", "Open &BagOfWord...", 0, QApplication::UnicodeUTF8));
        actionOpen_BagOfWord->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        action_Save_as->setText(QApplication::translate("MainWindow", "&Save as...", 0, QApplication::UnicodeUTF8));
        action_Save_as->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        actionE_xit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionZoom_In_25->setText(QApplication::translate("MainWindow", "Zoom &In (25%)", 0, QApplication::UnicodeUTF8));
        actionZoom_In_25->setShortcut(QApplication::translate("MainWindow", "Ctrl++", 0, QApplication::UnicodeUTF8));
        actionZoom_Out_25->setText(QApplication::translate("MainWindow", "Zoom &Out (25%)", 0, QApplication::UnicodeUTF8));
        actionZoom_Out_25->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionNormal_Size->setText(QApplication::translate("MainWindow", "&Normal Size", 0, QApplication::UnicodeUTF8));
        actionNormal_Size->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionFit_to_Window->setText(QApplication::translate("MainWindow", "Fit to Window", 0, QApplication::UnicodeUTF8));
        actionFit_to_Window->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionChoose_Color->setText(QApplication::translate("MainWindow", "Choose &Color", 0, QApplication::UnicodeUTF8));
        actionChoose_Color->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        lblImage->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
