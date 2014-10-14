#include "kurs.h"


Kurs::Kurs(QObject *parent, QString Name):QAbstractTableModel(parent)
{
    KursName=Name;
    Farbe=0;
    Rythmus=0;
}

void Kurs::addBlatt(int Max,int Erreicht,int Woche)
{
    int anzAlt=anzBlaetter()-1;
    beginInsertRows(QModelIndex(),anzAlt,anzAlt);
    MaxPunkteList.push_back(Max);
    ErPunkteList.push_back(Erreicht);
    if ((Woche==0)&&(Rythmus>0))
    {
        if (anzAlt==-1)
            Woche=1;
        else
            Woche=WochenList[anzAlt]+Rythmus;
    }
    WochenList.push_back(Woche);
    //QModelIndex topLeft=createIndex(0,0),bottomRight=createIndex(anzBlaetter(),1);
    endInsertRows();
    //emit dataChanged(topLeft,bottomRight);
}

int Kurs::anzBlaetter() const
{
    int a=0,b=0;
    a=MaxPunkteList.size();
    b=ErPunkteList.size();
    if (a!=b)
        return -1;
    return a;
}

int Kurs::getBlattErreicht(int Blatt)
{
    return ErPunkteList[Blatt];
}

int Kurs::getBlattMax(int Blatt)
{
    return MaxPunkteList[Blatt];
}

int Kurs::getBlattWoche(int Blatt)
{
    return WochenList[Blatt];
}

int Kurs::getFarbe()
{
    return Farbe;
}

int Kurs::getGesammtErreichtPunkte() const
{
    int Gesammt=0;
    for(vector<int>::const_iterator iter=ErPunkteList.begin();iter!=ErPunkteList.end();++iter)
    {
        Gesammt+=*iter;
    }
    return Gesammt;
}

int Kurs::getGesammtMaxPunkte() const
{
    int Gesammt=0;
    for(vector<int>::const_iterator iter=MaxPunkteList.begin();iter!=MaxPunkteList.end();++iter)
    {
        Gesammt+=*iter;
    }
    return Gesammt;
}

QString Kurs::getName() const
{
    return KursName;
}

//!Gibt die Kursfarbe Als QColorobjekt zurück.
QColor Kurs::getQColor()
{
    int r,g,b;
    r=(Farbe&0xff0000)>>16;
    g=(Farbe&0x00ff00)>>8;
    b=(Farbe&0x0000ff);
    return QColor(r,g,b);
}

int Kurs::getRythmus()
{
    return Rythmus;
}

//!Liefert das Verhaltnis von Gesamt zu den Erreichten Punkten von Blatto bis Einschließlich Blatt n-1.
double Kurs::getVerhalt(int Blatt)const
{
    ++Blatt;
    if ((Blatt==0)||(Blatt>anzBlaetter()))
        Blatt=anzBlaetter();
    int Max=0,Erreicht=0;
    for (int i=0;i<Blatt;++i)
    {
        Max+=MaxPunkteList[i];
        Erreicht+=ErPunkteList[i];
    }
    return Erreicht/(double)Max;
}

double Kurs::getVerhaltBlatt(int Blatt) const
{
    return ErPunkteList[Blatt]/(double)MaxPunkteList[Blatt];
}

int Kurs::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}
int Kurs::rowCount(const QModelIndex & /*parent*/)const
{
    return anzBlaetter()+1;
}

QVariant Kurs::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(index.row()==anzBlaetter())
        {
            if (index.column()==0)
            {
                return QString("");
            }
            if(index.column()==1)
            {
                return QString("%1").arg(getGesammtErreichtPunkte());
            }
            if(index.column()==2)
            {
                return QString("%1").arg(getGesammtMaxPunkte());
            }
            return QString("%1 %").arg(getVerhalt(-1)*100);
        }
        if(index.column()==0)
                return QString("%1").arg(WochenList[index.row()]);
        if(index.column()==1)
            return QString("%1").arg(ErPunkteList[index.row()]);
        if(index.column()==2)
            return QString("%1").arg(MaxPunkteList[index.row()]);
        if(index.column()==3)
            return QString("%1 %").arg(getVerhaltBlatt(index.row())*100);
        if(index.column()==4)
            return QString("%1 %").arg(getVerhalt(index.row())*100);
     }
    if (role==Qt::BackgroundColorRole)
    {
        if (index.row()%2==0)
        {
            return QColor(207,207,207);//Färbt jede Zeite Reihe ein.
        }
    }
    return QVariant();
}

QVariant Kurs::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                return QString("Woche");
                case 1:
                    return QString("Erreichte Punkte");
                case 2:
                    return QString("Maximale Punktzahl");
                case 3:
                    return QString("Prozentual Erreicht");
                case 4:
                    return QString("Gesamt Prozent");
            }
        }
        if(orientation==Qt::Vertical)
        {
            if(section==anzBlaetter())
                return QString("Gesammt");
        }
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

bool Kurs::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        if (index.column()==0)
        {
            WochenList[index.row()]=value.toInt();
        }
        if (index.column()==1)
        {
            ErPunkteList[index.row()]=value.toInt();
        }
        else if(index.column()==2)
        {
            MaxPunkteList[index.row()]=value.toInt();
        }
        //emit editCompleted()
    }
    return true;
}
Qt::ItemFlags Kurs::flags(const QModelIndex &index) const
{
    if ((index.column()<3)&&(index.row()<anzBlaetter()))
        return Qt::ItemIsEditable|Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled;
}

void Kurs::setFarbe(int Farbe)
{
    this->Farbe=Farbe&0xffffff;
}

void Kurs::setName(QString Name)
{
    if(!Name.isEmpty())
    {
        this->KursName=Name;
    }
}

void Kurs::setQFarbe(QColor Farbe)
{
    int r,g,b;
    Farbe.getRgb(&r,&g,&b);
    this->Farbe=(r<<16)|(g<<8)|b;
}

void Kurs::setRythmus(int Rythmus)
{
    if (Rythmus<0)
        Rythmus=0;
    this->Rythmus=Rythmus;
}
