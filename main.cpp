#include <QApplication>
#include <QPushButton>
#include "Logger.h"
#include "iostream"
#include "QDateTime"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QPushButton qPushButton;

    qPushButton.resize(100,100);
    qPushButton.click();

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
