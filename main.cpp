#include "sys/utama/utama.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Utama w;
    w.show();

    return a.exec();
}
