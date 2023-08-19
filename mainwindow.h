#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qtreewidget.h"
#include "ui_dialog_edit.h"
#include "dialog_edit.h"
#include <QMainWindow>
#include <sqlite3.h>
#include <string>
#include <QDesktopServices>
#include <QTextEdit>
#include <QProcess>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void on_AddJob_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_DeleteJob_clicked();

    //void on_Refresh_clicked();

    void on_LoadData_clicked();

    void on_Edit_clicked();

    void on_EditOkButtonClicked();

    void openURL (QTreeWidgetItem* item, int column);

//    void processStandardOutput();

//    void processStandardError();


private:

    Ui::MainWindow *ui;

    void displayTree();

    sqlite3* db = nullptr;

    Dialog_Edit* Edit_Popup;

    //QProcess* process;

    //QTextEdit* Log;

};


class Database {

public:

    static void create_database(sqlite3*& db, const char* filename);

    static void create_table (sqlite3* db);

    static void print_database_error(sqlite3* db);

    static void add_new_record(sqlite3* db, char* new_record_text[4]);

    static void delete_record(sqlite3* db, int index);

    static void update_record_by_input_text(sqlite3* db, int index, char* update_record_text[4]);

    static void update_record_by_sql_cmd (sqlite3* db, char* sql);

    static void count_applied_job (sqlite3* db);


};


#endif // MAINWINDOW_H
