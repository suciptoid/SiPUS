#ifndef PEMINJAMAN_H
#define PEMINJAMAN_H

#include <QtCore>

class Buku;
class QStandardItemModel;

class Peminjaman
{

public:
    Peminjaman();
    ~Peminjaman();
//    QStandardItemModel getDataModel(QString start, QString limit, QObject *parent=0);
    void setModelPeminjaman(QStandardItemModel *theModel);
    void cariPeminjaman(QStandardItemModel *theModel, const QString &kolom = "siswa", const QString &value = "", QString status = "", const QString &hal = "0");
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
