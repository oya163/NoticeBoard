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


void HomeWindow::openDBConn(){
    QMessageBox msgBox;

    //Database connection
    QSqlDatabase users_db = QSqlDatabase::addDatabase("QSQLITE");
    users_db.setDatabaseName(dbPath);
    if (!users_db.open())
    {
       msgBox.setText("Error: connection with database fail");
       msgBox.exec();
    }
}


//Displays and Updates the QTableView and Database
void HomeWindow::displayNotice(){
    //Database Connection
    HomeWindow::openDBConn();

    //Model Setup
    model = new QSqlTableModel(uih->tableView);
    model->setTable("NOTICE");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //Filter to display current month NOTICE only
    model->setFilter("strftime('%m',CREATEDON) = strftime('%m','now')");
    model->select();

    //Set Header for the model
    model->setHeaderData(0, Qt::Horizontal, tr("SN"));
    model->setHeaderData(1, Qt::Horizontal, tr("For"));
    model->setHeaderData(2, Qt::Horizontal, tr("Notice"));
    model->setHeaderData(3, Qt::Horizontal, tr("From"));
    model->setHeaderData(4, Qt::Horizontal, tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, tr("Read"));

    //Qtableview setup according to the model
    uih->tableView->setModel(model);
    uih->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    //Manually fitting the columns to QTableView
    uih->tableView->setColumnWidth(0,30);
    uih->tableView->setColumnWidth(2,250);
    uih->tableView->setColumnWidth(4,150);
    uih->tableView->setColumnWidth(5,30);
    uih->tableView->horizontalHeader()->setStretchLastSection(true);

    //Connect Press Buttons SIGNALS to the respective SLOTS
    connect(uih->updateButton, SIGNAL(clicked()), this, SLOT(updateData()));
    connect(uih->delButton, SIGNAL(clicked()), this, SLOT(removeData()));
    connect(uih->readButton,SIGNAL(clicked(bool)),this,SLOT(readData()));
}


void HomeWindow::updateData(){
    QMessageBox msgBox;
    model->database().transaction();
    if(model->submitAll()){
        model->database().commit();
        msgBox.setText("Data updated successfully");
        msgBox.exec();
    }
    else {
        model->database().rollback();
        msgBox.warning(this, tr("Notice Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
        msgBox.exec();
    }
}

//Removes data from QTableView and Database as well
void HomeWindow::removeData(){
    QMessageBox msgBox;
    QItemSelectionModel *selectModel = uih->tableView->selectionModel();
    QModelIndexList indexList = selectModel->selectedIndexes();
    QModelIndex index = indexList.at(0);
    qDebug() << index.row();

    model->database().transaction();
    if(model->removeRow(index.row())){
        qDebug() << "Remove successful" << endl;
        if(model->submitAll()){
            model->database().commit();
            msgBox.setText("Data removed successfully");
            msgBox.exec();
        }
        else {
            model->database().rollback();
            msgBox.warning(this, tr("Notice Table"),
                                 tr("The database reported an error: %1")
                                 .arg(model->lastError().text()));
            msgBox.exec();
        }

    }
    else{
        qDebug() << "Remove unsuccessful" << endl;
    }
}

//Update selected Notice as READ
void HomeWindow::readData(){
    QMessageBox msgBox;
    QItemSelectionModel *selectModel = uih->tableView->selectionModel();
    QModelIndexList indexList = selectModel->selectedIndexes();
    QModelIndex index = indexList.at(0);

    QModelIndex newIdx = model->index(index.row(),5,QModelIndex());

    qDebug() << newIdx.data().toString();

    if(newIdx.data().toString() == "NO"){
        model->setData(newIdx,QVariant(tr("YES")));
        model->database().transaction();
        if(model->submitAll()){
            qDebug() << "NOTICE is READ" << endl;
            model->database().commit();
        }
        else {
            model->database().rollback();
            msgBox.warning(this, tr("Notice Table"),
                                 tr("The database reported an error: %1")
                                 .arg(model->lastError().text()));
            msgBox.exec();
        }
    }
    else{
        qDebug() << "NOTICE is already READ";
    }

}


