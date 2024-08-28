#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setShortcuts()
{
    ui->Button_0->setShortcut(Qt::Key_0);
    ui->Button_1->setShortcut(Qt::Key_1);
    ui->Button_2->setShortcut(Qt::Key_2);
    ui->Button_3->setShortcut(Qt::Key_3);
    ui->Button_4->setShortcut(Qt::Key_4);
    ui->Button_5->setShortcut(Qt::Key_5);
    ui->Button_6->setShortcut(Qt::Key_6);
    ui->Button_7->setShortcut(Qt::Key_7);
    ui->Button_8->setShortcut(Qt::Key_8);
    ui->Button_9->setShortcut(Qt::Key_9);
    ui->Button_C->setShortcut(Qt::Key_Escape);
    ui->Button_CE->setShortcut(Qt::Key_Delete);
    ui->Button_back->setShortcut(Qt::Key_Backspace);
    ui->Button_div->setShortcut(Qt::Key_Slash);
    ui->Button_eq->setShortcut(Qt::Key_Enter);
    ui->Button_inv->setShortcut(Qt::Key_R);
    ui->Button_minus->setShortcut(Qt::Key_Minus);
    ui->Button_mult->setShortcut(Qt::Key_Asterisk);
    ui->Button_percent->setShortcut(Qt::Key_Percent);
    ui->Button_plus->setShortcut(Qt::Key_Plus);
    ui->Button_point->setShortcut(Qt::Key_Period);
    ui->Button_pow->setShortcut(Qt::Key_Q);
    ui->Button_sqrt->setShortcut(Qt::Key_At);
    ui->button_sign->setShortcut(Qt::Key_F9);
}

