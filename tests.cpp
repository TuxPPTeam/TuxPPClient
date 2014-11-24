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
    client->connectToServer();
    QVERIFY(client->isServerConnected() == true);
    delete client;
}

void Tests::testServerEcho()
{
    Client* client = new Client();
    client->connectToServer();
    //todo
    delete client;
}

/*void Tests::testClientConnection()
{
    QString message = "Hello!";

    Client* client1 = new Client();
    Client* client2 = new Client();

    client1->connect();
    client2->connect();

    //client1->sendData(client2, message);
    QVERIFY(client2->receiveData()? "" == message);

    delete client1;
    delete client2;
}*/
