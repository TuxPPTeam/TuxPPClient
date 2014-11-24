#ifndef CRYPTOR_H
#define CRYPTOR_H

#include "polarssl/aes.h"
#include <QObject>
#include <QFuture>

typedef unsigned char byte;

class Cryptor : public QObject
{
    Q_OBJECT
public:
    explicit Cryptor(byte *key, byte *iv, QObject *parent = 0);
    ~Cryptor();
    size_t process(byte *in, byte *out, size_t size);
    static void benchmark(size_t megabytes);
    
private:
    static const size_t SIZE = (1 << 20) * 100;
    static const size_t BLOCK = SIZE / 8;
    static const size_t THRESHOLD = SIZE - BLOCK;
    static const size_t PREPARE_BLOCK_SIZE = BLOCK / 2; // amount of bytes to precompute
    static const int BLOCK_SIZE = 16;
    static const int KEY_SIZE = 16;
    size_t availableBytes;
    size_t newBytes;
    
    QFuture<size_t> precompResult;
//    QFutureWatcher<size_t> watcher;
    
    byte *precomp;
    aes_context ctx;
    byte *next;
    byte *tomake;
    
    union _counter {
        byte data[BLOCK_SIZE];
        quint64 halves[2];

        void operator++(int) {
            halves[0]++;
            if (halves[0] == 0) {
                halves[1]++;
            }
        }
    } counter;

    void makeBlock();
    void precomputeCheck(size_t wantedBytes);
    size_t precompute(size_t num);
    
signals:
    
public slots:
    
//private slots:
//    void doPrecomputed();
};

#endif // CRYPTOR_H
