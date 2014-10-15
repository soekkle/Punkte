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
    connect(ui->actionSchlie_en,SIGNAL(triggered()),this,SLOT(close()));
}

DiagrammFenster::~DiagrammFenster()
{
    if (Zeichnung!=NULL)
        delete Zeichnung;
    delete ui;
}

void DiagrammFenster::closeEvent(QCloseEvent *event)
{
    delete Graphik;
    Graphik=NULL;
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

void DiagrammFenster::Zeichnen()
{
    if (Zeichnung==NULL&&(!offen))
        return;
    if (Graphik!=NULL)
        delete Graphik;
    Graphik=Zeichnung->Ausgabe();
    ui->DiagrammView->setScene(Graphik);
    return;
}
