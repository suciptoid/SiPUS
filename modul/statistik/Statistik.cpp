#include <modul/statistik/Statistik.h>
#include <QSqlQuery>
#include <QDate>
#include <QVariant>

Statistik::Statistik(QObject *parent) :
    QObject(parent)
{

}

int Statistik::getBukuTelat(){
    QSqlQuery query;
    QString sekarang = QDate().currentDate().toString("yyyy-MM-dd");
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE tgl_tempo < '"+sekarang+"' AND kembali = '0'");
    query.next();
    return query.value(0).toInt();

}

int Statistik::getTotalBuku(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getBukuTersedia(){
    QSqlQuery query;
    query.exec("SELECT SUM(tersedia) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getBukuKembali(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE kembali = '0'");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getTotalPeminjaman(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getItemBuku(){
    QSqlQuery query;
    query.exec("SELECT SUM(stock) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPinjamHari(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE tgl_pinjam = DATE(NOW())");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPInjamBulan(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE MONTH(tgl_pinjam) = MONTH(NOW()) AND YEAR(tgl_pinjam) = YEAR(NOW())");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPinjamTahun(){
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE YEAR(tgl_pinjam) = YEAR(NOW())");
    query.next();
    return query.value(0).toInt();
}
