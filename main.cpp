#include "MainWindow.h"
#include "ErrorWindow.h"
#include "DataBase.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ErrorWindow errorWindow;
    data::DataConnection D;

    if (D.ConnectingDataBase() == true) {
        w.show();
    }
    else {
        errorWindow.show();
    }

    return a.exec();
}
