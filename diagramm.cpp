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

int Diagramm::anzSchritte(int Stuffen, int Lange, float *Schritt)
{
    *Schritt=Lange/Stuffen;
    while ((*Schritt<25)&&(Stuffen>10))//Der wert muss noch angpasst werden.
    {
        Stuffen=Stuffen/10;
        *Schritt=Lange/Stuffen;
    }
    return Stuffen;
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
    int Blatter=Kurse->maxBlatter()-1;
    zeichneYAchse(50,25,Hoehe-25);
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
    /*Zeichnung->addLine(x+Lange-15,y-15,x+Lange,y);
    Zeichnung->addLine(x+Lange-15,y+15,x+Lange,y);*/
    QGraphicsTextItem * Text = new QGraphicsTextItem;//Objekt für die Texte der Legende.
    Text->setPos(x,y+10);
    Text->setPlainText("1");
    Zeichnung->addItem(Text);
    float Weite=0;
    int Anz=anzSchritte(Elemente,Lange,&Weite);
    int EleSchritt=Elemente/Anz;
    for (int i=1;i<=Anz;++i)
    {
        Zeichnung->addLine(i*Weite+x,y+15,i*Weite+x,y-15);
        Text =new QGraphicsTextItem;
        Text->setPlainText(QString("%1").arg(i*EleSchritt+1));
        Text->setPos(i*Weite+x,y+10);
        Zeichnung->addItem(Text);
    }
}

void Diagramm::zeichneYAchse(int x, int y, int Lange)
{
    Zeichnung->addLine(x,y,x,y+Lange);
    /*Zeichnung->addLine(x-15,y+15,x,y);
    Zeichnung->addLine(x+15,y+15,x,y);*/
    float Weite=0;
    int Anz=anzSchritte(100,Lange,&Weite);
    int proSchritt=100/Anz;
    for (int i=0;i<Anz;++i)
    {
        float yPos=y+Lange-((i+1)*Weite);
        Zeichnung->addLine(x-15,yPos,x+15,yPos);
        QGraphicsTextItem * Text = new QGraphicsTextItem;//Objekt für die Texte der Legende.
        Text->setPos(x-40,yPos);
        Text->setPlainText(QString("%1%").arg((i+1)*proSchritt));
        Zeichnung->addItem(Text);
    }
}
