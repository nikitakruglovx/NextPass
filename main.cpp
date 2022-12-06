#include "MainWindow.h"
#include "ErrorWindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ErrorWindow errorWindow;

    if(w.db.open()) {
        w.show();
    }
    else {
        errorWindow.show();
    }

    return a.exec();
}
