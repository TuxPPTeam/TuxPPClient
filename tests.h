#ifndef TESTS_H
#define TESTS_H

#include <QtTest/QtTest>
#include "client.h"
#include "cryptor.h"

class Tests: public QObject
{
    Q_OBJECT
private slots:
    void testLogin();
    void testKey();
    void testServerConnection();
    void testServerEcho();
    //void testClientConnection();
    void testEncryption();
    void testDecryption();
    void testCryptoSpeed();
};

#endif // TESTS_H
