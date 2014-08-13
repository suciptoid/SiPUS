#include "sys/databaseconfig/databaseconfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseConfig dc;

    return a.exec();
}
