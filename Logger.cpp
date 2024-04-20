
#include <QDateTime>
#include "Logger.h"
#include "iostream"
#include "QFile"


QSqlDatabase Logger::db;

void Logger::openDb() {
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }
}

void Logger::init(QString path) {
    Logger::db = QSqlDatabase::addDatabase("QSQLITE");
    Logger::db.setDatabaseName(path);
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }

    QSqlQuery query(
            "CREATE TABLE IF NOT EXISTS Log (id INTEGER PRIMARY KEY AUTOINCREMENT,text_id TEXT,date_time TEXT,type_msg TEXT,msg TEXT)");

    if (!query.exec()) {
        qDebug() << "Eror with add";
    }

    Logger::db.close();

    qInstallMessageHandler(Logger::customMessageHandler);
}

void Logger::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }

    QSqlQuery query;
    QString typeMsg;

    switch (type) {
        case QtInfoMsg:
            typeMsg = "INF";
            break;
        case QtDebugMsg:
            typeMsg = "DBG";
            break;
        case QtWarningMsg:
            typeMsg = "WRN";
            break;
        case QtCriticalMsg:
            typeMsg = "CRT";
            break;
        case QtFatalMsg:
            typeMsg = "FTL";
            break;
    }

    QString strInsert = "INSERT INTO Log(text_id,date_time,type_msg,msg) VALUES (:text_id,:date_time,:type_msg,:msg)";

    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    query.prepare(strInsert);
    query.bindValue(":text_id", context.category);
    query.bindValue(":date_time", dateTime);
    query.bindValue(":type_msg", typeMsg);
    query.bindValue(":msg", msg);

    if (!query.exec()) {
        qDebug() << "Eror with add";
    }

    Logger::db.close();

}

void Logger::addRecord(QString textId, QString type, QString message) {
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }

    QSqlQuery query;

    QString strInsert = "INSERT INTO Log(text_id,date_time,type_msg,msg) VALUES (:text_id,:date_time,:type_msg,:msg)";

    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    query.prepare(strInsert);
    query.bindValue(":text_id", textId);
    query.bindValue(":date_time", dateTime);
    query.bindValue(":type_msg", type);
    query.bindValue(":msg", message);

    if (!query.exec()) {
        qDebug() << "Eror with add";
    }

    Logger::db.close();
}

void Logger::getLogByDate(QString path, QDateTime firstDate, QDateTime secondDate) {
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }

    QFile logFile;
    QSqlQuery query;
    QString format = "yyyy-MM-dd HH:mm:ss";

    logFile.setFileName(path);
    logFile.open(QIODevice::Append | QIODevice::Text);
    logFile.resize(0);

//    '2024-03-31 22:56:00'

    QString strInsert = "SELECT text_id, date_time,type_msg,msg\n"
                        "FROM Log\n"
                        "WHERE datetime(date_time) BETWEEN :dateFirst AND :dateSecond";

    query.prepare(strInsert);
    query.bindValue(":dateFirst", firstDate.toString(format));
    query.bindValue(":dateSecond", secondDate.toString(format));

    if (!query.exec()) {
        qDebug() << "Error get data";
    }


    while (query.next()) {
        QString log = QObject::tr("%1 | %2 | %3 | %4\n").
                arg(query.value(0).toString()).
                arg(query.value(1).toString()).
                arg(query.value(2).toString()).
                arg(query.value(3).toString());
        logFile.write(log.toLocal8Bit());
    }

    logFile.close();
    Logger::db.close();
}

void Logger::getLogByDateWithFilters(QString path, QString filters) {
    if (!Logger::db.open()) {
        qDebug() << "Open Error";
    }

    QFile logFile;
    QSqlQuery query;
    QString format = "yyyy-MM-dd HH:mm:ss";

    logFile.setFileName(path);
    logFile.open(QIODevice::Append | QIODevice::Text);
    logFile.resize(0);

    QString strInsert;

    if (!filters.isEmpty()) {
        strInsert = "SELECT text_id, date_time,type_msg,msg\n"
                    "FROM Log\n"
                    "WHERE " + filters + " ;";
    }else{
        strInsert = "SELECT text_id, date_time,type_msg,msg\n"
                    "FROM Log\n";
    }

    query.prepare(strInsert);

    if (!query.exec()) {
        qDebug() << "Error get data";
    }


    while (query.next()) {
        QString log = QObject::tr("%1 | %2 | %3 | %4\n").
                arg(query.value(0).toString()).
                arg(query.value(1).toString()).
                arg(query.value(2).toString()).
                arg(query.value(3).toString());
        logFile.write(log.toLocal8Bit());
    }

    logFile.close();
    Logger::db.close();
}

void Logger::createError() {
    try {
        throw std::invalid_argument("Mess");
    } catch (const std::exception &e) {
        qCritical() << "Ошибка при делении:" << e.what();
    }
}

void Logger::close() {
    Logger::db.close();
}
