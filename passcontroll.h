#ifndef PASSCONTROLL_H
#define PASSCONTROLL_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class passcontroll;
}

class passcontroll : public QWidget
{
    Q_OBJECT

public:
    explicit passcontroll(QWidget *parent = nullptr);
    ~passcontroll();

private:
    Ui::passcontroll *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint curs;
    QPoint new_curs;

public slots:
    void on_closeButton_clicked();
    void on_minimizeButton_clicked();

};

#endif
