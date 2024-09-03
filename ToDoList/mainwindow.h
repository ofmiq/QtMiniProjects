#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QSqlDatabase>
#include "task.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void setupTable();
    void setupButtons();
    bool connectToDatabase();
    void addTask();
    void loadTasksFromDatabase();
    void addTaskToUI(Task task);
    void connectTaskUIElements(int row, Task task);
};

#endif // MAINWINDOW_H
