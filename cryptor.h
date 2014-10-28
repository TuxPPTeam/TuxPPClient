#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <QByteArray>

class Cryptor
{
public:
    Cryptor();
    static void encryptRSA(QByteArray in, QByteArray out, rsa_context *key);
    static void decryptRSA(QByteArray in, QByteArray out, rsa_context *key);
    static void encryptAES(QByteArray in, QByteArray out, QByteArray iv, aes_context *key);
    static void decryptAES(QByteArray in, QByteArray out, QByteArray iv, aes_context *key);
    void precomputeAES();
};

#endif // CRYPTOR_H
