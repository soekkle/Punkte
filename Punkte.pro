#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T10:58:21
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Punkte
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kurs.cpp \
    liste.cpp \
    neuerkurseingabe.cpp \
    diagrammfenster.cpp \
    diagramm.cpp

HEADERS  += mainwindow.h \
    kurs.h \
    liste.h \
    neuerkurseingabe.h \
    diagrammfenster.h \
    diagramm.h

FORMS    += mainwindow.ui \
    neuerkurseingabe.ui \
    diagrammfenster.ui

OTHER_FILES += \
    README.md
