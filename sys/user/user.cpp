#include "user.h"
#include <sys/databaseconfig/databaseconfig.h>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDebug>

User::User()
{
}

bool User::chekLogin(QString user, QString password){
    QSqlQuery queryLogin, idUser;
    bool ret=false;
    QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(password.toLatin1());
        QString hasPwd = hash.result().toHex();
    if(queryLogin.exec("SELECT * FROM tbl_pustakawan WHERE user=\""+user+"\" AND kunci=\""+hasPwd+"\" ")){

        int count=0;
        while(queryLogin.next()){
            count++;
        }

        if(count==1){
            if(idUser.exec("SELECT id FROM tbl_pustakawan WHERE user=\""+user+"\" AND kunci=\""+hasPwd+"\"")){
                while (idUser.next()) {
                    userId = idUser.value("id").toString();
                }
            }

            ret = true;
        }else{
            ret = false;
        }

    }else{
        qDebug()<< "User SQL Query : "<<queryLogin.lastError();
    }
    return ret;
}

QString User::getId(){
    return userId;
}

User::~User(){
}
