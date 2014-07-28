#ifndef KATALOGPINJAM_H
#define KATALOGPINJAM_H

#include <QDialog>

namespace Ui {
class KatalogPinjam;
}

class Buku;
class Anggota;
class Peminjaman;

class KatalogPinjam : public QDialog
{
    Q_OBJECT

public:
    explicit KatalogPinjam(QString kode = "",QWidget *parent = 0);
    ~KatalogPinjam();

private slots:
    void on_katBPinjam_clicked();

    void on_lineEdit_returnPressed();

private:
    Ui::KatalogPinjam *ui;
    Buku *buku;
    Anggota *anggota;
    Peminjaman *peminjaman;
};

#endif // KATALOGPINJAM_H
