#ifndef USER_H
#define USER_H

#include <QString>

class DatabaseConfig;

class User
{
public:
    User();
    ~User();
    bool chekLogin(const QString &user, const QString &password);
    QString getId();
private:
    DatabaseConfig *dbCon;
    QString userId;
};

#endif // USER_H
