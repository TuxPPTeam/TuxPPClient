#ifndef CLIENT_H
#define CLIENT_H

#include "user.h"

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

static const int PORT = 1234;
static const QHostAddress serverAddress = QHostAddress::LocalHostIPv6;
static const char commandDelimiter = '\31';

enum Command { ECHO, LOGIN, LOGOUT, REGISTER, GETUSERS };

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    //Session(QString login, QString keyFileName);
    void sendRequest(Command cmd, QString request);
    void createUserConnection(User user, QByteArray challenge/*, pk_context key*/);
    void sendData(User user, QByteArray data);
    bool isConnected();
    bool connectToServer();
    void setLogin(QString newLogin);
    QString getLogin();
    void setKeyFileName(QString newKeyFileName);
    QString getKeyFileName();
    void setReady(bool isReady) { ready = isReady; }

private:
    QString username;
    QString keyFile;
    bool ready;
    QList<User*> users;
    QTcpSocket *server;

    void login(QByteArray);
    void getUserList(QByteArray);
    void registerUser(QByteArray);

signals:
    void dataRecieved(QByteArray);
    void userListRecieved(QList<User*>);
    void serverDisconnected();
    void displayMsg(QString, QString);
    void loginSuccessful();

public slots:

private slots:
    void readyRead();
};

#endif // CLIENT_H
