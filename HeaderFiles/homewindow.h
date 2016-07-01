#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui{
    class HomeWindow;
}

class HomeWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    virtual ~HomeWindow();


public slots:
    void setWelcomeLabel(QString uName);


private slots:
    void on_logout_button_clicked();

    void on_createButton_clicked();

private:
    Ui::HomeWindow *uih;

    QString userName;

};


#endif // HOMEWINDOW_H
