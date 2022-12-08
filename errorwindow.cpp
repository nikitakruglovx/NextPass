#include "ErrorWindow.h"
#include "ui_errorwindow.h"
#include "DataBase.h"
#include "MainWindow.h"

ErrorWindow::ErrorWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    setObjectName("borderframeless");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}

void ErrorWindow::mousePressEvent(QMouseEvent *event) {
    curs = event->globalPos();

}

void ErrorWindow::mouseMoveEvent(QMouseEvent *event) {

    new_curs = QPoint(event->globalPos() - curs);
    move(x() + new_curs.x(), y() + new_curs.y());
    curs = event->globalPos();

}

void ErrorWindow::on_closeButton_clicked() {
    this->close();
}

void ErrorWindow::on_OKbutton_clicked() {
    this->close();
}

void ErrorWindow::on_reConnectButton_clicked() {
    data::DataConnection ConnectMethod;
    MainWindow w;

    if (ConnectMethod.ConnectingDataBase() == true) {
        w.show();
    }
    else {
        ui->label_3->setText("Failed to reconnect");
    }

}
