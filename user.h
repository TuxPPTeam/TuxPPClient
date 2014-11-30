#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSslKey>
#include <QSslSocket>
#include <QHostAddress>

#define ENCRYPTED

#ifdef ENCRYPTED
    typedef QSslSocket QSocket;
#else
    typedef QTcpSocket QSocket;
#endif

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0, QString userName = NULL, QByteArray pubKey = NULL, QHostAddress host = QHostAddress::Null);
    explicit User(QObject *parent = 0, qint64 = 0, QString userName = NULL, QByteArray pubKey = NULL, QHostAddress host = QHostAddress::Null);
    explicit User(QObject *parent = 0, qint64 = 0, QString userName = NULL, QByteArray pubKey = NULL, QSsl::KeyAlgorithm alg = QSsl::Rsa, QHostAddress host = QHostAddress::Null);
    

    qint64 getID() const;
    QString getUsername() const;
    QByteArray getPubKey() const;
    QHostAddress getHost() const;

    bool operator==(const User &) const;

signals:

public slots:
    bool setID(const qint64);
    bool setUsername(const QString);
    void setPubKey(const QByteArray);
    void setHost(QHostAddress);

private:
    qint64 ID;
    QString username;
    QByteArray pubKey;
    QSslKey key;
    QHostAddress host;

    static const unsigned int MAX_USERNAME_LENGTH = 32;
};

#endif // USER_H
