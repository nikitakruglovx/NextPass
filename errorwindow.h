#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class errorwindow;
}

class errorwindow : public QWidget
{
    Q_OBJECT

public:
    explicit errorwindow(QWidget *parent = nullptr);
    ~errorwindow();

private:
    Ui::errorwindow *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QPoint curs;
    QPoint new_curs;

public slots:
    void on_closeButton_clicked();
};

#endif // ERRORWINDOW_H
