#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentOperation(Operation::None)
    , currentValue(0.0)
    , memoryValue(0.0)
    , waitingForOperand(true)
{
    ui->setupUi(this);
    setShortcuts();
    connectButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setShortcuts() {
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
    ui->Button_eq->setShortcut(Qt::Key_Return);
    ui->Button_inv->setShortcut(Qt::Key_R);
    ui->Button_minus->setShortcut(Qt::Key_Minus);
    ui->Button_mult->setShortcut(Qt::Key_Asterisk);
    ui->Button_percent->setShortcut(Qt::Key_Percent);
    ui->Button_plus->setShortcut(Qt::Key_Plus);
    ui->Button_point->setShortcut(Qt::Key_Period);
    ui->Button_pow->setShortcut(Qt::Key_Q);
    ui->Button_sqrt->setShortcut(Qt::Key_At);
    ui->Button_sign->setShortcut(Qt::Key_F9);
}

void MainWindow::connectButtons()
{
    connect(ui->Button_0, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_1, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_2, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_3, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_4, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_5, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_6, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_7, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_8, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_9, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_C, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_CE, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_back, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_div, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_eq, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_inv, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_minus, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_mult, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_percent, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_plus, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_point, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_pow, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_sqrt, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->Button_sign, &QPushButton::clicked, this, &MainWindow::buttonClicked);
}

void MainWindow::buttonClicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString buttonText = button->text();
        if (button == ui->Button_C) {
            clear();
        } else if (button == ui->Button_CE) {
            clearEntry();
        } else if (button == ui->Button_back) {
            backspace();
        } else if (button == ui->Button_plus) {
            performOperation(Operation::Add);
        } else if (button == ui->Button_minus) {
            performOperation(Operation::Subtract);
        } else if (button == ui->Button_mult) {
            performOperation(Operation::Multiply);
        } else if (button == ui->Button_div) {
            performOperation(Operation::Divide);
        } else if (button == ui->Button_eq) {
            calculate();
        } else if (button == ui->Button_pow) {
            exponentiation();
        } else if (button == ui->Button_inv) {
            reciprocal();
        } else if (button == ui->Button_sqrt) {
            squareRoot();
        } else if (button == ui->Button_sign) {
            changeSign();
        } else if (button == ui->Button_percent) {
            percentage();
        } else if (button == ui->Button_point) {
            addDecimalPoint();
        } else {
            digitClicked(buttonText.toInt());
        }
    }
}

void MainWindow::digitClicked(int digit) {
    if (ui->Result_label->text() == "Division by zero" || ui->Result_label->text() == "Invalid input") {
        clear();
    }

    if (waitingForOperand) {
        ui->Result_label->setText(QString::number(digit));
        waitingForOperand = false;
    } else {
        if (ui->Result_label->text() == "0") {
            ui->Result_label->setText(QString::number(digit));
        } else {
            ui->Result_label->setText(ui->Result_label->text() + QString::number(digit));
        }
    }

    currentValue = ui->Result_label->text().toDouble();
}

void MainWindow::clear() {
    currentValue = 0.0;
    memoryValue = 0.0;
    currentOperation = Operation::None;
    waitingForOperand = true;
    ui->Result_label->setText("0");
    ui->History_label->setText("");
}

void MainWindow::clearEntry() {
    currentValue = 0.0;
    ui->Result_label->setText("0");
}

void MainWindow::backspace() {
    QString text = ui->Result_label->text();
    if (text.length() > 1) {
        text.chop(1);
    } else {
        text = "0";
    }
    currentValue = text.toDouble();
    ui->Result_label->setText(text);
}

void MainWindow::performOperation(Operation operation) {
    if (!waitingForOperand) {
        calculate();
    }
    memoryValue = currentValue;
    currentOperation = operation;
    waitingForOperand = true;
    ui->History_label->setText(QString::number(memoryValue) + " " + operationToString(currentOperation));
}

void MainWindow::calculate() {
    if (currentOperation == Operation::None) {
        return;
    }

    double result = memoryValue;

    switch(currentOperation) {
    case Operation::Add:
        result += currentValue;
        break;
    case Operation::Subtract:
        result -= currentValue;
        break;
    case Operation::Multiply:
        result *= currentValue;
        break;
    case Operation::Divide:
        if (currentValue) {
            result /= currentValue;
        } else {
            ui->Result_label->setText("Division by zero");
            return;
        }
        break;
    default:
        break;
    }

    ui->Result_label->setText(QString::number(result));
    ui->History_label->setText(QString::number(memoryValue) + " " + operationToString(currentOperation) + " " + QString::number(currentValue));
    currentValue = result;
    currentOperation = Operation::None;
    waitingForOperand = true;
}

void MainWindow::exponentiation() {
    double result = currentValue * currentValue;

    ui->Result_label->setText(QString::number(result));
    if (waitingForOperand) {
        ui->History_label->setText(QString::number(currentValue) + " ^ 2");
        currentValue = result;
    } else {
        ui->History_label->setText(ui->History_label->text() + QString::number(currentValue) + " ^ 2");
    }

    currentValue = result;
    waitingForOperand = true;
}

void MainWindow::reciprocal() {
    if (currentValue == 0.0) {
        ui->Result_label->setText("Division by zero");
        return;
    }

    currentValue = 1.0 / currentValue;
    ui->Result_label->setText(QString::number(currentValue));
    ui->History_label->setText("1 / " + QString::number(1.0 / currentValue));
}

void MainWindow::squareRoot() {
    if (currentValue < 0) {
        ui->Result_label->setText("Invalid input");
        return;
    }

    double result = sqrt(currentValue);

    if (waitingForOperand) {
        ui->History_label->setText("√(" + QString::number(currentValue) + ")");
        currentValue = result;
    } else {
        ui->History_label->setText(ui->History_label->text() + "√(" + QString::number(currentValue) + ")");
    }

    ui->Result_label->setText(QString::number(result));
    currentValue = result;

    waitingForOperand = true;
}

void MainWindow::changeSign() {
    currentValue = -currentValue;
    ui->Result_label->setText(QString::number(currentValue));
}

void MainWindow::percentage() {
    if (currentOperation == Operation::None) {
        currentValue = currentValue / 100.0;
        ui->Result_label->setText(QString::number(currentValue));
    } else {
        double percentageValue = memoryValue * (currentValue / 100.0);

        switch (currentOperation) {
        case Operation::Add:
            currentValue = memoryValue + percentageValue;
            break;
        case Operation::Subtract:
            currentValue = memoryValue - percentageValue;
            break;
        case Operation::Multiply:
            currentValue = memoryValue * (currentValue / 100.0);
            break;
        case Operation::Divide:
            if (percentageValue != 0) {
                currentValue = memoryValue / (currentValue / 100.0);
            } else {
                ui->Result_label->setText("Division by zero");
                return;
            }
            break;
        default:
            break;
        }

        ui->Result_label->setText(QString::number(currentValue));
        ui->History_label->setText(QString::number(memoryValue)
                                   + " " + operationToString(currentOperation)
                                   + " " + QString::number(currentValue));
        currentOperation = Operation::None;
        waitingForOperand = true;
    }
}

void MainWindow::addDecimalPoint() {
    if (waitingForOperand) {
        ui->Result_label->setText("0.");
        waitingForOperand = false;
    } else {
        if (!ui->Result_label->text().contains('.')) {
            ui->Result_label->setText(ui->Result_label->text() + '.');
        }
    }
    currentValue = ui->Result_label->text().toDouble();
}

QString MainWindow::operationToString(Operation operation) const {
    switch(operation) {
    case Operation::Add:
        return "+";
    case Operation::Subtract:
        return "-";
    case Operation::Multiply:
        return "*";
    case Operation::Divide:
        return "/";
    default:
        return "";
    }
}
