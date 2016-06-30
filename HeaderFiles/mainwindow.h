#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "HeaderFiles/usersdbmanager.h"
#include "HeaderFiles/homewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindow *ui;

    QMessageBox mainMsgBox;

    QString username;
    QString password;

    HomeWindow *home = new HomeWindow();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getUserName();
    void setUserName(QString inUserName);

    QString getPassword();
    void setPassword(QString inPassword);

private slots:
    void on_loginButon_clicked();

    void on_exitButton_clicked();

signals:
    void setUName(QString uName);



};

#endif // MAINWINDOW_H
