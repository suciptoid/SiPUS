#include "pustakawaneditor.h"
#include "ui_pustakawaneditor.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PustakawanEditor::PustakawanEditor(QWidget *parent, const QString &id, const QString &user, const QString &nama, const QString &level, bool login) :
    QDialog(parent),ui(new Ui::PustakawanEditor),edit_mode(false)
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

    tambahQuery.prepare("SELECT count(user) FROM tbl_pustakawan WHERE user=?");
    tambahQuery.bindValue(0, ui->userEdit->text());
    if(tambahQuery.exec()) {
        tambahQuery.next();
        if(tambahQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Gagal tambah user pustakawan", "User pustakawan sudah ada.");
            return;
        }
    }
    else {
        QMessageBox::critical(this, "Terjadi kesalahan", tambahQuery.lastError().text());
        return;
    }

    tambahQuery.prepare("INSERT INTO tbl_pustakawan(user,nama,kunci,level,login) VALUES(?,?,MD5(?),?,?)");
    tambahQuery.bindValue(0, ui->userEdit->text());
    tambahQuery.bindValue(1, ui->namaEdit->text());
    tambahQuery.bindValue(2, ui->passwordEdit->text());
    tambahQuery.bindValue(3, ui->levelCombo->currentText());
    tambahQuery.bindValue(4, (int)ui->loginCBox->isChecked());

    if(tambahQuery.exec()) {
        QMessageBox::information(this, "Info", "Tambah user pustakawan berhasil!");
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Terjadi kesalahan: " + tambahQuery.lastError().text());
    }
}

void PustakawanEditor::perbaruiUser()
{
    bool ubah_password = false;

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
        }
    }

    QSqlQuery ubahQuery;

    // cek jika user diganti
    if(old_user != ui->userEdit->text()) {
        ubahQuery.prepare("SELECT count(user) FROM tbl_pustakawan WHERE user=? AND id <> ?");
        ubahQuery.bindValue(0, ui->userEdit->text());
        ubahQuery.bindValue(1, pustakawan_id);
        if(ubahQuery.exec()) {
            ubahQuery.next();
            if(ubahQuery.value(0).toInt() > 0) {
                QMessageBox::warning(this, "Ubah data user pustakawan gagal", "User sudah ada, harap diperiksa kembali");
                return;
            }
        } else {
            QMessageBox::critical(this, "Terjadi kesalahan", ubahQuery.lastError().text());
            return;
        }
    }


    if(ubah_password) {
        ubahQuery.prepare("UPDATE tbl_pustakawan SET user=?, nama=?, password=?, level=?, login=? WHERE id=?");
        ubahQuery.bindValue(0, ui->userEdit->text());
        ubahQuery.bindValue(1, ui->namaEdit->text());
        ubahQuery.bindValue(2, ui->passwordEdit->text());
        ubahQuery.bindValue(3, ui->levelCombo->currentText());
        ubahQuery.bindValue(4, (int)ui->loginCBox->isChecked());
        ubahQuery.bindValue(5, pustakawan_id);
    } else {
        ubahQuery.prepare("UPDATE tbl_pustakawan SET user=?, nama=?,level=?,login=? WHERE id=?");
        ubahQuery.bindValue(0, ui->userEdit->text());
        ubahQuery.bindValue(1, ui->namaEdit->text());
        ubahQuery.bindValue(2, ui->levelCombo->currentText());
        ubahQuery.bindValue(3, (int)ui->loginCBox->isChecked());
        ubahQuery.bindValue(4, pustakawan_id);
    }

    if(!ubahQuery.exec()) {
        QMessageBox::critical(this, "Terjadi kesalahan", ubahQuery.lastError().text());
    } else {
        QMessageBox::information(this, "Ubah data sukses", "Data user berhasil diubah");
        accept();
    }
}
