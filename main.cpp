#include "mainwindow.h"
#include "tests.h"
#include <QApplication>
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Tests t;
    QTest::qExec(&t);
    
    return a.exec();
}
