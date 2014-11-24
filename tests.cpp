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

//http://developer.nokia.com/community/wiki/How_to_wait_synchronously_for_a_Signal_in_Qt
void Tests::testServerEcho()
{
    Client* client = new Client();
    QEventLoop loop;
    connect(client, SIGNAL(messageReceived()), &loop, SLOT(quit()));
    client->connectToServer();
    QString message = "abcd";
    client->sendRequest(ECHO, message);
    //loop.exec();
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
    byte key[] = "2b7e151628aed2a6abf7158809cf4f3c";
    byte initVector[] = "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";
    byte input[] = "6bc1bee22e409f96e93d7e117393172a";
    byte result[] = "00000000000000000000000000000000";
    byte expected[] = "874d6191b620e3261bef6864990db6ce";

    Cryptor* c = new Cryptor(key, initVector, this);
    c->process(input, result, 16);

    QVERIFY(memcmp(result, expected, 32));
    delete c;
}
