#include "passcontroll.h"
#include "ui_passcontroll.h"

passcontroll::passcontroll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passcontroll)
{
    ui->setupUi(this);
    setObjectName("borderframeless");
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
}

passcontroll::~passcontroll()
{
    delete ui;
}

void passcontroll::mousePressEvent(QMouseEvent *event) {
    curs = event->globalPos();

}

void passcontroll::mouseMoveEvent(QMouseEvent *event) {

    new_curs = QPoint(event->globalPos() - curs);
    move(x() + new_curs.x(), y() + new_curs.y());
    curs = event->globalPos();

}

void passcontroll::on_closeButton_clicked() {
    this->close();
}

void passcontroll::on_minimizeButton_clicked() {
    this->showMinimized();
}
