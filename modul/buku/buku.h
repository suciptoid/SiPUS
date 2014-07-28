#ifndef BUKU_H
#define BUKU_H

#include <QString>

class Buku
{
public:
    Buku();
    void setKode(QString kode);
    void setBarcode(QString barcode);
    QString getData(QString kolom);
    void bersihkan();
    int getResult();
private:
    QString barcode_buku;
    QString kode_buku;
    int querycount;
};

#endif // BUKU_H
