#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , textEdit(new QTextEdit(this))
{
    ui->setupUi(this);
    setCentralWidget(textEdit);
    createActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    connect(ui->actionNew_file, &QAction::triggered, this, &MainWindow::newDocument);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openDocument);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveDocument);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitApp);
    connect(ui->actionCut, &QAction::triggered, textEdit, &QTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, textEdit, &QTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, textEdit, &QTextEdit::paste);
    connect(ui->actionUndo, &QAction::triggered, textEdit, &QTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, textEdit, &QTextEdit::redo);
}

bool MainWindow::isDocumentSaved()
{
    if (!textEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveDocument();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void MainWindow::newDocument()
{
    if (isDocumentSaved()) {
        textEdit->clear();
        ui->statusbar->showMessage("New document created.");
    }
}

void MainWindow::openDocument()
{
    if (isDocumentSaved()) {
        const QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                textEdit->setPlainText(in.readAll());
                file.close();
                ui->statusbar->showMessage("File opened.");
            }
        }
    }
}

bool MainWindow::saveDocument()
{
    const QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << textEdit->toPlainText();
            file.close();
            ui->statusbar->showMessage("File saved.");
            return true;
        }
    }
    return true;
}

void MainWindow::exitApp()
{
    if (isDocumentSaved()) {
        close();
    }
}
