#include <modul/peminjaman/peminjaman.h>
#include <modul/buku/buku.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QSettings>
#include <QStandardItemModel>

Peminjaman::Peminjaman()
{
    buku = new Buku();
}

Peminjaman::~Peminjaman()
{
    delete buku;
}


//Gak Digunakan
void Peminjaman::setModelPeminjaman(QStandardItemModel *theModel){
    QSqlQuery queryPinjam;
    QSqlQuery countData;

    if(queryPinjam.exec("SELECT * FROM tbl_peminjaman LIMIT 0, 500")){

        theModel->clear();

        int counterPinjam=0;
				
        // column index
        int siswa 			= queryPinjam.record().indexOf("siswa");
        int buku 			  = queryPinjam.record().indexOf("buku");
        int tgl_pinjam  = queryPinjam.record().indexOf("tgl_pinjam");
        int tgl_tempo   = queryPinjam.record().indexOf("tgl_tempo");
        int tgl_kembali = queryPinjam.record().indexOf("tgl_kembali");
        int kembali 		= queryPinjam.record().indexOf("kembali");
				
        while(queryPinjam.next()){
            theModel->setItem(counterPinjam,0,new QStandardItem(QString(queryPinjam.value(siswa).toString())));
            theModel->setItem(counterPinjam,1,new QStandardItem(QString(queryPinjam.value(buku).toString())));
                this->buku->setKode(queryPinjam.value(buku).toString());
                theModel->setItem(counterPinjam,2,new QStandardItem(QString( this->buku->getData("judul") )));
            theModel->setItem(counterPinjam,3,new QStandardItem(QString(queryPinjam.value(tgl_pinjam).toString())));
            theModel->setItem(counterPinjam,4,new QStandardItem(QString(queryPinjam.value(tgl_tempo).toString())));
            theModel->setItem(counterPinjam,5,new QStandardItem(QString(queryPinjam.value(tgl_kembali).toString())));
                QString status;
                if(queryPinjam.value(kembali).toString() == "1"){
                    status = "Sudah Kembali";
                }else if(queryPinjam.value(kembali).toString() == "0"){
                    status = "Belum Kembali";
                }
            theModel->setItem(counterPinjam,6,new QStandardItem(QString(status)));
            counterPinjam++;
        }
        countResult = queryPinjam.size();

        //Count Data
        countData.exec("SELECT COUNT(*) FROM tbl_peminjaman");
        countData.next();
        countPeminjaman = countData.value(0).toInt();


    }

}

void Peminjaman::cariPeminjaman(QStandardItemModel *theModel, const QString &kolom, const QString &value, QString status, const QString &hal){

    model = theModel;
    kolomP = kolom;
    valueP = value;
    statusP = status;
    halP = hal;

    QSqlQuery queryPinjam;
    QSqlQuery countData;

    QString currentPage = QString::number(hal.toInt()*50);

    if(queryPinjam.exec("SELECT * FROM tbl_peminjaman WHERE "+kolom+" LIKE \"%"+value+"%\" AND kembali LIKE \"%"+status+"%\" LIMIT "+currentPage+", 50")){
        //qDebug()<<"Cari Query: "+queryPinjam.lastQuery();
        theModel->clear();

        int counterPinjam=0;
				
        // column index
        int siswa 		= queryPinjam.record().indexOf("siswa");
        int buku 		= queryPinjam.record().indexOf("buku");
        int tgl_pinjam 	= queryPinjam.record().indexOf("tgl_pinjam");
        int tgl_tempo 	= queryPinjam.record().indexOf("tgl_tempo");
        int tgl_kembali = queryPinjam.record().indexOf("tgl_kembali");
        int kembali 	= queryPinjam.record().indexOf("kembali");
				
        while(queryPinjam.next()){
            theModel->setItem(counterPinjam,0,new QStandardItem(QString(queryPinjam.value(siswa).toString())));
            theModel->setItem(counterPinjam,1,new QStandardItem(QString(queryPinjam.value(buku).toString())));
            this->buku->setKode(queryPinjam.value(buku).toString());
            theModel->setItem(counterPinjam,2,new QStandardItem(QString( this->buku->getData("judul") )));
            QString tglPinjam = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_pinjam).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            theModel->setItem(counterPinjam,3,new QStandardItem(QString(tglPinjam)));
            QString tglTempo = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_tempo).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            theModel->setItem(counterPinjam,4,new QStandardItem(QString(tglTempo)));
            QString tglKembali = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_kembali).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            theModel->setItem(counterPinjam,5,new QStandardItem(QString(tglKembali)));
                QString status;
                if(queryPinjam.value(kembali).toString() == "1"){
                    status = "Sudah Kembali";
                }else if(queryPinjam.value(kembali).toString() == "0"){
                    status = "Belum Kembali";
                }
            theModel->setItem(counterPinjam,6,new QStandardItem(QString(status)));
            counterPinjam++;
        }
        countResult = queryPinjam.size();


        //Count Peminjam
        countData.exec("SELECT COUNT(no_induk) FROM tbl_anggota WHERE no_induk = \""+value+"\" ");
        countData.next();
        countSiswa = countData.value(0).toInt();
        countData.clear();

    }

}

bool Peminjaman::pinjamBuku(QString peminjam, QString kdbuku){
    QString tanggalPinjam = QDate().currentDate().toString("yyyy-MM-dd");
    QString tanggalTempo = QDate().currentDate().addDays(7).toString("yyyy-MM-dd");
    QSqlQuery querypinjam, querybuku;
    QSettings userSession("Chip-Project","Libska-User");
    QString idPetugas = userSession.value("USERID").toString();
    bool ret=false;

    querybuku.exec("SELECT * FROM tbl_buku WHERE kd_buku = \""+kdbuku+"\" ");
    querybuku.next();
    //qDebug()<<querybuku.lastQuery();

    QString countTersedia = "0";
    if(querybuku.size() != 0){
        countTersedia = querybuku.value(querybuku.record().indexOf("tersedia")).toString();
    }else{
        //messagePinjam = "Buku tersedia";
    }


    if( countTersedia.toInt() >= 1){

        QString sql = "INSERT INTO tbl_peminjaman SET siswa=\""+peminjam+"\", buku=\""+kdbuku+"\", tgl_tempo=\""+tanggalTempo+"\", tgl_pinjam=\""+tanggalPinjam+"\", id_petugas=\""+idPetugas+"\" ";
        if(!querypinjam.exec(sql)){
            qDebug() << "Peminjaman::pinjamBuku : Error Query insert";
            qDebug() << sql;
            ret = false;
        }else{
            //Query Buku
            querybuku.clear();
            if(querybuku.exec("SELECT * FROM tbl_buku WHERE kd_buku = \""+kdbuku+"\" ")){
                querybuku.next();
                QString tersedia = querybuku.value(querybuku.record().indexOf("tersedia")).toString();
                int newTersedia = tersedia.toInt() - 1;
                int newCount = querybuku.value(querybuku.record().indexOf("count")).toInt() + 1;

                if(querybuku.size() == 1){
                    querybuku.clear();
                    if(querybuku.exec("UPDATE tbl_buku SET tersedia = \""+QString::number(newTersedia)+"\", count = \""+QString::number(newCount)+"\" WHERE kd_buku = \""+kdbuku+"\" ")){
                        QSqlQuery querySiswa;
                        querySiswa.exec("SELECT * FROM tbl_anggota WHERE no_induk = \""+peminjam+"\" ");
                        querySiswa.next();

                        int newCountSiswa = querySiswa.value(querybuku.record().indexOf("tersedia")).toInt() + 1;
                        querybuku.clear();
                        if(querybuku.exec("UPDATE tbl_anggota SET count = \""+QString::number(newCountSiswa)+"\" WHERE no_induk = \""+peminjam+"\" ")){

                        }else{
                            qDebug()<<"Error udpate tbl anggota : "+querybuku.lastQuery();
                        }
                        ret = true;
                    }else{
                        qDebug()<<"pinjamBuku : Error update stock";
                        ret = false;
                    }
                }

            }else{
                qDebug()<<"pinjamBuku : Error count stock";
                ret = false;
            }

        }
    }else{
        ret = false;
    }
    return ret;
}

int Peminjaman::getJumlahData(){
    //Count Data
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman");
    query.next();
    countPeminjaman = query.value(0).toInt();

    return countPeminjaman;
}

int Peminjaman::getBelumKembali(){
    //Count Data
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM tbl_peminjaman WHERE kembali = '0'");
    query.next();
    int belum = query.value(0).toInt();

    return belum;
}

int Peminjaman::getJumlahResult(){
    return countResult;
}

bool Peminjaman::isPeminjamSatu(){
    bool ret;
    if(countSiswa==1){
        ret = true;
    }else{
        ret = false;
    }

    return ret;
}

void Peminjaman::appendDataPinjam(QString hal){
    QSqlQuery queryPinjam;
    QString currentPage = "0";

    if(hal.toInt() > 0){

        currentPage = QString::number(hal.toInt()*50);

    }

    if(queryPinjam.exec("SELECT * FROM tbl_peminjaman WHERE "+kolomP+" LIKE \"%"+valueP+"%\" AND kembali LIKE \"%"+statusP+"%\" LIMIT "+currentPage+", 50")){
        //qDebug()<<"Append Query: "+queryPinjam.lastQuery();
        int counterPinjam=hal.toInt()*50;
        //qDebug()<<"Counter Pinjam : "<<counterPinjam;
				
        // volumn index
        int siswa 			= queryPinjam.record().indexOf("siswa");
        int buku 				= queryPinjam.record().indexOf("buku");
        int tgl_pinjam  = queryPinjam.record().indexOf("tgl_pinjam");
        int tgl_tempo 	= queryPinjam.record().indexOf("tgl_tempo");
        int tgl_kembali = queryPinjam.record().indexOf("tgl_kembali");
        int kembali			= queryPinjam.record().indexOf("kembali");
				
        while(queryPinjam.next()){
            model->setItem(counterPinjam,0,new QStandardItem(QString(queryPinjam.value(siswa).toString())));
            model->setItem(counterPinjam,1,new QStandardItem(QString(queryPinjam.value(buku).toString())));
            this->buku->setKode(queryPinjam.value(buku).toString());
            model->setItem(counterPinjam,2,new QStandardItem(QString( this->buku->getData("judul") )));
            QString tglPinjam = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_pinjam).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            model->setItem(counterPinjam,3,new QStandardItem(QString(tglPinjam)));
            QString tglTempo = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_tempo).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            model->setItem(counterPinjam,4,new QStandardItem(QString(tglTempo)));
            QString tglKembali = QLocale(QLocale::Indonesian,QLocale::Indonesia).toString(QDate::fromString(queryPinjam.value(tgl_kembali).toString(),"yyyy-MM-dd"),"dd MMMM yyyy");
            model->setItem(counterPinjam,5,new QStandardItem(QString(tglKembali)));
                QString status;
                if(queryPinjam.value(kembali).toString() == "1"){
                    status = "Sudah Kembali";
                }else if(queryPinjam.value(kembali).toString() == "0"){
                    status = "Belum Kembali";
                }
            model->setItem(counterPinjam,6,new QStandardItem(QString(status)));
            counterPinjam++;
        }
        countResult = queryPinjam.size();
    }else{
        qDebug()<<"Error Query Append :(\n"+queryPinjam.lastError().text()+"\n"+queryPinjam.lastQuery();
    }
}
