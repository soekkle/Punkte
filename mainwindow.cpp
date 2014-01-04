#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_Beenden,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->pushNBlatt,SIGNAL(clicked()),this,SLOT(slotNeuesBlatt()));
    connect(ui->pushNKurs,SIGNAL(clicked()),this,SLOT(slotNeuerKurs()));
    connect(ui->action_Speichern,SIGNAL(triggered()),this,SLOT(slotSpeichern()));
    connect(ui->actionSpeichern_unter,SIGNAL(triggered()),this,SLOT(slotSpeichernunter()));
    connect(ui->actionNeu,SIGNAL(triggered()),this,SLOT(slotNeu()));
    connect(ui->action_ffnen,SIGNAL(triggered()),this,SLOT(slotLaden()));
    connect(ui->action_ber_Punkte,SIGNAL(triggered()),this,SLOT(slotUber()));
    connect(ui->action_ber_Qt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    Liste=new QStandardItemModel(0,1,this);
    Liste->setHorizontalHeaderItem(0,new QStandardItem(QString("Name")));
    ui->listView->setModel(Liste);
    Auswahl=-1;
    SpeicherOrt="";
    connect(ui->listView->selectionModel(),SIGNAL(selectionChanged(QItemSelection ,QItemSelection )),
            this,SLOT(selectionChangedSlot(QItemSelection ,QItemSelection )));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::close()
{
    QMessageBox msgBox;
    msgBox.setText("Das Programm wird Beenden.");
    msgBox.setInformativeText("Wollen Sie vorm Beenden das Dokument Speichern ?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int Aussage=msgBox.exec();
    if (Aussage==QMessageBox::Cancel)
        return;
    if(Aussage==QMessageBox::Save)
        slotSpeichern();
    QMainWindow::close();
}


void MainWindow::leeren()
{
    for(vector<Kurs*>::const_iterator iter=Kurse.begin();iter!=Kurse.end();++iter)
    {
        delete *iter;
    }
    Kurse.erase(Kurse.begin(),Kurse.end());
    Liste->clear();
    Auswahl=-1;
    SpeicherOrt="";
}
bool MainWindow::laden()
{
    QFile Datei(SpeicherOrt);
    if (!Datei.open(QIODevice::ReadOnly | QIODevice::Text))
             return false;
    QTextStream Eingabe(&Datei);
    //int Max=0;
    QString Zeile=Eingabe.readLine();
    int Pos=Zeile.indexOf(';');
    while(-1<Pos)
    {
        QString Teil=Zeile.left(Pos);
        Kurs *Neu=new Kurs(this,Teil);
        Kurse.push_back(Neu);
        Liste->appendRow(new QStandardItem(Teil));
        Zeile=Zeile.right(Zeile.length()-Pos-1);
        Pos=Zeile.indexOf(';');
        Zeile=Zeile.right(Zeile.length()-Pos-1);
        Pos=Zeile.indexOf(';');
    }
    while(!Eingabe.atEnd())
    {
        Zeile=Eingabe.readLine();
        Pos=Zeile.indexOf(';');
        int i=0;
        do
        {
            QString Teil1=Zeile.left(Pos);
            Zeile=Zeile.right(Zeile.length()-Pos-1);
            Pos=Zeile.indexOf(';');
            QString Teil2=Zeile.left(Pos);
            Zeile=Zeile.right(Zeile.length()-Pos-1);
            Pos=Zeile.indexOf(';');
            Kurse[i]->addBlatt(Teil1.toInt(),Teil2.toInt());
            ++i;
        }
        while (-1<Pos);
    }
    Datei.close();
    return true;
}


void MainWindow::slotLaden()
{
    QString Path=SpeicherOrt;
    if (Path=="")
        Path=QDir::currentPath();
    QString Ort=QFileDialog::getOpenFileName(this,tr("Punkte Laden"),Path,tr("Tabelle(*.csv)"));
    leeren();
    SpeicherOrt=Ort;
    laden();
}

void MainWindow::slotNeu()
{
    leeren();
    return;
}

void MainWindow::slotNeuesBlatt()
{
    if ((Auswahl<0)||(Auswahl>=Kurse.size()))
        return;
    int Max=0,Erreicht=0;
    Max=ui->EMaxPunkte->text().toInt();
    Erreicht=ui->EErPunkte->text().toInt();
    Kurse[Auswahl]->addBlatt(Max,Erreicht);
    ui->tableView->setModel(NULL);
    ui->tableView->setModel(Kurse[Auswahl]);
}

void MainWindow::slotNeuerKurs()
{
    bool OK;
    QString Name=QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                       tr("Kursname:"), QLineEdit::Normal,"", &OK);
    if(OK)
    {
        Kurs* Neues=new Kurs(this,Name);
        Kurse.push_back(Neues);
        QStandardItem *neu=new QStandardItem(Name);
        Liste->appendRow(neu);
        //Neues->addBlatt(1,0);
        ui->tableView->setModel(Neues);
        Auswahl=Kurse.size()-1;
    }
}

void MainWindow::selectionChangedSlot(const QItemSelection& neu  , const QItemSelection & )
{
    QString Index=neu.indexes().first().data().toString();
    for (size_t i=0;i<Kurse.size();++i)
    {
        if(Kurse[i]->getName()==Index)
            Auswahl=i;
    }
    ui->tableView->setModel(Kurse[Auswahl]);
}

void MainWindow::slotSpeichern()
{
    if (SpeicherOrt=="")
        SpeicherOrt=SpeichernDialog();
    Speichern();
}

void MainWindow::slotSpeichernunter()
{
    SpeicherOrt=SpeichernDialog();
    Speichern();
}
QString MainWindow::SpeichernDialog()
{
    return QFileDialog::getSaveFileName(this,tr("Punkte Speichern"),QDir::currentPath(),tr("Tabelle(*.csv)"));
}

bool MainWindow::Speichern()
{
    QFile Datei(SpeicherOrt);
    if (!Datei.open(QIODevice::WriteOnly | QIODevice::Text))
             return false;
    QTextStream Ausgabe(&Datei);
    int Max=0;
    for(size_t i=0;i<Kurse.size();++i)
    {
        int anz=Kurse[i]->anzBlaetter();
        if(Max<anz)
            Max=anz;
        Ausgabe<<Kurse[i]->getName()<<";"<<anz<<";";
    }
    Ausgabe<<endl;
    for(int i=0;i<Max;++i)
    {
        for(size_t ii=0;ii<Kurse.size();++ii)
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
