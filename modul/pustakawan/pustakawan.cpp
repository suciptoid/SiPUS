#include "modul/pustakawan/pustakawan.h"
#include "ui_pustakawan.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>

Pustakawan::Pustakawan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pustakawan)
{
    ui->setupUi(this);
}

Pustakawan::~Pustakawan()
{
    delete ui;
}

void Pustakawan::on_bTambah_clicked()
{
    QString id,user,pwd;
    id = ui->lUserId->text();
    user = ui->lNama->text();
    pwd = ui->lPwd->text();

    QSqlQuery query;
    query.prepare("INSERT INTO tbl_pustakawan(user, nama, kunci) VALUES(?,?,MD5(?))");
    query.bindValue(0, id);
    query.bindValue(1, user);
    query.bindValue(2, pwd);
    if(query.exec()){
        QMessageBox::information(this,"Berhasil","Penambahan User Berhasil");
        ui->lUserId->clear();
        ui->lNama->clear();
        ui->lPwd->clear();
    }else{
        QMessageBox::critical(this,"Gagal","Penambahan Gagal \n\nAlasan : "+query.lastError().text());
    }
}

void Pustakawan::on_bBatal_clicked()
{
    // Issue #2 https://github.com/showcheap/SiPUS/issues/2
    if(ui->lUserId->text().isEmpty() && ui->lNama->text().isEmpty() && ui->lPwd->text().isEmpty()){
        //Tutup window jika semua sudah kosong
        this->close();
    }else{
        //Reset text edit
        ui->lUserId->clear();
        ui->lNama->clear();
        ui->lPwd->clear();
        //Focus ke text edit pertama
        ui->lUserId->setFocus();
    }
}
