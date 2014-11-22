#ifndef USER_H
#define USER_H

#include <QObject>
#include <QSslSocket>
#include <QHostAddress>

/*#ifdef ENCRYPTED
    typedef QSslSocket QSocket;
#else
    typedef QTcpSocket QHostAddress;
#endif*/

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0, QString userName = NULL, QByteArray pubKey = NULL, QHostAddress *host = NULL);
    explicit User(QObject *parent = 0, qint64 = 0, QString userName = NULL, QByteArray pubKey = NULL, QHostAddress *host = NULL);

    qint64 getID() const;
    QString getUsername() const;
    QByteArray getPubKey() const;
    QHostAddress* getSocket() const;

    bool operator==(const User &) const;

signals:

public slots:
    bool setID(const qint64);
    bool setUsername(const QString);
    void setPubKey(const QByteArray);
    void setSocket(QHostAddress *);

private:
    qint64 ID;
    QString username;
    QByteArray pubKey;
    QHostAddress* host;

    static const unsigned int MAX_USERNAME_LENGTH = 32;
};

#endif // USER_H
