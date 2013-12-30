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
    Liste=new QStandardItemModel(0,1,this);
    Liste->setHorizontalHeaderItem(0,new QStandardItem(QString("Name")));
    ui->listView->setModel(Liste);
    Auswahl=-1;
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
        Neues->addBlatt(0,0);
        Neues->addBlatt(0,0);
        Neues->addBlatt(0,0);
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
    //const QModelIndex index=ui->listView->selectionModel()->currentIndex();
    ui->tableView->setModel(Kurse[Auswahl]);
}
