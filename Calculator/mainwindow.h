#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void buttonClicked();

private:
    Ui::MainWindow *ui;

    enum Operation {
        None,
        Add,
        Subtract,
        Multiply,
        Divide
    };

    Operation currentOperation;
    double currentValue;
    double memoryValue;
    bool waitingForOperand;

    void setShortcuts();
    void connectButtons();
    void clear();
    void clearEntry();
    void backspace();
    void performOperation(Operation operation);
    void calculate();
    void reciprocal();
    void squareRoot();
    void changeSign();
    void percentage();
    void addDecimalPoint();
    void digitClicked(int digit);

    QString operationToString(Operation operation) const;
};

#endif // MAINWINDOW_H
