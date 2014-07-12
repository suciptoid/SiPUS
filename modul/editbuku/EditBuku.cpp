#include <modul/editbuku/EditBuku.h>
#include "ui_EditBuku.h"

EditBuku::EditBuku(QWidget *parent, QString mode) :
    QMainWindow(parent),
    ui(new Ui::EditBuku)
{
    ui->setupUi(this);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,this->size(),qApp->desktop()->availableGeometry()));
    this->setFixedSize(this->size());

    ui->lKdBuku->setFocus();
    ui->lKdBuku->setFocusPolicy(Qt::StrongFocus);
    ui->bEditHapus->setVisible(false);
    modeApp = mode;

    if(modeApp=="tambah"){
        ui->bEditSimpan->setVisible(false);
        this->setWindowTitle("Tambah Buku");
        ui->labelTitle->setText("Tambah Buku");
    }else if(modeApp=="edit"){
        ui->bEditTambah->setVisible(false);
        this->setWindowTitle("Edit Buku");
        ui->labelTitle->setText("Edit Buku");
    }else if( modeApp == "hapus" ){
        ui->bEditSimpan->setVisible(false);
        ui->bEditTambah->setVisible(false);
        ui->bEditHapus->setVisible(true);
        this->setWindowTitle("Hapus Buku");
        ui->labelTitle->setText("Hapus Buku");

        //Disable all
        ui->lKdBuku->setEnabled(false);
        ui->lISBN->setEnabled(false);
        ui->lJudul->setEnabled(false);
        ui->lPengarang->setEnabled(false);
        ui->lTahun->setEnabled(false);
        ui->lPenerbit->setEnabled(false);
        ui->lHarga->setEnabled(false);
        ui->lStock->setEnabled(false);
        ui->lTersedia->setEnabled(false);
    }
}

EditBuku::~EditBuku()
{
    delete ui;
}

void EditBuku::on_bEditTambah_clicked()
{
    buku = new Buku();
    buku->setKode(ui->lKdBuku->text());

    if( !ui->lKdBuku->text().isEmpty() ){

        buku->getData("kd_buku");

        if( buku->getResult() > 0 ){
            //Jika sudah ada
            QMessageBox::warning(this,"Error : Sudah Ada", "Buku dengan kode buku tersebut sudah ada");
        }else{
            //Jika kode masih belum digunakan

            QSqlQuery insertBuku;
            QString kdBuku,ISBN,judul,pengarang,tahun,penerbit,harga,stock,tersedia;

            kdBuku = ui->lKdBuku->text();
            ISBN = ui->lISBN->text();
            judul = ui->lJudul->text();
            pengarang = ui->lPengarang->text();
            tahun = ui->lTahun->text();
            penerbit = ui->lPenerbit->text();
            harga = ui->lHarga->text();
            stock = ui->lStock->text();
            tersedia = ui->lTersedia->text();

            if(insertBuku.exec("INSERT INTO tbl_buku SET kd_buku = \""+kdBuku+"\", barcode = \""+ISBN+"\", judul = \""+judul+"\", pengarang = \""+pengarang+"\", thn_terbit = \""+tahun+"\", penerbit = \""+penerbit+"\", harga = \""+harga+"\", stock = \""+stock+"\", tersedia = \""+tersedia+"\" ")){

                QMessageBox::information(this,"Berhasil Ditambahkan", "Buku berhasil ditambahkan");

                ui->lKdBuku->clear();
                ui->lISBN->clear();
                ui->lJudul->clear();
                ui->lPengarang->clear();
                ui->lTahun->clear();
                ui->lPenerbit->clear();
                ui->lHarga->clear();
                ui->lStock->clear();
                ui->lTersedia->clear();
                ui->lKdBuku->setFocus();

            }else{
                QMessageBox::critical(this,"Gagal Ditambahkan", "Terjadi Kesalahan Query SQL :(");
                qDebug()<<"Error Tambah Buku : "+insertBuku.lastQuery();
            }

        }

    }else{

        QMessageBox::warning(this,"Error : Kode Buku", "Kode Buku tidak boleh kosong !!");

    }
}

void EditBuku::on_bEditSimpan_clicked()
{
    buku = new Buku();
    buku->setKode(kodeEdit);

    if( !ui->lKdBuku->text().isEmpty() ){ //Kode buku tidak kosong

        buku->getData("kd_buku");

        if( buku->getResult() != 1 ){
            //Jika lebih satu
            QMessageBox::warning(this,"Error : Lebih dari 1", "Buku dengan kode buku tersebut ada lebih dari satu :(");
        }else{
            //Jika terpenuhi
            QSqlQuery insertBuku;
            QString kdBuku,ISBN,judul,pengarang,tahun,penerbit,harga,stock,tersedia;


            kdBuku = ui->lKdBuku->text();
            ISBN = ui->lISBN->text();
            judul = ui->lJudul->text();
            pengarang = ui->lPengarang->text();
            tahun = ui->lTahun->text();
            penerbit = ui->lPenerbit->text();
            harga = ui->lHarga->text();
            stock = ui->lStock->text();
            tersedia = ui->lTersedia->text();

            if(insertBuku.exec("UPDATE tbl_buku SET kd_buku = \""+kdBuku+"\", barcode = \""+ISBN+"\", judul = \""+judul+"\", pengarang = \""+pengarang+"\", thn_terbit = \""+tahun+"\", penerbit = \""+penerbit+"\", harga = \""+harga+"\", stock = \""+stock+"\", tersedia = \""+tersedia+"\" WHERE kd_buku =\""+kodeEdit+"\"")){

                QMessageBox::information(this,"Berhasil Disimpan", "Data Buku berhasil diperbaruhi");

            }else{
                QMessageBox::critical(this,"Gagal Ditambahkan", "Terjadi Kesalahan Query SQL :(");
                qDebug()<<"Error Update Buku : "+insertBuku.lastQuery();
            }

        }

    }else{

        QMessageBox::warning(this,"Error : Kode Buku", "Kode Buku tidak boleh kosong !!");

    }
}

void EditBuku::setKodeEdit(QString kode){
    kodeEdit = kode;

    buku = new Buku();
    buku->setKode(kodeEdit);

    ui->lKdBuku->setText(buku->getData("kd_buku"));
    ui->lISBN->setText(buku->getData("barcode"));
    ui->lJudul->setText(buku->getData("judul"));
    ui->lPengarang->setText(buku->getData("pengarang"));
    ui->lTahun->setText(buku->getData("thn_terbit"));
    ui->lPenerbit->setText(buku->getData("penerbit"));
    ui->lHarga->setText(buku->getData("harga"));
    ui->lStock->setText(buku->getData("stock"));
    ui->lTersedia->setText(buku->getData("tersedia"));

}

void EditBuku::on_bEditHapus_clicked()
{
    buku = new Buku();
    buku->setKode(kodeEdit);

    if( !ui->lKdBuku->text().isEmpty() ){ //Kode buku tidak kosong

        buku->getData("kd_buku");

        if( buku->getResult() != 1 ){
            //Jika lebih satu
            QMessageBox::warning(this,"Error : Lebih dari 1", "Buku dengan kode buku tersebut ada lebih dari satu :(");
        }else{
            //Jika terpenuhi
            QSqlQuery deletetBuku, chekBuku;

            if(chekBuku.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE buku = \""+buku->getData("kd_buku")+"\" AND kembali=\"0\" ")){
                chekBuku.next();
                if( chekBuku.value(0).toInt() == 0 ){//Chek buku di peminjaman dulu

                    if(deletetBuku.exec("DELETE FROM tbl_buku WHERE kd_buku = \""+buku->getData("kd_buku")+"\"")){

                        QMessageBox::information(this,"Berhasil Dihapus", "Data Buku berhasil dihapus dari sistem");

                    }else{
                        QMessageBox::critical(this,"Gagal Ditambahkan", "Terjadi Kesalahan Query SQL :(");
                        qDebug()<<"Error Update Buku : "+deletetBuku.lastQuery();
                    }

                }else{
                    QMessageBox::critical(this,"Gagal Dihapus", "Ada data peminjaman dengan buku terkait yang belum di kembalikan");
                }
            }else{
                QMessageBox::critical(this,"Gagal Dihapus", "Gagal Query Hapus (SQL)");
            }
        }

    }else{

        QMessageBox::warning(this,"Error : Kode Buku", "Kode Buku tidak boleh kosong !!");

    }
}
