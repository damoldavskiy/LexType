#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>

#include "editor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void open();
    void save();
    void quit();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createMenus();

    Editor *editor;
    QMenu *fileMenu;
    QMenu *editMenu;
};

#endif // MAINWINDOW_H
