#-------------------------------------------------
#
# Project created by QtCreator 2013-12-25T10:58:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Punkte
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kurs.cpp \
    liste.cpp \
    neuerkurseingabe.cpp \
    diagramm.cpp

HEADERS  += mainwindow.h \
    kurs.h \
    liste.h \
    neuerkurseingabe.h \
    diagramm.h

FORMS    += mainwindow.ui \
    neuerkurseingabe.ui \
    diagramm.ui

OTHER_FILES += \
    README.md
