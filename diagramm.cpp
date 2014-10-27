#include "diagramm.h"

Diagramm::Diagramm(Liste *Kurse,QWidget *parent = 0)
{
    this->Kurse=Kurse;
    Parent=parent;
    zHilfsLinien=false;
    EinzelBlaetter=false;
    setXAcheBlatt();
}

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
    int LastDiff=2;
    while ((*Schritt<25)&&(Stuffen>10))//Der wert muss noch angpasst werden.
    {
        if (LastDiff==2)
            LastDiff=5;
        else
            LastDiff=2;
        Stuffen=Stuffen/LastDiff;
        *Schritt=Lange/Stuffen;
    }
    return Stuffen;//Wiedergabe des Berechneten wertes.
}


QGraphicsScene* Diagramm::Ausgabe()
{
    QGraphicsScene* Zeichnung=new QGraphicsScene(Parent);
    int anzElemente=0;
    if (Woche)
        anzElemente=Kurse->maxWoche();//Ruft die Maximale Anzahl von Wochen ab.
    else
        anzElemente=Kurse->maxBlatter();//Ruft die Maximale Anzahl von Blättern ab.
    zeichneYAchse(Zeichnung,50,25,Hoehe-25);//Zeichnet eine X-Achse
    float Schritt=zeichneXAchse(Zeichnung,50,Hoehe,Breite-50,anzElemente);
    if (anzElemente==0)//Bricht bei 0 Blättern ab.
        return Zeichnung;
    for (vector<Kurs*>::const_iterator iter=Kurse->begin();iter!=Kurse->end();++iter)
    {
        Kurs *Element=*iter;
        if (Element->anzBlaetter()==0)//Prüft ob Blätter angelegt wurden.
        {
            continue;
        }
        QPen Farbe(Element->getQColor());//Setzt die Farbe
        QPainterPath Linie=LinieGesamt(Element,Schritt,Hoehe-25);
        Zeichnung->addPath(Linie,Farbe);//Zeichnet das Element.
        if (EinzelBlaetter)
        {
            Farbe.setStyle(Qt::DotLine);
            Linie=LinieEinzel(Element,Schritt,Hoehe-25);
            Zeichnung->addPath(Linie,Farbe);
        }
    }
    if (zHilfsLinien)
    {
        float xstart,xende,y;
        xstart=50;
        xende=Breite;
        y=25+(Hoehe-25)/2;
        QPen Gestrichelt(Qt::lightGray,1,Qt::DashLine);
        Zeichnung->addLine(xstart,y,xende,y,Gestrichelt);//Zeichnet einen Hilfslinie bei 50%
        y=25+(Hoehe-25)/4;
        Zeichnung->addLine(xstart,y,xende,y,Gestrichelt);//Zeichnent einen Hilfslinie bei 75%
    }
    return Zeichnung;
}

void Diagramm::disableEinzelBlaetter()
{
    EinzelBlaetter=false;
}

void Diagramm::disableHilfsLinien()
{
    zHilfsLinien=false;
}

void Diagramm::enableEinzelBlaetter()
{
    EinzelBlaetter=true;
}

void Diagramm::enableHilfsLinien()
{
    zHilfsLinien=true;
}

QPainterPath Diagramm::LinieEinzelBlaetter(Kurs *Element,double Schritt,int Hoehe)
{
    QPainterPath Linie(QPointF(50+Schritt,25+Hoehe*(1-Element->getVerhalt(0))));//Setzt den Startpunkt.
    for (int i=1;i<Element->anzBlaetter();++i)
    {
        float Verhalt=Element->getBlattErreicht(i)/(double)Element->getBlattMax(i);
        Linie.lineTo(QPointF((i+1)*Schritt+50,25+Hoehe*(1-Verhalt) ));//Setzt die Punkte der Linie
    }
    return Linie;
}

QPainterPath Diagramm::LinieEinzelWoche(Kurs *Element, double Schritt,int Hoehe)
{
    int start=0;
    double Verhalt=-1;
    do
    {
        Verhalt=Element->getWocheVerhalt(start++);
    }
    while(Verhalt==-1);
    QPainterPath Linie(QPointF(50+(start-1)*Schritt,25+Hoehe*(1-Verhalt)));//Setzt den Startpunkt.
    for (int i=start;i<Element->getMaxWoche()+1;++i)
    {
        Verhalt=Element->getWocheVerhalt(i);
        if (Verhalt==-1)
                continue;
        Linie.lineTo(QPointF(i*Schritt+50,25+Hoehe*(1-Verhalt) ));//Setzt die Punkte der Linie
    }
    return Linie;
}

QPainterPath Diagramm::LinieGesamtBlaetter(Kurs *Element, double Schritt,int Hoehe)
{
    QPainterPath Linie(QPointF(50+Schritt,25+Hoehe*(1-Element->getVerhalt(0))));//Setzt den Startpunkt.
    for (int i=1;i<Element->anzBlaetter();++i)
    {
        Linie.lineTo(QPointF((i+1)*Schritt+50,25+Hoehe*(1-Element->getVerhalt(i)) ));//Setzt die Punkte der Linie
    }
    return Linie;
}

QPainterPath Diagramm::LinieGesamtWoche(Kurs *Element, double Schritt,int Hoehe)
{
    int start=0;
    double Verhalt=-1;
    do
    {
        Verhalt=Element->getWocheVerhalt(start++);
    }
    while(Verhalt==-1);
    QPainterPath Linie(QPointF(50+(start-1)*Schritt,25+Hoehe*(1-Verhalt)));//Setzt den Startpunkt.
    for (int i=start;i<Element->getMaxWoche()+1;++i)
    {
        Linie.lineTo(QPointF(i*Schritt+50,25+Hoehe*(1-Element->getWochenVerhalt(i)) ));//Setzt die Punkte der Linie
    }
    return Linie;
}

void Diagramm::setMaase(int Breite, int Hoehe)
{
    if ((Breite>0)&&(Breite<5000))
        this->Breite=Breite;
    if ((Hoehe>0)&&(Hoehe<5000))
        this->Hoehe=Hoehe;
}

void Diagramm::setXAcheBlatt()
{
    Woche=false;
    LinieEinzel=&LinieEinzelBlaetter;
    LinieGesamt=&LinieGesamtBlaetter;
}

void Diagramm::setXAchseWoche()
{
    Woche=true;
    LinieEinzel=&LinieEinzelWoche;
    LinieGesamt=&LinieGesamtWoche;
}

/*!
 * \brief Diagramm::zeichneXAchse zeichet die X-Achse auf die Übergebenen Zeichenfläche mit den Übergebenen Startpunkten.
 * \param x x Kordionate des Startpunkt der x-Achse
 * \param y y Kordionate des Startpunkt der x-Achse
 * \param Lange Länge der x-Achse
 * \param Elemente Anzahl der Unterteilungen
 */
float Diagramm::zeichneXAchse(QGraphicsScene* Zeichnung,int x, int y, int Lange, int Elemente)
{
    Zeichnung->addLine(x,y,x+Lange,y);//Fügt die Linie der X-Achse Hinzu.
    QGraphicsTextItem * Text = new QGraphicsTextItem;//Objekt für die Texte der Legende.
    Text->setPos(x,y+10);
    Text->setPlainText("0");
    Zeichnung->addItem(Text);
    float Weite=0;
    int Anz=anzSchritte(Elemente,Lange,&Weite);
    int EleSchritt=Elemente/Anz;
    for (int i=1;i<=Anz;++i)
    {
        Zeichnung->addLine(i*Weite+x,y+15,i*Weite+x,y-15);
        Text =new QGraphicsTextItem;
        Text->setPlainText(QString("%1").arg(i*EleSchritt));
        Text->setPos(i*Weite+x,y+10);
        Zeichnung->addItem(Text);
    }
    return Weite/EleSchritt;
}

/*!
 * \brief Diagramm::zeichneYAchse
 * \param x x Kordionate des Startpunkt der y-Achse
 * \param y y Kordionate des Startpunkt der y-Achse
 * \param Lange Länge der y-Achse
 */
void Diagramm::zeichneYAchse(QGraphicsScene *Zeichnung,int x, int y, int Lange)
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
