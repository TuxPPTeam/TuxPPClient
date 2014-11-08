#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#define SERVER_PORTNO 1234

#include <QByteArray>
#include <QTcpServer>
#include <QSslSocket>
#include <vector>
#include "user.h"

class ServerSession
{
    std::string id;
    std::vector<User> users;
    QSslSocket server;

    //rsa_context privateKey;

public:
    ServerSession();
    ~ServerSession();

    //bool isConnected() { return server->listen(QHostAddress::LocalHost, SERVER_PORTNO); }

    QByteArray sendRequest(User user);

    void createUserConnection(User user, QByteArray challenge/*, pk_context key*/);

    void sendData(User user, QByteArray data);

    //std::vector<pk_context> getPublicKeys();
};

#endif // SERVERSESSION_H
