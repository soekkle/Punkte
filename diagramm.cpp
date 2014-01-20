#include "diagramm.h"
#include "ui_diagramm.h"

Diagramm::Diagramm(QWidget *parent):
    QWidget(parent),
    ui(new Ui::Diagramm)
{
    ui->setupUi(this);
    offen=false;
    Breite=ui->graphicsView->width();
    Hoehe=ui->graphicsView->height();
}

Diagramm::~Diagramm()
{
    delete ui;
}

void Diagramm::DatenGeaendert()
{
    zeichnen();
}

void Diagramm::show()
{
    if (offen)
        return;
    QWidget::show();
    zeichnen();
    offen=true;
}

void Diagramm::zeichnen()
{
    Zeichnung=new QGraphicsScene(this);
    ui->graphicsView->setScene(Zeichnung);
    //Zeichnung->addText("Test");
}
