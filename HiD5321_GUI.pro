#-------------------------------------------------
#
# Project created by QtCreator 2014-08-21T14:33:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HiD5321_GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scard.cpp \
    globals.cpp \
    response.cpp

HEADERS  += mainwindow.h \
#    debuglog.h \
#    ifdhandler.h \
#    pcsclite.h \
#    reader.h \
#    winscard.h \
    scard.h \
#    globals.h \
    response.h \
#    mem.h \
#    ok.h \
#    okos.h \
#    okpoff.h \
#    okpon.h \
#    pcscdefines.h \
#    pcscsub.h \
#    scard2w.h \
#    scard3w.h \
#    scardcl.h \
#    scardi2c.h


HEADERS += /usr/include/openssl/*.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib64/ -lpcsclite

#unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lsyncapi
#unix:!macx: LIBS += /usr/local/lib/libokbase2.so
INCLUDEPATH += $$PWD/../../../../usr/local/lib
DEPENDPATH += $$PWD/../../../../usr/local/lib



unix:!macx: LIBS += -L$$PWD/../../../../usr/lib64/libssl.so.0.9.8
unix:!macx: LIBS += -L$$PWD/../../../../usr/lib64/libcrypto.so.0.9.8
INCLUDEPATH += $$PWD/../../../../usr/lib64
DEPENDPATH += $$PWD/../../../../usr/lib64
