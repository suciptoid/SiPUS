//#include "sys/utama/utama.h"
#include "sys/databaseconfig/databaseconfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Utama w;
//    w.show();

    DatabaseConfig dc;


    return a.exec();
}
