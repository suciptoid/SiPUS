#ifndef DATAPEMINJAMAN_H
#define DATAPEMINJAMAN_H

#include <QDialog>


class Anggota;
class Buku;
class QStandardItemModel;

namespace Ui {
class DataPeminjaman;
}

class DataPeminjaman : public QDialog
{
    Q_OBJECT

public:
    explicit DataPeminjaman(QWidget *parent = 0, const QString &no = "0");
    void setNoAnggota(QString no);
    ~DataPeminjaman();

private slots:
    void on_bClose_clicked();

    void on_bKembali_clicked();

    void on_bKembaliAll_clicked();

private:
    void setTabel();
    void refreshData();
    Ui::DataPeminjaman *ui;
    QStandardItemModel *model;
    QString noAnggota;
    Anggota *peminjam;
    Buku *buku;
};

#endif // DATAPEMINJAMAN_H
