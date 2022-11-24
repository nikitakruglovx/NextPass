#include "errorwindow.h"
#include "ui_errorwindow.h"

errorwindow::errorwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::errorwindow)
{
    ui->setupUi(this);
    setObjectName("borderframeless");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
}

errorwindow::~errorwindow()
{
    delete ui;
}

void errorwindow::mousePressEvent(QMouseEvent *event) {
    curs = event->globalPos();

}

void errorwindow::mouseMoveEvent(QMouseEvent *event) {

    new_curs = QPoint(event->globalPos() - curs);
    move(x() + new_curs.x(), y() + new_curs.y());
    curs = event->globalPos();

}

void errorwindow::on_closeButton_clicked() {
    this->close();
}
