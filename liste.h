#ifndef LISTE_H
#define LISTE_H

#include "kurs.h"

class Liste : public QAbstractTableModel
{
public:
    Liste();
    Liste(QObject *parent);
    Kurs *addKurs(QString Name);
    vector<Kurs*>::const_iterator begin();
    void clear();
    int columnCount(const QModelIndex &/*parent*/) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    vector<Kurs*>::const_iterator end();
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Kurs* operator [](int i);
    int rowCount(const QModelIndex &) const;
    int size();
private:
    vector<Kurs*> Kurse;
};

#endif // LISTE_H
