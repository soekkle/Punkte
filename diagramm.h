#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWidget>
#include "liste.h"
/*!
 * \brief Die Klasse Diagramm Bereitet die Daten aus einer Liste in einen Diagramm auf.
 * Mit den Einzelen Funktionen Kann man das Diagramm beinflussen, bevor man es sich ausgeben lässt.
 * Standard ist das ur die Gesammtprozentzahl bis einschließlich Blatt n geplottet wird.
 */
class Diagramm
{
public:
    Diagramm(Liste *Kurse, QWidget *parent);
    //! Gibt ein Zeiger auf ein Diagramm zurück
    QGraphicsScene* Ausgabe();
    //! Schaltet zusätzlich die Prozentwerte der Einzelnen Blätter aus.
    void disableEinzelBlaetter();
    //! Schaltet die Hilfslinien aus.
    void disableHilfsLinien();
    //! Schaltet zusätzlich die Prozentwerte der Einzelnen Blätter ein.
    void enableEinzelBlaetter();
    //! Schaltet die Hilfslinien ein.
    void enableHilfsLinien();
    //! Setzt die Maaße der Zeichnung.
    void setMaase(int Breite,int Hoehe);
    //! Schaltet auf einen x Achse mit Blättern um.
    void setXAcheBlatt();
    //! Schaltet auf eine x Achse mit Wochen um.
    void setXAchseWoche();

private:
    int Breite;
    int Hoehe;
    Liste *Kurse;
    QWidget *Parent;
    QPainterPath (*LinieEinzel)(Kurs* Element, double Schritt,int Hoehe);
    QPainterPath (*LinieGesamt)(Kurs* Element, double Schritt,int Hoehe);
    bool zHilfsLinien,EinzelBlaetter,Woche;
    int anzSchritte(int Stuffen, int Lange, float *Schritt);
    //! Zeichnet die X-Achse in ein Diagramm
    static QPainterPath LinieEinzelBlaetter(Kurs* Element, double Schritt,int Hoehe);
    static QPainterPath LinieEinzelWoche(Kurs* Element, double Schritt,int Hoehe);
    static QPainterPath LinieGesamtBlaetter(Kurs* Element, double Schritt, int Hoehe);
    static QPainterPath LinieGesamtWoche(Kurs* Element, double Schritt,int Hoehe);
    float zeichneXAchse(QGraphicsScene* Zeichnung, int x, int y, int Lange, int Elemente);
    //! Zeichnet die Y-Achse in ein Diagramm
    void zeichneYAchse(QGraphicsScene *Zeichnung, int x, int y, int Lange);
};

#endif // DIAGRAMM_H
