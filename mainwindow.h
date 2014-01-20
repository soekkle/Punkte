#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QFile>
#include <QTextStream>
#include <QApplication>

#include "kurs.h"
#include "liste.h"
#include "neuerkurseingabe.h"
#include "diagramm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Liste Kurse;
    int Auswahl;
    QString SpeicherOrt;
    Diagramm Graphik;
    QString SpeichernDialog();
    bool Speichern();
    void leeren();
    bool laden();

private slots:
    void close();
    void slotNeu();
    void slotFarbe();
    void slotNeuesBlatt();
    void slotNeuerKurs();
    void slotLaden();
    void slotSpeichern();
    void slotSpeichernunter();
    void selectionChangedSlot(const  QItemSelection &,const QItemSelection & );
    void slotUber();
};

#endif // MAINWINDOW_H
