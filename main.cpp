#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include "Logger.h"
#include "iostream"
#include "QDateTime"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Logger::init();

    qInfo("Info1");
    qInfo("Info2");
    qInfo("Info3");
    qWarning("Warning");
    qDebug("DBG");
    qCritical("Crit");

    Logger::close();

    return QApplication::exec();
}
