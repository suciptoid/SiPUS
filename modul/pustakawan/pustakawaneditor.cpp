#include "pustakawaneditor.h"
#include "ui_pustakawaneditor.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QCryptographicHash>
#include "pustakawanmodel.h"


PustakawanEditor::PustakawanEditor(PustakawanModel *pmodel, QWidget *parent, const QString &id, const QString &user, const QString &nama, const QString &level, bool login) :
    QDialog(parent),ui(new Ui::PustakawanEditor),edit_mode(false), model(pmodel)
{
    ui->setupUi(this);

    if(!id.isEmpty()) {
        edit_mode = true;
        pustakawan_id = id.toInt();
        ui->userEdit->setText(old_user = user);
        ui->namaEdit->setText(nama);
        ui->loginCBox->setChecked(login);
        ui->levelCombo->setCurrentIndex( level == "Admin" ? 0 : 1);
        setWindowTitle("Ubah data user pustakawan");
    } else {
        setWindowTitle("Tambah data user pustakawan");
    }
}

PustakawanEditor::~PustakawanEditor()
{
    qDebug() << this << "destroyed!";
    delete ui;
}

void PustakawanEditor::on_simpanButton_clicked()
{
    if(ui->userEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "User tidak boleh kosong!");
        return;
    }

    if(ui->namaEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Nama tidak boleh kosong!");
        return;
    }

    if(!edit_mode && (ui->passwordEdit->text().isEmpty() || ui->password2Edit->text().isEmpty())) {
        QMessageBox::warning(this, "Peringatan", "Password & ulangi password harap diisi!");
        return;
    }

    if(ui->passwordEdit->text() != ui->password2Edit->text()) {
        QMessageBox::warning(this, "Peringatan", "Paswword & ulangi password harus sama!");
        return;
    }

    if(edit_mode)
        perbaruiUser();
    else
        tambahUser();
}

void PustakawanEditor::tambahUser()
{
    QSqlQuery tambahQuery;

    bool user_found = false;
    for(int i = 0 ; i < model->rowCount(); i++) {
        if(model->data(model->index(i, 1)).toString() != ui->userEdit->text()) {
            continue;
        }

        user_found = true;
        break;
    }

    if(user_found) {
        QMessageBox::warning(this, "Gagal tambah user pustakawan", "User pustakawan sudah ada.");
        return;
    }

    model->database().transaction();

    QSqlRecord rec = model->record();
    QString password = QCryptographicHash::hash(ui->passwordEdit->text().toLatin1(), QCryptographicHash::Md5).toHex();
    rec.setValue("user", ui->userEdit->text());
    rec.setValue("nama", ui->namaEdit->text());
    rec.setValue("kunci", password);
    rec.setValue("level", ui->levelCombo->currentText());
    rec.setValue("login", ui->loginCBox->isChecked());
    model->insertRecord(-1, rec);
    if(model->submitAll()) {
        QMessageBox::information(this, "Info", "Tambah user pustakawan berhasil!");
        model->database().commit();
        accept();
    } else {
        model->database().rollback();
         QMessageBox::critical(this, "Error", "Terjadi kesalahan: " + model->lastError().text());
    }
}

void PustakawanEditor::perbaruiUser()
{
    bool ubah_password = false;
    bool user_found = false;
    QSqlQuery ubahQuery;

    if(ui->userEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Data tidak lengkap", "User tidak boleh kosong");
        return;
    }

    if(ui->namaEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Data tidak lengkap", "Nama harus diisi");
        return;
    }

    // jika salah satu tidak kosong berarti ada rencana untuk ganti password
    if(!ui->passwordEdit->text().isEmpty() || !ui->password2Edit->text().isEmpty()) {
        // jika salah satu kosong, tampilkan peringatan
        if(ui->passwordEdit->text().isEmpty() || ui->password2Edit->text().isEmpty()) {
            QMessageBox::warning(this, "Data tidak lengkap", "Password & ulangi password harus diisi");
            return;
        }
        // jika tidak sama, tampilkan peringatan
        else if(ui->passwordEdit->text() != ui->password2Edit->text()) {
            QMessageBox::warning(this, "Data tidak lengkap", "Password & ulangi password harus sama");
            return;
        } else {
            ubah_password = true;
        }
    }



    // cek jika user diganti
    if(old_user != ui->userEdit->text()) {
        for(int i = 0; i < model->rowCount(); i++) {
            if(model->data(model->index(i, 0)).toInt() == pustakawan_id)
                continue;
            if(model->data(model->index(i, 1)).toString() != ui->userEdit->text())
                continue;

            user_found = true;
            break;
        }

        if(user_found) {
            QMessageBox::warning(this, "Ubah data user pustakawan gagal", "User sudah ada, harap diperiksa kembali");
            return;
        }
    }

    // find record
    model->database().transaction();

    QSqlRecord rec;
    int row = -1;
    for(int i = 0; i < model->rowCount(); i++) {
        QSqlRecord tmp = model->record(i);
        if(tmp.value("id").toInt() == pustakawan_id) {
            rec = tmp;
            row = i;
            break;
        }
    }

    if(row > -1) {
        rec.setValue("user", ui->userEdit->text());
        rec.setValue("nama", ui->namaEdit->text());
        rec.setValue("level", ui->levelCombo->currentText());
        rec.setValue("login", ui->loginCBox->isChecked());

        if(ubah_password) {
            QString password = QCryptographicHash::hash(ui->passwordEdit->text().toLatin1(), QCryptographicHash::Md5).toHex();
            rec.setValue("kunci", password);
        }
    } else {
        QMessageBox::critical(this, "Data tidak ditemukan", "Data di tabel tidak ditemukan");
        model->database().rollback();
        return;
    }

    model->setRecord(row, rec);
    if(model->submitAll()) {
        QMessageBox::information(this, "Ubah data sukses", "Data user berhasil diubah");
        model->database().commit();
        accept();
    } else {
        QMessageBox::critical(this, "Terjadi kesalahan", ubahQuery.lastError().text());
        model->database().rollback();
        return;
    }
}
