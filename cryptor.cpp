#include "cryptor.h"
#include "polarssl/config.h"
#include "polarssl/sha512.h"
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/pk.h"

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

    availableBytes += precompute(SIZE);
}

Cryptor::~Cryptor() {
    availableBytes = SIZE; // force precomputation to stop
//    watcher.waitForFinished();
    precompResult.waitForFinished();
    memset(precomp, 0, SIZE);
    memset(counter.data, 0, BLOCK_SIZE);
    aes_free(&ctx);
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
    byte out[512 / 8];
    sha512_hmac((byte*) key.constData(), 16, (byte*) in.constData(), in.size(), out, 1);
    
    return QByteArray((char*) out, 512 / 8);
}

/*
 * ALL THE CODE BELOW IS HACKED SO BADLY...
 */

// just a quick solution from https://polarssl.org/kb/how-to/generate-an-aes-key
QByteArray generateRandom(size_t size) {
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
        ctr_drbg_free(&ctr_drbg);
        return QByteArray();
    }
    
    if( ( ret = ctr_drbg_random( &ctr_drbg, data, size ) ) != 0 )
    {
        qDebug() << "Random generation failed!";
//        printf( " failed\n ! ctr_drbg_random returned -0x%04x\n", -ret );
        ctr_drbg_free(&ctr_drbg);
        return QByteArray();
    }
    
    ctr_drbg_free(&ctr_drbg);
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
    QFile pkfile("tmp.pub");
    pkfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    pkfile.write(pubKey);
    pkfile.close();
    
    int ret = 0;
    pk_context pk;
    ctr_drbg_context ctr_drbg;
    entropy_context entropy;
    
    entropy_init( &entropy );
    ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                  (unsigned char *) "hello", strlen("hello") );

    pk_init( &pk );

    /*
     * Read the RSA public key
     */
    if( ( ret = pk_parse_public_keyfile( &pk, "tmp.pub" ) ) != 0 )
    {
        qDebug() << "encryptRSA Reading public key failed!";
//        printf( " failed\n  ! pk_parse_public_keyfile returned -0x%04x\n", -ret );
        return QByteArray();
    }
    
    unsigned char buf[POLARSSL_MPI_MAX_SIZE];
    size_t olen = 0;

    /*
     * Calculate the RSA encryption of the data.
     */
    qDebug() << "Calculating encryption";
//    fflush( stdout );

    if( ( ret = pk_encrypt( &pk, (const unsigned char*) data.constData(), data.size(),
                            buf, &olen, sizeof(buf),
                            ctr_drbg_random, &ctr_drbg ) ) != 0 )
    {
        qDebug() << "Encryption failed!";
//        printf( " failed\n  ! pk_encrypt returned -0x%04x\n", -ret );
        return QByteArray();
    }
    
    ctr_drbg_free(&ctr_drbg);
    entropy_free(&entropy);
    pkfile.remove();
    pk_free(&pk);
    
    return QByteArray((char*) buf, olen);
}

// https://polarssl.org/kb/how-to/encrypt-and-decrypt-with-rsa
QByteArray Cryptor::decryptRSA(QByteArray data, QString keyfile) {
    int ret = 0;
    pk_context pk;
    ctr_drbg_context ctr_drbg;
    entropy_context entropy;
    
    entropy_init( &entropy );
    ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                  (unsigned char *) "hello", strlen("hello") );

    pk_init( &pk );

    /*
     * Read the RSA privatekey
     */
    if( ( ret = pk_parse_keyfile( &pk, keyfile.toStdString().c_str(), "" ) ) != 0 )
    {
        qDebug() << "decryptRSA Reading private key failed!";
//        printf( " failed\n  ! pk_parse_keyfile returned -0x%04x\n", -ret );
        return QByteArray();
    }
    
    unsigned char result[POLARSSL_MPI_MAX_SIZE];
    size_t olen = 0;

    /*
     * Calculate the RSA encryption of the data.
     */
//    printf( "\n  . Generating the encrypted value" );
//    fflush( stdout );

    if( ( ret = pk_decrypt( &pk, (const unsigned char*) data.constData(), data.size(), result, &olen, sizeof(result),
                            ctr_drbg_random, &ctr_drbg ) ) != 0 )
    {
        qDebug() << "Decryption failed!";
//        printf( " failed\n  ! pk_decrypt returned -0x%04x\n", -ret );
//        goto exit;
    }
    
    ctr_drbg_free(&ctr_drbg);
    entropy_free(&entropy);
    pk_free(&pk);
    
    return QByteArray((char*) result, olen);
}
