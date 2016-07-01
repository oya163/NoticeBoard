#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <QDialog>
#include <HeaderFiles/homewindow.h>

namespace Ui {
    class CreateWindow;
}

class CreateWindow : public QDialog{
    Q_OBJECT

public:
    explicit CreateWindow(QWidget *parent = 0);
    virtual ~CreateWindow();
    bool createNotice();
    QString getCurrentEmpUserName();


private slots:
    void on_cancelButton_clicked();

    void on_createButton_clicked();

private:
    Ui::CreateWindow *cwUI;

};

#endif // CREATEWINDOW_H
