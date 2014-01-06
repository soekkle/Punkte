#include "liste.h"

Liste::Liste()
{
}

Liste::Liste(QObject *parent):QAbstractTableModel(parent)
{
}

Kurs* Liste::addKurs(QString Name)
{
    beginInsertRows(QModelIndex(),size()-1,size()-1);
    Kurs* Neues=new Kurs(this,Name);
    Kurse.push_back(Neues);
    endInsertRows();
    return Neues;
}

void Liste::clear()
{
    beginRemoveRows(QModelIndex(),0,size()-1);
    for(vector<Kurs*>::const_iterator iter=Kurse.begin();iter!=Kurse.end();++iter)
    {
        delete *iter;
    }
    Kurse.erase(Kurse.begin(),Kurse.end());
    endRemoveRows();
}

int Liste::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant Liste::data(const QModelIndex &index, int role) const
{
    if (role==Qt::DisplayRole)
    {
        int max=Kurse.size()+1;
        if (index.row()<max)
        {
            switch(index.column())
            {
                case 0:
                    return Kurse[index.row()]->getName();
                case 1:
                    return QString::number(Kurse[index.row()]->getFarbe(),16);
            }
        }
    }
    if (role==Qt::BackgroundColorRole)
    {
        if (index.column()==1)
        {
            return Kurse[index.row()]->getQColor();
        }
    }

    return QVariant();
}

QVariant Liste::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return QString("Name");
                case 1:
                    return QString("Farbe");
            }
        }
    }
    return QVariant();
}

Kurs* Liste::operator [](int i)
{
    if ((i<-1)||(i>=size()))
        return NULL;
    return Kurse[i];
}

int Liste::rowCount(const QModelIndex &/*parent*/) const
{
    return Kurse.size();
}

int Liste::size()
{
    return Kurse.size();
}
