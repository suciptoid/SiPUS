#include <modul/bukutelat/bukutelat.h>
#include "ui_bukutelat.h"

BukuTelat::BukuTelat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BukuTelat)
{
    ui->setupUi(this);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,this->size(),qApp->desktop()->availableGeometry()));
    //Set Default Layout
    modelTelat = new QStandardItemModel(0,4,this);
    ui->tblTelat->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblTelat->setSortingEnabled(true);
    ui->tblTelat->resizeColumnsToContents();
    ui->tblTelat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblTelat->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblTelat->horizontalHeader()->setStretchLastSection(true);
    ui->tblTelat->setModel(modelTelat);
    this->refreshData();
    this->setTabel();

}

BukuTelat::~BukuTelat()
{
    delete ui;
}

void BukuTelat::refreshData(){
    buku = new Buku();
    anggota = new Anggota();

    QSqlQuery query;
    QString sekarang = QDate().currentDate().toString("yyyy-MM-dd");
    if(query.exec("SELECT * FROM tbl_peminjaman WHERE kembali = '0' AND tgl_tempo < '"+sekarang+"'")){
        statusBar()->showMessage("Jumlah Buku Telat : "+QString::number(query.size()));
        int counter=0;
        while(query.next()){
            buku->setKode(query.value("buku").toString());
            anggota->setId(query.value("siswa").toString());

            modelTelat->setItem(counter,0,new QStandardItem(QString(query.value("buku").toString())));
            modelTelat->setItem(counter,1,new QStandardItem(QString(buku->getData("judul"))));
            modelTelat->setItem(counter,2,new QStandardItem(QString(anggota->getData("nama"))));
            QString tglTempo = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(query.value("tgl_tempo").toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            modelTelat->setItem(counter,3,new QStandardItem(QString(tglTempo)));

            counter++;
        }
    }else{
        statusBar()->showMessage("Error Query Data :(");
        qDebug()<<"Error SQL : "+query.lastError().text();
    }
}

void BukuTelat::setTabel(){
    modelTelat->setHorizontalHeaderItem(0, new QStandardItem(QString("Kode Buku")));
    modelTelat->setHorizontalHeaderItem(1, new QStandardItem(QString("Judul Buku")));
    modelTelat->setHorizontalHeaderItem(2, new QStandardItem(QString("Peminjam")));
    modelTelat->setHorizontalHeaderItem(3, new QStandardItem(QString("Jatuh Tempo")));

    ui->tblTelat->setColumnWidth(0, 200);//Kode buku
    ui->tblTelat->setColumnWidth(1, 350);//Judul buku
    ui->tblTelat->setColumnWidth(2, 200);//Tempo
    ui->tblTelat->setColumnWidth(3, 10);//Tempo
}

void BukuTelat::on_bExportCSV_clicked()
{
    QFileDialog *dialog = new QFileDialog();
    QString fileExport = "Daftar_Buku_Telat-"+QDate().currentDate().toString("yyyyMMdd")+".xls";

    QString pathExport = dialog->getExistingDirectory(this, "Pilih Folder Export",QDir::homePath(),QFileDialog::ShowDirsOnly)+"/"+fileExport;
    if(pathExport == "/"+fileExport){
        //Cancel, do nothing
    }else{
        QFile file(pathExport);
        if(file.open(QFile::WriteOnly | QFile::Truncate)){
            QTextStream stream(&file);
            stream << "\"No\"\t\"Kode Buku\"\t\"Judul Buku\"\t\"Peminjam\"\t\"Jatuh Tempo\"\n";
            int total = modelTelat->rowCount()-1;
            for(int x=0; x <= total; x++){
                stream << "\""+QString::number(x+1) << "\"\t\""
                       << modelTelat->data(modelTelat->index(x,0)).toString() << "\"\t\""
                       << modelTelat->data(modelTelat->index(x,1)).toString() << "\"\t\""
                       << modelTelat->data(modelTelat->index(x,2)).toString() << "\"\t\""
                       << modelTelat->data(modelTelat->index(x,3)).toString() << "\"\n";
            }
            file.close();
            QMessageBox::information(this,"Berhasil","File berhasil di export ke: \n"+pathExport);
        }else{
            QMessageBox::critical(this,"Error","File tidak dapat di simpan di lokasi tersebut");
        }
    }

}
