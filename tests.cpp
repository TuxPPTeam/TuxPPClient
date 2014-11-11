#include "tests.h"

void Tests::testLogin()
{
    QString login = "newLogin";
    Client* client = new Client();
    client->setLogin(login);
    QVERIFY(client->getLogin() == login);
    delete client;
}

void Tests::testKey()
{
    QString keyFileName = "newKeyFileName";
    Client* client = new Client();
    client->setKeyFileName(keyFileName);
    QVERIFY(client->getKeyFileName() == keyFileName);
    delete client;
}

void Tests::testServerConnection()
{
    Client* client = new Client();
    client->connect();
    QVERIFY(client->isConnected() == true);
    delete client;
}
