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
    Logger::close();

    return QApplication::exec();
}
