#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QFileDialog>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDate>
#include <QDebug>

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
    QFileDialog *dialog;
    QString target;
};

#endif // EXPORT_H
