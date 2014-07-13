#ifndef UTAMA_H
#define UTAMA_H

#include <QMainWindow>
#include <sys/databaseconfig/databaseconfig.h>
#include <sys/fileconfig/fileconfig.h>
#include <sys/user/user.h>
#include <sys/loginwindow/loginwindow.h>
#include <QStandardItemModel>
#include <modul/peminjaman/peminjaman.h>
#include <sys/about/about.h>
#include <modul/datapeminjam/datapeminjaman.h>
#include <modul/katalog/KatalogBuku.h>
#include <modul/katalogpinjam/KatalogPinjam.h>
#include <modul/barcode/barcode.h>
#include <QCloseEvent>
#include <QSettings>
#include <modul/editbuku/EditBuku.h>
#include <sys/export/Export.h>
#include <QScrollBar>
#include <modul/bukutelat/bukutelat.h>
#include <modul/statistik/Statistik.h>
#include <modul/anggota/Anggota.h>
#include <modul/kas/kas.h>
#include <modul/listanggota/listanggota.h>
#include <modul/pustakawan/pustakawan.h>

namespace Ui {
class Utama;
}

class Utama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Utama(QWidget *parent = 0);
    ~Utama();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void unLock();
    void tblPinjamBottom(int pos);

private slots:
    void on_actionKeluar_triggered();

    void on_actionKonfigurasi_DB_triggered();

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

private:
    Ui::Utama *ui;
    bool connected;
    int halamanPinjam;
    QString userId;
    DatabaseConfig *configWindow;
    FileConfig *config;
    User *userLogin;
    LoginWindow *loginWindow;

    QStandardItemModel *model;
    QStandardItemModel *modelKatalog;
    Statistik *statistik;

    Peminjaman *pinjam;
    About *tentang;
    DataPeminjaman *dialogPinjam;
    KatalogBuku *katalog;
    KatalogPinjam *katalogPinjam;
    Barcode *barcodeWindow;
    EditBuku *editbuku;
    Export *exportWindow;
    BukuTelat *telatWindow;
    ListAnggota *listanggota;

    //Function
    void setTblPeminjaman();
    void setTblKatalog();
    void setStatistik();
    void initChart();
    //void closeEvent(QCloseEvent *bar);
};

#endif // UTAMA_H
