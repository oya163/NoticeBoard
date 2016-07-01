#include "HeaderFiles/homewindow.h"
#include "ui_homewindow.h"
#include "HeaderFiles/createwindow.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    uih(new Ui::HomeWindow)
{
    uih->setupUi(this);
}

HomeWindow::~HomeWindow()
{
    delete uih;
}

void HomeWindow::on_logout_button_clicked()
{
    MainWindow *mw = new MainWindow();
    this->close();
    mw->show();
}

void HomeWindow::setWelcomeLabel(QString uName){
    userName = uName;
    uih->wcName->setText(uName);
}

void HomeWindow::on_createButton_clicked()
{
    QString un = userName;
    CreateWindow *create = new CreateWindow(0,un);
    create->show();
}
