#include "mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor = new Editor(this);
    editor->setFocus();
    setCentralWidget(editor);

    createMenus();

    menuBar()->setStyleSheet("QWidget { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }"
                             "QMenu { background: rgb(50, 50, 50); }"
                             "QMenu::item { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }");
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this, "Open");
    if (path != "")
    {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        editor->setText(in.readAll());
    }
}

void MainWindow::save()
{
    QString path = QFileDialog::getSaveFileName(this, "Save");
    if (path != "")
    {
        QFile file(path);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out << editor->text() << endl;
    }
}

void MainWindow::quit()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int res = QMessageBox::question(this, "Quit", "Do you really want to exit the program?");
    if (res != QMessageBox::Yes)
        event->ignore();
}

void MainWindow::createMenus()
{
    QAction *action;

    fileMenu = menuBar()->addMenu("File");
    action = fileMenu->addAction("Open");
    connect(action, &QAction::triggered, this, &MainWindow::open);
    action = fileMenu->addAction("Save");
    connect(action, &QAction::triggered, this, &MainWindow::save);
    action = fileMenu->addAction("Quit");
    connect(action, &QAction::triggered, this, &MainWindow::quit);

    editMenu = menuBar()->addMenu("Edit");
    action = editMenu->addAction("Cut");
    connect(action, &QAction::triggered, editor, &Editor::cut);
    action = editMenu->addAction("Copy");
    connect(action, &QAction::triggered, editor, &Editor::copy);
    action = editMenu->addAction("Paste");
    connect(action, &QAction::triggered, editor, &Editor::paste);
    action = editMenu->addAction("Select all");
    connect(action, &QAction::triggered, editor, &Editor::selectAll);
}
