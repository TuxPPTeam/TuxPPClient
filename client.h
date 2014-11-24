#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

#include <QObject>
#include <QSslSocket>
#include <QUdpSocket>
#include <QHostAddress>

static const int SERVER_PORT = 1234;
static const int CLIENT_PORT = 4321;
static const QHostAddress serverAddress = QHostAddress::LocalHostIPv6;
static const char commandDelimiter = '\31';

enum Command { ECHO, LOGIN, LOGOUT, REGISTER, GETUSERS };

class Client : public QObject
{
    Q_OBJECT
    friend class Tests;
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    void sendRequest(Command cmd, QString request);
    bool createUserConnection(User *user);
    bool sendData(QByteArray data);
    bool isServerConnected();
    bool isClientConnected();
    bool connectToServer();
    void setLogin(QString newLogin);
    QString getLogin();
    void setKeyFileName(QString newKeyFileName);
    QString getKeyFileName();
    void setReady(bool isReady) { ready = isReady; }
    int getUsersCount();

private:
    QString username;
    QString keyFile;
    bool ready;
    QList<User*> users;
    QSocket *server;
    QUdpSocket *partner;

    void login(QByteArray);
    void getUserList(QByteArray);
    void registerUser(QByteArray);

signals:
    void dataRecieved(QByteArray);
    void messageReceived();
    void userListRecieved(QList<User*>);
    void serverDisconnected();
    void displayMsg(QString, QString);
    void loginSuccessful();

public slots:

private slots:
    void serverReadyRead();
    void clientReadyRead();
    void clientConnectionEstablished();
};

#endif // CLIENT_H
