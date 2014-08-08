#include <modul/katalogpinjam/KatalogPinjam.h>
#include "ui_KatalogPinjam.h"
#include <modul/buku/buku.h>
#include <modul/anggota/Anggota.h>
#include <modul/peminjaman/peminjaman.h>
#include <QMessageBox>

KatalogPinjam::KatalogPinjam(QString kode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KatalogPinjam), buku(new Buku), anggota(new Anggota), peminjaman(new Peminjaman)
{
    ui->setupUi(this);

    this->setFixedSize(this->size());

    buku->setKode(kode);

    ui->lKatKode->setText(kode);
    ui->lKatHarga->setText(buku->getData("harga"));
    ui->lKatJudul->setText(buku->getData("judul"));
    this->setWindowTitle(buku->getData("judul"));
    ui->lKatPenerbit->setText(buku->getData("penerbit"));
    ui->lKatPengarang->setText(buku->getData("pengarang"));
    ui->lKatDipinjam->setText(buku->getData("tersedia"));
    if(ui->lKatDipinjam->text().toInt() <= 0){
        ui->lKatDipinjam->setStyleSheet("font-weight: bold; color: red;");
        ui->lKatPinjamPesan->setText("Stock Buku habis !!");
        ui->lKatPinjamPesan->setStyleSheet("font-weight: bold; color: red;");
    }
}

KatalogPinjam::~KatalogPinjam()
{
    delete buku;
    delete anggota;
    delete peminjaman;
    delete ui;
}

void KatalogPinjam::on_katBPinjam_clicked()
{
    bool pinjam = peminjaman->pinjamBuku(anggota->getData("no_induk"),buku->getData("kd_buku"));
    if(pinjam){
        this->accept();
        ui->katBPinjam->setEnabled(false);
    }else{
        if(ui->lKatDipinjam->text().toInt() <= 0){
            QMessageBox::warning(this,"Buku Habis","Stok buku di database habis, mohon periksa kembali");
        }
    }
}

void KatalogPinjam::on_lineEdit_returnPressed()
{
    anggota->setId(ui->lineEdit->text());

    ui->katLKelas->setText(anggota->getData("kelas")+" "+anggota->getData("jurusan"));
    ui->katLPeminjam->setText(anggota->getData("nama"));

    if(!anggota->isError()){
        ui->katBPinjam->setFocus();
        ui->katBPinjam->setDisabled(false);
    }else{
        ui->katBPinjam->setEnabled(false);
        //ui->lineEdit->setFocus();
    }

}
