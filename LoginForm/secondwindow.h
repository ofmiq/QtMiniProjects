#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    void SetUserName(QString username);
    ~SecondWindow();

private slots:
    void on_Logout_button_clicked();

private:
    Ui::SecondWindow *ui;
};

#endif // SECONDWINDOW_H
