#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>


namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    explicit Export(QWidget *parent = 0);
    ~Export();

private slots:

    void on_btnExport_clicked();

    void on_comExport_currentIndexChanged(int index);

private:
    Ui::Export *ui;
    QString target;
};

#endif // EXPORT_H
