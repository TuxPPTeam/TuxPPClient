#ifndef SERVERSESSION_H
#define SERVERSESSION_H

class User
{
    pk_context key; //public key
    std::string id;

public:
    std::string getId();
    pk_context getKey();
};

class ServerSession
{
    std::string id;
    std::Vector<User> users;
    QSslSocket server;

    rsa_context rsa_ctx;

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
