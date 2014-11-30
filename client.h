#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"
#include "cryptor.h"

#include <QObject>
#include <QSslSocket>
#include <QUdpSocket>
#include <QHostAddress>
//#include <QtNetwork>

static const int SERVER_PORT = 1234;
static const int CLIENT_PORT = 4321;
//static const QHostAddress serverAddress = QHostAddress::LocalHostIPv6;
static const QString serverAddress = QString("::1");
//static const QHostAddress serverAddress("147.251.44.155");
static const char commandDelimiter = '\31';

enum Command { ECHO, LOGIN, LOGOUT, REGISTER, GETUSERS, GENKEY };

class Client : public QObject
{
    Q_OBJECT
    friend class Tests;
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    void sendRequest(Command cmd, QString request);
    bool connectToUSer(User *user);
    void closeUserConnection();
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

    void setSsl();
private:
    qint64 myID;
    Cryptor *cryptor;
    QString username;
    QString keyFile;
    bool ready;
    QList<User*> users;
    QSocket *socket;
    QUdpSocket *partnerSocket;
    User *partner;
    QByteArray lastMessage;
    QByteArray halfKey;

    void login(QByteArray);
    void getUserList(QByteArray);
    void registerUser(QByteArray);

    bool createUserConnection(QByteArray data);

signals:
    void dataRecieved(QByteArray);
    void userListRecieved(QList<User*>);
    void serverDisconnected();
    void displayMsg(QString, QString);
    void loginSuccessful();

public slots:

private slots:
    void errorOccured(QList<QSslError>);
    void serverReadyRead();
    void clientReadyRead();
    void socketReady();
};

#endif // CLIENT_H
