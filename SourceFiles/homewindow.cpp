#include "HeaderFiles/homewindow.h"
#include "ui_homewindow.h"
#include "HeaderFiles/createwindow.h"
#include <QSqlTableModel>

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    uih(new Ui::HomeWindow)
{
    uih->setupUi(this);

    //Database Connection
    openDBConn();

    displayNotice();

    //Connect Press Buttons SIGNALS to the respective SLOTS
    connect(uih->updateButton, SIGNAL(clicked()), this, SLOT(updateData()));
    connect(uih->delButton, SIGNAL(clicked()), this, SLOT(removeData()));
    connect(uih->readButton,SIGNAL(clicked()),this,SLOT(readData()));
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
//    CreateWindow *create = new CreateWindow(0,un);
//    create->show();
    model->insertRow(0,QModelIndex());

    QSqlRecord newRecord = model->record();

    newRecord.setValue(1,QVariant("OYESH"));
    newRecord.setValue(2,QVariant("OYESH"));
    newRecord.setValue(3,QVariant("OYESH"));
    newRecord.setValue(4,QVariant("2016-07-05 06:36:29"));
    newRecord.setValue(5,QVariant("NO"));

    newRecord.insert(0,newRecord.field("SN"));
    newRecord.insert(1,newRecord.field("CREATEDTO"));
    newRecord.insert(2,newRecord.field("MESSAGE"));
    newRecord.insert(3,newRecord.field("CREATEDBY"));
    newRecord.insert(4,newRecord.field("CREATEDON"));
    newRecord.insert(5,newRecord.field("ISREAD"));

    model->setRecord(0,newRecord);

    model->database().transaction();
    if(model->submitAll()){
        model->database().commit();
        qDebug() << "Successfully inserted";
    }

    //connect(uih->tableView,SIGNAL(entered(QModelIndex)),this,SLOT(createData()));
}

//Connects the database
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

//Insert data from newly created row into database
void HomeWindow::createData(){

}

//Displays and Updates the QTableView and Database
void HomeWindow::displayNotice(){
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

    //Sorting in descending order in order to display latest post first
    uih->tableView->setSortingEnabled(true);
    uih->tableView->sortByColumn(0,Qt::DescendingOrder);
    //Manually fitting the columns to QTableView
    uih->tableView->setColumnWidth(0,30);
    uih->tableView->setColumnWidth(2,250);
    uih->tableView->setColumnWidth(4,150);
    uih->tableView->setColumnWidth(5,30);
    uih->tableView->horizontalHeader()->setStretchLastSection(true);
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

    //fromIdx to fetch name of user FROM whom message is intended
    QModelIndex fromIdx = model->index(index.row(),3,QModelIndex());

    model->database().transaction();
    if(fromIdx.data().toString() == userName){
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
    else{
        msgBox.setText("You are not authorized to remove this record");
        msgBox.exec();
    }

}

//Update selected Notice as READ
void HomeWindow::readData(){
    QMessageBox msgBox;
    QItemSelectionModel *selectModel = uih->tableView->selectionModel();
    QModelIndexList indexList = selectModel->selectedIndexes();
    QModelIndex index = indexList.at(0);

    //forIdx to fetch name of user FOR whom message is intended
    QModelIndex forIdx = model->index(index.row(),1,QModelIndex());

    //statusIdx to fetch the READ status of new message
    QModelIndex statusIdx = model->index(index.row(),5,QModelIndex());

    qDebug() << statusIdx.data().toString();

    if(statusIdx.data().toString() == "NO"){
        if(userName == forIdx.data().toString()){
            model->setData(statusIdx,QVariant(tr("YES")));
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
            msgBox.setText(tr("You are not %1").arg(forIdx.data().toString()));
            msgBox.exec();
        }
    }
    else{
        qDebug() << "NOTICE is already READ";
        msgBox.setText("NOTICE is already READ");
        msgBox.exec();
    }
}



