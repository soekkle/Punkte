#ifndef NEUERKURSEINGABE_H
#define NEUERKURSEINGABE_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class NeuerKursEingabe;
}

class NeuerKursEingabe : public QDialog
{
    Q_OBJECT
    
public:
    explicit NeuerKursEingabe(QWidget *parent = 0);
    ~NeuerKursEingabe();
    static QString GetNeuerKurs(QWidget *parent, QColor *Farbe, bool *Ok);

private:

    static QColor nextColor();
    static int nextColorint;
    Ui::NeuerKursEingabe *ui;
    bool Erfolgreich;
    QColor Farbwahl;
    QString Name;

private slots:
    void accept();
    void ButtonFarbe();
    void rejected();
};

#endif // NEUERKURSEINGABE_H
