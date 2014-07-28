#include "fileconfig.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QTextStream>

FileConfig::FileConfig()
{

}

void FileConfig::ReadConfig(){
    QDir dir;
    QString path = dir.absolutePath();
    QString file = path+"/config.cfg";

    QFile config(file);
    if(!config.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "ReadConfig : Error Saat membaca file konfigurasi";
        return;
    }

    QTextStream result(&config);
    QString hasil = result.readAll();

    qDebug() << hasil;
}

void FileConfig::WriteConfig(){
    QDir dir;
    QString path = dir.absolutePath();
    QString file = path+"/config.cfg";

    QFile config(file);
    if(!config.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
        qDebug() << "WriteConfig : Error Saat membaca file konfigurasi";
        return;
    }

    QTextStream result(&config);
    result << "ChipTooooooooooooooooooo=ok\nRumahnya=Sirigan\n";
    config.flush();
    config.close();

}
//Get value from file configuration
QString FileConfig::getConfig(QString value){
    QDir dir;
    QString path = dir.absolutePath();
    QString file = path+"/config.cfg";

    QFile config(file);
    if(!config.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "getConfig : Error Saat membaca file konfigurasi";
        return "error";
    }

    QTextStream result(&config);
    QString configValue =result.readAll();
    QStringList lineConfig = configValue.split("\n");

    QString lineResult;
    int lineCount = lineConfig.count();
    for(int x=0; x<= lineCount; x++){
        QStringList lineValue = lineConfig.value(x).split(":");
        if(value == lineValue.value(0)){
            lineResult = lineValue.value(1);
            break;
        }
    }
    config.close();
    return lineResult;
}
