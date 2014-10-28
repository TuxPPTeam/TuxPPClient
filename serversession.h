#ifndef SERVERSESSION_H
#define SERVERSESSION_H

#include <QByteArray>
#include <QSslSocket>

class User
{
    rsa_context publicKey;
    std::string id;

public:
    QString getId();
    rsa_context* getKey();
};

class ServerSession
{
    std::string id;
    std::Vector<User> users;
    QObject::QSslSocket server;

    rsa_context privateKey;

public:
    ServerSession();
    ~ServerSession();

    bool isConnected();

    QByteArray sendRequest(User user);

    void createUserConnection(User user, QByteArray challenge, pk_context key);

    void sendData(User user, QByteArray data);

    Vector<pk_context> getPublicKeys();
};

#endif // SERVERSESSION_H
