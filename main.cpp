#include "mainwindow.h"
#include "errorwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    errorwindow error_window;

    if(!w.db.open()) {
        error_window.show();
    }
    else {
        w.show();
    }

    return a.exec();
}
