#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

namespace data {

class DataCon : QObject {
    Q_OBJECT

public:
    QString DBMS = "QPSQL";
    QString DB_NAME = "postgres";
    QString DB_USERNAME = "postgres";
    QString DB_PASSWORD = "admin";
    QString DB_HOST = "127.0.0.1";
    int DB_PORT = 5433;

};
}


#endif
