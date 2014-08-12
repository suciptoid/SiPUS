#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QPointer>

namespace Ui {
class DatabaseConfig;
}

class LoginWindow;

class DatabaseConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseConfig(QWidget *parent = 0);
    QSqlDatabase getDb();
    bool isDbConnect();
    QString getError();
    ~DatabaseConfig();

private slots:
    void on_pushButton_2_clicked();

    void on_pTest_clicked();

    void on_pSimpan_clicked();

private:
    //Variable
    QString appPath;
    QSqlDatabase db;
    Ui::DatabaseConfig *ui;
    //Function
    void ReadConfig();
    void simpanSetting();
    //FileConfig *config;
    QPointer<LoginWindow> loginWindow;
};

#endif // DATABASECONFIG_H
