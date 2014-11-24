#include "user.h"

User::User(QObject *parent) :
    QObject(parent)
{

}

User::User(QObject *parent, QString userName, QByteArray pubKey, QHostAddress host) :
    QObject(parent)
{
    this->ID = -1;
    setUsername(userName);
    this->pubKey = pubKey;
    this->host = host;
}

User::User(QObject *parent, qint64 ID, QString userName, QByteArray pubKey, QHostAddress host) :
    QObject(parent)
{
    setID(ID);
    setUsername(userName);
    this->pubKey = pubKey;
    this->host = host;
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

void User::setHost(QHostAddress newHost) {
    host = newHost;
}

// #########################################################################
qint64 User::getID() const {
    return ID;
}

QString User::getUsername() const {
    return username;
}

QByteArray User::getPubKey() const {
    return pubKey;
}

QHostAddress User::getHost() const {
    return host;
}

bool User::operator ==(const User &user) const {
    return  (user.getID() == ID) &&
            (user.getUsername() == username) &&
            (user.getPubKey() == pubKey);
}
