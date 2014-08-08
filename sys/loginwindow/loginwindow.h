#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class User;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_logMasuk_clicked();

    void on_logKeluar_clicked();
signals:
    void sendUnlock();

private:
    Ui::LoginWindow *ui;
    User *userLog;
    QString user, password;
};

#endif // LOGINWINDOW_H
