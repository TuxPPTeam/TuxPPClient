#include "user.h"

User::User(QString name, rsa_context key):
    name(name), key(key)
{
}

QString User::getName() const { return name; }
rsa_context User::getKey() const { return key; }
