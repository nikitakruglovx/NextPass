#ifndef PASSCONTROLL_H
#define PASSCONTROLL_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QListWidgetItem>

#include "DataBase.h"

namespace Ui {
class PassControll;
}

class PassControll : public QWidget
{
    Q_OBJECT

public:
    explicit PassControll(QWidget *parent = nullptr);
    ~PassControll();

public:
    QSqlDatabase db;

private:
    Ui::PassControll *ui;
    data::DataConnection *datas;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void validationPass();
    void AddItem();
    void Updater();
    void AddNotes();

    QPoint curs;
    QPoint new_curs;
//    QString PROFILEPATH = "C:\\Users\\user\\Desktop\\PROJECTS\\VivaPassLock\\profile\\profile.json";
    QString PROFILEPATH = "C:\\VivaPassLock\\profile\\profile.json";

public slots:
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();
    void on_hideandseeButton_clicked();
    void on_hideandseeButton_2_clicked();
    void on_hideandseeButton_3_clicked();
    void on_passwordsButton_clicked();
    void on_notesButton_clicked();
    void on_addItemButton_clicked();
    void on_clearButton_clicked();
    void on_clearButton_2_clicked();
    void on_addsaveButton_clicked();
    void on_deleteButton_clicked();
    void on_updateButton_clicked();
    void on_addNotesButton_clicked();
    void on_addsaveButton_2_clicked();

private slots:
    void on_passEdit_returnPressed();
    void on_Titleedit_returnPressed();
    void on_UsernameEdit_returnPressed();
    void on_PasswordEdit_returnPressed();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_listWidget_2_itemClicked(QListWidgetItem *itemNotes);
    void on_Titleedit_2_returnPressed();
    void on_UsernameEdit_2_returnPressed();
    void on_PasswordEdit_2_returnPressed();
    void on_lineEdit_returnPressed();
    void on_textEdit_returnPressed();
};

#endif
