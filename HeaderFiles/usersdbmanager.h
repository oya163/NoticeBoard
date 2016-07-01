#ifndef USERSDBMANAGER_H
#define USERSDBMANAGER_H

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>

#include <QMessageBox>
#include <HeaderFiles/mainwindow.h>
#include <HeaderFiles/homewindow.h>

class UsersDBManager
{
private:
    QSqlDatabase users_db;
    QSqlRelationalTableModel *model;
    QMessageBox msgBox;

public:
    UsersDBManager();

    //CRUD operation relevant with USERS table
    bool checkLogin(QString uname, QString pwd);

    //CRUD operation relevant with NOTICE table
    bool insertNotice(QString createdTo, QString msg, QString createdBy);
    void checkNotice(int noticeID);
    void updateNotice(int noticeID, QString createdTo, QString msg, QString createdBy);
    void deleteNotice(int noticeID);

    //Connects the combo box of Create window to USERNAME column of USERS table
    QSqlQueryModel* loadEmployeeCombo();

    void closeConnection();
};

#endif // USERSDBMANAGER_H
