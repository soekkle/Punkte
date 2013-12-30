#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QInputDialog>
#include <QMainWindow>

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
    Ui::MainWindow *ui;
    QStandardItemModel *Tabelle,*Liste;
    vector<Kurs*> Kurse;
    int Auswahl;
private slots:
    void slotClose();
    void slotNeuesBlatt();
    void slotNeuerKurs();
};

#endif // MAINWINDOW_H
