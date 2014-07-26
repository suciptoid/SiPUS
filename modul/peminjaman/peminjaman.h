#ifndef PEMINJAMAN_H
#define PEMINJAMAN_H

#include <QtCore>
#include <QStandardItemModel>

class Buku;

class Peminjaman
{

public:
    Peminjaman();
		~Peminjaman();
    QStandardItemModel getDataModel(QString start, QString limit, QObject *parent=0);
    void setModelPeminjaman(QStandardItemModel *theModel);
    void cariPeminjaman(QStandardItemModel *theModel, QString kolom, QString value, QString status, QString hal);
    void appendDataPinjam(QString hal);
    int getJumlahData();
    int getJumlahResult();
    int getBelumKembali();
    bool isPeminjamSatu();
    bool pinjamBuku(QString peminjam, QString kdbuku);
private:
    int countPeminjaman;
    int countResult;
    int countSiswa;
    Buku *buku;

    QStandardItemModel *model;
    QString kolomP, valueP, statusP, halP;

};

#endif // PEMINJAMAN_H
