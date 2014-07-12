#include "modul/pustakawan/pustakawan.h"
#include "ui_pustakawan.h"

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
    if(query.exec("INSERT INTO tbl_pustakawan SET user = '"+id+"', nama = '"+user+"', kunci =MD5('"+id+"') ")){
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
    ui->lUserId->clear();
    ui->lNama->clear();
    ui->lPwd->clear();
}
