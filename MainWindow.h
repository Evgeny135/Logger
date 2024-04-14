#ifndef LOGGER_MAINWINDOW_H
#define LOGGER_MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QInputDialog>
#include <QComboBox>
#include "QDateTime"
#include "iostream"
#include "Logger.h"
#include "QDialogButtonBox"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow() {
        this->setMinimumSize(800,600);
        QWidget *centralWidget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        textIdentifier = new QLineEdit;
        comboBox = new QComboBox;
        comboBox->addItem("CRT");
        comboBox->addItem("WRN");
        comboBox->addItem("INF");
        comboBox->addItem("DBG");
        comboBox->addItem("FTL");
        message = new QLineEdit;

        QPushButton *button = new QPushButton("Добавить");
        QPushButton *buttonRefresh = new QPushButton("Сбросить фильтры");

        layout->addWidget(textIdentifier);
        layout->addWidget(comboBox);
        layout->addWidget(message);
        layout->addWidget(button);
        layout->addWidget(buttonRefresh);

        QTableView *view = new QTableView(centralWidget);
        layout->addWidget(view);

        model = new QSqlTableModel(this);
        model->setTable("Log");
        model->select();

        view->setModel(model);

        connect(view->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);
        connect(button,&QPushButton::clicked, this,&MainWindow::buttonClick);
        connect(buttonRefresh,&QPushButton::clicked,this,&MainWindow::refreshFilter);

        setCentralWidget(centralWidget);
    }

private slots:
    void refreshFilter(){
        Logger::openDb();
        model->setFilter("");
        model->select();
};
    void buttonClick(){

        Logger::addRecord(textIdentifier->text(),comboBox->currentText(),message->text());
        Logger::openDb();
        model->select();
};
    void onHeaderClicked(int column) {
        Logger::openDb();
        if (column==1){

            bool ok;

            QString text = QInputDialog::getText(this, tr("Фильтр по идентификатору"), tr("Введите идентификатор"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
            } else {
                return;
            }


            QString filter = QString("text_id = '%1'")
                    .arg(text);

            model->setFilter(filter);
            model->select();
        }else if (column == 2) {
            QDateTime startDate = QDateTime::currentDateTime();
            QDateTime endDate = QDateTime::currentDateTime();


            bool ok;
            QString text = QInputDialog::getText(this, tr("Фильтр дат"), tr("Введите начальную дату (yyyy-MM-dd hh:mm:ss):"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                startDate = QDateTime::fromString(text, "yyyy-MM-dd hh:mm:ss");
            } else {
                return;
            }


            text = QInputDialog::getText(this, tr("Фильтр дат"), tr("Введите конечную дату (yyyy-MM-dd hh:mm:ss):"), QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                endDate = QDateTime::fromString(text, "yyyy-MM-dd hh:mm:ss");
            } else {
                return;
            }


            QString filter = QString("datetime(date_time) BETWEEN '%1' AND '%2'")
                    .arg(startDate.toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(endDate.toString("yyyy-MM-dd hh:mm:ss"));

            model->setFilter(filter);
            model->select();
        }else if (column==3){
            QDialog dialog(this);
            QVBoxLayout layout(&dialog);

            QComboBox comboBox;
            comboBox.addItem("CRT");
            comboBox.addItem("WRN");
            comboBox.addItem("INF");
            comboBox.addItem("DBG");
            comboBox.addItem("FTL");

            layout.addWidget(&comboBox);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            layout.addWidget(&buttonBox);


            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);


            if (dialog.exec() == QDialog::Accepted) {

                QString selectedValue = comboBox.currentText();
                QString filter = QString("type_msg = '%1'").arg(selectedValue);
                model->setFilter(filter);
                model->select();
            }
        }
    }

private:
    QSqlTableModel *model;
    QLineEdit *textIdentifier;
    QComboBox* comboBox;
    QLineEdit *message;
};


#endif //LOGGER_MAINWINDOW_H
