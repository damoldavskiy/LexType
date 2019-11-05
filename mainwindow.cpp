#include "mainwindow.h"
#include "editor.h"

#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    setStyleSheet("background-color: #323232");
    Editor *editor = new Editor(this);
    resize(500, 500);
    editor->move(20, 20);
    editor->resize(400, 400);
    editor->setFocus();
    setCentralWidget(editor);
}
