#ifndef TESTS_H
#define TESTS_H

#include <QtTest/QtTest>
#include "client.h"

class Tests: public QObject
{
    Q_OBJECT
private slots:
    void testLogin();
    void testKey();
    void testServerConnection();
};

#endif // TESTS_H
