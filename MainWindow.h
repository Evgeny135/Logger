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
#include <QDateTimeEdit>
#include <QFileDialog>
#include "QDateTime"
#include "iostream"
#include "Logger.h"
#include "QDialogButtonBox"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow() {
        this->setMinimumSize(800, 600);
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
        comboBox->setEditable(true);

        QLabel *labelMessage = new QLabel("Сообщение:");

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
        connect(button, &QPushButton::clicked, this, &MainWindow::buttonClick);
        connect(buttonRefresh, &QPushButton::clicked, this, &MainWindow::refreshFilter);
        connect(buttonSave, &QPushButton::clicked, this, &MainWindow::buttonSave);

        textFilters = new QLabel;

        textFilters->setText("");

        layout->addWidget(textFilters);

        setCentralWidget(centralWidget);
    }

private slots:

    void refreshFilter() {
        Logger::openDb();
        filter[0] = "";
        filter[1] = "";
        filter[2] = "";
        filterText[0] = "";
        filterText[1] = "";
        filterText[2] = "";
        textFilters->setText("");
        model->setFilter("");
        model->select();
    };

    void buttonClick() {

        Logger::addRecord(textIdentifier->text(), comboBox->currentText(), message->text());
        Logger::openDb();
        model->select();
    };

    void buttonSave() {
        QString filePath = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "C:/Users/trish/Desktop/log.log", tr("Лог-файлы (*.log);;Все файлы (*)"));
        if (!filePath.isEmpty()) {
            Logger::getLogByDateWithFilters(filePath.toUtf8().constData(), (filter[0] + filter[1] + filter[2]).toUtf8().constData());
        }

//        Logger::getLogByDateWithFilters("C:\\Users\\trish\\Desktop\\log.log",
//                                        (filter[0] + filter[1] + filter[2]).toUtf8().constData());
    };

    void onHeaderClicked(int column) {
        Logger::openDb();
        if (column == 1) {

            bool ok;

            QString text = QInputDialog::getText(this, tr("Фильтр по идентификатору"), tr("Введите идентификатор"),
                                                 QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
            } else {
                return;
            }

            filter[0] = QString("text_id = '%1'")
                    .arg(text);

            filterText[0] = "Текстовый идентификатор " + text;

            model->setFilter(getFilterString());
            model->select();
        } else if (column == 2) {
            QDialog dialog(this);
            QVBoxLayout layout(&dialog);

            QDateTimeEdit startDateEdit;
            startDateEdit.setDateTime(QDateTime::currentDateTime());

            QDateTimeEdit endDateEdit;
            endDateEdit.setDateTime(QDateTime::currentDateTime());

            layout.addWidget(new QLabel("Начальная дата и время:"));
            layout.addWidget(&startDateEdit);
            layout.addWidget(new QLabel("Конечная дата и время:"));
            layout.addWidget(&endDateEdit);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            layout.addWidget(&buttonBox);

            connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

            if (dialog.exec() == QDialog::Accepted) {
                QDateTime startDate = startDateEdit.dateTime();
                QDateTime endDate = endDateEdit.dateTime();

                filter[1] = QString("datetime(date_time) BETWEEN '%1' AND '%2'")
                        .arg(startDate.toString("yyyy-MM-dd hh:mm:ss"))
                        .arg(endDate.toString("yyyy-MM-dd hh:mm:ss"));

                filterText[1] = " Даты:  " + startDate.toString("yyyy-MM-dd hh:mm:ss") + " " +
                                endDate.toString("yyyy-MM-dd hh:mm:ss");

                model->setFilter(getFilterString());
                model->select();
            }
        } else if (column == 3) {
            QDialog dialog(this);
            QVBoxLayout layout(&dialog);

            QComboBox comboBox;
            comboBox.setEditable(true);
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

                filter[2] = QString("type_msg = '%1'").arg(selectedValue);

                filterText[2] = " Тип: " + selectedValue;

                model->setFilter(getFilterString());

                model->select();
            }
        }

        textFilters->setText(filterText[0] + " " + filterText[1] + " " + filterText[2]);
    }

private:
    QSqlTableModel *model;
    QLineEdit *textIdentifier;
    QComboBox *comboBox;
    QLineEdit *message;
    QString filter[3] = {"", "", ""};
    QString filterText[3];
    QLabel *textFilters;

    QString getFilterString() {
        QString composedFilter;
        for (int i = 0; i < 3; ++i) {
            if (!filter[i].isEmpty()) {
                if (!composedFilter.isEmpty())
                    composedFilter += " AND ";
                composedFilter += filter[i];
            }
        }
        return composedFilter;
    };
};


#endif //LOGGER_MAINWINDOW_H
