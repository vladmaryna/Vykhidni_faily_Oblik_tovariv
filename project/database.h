#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class Database {
public:
    static bool connectToDatabase();
    static bool createTable();
};

#endif
