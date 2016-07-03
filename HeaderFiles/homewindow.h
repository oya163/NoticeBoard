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

public slots:
    void setWelcomeLabel(QString uName);


private slots:
    void on_logout_button_clicked();
    void on_createButton_clicked();

    void on_updateButton_clicked(const QModelIndex &);

private:
    Ui::HomeWindow *uih;
    QSqlTableModel *model;
    QString userName;

};


#endif // HOMEWINDOW_H
