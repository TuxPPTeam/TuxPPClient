#include "client.h"

#include <QUdpSocket>
#include <QTcpServer>
#include <QMessageBox>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QTcpSocket(this))
{}

Client::~Client() {}

void Client::sendRequest(Command cmd, QString request) {
    qDebug() << "Client::sendRequest()";
    if (!isConnected()) {
        qDebug() << "Cannot send data, disconnected from server.";
        return;
    }

    QByteArray message(request.toLocal8Bit());
    message.prepend((char*)&cmd, sizeof(char*));
    qDebug() << "Sending: " << request;
    int written = server->write(message);    
    qDebug() << "Written: " << written << " bytes.";

    /*server->waitForReadyRead();
    QByteArray received = server->readAll();
    emit dataRecieved(received);
    qDebug() << "Received: " << received;*/
}

void Client::readyRead() {
    qDebug() << "Client::readyRead()";
    QByteArray data = server->readAll();
    if (data.isEmpty()) {
        qDebug("readyRead(): no data received");
        return;
    }

    switch (data.at(0))
    {
        //case ECHO:      echo(data.mid(sizeof(Command)), socket); break;
        //case LOGIN:     login(data.mid(sizeof(Command)), socket); break;
        //case LOGOUT:    logout(data.mid(sizeof(Command)), socket); break;
        case REGISTER:  registerUser(data.mid(sizeof(Command))); break;
        case GETUSERS:  getUserList(data.mid(sizeof(char*))); break;
        default:        emit dataRecieved(data);
    }
}

void Client::registerUser(QByteArray data) {
    qDebug() << "Client::registerUser(): data size =" << data.size();
    
    QString message = QString::fromLocal8Bit(data);
    
    QMessageBox mbox;
    
    if (message == "success") {
        qDebug("Successful registration");
        mbox.setText("Registration successful");
    } else {
        qDebug("Registration failed");
        mbox.setText("Error: " + message);
    }
    mbox.exec();
}

void Client::getUserList(QByteArray data) {
    qDebug() << "Client::getUserList()";
    QList<QByteArray> tokens = data.split(commandDelimiter);

    users.clear();
    for (int i = 0; i < tokens.count() / 2; ++i) {
        User *u = new User(this, tokens[i*2], NULL, new QHostAddress(QString(tokens[i*2+1])));
        users.append(u);
    }

    emit userListRecieved(users);
}

void Client::sendData(User user, QByteArray data) {
    qDebug() << "Client::sendData()";
    QUdpSocket sock;
    sock.connectToHost(QHostAddress::LocalHost, PORT);
    sock.waitForConnected();
    sock.write(data);
}

bool Client::isConnected() {
    qDebug() << "Client::isConnected()";
    return server->state() == QTcpSocket::SocketState::ConnectedState;
}

bool Client::connectToServer() {
    qDebug() << "Client::connectToServer()";
    server->connectToHost(serverAddress, PORT);
    bool res = server->waitForConnected();
    if (res) {
        connect(server, SIGNAL(disconnected()), this, SIGNAL(serverDisconnected()));
        connect(server, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    return res;
}

void Client::setLogin(QString newLogin) {
    qDebug() << "Client::setLogin():" << newLogin;
    login = newLogin;
}

void Client::setKeyFileName(QString newKeyFileName) {
    qDebug() << "Client::setKeyFileName():" << newKeyFileName;
    keyFile = newKeyFileName;
}

QString Client::getLogin() {
    qDebug("Client::getLogin()");
    return login;
}

QString Client::getKeyFileName() {
    qDebug("Client::getKeyFileName()");
    return keyFile;
}
