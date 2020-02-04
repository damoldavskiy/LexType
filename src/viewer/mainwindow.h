#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <poppler-qt5.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *_parent = 0);

public slots:
    void open();
    void quit();
    void zoomIn();
    void zoomOut();

private slots:
    void fileChanged();

private:
    void createActions();
    void createMenus();
    void loadDocument();

    QScrollArea *_scroll;
    QWidget *_parent;
    QVBoxLayout *_layout;

    QAction *_openAction;
    QAction *_quitAction;
    QAction *_zoomInAction;
    QAction *_zoomOutAction;

    QFileInfo _fileInfo;
    qreal _res = 300;

    QFileSystemWatcher _watcher;
};

#endif // MAINWINDOW_H
