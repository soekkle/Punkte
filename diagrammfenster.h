#ifndef DIAGRAMMFENSTER_H
#define DIAGRAMMFENSTER_H

#include <QMainWindow>
#include <QCloseEvent>
#include "diagramm.h"

namespace Ui {
class DiagrammFenster;
}

class DiagrammFenster : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DiagrammFenster(QWidget *parent = 0);
    ~DiagrammFenster();
    void setListe(Liste* newListe);
public slots:
    void DatenGeaendert();
    void show();
protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    
private:
    Ui::DiagrammFenster *ui;
    QGraphicsScene *Graphik;
    bool offen;
    Diagramm *Zeichnung;
    void Zeichnen();
private slots:
    void hilfslinien(bool tiggered);
    void einzelblaetter(bool tiggered);
    void WocheBlaetter(bool tiggered);
};

#endif // DIAGRAMMFENSTER_H
