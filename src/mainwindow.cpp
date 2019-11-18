#include "mainwindow.h"

#include "editor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Editor *editor = new Editor(this);
    editor->setFocus();
    setCentralWidget(editor);
}
