#include <modul/buku/buku.h>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

Buku::Buku()
{
    querycount = 0;
}

void Buku::setBarcode(QString barcode){
    barcode_buku = barcode;
    kode_buku.clear();
}

void Buku::setKode(QString kode){
    kode_buku = kode;
    barcode_buku.clear();
}

QString Buku::getData(QString kolom){
    QSqlQuery queryBuku;
    QString where, ret;
    if(!kode_buku.isEmpty()){
        queryBuku.prepare("SELECT * FROM tbl_buku WHERE kd_buku = ?");
        queryBuku.bindValue(0, kode_buku);
    }else if(!barcode_buku.isEmpty()){
        where = "barcode = \""+barcode_buku+"\" ";
        queryBuku.prepare("SELECT * FROM tbl_buku WHERE barcode = ?");
        queryBuku.bindValue(0, barcode_buku);
    }
    if(queryBuku.exec()){
        queryBuku.next();
//        qDebug()<<"Buku::getData Where class buku : "+where;
//        qDebug()<<"Query buku class buku : "+queryBuku.lastQuery();
        if(queryBuku.size() == 1){
            ret = queryBuku.value(queryBuku.record().indexOf(kolom)).toString();
        }else{
            ret = "";
        }
//        qDebug()<<"Buku::getData Querycount "+QString::number(queryBuku.size());
//        qDebug()<<"Value "+queryBuku.value(0).toString();
        querycount = queryBuku.size();
    }else{
        qDebug() << "Buku::getData Error Select buku :("+queryBuku.lastError().text()+"\n\n"+queryBuku.lastQuery();
    }

    return ret;
}

void Buku::bersihkan(){
    barcode_buku.clear();
    kode_buku.clear();
}

int Buku::getResult(){
    return querycount;
}
