#ifndef FILECONFIG_H
#define FILECONFIG_H

#include <QString>
#include <QTextStream>

class FileConfig
{
public:
    FileConfig();
    void ReadConfig();
    void WriteConfig();
    QString getConfig(QString value);
private:
    QString appPath;
    QString hasil;
};

#endif // FILECONFIG_H
