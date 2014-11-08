#ifndef USER_H
#define USER_H

#include <QString>
#include <QSslSocket>

class User
{
public:
    //User(QString name, rsa_context key);
    QString getName() const;
    //QSslSocket getSocket() const;
    //rsa_context getKey();
private:
    QString name;
    //QSslSocket socket;
    //rsa_context publicKey;
};

#endif // USER_H
