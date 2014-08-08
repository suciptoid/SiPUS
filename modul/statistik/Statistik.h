#ifndef STATISTIK_H
#define STATISTIK_H

#include <QObject>

class Statistik : public QObject
{
    Q_OBJECT
public:
    explicit Statistik(QObject *parent = 0);
    int getTotalBuku();
    int getBukuTersedia();
    int getBukuTelat();
    int getBukuKembali();
    int getTotalPeminjaman();
    int getItemBuku();
    int getPinjamHari();
    int getPInjamBulan();
    int getPinjamTahun();
};

#endif // STATISTIK_H
