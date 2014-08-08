#ifndef ANGGOTA_H
#define ANGGOTA_H

#include <QString>

class Anggota
{
public:
    Anggota();
    void setId(const QString &idAnggota);
    QString getData(const QString &kolom);
    bool isError();
    int getCount();
private:
    QString id;
    bool error;
    int count;
};

#endif // ANGGOTA_H
