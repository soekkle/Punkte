#ifndef LISTE_H
#define LISTE_H

#include "kurs.h"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDomDocument>

/*!
 *@autor soekkle
 *@date 14.10.14
 *@version 0.2
*/

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
    //! Läd die Daten aus einer cvs-Tabelle
    bool loadcvsfile(QFile *Datei);
    //! Läd die Daten aus einer xml-Tabelle
    bool loadxmlfile(QFile *Datei);
    Kurs* operator [](int i);
    int rowCount(const QModelIndex &) const;
    //! Speichert den Inhalt in einer cvs-Tabelle
    bool savecvsfile(QFile *Datei);
    //! Speichert den Inhalt in einer xml-Tabelle
    bool savexmlfile(QFile *Datei);
    int size();
    //! Liefert die Maximale Anzahl an Blättern zurück.
    int maxBlatter();
    //! Liefet die Größe Benutzte Woche zurück
    int maxWoche();
private:
    vector<Kurs*> Kurse;
};

#endif // LISTE_H
