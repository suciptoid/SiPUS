#ifndef KATALOGBUKU_H
#define KATALOGBUKU_H

#include <QString>

class QStandardItemModel;

class KatalogBuku
{
public:
    KatalogBuku();

    //Fungsi
    void cariKatalog(QStandardItemModel *theModel, const QString &kolom, const QString &value, const QString &status);
};

#endif // KATALOGBUKU_H
