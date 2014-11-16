#include "mainwindow.h"
#include "loginscreen.h"
#include "tests.h"
#include <QApplication>
#include <QtTest/QtTest>

int main(int argc, char *argv[])
{
#ifdef TEST
    Tests t;
    QTest::qExec(&t);

    return 0;
#else
    QApplication a(argc, argv);
    LoginScreen l;
    l.show();
    /*MainWindow w;
    w.show();*/

    return a.exec();
#endif
}
