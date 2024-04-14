#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QSlider>
#include <QSqlTableModel>
#include <QTableView>
#include <QLineEdit>
#include "Logger.h"
#include "iostream"
#include "QDateTime"
#include "QInputDialog"
#include "QHeaderView"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Logger::init("C:\\Users\\trish\\Desktop\\db.db");

    MainWindow mainWindow;
    mainWindow.show();

//    QDateTime firstDate = QDateTime::fromString("2024-03-31 22:56:00", "yyyy-MM-dd hh:mm:ss");
//    QDateTime secondDate = QDateTime::fromString("2024-03-31 22:57:00", "yyyy-MM-dd hh:mm:ss");
//
//    Logger::getLogByDate("C:\\Users\\trish\\Desktop\\log.log", firstDate,secondDate);
//
//    Logger::createError();
//
    Logger::close();

    return QApplication::exec();
}
