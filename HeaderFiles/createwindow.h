#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <QDialog>
//#include <HeaderFiles/homewindow.h>
#include <HeaderFiles/usersdbmanager.h>

namespace Ui {
    class CreateWindow;
}

class CreateWindow : public QDialog{
    Q_OBJECT

public:
    CreateWindow(QWidget *parent, const QString &username);
    virtual ~CreateWindow();
    bool createNotice();
    QString getCurrentEmpUserName();


private slots:
    void on_cancelButton_clicked();

    void on_createButton_clicked();

private:
    Ui::CreateWindow *cwUI;
    QString noticeFrom;
};

#endif // CREATEWINDOW_H
