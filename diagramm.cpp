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
    Zeichnung->addLine(35,35,50,25);
    Zeichnung->addLine(65,35,50,25);
    int Blatter=Kurse->maxBlatter()-1;
    zeichneXAchse(50,Hoehe,Breite-50,Blatter);
    int Schritt=(Breite-50)/Blatter;

    for (vector<Kurs*>::const_iterator iter=Kurse->begin();iter!=Kurse->end();++iter)
    {
        Kurs *Element=*iter;
        if (Element->anzBlaetter()==0)
        {
            continue;
        }
        QPainterPath Linie(QPointF(50,25+(Hoehe-25)*(1-Element->getVerhalt(0))));
        for (int i=1;i<Element->anzBlaetter();++i)
        {
            Linie.lineTo(QPointF(i*Schritt+50,25+(Hoehe-25)*(1-Element->getVerhalt(i)) ));
        }
        QPen Farbe(Element->getQColor());
        Zeichnung->addPath(Linie,Farbe);
    }
}

void Diagramm::zeichneXAchse(int x, int y, int Lange, int Elemente)
{
    Zeichnung->addLine(x,y,x+Lange,y);
    Zeichnung->addLine(x+Lange-15,y-15,x+Lange,y);
    Zeichnung->addLine(x+Lange-15,y+15,x+Lange,y);
    int Schritt=(Lange)/Elemente;
    QGraphicsTextItem * Text = new QGraphicsTextItem;//Objekt für die Texte der Legende.
    Text->setPos(x,y+10);
    Text->setPlainText("1");
    Zeichnung->addItem(Text);
    for (int i=1;i<=Elemente;++i)
    {
        Zeichnung->addLine(i*Schritt+x,y+15,i*Schritt+x,y-15);
        Text =new QGraphicsTextItem;
        Text->setPlainText(QString("%1").arg(i+1));
        Text->setPos(i*Schritt+x,y+10);
        Zeichnung->addItem(Text);
    }
}
