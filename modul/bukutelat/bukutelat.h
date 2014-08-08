#ifndef BUKUTELAT_H
#define BUKUTELAT_H

#include <QMainWindow>

class QStandardItemModel;
class Buku;
class Anggota;

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
