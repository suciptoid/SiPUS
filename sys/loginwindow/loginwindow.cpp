#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "sys/user/user.h"
#include "sys/utama/utama.h"
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    setAttribute(Qt::WA_DeleteOnClose);
    userLog = new User();
}

LoginWindow::~LoginWindow()
{
    qDebug() << this << "destroyed!";
    delete ui;
}

void LoginWindow::on_logMasuk_clicked()
{
    user = ui->logUser->text();
    password = ui->logPwd->text();
    ui->logUser->setText("");
    ui->logUser->setFocus();
    ui->logPwd->setText("");

    if(userLog->chekLogin(user,password)){
        //qDebug()<<"User Login Success, ID: "<<userLog->getId();
        QSettings userSession("Chip-Project","Libska-User");
        userSession.setValue("USERID",userLog->getId());
        emit sendUnlock();
//        this->close();
        (new Utama(this))->show();
        hide();
    }else{
        QMessageBox::warning(this,"Gagal..","Login User Gagal, Periksa Kembali !!");
    }
}

void LoginWindow::on_logKeluar_clicked()
{
    this->close();
}
