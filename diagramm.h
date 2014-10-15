#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWidget>
#include "liste.h"

class Diagramm
{
public:
    Diagramm(Liste *Kurse, QWidget *parent);
    QGraphicsScene* Ausgabe();
    void disableHilfsLinien();
    void enableHilfsLinien();
    void setMaase(int Breite,int Hoehe);

private:
    int Breite;
    int Hoehe;
    Liste *Kurse;
    QWidget *Parent;
    bool zHilfsLinien;
    int anzSchritte(int Stuffen, int Lange, float *Schritt);
    //! Zeichnet die X-Achse in ein Diagramm
    float zeichneXAchse(QGraphicsScene* Zeichnung, int x, int y, int Lange, int Elemente);
    //! Zeichnet die Y-Achse in ein Diagramm
    void zeichneYAchse(QGraphicsScene *Zeichnung, int x, int y, int Lange);
};

#endif // DIAGRAMM_H
