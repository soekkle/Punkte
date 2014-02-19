#include "diagramm.h"
#include "ui_diagramm.h"
/*!
 *@autor soekkle
 *@date 17.02.14
 *@version 0.1
*/

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
//! Die Funktion Berechnet die Anzahl der Striche für die Achsen somie die Scallierung
/*!
 *Berechnet wie die Achsen Gezeichnet werden sollen. Und Das Diagram in Eine Richtung Scallirt werden soll.
 *@param[in] Stuffen Anzahl der Stuffen für die Scalliert werden soll.
 *@param[in] Lange Anzahl der Pixel die Für die Ache zur verfügung stehen.
 *@param[out] Schritt ist giebtden Faktor aus mit, dem die Einheit scalliert wurde
 *@return Anzahl der Abschnitte Auf der Achse
*/
int Diagramm::anzSchritte(int Stuffen, int Lange, float *Schritt)
{
    if(Stuffen==0)//Um Division durch Null zu verhindern
        Stuffen=1;
    *Schritt=Lange/Stuffen;//Berechnen der ersten scallierung
    while ((*Schritt<25)&&(Stuffen>10))//Der wert muss noch angpasst werden.
    {
        Stuffen=Stuffen/10;
        *Schritt=Lange/Stuffen;
    }
    return Stuffen;//Wiedergabe des Berechneten wertes.
}
//! Wirde Beim Schließen Ausgeführt
/*!
 *Leert die Zeichnung.
*/
void Diagramm::closeEvent(QCloseEvent *event)
{
    delete Zeichnung;
    offen=false;//Setzen, dass das geschlossen ist.
    event->accept();
}
//! Aktuallisiert die Angezeigten Daten
void Diagramm::DatenGeaendert()
{
    if (offen)
        zeichnen();
}
//! Passt die Größe der Zeichnung der Fenstergröße an.
void Diagramm::resizeEvent(QResizeEvent *)
{
    if (offen)//Prüfen ob die Zeichnung angezeigt wird.
    {
        Breite=ui->graphicsView->width()-15;//Setzen der Neuen Größe in den Privaten Variablen.
        Hoehe=ui->graphicsView->height()-15;
        zeichnen();//Diagramm neu Zeichnen Lassen.
    }
}
//! Setzt die übergebene Liste zum Zeichnen
/*!
 *@param[in] newListe Pinter auf die Neue Liste.
*/
void Diagramm::setListe(Liste *newListe)
{
    if (newListe!=NULL)//Prüft ob ein NULL-Pointer Übergeben wurde
        Kurse=newListe;
}

//! Öfnnet das Diagrammfenster
/*!
 *Wenn noch nicht das Diagrammfenster offen ist wird es geöffnet,
*/
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
//! Zeichnet das Diagramm
/*!
 *Die Funktion Zeichnet Das Diagramm in das Fenstr hinnein
*/
void Diagramm::zeichnen()
{
    if (Kurse==NULL)//Prüft ob ein NULL Pointer vorliegt.
        return;
    if (offen)//Wenn Schon eine Zeichenfläche Existert wird sie Gelöscht.
        delete Zeichnung;
    Zeichnung=new QGraphicsScene(this);//Erstellt einen Neuezeichenfläche
    ui->graphicsView->setScene(Zeichnung);
    int Blatter=Kurse->maxBlatter();//Ruft die Maximale Anzahl von Blättern ab.
    zeichneYAchse(50,25,Hoehe-25);//Zeichnet eine X-Achse
    zeichneXAchse(50,Hoehe,Breite-50,Blatter-1);//Zeichnet eine y-Achse.
    if (Blatter==0)//Bricht bei 0 Blättern ab.
        return;
    int Schritt=(Breite-50)/Blatter;//Berechnet die Schrittbreite Für ein Blatt
    for (vector<Kurs*>::const_iterator iter=Kurse->begin();iter!=Kurse->end();++iter)//Zeichnet für jeden Kurs
    {
        Kurs *Element=*iter;
        if (Element->anzBlaetter()==0)//Prüft ob Blätter angelegt wurden.
        {
            continue;
        }
        QPainterPath Linie(QPointF(50,25+(Hoehe-25)*(1-Element->getVerhalt(0))));//Setzt den Startpunkt.
        for (int i=1;i<Element->anzBlaetter();++i)
        {
            Linie.lineTo(QPointF(i*Schritt+50,25+(Hoehe-25)*(1-Element->getVerhalt(i)) ));//Setzt die Punkte der Linie
        }
        QPen Farbe(Element->getQColor());//Setzt die Farbe
        Zeichnung->addPath(Linie,Farbe);//Zeichnet das Element.
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
