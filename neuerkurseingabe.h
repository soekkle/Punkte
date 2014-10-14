#ifndef NEUERKURSEINGABE_H
#define NEUERKURSEINGABE_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class NeuerKursEingabe;
}

/*!
 *@autor soekkle
 *@date 14.10.14
 *@version 0.1
*/

class NeuerKursEingabe : public QDialog
{
    Q_OBJECT
    
public:
    explicit NeuerKursEingabe(QWidget *parent = 0);
    ~NeuerKursEingabe();
    //Zeigt das Dialogfenster an, Mit den Informationen Einens Kurses.
    static bool EditKurs(QWidget *parent,QString *Name,QColor *Farbe,int *Rythmus);
    static QString GetNeuerKurs(QWidget *parent, QColor *Farbe, bool *Ok, int *Rythmus);

private:

    static QColor nextColor();
    static int nextColorint;
    Ui::NeuerKursEingabe *ui;
    bool Erfolgreich;
    QColor Farbwahl;
    QString Name;
    int Rythmus;
    void setData(QString Name,QColor Farbe,int Rythmus);

private slots:
    void accept();
    void ButtonFarbe();
    void rejected();
};

#endif // NEUERKURSEINGABE_H
