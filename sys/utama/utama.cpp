#include "utama.h"
#include "ui_utama.h"
#include <sys/databaseconfig/databaseconfig.h>
#include <sys/user/user.h>
#include <modul/peminjaman/peminjaman.h>
#include <sys/databaseconfig/databaseconfig.h>
#include <sys/fileconfig/fileconfig.h>
#include <sys/user/user.h>
#include <sys/loginwindow/loginwindow.h>
#include <QStandardItemModel>
#include <modul/peminjaman/peminjaman.h>
#include <sys/about/about.h>
#include <modul/datapeminjam/datapeminjaman.h>
#include <modul/katalog/KatalogBuku.h>
#include <modul/katalogpinjam/KatalogPinjam.h>
#include <modul/barcode/barcode.h>
#include <modul/editbuku/EditBuku.h>
#include <sys/export/Export.h>
#include <modul/bukutelat/bukutelat.h>
#include <modul/statistik/Statistik.h>
#include <modul/anggota/Anggota.h>
#include <modul/kas/kas.h>
#include <modul/listanggota/listanggota.h>
#include <modul/pustakawan/pustakawan.h>
#include <QScrollBar>
#include <QCloseEvent>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <QLocale>
#include <QSqlQuery>

Utama::Utama(LoginWindow *lg, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Utama), logout(false), loginWindow(lg),barcodeWindow(0),telatWindow(0)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("SiPUS | "+ QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate().currentDate(),"dd MMMM yyyy") );
    ui->tabMain->setCurrentIndex(0);
    halamanPinjam = 0;
    ui->tblKatalog->verticalScrollBar()->sliderPosition();
    connect(ui->tblPeminjaman->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(tblPinjamBottom(int)));

    //Sementara disable qplot sebelum fix dikerjakan
    ui->chart->hide();

    //CONNECT
    connect(ui->tabMain,SIGNAL(currentChanged(int)),this,SLOT(getTableData(int)));
    //connect(ui->tblKatalog,SIGNAL(entered(QModelIndex)),this,SLOT(katEnableButton()));

    //********************************************* KATALOG *********************************************//
    //Class Katalog
    katalog = new KatalogBuku();
    //Buat Model Tabel
    modelKatalog = new QStandardItemModel(2,6, this);

    //Set Value Combo Box
    ui->catKomboKolom->addItems(QStringList()<<"Kd. Buku"<<"Judul"<<"Pengarang"<<"Penerbit");
    ui->catKomboStatus->addItems(QStringList()<<"Semua"<<"Tersedia"<<"Kosong");
    //Setting Tabel
    ui->tblKatalog->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblKatalog->setSortingEnabled(true);
    ui->tblKatalog->setModel(modelKatalog);
    ui->tblKatalog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblKatalog->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblKatalog->horizontalHeader()->setStretchLastSection(true);

    //********************************************* KATALOG *********************************************//

    //********************************************* PEMINJAMAN *********************************************//
    pinjam = new Peminjaman();

    model = new QStandardItemModel(2,7,this);
    //Isi Data Peminjaman
    //pinjam->cariPeminjaman(model,"siswa","","");

    //Atur Layout Tabel Peminjaman
    ui->tblPeminjaman->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tblPeminjaman->setSortingEnabled(true);
    ui->tblPeminjaman->resizeColumnsToContents();
    ui->tblPeminjaman->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tblPeminjaman->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblPeminjaman->horizontalHeader()->setStretchLastSection(true);
    ui->tblPeminjaman->setModel(model);

    setTblPeminjaman();

    //Combo Cari By
    ui->comboCari->addItems(QStringList()<<"No. Induk"<<"Buku");
    //Combo Status
    ui->comboStatus->addItems(QStringList()<<"Semua"<<"Kembali"<<"Belum Kembali");
    //Jumlah Data dan Pagination
    ui->totalDataPinjam->setText(QString::number(pinjam->getJumlahData()));
    //********************************************* PEMINJAMAN *********************************************//

    //Statistik
    this->setStatistik();

    //Init Chart
    this->initChart();
    setAttribute(Qt::WA_DeleteOnClose);

}

void Utama::on_actionKeluar_triggered()
{
    this->close();
}

void Utama::getTableData(int tab){
    //Slot TabChange
    switch (tab) {
    case 0:
        //Welcome
        this->setStatistik();
        break;
    case 1:
        //Katalog
        ui->lKatalogCari->setFocus();
        if(ui->lKatalogCari->text().isEmpty()){
            katalog->cariKatalog(modelKatalog,"kd_buku","","");
            this->setTblKatalog();
        }
        break;
    case 2:
        //Peminjaman
        ui->lCariPinjam->setFocus();
        halamanPinjam = 0;
        if(ui->lCariPinjam->text().isEmpty()){
            model->clear();
            pinjam->cariPeminjaman(model,"siswa","","",QString::number(halamanPinjam));
            this->setTblPeminjaman();
            ui->totalPagePinjam->setText(QString::number(pinjam->getJumlahResult()));
        }
        break;
    }
}

//End
Utama::~Utama()
{
    qDebug() << this << "destroyed!";
    delete pinjam;
    delete katalog;
    delete ui;
}


void Utama::setTblPeminjaman(){

    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Peminjam")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Kode")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Judul")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Tgl. Pinjam")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("Tgl. Tempo")));
    model->setHorizontalHeaderItem(5, new QStandardItem(QString("Tgl. Kembali")));
    model->setHorizontalHeaderItem(6, new QStandardItem(QString("Status")));

    ui->tblPeminjaman->setColumnWidth(0, 100);//peminjam
    ui->tblPeminjaman->setColumnWidth(1, 120);//kode
    ui->tblPeminjaman->setColumnWidth(2, 350);//judul
    ui->tblPeminjaman->setColumnWidth(3, 160);//pinjam
    ui->tblPeminjaman->setColumnWidth(4, 160);//tempo
    ui->tblPeminjaman->setColumnWidth(5, 160);//kembali
    ui->tblPeminjaman->setColumnWidth(6, 200);//status

    ui->totalDataPinjam->setText(QString::number(pinjam->getJumlahData()));
    ui->lBelumKembali->setText(QString::number(pinjam->getBelumKembali()));
}

void Utama::setTblKatalog(){
    modelKatalog->setHorizontalHeaderItem(0, new QStandardItem(QString("Kode Buku")));
    modelKatalog->setHorizontalHeaderItem(1, new QStandardItem(QString("Judul")));
    modelKatalog->setHorizontalHeaderItem(2, new QStandardItem(QString("Pengarang")));
    modelKatalog->setHorizontalHeaderItem(3, new QStandardItem(QString("Penerbit")));
    modelKatalog->setHorizontalHeaderItem(4, new QStandardItem(QString("Stok")));
    modelKatalog->setHorizontalHeaderItem(5, new QStandardItem(QString("Tersedia")));

    ui->tblKatalog->setColumnWidth(0, 200);//Kode Buku
    ui->tblKatalog->setColumnWidth(1, 400);//Judul
    ui->tblKatalog->setColumnWidth(2, 250);//Pengarang
    ui->tblKatalog->setColumnWidth(3, 250);//Penerbit
    ui->tblKatalog->setColumnWidth(4, 50);//Stok
    ui->tblKatalog->setColumnWidth(5, 10);//Tersedia
}


void Utama::on_lCariPinjam_returnPressed()
{
    //Reset Halaman Pinjam
    halamanPinjam = 0;
    ui->currentPagePinjam->setText(QString::number(halamanPinjam+1));
    //Combobox Kolom / Cari berdasar
    QString kolom;
    switch (ui->comboCari->currentIndex()){
    case 0:
        kolom = "siswa";
        break;
    case 1:
        kolom = "buku";
        break;
    }
    //Combobox Status
    QString status;
    switch (ui->comboStatus->currentIndex()) {
    case 0:
        status = "";
        break;
    case 1:
        status = "1";
        break;
    case 2:
        status = "0";
        break;
    default:
        status = "";
    }

    QString value = ui->lCariPinjam->text();
    //Fungsi Cari Data Peminjaman
    pinjam->cariPeminjaman(model,kolom,value,status,QString::number(halamanPinjam));

    if(pinjam->isPeminjamSatu() && ui->comboCari->currentIndex() == 0){
        QSqlQuery chekPinjam;
        chekPinjam.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE kembali = \"0\" AND siswa = \""+ui->lCariPinjam->text()+"\" ");
        chekPinjam.next();
        Anggota *anggota = new Anggota();
        anggota->setId(ui->lCariPinjam->text());
        ui->lblPeminjam->setText(anggota->getData("nama"));
        delete anggota;
        if(chekPinjam.value(0).toInt() != 0){

            dialogPinjam = new DataPeminjaman(this,value);
            dialogPinjam->setAttribute(Qt::WA_DeleteOnClose);
            dialogPinjam->setModal(true);
            dialogPinjam->setFocus();
            dialogPinjam->setFocusPolicy(Qt::StrongFocus);
            dialogPinjam->setNoAnggota(value);
            dialogPinjam->show();

        }
    }else{
        ui->lblPeminjam->clear();
    }

    int jumlahData = pinjam->getJumlahResult();

    ui->totalPagePinjam->setText(QString::number(jumlahData));

    setTblPeminjaman();
}

void Utama::on_actionTentang_Aplikasi_triggered()
{
    //About
    tentang = new About(this);
    tentang->setAttribute(Qt::WA_DeleteOnClose);
    tentang->show();
}

void Utama::on_pushButton_4_clicked()
{
    //GO Search
    on_lCariPinjam_returnPressed();
    ui->lCariPinjam->setFocus();
}

void Utama::on_pushButton_clicked()
{
    //Katalog Search
    QString kolom,status,value;
    value = ui->lKatalogCari->text();
    //Combo KOlom
    switch(ui->catKomboKolom->currentIndex()){
    case 0:
        kolom = "kd_buku";
        break;
    case 1:
        kolom = "judul";
        break;
    case 2:
        kolom = "pengarang";
        break;
    case 3:
        kolom = "penerbit";
        break;
    default:
        kolom = "kd_buku";
    }
    //Kombo Status
    status = QString::number(ui->catKomboStatus->currentIndex());
    katalog->cariKatalog(modelKatalog,kolom,value,status);
    this->setTblKatalog();

}

void Utama::katEnableButton(){
    ui->katBPinjam->setEnabled(true);
    ui->katBHapus->setEnabled(true);
}

void Utama::on_katBPinjam_clicked()
{
    if(ui->tblKatalog->currentIndex().row() >= 0){
        QString kode_buku = modelKatalog->data(modelKatalog->index(ui->tblKatalog->currentIndex().row(),0)).toString();
        katalogPinjam = new KatalogPinjam(kode_buku,this);
        katalogPinjam->setAttribute(Qt::WA_DeleteOnClose);
        katalogPinjam->show();
        katalogPinjam->setModal(true);
    }else{
        QMessageBox::warning(this,"Pilih Buku Dulu","Pilih Buku Terlebih dahulu untuk melakukan peminjaman");
    }
}

void Utama::on_openBarcode_triggered()
{
    if(barcodeWindow == 0)
        barcodeWindow = new Barcode(this);
    barcodeWindow->show();
}

void Utama::closeEvent(QCloseEvent *event){
    if(logout) {
        loginWindow->show();
        QMainWindow::closeEvent(event);
        return;
    }

    QMessageBox::StandardButton dialog;
    dialog = QMessageBox::warning(this, "Keluar aplikasi Libska?",
                                  "Anda Yakin Keluar aplikasi Libska?",
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if( dialog == QMessageBox::Ok) {
        QSettings userSession("Chip-Project","Libska-User");
        userSession.remove("USERID");
        close();
    } else {
        event->ignore();
    }
}

void Utama::on_katBTambah_clicked()
{
    EditBuku *editbuku = new EditBuku(this,"tambah");
    editbuku->setAttribute(Qt::WA_DeleteOnClose);
    editbuku->show();
}

void Utama::on_katBEdit_clicked()
{
    if(ui->tblKatalog->currentIndex().row() >= 0){

        QString edit = modelKatalog->data(modelKatalog->index(ui->tblKatalog->currentIndex().row(),0)).toString();
        EditBuku *editbuku = new EditBuku(this,"edit");
        editbuku->setAttribute(Qt::WA_DeleteOnClose);
        editbuku->setKodeEdit(edit);
        editbuku->show();

    }else{

        QMessageBox::warning(this,"Warning !!!","Pilih buku yang akan di edit terlebih dahulu");

    }
}

void Utama::on_katBHapus_clicked()
{
    if(ui->tblKatalog->currentIndex().row() >= 0){

        QString edit = modelKatalog->data(modelKatalog->index(ui->tblKatalog->currentIndex().row(),0)).toString();
        EditBuku *editbuku = new EditBuku(this,"hapus");
        editbuku->setAttribute(Qt::WA_DeleteOnClose);
        editbuku->setKodeEdit(edit);
        editbuku->show();

    }else{

        QMessageBox::warning(this,"Warning !!!","Pilih buku yang akan di hapus terlebih dahulu");

    }

}

void Utama::on_actionExport_triggered()
{
    Export *exportWindow = new Export();
    exportWindow->setAttribute(Qt::WA_DeleteOnClose);
    exportWindow->show();
}


void Utama::tblPinjamBottom(int pos){
    int endPos = ui->tblPeminjaman->verticalScrollBar()->maximum();
    //qDebug()<<"Pos : "+QString::number(pos);
    if(pos == endPos){
        pinjam->appendDataPinjam(QString::number(halamanPinjam));
        this->setTblPeminjaman();
        ui->totalPagePinjam->setText(QString::number(model->rowCount()));
        if(pinjam->getJumlahResult() > 0){

            ui->currentPagePinjam->setText(QString::number(halamanPinjam+1));
            halamanPinjam++;

        }
        //qDebug()<<"Akhirrr : "+QString::number(pos);
    }

}

void Utama::on_actionBuku_Telat_triggered()
{
    if(telatWindow == 0)
        telatWindow = new BukuTelat(this);
    telatWindow->show();
}

void Utama::setStatistik(){
    Statistik *statistik = new Statistik();
    //Buku
    ui->lBukuTelat->setText(QLocale::system().toString(statistik->getBukuTelat()));
    ui->statBelumKembali->setText(QLocale::system().toString(statistik->getBukuKembali()));
    ui->statItemBuku->setText(QLocale::system().toString(statistik->getItemBuku()));
    ui->statTersedia->setText(QLocale::system().toString(statistik->getBukuTersedia()));
    ui->statTotalBuku->setText(QLocale::system().toString(statistik->getTotalBuku()));
    ui->statPeminjaman->setText(QLocale::system().toString(statistik->getTotalPeminjaman()));

    //Peminjaman
    ui->statPinjamHariIni->setText(QLocale::system().toString(statistik->getPinjamHari()));
    ui->statPinjamBulanIni->setText(QLocale::system().toString(statistik->getPInjamBulan()));
    ui->statPinjamTahunIni->setText(QLocale::system().toString(statistik->getPinjamTahun()));

    delete statistik;
}

void Utama::on_actionKas_triggered()
{
    Kas *kas = new Kas(this);
    kas->setAttribute(Qt::WA_DeleteOnClose);
    kas->showMaximized();
}

void Utama::on_actionDaftar_Anggota_triggered()
{
    ListAnggota *listanggota = new ListAnggota();
    listanggota->setAttribute(Qt::WA_DeleteOnClose);
    listanggota->showMaximized();
}

void Utama::on_actionUser_triggered()
{
    Pustakawan *pustWindow = new Pustakawan(this);
    pustWindow->setAttribute(Qt::WA_DeleteOnClose);
    pustWindow->show();
}

void Utama::initChart(){
    //Set Label
    ui->chart->xAxis->setLabel("Bulan");
    ui->chart->yAxis->setLabel("Peminjaman");
    //Enable legend
    ui->chart->legend->setVisible(true);
    //Set Range
    ui->chart->xAxis->setRange(0,30);
    ui->chart->yAxis->setRange(0,30);
    //Interaction
    ui->chart->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);

    //example
    QVector<double> a(30), z(30);
    for(int i=0;i<30;i++){
        a[i]=i;
        z[i]=i/0.5;

        //qDebug()<<"("<<a[i]<<","<<z[i]<<")";
    }
    QCPBars *bars1 = new QCPBars(ui->chart->xAxis, ui->chart->yAxis);
    ui->chart->addPlottable(bars1);
    bars1->setWidth(9/(double)a.size());
    bars1->setData(a, z);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(10, 140, 70, 160));
}

void Utama::on_actionLogout_triggered()
{
    logout = true;
    close();
}
