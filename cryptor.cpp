#include "cryptor.h"
#include <polarssl/rsa.h>
#include <polarssl/aes.h>
#include <polarssl/sha256.h>
#include <polarssl/sha512.h>
#include <polarssl/entropy.h>
#include <polarssl/ctr_drbg.h>
#include <polarssl/x509.h>
#include <polarssl/compat-1.2.h>

#include <QFile>
#include <cstdlib>
#include <QElapsedTimer>
#include <QtCore>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

Cryptor::Cryptor(byte *key, byte *iv, QObject *parent) :
    QObject(parent),
    availableBytes(0),
    newBytes(0),
    precomp(new byte[SIZE]),
    next(precomp),
    tomake(precomp)
{
//    auto result = connect(&watcher, &QFutureWatcher<size_t>::finished, this, &Cryptor::doPrecomputed);
//    qDebug() << "Connection result:" << result;
    
    memcpy(counter.data, iv, BLOCK_SIZE);
    aes_setkey_enc(&ctx, key, KEY_SIZE * 8);

    availableBytes += precompute(PREPARE_BLOCK_SIZE);
}

Cryptor::~Cryptor() {
    availableBytes = SIZE; // force precomputation to stop
//    watcher.waitForFinished();
    precompResult.waitForFinished();
    memset(precomp, 0, SIZE);
    memset(counter.data, 0, BLOCK_SIZE);
//    aes_free(&ctx);
    delete[] precomp;
}

size_t Cryptor::process(byte *in, byte *out, size_t size) {
    qDebug() << "Cryptor::process: size =" << size;
    
    size_t processedBytes = qMin(size, availableBytes);

    for (size_t i = 0; i < processedBytes; i++) {
        out[i] = in[i] ^ *next;
        next++;
        if (next == precomp + SIZE) {
            next = precomp;
        }
    }
    
    availableBytes -= processedBytes;

    precomputeCheck(PREPARE_BLOCK_SIZE);
    return processedBytes; // amount of processed data
}

void Cryptor::makeBlock() {
    aes_crypt_ecb(&ctx, AES_ENCRYPT, counter.data, tomake);
    tomake += BLOCK_SIZE;
    if (tomake == precomp + SIZE) {
        tomake = precomp;
    }
    counter++;
}

size_t Cryptor::precompute(size_t num) {
//    qDebug() << "Cryptor::precompute: to prepare =" << num << "bytes";
//    qDebug() << "Cryptor::precompute: available bytes =" << availableBytes << "bytes";
    for (size_t i = 0; i < num / BLOCK_SIZE; i++) {
        makeBlock();
    }
    newBytes = num;
    return num;
}

void Cryptor::precomputeCheck(size_t wantedBytes) {
    qDebug() << "Cryptor::precomputeCheck";
    if (precompResult.isFinished() && newBytes > 0) {
        availableBytes += newBytes;
        newBytes = 0;
    }
    if (availableBytes < THRESHOLD && !precompResult.isRunning()) {
//        watcher.waitForFinished();
        qDebug() << "Cryptor::precomputeCheck starting precomputation";
        precompResult = QtConcurrent::run(this, &Cryptor::precompute, wantedBytes);
//        watcher.setFuture(precompResult);
    }
}

//void Cryptor::doPrecomputed() {
//    qDebug() << "Cryptor::precomputed result =" << precompResult.result(); 
    
//    availableBytes += watcher.result();
//    precomputeCheck(PREPARE_BLOCK_SIZE);
//}

void Cryptor::benchmark(size_t megabytes) {
    // random memory values will be used
    byte key[16];
    byte iv[16];
    QElapsedTimer timer;
    const int PACKET_SIZE = 65536;
    
    timer.start();
    Cryptor enc(key, iv);
    qDebug() << "Encryptor create time:" << timer.restart() << "ms";
    
    Cryptor dec(key, iv);
//    qDebug() << "Decryptor create time:" << timer.restart() << "ms";
    
    byte *message = new byte[PACKET_SIZE];
    byte *encrypted = new byte[PACKET_SIZE];
    byte *decrypted = new byte[PACKET_SIZE];
    
    size_t length = (1 << 20) * megabytes;
    
    timer.restart();
    size_t processed = 0;
    while (processed < length) {
        size_t current = enc.process(message, encrypted, PACKET_SIZE);
        if (current < PACKET_SIZE) {
            break;
        }
//        if (dec.process(encrypted, decrypted, PACKET_SIZE) < PACKET_SIZE) {
//            break;
//        }
//        if (memcmp(message, decrypted, PACKET_SIZE) != 0) {
//            qDebug() << "Decryption failed!";
//            break;
//        }
        processed += current;
    }
    if (processed == length) {
        qDebug() << "All data successfully processed";
    } else {
        qDebug() << "Cryptor failed to process" << megabytes << "MB";
    }
    qDebug() << "Processed:" << processed << "bytes in" << timer.elapsed() << "ms";
    qDebug() << "Encryption + decryption speed:" << (static_cast<double>(processed) / timer.elapsed()) / 1000 << "MB/s";
    
    delete message;
    delete encrypted;
    delete decrypted;
}

/**
 * @brief Cryptor::makeKey
 * @param data1 200 bytes
 * @param data2 200 bytes
 * @return 
 */
QByteArray Cryptor::makeKey(QByteArray data1, QByteArray data2) {
    size_t bytes = (200 * 2) / 8;
    byte toHash[bytes];
    byte result[384 / 8];
    
    memcpy(toHash, data1.constData(), 200 / 8);
    memcpy(toHash + 200 / 8, data2.constData(), 200 / 8);
    
    sha512(toHash, bytes, result, 1);
    
    return QByteArray((char*) result, 384 / 8);
}

/**
 * @brief Cryptor::makeHmac
 * @param in
 * @param key 16 bytes
 * @return 
 */
QByteArray Cryptor::makeHmac(QByteArray in, QByteArray key) {
    byte out[256 / 8];
    sha256_hmac((byte*) key.constData(), 16, (byte*) in.constData(), in.size(), out, 0);
    
    return QByteArray((char*) out, 256 / 8);
}

// just a quick solution from https://polarssl.org/kb/how-to/generate-an-aes-key
QByteArray Cryptor::generateRandom(size_t size) {
    ctr_drbg_context ctr_drbg;
    entropy_context entropy;
    unsigned char *data = new byte[size];
    
    char *init_string = "aes_generate_key";
    int ret;
    
    entropy_init( &entropy );
    if( ( ret = ctr_drbg_init( &ctr_drbg, entropy_func, &entropy,
        (unsigned char *) init_string, strlen( init_string ) ) ) != 0 )
    {
        qDebug() << "Random generation init failed!";
//        printf( " failed\n ! ctr_drbg_init returned -0x%04x\n", -ret );
//        ctr_drbg_free(&ctr_drbg);
        return QByteArray();
    }
    
    if( ( ret = ctr_drbg_random( &ctr_drbg, data, size ) ) != 0 )
    {
        qDebug() << "Random generation failed!";
//        printf( " failed\n ! ctr_drbg_random returned -0x%04x\n", -ret );
//        ctr_drbg_free(&ctr_drbg);
        return QByteArray();
    }
    
//    ctr_drbg_free(&ctr_drbg);
    return QByteArray((char*) data, size);
}

// https://polarssl.org/kb/how-to/encrypt-and-decrypt-with-rsa
/**
 * @brief encryptRSA
 * @param data max 245 bytes
 * @param pubKey
 * @return 
 */
QByteArray Cryptor::encryptRSA(QByteArray data, QByteArray pubKey) {
    ctr_drbg_context ctr_drbg;
    entropy_context entropy;
    
    entropy_init( &entropy );
    ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                  (unsigned char *) "hello", strlen("hello") );
    
    rsa_context ctx;
    byte out[2048 / 8];
    rsa_init(&ctx, RSA_PKCS_V15, 0 /*ignored*/);
    qDebug() << "Public key parsing result:" << x509parse_public_key(&ctx, (const byte*) pubKey.constData(), pubKey.size());
    qDebug() << "Encryption result:" << rsa_pkcs1_encrypt(&ctx, ctr_drbg_random, &ctr_drbg, RSA_PUBLIC, 
                                  data.size(), (const byte*) data.constData(), out);
    
    return QByteArray((char*) out, 2048 / 8);
}

/**
 * @brief Cryptor::decryptRSA
 * @param data 256 bytes
 * @param keyfile PEM encoded
 * @return 
 */
QByteArray Cryptor::decryptRSA(QByteArray data, QString keyfile) {
    ctr_drbg_context ctr_drbg;
    entropy_context entropy;
    
    entropy_init( &entropy );
    ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                  (unsigned char *) "hello", strlen("hello") );
    rsa_context ctx;
    size_t olen = 0;
    byte dec[2048 / 8];
    
    rsa_init(&ctx, RSA_PKCS_V15, 0 /*ignored*/);
    qDebug() << "Private key parsing result:" << x509parse_keyfile(&ctx, keyfile.toStdString().c_str(), NULL);
    
    qDebug() << "Decryption result:" << rsa_pkcs1_decrypt(&ctx, ctr_drbg_random, &ctr_drbg, RSA_PRIVATE, 
                                                          &olen, (const byte*) data.constData(), dec, 2048 / 8);
    
    return QByteArray((char*) dec, olen);
}
