#include <modul/datapeminjam/datapeminjaman.h>
#include "ui_datapeminjaman.h"
#include <modul/anggota/Anggota.h>
#include <modul/buku/buku.h>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QDebug>

DataPeminjaman::DataPeminjaman(QWidget *parent, const QString &no) :
    QDialog(parent),
    ui(new Ui::DataPeminjaman), buku(new Buku)
{
    //window size
    ui->setupUi(this);
    this->setFixedSize(this->size());


    //Inisialisasi
    peminjam = new Anggota();
    noAnggota = no;

    //List Peminjaman
    model = new QStandardItemModel(0,5,this);
    ui->tblListPinjam->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblListPinjam->setSortingEnabled(true);
    ui->tblListPinjam->resizeColumnsToContents();
    ui->tblListPinjam->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblListPinjam->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblListPinjam->horizontalHeader()->setStretchLastSection(true);
    ui->tblListPinjam->setModel(model);

    //Data Peminjam
    peminjam->setId(no);
    noAnggota = peminjam->getData("no_induk");
    this->setWindowTitle("Buku di pinjam "+peminjam->getData("nama"));
    ui->lPinjamNo->setText(peminjam->getData("no_induk"));
    ui->lPinjamNama->setText(peminjam->getData("nama"));
    ui->lPinjamKelas->setText(peminjam->getData("kelas")+"/"+peminjam->getData("jurusan"));

    this->refreshData();
    this->setTabel();
}

DataPeminjaman::~DataPeminjaman()
{
		delete buku;
    delete ui;
		delete peminjam;
}

void DataPeminjaman::on_bClose_clicked()
{
    this->close();
}

void DataPeminjaman::setNoAnggota(QString no){
    noAnggota = no;
}

void DataPeminjaman::setTabel(){
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Kode Buku")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Judul Buku")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Tanggal Pinjam")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Jatuh Tempo")));

    ui->tblListPinjam->setColumnWidth(0, 1);//Tempo
    ui->tblListPinjam->setColumnWidth(1, 150);//Kode buku
    ui->tblListPinjam->setColumnWidth(2, 400);//Judul buku
    ui->tblListPinjam->setColumnWidth(3, 120);//Tgl Pinjam
    ui->tblListPinjam->setColumnWidth(4, 50);//Tempo

}

void DataPeminjaman::refreshData(){
    QSqlQuery query;
    model->clear();
    this->setTabel();

    int counter = 0;
    if(query.exec(" SELECT * FROM tbl_peminjaman WHERE siswa=\""+noAnggota+"\" AND kembali=\"0\" ")){
		
				// column index
				int id 				 = query.record().indexOf("id");
				int buku 			 = query.record().indexOf("buku");
				int tgl_pinjam = query.record().indexOf("tgl_pinjam");
				int tgl_tempo  = query.record().indexOf("tgl_tempo");
				
        while(query.next()){
            model->setItem(counter,0,new QStandardItem(QString(query.value(id).toString())));
            model->setItem(counter,1,new QStandardItem(QString(query.value(buku).toString())));
            this->buku->setKode(query.value(buku).toString());
            model->setItem(counter,2,new QStandardItem(QString(this->buku->getData("judul"))));
            QString tglPinjam = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(query.value(tgl_pinjam).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            model->setItem(counter,3,new QStandardItem(QString(tglPinjam)));
            QString tglTempo = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(query.value(tgl_tempo).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            model->setItem(counter,4,new QStandardItem(QString(tglTempo)));

            counter++;
        }
    }else{
        qDebug()<<"Error Data Peminjaman:refreshData() -> Query Error : "+query.lastQuery();
    }
}

void DataPeminjaman::on_bKembali_clicked()
{
    QSqlQuery query;
    QString idPinjam = model->data(model->index(ui->tblListPinjam->currentIndex().row(),0)).toString();
    QString kdBuku = model->data(model->index(ui->tblListPinjam->currentIndex().row(),1)).toString();
    QString tglKembali = QDate().currentDate().toString("yyyy-MM-dd");
    if(ui->tblListPinjam->currentIndex().row() > -1){
        if(query.exec("UPDATE tbl_peminjaman SET kembali = \"1\", tgl_kembali=\""+tglKembali+"\" WHERE id=\""+idPinjam+"\" ")){
            buku->bersihkan();
            buku->setKode(kdBuku);
            QString datatersedia = buku->getData("tersedia");
            int tersedia = datatersedia.toInt()+1;
            if(query.exec("UPDATE tbl_buku SET tersedia = \""+QString::number(tersedia)+"\" WHERE kd_buku = \""+kdBuku+"\" ")){
                this->refreshData();
            }else{
                qDebug()<<"DataPeminjaman:kembali Error Query Update buku "+query.lastQuery();
                QMessageBox::warning(this,"Gagal dikembalikan","Gagal Menambah stok buku");
            }
            this->refreshData();
        }else{
            QMessageBox::warning(this,"Gagal dikembalikan","Gagal Menambah data peminjaman");
            qDebug()<<"DataPeminjaman:kembali Error Query Update peminjaman "+query.lastQuery();
        }
    }else{
        QMessageBox::warning(this,"Gagal dikembalikan","Pilih buku terlebih dahulu !!");
    }
}

void DataPeminjaman::on_bKembaliAll_clicked()
{
    QSqlQuery query, queryAll;
    QString tglKembali = QDate().currentDate().toString("yyyy-MM-dd");
    int count=0;
    int countbook=0;

		if(buku != 0) delete buku;
    buku = new Buku();
    buku->bersihkan();

    QMessageBox::StandardButton dialog;
    dialog = QMessageBox::warning(this, "Kembali Semua",
                                  "Anda Yakin Buku Dikembalikan Semua??",
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if( dialog == QMessageBox::Ok) {

        if(query.exec("SELECT * FROM tbl_peminjaman WHERE kembali='0' AND siswa = '"+noAnggota+"'")){
            //qDebug()<<"1. Selec peminjaman belum kembali: "+query.lastQuery();

						// oolumn index
						int buku = query.record().indexOf("buku");
						
            //Update buku satu satu
            while(query.next()){
                //qDebug()<<"2. Loop jumlah buku "+count;
                this->buku->setKode(query.value(buku).toString());
                QString tersedia = QString::number(this->buku->getData("tersedia").toInt()+1);

                if(queryAll.exec("UPDATE tbl_buku SET tersedia = '"+tersedia+"' WHERE kd_buku = '"+query.value(buku).toString()+"' ")){
                    //qDebug()<<"3. Update tbl buku satu persatu dari looping "+countbook;
                    countbook++;
                }else{
                    qDebug()<<"DataPeminjaman:error update buku, pengembalian";
                    countbook--;
                }

                count++;
            }
            //Update tbl_peminjaman
            //Query update set kembali tabel peminjaman
            if(query.exec("UPDATE tbl_peminjaman SET kembali='1', tgl_kembali='"+tglKembali+"' WHERE siswa='"+noAnggota+"' AND kembali='0'")){
                //qDebug()<<"4. Update peminjaman : "+query.lastQuery();
            }else{
                qDebug()<<"DataPeminjaman::kembaliall > Error update tbl peminjaman :'(";
            }
            //qDebug()<<"Dikembalikan : "+QString::number(countbook);
        }
        this->refreshData();

    }
}
