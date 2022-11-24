#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMovie>
#include <random>
#include <QCryptographicHash>
#include <QTimer>
#include <QSqlQuery>
#include <QJsonDocument>
#include <iostream>
#include <fstream>
#include <QFileInfo>
#include <QFile>
#include <QClipboard>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setObjectName("borderframeless");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

    db = QSqlDatabase::addDatabase(DBMS);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USERNAME);
    db.setHostName(DB_HOST);
    db.setPort(DB_PORT);
    db.setPassword(DB_PASSWORD);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    curs = event->globalPos();

}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    new_curs = QPoint(event->globalPos() - curs);
    move(x() + new_curs.x(), y() + new_curs.y());
    curs = event->globalPos();

}

void MainWindow::on_closeButton_clicked() {
    this->close();
}

void MainWindow::on_minimizeButton_clicked() {
    this->showMinimized();
}

void MainWindow::on_loginButton_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
    ui->label_3->setMovie(pic);
    ui->label_3->setAlignment(Qt::AlignCenter);
    ui->label_3->setGeometry(150, 10, 200, 200);
    pic->start();
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    if(load_profile.exists() && load_profile.isFile()) {
        f.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val;
        val = f.readAll();
        f.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QString load_id = json["id"].toString();
        ui->idEdit->setText(load_id);
    }
}

void MainWindow::on_backtohomeButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_backtohomeButton2_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_createButton_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    generationId();
    ui->label_4->setMovie(pic_2);
    ui->label_4->setAlignment(Qt::AlignCenter);
    ui->label_4->setGeometry(150, 10, 200, 200);
    pic_2->start();
}

void MainWindow::on_hideandseeButton_clicked() {
    if (ui->hideandseeButton->isChecked()) {
        ui->createPassword->setEchoMode(QLineEdit::Password);
        ui->createrePassword->setEchoMode(QLineEdit::Password);
        ui->hideandseeButton->setIcon(QIcon(":/new/icon/Resources/assets/icon/hidden.png"));
    }
    else {
        ui->createPassword->setEchoMode(QLineEdit::Normal);
        ui->createrePassword->setEchoMode(QLineEdit::Normal);
        ui->hideandseeButton->setIcon(QIcon(":/new/icon/Resources/assets/icon/eye.png"));
    }
}

void MainWindow::on_hideandseeButton_2_clicked() {
    if (ui->hideandseeButton_2->isChecked()) {
        ui->passEdit->setEchoMode(QLineEdit::Normal);
        ui->hideandseeButton_2->setIcon(QIcon(":/new/icon/Resources/assets/icon/eye.png"));
    }
    else {
        ui->passEdit->setEchoMode(QLineEdit::Password);
        ui->hideandseeButton_2->setIcon(QIcon(":/new/icon/Resources/assets/icon/hidden.png"));
    }
}

int MainWindow::generationId() {
    srand(time(0));
    int x = 0;
    x = rand() % (1000000 - 9000000 + 1) + 1000000;
    return x;

}

void MainWindow::create(int int_id) {
    QString id = QString::number(int_id);
    QString passValue = ui->createPassword->text();
    QByteArray passByte = passValue.toUtf8();
    QString hashedPass = QCryptographicHash::hash(passByte, QCryptographicHash::Sha256).toHex();
    QSqlQuery query;
    query.prepare("INSERT INTO users(id, password)VALUES(:id, :password);");
    query.bindValue(":id", id);
    query.bindValue(":password", hashedPass);
    query.exec();
    conf_obj.insert("id", QJsonValue::fromVariant(id));
    qDebug() << id;
    QJsonDocument doc(conf_obj);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    jsonfile.setFileName(PROFILEPATH);
    jsonfile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &jsonfile );
    stream << jsonString;
    jsonfile.close();
}

void MainWindow::on_creatingButton_clicked() {
    if (ui->createPassword->text() == "" || ui->createrePassword->text() == "") {
        ui->createPassword->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        ui->createrePassword->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        QTimer::singleShot(3000, [this] ()
        {
            ui->createPassword->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
            ui->createrePassword->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
        });

    }
    else {
        if (ui->createPassword->text() == ui->createrePassword->text()) {
            QFileInfo checking_profile(PROFILEPATH);
            QFile f(PROFILEPATH);
            if(checking_profile.exists() && checking_profile.isFile()) {
                ui->label_6->setText("               Sorry, you can't have multiple accounts at once.");
            }
            else {
                f.open(QIODevice::WriteOnly | QIODevice::Text);
                if (f.isOpen()) {
                    int id = generationId();
                    QSqlQuery query;
                    QString query_id = QString::number(id);
                    query.exec("SELECT * FROM users WHERE ID =\"" + query_id + "\"");
                    if (query.next() == false) {
                        create(id);
                        ui->label_8->setMovie(pic_success);
                        ui->label_8->setAlignment(Qt::AlignCenter);
                        ui->label_8->setGeometry(150, 10, 200, 200);
                        pic_success->start();
                        QTimer::singleShot(1000, [this] ()
                        {
                            ui->stackedWidget->setCurrentIndex(2);
                            ui->label_9->setText("Creating success!");
                        });
                    }
                    else {
                        while (query.next() == false) {
                            id = generationId();
                            break;
                            create(id);
                        }
                    }
                }
            }
        }
        else {
            ui->createrePassword->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
            QTimer::singleShot(3000, [this] ()
            {
                ui->createrePassword->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
            });
        }
    }
}

void MainWindow::systemLogin() {
    if (ui->idEdit->text() == "" || ui->passEdit->text() == "") {
        ui->idEdit->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        ui->passEdit->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
        QTimer::singleShot(3000, [this] ()
        {
            ui->idEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
            ui->passEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
        });

    }
    else {
        QSqlQuery query;
        QString passValue = ui->passEdit->text();
        QByteArray passByte = passValue.toUtf8();
        QString HDpass_value = QCryptographicHash::hash(passByte, QCryptographicHash::Sha256).toHex();
        QString id = ui->idEdit->text();
        if (!query.exec("SELECT password FROM users WHERE id =\'"+id+"\'")) {
            qDebug() << query.lastError();
        }
        else {
            while (query.next()) {
                QString pass_value = query.value(0).toString();
                qDebug() << HDpass_value;
                qDebug() << pass_value;
                if (HDpass_value != pass_value) {
                    ui->passEdit->setStyleSheet("border: 1px solid rgb(255, 74, 92); border-radius: 14px; font: 18pt 'Nirmala UI'; color: rgb(255, 74, 92);");
                    QTimer::singleShot(3000, [this] ()
                    {
                        ui->idEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
                        ui->passEdit->setStyleSheet("border: 1px solid '#30C193'; border-radius: 14px; font: 18pt 'Nirmala UI'; color: '#30C193';");
                    });
                }
                else {
                    delete pic;
                    delete pic_2;
                    delete pic_success;
                    this->close();
                    home_window->show();
                }
                break;
            }
        }
    }
}

void MainWindow::on_GoButton_clicked() {
    ui->label_3->setMovie(pic);
    ui->label_3->setAlignment(Qt::AlignCenter);
    ui->label_3->setGeometry(150, 10, 200, 200);
    pic->start();
    ui->stackedWidget->setCurrentIndex(4);
    QFileInfo load_profile(PROFILEPATH);
    QFile f(PROFILEPATH);
    if(load_profile.exists() && load_profile.isFile()) {
        f.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val;
        val = f.readAll();
        f.close();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject json = doc.object();
        QString load_id = json["id"].toString();
        ui->idEdit->setText(load_id);
    }
}

void MainWindow::on_copyidButton_clicked() {
    QString id = ui->idEdit->text();
    QApplication::clipboard()->setText(id);
    ui->idEdit->setText("Copied!");
    QTimer::singleShot(1000, [id, this] (){
        ui->idEdit->setText(id);
    });
}

void MainWindow::on_loginingButton_clicked() {
    systemLogin();
}

void MainWindow::on_passEdit_returnPressed() {
    systemLogin();
}

void MainWindow::on_idEdit_returnPressed() {
    systemLogin();
}


