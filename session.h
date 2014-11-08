#ifndef SESSION_H
#define SESSION_H

static const int PORT = 1234;

#include <QByteArray>
#include <QTcpServer>
#include <QSslSocket>
#include <vector>
#include "user.h"

class Session {
    
    QString login;
    QList<User> users;
    QTcpSocket *server;

    //rsa_context privateKey;

public:
    Session(QString login, QString keyFileName);
    ~Session();

    //bool isConnected() { return server->listen(QHostAddress::LocalHost, SERVER_PORTNO); }

    void sendRequest(QString request);

    void createUserConnection(User user, QByteArray challenge/*, pk_context key*/);

    void sendData(User user, QByteArray data);
    
    bool isConnected();
    
    void connect();

    //std::vector<pk_context> getPublicKeys();
};

#endif // SESSION_H
