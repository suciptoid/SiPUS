#ifndef LISTANGGOTA_H
#define LISTANGGOTA_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class ListAnggota;
}

class ListAnggota : public QDialog
{
    Q_OBJECT

public:
    explicit ListAnggota(QWidget *parent = 0);
    ~ListAnggota();

private slots:
    void on_btnAgtBtl_clicked();

    void on_bAgtSimpan_clicked();

    void setTabelAnggota();
    void refreshData(QString key = "");

    void on_btnCari_clicked();

private:
    Ui::ListAnggota *ui;

    QString noAnggota, namaAnggota, kelasAnggota, jurusanAnggota;
    QSqlQuery query;
    QStandardItemModel *modelanggota;
};

#endif // LISTANGGOTA_H
