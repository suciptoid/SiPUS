#include <modul/listanggota/listanggota.h>
#include "ui_listanggota.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

ListAnggota::ListAnggota(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListAnggota)
{
    ui->setupUi(this);
    //atur model tabel anggota
    modelanggota = new QStandardItemModel(0,5,this);
    ui->tblAnggota->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblAnggota->setSortingEnabled(true);
    ui->tblAnggota->resizeColumnsToContents();
    ui->tblAnggota->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblAnggota->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblAnggota->horizontalHeader()->setStretchLastSection(true);
    ui->tblAnggota->setModel(modelanggota);
    this->setTabelAnggota();
    this->refreshData();

}

ListAnggota::~ListAnggota()
{
    delete ui;
}

void ListAnggota::on_btnAgtBtl_clicked()
{
    this->close();
}

void ListAnggota::on_bAgtSimpan_clicked()
{
		QSqlQuery query;
    noAnggota = ui->lAgtNo->text();
    namaAnggota = ui->lAgtNama->text().toUpper();
    kelasAnggota = ui->lAgtKelas->text();
    jurusanAnggota = ui->lAgtJurusan->text();

    if(query.exec("INSERT INTO tbl_anggota SET no_induk = \""+noAnggota+"\", nama = \""+namaAnggota+"\", kelas = \""+kelasAnggota+"\", jurusan = \""+jurusanAnggota+"\" ")){
        QMessageBox::information(this,"Berhasil",namaAnggota+" Berhasil Ditambahkan");

        ui->lAgtNo->clear();
        ui->lAgtJurusan->clear();
        ui->lAgtKelas->clear();
        ui->lAgtNama->clear();
        ui->lAgtNo->setFocus();
    }else{
        if(query.lastError().number() == 1062 ){ //if error duplicate entry
            QMessageBox::critical(this,"Duplikat !!","Nomor tersebut sudah digunakan oleh anggota lain.");
            ui->lAgtNo->setFocus();
        }
        qDebug()<<"Error Insert Anggota : "<<QString::number(query.lastError().number())<<query.lastError().text();
    }
}

void ListAnggota::setTabelAnggota(){
    modelanggota->setHorizontalHeaderItem(0, new QStandardItem(QString("No Anggota")));
    modelanggota->setHorizontalHeaderItem(1, new QStandardItem(QString("Nama")));
    modelanggota->setHorizontalHeaderItem(2, new QStandardItem(QString("Kelas")));
    modelanggota->setHorizontalHeaderItem(3, new QStandardItem(QString("Jurusan")));
    modelanggota->setHorizontalHeaderItem(4, new QStandardItem(QString("Count Pinjam")));

    ui->tblAnggota->setColumnWidth(0, 200);//No
    ui->tblAnggota->setColumnWidth(1, 400);//Nama
    ui->tblAnggota->setColumnWidth(2, 200);//Kelas
    ui->tblAnggota->setColumnWidth(3, 300);//Jurusan
    ui->tblAnggota->setColumnWidth(4, 1);//Count
}

void ListAnggota::refreshData(const QString &key){
    QSqlQuery query;
    QString sql = "SELECT * FROM tbl_anggota WHERE no_induk LIKE \"%"+key+"%\" OR nama LIKE \"%"+key+"%\"";
    int count=0;
    modelanggota->clear();
    this->setTabelAnggota();
    if(query.exec(sql)){
        // index column
        int no_induk = query.record().indexOf("no_induk");
        int nama 		 = query.record().indexOf("nama");
        int kelas 	 = query.record().indexOf("kelas");
        int jurusan  = query.record().indexOf("jurusan");
        int count_ 	 = query.record().indexOf("count");

        while(query.next()){
            modelanggota->setItem(count,0,new QStandardItem(QString(query.value(no_induk).toString())));
            modelanggota->setItem(count,1,new QStandardItem(QString(query.value(nama).toString())));
            modelanggota->setItem(count,2,new QStandardItem(QString(query.value(kelas).toString())));
            modelanggota->setItem(count,3,new QStandardItem(QString(query.value(jurusan).toString())));
            modelanggota->setItem(count,4,new QStandardItem(QString(query.value(count_).toString())));
            count++;
        }
    }
}

void ListAnggota::on_btnCari_clicked()
{
    this->refreshData(ui->cariAnggota->text());
}
