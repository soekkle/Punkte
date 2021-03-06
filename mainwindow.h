#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QCloseEvent>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QApplication>

#include "kurs.h"
#include "liste.h"
#include "neuerkurseingabe.h"
#include "diagrammfenster.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent( QCloseEvent *event );

private:
    Ui::MainWindow *ui;
    Liste Kurse;
    int Auswahl;
    QString SpeicherOrt;
    DiagrammFenster Graphik;
    void setWoche();
    QString SpeichernDialog();
    bool Speichern();
    void leeren();
    bool laden();
    unsigned int Dateityp;//Zum Speichern des Verwendeten Dateitypens.

private slots:
    void slotNeu();
    void slotFarbe();
    void slotNeuesBlatt();
    void slotNeuerKurs();
    void slotKursBearbeiten();
    void slotLaden();
    bool slotSpeichern();
    void slotSpeichernunter();
    void selectionChangedSlot(const  QItemSelection &,const QItemSelection & );
    void slotUber();
};

#endif // MAINWINDOW_H
