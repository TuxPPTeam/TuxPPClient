#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serversession.h"
#include <QtDebug>

ServerSession * session;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label1->setText("Welcome to the client program!");

    session = new ServerSession();
}

MainWindow::~MainWindow()
{
    delete session;
    delete ui;
}

void MainWindow::on_requestButton_clicked() {
    qDebug() << "request button clicked";
    User user;
    session->sendRequest(user);
}
