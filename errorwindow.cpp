#include "ErrorWindow.h"
#include "ui_errorwindow.h"

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
