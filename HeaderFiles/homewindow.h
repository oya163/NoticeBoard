#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>

#include "mainwindow.h"

namespace Ui{
    class HomeWindow;
}

class HomeWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    virtual ~HomeWindow();
    void displayNotice();
    void openDBConn();

public slots:
    void setWelcomeLabel(QString uName);


private slots:
    void on_logout_button_clicked();
    void on_createButton_clicked();
    void insertData();
    void updateData(const QModelIndex &);
    void removeData();
    void readData();

    void on_updateButton_clicked();

private:
    Ui::HomeWindow *uih;
    QSqlTableModel *model;
    QString userName;
    QString dbPath = "/home/oyashi/Qt_Project/NoticeBoard/Databases/NoticeBoard.db";

};


#endif // HOMEWINDOW_H
