#include "loginscreen.h"
#include "ui_loginscreen.h"

LoginScreen::LoginScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginScreen),
    cl(new Client(this))
{
    ui->setupUi(this);
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

void LoginScreen::mainWindowClosed() {
    w->hide();
    this->show();
}

void LoginScreen::on_loginBtn_clicked()
{


    if (!ui->loginBox->text().isEmpty()) {
        cl->setLogin(ui->loginBox->text());
    }
    else {
        QMessageBox msg;
        msg.setText("You have to fill login!");
        msg.exec();
        return;
    }

    if (!ui->fileBox->text().isEmpty()) {
        cl->setKeyFileName(ui->fileBox->text());
    }
    else {
        QMessageBox msg;
        msg.setText("You have to provide key file!");
        msg.exec();
        return;
    }

    if (cl->connectToServer()) {
        w = new MainWindow(this, cl);
        w->setWindowFlags(Qt::Window);
        connect(cl, SIGNAL(serverDisconnected()), SLOT(mainWindowClosed()));
        connect(w, SIGNAL(closed()), this, SLOT(mainWindowClosed()));
        this->hide();
        w->show();
        cl->sendRequest(LOGIN, cl->getLogin());
    }
    else {
        QMessageBox msg;
        msg.setText("Error occured, could not login.");
        msg.exec();
    }
}

void LoginScreen::on_fileDialogBtn_clicked()
{
    QString keyFileName = QFileDialog::getOpenFileName(this, tr("Choose key file"));
    if (keyFileName.isNull()) {
        qDebug("File not chosen");
        return;
    }
    qDebug() << "Key file:" << keyFileName;
    ui->fileBox->setText(keyFileName);
}
