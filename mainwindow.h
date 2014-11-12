#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void dataRecieved(QByteArray);

    void on_connectButton_clicked();
    void on_fileDialogButton_clicked();
    
    void on_loginButton_clicked();

    void on_logoutButton_clicked();

    void on_getUsersButton_clicked();

private:
    Ui::MainWindow *ui;
    Client *client;
};

#endif // MAINWINDOW_H
