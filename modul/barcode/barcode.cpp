#include "barcode.h"
#include "ui_barcode.h"
#include <modul/anggota/Anggota.h>
#include <modul/buku/buku.h>
#include <modul/peminjaman/peminjaman.h>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStyle>
#include <QDesktopWidget>
#include <QDate>
#include <QSqlRecord>
#include <QSqlQuery>

Barcode::Barcode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Barcode), peminjam(new Anggota), buku(new Buku), peminjaman(new Peminjaman)
{
    ui->setupUi(this);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,this->size(),qApp->desktop()->availableGeometry()));
    ui->lAnggotaPeminjam->setVisible(false);
    ui->bBarcodeSelesai->setVisible(false);
    ui->bBarcodeTutup->setVisible(false);
    ui->lScanBuku->setVisible(false);

    //Atur Layout Tabel Peminjaman
    modelPinjam = new QStandardItemModel(0,3,this);
    ui->tblPeminjaman->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblPeminjaman->setSortingEnabled(true);
    ui->tblPeminjaman->resizeColumnsToContents();
    ui->tblPeminjaman->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblPeminjaman->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblPeminjaman->horizontalHeader()->setStretchLastSection(true);
    ui->tblPeminjaman->setModel(modelPinjam);
    this->setTblBarcodePinjam();
}

Barcode::~Barcode()
{
    delete ui;
		delete peminjam;
		delete buku;
		delete peminjaman;
}

void Barcode::on_lScanAnggota_returnPressed()
{
//    QSqlQuery queryAnggota;

    peminjam->setId(ui->lScanAnggota->text());
    peminjam->getData("nama");
    if(!peminjam->isError()){
        ui->lAnggotaPeminjam->setVisible(true);
        ui->bBarcodeSelesai->setVisible(true);
        ui->bBarcodeTutup->setVisible(true);
        ui->lAnggotaPeminjam->setText("<b style='color: green'>Peminjam</b> "+peminjam->getData("no_induk")+" : "+peminjam->getData("nama"));
        ui->lScanAnggota->setVisible(false);
        ui->lScanBuku->setVisible(true);
        ui->lScanBuku->setFocus();
        ui->lScanBuku->setFocusPolicy(Qt::StrongFocus);

        this->setWindowTitle("Data Peminjaman "+peminjam->getData("nama"));

        this->refreshData();

        //Cari Data peminjaman yang sudah ada
//        QString anggota = peminjam->getData("no_induk");

//        int counter=0;
//        if(queryAnggota.exec("SELECT buku, tgl_tempo FROM tbl_peminjaman WHERE siswa = \""+anggota+"\" AND kembali=\"0\"")){
//            while(queryAnggota.next()){
//                modelPinjam->setItem(counter,0,new QStandardItem(QString(queryAnggota.value("buku").toString())));
//                buku->setKode(queryAnggota.value("buku").toString());
//                modelPinjam->setItem(counter,1,new QStandardItem(QString(buku->getData("judul"))));
//                modelPinjam->setItem(counter,2,new QStandardItem(QString(queryAnggota.value("tgl_tempo").toString())));

//                counter++;
//            }
//        }else{
//            qDebug()<<"Error Barcode:: queryAnggota :: "+queryAnggota.lastQuery();
//        }
    }else{
        QMessageBox::critical(this,"Tidak Ditemukan","Hasil pencarian nomor anggota : "+ui->lScanAnggota->text()+" tidak ditemukan ("+QString::number(peminjam->getCount())+").\n");
    }
}

void Barcode::on_bBarcodeSelesai_clicked()
{
    //Enable All COmponent
    ui->lAnggotaPeminjam->setVisible(true);
    ui->bBarcodeSelesai->setVisible(true);
    ui->bBarcodeTutup->setVisible(true);
    ui->lScanAnggota->setVisible(true);
    ui->lAnggotaPeminjam->setVisible(false);
    ui->bBarcodeSelesai->setVisible(false);
    ui->bBarcodeTutup->setVisible(false);
    ui->lScanBuku->setVisible(false);
    ui->lScanAnggota->clear();
    ui->lScanAnggota->setFocus();
    ui->lScanAnggota->setFocusPolicy(Qt::StrongFocus);
    statusBar()->clearMessage();

    //Reset All User data
    this->setWindowTitle("Barcode Mode");
    modelPinjam->clear();
    this->setTblBarcodePinjam();
}

void Barcode::on_bBarcodeTutup_clicked()
{
    this->close();
}

void Barcode::setTblBarcodePinjam(){
    modelPinjam->setHorizontalHeaderItem(0, new QStandardItem(QString("Kode Buku")));
    modelPinjam->setHorizontalHeaderItem(1, new QStandardItem(QString("Judul Buku")));
    modelPinjam->setHorizontalHeaderItem(2, new QStandardItem(QString("Jatuh Tempo")));

    ui->tblPeminjaman->setColumnWidth(0, 200);//Kode buku
    ui->tblPeminjaman->setColumnWidth(1, 400);//Judul buku
    ui->tblPeminjaman->setColumnWidth(2, 200);//Tempo

}

void Barcode::on_lScanBuku_returnPressed()
{
    //Scan ISBN Buku
    QString barcode = ui->lScanBuku->text();
    QString noAnggota = peminjam->getData("no_induk");

    if(!barcode.isEmpty()){

        buku->bersihkan();
        buku->setBarcode(barcode);
        QString kodebuku = buku->getData("kd_buku");
        bool pinjam = peminjaman->pinjamBuku(noAnggota,kodebuku);

        if(pinjam){
            this->refreshData();
            ui->lScanBuku->clear();
            statusBar()->showMessage("Peminjaman "+buku->getData("judul")+" Berhasil, Stok tersedia saat ini "+buku->getData("tersedia"),10000);
        }else{
            statusBar()->showMessage("Peminjaman Gagal, Buku tidak tersedia untuk di pinjam",10000);
            ui->lScanBuku->clear();
//            qDebug()<<"Result Buku "+QString::number(buku->getResult());
            if(!buku->getResult()==0){
                if(buku->getData("tersedia")=="0"){
                    statusBar()->showMessage("Peminjaman "+buku->getData("judul")+" Gagal, Stok tersedia "+buku->getData("tersedia"),20000);
                    QMessageBox::warning(this,"Gagal","Buku tidak dapat di pinjam, Stock buku "+buku->getData("tersedia"));
                }else{
                    statusBar()->showMessage("Peminjaman Gagal, Buku tidak ditemukan ",20000);
                    QMessageBox::warning(this,"Gagal","Buku tidak dapat di temukan");
                }
            }else{
                QMessageBox::warning(this,"Gagal","Buku tidak dapat di temukan");
            }
        }

    }else{
        QMessageBox::warning(this,"Gagal","Pencarian barcode harus di isi");
    }


}

void Barcode::refreshData(){
    QSqlQuery queryAnggota;

    peminjam->setId(ui->lScanAnggota->text());
    peminjam->getData("nama");
    if(!peminjam->isError()){
        //Cari Data peminjaman yang sudah ada
        QString anggota = peminjam->getData("no_induk");

        int counter=0;
        if(queryAnggota.exec("SELECT buku, tgl_tempo FROM tbl_peminjaman WHERE siswa = \""+anggota+"\" AND kembali=\"0\"")){
						// index column
						int buku = queryAnggota.record().indexOf("buku");
						int tgl_tempo = queryAnggota.record().indexOf("tgl_tempo");
						
            while(queryAnggota.next()){
                modelPinjam->setItem(counter,0,new QStandardItem(QString(queryAnggota.value(buku).toString())));
                this->buku->setKode(queryAnggota.value(buku).toString());
                modelPinjam->setItem(counter,1,new QStandardItem(QString(this->buku->getData("judul"))));
                QString tglTempo = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryAnggota.value(tgl_tempo).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
                modelPinjam->setItem(counter,2,new QStandardItem(QString(tglTempo)));

                counter++;
            }
        }else{
            qDebug()<<"Error Barcode:: queryAnggota :: "+queryAnggota.lastQuery();
        }
    }else{
        QMessageBox::critical(this,"Tidak Ditemukan","Hasil pencarian nomor anggota : "+ui->lScanAnggota->text()+" tidak ditemukan ("+QString::number(peminjam->getCount())+").\n");
    }
}
