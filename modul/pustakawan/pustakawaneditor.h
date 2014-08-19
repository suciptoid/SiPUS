#ifndef PUSTAKAWANEDITOR_H
#define PUSTAKAWANEDITOR_H

#include <QDialog>

namespace Ui {
class PustakawanEditor;
}

class PustakawanModel;

class PustakawanEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PustakawanEditor(PustakawanModel *pmodel, QWidget *parent = 0, const QString &id = "", const QString &user = "", const QString &nama = "", const QString &level = "", bool login = false);
    ~PustakawanEditor();

private slots:
    void on_simpanButton_clicked();

private:
    Ui::PustakawanEditor *ui;
    bool edit_mode;
    int pustakawan_id;
    QString old_user;
    PustakawanModel *model;

    void tambahUser();
    void perbaruiUser();
};

#endif // PUSTAKAWANEDITOR_H
