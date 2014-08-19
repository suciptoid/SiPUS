#include <modul/listanggota/listanggota.h>
#include "ui_listanggota.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
//#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QIntValidator>

ListAnggota::ListAnggota(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListAnggota)
{
    ui->setupUi(this);
    //atur model tabel anggota
    modelAnggota = new QSqlTableModel(this);
    modelAnggota->setTable("tbl_anggota");
    modelAnggota->select();
    modelAnggota->setHeaderData(0, Qt::Horizontal, "No Anggota");
    modelAnggota->setHeaderData(1, Qt::Horizontal, "Nama");
    modelAnggota->setHeaderData(2, Qt::Horizontal, "Kelas");
    modelAnggota->setHeaderData(3, Qt::Horizontal, "Jurusan");
    modelAnggota->setHeaderData(4, Qt::Horizontal, "Count Pinjam");


    ui->tblAnggota->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblAnggota->setSortingEnabled(true);
    ui->tblAnggota->resizeColumnsToContents();
    ui->tblAnggota->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblAnggota->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblAnggota->horizontalHeader()->setStretchLastSection(true);
    ui->tblAnggota->setModel(modelAnggota);
    ui->tblAnggota->setColumnHidden(5, true);

    QIntValidator *intValidator = new QIntValidator(this);
    intValidator->setRange(1, INT_MAX);
    ui->lAgtNo->setValidator(intValidator);

}

ListAnggota::~ListAnggota()
{
    qDebug() << this << "destroyed!";
    delete ui;
}

void ListAnggota::on_btnAgtBtl_clicked()
{
    this->close();
}

void ListAnggota::on_bAgtSimpan_clicked()
{
    if(ui->lAgtNo->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Tambah Anggota", "No Anggota Tidak boleh kosong!");
        return;
    }

    if(ui->lAgtNama->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Tambah Anggota", "Nama tidak boleh kosong!");
        return;
    }

    if(ui->lAgtKelas->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Tambah Anggota", "Kelas tidak boleh kosong!");
        return;
    }

    if(ui->lAgtJurusan->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Tambah Anggota", "Jurusan tidak boleh kosong!");
        return;
    }

    QSqlRecord record = modelAnggota->record();
    record.setValue("no_induk", ui->lAgtNo->text().toInt());
    record.setValue("nama", ui->lAgtNama->text());
    record.setValue("kelas", ui->lAgtKelas->text());
    record.setValue("jurusan", ui->lAgtJurusan->text());
    record.setValue("count", 0);
    record.setValue("denda", 0);

    if(modelAnggota->insertRecord(-1, record) == 0) {
        if(modelAnggota->lastError().number() == 1062)
            QMessageBox::critical(this, "Duplikat", "Nomor tersebut sudah digunakan oleh anggota lain.");
        else
            QMessageBox::critical(this, "Error QUery", modelAnggota->lastError().text());
    }
    else {
        QMessageBox::information(this, "Tambah Anggota", "Anggota baru berhasil ditambahkan.");
    }
}


void ListAnggota::on_btnCari_clicked()
{
    // todo
}
