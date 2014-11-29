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
    cl->sendRequest(LOGOUT, cl->getLogin());
    delete ui;
}

void LoginScreen::mainWindowClosed() {
    w->hide();
    this->show();
}

void LoginScreen::displayMsg(QString caption, QString msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle(caption);
    msgBox.exec();
}

void LoginScreen::loginSuccessful() {
    this->hide();
    w->show();
}

void LoginScreen::on_loginBtn_clicked()
{
    qDebug("LoginScreen::on_loginBtn_clicked()");
    if (!validInputs()) {
        return;
    }
    else {
        cl->setLogin(ui->loginBox->text());
        cl->setKeyFileName(ui->fileBox->text());
    }

    if (cl->connectToServer()) {
        w = new MainWindow(this, cl);
        w->setWindowFlags(Qt::Window);

        connect(cl, SIGNAL(serverDisconnected()),           this, SLOT(mainWindowClosed()));
        connect(w,  SIGNAL(closed()),                       this, SLOT(mainWindowClosed()));
        connect(cl, SIGNAL(displayMsg(QString,QString)),    this, SLOT(displayMsg(QString,QString)));
        connect(cl, SIGNAL(loginSuccessful()),              this, SLOT(loginSuccessful()));

        cl->sendRequest(LOGIN, cl->getLogin());
    }
    else {
        displayMsg("Connection error", "Unable to connect to server.");
    }
}

void LoginScreen::on_fileDialogBtn_clicked()
{
    qDebug("LoginScreen::on_fileBtn_clicked()");
    QString keyFileName = QFileDialog::getOpenFileName(this, tr("Choose key file"));
    if (keyFileName.isNull()) {
        qDebug("File not chosen");
        return;
    }
    qDebug() << "Key file:" << keyFileName;
    ui->fileBox->setText(keyFileName);
}

void LoginScreen::on_registerBtn_clicked()
{
    qDebug("LoginScreen::on_registerBtn_clicked()");
    
    if (!validInputs()) {
        return;
    }
    else {
        cl->setLogin(ui->loginBox->text());
        cl->setKeyFileName(ui->fileBox->text());
    }
 
    if (cl->connectToServer()) {
        connect(cl, SIGNAL(displayMsg(QString,QString)),    this, SLOT(displayMsg(QString,QString)));
        cl->sendRequest(REGISTER, cl->getLogin());
    }
    else {
        displayMsg("Connection error", "Unable to connect to server.");
    }
}

bool LoginScreen::validInputs() {
    qDebug("LoginScreen::validInputs()");

    if (ui->loginBox->text().isEmpty() || ui->loginBox->text().length() > 32) {
        displayMsg("Login", "Invalid username!");
        return false;
    }

    if (ui->fileBox->text().isEmpty()) {
        displayMsg("Login", "Invalid key file!");
        return false;
    }
    
    return true;
}
