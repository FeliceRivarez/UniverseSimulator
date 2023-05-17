
#include "mainwindow.h"
#include "titlepage.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    titlepage t;
    t.show();
    return a.exec();
}
