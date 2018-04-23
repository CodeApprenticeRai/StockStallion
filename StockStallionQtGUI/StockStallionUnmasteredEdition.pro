#-------------------------------------------------
#
# Project created by QtCreator 2018-03-24T22:42:47
#
#-------------------------------------------------

QT       += core gui sql network

#LIBS += C:/Qt/5.10.1/msvc2015/bin/libcrypto-1_1.lib
#LIBS += C:/Qt/5.10.1/msvc2015/bin/libssl-1_1.lib
INCLUDEPATH += C:/OpenSSL-Win32/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StockStallionUnmasteredEdition
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    registerwindow.cpp \
    stockstallionwindow.cpp \
    tab_portfolio.cpp \
    tab_profile.cpp \
    tab_about.cpp \
    portfoliostockwidget.cpp \
    stockselection.cpp \
    sharesselection.cpp \
    tickerprice.cpp

HEADERS += \
        mainwindow.h \
    registerwindow.h \
    stockstallionwindow.h \
    tab_portfolio.h \
    tab_profile.h \
    tab_about.h \
    portfoliostockwidget.h \
    stockselection.h \
    sharesselection.h \
    tickerprice.h

FORMS += \
    mainwindow.ui \
    registerwindow.ui \
    stockstallionwindow.ui \
    tab_portfolio.ui \
    tab_profile.ui \
    tab_about.ui \
    portfoliostockwidget.ui \
    stockselection.ui \
    sharesselection.ui

RESOURCES += \
    images.qrc
