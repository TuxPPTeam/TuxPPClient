#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QtDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(new Client(this))
{
    ui->setupUi(this);
    connect(client, SIGNAL(dataRecieved(QByteArray)), this, SLOT(dataRecieved(QByteArray)));
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::dataRecieved(QByteArray data) {
    ui->outputBox->appendPlainText(data);
}

void MainWindow::on_connectButton_clicked() {
    qDebug() << "connect button clicked";
    
    QString login = ui->loginBox->text();
    QString fileName = ui->fileBox->text();
    if (client != nullptr) {
        if (login.isEmpty() || fileName.isEmpty()) {
            qDebug("missing login or file name");
            return;
        }
        client->setLogin(login);
        client->setKeyFileName(fileName);
        client->setReady(true);
    }
    if (!client->isConnected()) {
        if (client->connect()) {
            ui->outputBox->appendPlainText("Connected");
        }
        else {
            ui->outputBox->appendPlainText("Could not connect");
        }
    }
}

void MainWindow::on_fileDialogButton_clicked() {
    QString keyFileName = QFileDialog::getOpenFileName(this, tr("Choose key file"));
    if (keyFileName.isNull()) {
        qDebug("File not chosen");
        return;
    }
    qDebug() << "Key file:" << keyFileName;
    ui->fileBox->setText(keyFileName);
}

void MainWindow::on_loginButton_clicked()
{
    client->sendRequest(LOGIN, client->getLogin());
}

void MainWindow::on_logoutButton_clicked()
{
    client->sendRequest(LOGOUT, client->getLogin());
}

void MainWindow::on_getUsersButton_clicked()
{
    client->sendRequest(GETUSERS, "");
}

void MainWindow::on_registerButton_clicked()
{
    client->sendRequest(REGISTER, client->getLogin() + '\0' + client->getKeyFileName());
}

void MainWindow::on_clearButton_clicked()
{
    ui->outputBox->clear();
}
