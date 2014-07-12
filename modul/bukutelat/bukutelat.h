#ifndef BUKUTELAT_H
#define BUKUTELAT_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QDate>
#include <QDebug>
#include <QSqlError>
#include <modul/anggota/Anggota.h>
#include <modul/buku/buku.h>
#include <QLocale>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStandardItem>
#include <QMessageBox>

namespace Ui {
class BukuTelat;
}

class BukuTelat : public QMainWindow
{
    Q_OBJECT

public:
    explicit BukuTelat(QWidget *parent = 0);
    ~BukuTelat();

private slots:
    void on_bExportCSV_clicked();

private:
    Ui::BukuTelat *ui;
    void refreshData();
    void setTabel();
    QStandardItemModel *modelTelat;
    Buku *buku;
    Anggota *anggota;
};

#endif // BUKUTELAT_H
