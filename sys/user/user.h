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
    QString getId() const;

    bool isAdmin() const;

private:
    DatabaseConfig *dbCon;
    QString userId;
    bool is_admin;
};

#endif // USER_H
