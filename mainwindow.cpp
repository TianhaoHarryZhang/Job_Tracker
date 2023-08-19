#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <sqlite3.h>
#include "dialog_edit.h"
#include <iostream>
#include <QTextEdit>
#include "ui_mainwindow.h"
#include <QProcess>
#include <filesystem>



using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    displayTree();

    Edit_Popup = new Dialog_Edit(this);

    //Ui::Dialog_Edit* Edit_Ui = Edit_Popup->getUi();


    //connect(ui->Edit, &QPushButton::clicked, this, &MainWindow::on_Edit_clicked);

    //connect(Edit_Popup)

    connect(Edit_Popup, &Dialog_Edit::EditOkButtonClicked, this, &MainWindow::on_EditOkButtonClicked);

    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked, this, &MainWindow::openURL);


//    process = new QProcess(this);



//    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::processStandardOutput);
//    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::processStandardError);

//    qDebug() << process;

//    process->start("main");


}



MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_AddJob_clicked(sqlite3* db)
//{
//    QTreeWidgetItem* new_item = new QTreeWidgetItem (ui->treeWidget);

//    char* new_record[4] = {"New Company","New Position", "New Status", "New Weblink"};


//    new_item->setText(0,new_record[0]);
//    new_item->setText(1,new_record[1]);
//    new_item->setText(2,new_record[2]);
//    new_item->setText(3,new_record[3]);

//    new_item->setTextAlignment(0,Qt::AlignCenter);
//    new_item->setTextAlignment(1,Qt::AlignCenter);
//    new_item->setTextAlignment(2,Qt::AlignCenter);
//    new_item->setTextAlignment(3,Qt::AlignCenter);

//    ui->treeWidget->addTopLevelItem(new_item);

//    Database::add_new_record(db,new_record);


//}




void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

    return;
}



void MainWindow::displayTree()
{
    ui->treeWidget->setColumnCount(3);

    QStringList Column_Names;

    Column_Names << "Company" << "Job" << "Status" << "Weblink";

    ui->treeWidget->setHeaderLabels(Column_Names);

    ui->treeWidget->header()->resizeSection(0, 160);
    ui->treeWidget->header()->resizeSection(1, 460);
    ui->treeWidget->header()->resizeSection(2, 250);

    ui->treeWidget->clear();

}




void MainWindow::on_DeleteJob_clicked()
{
    QTreeWidgetItem* selected_item = ui->treeWidget->currentItem();

    int index = ui->treeWidget->indexOfTopLevelItem(selected_item);

    Database::delete_record(db,index);

    delete selected_item;


}


//void MainWindow::on_Refresh_clicked(sqlite3* db)
//{
//    QTreeWidgetItem* new_item1 = new QTreeWidgetItem (ui->treeWidget);


//    new_item1->setText(0,"New ");
//    new_item1->setText(1,"New Job ");
//    new_item1->setText(2,"New Status");

//    new_item1->setTextAlignment(0,Qt::AlignCenter);
//    new_item1->setTextAlignment(1,Qt::AlignCenter);
//    new_item1->setTextAlignment(2,Qt::AlignCenter);

//    ui->treeWidget->addTopLevelItem(new_item1);

//}


void MainWindow::on_LoadData_clicked()
{


    //Check file path
    //std::filesystem::path SourceFilePath = std::filesystem::current_path();
    //std::filesystem::path filepath_original = SourceFilePath / "Job_Database.db";
    const char* filepath = "/Users/harryzhang/Documents/MySQL+C++/Job_Database.db";


    //Create or open database
    Database::create_database(db,filepath);
    Database::create_table(db);

    //Clear the treewidget
    ui->treeWidget->clear();


    //Populate the tree widget with data from database
    sqlite3_stmt* stmt;

    const char* query = "SELECT Company, Position, Status, Weblink FROM Job_List";

    int rc = sqlite3_prepare_v2(db,query,-1,&stmt, NULL);

    if (rc != SQLITE_OK) {

        cout << "Error in preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW){

        QString company = QString::fromUtf8((const char*) sqlite3_column_text(stmt, 0));
        QString position = QString::fromUtf8((const char*) sqlite3_column_text(stmt, 1));
        QString status = QString::fromUtf8((const char*) sqlite3_column_text(stmt, 2));
        QString weblink = QString::fromUtf8((const char*) sqlite3_column_text(stmt, 3));

        QTreeWidgetItem* item = new QTreeWidgetItem (ui->treeWidget);

        item->setText(0, company);
        item->setText(1, position);
        item->setText(2, status);
        item->setText(3, weblink);


    }

    // To release any resources associated with the statement
    sqlite3_finalize(stmt);


    //count the number of applied jobs and total



}



void MainWindow::on_AddJob_clicked()
{
    QTreeWidgetItem* new_item = new QTreeWidgetItem (ui->treeWidget);

    char* new_record[4] = {"New Company","New Position", "New Status", "New Weblink"};

    new_item->setText(0,new_record[0]);
    new_item->setText(1,new_record[1]);
    new_item->setText(2,new_record[2]);
    new_item->setText(3,new_record[3]);

    new_item->setTextAlignment(0,Qt::AlignLeft);
    new_item->setTextAlignment(1,Qt::AlignLeft);
    new_item->setTextAlignment(2,Qt::AlignLeft);
    new_item->setTextAlignment(3,Qt::AlignLeft);

    ui->treeWidget->addTopLevelItem(new_item);

    Database::add_new_record(db,new_record);
}


//void MainWindow::on_Refresh_clicked()
//{
//    QTreeWidgetItem* new_item1 = new QTreeWidgetItem (ui->treeWidget);


//    new_item1->setText(0,"New ");
//    new_item1->setText(1,"New Job ");
//    new_item1->setText(2,"New Status");

//    new_item1->setTextAlignment(0,Qt::AlignCenter);
//    new_item1->setTextAlignment(1,Qt::AlignCenter);
//    new_item1->setTextAlignment(2,Qt::AlignCenter);

//    ui->treeWidget->addTopLevelItem(new_item1);
//}


void MainWindow::on_Edit_clicked()
{

    QTreeWidgetItem* selected_item = ui->treeWidget->currentItem();


    QString selected_Company = "No item selected";
    QString selected_Position = "No item selected";
    QString selected_Status = "No item selected";
    QString selected_Weblink = "No item selected";


    if (selected_item != nullptr) {
        //int selected_index = ui->treeWidget->indexOfTopLevelItem(selected_item);
        selected_Company = selected_item->text(0);
        selected_Position = selected_item->text(1);
        selected_Status = selected_item->text(2);
        selected_Weblink = selected_item->text(3);

    }
    else{
        cout<< "No Treewidget item selected!"<<endl;
    }


    Edit_Popup->getUi()->Company_Edit->setText(selected_Company);
    Edit_Popup->getUi()->Position_Edit->setText(selected_Position);
    Edit_Popup->getUi()->Status_Edit->setText(selected_Status);
    Edit_Popup->getUi()->Weblink_Edit->setText(selected_Weblink);

    Edit_Popup->show();

}



void MainWindow::on_EditOkButtonClicked(){

    QTreeWidgetItem* selected_item = ui->treeWidget->currentItem();


    if (selected_item){

        int selected_index = ui->treeWidget->indexOfTopLevelItem(selected_item);

        QString Edited_Company_Text = Edit_Popup->getUi()->Company_Edit->text();
        QString Edited_Position_Text = Edit_Popup->getUi()->Position_Edit->text();
        QString Edited_Status_Text = Edit_Popup->getUi()->Status_Edit->text();
        QString Edited_Weblink_Text = Edit_Popup->getUi()->Weblink_Edit->text();

        selected_item->setText(0,Edited_Company_Text);
        selected_item->setText(1,Edited_Position_Text);
        selected_item->setText(2,Edited_Status_Text);
        selected_item->setText(3,Edited_Weblink_Text);

        char* update_sql_cmd = sqlite3_mprintf("UPDATE Job_List SET Company='%s', Position='%s', Status='%s', Weblink='%s' WHERE ROWID=%d;",
                                               Edited_Company_Text.toUtf8().constData(),
                                               Edited_Position_Text.toUtf8().constData(),
                                               Edited_Status_Text.toUtf8().constData(),
                                               Edited_Weblink_Text.toUtf8().constData(),
                                               selected_index + 1);

        Database::update_record_by_sql_cmd(db, update_sql_cmd);

        cout<<"Record update completed!"<<endl;
        Edit_Popup->close();

    }

    else{
        cout<<"No item selected for edition"<<endl;
        Edit_Popup->close();
    }


}

void MainWindow::openURL(QTreeWidgetItem* item, int column)
{
    if (column == 3) { // check if the column index is 3 (weblink)
        QString url = item->text(column);
        QDesktopServices::openUrl(QUrl(url)); // open the URL in the default web browser
    }
}


//void MainWindow::ShowLogMessage (const QString& message){

//    ui->LogWidget->append(message);

//}

//void MainWindow::processStandardOutput()
//{
//    QString output = process->readAllStandardOutput();
//    ui->LogWidget->append(output);

//}

//void MainWindow::processStandardError()
//{
//    QString error = process->readAllStandardError();
//    ui->LogWidget->append(error);
//}




