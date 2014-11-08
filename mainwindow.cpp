#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "session.h"
#include <QtDebug>
#include <QFileDialog>

Session * session;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    session = new ServerSession();
}

MainWindow::~MainWindow()
{
    delete session;
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    qDebug() << "request button clicked";
    User user;
    session->sendRequest(user);
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
