#include "session.h"

Session::Session(QString login, QString keyFileName)
{
    server = new QTcpSocket();
    server->connectToHost(QHostAddress::LocalHost, PORT);
    qDebug() << server->state();
}

Session::~Session()
{
    server->flush();
    server->close();
    delete server;
}

void Session::sendRequest(QString request) {
    QString message = "Hello||";
//    server->waitForConnected();
    qDebug() << "Server status:" << server->state();
    int written = server->write(message.toLocal8Bit());
    qDebug() << written;
}

void Session::sendData(User user, QByteArray data) {
    QTcpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, PORT);
    sock.waitForConnected();
    sock.write(data);
}

bool Session::isConnected() {
    return server->state() == QTcpSocket::SocketState::ConnectedState;
}

void Session::connect() {
    server->connectToHost(QHostAddress::LocalHost, PORT);
    server->waitForConnected();
}
