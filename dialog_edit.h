#ifndef DIALOG_EDIT_H
#define DIALOG_EDIT_H

#include <QDialog>

namespace Ui {
class Dialog_Edit;
}

class Dialog_Edit : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Edit(QWidget *parent = nullptr);
    ~Dialog_Edit();

    Ui::Dialog_Edit* getUi();

private slots:

    void on_Edit_OK_clicked();

signals:

    void EditOkButtonClicked();

private:
    Ui::Dialog_Edit *ui;
};

#endif // DIALOG_EDIT_H
