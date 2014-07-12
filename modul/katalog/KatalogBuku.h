#ifndef KATALOGBUKU_H
#define KATALOGBUKU_H

#include <QStandardItemModel>
#include <QSqlQuery>
#include <QDebug>
#include <QStatusBar>

class KatalogBuku
{
public:
    KatalogBuku();

    //Fungsi
    void cariKatalog(QStandardItemModel *theModel, QString kolom, QString value, QString status);
};

#endif // KATALOGBUKU_H
