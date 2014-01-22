#ifndef DIAGRAMM_H
#define DIAGRAMM_H

#include <QWidget>
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
    void close();
    void DatenGeaendert();
    void resize(int w, int h);
    void show();
private:
    Ui::Diagramm *ui;
    int Breite;
    int Hoehe;
    Liste* Kurse;
    bool offen;
    QGraphicsScene *Zeichnung;

    void zeichnen();
};

#endif // DIAGRAMM_H
