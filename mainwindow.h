#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client.h"
#include "userlistmodel.h"
#include <QWidget>
//#include <QItemSelectionModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0, Client* client = 0);
    ~MainWindow();

signals:
    void closed();

private slots:
    void dataRecieved(QByteArray);

    void userListRecieved(QList<User*>);

    void on_connectButton_clicked();

    void on_logoutButton_clicked();

    void on_getUsersButton_clicked();

    void on_clearButton_clicked();

    void on_sendButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;
    UserListModel *model;
    Client *client;
    QList<User*> users;
};

#endif // MAINWINDOW_H
