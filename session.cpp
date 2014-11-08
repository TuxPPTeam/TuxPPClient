#include "session.h"

Session::Session()
{
    server.connectToHost(QHostAddress::LocalHost, PORT);
    qDebug() << server.state();
}

Session::~Session()
{
    server.flush();
    server.close();
}

QByteArray Session::sendRequest(User user) {
    QString message = "Hello||";
    server.waitForConnected();
    int written = server.write(message.toLocal8Bit());
    qDebug() << written;
}

void Session::sendData(User user, QByteArray data) {
    QTcpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, PORT);
    sock.waitForConnected();
    sock.write(data);
}
