#include "HeaderFiles/mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //UsersDBManager();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getUserName(){
    return MainWindow::username;
}

QString MainWindow::getPassword(){
    return MainWindow::password;
}

void MainWindow::setUserName(QString inUserName){
    MainWindow::username = inUserName;
}

void MainWindow::setPassword(QString inPassword){
    MainWindow::password = inPassword;
}

void MainWindow::on_loginButon_clicked()
{
    UsersDBManager udb;

    //Checking the input username and password are give or not
    if(ui->userName_txt->text().isEmpty() || ui->password_txt->text().isEmpty()){
        mainMsgBox.setText("Please input the username and password");
        mainMsgBox.exec();
    }
    setUserName(ui->userName_txt->text());
    setPassword(ui->password_txt->text());

    bool successLogin = udb.checkLogin(getUserName(),getPassword());

    if(successLogin){
        //display another home window

    }
    else
    {
        //display qmessage login failed
        mainMsgBox.setText("Login failed. Please enter correct \nusername and password");
        mainMsgBox.exec();
    }

}



void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();

}
