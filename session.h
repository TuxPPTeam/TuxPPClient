#ifndef SESSION_H
#define SESSION_H

static const int PORT = 1234;

#include <QByteArray>
#include <QTcpServer>
#include <QSslSocket>
#include <vector>
#include "user.h"

class Session
{
    std::string id;
    std::vector<User> users;
    QSslSocket server;

    //rsa_context privateKey;

public:
    Session();
    ~Session();

    //bool isConnected() { return server->listen(QHostAddress::LocalHost, SERVER_PORTNO); }

    QByteArray sendRequest(User user);

    void createUserConnection(User user, QByteArray challenge/*, pk_context key*/);

    void sendData(User user, QByteArray data);

    //std::vector<pk_context> getPublicKeys();
};

#endif // SESSION_H
