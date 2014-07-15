#ifndef KAS_H
#define KAS_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QDate>
#include <QSqlError>
#include <QDebug>
#include <QLocale>
#include <QMessageBox>

namespace Ui {
class Kas;
}

class Kas : public QDialog
{
    Q_OBJECT

public:
    explicit Kas(QWidget *parent = 0);
    ~Kas();

private slots:
    void on_pSimpan_clicked();

    void on_comboBulan_currentIndexChanged(int index);

    void on_bRefresh_clicked();

private:
    Ui::Kas *ui;
    void tataLayout();
    void tataTabel();
    void refreshData();
    void initComboTahun();
    QStandardItemModel *modelKas;
};

#endif // KAS_H
