#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLabelWithName() {
  QString name = "Hello " + ui->lineEdit->text() + "!";
  ui->label->setText(name);
}

void MainWindow::on_pushButton_clicked()
{
  updateLabelWithName();
}

void MainWindow::on_lineEdit_editingFinished()
{
  updateLabelWithName();
}

