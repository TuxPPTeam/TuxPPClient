#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFileDialog>
#include <QListView>

#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent, Client* client) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    client(client),
    model(new UserListModel(this))
{
    ui->setupUi(this);
    connect(client, SIGNAL(dataRecieved(QByteArray)), this, SLOT(dataRecieved(QByteArray)));
    connect(client, SIGNAL(userListRecieved(QList<User*>)), this, SLOT(userListRecieved(QList<User*>)));
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataRecieved(QByteArray data) {
    ui->outputBox->appendPlainText(data);
}

void MainWindow::userListRecieved(QList<User*> userList) {
    model->setUsers(userList);
}

void MainWindow::on_logoutButton_clicked()
{
    client->sendRequest(LOGOUT, client->getLogin());
    emit closed();
}

void MainWindow::on_getUsersButton_clicked()
{
    client->sendRequest(GETUSERS, "");
}

void MainWindow::on_clearButton_clicked()
{
    ui->outputBox->clear();
}

void MainWindow::on_connectButton_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    User *u = model->getUser(index);
    if (u != NULL) {
        client->createUserConnection(u);
    }
}

void MainWindow::on_sendButton_clicked()
{
    //QPlainTextEdit::toPlainText().toLocal8Bit()
    if (client->isClientConnected()) {
        client->sendData(ui->inputBox->toPlainText().toLocal8Bit());
    }
}
