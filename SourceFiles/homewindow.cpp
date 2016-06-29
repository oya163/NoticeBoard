#include "HeaderFiles/homewindow.h"
#include "ui_homewindow.h"


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
    this->close();
}

void HomeWindow::setWelcomeLabel(QString uName){
    uih->wcLabel->setText(uName);
}

