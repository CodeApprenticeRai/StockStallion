#include "mainwindow.h"
#include <QApplication>

//Entry point for StockStallion. Opens login window.
int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
