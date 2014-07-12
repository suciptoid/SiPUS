#ifndef STATISTIK_H
#define STATISTIK_H

#include <QObject>
#include <QSqlQuery>
#include <QDate>
#include <QVariant>

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

signals:

public slots:

private:
    QString sekarang;
    QSqlQuery query;
};

#endif // STATISTIK_H
