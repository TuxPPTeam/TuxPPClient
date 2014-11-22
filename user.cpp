#include "user.h"

User::User(QObject *parent, QString userName, /*qint32 host,*/ QByteArray pubKey, QHostAddress *socket) :
    QObject(parent)
{
    this->ID = -1;
    setUsername(userName);
    //setHost(host);
    this->pubKey = pubKey;
    this->host = socket;
}

User::User(QObject *parent, qint64 ID, QString userName, /*qint32 host,*/ QByteArray pubKey, QHostAddress *socket) :
    QObject(parent)
{
    setID(ID);
    setUsername(userName);
    //setHost(host);
    this->pubKey = pubKey;
    this->host = socket;
}

// #################################################################################
bool User::setID(const qint64 newID) {
    if (ID != -1) {
        ID = newID;
        return true;
    }
    return false;
}

bool User::setUsername(const QString newUsername) {
    if ((unsigned int)newUsername.length() > MAX_USERNAME_LENGTH) {
        return false;
    }
    else {
        username = newUsername;
        return true;
    }
}

/*void User::setHost(const qint32 newHost) {
    host = newHost;
}*/

void User::setPubKey(const QByteArray newPubKey) {
    pubKey = newPubKey;
}

void User::setSocket(QHostAddress *newSocket) {
    host = newSocket;
}

// #########################################################################
qint64 User::getID() const {
    return ID;
}

QString User::getUsername() const {
    return username;
}

/*qint32 User::getHost() const {
    return host;
}*/

QByteArray User::getPubKey() const {
    return pubKey;
}

QHostAddress* User::getSocket() const {
    return host;
}

bool User::operator ==(const User &user) const {
    return  (user.getID() == ID) &&
            (user.getUsername() == username) &&
            (user.getPubKey() == pubKey);
}
