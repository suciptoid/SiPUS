#include <modul/katalog/KatalogBuku.h>
#include <QSqlRecord>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QDebug>
#include <QStatusBar>

KatalogBuku::KatalogBuku()
{
}

void KatalogBuku::cariKatalog(QStandardItemModel *theModel, const QString &kolom, const QString &value, const QString &status){
    QSqlQuery query;
    QString statusBuku;
    switch(status.toInt()){
    case 0:
        statusBuku = "LIKE \"%%\" ";
        break;
    case 1:
        statusBuku = ">0";
        break;
    case 2:
        statusBuku = "=0";
        break;
    default:
        statusBuku = "LIKE \"%%\"";
    }

    QString sql = "SELECT * FROM tbl_buku WHERE "+kolom+" LIKE \"%"+value+"%\" AND tersedia "+statusBuku+" LIMIT 0,200";
    if(query.exec(sql)){
        theModel->clear();

        int counter=0;
				
				// index column
				int kd_buku 	= query.record().indexOf("kd_buku");
				int judul 		= query.record().indexOf("judul");
				int pengarang = query.record().indexOf("pengarang");
				int penerbit  = query.record().indexOf("penerbit");
				int stock 		= query.record().indexOf("stock");
				int tersedia  = query.record().indexOf("tersedia");
				
        while(query.next()){
            theModel->setItem(counter,0,new QStandardItem(QString(query.value(kd_buku).toString())));
            theModel->setItem(counter,1,new QStandardItem(QString(query.value(judul).toString())));
            theModel->setItem(counter,2,new QStandardItem(QString(query.value(pengarang).toString())));
            theModel->setItem(counter,3,new QStandardItem(QString(query.value(penerbit).toString())));
            theModel->setItem(counter,4,new QStandardItem(QString(query.value(stock).toString())));
            theModel->setItem(counter,5,new QStandardItem(QString(query.value(tersedia).toString())));

            counter++;

        }
    }else{
        qDebug()<< "KatalogBuku :: Error query katalog buku :'(";
    }
}
