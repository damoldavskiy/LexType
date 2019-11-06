#include "mainwindow.h"
#include "editor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Editor *editor = new Editor(this);
    resize(500, 500);
    editor->setFocus();
    setCentralWidget(editor);
}
