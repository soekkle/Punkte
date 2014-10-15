#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 *@autor soekkle
 *@date 28.02.14
 *@version 0.1
*/


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_Beenden,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionNeues_Blatt,SIGNAL(triggered()),this,SLOT(slotNeuesBlatt()));
    connect(ui->pushNBlatt,SIGNAL(clicked()),this,SLOT(slotNeuesBlatt()));
    connect(ui->actionNeuer_Kurs,SIGNAL(triggered()),this,SLOT(slotNeuerKurs()));
    connect(ui->pushNKurs,SIGNAL(clicked()),this,SLOT(slotNeuerKurs()));
    connect(ui->action_Speichern,SIGNAL(triggered()),this,SLOT(slotSpeichern()));
    connect(ui->actionSpeichern_unter,SIGNAL(triggered()),this,SLOT(slotSpeichernunter()));
    connect(ui->actionNeu,SIGNAL(triggered()),this,SLOT(slotNeu()));
    connect(ui->action_ffnen,SIGNAL(triggered()),this,SLOT(slotLaden()));
    connect(ui->action_ber_Punkte,SIGNAL(triggered()),this,SLOT(slotUber()));
    connect(ui->action_ber_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui->actionFarbe_W_hlen,SIGNAL(triggered()),this,SLOT(slotFarbe()));
    connect(ui->actionDiagramm_anzeigen,SIGNAL(triggered()),&Graphik,SLOT(show()));
    connect(ui->actionKurs_Bearbeiten,SIGNAL(triggered()),this,SLOT(slotKursBearbeiten()));
    ui->listView->setModel(&Kurse);
    Auswahl=-1;
    SpeicherOrt="";
    connect(ui->listView->selectionModel(),SIGNAL(selectionChanged(QItemSelection ,QItemSelection )),
            this,SLOT(selectionChangedSlot(QItemSelection ,QItemSelection )));
    Graphik.setListe(&Kurse);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Versichert sich beim Anwender ob das Programm geschlossen werden soll.
/*!
 * Nach Rückfrage beim Benutzer wird alles vom Programm ordnungsgemäß geschlossen.
*/
void MainWindow::closeEvent( QCloseEvent *event )
{
    QMessageBox msgBox;//Inizalisiert den Prüfdialog
    msgBox.setText("Das Programm wird Beenden.");
    msgBox.setInformativeText("Wollen Sie vorm Beenden das Dokument Speichern ?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int Aussage=msgBox.exec();//Ruft den Dialog auf.
    if (Aussage==QMessageBox::Cancel)//Nach Cancel wird das Programm weiter Normal Ausgeführt
    {
        event->ignore();
        return;
    }
    if(Aussage==QMessageBox::Save)//Nach Save wird die Eingabe gespeichert.
        if (!slotSpeichern())
        {
            event->ignore();
            return;
        }
    Graphik.close();
    event->accept();
}

//! Setzt alles in den Startzustand zurück
void MainWindow::leeren()
{
    Kurse.clear();//Loscht alle Kurse
    Auswahl=-1;//Setzt die Auswahl auf ein nicht exestirendes Objekt.
    SpeicherOrt="";//Setzt den Speicherort Zurück.
    Dateityp=0;
}
//! Laden von Gesicherten Daten
/*!
 *Daten aus einer csv Datei Auslesen die Das Vormat einhält in dem Gespeichert wurde.
*/
bool MainWindow::laden()
{
    QFile Datei(SpeicherOrt);
    if (!Datei.open(QIODevice::ReadOnly | QIODevice::Text)) //Öfnet die Datei und Prüft ob die Erfolgreich war.
             return false; //Gibt zurück das ein Fehler aufgetreten ist.
    bool Erfolgreich=false;
    if(Dateityp==1)
        Erfolgreich=Kurse.loadcvsfile(&Datei);
    if(Dateityp==2)
        Erfolgreich=Kurse.loadxmlfile(&Datei);
    Datei.close();//Schließt die Datei
    if (!Erfolgreich)
    {
        leeren();
        QMessageBox::warning(this,"Fehler beim Laden",QString("Es ist ein Fehler beim Laden der Datei \"%1\" aufgeterten.\nDie Datei wurde nicht geladen.").arg(SpeicherOrt));
    }
    Graphik.DatenGeaendert();
    return Erfolgreich;//Gibt zurück das die Datei Erfolgreich gelesen wurde
}

void MainWindow::setWoche()
{
    int nr=Kurse[Auswahl]->anzBlaetter()-1;
    int woche=Kurse[Auswahl]->getBlattWoche(nr)+Kurse[Auswahl]->getRythmus();
    ui->spinBox->setValue(woche);
}

//! Ädert die Farbe des ausgewhlten kurses
void MainWindow::slotFarbe()
{
    if ((Auswahl<0)||(Auswahl>=Kurse.size()))//Prüft ob ein Kurs ausgewählt ist.
        return;
    Kurse[Auswahl]->setQFarbe(QColorDialog::getColor(Kurse[Auswahl]->getQColor(),this,"Kurs Farbe"));//Ruft einen getColor Dialog auf und speichert den zurückgegeben wert in den Kurs.
    Graphik.DatenGeaendert();//Läst das Diagramm neuzeichnen.
}

void MainWindow::slotKursBearbeiten()
{
    Kurs *ausgewaehlt=Kurse[Auswahl];
    QString Name=ausgewaehlt->getName();
    QColor Farbe=ausgewaehlt->getQColor();
    int Rythmus=ausgewaehlt->getRythmus();
    if (NeuerKursEingabe::EditKurs(this,&Name,&Farbe,&Rythmus))
    {
        ausgewaehlt->setName(Name);
        ausgewaehlt->setQFarbe(Farbe);
        ausgewaehlt->setRythmus(Rythmus);
    }
    Graphik.DatenGeaendert();
}

//! Solt der Das Laden von datein startet.
void MainWindow::slotLaden()
{
    QString Path=SpeicherOrt,Filter;//Läht den Zuletzt verwendeten. Pfard.
    if (Path=="")//Prüft ob er Leer ist
        Path=QDir::currentPath();//Stzt in auf den Ausführort des Programms.
    QString Ort=QFileDialog::getOpenFileName(this,tr("Punkte Laden"),Path,tr("Tabelle(*.csv);;XML-Datei(*.xml)"),&Filter);//rüft einen GetFileName dialog auf
    leeren();//Setzt das Programm zurück.
    if(Filter=="Tabelle(*.csv)")//Prüft ob Tabelle als Filter gewählt wurde
    {
        Dateityp=1;
    }
    else if(Filter=="XML-Datei(*.xml)")//Prüft ob xml als Filter gewählt wurde
    {
        Dateityp=2;
    }
    SpeicherOrt=Ort;//Setzt den Ort aus dem Geladen wurde.
    laden();//Läd die Datei mit der Entsprechenden Funktion.
}

//! Slot der Den Startzustand des Programms wiederherstellt.
void MainWindow::slotNeu()
{
    leeren();
    return;
}

//! Fugt dem Ausgewählten Kurs ein Blatt hinzu
void MainWindow::slotNeuesBlatt()
{
    if ((Auswahl<0)||(Auswahl>=Kurse.size()))//Prüft ob des ausgewählte Element gültig ist.
        return;
    int Max=0,Erreicht=0,Woche=0;//Inizalisirt die Punktzahl mit 0
    Max=ui->EMaxPunkte->text().toInt();//List die Entsprechenden Zahle aus dem Formula aus.
    Erreicht=ui->EErPunkte->text().toInt();
    Woche=ui->spinBox->value();
    Kurse[Auswahl]->addBlatt(Max,Erreicht,Woche);//Fügt das Blatt mit den Ausgelesenen Werten den Ausgewählten kurs hinzu.
    Graphik.DatenGeaendert();//Aktuallisirt die Graphik
    setWoche();
}

//! Fügt einen neuen Kurs hinzu
void MainWindow::slotNeuerKurs()
{
    bool OK;
    QColor Farbe;//Inizalisirung der Variablen.
    int Rythmus;
    QString Name=NeuerKursEingabe::GetNeuerKurs(this,&Farbe,&OK,&Rythmus);//Aufrufen des Dialoges zum erstellen des Neuen Kurses.
    if(OK)//Rückgabe auswerten
    {
        Kurs* Neues=Kurse.addKurs(Name);//Erzeugen des Neuenkurses und setzen der Übergebenen Eigenschaften.
        Neues->setQFarbe(Farbe);
        Neues->setRythmus(Rythmus);
        ui->tableView->setModel(Neues);
        Auswahl=Kurse.size()-1;
        Graphik.DatenGeaendert();//Diagramm über die Änderung Informiren.
    }
}

//! Verarbeitet das auswählen einen Elementes in der Listview
void MainWindow::selectionChangedSlot(const QItemSelection& neu  , const QItemSelection & )
{
    if (neu.isEmpty())// Prüft ob Tatsechlich ein Element ausgeählt wurde.
        return;
    Auswahl=neu.indexes().first().row();//Speicher die Zeilennummer des Ausgewählten Kurses in einer Klassen Variable.
    ui->tableView->setModel(Kurse[Auswahl]);//Änder in der tableview die Angezeigte Tabelle
    setWoche();
}

//! Leitet das Speichern der Daten ein.
bool MainWindow::slotSpeichern()
{
    if (SpeicherOrt=="")//Prüft ob schon ein Speicherort bekannt ist.
        SpeicherOrt=SpeichernDialog();//Wenn nicht wird einer Abgefragt.
    return Speichern();
}

//! Führt das Speichern unter aus.
void MainWindow::slotSpeichernunter()
{ 
    SpeicherOrt=SpeichernDialog();//Fragt den Speicherort von Benutzer ab.
    Speichern();//Ruft die Funktion zum Speichern auf.
}

//! Verarbeitet den Dialog Speichern unter.
QString MainWindow::SpeichernDialog()
{
    QString Filter,Endung="";
    QString Datei=QFileDialog::getSaveFileName(this,tr("Punkte Speichern"),QDir::currentPath(),tr("Tabelle(*.csv);;XML-Datei(*.xml)"),&Filter);//Öffnet den Dialog Speichern unter.
    if(Filter=="Tabelle(*.csv)")//Prüft ob Tabelle als Filter gewählt wurde
    {
        Dateityp=1;
        Endung=".csv";
    }
    else if(Filter=="XML-Datei(*.xml)")//Prüft ob xml als Filter gewählt wurde
    {
        Dateityp=2;
        Endung=".xml";
    }
    else
        return "";
    if (Datei.right(Endung.length())!=Endung)//Prüft ob die Datei die Rechtige Endung hat, wenn nicht wird sie Angehangen.
        Datei+=Endung;
    return Datei;//Gibt den Dateinamen mit Pfrad zurück.
}

//! Schreibt die Daten auf die Festplatte.
/*!
 * \brief MainWindow::Speichern
 * Als Speicherort sird der in der Klassenvariable Speicherort verwendet.
 * \return Ob Speichern Erfolgreich verlaufen ist.
 */
bool MainWindow::Speichern()
{
    if (SpeicherOrt=="")//Prüft ob überhaubt ein Speicherort angegeben ist.
        return false;
    QFile Datei(SpeicherOrt);//Ersteilt ein dateiobjekt, dass auf die Datei zeigt.
    if (!Datei.open(QIODevice::WriteOnly | QIODevice::Text))// Prüft ob die Datei erfolgreich geöffnet werden kann.
             return false;
    bool Erfolgreich=false;
    if (Dateityp==1)//Prüft den Gewünschten Dateityp Und Speichert entsprechend.
        Erfolgreich=Kurse.savecvsfile(&Datei);
    if (Dateityp==2)
        Erfolgreich=Kurse.savexmlfile(&Datei);
    Datei.close();//Schließt die Datei.
    return Erfolgreich;
}

//! Öffnet einen Dialog der Infos Über das Programm anzeigt.
void MainWindow::slotUber()
{
    QMessageBox::about(this,"Punkte","Diese Anwendung steht unter der GPLv3 Lizenz\n(c) 2014 soekkle\nsoekkle@freenet.de");
}
