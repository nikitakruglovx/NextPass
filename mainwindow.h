#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int generationId();
    void connectDB();
    void create(int int_id);

    QPoint curs;
    QPoint new_curs;

    QSqlDatabase db;

    QString PROFILEPATH = "../VivaPassLock/profile.json";
    std::string PROFILEPATHsys = "C:/Users/user/Desktop/VivaPassLock/profile.json";

    QString DBMS = "QPSQL";
    QString DB_NAME = "postgres";
    QString DB_USERNAME = "postgres";
    QString DB_PASSWORD = "admin";
    QString DB_HOST = "127.0.0.1";
    int DB_PORT = 5433;

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
    void on_creatingButton_clicked();
    void on_GoButton_clicked();

};
#endif
