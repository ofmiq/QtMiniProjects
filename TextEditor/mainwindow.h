#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void newDocument();
    void openDocument();
    bool saveDocument();
    void exitApp();

private:
    void createActions();
    bool isDocumentSaved();

    Ui::MainWindow *ui;
    QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
