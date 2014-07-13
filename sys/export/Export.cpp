#include <sys/export/Export.h>
#include "ui_Export.h"

Export::Export(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    dialog = new QFileDialog();

    ui->comExport->addItems(QStringList()<<"Pilih Data"<<"Buku"<<"Peminjaman"<<"Daftar Anggota"<<"Kas");
    ui->progresExport->setVisible(false);
}

Export::~Export()
{
    delete ui;
}

void Export::on_btnExport_clicked()
{
    ui->labelExport->setText("Mohon Tunggu, Sedang Membuat Laporan...");

    QFileDialog *dialog = new QFileDialog();
    QString fileExport = "SiPUS-Export-"+ui->comExport->currentText()+"-"+QDate().currentDate().toString("yyyyMMdd")+".csv";
    QString pathExport = dialog->getExistingDirectory(this, "Pilih Folder Export",QDir::homePath(),QFileDialog::ShowDirsOnly)+"/"+fileExport;

    QSqlQuery query;

    if(pathExport != "/"+fileExport){
        ui->lineExport->setText(pathExport);
        int columcount = 0;
        QFile file(pathExport);
        if(file.open(QFile::ReadWrite | QFile::Truncate)){

            QTextStream stream(&file);


            QString columsql = "SHOW COLUMNS FROM "+target;
            if(query.exec(columsql)){

                QString columname;

                while(query.next()){
                    columname += "\""+query.value(0).toString()+"\",";
                    //qDebug()<<"Colum : "+query.value(0).toString();
                }
                columcount = query.size();
                columname = columname+"\n";
                stream << columname;
                ui->labelExport->setText("Insert Nama kolom..");
                query.clear();

            }

            QString rowsql = "SELECT * FROM "+target;
            if(query.exec(rowsql)){
                //qDebug()<<"Query size :"+QString::number(query.size());
                ui->progresExport->setMaximum(query.size());
                ui->progresExport->setVisible(true);
                int count=0;
                while(query.next()){
                    QStringList rowVal;
                    for(int x=0; x<=columcount-1; x++){
                        rowVal << "\""+query.value(x).toString()+"\"";
                    }
                    stream << rowVal.join(",")+"\n";
                    ui->labelExport->setText("Insert Baris "+QString::number(count+1));
                    ui->progresExport->setValue(count+1);
                    count++;
                }
                ui->progresExport->setVisible(false);
                ui->labelExport->setText("Berhasil membuat laporan sejumlah "+QString::number(count+1)+" baris data.");
            }
            file.close();
        }else{
            qDebug()<<"Failed Open FIle";
        }
    }else{
        ui->labelExport->clear();
    }

}

void Export::on_comExport_currentIndexChanged(int index)
{
    if(index == 0){
        ui->btnExport->setEnabled(false);
    }else{
        ui->btnExport->setEnabled(true);

        switch (index) {
        case 1:
            target = "tbl_buku";
            break;
        case 2:
            target = "LAP_PEMINJAMAN";
            break;
        case 3:
            target = "tbl_anggota";
            break;
        case 4:
            target = "tbl_kas";
            break;
        case 5:
            target = "tbl_buku";
            break;
        default:
            break;
        }
    }

    //debug
    //qDebug()<<"Target : "+target;

}
