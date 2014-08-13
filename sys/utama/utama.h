#ifndef UTAMA_H
#define UTAMA_H

#include <QMainWindow>

namespace Ui {
class Utama;
}

class FileConfig;
class User;
class LoginWindow;
class QStandardItemModel;
class Statistik;
class Peminjaman;
class About;
class DataPeminjaman;
class KatalogBuku;
class KatalogPinjam;
class Barcode;
class EditBuku;
class Export;
class BukuTelat;
class ListAnggota;
class QCloseEvent;
class LoginWindow;

class Utama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Utama(LoginWindow *lg, QWidget *parent = 0);
    ~Utama();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void tblPinjamBottom(int pos);

private slots:
    void on_actionKeluar_triggered();

    void on_lCariPinjam_returnPressed();

    void on_actionTentang_Aplikasi_triggered();

    void on_pushButton_4_clicked();

    void getTableData(int tab);

    void on_pushButton_clicked();

    void katEnableButton();

    void on_katBPinjam_clicked();

    void on_openBarcode_triggered();

    void on_katBTambah_clicked();

    void on_katBEdit_clicked();

    void on_katBHapus_clicked();

    void on_actionExport_triggered();


    void on_actionBuku_Telat_triggered();

    void on_actionKas_triggered();

    void on_actionDaftar_Anggota_triggered();

    void on_actionUser_triggered();

    void on_actionLogout_triggered();

private:
    Ui::Utama *ui;
    bool connected;
    bool logout;
    int halamanPinjam;
    QString userId;
    FileConfig *config;
    User *userLogin;
    LoginWindow *loginWindow;

    QStandardItemModel *model;
    QStandardItemModel *modelKatalog;

    Peminjaman *pinjam;
    About *tentang;
    DataPeminjaman *dialogPinjam;
    KatalogBuku *katalog;
    KatalogPinjam *katalogPinjam;
    Barcode *barcodeWindow;
    BukuTelat *telatWindow;

    //Function
    void setTblPeminjaman();
    void setTblKatalog();
    void setStatistik();
    void initChart();
};

#endif // UTAMA_H
