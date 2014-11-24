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
    byte key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    //"2b7e151628aed2a6abf7158809cf4f3c"
    byte initVector[] = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };
    //"f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff"
    byte input[] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };
    //"6bc1bee22e409f96e93d7e117393172a"
    byte result[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    //"00000000000000000000000000000000"
    byte expected[] = { 0x87, 0x4d, 0x61, 0x91, 0xb6, 0x20, 0xe3, 0x26, 0x1b, 0xef, 0x68, 0x64, 0x99, 0x0d, 0xb6, 0xce };
    //"874d6191b620e3261bef6864990db6ce"

    Cryptor* c = new Cryptor(key, initVector, this);
    c->process(input, result, 16);

    QVERIFY(memcmp(result, expected, 32));
    delete c;
}
