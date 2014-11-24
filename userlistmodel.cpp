#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int UserListModel::rowCount(const QModelIndex &) const {
    return users.count();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
         return QVariant();

     if (index.row() >= users.size())
         return QVariant();

     if (role == Qt::DisplayRole) {
         QString name = users.at(index.row())->getUsername();
         return name;
     }
     else
         return QVariant();
}

void UserListModel::setUsers(QList<User *> newUsers) {
    beginResetModel();
    users = newUsers;
    endResetModel();
}

User* UserListModel::getUser(const QModelIndex &index) {
    if (index.isValid())
        if (index.row() <= users.count())
            return users[index.row()];
    return NULL;
}
