#include <modul/anggota/Anggota.h>

Anggota::Anggota()
{
    error = true;
    count = 0;
}

void Anggota::setId(QString idAnggota){
    id = idAnggota;
}

QString Anggota::getData(QString kolom){
    QSqlQuery query;
    QString ret;

    if(query.exec("SELECT * FROM tbl_anggota WHERE no_induk = \""+id+"\" ")){
        query.next();
        if(query.size() == 1){
            ret = query.value(kolom).toString();
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
