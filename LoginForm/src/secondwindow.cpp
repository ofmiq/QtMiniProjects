#include "secondwindow.h"
#include "ui_secondwindow.h"

SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::SetUserName(QString username)
{
    ui->Welcome_label->setText("Welcome " + username + "!");
}

void SecondWindow::on_Logout_button_clicked()
{
    parentWidget()->show();
    this->close();
}
