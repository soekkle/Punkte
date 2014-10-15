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
//Wird Nach dem OK-drücken ausgeführt.
void NeuerKursEingabe::accept()
{
    Name=ui->lineEdit->text();
    Rythmus=ui->spinBox->value();
    if (Name!="")//Prüft ob ein Kursname eingegeben wurde
        Erfolgreich=true;
    close();//Schließt das aktuelle Dialogfenster
}

void NeuerKursEingabe::ButtonFarbe()
{
    Farbwahl=QColorDialog::getColor(Farbwahl,this,"Kurs Farbe");
}

/*!
 * \brief NeuerKursEingabe::EditKurs Startet einen Dialog der mit den Angegebenen Daten Ausgefüllt ist.
 * \param parent
 * \param Name Name Des zu berarbeiten Kurses
 * \param Farbe Farbe des Kurses
 * \param Rythmus Rythmus des Kurses
 * \return Ob die Daten verändert wurden.
 */
bool NeuerKursEingabe::EditKurs(QWidget *parent, QString *Name, QColor *Farbe, int *Rythmus)
{
    NeuerKursEingabe *Maske=new NeuerKursEingabe(parent);//Erzeugt ein neues Dialog Objekt.
    Maske->setData(*Name,*Farbe,*Rythmus);
    Maske->setWindowTitle("Kurs bearbeiten");
    Maske->exec();
    if (Maske->Erfolgreich)
    {
        *Name=Maske->Name;
        *Farbe=Maske->Farbwahl;
        *Rythmus=Maske->Rythmus;
        delete Maske;
        return true;
    }
    delete Maske;
    return false;
}

//! Zeigt den Dialog zum Erstellren des Neuen Kurses
/*!
 *@param *parent Pointer auf das Eltern Objekt
 *@param *Farbe schreibt in die Adresse die ausgewählte Farbe des Kurses
 *@param *Ok ist true wenn der Dialog mit OK Beendet wurde sonst false
 *@return Name des Neuen Kurses
*/
QString NeuerKursEingabe::GetNeuerKurs(QWidget *parent,QColor *Farbe, bool *Ok,int *Rythmus)
{
    NeuerKursEingabe *Maske=new NeuerKursEingabe(parent);//Erzeugt ein neues Dialog Objekt.
    Maske->exec();//Führ den Dialog aus.
    *Ok=Maske->Erfolgreich;//Schreibt die Ausgewählten werte zurück.
    *Farbe=Maske->Farbwahl;
    *Rythmus=Maske->Rythmus;
    QString Name=Maske->Name;
    delete Maske;
    return Name;
}

//! Gibt bei jeden Aufruf einen ander Farbe wieder
/*!
 *Die Farben Haben einen Feste Reihenfolge. nach einer festen Anzahl von Aufrufen wiederholt sich die Reihenfolge
 *@return eine Farbe
*/
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
        Farbe=Qt::green;//Setzt bei nicht aufgeführten werten alles zurück.
        nextColorint=-1;
    }
    }
    return Farbe;
}
//Wird nach dem Caneld Drücken aufgerufen
void NeuerKursEingabe::rejected()
{
    Erfolgreich=false;//Setzt Alles auf Falses
}

void NeuerKursEingabe::setData(QString Name, QColor Farbe, int Rythmus)
{
    this->Name=Name;
    this->Farbwahl=Farbe;
    this->Rythmus=Rythmus;
    ui->lineEdit->setText(Name);
    ui->spinBox->setValue(Rythmus);
}
