#include "HeaderFiles/usersdbmanager.h"

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
    checkQuery.prepare("SELECT COUNT(*) FROM USERS WHERE USERNAME = (:uname) AND PASSWORD = (:pwd)");
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

bool UsersDBManager::insertNotice(QString createdTo, QString msg, QString createdBy){
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO NOTICE (CREATEDTO, MESSAGE, CREATEDBY) VALUES((:CREATEDTO),(:MESSAGE),(:CREATEDBY)");
    insertQuery.bindValue(":CREATEDTO", createdTo);
    insertQuery.bindValue(":MESSAGE", msg);
    insertQuery.bindValue(":CREATEDBY", createdBy);
    if(insertQuery.exec()){
        return true;
    }
    return false;
}

QSqlQueryModel* UsersDBManager::loadEmployeeCombo(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery getUsersQuery;
    getUsersQuery.prepare("SELECT USERNAME FROM USERS");
    if(getUsersQuery.exec()){
        model->setQuery(getUsersQuery);
        return model;
    }
    return model;
}

void UsersDBManager::closeConnection(){
    users_db.close();
}


