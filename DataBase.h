#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

namespace data {

class DataConnection : QObject {
    Q_OBJECT

public:
    QSqlDatabase db;
    QString DBMS = "QPSQL";
    QString DB_NAME = "postgres";
    QString DB_USERNAME = "postgres";
    QString DB_PASSWORD = "admin";
    QString DB_HOST = "127.0.0.1";
    int DB_PORT = 5433;

public:
    bool ConnectingDataBase() {

        db = QSqlDatabase::addDatabase(DBMS);
        db.setDatabaseName(DB_NAME);
        db.setUserName(DB_USERNAME);
        db.setHostName(DB_HOST);
        db.setPort(DB_PORT);
        db.setPassword(DB_PASSWORD);

        if (db.open()) {
            return true;
        }
        else {
            return false;
        }
    }
};
}


#endif
