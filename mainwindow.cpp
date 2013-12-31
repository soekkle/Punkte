#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_Beenden,SIGNAL(triggered()),this,SLOT(slotClose()));
    connect(ui->pushNBlatt,SIGNAL(clicked()),this,SLOT(slotNeuesBlatt()));
    connect(ui->pushNKurs,SIGNAL(clicked()),this,SLOT(slotNeuerKurs()));
    connect(ui->action_Speichern,SIGNAL(triggered()),this,SLOT(slotSpeichern()));
    connect(ui->actionSpeichern_unter,SIGNAL(triggered()),this,SLOT(slotSpeichernunter()));
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

void MainWindow::slotClose()
{
    close();
}

void MainWindow::slotNeuesBlatt()
{
    Kurse[Auswahl]->addBlatt(0,0);
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
        Neues->addBlatt(1,0);
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
