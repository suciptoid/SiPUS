#ifndef ANGGOTA_H
#define ANGGOTA_H

#include <QString>
#include <QSqlQuery>
#include <QDebug>

class Anggota
{
public:
    Anggota();
    void setId(QString idAnggota);
    QString getData(QString kolom);
    bool isError();
    int getCount();
private:
    QString id;
    bool error;
    int count;
};

#endif // ANGGOTA_H
