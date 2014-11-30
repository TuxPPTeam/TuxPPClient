#include "cryptor.h"
#include <cstdlib>
#include <cstring>
#include <QElapsedTimer>
#include <QtCore>
#include <QtConcurrent/QtConcurrent>

Cryptor::Cryptor(QObject *parent) :
    QObject(parent)
{
}

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
    //qDebug() << "Cryptor::process: size =" << size;
    
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
    //qDebug() << "Cryptor::precomputeCheck";
    if (precompResult.isFinished() && newBytes > 0) {
        availableBytes += newBytes;
        newBytes = 0;
    }
    if (availableBytes < THRESHOLD && !precompResult.isRunning()) {
//        watcher.waitForFinished();
        //qDebug() << "Cryptor::precomputeCheck starting precomputation";
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
        if (dec.process(encrypted, decrypted, PACKET_SIZE) < PACKET_SIZE) {
            break;
        }
        if (memcmp(message, decrypted, PACKET_SIZE) != 0) {
            qDebug() << "Decryption failed!";
            break;
        }
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

QByteArray Cryptor::generateRandom(size_t size) {
    QByteArray result;
    for (unsigned i = 0; i < size; ++i) {
        result.append('a'+i);
    }
    return result;
}

QByteArray Cryptor::makeKey(QByteArray data1, QByteArray data2) {
    return data1.append(data2);
}

QByteArray Cryptor::encryptRSA(QByteArray input) {
    return input;
}

QByteArray Cryptor::decryptRSA(QByteArray input) {
    return input;
}
