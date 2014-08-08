#ifndef EDITBUKU_H
#define EDITBUKU_H

#include <QMainWindow>

namespace Ui {
class EditBuku;
}

class Buku;

class EditBuku : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditBuku(QWidget *parent = 0, const QString &mode = "tambah");
    void setKodeEdit(QString kode);
    ~EditBuku();

private slots:
    void on_bEditTambah_clicked();

    void on_bEditSimpan_clicked();


    void on_bEditHapus_clicked();

private:
    Ui::EditBuku *ui;
    QString modeApp;
    QString kodeEdit;
    Buku *buku;

};

#endif // EDITBUKU_H
