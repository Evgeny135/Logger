
#include <QDateTime>
#include "Logger.h"

QSqlDatabase Logger::db;

void Logger::init() {
    Logger::db = QSqlDatabase::addDatabase("QSQLITE");
    Logger::db.setDatabaseName("C:\\Users\\trish\\Desktop\\db.db");

    qInstallMessageHandler(Logger::customMessageHandler);
}

void Logger::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    if (!Logger::db.open()){
        qDebug() << "Open Error";
    }


    QSqlQuery query;
    QString typeMsg;

    switch(type){
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
    query.bindValue(":text_id",context.category);
    query.bindValue(":date_time",dateTime);
    query.bindValue(":type_msg",typeMsg);
    query.bindValue(":msg",msg);

    if (!query.exec()) {
        qDebug() << "Eror with add";
    }

    Logger::db.close();

}

void Logger::close() {
    Logger::db.close();
}