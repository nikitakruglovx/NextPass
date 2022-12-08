#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = nullptr);
    ~ErrorWindow();

private:
    Ui::ErrorWindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint curs;
    QPoint new_curs;

public slots:
    void on_closeButton_clicked();
    void on_OKbutton_clicked();
    void on_reConnectButton_clicked();
};

#endif // ERRORWINDOW_H
