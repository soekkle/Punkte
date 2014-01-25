#include "diagramm.h"
#include "ui_diagramm.h"

Diagramm::Diagramm(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Diagramm)
{
    ui->setupUi(this);
    offen=false;
    Breite=ui->graphicsView->width();
    Hoehe=ui->graphicsView->height();
    Kurse=NULL;
}

Diagramm::~Diagramm()
{
    delete ui;
}

void Diagramm::closeEvent(QCloseEvent *event)
{
    delete Zeichnung;
    offen=false;
    event->accept();
}

void Diagramm::DatenGeaendert()
{
    zeichnen();
}

void Diagramm::resizeEvent(QResizeEvent *)
{
    if (offen)
    {
        Breite=ui->graphicsView->width()-15;
        Hoehe=ui->graphicsView->height()-15;
        zeichnen();
    }
}

void Diagramm::setListe(Liste *newListe)
{
    if (newListe!=NULL)
        Kurse=newListe;
}

void Diagramm::show()
{
    if (offen)
        return;
    QWidget::show();
    Breite=ui->graphicsView->width()-15;
    Hoehe=ui->graphicsView->height()-15;
    zeichnen();
    offen=true;
}

void Diagramm::zeichnen()
{
    if (Kurse==NULL)
        return;
    if (offen)
        delete Zeichnung;
    Zeichnung=new QGraphicsScene(this);
    ui->graphicsView->setScene(Zeichnung);
    Zeichnung->addLine(50,25,50,Hoehe);
    Zeichnung->addLine(50,Hoehe,Breite,Hoehe);
    Zeichnung->addLine(50,Hoehe,Breite,Hoehe);
    Zeichnung->addLine(Breite-15,Hoehe-15,Breite,Hoehe);
    Zeichnung->addLine(Breite-15,Hoehe+15,Breite,Hoehe);
    Zeichnung->addLine(35,35,50,25);
    Zeichnung->addLine(65,35,50,25);
    int Blatter=Kurse->maxBlatter()+1;
    int Schritt=(Breite-15)/Blatter;
    for (int i=1;i<=Blatter;++i)
    {
        Zeichnung->addLine(i*Schritt,Hoehe+15,i*Schritt,Hoehe-15);
    }
    for (vector<Kurs*>::const_iterator iter=Kurse->begin();iter!=Kurse->end();++iter)
    {
        Kurs *Element=*iter;
        QPolygonF Linie;
        for (int i=0;i<Element->anzBlaetter();++i)
        {
            Linie.append(QPointF((i+1)*Schritt,(Hoehe-25)*(1-Element->getVerhalt(i))));
        }
        QPen Farbe(Element->getQColor());
        Zeichnung->addPolygon(Linie,Farbe);
    }
}
