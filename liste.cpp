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

vector<Kurs*>::const_iterator Liste::begin()
{
    return Kurse.begin();
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
                return Kurse[index.row()]->getQColor().name();
                    //return QString::number(Kurse[index.row()]->getFarbe(),16);
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

vector<Kurs*>::const_iterator Liste::end()
{
    return Kurse.end();
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

/*!
 * \brief Liste::savecvsfile
 * \param Datei Zeiger Auf Die Datein in die Geschrieben werden soll.
 * \return Ob das Speichern erfolgreich war.
 */
bool Liste::savecvsfile(QFile *Datei)
{
    QTextStream Ausgabe(Datei);//Verbindet die Datei mit einen Stream.
    int Max=this->maxBlatter();//Variable zum Speichern der Maxiamlaanzahl von Blättern.
    for(int i=0;i<this->size();++i)//Abeitet alle Kurse Ab und Schreibt die Kopfzeile der Tabelle
    {
        Ausgabe<<Kurse[i]->getName()<<";"<<Kurse[i]->getFarbe()<<";";
    }
    Ausgabe<<endl;
    for(int i=0;i<Max;++i)//Get die Kanzen Blätter aller Kurse durch.
    {
        for(int ii=0;ii<this->size();++ii)//Schreibt eine Zeile Daten in die Tabelle.
        {
            int anz=Kurse[ii]->anzBlaetter();
            if(i<anz)//Prüft ob Speicherzugriffs Fehler aufterten könnten.
            {
                Ausgabe<<Kurse[ii]->getBlattMax(i)<<";"<<Kurse[ii]->getBlattErreicht(i)<<";";//Schreibt die Daten in die Datei.
            }
            else
                Ausgabe<<";;";//Sonst wird ein Platzhalter verwendet.
        }
        Ausgabe<<endl;
    }
    return true;
}

/*!
 * \brief Liste::savexmlfile
 * \param Datei Zeiger Auf Die Datein in die Geschrieben werden soll.
 * \return ob das Speichern erfolgreich war.
 */
bool Liste::savexmlfile(QFile *Datei)
{
    QXmlStreamWriter Ausgabe(Datei);//Erzeugt Ein QXLMStream.
    Ausgabe.writeStartDocument ();//Schreibt den Kopf des Dokumentes.
    Ausgabe.writeStartElement("Kurse");//Erstellt das Wurzelelement des Dokumenntes.
    for (vector<Kurs*>::const_iterator iter=begin();iter!=end();++iter)
    {
        Kurs* Element=*iter;
        Ausgabe.writeStartElement("Kurs");//Beginnt das Element des aktuellen Kurses.
        Ausgabe.writeTextElement("Name",Element->getName());//Schreibt den Namen des Aktuellen Kurses.
        Ausgabe.writeTextElement("Farbe",QString::number(Element->getFarbe()));//Schreibt die Farbe des Aktuellen Kurses.
        for(int i=0;i<Element->anzBlaetter();i++)
        {
            Ausgabe.writeStartElement("Blatt");//Öffnet das Element für das aktuelle Blatt.
            Ausgabe.writeTextElement("MaxPunkte",QString::number(Element->getBlattMax(i)));//Schreibt die Maximal Erreichbare Punktzahl
            Ausgabe.writeTextElement("ErreichtePunkte",QString::number(Element->getBlattErreicht(i)));//Schreibt die Erreichte Punktzahl
            Ausgabe.writeEndElement();//Schließt das aktuelle Blatt.
        }
        Ausgabe.writeEndElement();//Schließt das element des Aktuellen Kurses.
    }
    Ausgabe.writeEndDocument();//Schließt das Aktuelle Dokument.
    return true;
}

int Liste::size()
{
    return Kurse.size();
}

int Liste::maxBlatter()
{
    int max=0;
    for (vector<Kurs*>::const_iterator iter=begin();iter!=end();++iter)
    {
        Kurs* Element=*iter;
        if (max<Element->anzBlaetter())
            max=(Element->anzBlaetter());
    }
    return max;
}
