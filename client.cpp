#include "client.h"

#include <QUdpSocket>
#include <QTcpServer>
#include <QMessageBox>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QTcpSocket(this)),
    partner(new QUdpSocket(this))
{
    connect(partner, SIGNAL(connected()), this, SLOT(clientConnectionEstablished()));
}

Client::~Client() {
    sendRequest(LOGOUT, username);
}

void Client::sendRequest(Command cmd, QString request) {
    qDebug() << "Client::sendRequest()";
    if (!isServerConnected()) {
        qDebug() << "Cannot send data, disconnected from server.";
        return;
    }

    QByteArray message(request.toLocal8Bit());
    message.prepend((char*)&cmd, 1);
    qDebug() << "Sending: " << request;
    int written = server->write(message);
    server->waitForBytesWritten();
    qDebug() << "Written: " << written << " bytes.";
}

void Client::serverReadyRead() {
    qDebug() << "Client::readyRead()";
    QByteArray data = server->readAll();
    if (data.isEmpty()) {
        qDebug("readyRead(): no data received");
        return;
    }

    switch (data.at(0))
    {
        //case ECHO:      echo(data.mid(sizeof(Command)), socket); break;
        case LOGIN:     login(data.mid(1)); break;
        //case LOGOUT:    logout(data.mid(sizeof(Command)), socket); break;
        case REGISTER:  registerUser(data.mid(1)); break;
        case GETUSERS:  getUserList(data.mid(1)); break;
        default:        emit dataRecieved(data);
    }
}

void Client::login(QByteArray data) {
    qDebug() << "Client::login()";
    if (data.length() < 1) {
        emit displayMsg("Login", "Unknown response from server.");
        return;
    }

    if (data[0] == '\1') {
        emit loginSuccessful();
    }
    else {
        emit displayMsg("Login", data.mid(1));
    }
}

void Client::registerUser(QByteArray data) {
    qDebug() << "Client::registerUser(): data size =" << data.size();
    if (data.length() < 1) {
        emit displayMsg("Registration", "Unknown response from server.");
        return;
    }
    
    if (data[0] == '\1') {
        qDebug("Successful registration");
        emit displayMsg("Registration", data.mid(1));
    } else {
        qDebug("Registration failed");
        emit displayMsg("Registration", data.mid(1));
    }
}

void Client::getUserList(QByteArray data) {
    qDebug() << "Client::getUserList()";
    QList<QByteArray> tokens = data.split(commandDelimiter);

    users.clear();
    for (int i = 0; i < tokens.count() / 3; ++i) {
        User *u = new User(this, tokens[i*3], tokens[i*3+1], QHostAddress(QString(tokens[i*3+2])));
        users.append(u);
    }

    emit userListRecieved(users);
}

bool Client::sendData(QByteArray data) {
    bool result = false;
    if (partner->isWritable()) {
        result = partner->write(data);
    }

    return result;
}

bool Client::isServerConnected() {
    qDebug() << "Client::isServerConnected()";
    return server->isOpen();
}

bool Client::isClientConnected() {
    qDebug() << "Client::isClientConnected()";
    return partner->isOpen();
}

bool Client::connectToServer() {
    qDebug() << "Client::connectToServer()";
    server->connectToHost(serverAddress, SERVER_PORT);
    bool res = server->waitForConnected();
    if (res) {
        connect(server, SIGNAL(disconnected()), this, SIGNAL(serverDisconnected()));
        connect(server, SIGNAL(readyRead()), this, SLOT(serverReadyRead()));
    }
    return res;
}

bool Client::createUserConnection(User *user) {
    partner->connectToHost(user->getHost(), CLIENT_PORT);
    return partner->waitForConnected();
}

void Client::clientReadyRead() {
    while (partner->hasPendingDatagrams()) {
        emit dataRecieved(partner->readAll());
    }
}

void Client::clientConnectionEstablished() {
    connect(partner, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
}

void Client::setLogin(QString newLogin) {
    qDebug() << "Client::setLogin():" << newLogin;
    username = newLogin;
}

void Client::setKeyFileName(QString newKeyFileName) {
    qDebug() << "Client::setKeyFileName():" << newKeyFileName;
    keyFile = newKeyFileName;
}

QString Client::getLogin() {
    qDebug("Client::getLogin()");
    return username;
}

QString Client::getKeyFileName() {
    qDebug("Client::getKeyFileName()");
    return keyFile;
}

int Client::getUsersCount() {
    return users.count();
}
