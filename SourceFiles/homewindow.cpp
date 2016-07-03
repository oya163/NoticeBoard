#include "HeaderFiles/homewindow.h"
#include "ui_homewindow.h"
#include "HeaderFiles/createwindow.h"
#include <QSqlTableModel>

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    uih(new Ui::HomeWindow)
{
    uih->setupUi(this);

    displayNotice();
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

void HomeWindow::displayNotice(){
    //Database connection
    QSqlDatabase users_db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = "/home/oyashi/Qt_Project/NoticeBoard/Databases/NoticeBoard.db";
    users_db.setDatabaseName(path);

    if (!users_db.open())
    {
       qDebug() << "Error: connection with database fail";
    }
    else
    {
       qDebug() << "Database: connection ok FROM HOME WINDOW";
    }

    //Model Setup
    model = new QSqlTableModel(uih->tableView);
    model->setTable("NOTICE");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //Filter to display current month NOTICE only
    model->setFilter("strftime('%m',CREATEDON) = strftime('%m','now')");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("SN"));
    model->setHeaderData(1, Qt::Horizontal, tr("For"));
    model->setHeaderData(2, Qt::Horizontal, tr("Notice"));
    model->setHeaderData(3, Qt::Horizontal, tr("From"));
    model->setHeaderData(4, Qt::Horizontal, tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, tr("Read"));

    uih->tableView->setModel(model);
    uih->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    uih->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);


    //Manually fitting the columns to QTableView
    uih->tableView->setColumnWidth(0,30);
    uih->tableView->setColumnWidth(2,250);
    uih->tableView->setColumnWidth(4,150);
    uih->tableView->setColumnWidth(5,30);
    uih->tableView->horizontalHeader()->setStretchLastSection(true);

    connect(uih->tableView, SIGNAL(entered(QModelIndex)), this, SLOT(on_updateButton_clicked(const QModelIndex &)));
}

void HomeWindow::on_updateButton_clicked(const QModelIndex &index)
{
    QMessageBox msgBox;
    int row = index.row();
    int col = index.column();

    qDebug() << "Row = " << row << " Column = " << col << endl;

    QString newValue = uih->tableView->model()->data(model->index(row,col)).toString();
    qDebug() << newValue << endl;
    model->setData(model->index(row,col),newValue);
    if(model->submitAll()){
        msgBox.setText("Data updated successfully");
    }
    else {
        msgBox.setText("Error : " + model->lastError().text());
    }
}
