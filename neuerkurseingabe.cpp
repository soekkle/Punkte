#include "neuerkurseingabe.h"
#include "ui_neuerkurseingabe.h"

int NeuerKursEingabe::nextColorint=-1;

NeuerKursEingabe::NeuerKursEingabe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeuerKursEingabe)
{
    ui->setupUi(this);
    Erfolgreich=false;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(ButtonFarbe()));
    Farbwahl=NeuerKursEingabe::nextColor();
}

NeuerKursEingabe::~NeuerKursEingabe()
{
    delete ui;
}

void NeuerKursEingabe::accept()
{
    Name=ui->lineEdit->text();
    if (Name!="")
        Erfolgreich=true;
    close();
}

void NeuerKursEingabe::ButtonFarbe()
{
    Farbwahl=QColorDialog::getColor(Farbwahl,this,"Kurs Farbe");
}

QString NeuerKursEingabe::GetNeuerKurs(QWidget *parent,QColor *Farbe, bool *Ok)
{
    NeuerKursEingabe *Maske=new NeuerKursEingabe(parent);
    Maske->exec();
    *Ok=Maske->Erfolgreich;
    *Farbe=Maske->Farbwahl;
    return Maske->Name;
}

//!Gibt bei jeden Aufruf einen ander Farbe wieder
/*!Die Farben Haben einen Feste Reihenfolge. nach einer festen Anzahl von Aufrufen wiederholt sich die Reihenfolge
@return eine Farbe*/
QColor NeuerKursEingabe::nextColor()
{
    QColor Farbe;
    switch(++nextColorint)
    {
        case 0: Farbe=Qt::red;break;
    case 1: Farbe=Qt::blue;break;
    case 2:Farbe=Qt::yellow;break;
    case 3:Farbe.setRgb(255,85,0);break;
    case 4:Farbe=Qt::magenta;break;
    case 5:Farbe.setRgb(57,255,239);
    default:
    {
        Farbe=Qt::green;
        nextColorint=-1;
    }
    }
    return Farbe;
}

void NeuerKursEingabe::rejected()
{
    Erfolgreich=false;
}
