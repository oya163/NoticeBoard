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

    //CRUD operation relevant with USERS table
    bool checkLogin(QString uname, QString pwd);

    //CRUD operation relevant with NOTICE table
    void displayNotice();
    bool insertNotice(QString createdTo, QString msg, QString createdBy);
    void checkNotice(int noticeID);
    void updateNotice(int noticeID, QString createdTo, QString msg, QString createdBy);
    void deleteNotice(int noticeID);
};

#endif // USERSDBMANAGER_H
