#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include <QAbstractListModel>
#include "user.h"

class UserListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UserListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setUsers(QList<User*> newUsers);

private:
    QList<User*> users;
};

#endif // USERLISTMODEL_H
