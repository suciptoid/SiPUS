#ifndef BARCODE_H
#define BARCODE_H

#include <QMainWindow>
#include <modul/anggota/Anggota.h>
#include <QStandardItemModel>
#include <modul/buku/buku.h>
#include <modul/peminjaman/peminjaman.h>
#include <QStyle>
#include <QDesktopWidget>
#include <QDate>

namespace Ui {
class Barcode;
}

class Barcode : public QMainWindow
{
    Q_OBJECT

public:
    explicit Barcode(QWidget *parent = 0);
    ~Barcode();

private slots:
    void on_lScanAnggota_returnPressed();

    void on_bBarcodeSelesai_clicked();

    void on_bBarcodeTutup_clicked();

    void setTblBarcodePinjam();

    void on_lScanBuku_returnPressed();

    void refreshData();

private:
    Ui::Barcode *ui;
    Anggota *peminjam;
    QStandardItemModel *modelPinjam;
    Buku *buku;
    Peminjaman *peminjaman;
};

#endif // BARCODE_H
