#ifndef PUSTAKAWAN_H
#define PUSTAKAWAN_H

#include <QDialog>

namespace Ui {
class Pustakawan;
}

class Pustakawan : public QDialog
{
    Q_OBJECT

public:
    explicit Pustakawan(QWidget *parent = 0);
    ~Pustakawan();

private slots:
    void on_bTambah_clicked();

    void on_bBatal_clicked();

private:
    Ui::Pustakawan *ui;
};

#endif // PUSTAKAWAN_H
