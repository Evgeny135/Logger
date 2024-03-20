#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include "QtSql/QSqlDatabase"
#include <QSqlQuery>

class Logger {
public:
    static void init();;
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void close();
private:
    static QSqlDatabase db;
};

#endif //LOGGER_LOGGER_H
