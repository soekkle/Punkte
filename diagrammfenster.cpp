#include "diagrammfenster.h"
#include "ui_diagrammfenster.h"

DiagrammFenster::DiagrammFenster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiagrammFenster)
{
    ui->setupUi(this);
    offen=false;
    Zeichnung=NULL;
    Graphik=NULL;
    connect(ui->actionHilfs_Lienien_anzeigen,SIGNAL(triggered(bool)),this,SLOT(hilfslinien(bool)));
    connect(ui->actionEinzelBl_tter,SIGNAL(triggered(bool)),this,SLOT(einzelblaetter(bool)));
    connect(ui->actionWoche,SIGNAL(triggered(bool)),this,SLOT(WocheBlaetter(bool)));
}

DiagrammFenster::~DiagrammFenster()
{
    delete ui;
}

void DiagrammFenster::closeEvent(QCloseEvent *event)
{
    delete Zeichnung;
    offen=false;//Setzen, dass das geschlossen ist.
    event->accept();
    return;
}

void DiagrammFenster::DatenGeaendert()
{
    if (offen)
        Zeichnen();
    return;
}

void DiagrammFenster::einzelblaetter(bool tiggered)
{
    if (tiggered)
        Zeichnung->enableEinzelBlaetter();
    else
        Zeichnung->disableEinzelBlaetter();
    Zeichnen();
    return;
}

void DiagrammFenster::hilfslinien(bool tiggered)
{
    if (tiggered)
        Zeichnung->enableHilfsLinien();
    else
        Zeichnung->disableHilfsLinien();
    Zeichnen();
    return;
}

void DiagrammFenster::resizeEvent(QResizeEvent *event)
{
    if (offen)
    {
        Zeichnung->setMaase(ui->DiagrammView->width()-15,ui->DiagrammView->height()-15);
        Zeichnen();
    }
    return;
}

void DiagrammFenster::setListe(Liste *newListe)
{
    if (Zeichnung!=NULL)
        delete Zeichnung;
    Zeichnung=new Diagramm(newListe,this);
}

void DiagrammFenster::show()
{

    if ((offen)||(Zeichnung==NULL))
        return;
    QMainWindow::show();
    offen=true;
    Zeichnung->setMaase(ui->DiagrammView->width()-15,ui->DiagrammView->height()-15);
    Zeichnen();
    return;
}

void DiagrammFenster::WocheBlaetter(bool tiggered){
    if (tiggered)
        Zeichnung->setXAchseWoche();
    else
        Zeichnung->setXAcheBlatt();
    Zeichnen();
}

void DiagrammFenster::Zeichnen()
{
    if (Zeichnung==NULL&&(!offen))
        return;
    delete Graphik;
    Graphik=Zeichnung->Ausgabe();
    ui->DiagrammView->setScene(Graphik);
    return;
}
