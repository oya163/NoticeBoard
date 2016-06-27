#ifndef USERSDBMANAGER_H
#define USERSDBMANAGER_H

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>

#include <QMessageBox>
#include <HeaderFiles/mainwindow.h>

class UsersDBManager
{
private:
    QSqlDatabase users_db;
    QMessageBox msgBox;

public:
    UsersDBManager();
    bool checkLogin(QString uname, QString pwd);


};

#endif // USERSDBMANAGER_H
