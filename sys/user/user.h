#ifndef USER_H
#define USER_H

#include <QString>
#include <QCryptographicHash>
#include <sys/user/user.h>
#include <sys/databaseconfig/databaseconfig.h>

class User
{
public:
    User();
    ~User();
    bool chekLogin(QString user, QString password);
    QString getId();
private:
    DatabaseConfig *dbCon;
    QString userId;



};

#endif // USER_H
