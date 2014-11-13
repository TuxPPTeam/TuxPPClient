#include "client.h"

#include <QUdpSocket>
#include <QTcpServer>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QTcpSocket(this))
{}

Client::~Client() {}

void Client::sendRequest(Command cmd, QString request) {
    if (!isConnected()) {
        qDebug() << "Cannot send data, disconnected from server.";
        return;
    }

    QByteArray message(request.toLocal8Bit());
    message.prepend((char*)&cmd, sizeof(Command));
    qDebug() << "Sending: " << request;
    int written = server->write(message);    
    qDebug() << "Written: " << written << " bytes.";

    server->waitForReadyRead();
    QByteArray received = server->readAll();
    emit dataRecieved(received);
    qDebug() << "Received: " << received;
}

void Client::sendData(User user, QByteArray data) {
    QUdpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, PORT);
    sock.waitForConnected();
    sock.write(data);
}

bool Client::isConnected() {
    return server->state() == QTcpSocket::SocketState::ConnectedState;
}

bool Client::connect() {
    server->connectToHost(serverAddress, PORT);
    return server->waitForConnected();
}

void Client::setLogin(QString newLogin) {
    login = newLogin;
}

void Client::setKeyFileName(QString newKeyFileName) {
    keyFile = newKeyFileName;
}

QString Client::getLogin() {
    return login;
}

QString Client::getKeyFileName() {
    return keyFile;
}
