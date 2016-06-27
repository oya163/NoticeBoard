#include "HeaderFiles/usersdbmanager.h"
#include <algorithm>

UsersDBManager::UsersDBManager()
{
    users_db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = "/home/oyashi/Qt_Project/NoticeBoard/Databases/NoticeBoard.db";
    users_db.setDatabaseName(path);

    if (!users_db.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    else
    {
       qDebug() << "Database: connection ok";
    }
}

//Check the login credentials are correct or not
bool UsersDBManager::checkLogin(QString uname, QString pwd){
    //For unit testing purpose
    qDebug() << uname << " " << pwd << endl;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE USERNAME = (:uname) AND PASSWORD = (:pwd)");
    checkQuery.bindValue(":uname",uname);
    checkQuery.bindValue(":pwd",pwd);
    checkQuery.exec();

    if(checkQuery.next()){
        if(checkQuery.value(0).toInt() == 1){
            return true;
        }
    }
    return false;
}

