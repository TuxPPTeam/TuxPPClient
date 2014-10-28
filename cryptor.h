#ifndef CRYPTOR_H
#define CRYPTOR_H

#include <QByteArray>

class Cryptor
{
public:
    Cryptor();
    static void encryptRSA(const QByteArray& in, QByteArray& out, rsa_context *key);
    static void decryptRSA(const QByteArray& in, QByteArray& out, rsa_context *key);
    static void encryptAES(const QByteArray& in, QByteArray& out, QByteArray iv, aes_context *key);
    static void decryptAES(const QByteArray& in, QByteArray& out, QByteArray iv, aes_context *key);
    void precomputeAES();
};

#endif // CRYPTOR_H
