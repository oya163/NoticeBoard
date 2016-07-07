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
    connect(uih->updateButton,SIGNAL(clicked()), this, SLOT(updateData()));
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
//    Uncomment below lines to display new Create Window
//    However, below lines has become obsolete now
//    QString un = userName;
//    CreateWindow *create = new CreateWindow(0,un);
//    create->show();

    //Inserts new row in the model
    model->insertRow(0,QModelIndex());

    //Goes to insertData() only after records are written on model
    connect(uih->tableView,SIGNAL(entered(QModelIndex)),this,SLOT(insertData()));
}

//Inserts new row on model and into database
void HomeWindow::insertData(){
    disconnect(uih->tableView,SIGNAL(entered(QModelIndex)),this,SLOT(insertData()));

    QSqlRecord newRecord = model->record();

    QModelIndex forIdx = model->index(0,1,QModelIndex());
    newRecord.setValue(1,forIdx.data());

    QModelIndex msgIndex = model->index(0,2,QModelIndex());
    newRecord.setValue(2,msgIndex.data());

    //Below section will set the default values
    newRecord.setValue(3,QVariant(userName));
    newRecord.setGenerated(4,false);
    newRecord.setGenerated(5,false);

    model->setRecord(0,newRecord);
    if(model->insertRecord(0,newRecord)){
        model->database().transaction();
        if(model->submitAll()){
            model->database().commit();
            qDebug() << "Successfully inserted";
        }
    }

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

    emit (model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)));
}

//Updates data changed on model to Database
void HomeWindow::updateData(){
    QMessageBox msgBox;

    QItemSelectionModel *selectModel = uih->tableView->selectionModel();
    QModelIndexList indexList = selectModel->selectedIndexes();
    QModelIndex index = indexList.at(0);

    //fromIdx to fetch name of user FROM whom message is intended
    QModelIndex fromIdx = model->index(index.row(),3,QModelIndex());

    model->database().transaction();
    if(fromIdx.data().toString() == userName){
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
    else{
        msgBox.setText("You are not authorized to update this");
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

    if(statusIdx.data().toString() != "YES"){
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
