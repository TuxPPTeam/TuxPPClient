#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User(QString name, rsa_context key);
    QString getName();
    rsa_context getKey();
private:
    QString name;
    rsa_context publicKey;
};

#endif // USER_H
