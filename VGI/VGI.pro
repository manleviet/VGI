# -------------------------------------------------
# Project created by QtCreator 2010-05-26T20:42:49
# -------------------------------------------------
QT += xml
TARGET = VGI
TEMPLATE = app
SOURCES += main.cpp \
    util.cpp \
    GraphImage/line.cpp \
    GraphImage/image.cpp \
    GraphImage/graphimage.cpp \
    Scene/linescene.cpp \
    Scene/levelscene.cpp \
    Scene/imagescene.cpp \
    Scene/graphscene.cpp \
    UI/savefiledialog.cpp \
    UI/graphtreeview.cpp \
    UI/choosefiledialog.cpp \
    UI/mainwindow.cpp \
    IO/loaddatabaseconfig.cpp \
    IO/graphxml.cpp \
    IO/graphtlp.cpp \
    Scene/linelevelscene.cpp
HEADERS += util.h \
    GraphImage/line.h \
    GraphImage/image.h \
    GraphImage/graphimage.h \
    Scene/linescene.h \
    Scene/levelscene.h \
    Scene/imagescene.h \
    Scene/graphscene.h \
    UI/savefiledialog.h \
    UI/graphtreeview.h \
    UI/choosefiledialog.h \
    UI/mainwindow.h \
    IO/loaddatabaseconfig.h \
    IO/graphxml.h \
    IO/graphtlp.h \
    Scene/linelevelscene.h
FORMS += mainwindow.ui \
    choosefiledialog.ui \
    savefiledialog.ui \
    graphtreeview.ui
OTHER_FILES += Notes.txt
