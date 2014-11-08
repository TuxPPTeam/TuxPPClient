#include "serversession.h"

ServerSession::ServerSession()
{
    server.connectToHost(QHostAddress::LocalHost, SERVER_PORTNO);
    qDebug() << server.state();
}

ServerSession::~ServerSession()
{
    server.flush();
    server.close();
}

QByteArray ServerSession::sendRequest(User user) {
    QString message = "Hello||";
    server.waitForConnected();
    int written = server.write(message.toLocal8Bit());
    qDebug() << written;
}

void ServerSession::sendData(User user, QByteArray data) {
    QTcpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, SERVER_PORTNO);
    sock.waitForConnected();
    sock.write(data);
}
