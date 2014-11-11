#include "client.h"

#include <QTcpSocket>
#include <QTcpServer>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QTcpSocket(this))
{}

Client::~Client() {}

void Client::sendRequest(QString request) {
//    QString message = "Hello||";
////    server->waitForConnected();
//    qDebug() << "Server status:" << server->state();
//    int written = server->write(message.toLocal8Bit());
//    qDebug() << written;

    QByteArray message(request.toLocal8Bit());
//    QDataStream stream(message);
//    stream << ECHO;
//    stream << request;
    command echo = ECHO;
    message.prepend((char*) &echo, sizeof(command));
    int written = server->write(message);
    server->flush();
//    qDebug() << server->waitForBytesWritten();
    qDebug() << "Sending echo" << request;
    qDebug() << "Written:" << written;

    server->waitForReadyRead();
    QByteArray received = server->readAll();
    qDebug() << "Received:" << received;
//    qDebug() << "Message type:" << received.at(0);
//    qDebug() << "Message:" << received.mid(sizeof(command));
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
    server->connectToHost("147.251.46.146", PORT, QIODevice::ReadWrite);
    qDebug() << "Client connected:" << server->waitForConnected();
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
