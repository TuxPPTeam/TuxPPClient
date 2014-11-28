#include "client.h"

#include <QUdpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QSslConfiguration>

Client::Client(QObject *parent) :
    QObject(parent),
    ready(false),
    server(new QSslSocket(this)),
    partner(NULL),
    partnerSocket(NULL)
{
//    QSslCertificate cert = QSslCertificate::fromPath("ca.crt");
    
    QSslCertificate ca = QSslCertificate::fromPath("../certs/labak/ca.crt").first();
    server->addCaCertificate(ca);
    QSslCertificate servCert = QSslCertificate::fromPath("../certs/labak/server.crt").first();
//    server->setLocalCertificate("../certs/labak/server.crt");
//    server->setPrivateKey("../certs/labak/server.key");
    QList<QSslError> errors;
    QSslError error(QSslError::HostNameMismatch, servCert);
    errors.append(error);
    server->ignoreSslErrors(errors);
//    server->setLocalCertificate();
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
    lastMessage = data;

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
    int result;
    if (partnerSocket != NULL && partner != NULL) {
        result = partnerSocket->writeDatagram(data, data.length(), partner->getHost(), CLIENT_PORT);
    }
    return result > 0;
}

bool Client::isServerConnected() {
    qDebug() << "Client::isServerConnected()";
    return server->isOpen();
}

bool Client::isClientConnected() {
    qDebug() << "Client::isClientConnected()";
    return partnerSocket != NULL && partnerSocket->state() == QAbstractSocket::BoundState;
}

bool Client::connectToServer() {
    qDebug() << "Client::connectToServer()";
    server->connectToHostEncrypted(serverAddress, SERVER_PORT);
    bool res = server->waitForConnected();
    qDebug() << "Connection established:" << res;
    qDebug() << "Encryption:" << server->waitForEncrypted();
    qDebug() << "Error:" << server->error() << server->errorString();
    if (res) {
        connect(server, SIGNAL(disconnected()), this, SIGNAL(serverDisconnected()));
        connect(server, SIGNAL(readyRead()), this, SLOT(serverReadyRead()));
    }
    return res;
}

bool Client::createUserConnection(User *user) {
    partner = user;
    bool res;
    if (partnerSocket == NULL) {
        partnerSocket = new QUdpSocket(this);
        res = partnerSocket->bind(partner->getHost(), CLIENT_PORT);
        connect(partnerSocket, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
    }
    qDebug() << "Binding error: " << partnerSocket->errorString();

    return res;
}

void Client::closeUserConnection() {
    partner = NULL;
    partnerSocket->close();
    delete partnerSocket;
    partnerSocket = NULL;
}

void Client::clientReadyRead() {
    while (partnerSocket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(partnerSocket->pendingDatagramSize());
        partnerSocket->readDatagram(data.data(), partnerSocket->pendingDatagramSize());
        emit dataRecieved(data);
    }
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
