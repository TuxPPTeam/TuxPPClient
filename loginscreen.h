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

    void on_fileDialogBtn_clicked();

private:
    Ui::LoginScreen *ui;
    Client *cl;
    MainWindow *w;
};

#endif // LOGINSCREEN_H
