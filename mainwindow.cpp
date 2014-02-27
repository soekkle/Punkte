#include "mainwindow.h"
#include "ui_mainwindow.h"

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
            event->ignore();
    Graphik.close();
    event->accept();
}

//! Setzt alles in den Startzustand zurück
void MainWindow::leeren()
{
    Kurse.clear();//Loscht alle Kurse
    Auswahl=-1;//Setzt die Auswahl auf ein nicht exestirendes Objekt.
    SpeicherOrt="";//Setzt den Speicherort Zurück.
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
    QTextStream Eingabe(&Datei); //Verbindet die Datei mit einen Stream.
    QString Zeile=Eingabe.readLine(); //Liest die Erste Zeile aus.
    int Pos=Zeile.indexOf(';'); //Sucht den ersten Separator
    while(-1<Pos) //Slange es weitere Separatoren gibt.
    {
        QString Teil=Zeile.left(Pos); //Lesen des Namen des Kurses
        Kurs* Neu=Kurse.addKurs(Teil);
        Zeile=Zeile.right(Zeile.length()-Pos-1); //Verkürzen der Zeichenkette
        Pos=Zeile.indexOf(';');//Suche nächstes Trennzeichen
        Teil=Zeile.left(Pos);//Auslesen der Farbe als int
        Neu->setFarbe(Teil.toInt());//Setzen der Fabe des Kurses
        Zeile=Zeile.right(Zeile.length()-Pos-1);//Verkürzen der Zeichenkette
        Pos=Zeile.indexOf(';');
    }
    while(!Eingabe.atEnd())//Solange noch Zeilen in der Datei sind
    {
        Zeile=Eingabe.readLine();//Lesen der nächsten Zeile
        Pos=Zeile.indexOf(';');//Suchen des Trennzeichens
        int i=0;//Zähler auf 0 Inizalisiren.
        do
        {
            QString Teil1=Zeile.left(Pos);//Auslesen der Maximal Punktzahl
            Zeile=Zeile.right(Zeile.length()-Pos-1);
            Pos=Zeile.indexOf(';');
            QString Teil2=Zeile.left(Pos);//Auslesen der Erreichten Punktzahl
            Zeile=Zeile.right(Zeile.length()-Pos-1);
            Pos=Zeile.indexOf(';');
            Kurse[i]->addBlatt(Teil1.toInt(),Teil2.toInt());//Setzen Der Gelesenen Werte in den Entsprechenden Kurs
            ++i;//Erhöhen des Zählers
        }
        while (-1<Pos);
    }
    Datei.close();//Schließt die Datei
    return true;//Gibt zurück das die Datei Erfolgreich gelesen wurde
}

//! Ädert die Farbe des ausgewhlten kurses
void MainWindow::slotFarbe()
{
    if ((Auswahl<0)||(Auswahl>=Kurse.size()))//Prüft ob ein Kurs ausgewählt ist.
        return;
    Kurse[Auswahl]->setQFarbe(QColorDialog::getColor(Kurse[Auswahl]->getQColor(),this,"Kurs Farbe"));//Ruft einen getColor Dialog auf und speichert den zurückgegeben wert in den Kurs.
    Graphik.DatenGeaendert();//Läst das Diagramm neuzeichnen.
}

//! Solt der Das Laden von datein startet.
void MainWindow::slotLaden()
{
    QString Path=SpeicherOrt;//Läht den Zuletzt verwendeten. Pfard.
    if (Path=="")//Prüft ob er Leer ist
        Path=QDir::currentPath();//Stzt in auf den Ausführort des Programms.
    QString Ort=QFileDialog::getOpenFileName(this,tr("Punkte Laden"),Path,tr("Tabelle(*.csv)"));//rüft einen GetFileName dialog auf
    leeren();//Setzt das Programm zurück.
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
    int Max=0,Erreicht=0;//Inizalisirt die Punktzahl mit 0
    Max=ui->EMaxPunkte->text().toInt();//List die Entsprechenden Zahle aus dem Formula aus.
    Erreicht=ui->EErPunkte->text().toInt();
    Kurse[Auswahl]->addBlatt(Max,Erreicht);//Fügt das Blatt mit den Ausgelesenen Werten den Ausgewählten kurs hinzu.
    Graphik.DatenGeaendert();//Aktuallisirt die Graphik
}

void MainWindow::slotNeuerKurs()
{
    bool OK;
    QColor Farbe;
    QString Name=NeuerKursEingabe::GetNeuerKurs(this,&Farbe,&OK);
    if(OK)
    {
        Kurs* Neues=Kurse.addKurs(Name);
        Neues->setQFarbe(Farbe);
        ui->tableView->setModel(Neues);
        Auswahl=Kurse.size()-1;
        Graphik.DatenGeaendert();
    }
}

void MainWindow::selectionChangedSlot(const QItemSelection& neu  , const QItemSelection & )
{
    if (neu.isEmpty())
        return;
    Auswahl=neu.indexes().first().row();
    ui->tableView->setModel(Kurse[Auswahl]);
}

bool MainWindow::slotSpeichern()
{
    if (SpeicherOrt=="")
        SpeicherOrt=SpeichernDialog();
    return Speichern();
}

void MainWindow::slotSpeichernunter()
{
    SpeicherOrt=SpeichernDialog();
    Speichern();
}
QString MainWindow::SpeichernDialog()
{
    QString Datei=QFileDialog::getSaveFileName(this,tr("Punkte Speichern"),QDir::currentPath(),tr("Tabelle(*.csv)"));
    if (Datei.right(4)!=".csv")
        Datei+=".csv";
    return Datei;
}

bool MainWindow::Speichern()
{
    if (SpeicherOrt=="")
        return false;
    QFile Datei(SpeicherOrt);
    if (!Datei.open(QIODevice::WriteOnly | QIODevice::Text))
             return false;
    QTextStream Ausgabe(&Datei);
    int Max=0;
    for(int i=0;i<Kurse.size();++i)
    {
        int anz=Kurse[i]->anzBlaetter();
        if(Max<anz)
            Max=anz;
        Ausgabe<<Kurse[i]->getName()<<";"<<Kurse[i]->getFarbe()<<";";
    }
    Ausgabe<<endl;
    for(int i=0;i<Max;++i)
    {
        for(int ii=0;ii<Kurse.size();++ii)
        {
            int anz=Kurse[ii]->anzBlaetter();
            if(i<anz)
            {
                Ausgabe<<Kurse[ii]->getBlattMax(i)<<";"<<Kurse[ii]->getBlattErreicht(i)<<";";
            }
            else
                Ausgabe<<";;";
        }
        Ausgabe<<endl;
    }
    Datei.close();
    return true;
}

void MainWindow::slotUber()
{
    QMessageBox::about(this,"Punkte","Diese Anwendung steht unter der GPLv3 Lizenz\n(c) 2014 soekkle\nsoekkle@freenet.de");
}
