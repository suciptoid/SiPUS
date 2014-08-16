#ifndef PUSTAKAWAN_H
#define PUSTAKAWAN_H

#include <QDialog>

namespace Ui {
class Pustakawan;
}

//class QSqlTableModel;
class PustakawanModel;

class Pustakawan : public QDialog
{
    Q_OBJECT

public:
    explicit Pustakawan(QWidget *parent = 0);
    ~Pustakawan();

private slots:


    void on_tambahButton_clicked();

    void on_ubahButton_clicked();

    void on_hapusButton_clicked();

private:
    Ui::Pustakawan *ui;
    PustakawanModel *pustakawanModel;
};

#endif // PUSTAKAWAN_H
