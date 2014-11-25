#include "mainwindow.h"
#include "loginscreen.h"
#include "tests.h"
#include <QApplication>
#include <QtTest/QtTest>

//#define TEST

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef TEST
    Tests t;
    QTest::qExec(&t);
#else
    LoginScreen l;
    l.show();
    /*MainWindow w;
    w.show();*/
#endif
    return a.exec();
}
