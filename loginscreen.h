#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "client.h"
#include "mainwindow.h"

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = 0);
    ~LoginScreen();

private slots:
    void on_loginBtn_clicked();
    void mainWindowClosed();
    void loginSuccessful();
    void displayMsg(QString, QString);

    void on_fileDialogBtn_clicked();

    void on_registerBtn_clicked();
    
private:
    Ui::LoginScreen *ui;
    Client *cl;
    MainWindow *w;
    
    bool validInputs();
};

#endif // LOGINSCREEN_H
