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
#include <QLabel>
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

        QLabel *labelIdentifier = new QLabel("Текстовый идентификатор:");

        textIdentifier = new QLineEdit;

        QLabel *labelTypeMessage = new QLabel("Тип сообщения:");

        comboBox = new QComboBox;
        comboBox->addItem("Критическая");
        comboBox->addItem("Предупреждение");
        comboBox->addItem("Информационное");
        comboBox->addItem("Отладочное");
        comboBox->addItem("Фатальная");

        QLabel *labelMessage = new QLabel("Тип сообщения:");

        message = new QLineEdit;

        QPushButton *button = new QPushButton("Добавить");
        QPushButton *buttonRefresh = new QPushButton("Сбросить фильтры");
        QPushButton *buttonSave = new QPushButton("Сохранить в файл");

        layout->addWidget(labelIdentifier);
        layout->addWidget(textIdentifier);
        layout->addWidget(labelTypeMessage);
        layout->addWidget(comboBox);
        layout->addWidget(labelMessage);
        layout->addWidget(message);
        layout->addWidget(button);
        layout->addWidget(buttonRefresh);
        layout->addWidget(buttonSave);

        QTableView *view = new QTableView(centralWidget);
        layout->addWidget(view);

        model = new QSqlTableModel(this);
        model->setTable("Log");
        model->setHeaderData(0, Qt::Horizontal, "Ид");
        model->setHeaderData(1, Qt::Horizontal, "Текстовый идентификатор");
        model->setHeaderData(2, Qt::Horizontal, "Дата");
        model->setHeaderData(3, Qt::Horizontal, "Тип сообщения");
        model->setHeaderData(4, Qt::Horizontal, "Сообщение");
        model->select();

        view->setModel(model);

        connect(view->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);
        connect(button,&QPushButton::clicked, this,&MainWindow::buttonClick);
        connect(buttonRefresh,&QPushButton::clicked,this,&MainWindow::refreshFilter);
        connect(buttonSave,&QPushButton::clicked,this,&MainWindow::buttonSave);

        textFilters = new QLabel;

        textFilters->setText(filterText);

        layout->addWidget(textFilters);

        setCentralWidget(centralWidget);
    }

private slots:
    void refreshFilter(){
        Logger::openDb();
        filter = "";
        filterText = "";
        textFilters->setText(filterText);
        model->setFilter(filter);
        model->select();
};
    void buttonClick(){

        Logger::addRecord(textIdentifier->text(),comboBox->currentText(),message->text());
        Logger::openDb();
        model->select();
};

    void buttonSave(){
        Logger::getLogByDateWithFilters("C:\\Users\\trish\\Desktop\\log.log",filter.toUtf8().constData());
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

            if (!filter.isEmpty()) filter+= QString(" AND ");

            filter += QString("text_id = '%1'")
                    .arg(text);

            filterText += "Текстовый идентификатор "+text;

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

            if (!filter.isEmpty()) filter+= QString(" AND ");

            filter += QString("datetime(date_time) BETWEEN '%1' AND '%2'")
                    .arg(startDate.toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(endDate.toString("yyyy-MM-dd hh:mm:ss"));

            filterText += " Даты:  "+startDate.toString("yyyy-MM-dd hh:mm:ss") + " " + endDate.toString("yyyy-MM-dd hh:mm:ss");

            model->setFilter(filter);
            model->select();
        }else if (column==3){
            QDialog dialog(this);
            QVBoxLayout layout(&dialog);

            QComboBox comboBox;
            comboBox.addItem("Критическая");
            comboBox.addItem("Предупреждение");
            comboBox.addItem("Информационное");
            comboBox.addItem("Отладочное");
            comboBox.addItem("Фатальная");

            layout.addWidget(&comboBox);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            layout.addWidget(&buttonBox);


            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);


            if (dialog.exec() == QDialog::Accepted) {

                QString selectedValue = comboBox.currentText();
                if (!filter.isEmpty()) filter+= QString(" AND ");
                filter += QString("type_msg = '%1'").arg(selectedValue);

                filterText += " Тип: " + selectedValue;

                model->setFilter(filter);
                model->select();
            }
        }

        textFilters->setText(filterText);
    }

private:
    QString filter;
    QSqlTableModel *model;
    QLineEdit *textIdentifier;
    QComboBox* comboBox;
    QLineEdit *message;
    QString filterText = "";
    QLabel *textFilters;
};


#endif //LOGGER_MAINWINDOW_H
