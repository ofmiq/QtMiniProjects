#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "secondwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , secondwindow(new SecondWindow(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_button_clicked()
{
    QString username = ui->Username_line_edit->text();
    QString password = ui->Password_line_edit->text();

    if (username == "login" && password == "password") {
        this->hide();
        secondwindow->SetUserName(username);
        secondwindow->show();
    } else {
        QMessageBox::warning(this, "Login incorrect", "Username or password is incorrect");
    }
}
