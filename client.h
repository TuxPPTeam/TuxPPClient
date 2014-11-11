#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

#include <QObject>
#include <QTcpSocket>

static const int PORT = 1234;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    //Session(QString login, QString keyFileName);
    void sendRequest(QString request);
    void createUserConnection(User user, QByteArray challenge/*, pk_context key*/);
    void sendData(User user, QByteArray data);
    bool isConnected();
    void connect();
    void setLogin(QString newLogin);
    QString getLogin();
    void setKeyFileName(QString newKeyFileName);
    QString getKeyFileName();
    void setReady(bool isReady) { ready = isReady; }

private:
    QString login;
    QString keyFile;
    bool ready;
    QList<User> users;
    QTcpSocket *server;
    enum command { ECHO, LOGIN, LOGOUT };

signals:

public slots:

};

#endif // CLIENT_H
