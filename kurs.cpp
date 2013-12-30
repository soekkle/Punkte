#include "kurs.h"

Kurs::Kurs(QObject *parent, QString Name):QAbstractTableModel(parent)
{
    KursName=Name;
}

void Kurs::addBlatt(int Max,int Erreicht)
{
    MaxPunkte.push_back(Max);
    ErPunkte.push_back(Erreicht);
}

int Kurs::anzBlaetter() const
{
    int a=0,b=0;
    a=MaxPunkte.size();
    b=ErPunkte.size();
    if (a!=b)
        return -1;
    return a;
}

QString Kurs::getName() const
{
    return KursName;
}

double Kurs::getVerhalt(int Blatt)
{
    if ((Blatt==-1)||(Blatt>anzBlaetter()))
        Blatt=anzBlaetter();
    int Max=0,Erreicht=0;
    for (int i=0;i<Blatt;++i)
    {
        Max+=MaxPunkte[i];
        Erreicht+=ErPunkte[i];
    }
    return Erreicht/(double)Max;
}

int Kurs::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}
int Kurs::rowCount(const QModelIndex & /*parent*/)const
{
    return anzBlaetter();
}

QVariant Kurs::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(index.column()==0)
            return QString("%1").arg(ErPunkte[index.column()]);
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    }
    return QVariant();
}

QVariant Kurs::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    if(orientation==Qt::Horizontal)
    {
        switch(section)
        {
            case 0:
                return QString("Erreichte Punkte");
             case 1:
                return QString("Maximale Punktzahl");
        }
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}
