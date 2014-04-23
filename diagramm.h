#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QWidget>
#include <QCloseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "liste.h"

namespace Ui {
class Diagramm;
}

class Diagramm : public QWidget
{
    Q_OBJECT
    
public:
    explicit Diagramm(QWidget *parent = 0);
    ~Diagramm();
    void setListe(Liste* newListe);
public slots:
    void DatenGeaendert();
    void show();
protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    Ui::Diagramm *ui;
    int Breite;
    int Hoehe;
    Liste* Kurse;
    bool offen;
    QGraphicsScene *Zeichnung;
    int anzSchritte(int Stuffen,int Lange,float *Schritt);
    void zeichnen();
    float zeichneXAchse(int x,int y,int Lange,int Elemente);
    void zeichneYAchse(int x,int y,int Lange);
};

#endif // DIAGRAMM_H
