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
    QString getName() const;
    double getVerhalt(int Blatt);
    double getVerhaltBlatt(int Blatt);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QString KursName;
    vector<int>ErPunkte;
    vector<int>MaxPunkte;
};

#endif // KURS_H
