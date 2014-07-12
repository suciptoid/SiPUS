#-------------------------------------------------
#
# Project created by QtCreator 2013-11-10T08:11:40
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += static staticlib

QMAKE_LFLAGS += -Wl,-rpath,\\$\$ORIGIN/lib/:\\$\$ORIGIN/../lib/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SiPUS
TEMPLATE = app


SOURCES += main.cpp\
        sys/utama/utama.cpp \
    sys/databaseconfig/databaseconfig.cpp \
    sys/fileconfig/fileconfig.cpp \
    sys/user/user.cpp \
    sys/loginwindow/loginwindow.cpp \
    modul/peminjaman/peminjaman.cpp \
    modul/buku/buku.cpp \
    sys/about/about.cpp \
    modul/datapeminjam/datapeminjaman.cpp \
    modul/katalog/KatalogBuku.cpp \
    modul/katalogpinjam/KatalogPinjam.cpp \
    modul/anggota/Anggota.cpp \
    modul/barcode/barcode.cpp \
    modul/editbuku/EditBuku.cpp \
    sys/export/Export.cpp \
    modul/statistik/Statistik.cpp \
    modul/jatuhtempo/jatuhtempo.cpp \
    modul/bukutelat/bukutelat.cpp \
    modul/kas/kas.cpp \
    modul/listanggota/listanggota.cpp \
    modul/pustakawan/pustakawan.cpp

HEADERS  += sys/utama/utama.h \
    sys/databaseconfig/databaseconfig.h \
    sys/fileconfig/fileconfig.h \
    sys/user/user.h \
    sys/loginwindow/loginwindow.h \
    modul/peminjaman/peminjaman.h \
    modul/buku/buku.h \
    sys/about/about.h \
    modul/datapeminjam/datapeminjaman.h \
    modul/katalog/KatalogBuku.h \
    modul/katalogpinjam/KatalogPinjam.h \
    modul/anggota/Anggota.h \
    modul/barcode/barcode.h \
    modul/editbuku/EditBuku.h \
    sys/export/Export.h \
    modul/statistik/Statistik.h \
    modul/jatuhtempo/jatuhtempo.h \
    modul/bukutelat/bukutelat.h \
    modul/kas/kas.h \
    modul/listanggota/listanggota.h \
    modul/pustakawan/pustakawan.h

FORMS    += sys/utama/utama.ui \
    sys/databaseconfig/databaseconfig.ui \
    sys/loginwindow/loginwindow.ui \
    sys/about/about.ui \
    modul/datapeminjam/datapeminjaman.ui \
    modul/katalogpinjam/KatalogPinjam.ui \
    modul/barcode/barcode.ui \
    modul/editbuku/EditBuku.ui \
    sys/export/Export.ui \
    modul/bukutelat/bukutelat.ui \
    modul/kas/kas.ui \
    modul/listanggota/listanggota.ui \
    modul/pustakawan/pustakawan.ui

RESOURCES += \
    glypticon.qrc
