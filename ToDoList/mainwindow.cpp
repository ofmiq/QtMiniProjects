#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

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

bool MainWindow::connectToDatabase()
{
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

void MainWindow::setupTable()
{
    ui->tableWidget->setColumnCount(5);

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

void MainWindow::setupButtons()
{
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::addTask);
}

void MainWindow::addTask()
{
    QString description = ui->lineEdit->text();
    QString priority = ui->comboBox->currentText();
    QDate deadline = ui->dateEdit->date();

    if (description.isEmpty() || priority == "Priority") {
        QMessageBox::warning(this, "Input Error", "Please fill in the task description and select a priority.");
        return;
    }

    Task newTask(0, description, priority, deadline, false);
    if (newTask.saveToDatabase()) {
        addTaskToUI(newTask);
    } else {
        QMessageBox::critical(this, "Database Error", "Failed to add task to database.");
    }

    ui->lineEdit->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
}

void MainWindow::addTaskToUI(Task task)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QTableWidgetItem *descriptionItem = new QTableWidgetItem(task.getDescription());
    descriptionItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, 0, descriptionItem);

    QTableWidgetItem *priorityItem = new QTableWidgetItem(task.getPriority());
    priorityItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, priorityItem);

    QTableWidgetItem *deadlineItem = new QTableWidgetItem(task.getDeadline().toString("yyyy-MM-dd"));
    deadlineItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, deadlineItem);

    QCheckBox *checkBox = new QCheckBox;
    checkBox->setChecked(task.isCompleted());
    QWidget *checkBoxContainer = new QWidget;
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxContainer);
    checkBoxLayout->addWidget(checkBox);
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);
    checkBoxContainer->setLayout(checkBoxLayout);
    ui->tableWidget->setCellWidget(row, 3, checkBoxContainer);

    connectTaskUIElements(row, task);
}

void MainWindow::connectTaskUIElements(int row, Task task)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(row, 3)->layout()->itemAt(0)->widget());
    QPushButton *deleteButton = new QPushButton("🗑️");

    connect(checkBox, &QCheckBox::stateChanged, this, [task](int state) mutable {
        task.setCompleted(state == Qt::Checked);
        if (!task.updateInDatabase()) {
            QMessageBox::critical(nullptr, "Database Error", "Failed to update task in database.");
        }
    });

    connect(deleteButton, &QPushButton::clicked, this, [this, row, task]() mutable {
        if (task.deleteFromDatabase()) {
            ui->tableWidget->removeRow(row);
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to delete task from database.");
        }
    });

    ui->tableWidget->setCellWidget(row, 4, deleteButton);
}

void MainWindow::loadTasksFromDatabase()
{
    QSqlQuery query("SELECT id, description, priority, deadline, completed FROM tasks");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString description = query.value(1).toString();
        QString priority = query.value(2).toString();
        QDate deadline = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
        bool completed = query.value(4).toBool();

        Task task(id, description, priority, deadline, completed);
        addTaskToUI(task);
    }
}
