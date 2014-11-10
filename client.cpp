#include "client.h"

#include <QTcpSocket>
#include <QTcpServer>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QTcpSocket(this))
{}

void Client::sendRequest(QString request) {
    QString message = "Hello||";
//    server->waitForConnected();
    qDebug() << "Server status:" << server->state();
    int written = server->write(message.toLocal8Bit());
    qDebug() << written;
}

void Client::sendData(User user, QByteArray data) {
    QTcpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, PORT);
    sock.waitForConnected();
    sock.write(data);
}

bool Client::isConnected() {
    return server->state() == QTcpSocket::SocketState::ConnectedState;
}

void Client::connect() {
    server->connectToHost(QHostAddress::LocalHost, PORT);
    server->waitForConnected();
    qDebug() << "client connected to the server";
}

void Client::setLogin(QString newLogin) {
    login = newLogin;
}

void Client::setKeyFileName(QString newKeyFileName) {
    keyFile = newKeyFileName;
}
