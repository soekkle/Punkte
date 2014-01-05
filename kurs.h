#ifndef KURS_H
#define KURS_H

#include <vector>
#include <QString>
#include <QAbstractTableModel>

using namespace std;

class Kurs : public QAbstractTableModel
{
    Q_OBJECT
public:
    Kurs(QObject *parent, QString Name);
    void addBlatt(int Max,int Erreicht);
    int anzBlaetter() const;
    int getBlattErreicht(int Blatt);
    int getBlattMax(int Blatt);
    int getFarbe();
    int getGesammtMaxPunkte() const;
    int getGesammtErreichtPunkte()const;
    QString getName() const;
    double getVerhalt(int Blatt) const;
    double getVerhaltBlatt(int Blatt)const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setFarbe(int Farbe);
private:
    QString KursName;
    vector<int>ErPunkte;
    vector<int>MaxPunkte;
    int Farbe;
};

#endif // KURS_H
