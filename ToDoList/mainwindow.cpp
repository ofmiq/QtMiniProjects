#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPainter>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTable();
    setupButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTable() {
    ui->dateEdit->setDate(QDate::currentDate());

    ui->tableWidget->setColumnWidth(0, 165);
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 70);
    ui->tableWidget->setColumnWidth(3, 60);
    ui->tableWidget->setColumnWidth(4, 45);
}

void MainWindow::setupButtons() {
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::addTask);
}

void MainWindow::addTask() {
    // Gather input values
    QString description = ui->lineEdit->text();
    QString priority = ui->comboBox->currentText();
    QDate deadline = ui->dateEdit->date();

    if (description.isEmpty() || priority == "Priority") {
        QMessageBox::warning(this, "Input Error", "Please fill in the task description and select a priority.");
        return;
    }

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Description
    QTableWidgetItem *descriptionItem = new QTableWidgetItem(description);
    descriptionItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, 0, descriptionItem);

    // Priority
    QTableWidgetItem *priorityItem = new QTableWidgetItem(priority);
    priorityItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, priorityItem);

    // Deadline
    QTableWidgetItem *deadlineItem = new QTableWidgetItem(deadline.toString("yyyy-MM-dd"));
    deadlineItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 2, deadlineItem);

    // Complete checkbox
    QCheckBox *checkBox = new QCheckBox;
    QWidget *checkBoxContainer = new QWidget;
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxContainer);
    checkBoxLayout->addWidget(checkBox);
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);
    checkBoxContainer->setLayout(checkBoxLayout);
    ui->tableWidget->setCellWidget(row, 3, checkBoxContainer);

    // Delete button
    QPushButton *deleteButton = new QPushButton("🗑️");
    connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
        int row = ui->tableWidget->indexAt(deleteButton->pos()).row();
        ui->tableWidget->removeRow(row);
    });
    ui->tableWidget->setCellWidget(row, 4, deleteButton);

    // Clear input fields after adding the task
    ui->lineEdit->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
}

