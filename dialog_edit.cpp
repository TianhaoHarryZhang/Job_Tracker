#include "dialog_edit.h"
#include "ui_dialog_edit.h"
#include <sqlite3.h>
#include <iostream>

Dialog_Edit::Dialog_Edit(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog_Edit)
{
    ui->setupUi(this);

}

Dialog_Edit::~Dialog_Edit()
{
    delete ui;
}

void Dialog_Edit::on_Edit_OK_clicked()
{
    emit EditOkButtonClicked();
}

Ui::Dialog_Edit* Dialog_Edit::getUi(){

    return ui;
}



