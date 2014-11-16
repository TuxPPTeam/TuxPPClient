#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Client* client) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    client(client)
{
    ui->setupUi(this);
    connect(client, SIGNAL(dataRecieved(QByteArray)), this, SLOT(dataRecieved(QByteArray)));
    connect(client, SIGNAL(userListRecieved(QList<User*>)), this, SLOT(userListRecieved(QList<User*>)));
    model = new UserListModel(this);
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    client->sendRequest(LOGOUT, client->getLogin());
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
    //ui->userList;
}
