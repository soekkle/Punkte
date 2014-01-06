#include "neuerkurseingabe.h"
#include "ui_neuerkurseingabe.h"

NeuerKursEingabe::NeuerKursEingabe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeuerKursEingabe)
{
    ui->setupUi(this);
    Erfolgreich=false;
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(ButtonFarbe()));
    Farbwahl=Qt::gray;
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

void NeuerKursEingabe::rejected()
{
    Erfolgreich=false;
}

QString NeuerKursEingabe::GetNeuerKurs(QWidget *parent,QColor *Farbe, bool *Ok)
{
    NeuerKursEingabe *Maske=new NeuerKursEingabe(parent);
    Maske->exec();
    *Ok=Maske->Erfolgreich;
    *Farbe=Maske->Farbwahl;
    return Maske->Name;
}
