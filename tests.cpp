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
    QVERIFY(client->isConnected() == true);
    delete client;
}

//http://developer.nokia.com/community/wiki/How_to_wait_synchronously_for_a_Signal_in_Qt
void Tests::testServerEcho()
{
    Client* client = new Client();
    QEventLoop loop;
    connect(client, SIGNAL(messageReceived()), &loop, SLOT(quit()));
    client->connectToServer();
    QString message = "abcd";
    client->sendRequest(ECHO, message);
    loop.exec();
    qDebug() << "Last message:" << client->lastMessage;
    QVERIFY(message == client->lastMessage);
    delete client;
}

void Tests::testClientConnection()
{
    QString message = "Hello!";

    Client* client1 = new Client();
    Client* client2 = new Client();

    //client1->connect();
    //client2->connect();

    //client1->sendData(client2, message);
    QVERIFY(false/*client2->receiveData()? "" == message*/);

    delete client1;
    delete client2;
}

void Tests::testEncryption()
{
    QVERIFY(false);
}
