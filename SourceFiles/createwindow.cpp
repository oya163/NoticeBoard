#include <HeaderFiles/createwindow.h>
#include "ui_createwindow.h"

CreateWindow::CreateWindow(QWidget *parent):
    QDialog(parent),
    cwUI(new Ui::CreateWindow){
    cwUI->setupUi(this);

    UsersDBManager userDB;
    cwUI->empCombo->setModel(userDB.loadEmployeeCombo());
    userDB.closeConnection();

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
    QString createdTo = getCurrentEmpUserName();
    qDebug() << createdTo;
    return true;
}



void CreateWindow::on_createButton_clicked()
{
    bool onSuccess = createNotice();
}
