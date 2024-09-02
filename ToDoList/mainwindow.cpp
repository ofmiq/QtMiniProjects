#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!connectToDatabase()) {
        qFatal("Failed to connect to the database.");
    }

    setupTable();
    setupButtons();
    loadTasksFromDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tasks.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        return false;
    }

    QSqlQuery query;
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            description TEXT NOT NULL,
            priority TEXT NOT NULL,
            deadline DATE NOT NULL,
            completed INTEGER NOT NULL DEFAULT 0
        )
    )";

    if (!query.exec(createTableQuery)) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return false;
    }

    return true;
}

void MainWindow::setupTable() {
    ui->tableWidget->setColumnWidth(1, 80);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 50);
    ui->tableWidget->setColumnWidth(4, 50);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    ui->dateEdit->setDate(QDate::currentDate());

}

void MainWindow::setupButtons() {
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::addTask);
}

void MainWindow::addTask() {
    QString description = ui->lineEdit->text();
    QString priority = ui->comboBox->currentText();
    QDate deadline = ui->dateEdit->date();

    if (description.isEmpty() || priority == "Priority") {
        QMessageBox::warning(this, "Input Error", "Please fill in the task description and select a priority.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO tasks (description, priority, deadline, completed) VALUES (:description, :priority, :deadline, 0)");
    query.bindValue(":description", description);
    query.bindValue(":priority", priority);
    query.bindValue(":deadline", deadline.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    int taskId = query.lastInsertId().toInt();
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);
    descriptionItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, 0, descriptionItem);

    QTableWidgetItem *priorityItem = new QTableWidgetItem(priority);
    priorityItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, priorityItem);

    QTableWidgetItem *deadlineItem = new QTableWidgetItem(deadline.toString("yyyy-MM-dd"));
    deadlineItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, deadlineItem);

    QCheckBox *checkBox = new QCheckBox;
    QWidget *checkBoxContainer = new QWidget;
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxContainer);
    checkBoxLayout->addWidget(checkBox);
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);
    checkBoxContainer->setLayout(checkBoxLayout);
    ui->tableWidget->setCellWidget(row, 3, checkBoxContainer);

    connect(checkBox, &QCheckBox::stateChanged, this, [this, taskId](int state) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
        updateQuery.bindValue(":completed", (state == Qt::Checked) ? 1 : 0);
        updateQuery.bindValue(":id", taskId);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Database Error", updateQuery.lastError().text());
        }
    });

    QPushButton *deleteButton = new QPushButton("🗑️");
    connect(deleteButton, &QPushButton::clicked, this, [this, row, taskId]() {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM tasks WHERE id = :id");
        deleteQuery.bindValue(":id", taskId);

        if (deleteQuery.exec()) {
            ui->tableWidget->removeRow(row);
        } else {
            QMessageBox::critical(this, "Database Error", deleteQuery.lastError().text());
        }
    });
    ui->tableWidget->setCellWidget(row, 4, deleteButton);

    ui->lineEdit->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
}


void MainWindow::loadTasksFromDatabase() {
    QSqlQuery query("SELECT id, description, priority, deadline, completed FROM tasks");

    while (query.next()) {
        int taskId = query.value("id").toInt();
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *descriptionItem = new QTableWidgetItem(query.value("description").toString());
        descriptionItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 0, descriptionItem);

        QTableWidgetItem *priorityItem = new QTableWidgetItem(query.value("priority").toString());
        priorityItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 1, priorityItem);

        QTableWidgetItem *deadlineItem = new QTableWidgetItem(query.value("deadline").toString());
        deadlineItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 2, deadlineItem);

        QCheckBox *checkBox = new QCheckBox;
        checkBox->setChecked(query.value("completed").toBool());
        QWidget *checkBoxContainer = new QWidget;
        QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxContainer);
        checkBoxLayout->addWidget(checkBox);
        checkBoxLayout->setAlignment(Qt::AlignCenter);
        checkBoxLayout->setContentsMargins(0, 0, 0, 0);
        checkBoxContainer->setLayout(checkBoxLayout);
        ui->tableWidget->setCellWidget(row, 3, checkBoxContainer);

        connect(checkBox, &QCheckBox::stateChanged, this, [this, taskId](int state) {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
            updateQuery.bindValue(":completed", (state == Qt::Checked) ? 1 : 0);
            updateQuery.bindValue(":id", taskId);

            if (!updateQuery.exec()) {
                QMessageBox::critical(this, "Database Error", updateQuery.lastError().text());
            }
        });

        QPushButton *deleteButton = new QPushButton("🗑️");
        connect(deleteButton, &QPushButton::clicked, this, [this, row, taskId]() {
            QSqlQuery deleteQuery;
            deleteQuery.prepare("DELETE FROM tasks WHERE id = :id");
            deleteQuery.bindValue(":id", taskId);

            if (deleteQuery.exec()) {
                ui->tableWidget->removeRow(row);
            } else {
                QMessageBox::critical(this, "Database Error", deleteQuery.lastError().text());
            }
        });
        ui->tableWidget->setCellWidget(row, 4, deleteButton);
    }
}
