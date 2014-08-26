#include "modul/pustakawan/pustakawan.h"
#include "ui_pustakawan.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QSqlTableModel>
#include "pustakawaneditor.h"
#include "pustakawanmodel.h"

Pustakawan::Pustakawan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pustakawan)
{
    ui->setupUi(this);
    pustakawanModel = new PustakawanModel(this);
    pustakawanModel->select();
    pustakawanModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    pustakawanModel->setHeaderData(0, Qt::Horizontal, "ID");
    pustakawanModel->setHeaderData(1, Qt::Horizontal, "User");
    pustakawanModel->setHeaderData(2, Qt::Horizontal, "Nama");
    pustakawanModel->setHeaderData(3, Qt::Horizontal, "Kunci");
    pustakawanModel->setHeaderData(4, Qt::Horizontal, "Level");

    ui->pustakawanView->setModel(pustakawanModel);
    ui->pustakawanView->horizontalHeader()->hideSection(0);
    ui->pustakawanView->horizontalHeader()->hideSection(3);
    ui->pustakawanView->horizontalHeader()->hideSection(5);
    ui->pustakawanView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

Pustakawan::~Pustakawan()
{
    qDebug() << this << "destroyed!";
    delete ui;
}

void Pustakawan::on_tambahButton_clicked()
{
    PustakawanEditor pe(pustakawanModel, this);
    if(pe.exec()) {
        pustakawanModel->select();
    }
}

void Pustakawan::on_ubahButton_clicked()
{
    if(ui->pustakawanView->selectionModel()->hasSelection() == 1) {
        int baris = ui->pustakawanView->selectionModel()->selectedRows().at(0).row();
        QString id = pustakawanModel->data(pustakawanModel->index(baris, 0)).toString();
        QString user = pustakawanModel->data(pustakawanModel->index(baris, 1)).toString();
        QString nama = pustakawanModel->data(pustakawanModel->index(baris, 2)).toString();
        QString level = pustakawanModel->data(pustakawanModel->index(baris, 4)).toString();
        bool login = pustakawanModel->data(pustakawanModel->index(baris, 5), Qt::EditRole).toBool();

        PustakawanEditor pe(pustakawanModel, this, id, user, nama, level, login);
        pe.exec();
    }
    else {
        QMessageBox::warning(this, "Data belum dipilih", "Harap memilih salah satu data untuk diubah!");
    }
}

void Pustakawan::on_hapusButton_clicked()
{
    QMessageBox mb(this);
    mb.setWindowTitle("Hapus data user");
    mb.setText("Anda yakin ingin menghapus data user ini?");
    QAbstractButton *yaButton = (QAbstractButton*)mb.addButton(QString("Ya"), QMessageBox::YesRole);
    mb.addButton("Tidak", QMessageBox::NoRole);

    mb.exec();

    if(mb.clickedButton() != yaButton)
        return;

    if(ui->pustakawanView->selectionModel()->hasSelection() == 1) {
         int baris = ui->pustakawanView->selectionModel()->selectedRows().at(0).row();

         pustakawanModel->database().transaction();
         if(!pustakawanModel->removeRow(baris)) {
             QMessageBox::critical(this, "Terjadi Kesalahan", pustakawanModel->lastError().text());
             pustakawanModel->database().rollback();
             return;
         } else {
              QMessageBox::information(this, "Informasi", "Data user berhasil dihapus");
              pustakawanModel->submitAll();
              pustakawanModel->database().commit();
         }
    }
    else {
        QMessageBox::warning(this, "Data belum dipilih", "Harap memilih salah satu data untuk dihapus!");
    }
}
