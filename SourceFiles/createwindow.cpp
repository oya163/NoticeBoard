#include <HeaderFiles/createwindow.h>
#include "ui_createwindow.h"

CreateWindow::CreateWindow(QWidget *parent, const QString &username):
    QDialog(parent),
    cwUI(new Ui::CreateWindow)
{
    cwUI->setupUi(this);
    UsersDBManager userDB;
    cwUI->empCombo->setModel(userDB.loadEmployeeCombo());
    userDB.closeConnection();

    //Set the name of USER who is creating a notice
    noticeFrom = username;
}

CreateWindow::~CreateWindow(){
    delete cwUI;
}

void CreateWindow::on_cancelButton_clicked()
{
    this->close();
}

QString CreateWindow::getCurrentEmpUserName(){
    return cwUI->empCombo->currentText();
}

bool CreateWindow::createNotice(){
    UsersDBManager userDB;
    QString createdTo = getCurrentEmpUserName();
    QString message = cwUI->messageTxtEdit->toPlainText();
    QString createdBy = noticeFrom;
    bool isSuccess = userDB.insertNotice(createdTo,message,createdBy);
    if(isSuccess){
        return true;
    }
    return false;
}

void CreateWindow::on_createButton_clicked()
{
    QMessageBox msgBox;
    bool onSuccess = createNotice();
    if(onSuccess){
        msgBox.setText("Notice created successfully");
        msgBox.exec();
    }
    else{
        msgBox.setText("Notice creation unsuccessful");
        msgBox.exec();
    }
}
