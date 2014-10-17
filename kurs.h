#ifndef KURS_H
#define KURS_H

#include <vector>
#include <QString>
#include <QColor>
#include <QAbstractTableModel>

using namespace std;

/*!
 *@autor soekkle
 *@date 13.10.14
 *@version 0.2
*/

class Kurs : public QAbstractTableModel
{
    Q_OBJECT
public:
    Kurs(QObject *parent, QString Name);
    void addBlatt(int Max, int Erreicht, int Woche=0);
    int anzBlaetter() const;
    int getBlattErreicht(int Blatt);
    int getBlattMax(int Blatt);
    int getBlattWoche(int Blatt);
    int getFarbe();
    int getGesammtMaxPunkte() const;
    int getGesammtErreichtPunkte()const;
    int getMaxWoche() const;
    QString getName() const;
    QColor getQColor();
    int getRythmus();
    double getVerhalt(int Blatt) const;
    double getVerhaltBlatt(int Blatt)const;
    double getWochenVerhalt(int Woche) const;
    double getWocheVerhalt(int Woche)const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setFarbe(int Farbe);
    void setName(QString Name);
    void setQFarbe(QColor Farbe);
    void setRythmus(int Rythmus);
private:
    int Farbe;
    QString KursName;
    //! Vector der alle Erreichten Punkte enthält.
    vector<int>ErPunkteList;
    //! Vector der alle Erreichbaren Punkte enthält.
    vector<int>MaxPunkteList;
    //! Speichert den Rythmus der Übungsblätter
    int Rythmus;
    //! Vector der die zu den Blättern gehörige Woche Enthält.
    vector<int>WochenList;
};

#endif // KURS_H
