#include "database.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

bool Database::connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");


    QDir baseDir(QCoreApplication::applicationDirPath());

#ifdef Q_OS_MAC
    baseDir.cdUp();
    baseDir.cdUp();
    baseDir.cdUp();
#elif defined(Q_OS_WIN)
    baseDir.cdUp();
    baseDir.cdUp();
#endif



    QString dbPath = baseDir.absoluteFilePath("prykrasy.db");


    qDebug() << "БАЗА МАЄ БУТИ ТУТ:" << dbPath;

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Помилка відкриття за шляхом:" << dbPath;
        return false;
    }
    return true;
}

bool Database::createTable() {
    QSqlQuery query;

    QString str = "CREATE TABLE IF NOT EXISTS Prykrasy ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "name TEXT, "
                  "material TEXT, "
                  "weight REAL, "
                  "price REAL, "
                  "brand TEXT);";
    return query.exec(str);
}
