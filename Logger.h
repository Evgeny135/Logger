#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include "QtSql/QSqlDatabase"
#include <QSqlQuery>
#include "QDebug"

class Logger {
public:
    static void init(QString path);
    static void getLogByDate(QString path, QDateTime firstDate, QDateTime secondDate);
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void createError();
    static void close();
    static void openDb();
    static void addRecord(QString textId, QString type, QString message);
    static void getLogByDateWithFilters(QString path, QString filters);
    static void setPath(QString path);
    static void setDatabaseName(QString path);
    static void saveDatabase(QString path, QString filters);
private:
    static QSqlDatabase db;

};

#endif //LOGGER_LOGGER_H
