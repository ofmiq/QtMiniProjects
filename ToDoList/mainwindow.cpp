#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPainter>
#include <QCheckBox>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTable();
    addTask();
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

// temporary implementation
void MainWindow::addTask() {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // Description
    QTableWidgetItem *descriptionItem = new QTableWidgetItem("Sample Task Description");
    descriptionItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(row, 0, descriptionItem);

    // Priority
    QTableWidgetItem *priorityItem = new QTableWidgetItem("HIGH");
    priorityItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(row, 1, priorityItem);

    // Deadline
    QTableWidgetItem *deadlineItem = new QTableWidgetItem("2024-01-01");
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
    QPushButton *deleteButton = new QPushButton;
    deleteButton->setText("🗑️");
    ui->tableWidget->setCellWidget(row, 4, deleteButton);
}

