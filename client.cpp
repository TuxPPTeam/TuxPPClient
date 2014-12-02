#include "client.h"

#include <QUdpSocket>
#include <QTcpServer>
#include <QMessageBox>
#include <QSslConfiguration>
#include <fstream>
#include <iostream>
#include <QFile>


Client::Client(QObject *parent) :
    QObject(parent),
    //cryptor(),
    ready(false),
    socket(new QSocket(this)),
    partner(NULL),
    partnerSocket(NULL)
{
    
    setSsl();
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(errorOccured(QList<QSslError>)));
    connect(socket, SIGNAL(encrypted()), this, SLOT(socketReady()));
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
    int written = socket->write(message);
    socket->waitForBytesWritten();
    qDebug() << "Written: " << written << " bytes.";
}

void Client::serverReadyRead() {
    qDebug() << "Client::readyRead()";
    QByteArray data = socket->readAll();
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
        case GENKEY:    createUserConnection(data.mid(1)); break;
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
        myID = data.mid(1).toLong();
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
    for (int i = 0; i < tokens.count() / 5; ++i) {

        User *u = new User(this, tokens[i*5].toLong(), tokens[i*5+1], tokens[i*5+2], QSsl::Rsa, QHostAddress(QString(tokens[i*5+4])));
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
    return socket->isOpen();
}

bool Client::isClientConnected() {
    qDebug() << "Client::isClientConnected()";
    return partnerSocket != NULL && partnerSocket->state() == QAbstractSocket::BoundState;
}

bool Client::connectToServer() {
    qDebug() << "Client::connectToServer()";
    socket->connectToHostEncrypted(serverAddress.toString(), SERVER_PORT);

    bool res = socket->waitForEncrypted();
    qDebug() << "Connection established:" << socket->waitForConnected();
    qDebug() << "Encryption:" << res;
    qDebug() << "Error:" << socket->error() << socket->errorString();

    return res;
}

bool Client::connectToUSer(User *user) {
    partner = user;
    halfKey = Cryptor::generateRandom(200);
    QByteArray data = Cryptor::encryptRSA(halfKey, user->getPubKey());
    QByteArray myIdByte;
    QByteArray partnerIdByte;
    QByteArray message;
    message.append(myIdByte.setNum(myID))
           .append(commandDelimiter)
           .append(partnerIdByte.setNum(user->getID()))
           .append(commandDelimiter)
           .append(data);
    sendRequest(GENKEY, message);
}

bool Client::createUserConnection(QByteArray data) {
    if (partner != NULL)
        return false;

    QList<QByteArray> tokens = data.split(commandDelimiter);
    if (halfKey == NULL) {
        halfKey = Cryptor::generateRandom(200);
        QByteArray myIdByte;
        QByteArray message;
        message.append(myIdByte.setNum(myID))
                .append(commandDelimiter)
                .append(tokens[0])
                .append(commandDelimiter)
                .append(tokens[1]);
        sendRequest(GENKEY, message);
        sessionKey = Cryptor::makeKey(Cryptor::decryptRSA(tokens[1], keyFile), halfKey);
    }
    else {
        sessionKey = Cryptor::makeKey(halfKey, Cryptor::decryptRSA(tokens[1], keyFile));
    }
    halfKey = NULL;

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

void Client::setSsl()
{
    QSslCertificate ca = QSslCertificate::fromPath("../certs/ca.crt").first();
    if (ca.isNull()) {
        qDebug() << "Failed to load CA certificate";
    }
    socket->addCaCertificate(ca);
    QSslCertificate clientCert = QSslCertificate::fromPath("../certs/maros.pem").first();
    if (clientCert.isNull()) {
        qDebug() << "Failed to load CLIENT certificate";
    }
    socket->setLocalCertificate(clientCert);
    socket->setPrivateKey("../certs/maros_privkey.pem");
}

void Client::errorOccured(QList<QSslError> errors) {
    QSocket *socket = qobject_cast<QSocket*>(sender());
    foreach (QSslError error, errors) {
        qDebug() << "SSL error during hadshake: " << error.errorString();
    }
    socket->ignoreSslErrors(errors);
}

void Client::socketReady() {
    // Get calling socket
    QSocket *socket = qobject_cast<QSocket*>(sender());
    qDebug() << socket->peerAddress() << " connected.";

    // Start receiving data
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(serverDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(serverReadyRead()));
}
