#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMovie>

#include "PassControll.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    PassControll *homeWindow = new PassControll;
    QSqlDatabase db;

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void systemLogin();
    int generationId();
    void connectDB();
    void create(int int_id);

    QPoint curs;
    QPoint new_curs;
    QMovie *pic = new QMovie(":/new/icon/Resources/assets/icon/connect.gif");
    QMovie *pic_2 = new QMovie(":/new/icon/Resources/assets/icon/reg.gif");
    QString PROFILEPATH = "C:\\NextPass\\profile\\profile.json";
    QMovie *pic_success = new QMovie(":/new/icon/Resources/assets/icon/success.gif");
    QJsonObject conf_obj;
    QFile jsonfile;


public slots:
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();
    void on_loginButton_clicked();
    void on_backtohomeButton_clicked();
    void on_createButton_clicked();
    void on_backtohomeButton2_clicked();
    void on_hideandseeButton_clicked();
    void on_hideandseeButton_2_clicked();
    void on_creatingButton_clicked();
    void on_GoButton_clicked();
    void on_loginingButton_clicked();
    void on_copyidButton_clicked();

private slots:
    void on_passEdit_returnPressed();
    void on_idEdit_returnPressed();
};
#endif
