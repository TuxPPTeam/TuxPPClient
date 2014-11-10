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
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    qDebug() << "connect button clicked";
    
    QString login = ui->loginLine->text();
    QString fileName = ui->fileDialogLine->text();
    if (client == nullptr) {
        if (login.isEmpty() || fileName.isEmpty()) {
            qDebug("missing login or file name");
            return;
        }
        client->setLogin(login);
        client->setKeyFileName(fileName);
        client->setReady(true);
    }
    if (!client->isConnected()) {
        client->connect();
    }
    
    client->sendRequest("whatever");
//    session->sendRequest()
}

void MainWindow::on_fileDialogButton_clicked() {
    QString keyFileName = QFileDialog::getOpenFileName(this, tr("Choose key file"));
    if (keyFileName.isNull()) {
        qDebug("File not chosen");
        return;
    }
    qDebug() << "Key file:" << keyFileName;
    ui->fileDialogLine->setText(keyFileName);
}
