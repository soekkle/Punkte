#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QInputDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QItemSelectionModel>
#include <QFile>
#include <QTextStream>

#include "kurs.h"

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

    QString SpeichernDialog();
    bool Speichern();

    Ui::MainWindow *ui;
    QStandardItemModel *Tabelle,*Liste;
    vector<Kurs*> Kurse;
    int Auswahl;
    QString SpeicherOrt;
private slots:
    void slotClose();
    void slotNeuesBlatt();
    void slotNeuerKurs();
    void slotSpeichern();
    void slotSpeichernunter();
    void selectionChangedSlot(const  QItemSelection &,const QItemSelection & );
};

#endif // MAINWINDOW_H
