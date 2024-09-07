#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

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


void MainWindow::on_Submit_button_clicked()
{
    QString first_name = ui->FirstName_line_edit->text();
    QString last_name = ui->LastName_line_edit->text();
    QString feedback = ui->Reason_line_edit->text();

    if (first_name.isEmpty() || last_name.isEmpty() || feedback.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in the first name, last name, and feedback fields.");
        return;
    }

    QString rating;
    if (ui->Excellent_radio_button->isChecked()) {
        rating = "Excellent";
    } else if (ui->VeryGood_radio_button->isChecked()) {
        rating = "Very good";
    } else if (ui->Good_radio_button->isChecked()) {
        rating = "Good";
    } else if (ui->Average_radio_button->isChecked()) {
        rating = "Average";
    } else if (ui->Poor_radio_button->isChecked()) {
        rating = "Poor";
    } else {
        QMessageBox::warning(this, "Input Error", "Please select a rating.");
        return;
    }

    QMessageBox::information(this, "Feedback Received", "Thank you, " + first_name + "!\nYour feedback: " + feedback + "\nRating: " + rating);
}
