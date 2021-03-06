#include <modul/anggota/Anggota.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

Anggota::Anggota()
{
    error = true;
    count = 0;
}

void Anggota::setId(const QString &idAnggota){
    id = idAnggota;
}

QString Anggota::getData(const QString &kolom){
    QSqlQuery query;
    QString ret;

    query.prepare("SELECT * FROM tbl_anggota WHERE no_induk = ?");
    query.bindValue(0, id);
    if(query.exec()){
        query.next();
        if(query.size() == 1){ // not all databases support this method
            ret = query.value(query.record().indexOf(kolom)).toString();
            count = query.size();
            error = false;
        }else{
            error = true;
        }
    }else{
        error = true;
        qDebug() << "Anggota :: Error Query anggota";
    }
    return ret;
}

bool Anggota::isError(){
    return error;
}

int Anggota::getCount(){
    return count;
}
