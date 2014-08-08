#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <sys/user/user.h>
#include <QMessageBox>
#include <QSettings>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    //Signal
    connect(this,SIGNAL(sendUnlock()),parentWidget(),SLOT(unLock()));

    ui->setupUi(this);
    this->setFixedSize(this->size());
    //User Class
    userLog = new User();


}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_logMasuk_clicked()
{
    user = ui->logUser->text();
    password = ui->logPwd->text();
    if(userLog->chekLogin(user,password)){
        //qDebug()<<"User Login Success, ID: "<<userLog->getId();
        QSettings userSession("Chip-Project","Libska-User");
        userSession.setValue("USERID",userLog->getId());
        emit sendUnlock();
        this->close();
    }else{
        QMessageBox::warning(this,"Gagal..","Login User Gagal, Periksa Kembali !!");
    }
}

void LoginWindow::on_logKeluar_clicked()
{
    this->close();
}
