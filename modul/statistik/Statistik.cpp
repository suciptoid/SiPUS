#include <modul/statistik/Statistik.h>

Statistik::Statistik(QObject *parent) :
    QObject(parent)
{
    sekarang = QDate().currentDate().toString("yyyy-MM-dd");
}

int Statistik::getBukuTelat(){

    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE tgl_tempo < '"+sekarang+"' AND kembali = '0'");
    query.next();
    return query.value(0).toInt();

}

int Statistik::getTotalBuku(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getBukuTersedia(){
    query.clear();
    query.exec("SELECT SUM(tersedia) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getBukuKembali(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE kembali = '0'");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getTotalPeminjaman(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getItemBuku(){
    query.clear();
    query.exec("SELECT SUM(stock) FROM tbl_buku");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPinjamHari(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE tgl_pinjam = DATE(NOW())");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPInjamBulan(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE MONTH(tgl_pinjam) = MONTH(NOW()) AND YEAR(tgl_pinjam) = YEAR(NOW())");
    query.next();
    return query.value(0).toInt();
}

int Statistik::getPinjamTahun(){
    query.clear();
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE YEAR(tgl_pinjam) = YEAR(NOW())");
    query.next();
    return query.value(0).toInt();
}
