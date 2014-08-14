#include "user.h"
#include <sys/databaseconfig/databaseconfig.h>
#include <sys/user/user.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QSqlRecord>
#include <QDebug>

User::User() : is_admin(false)
{
}

bool User::chekLogin(const QString &user, const QString &password){
    QSqlQuery queryLogin, idUser;
    bool ret=false;
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(password.toLatin1());
    QString hasPwd = hash.result().toHex();
    queryLogin.prepare("SELECT * FROM tbl_pustakawan WHERE user = ? AND kunci = ?");
    queryLogin.bindValue(0, user);
    queryLogin.bindValue(1, hasPwd);
    if(queryLogin.exec()){
        int count=0;
        while(queryLogin.next()){
            count++;
        }

        if(count==1){
            queryLogin.first();
            userId = queryLogin.value(queryLogin.record().indexOf("id")).toString();
            is_admin = (queryLogin.value(queryLogin.record().indexOf("level")).toString() == QString("Admin"));
            ret = true;
        }else{
            ret = false;
        }

    }else{
        qDebug()<< "User SQL Query : "<<queryLogin.lastError();
    }
    return ret;
}

QString User::getId() const{
    return userId;
}

User::~User(){
    qDebug() << "User(" << this << ") deleted!";
}

bool User::isAdmin() const {
    return is_admin;
}
