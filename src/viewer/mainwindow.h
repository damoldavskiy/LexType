#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include "scrollarea.h"
#include "../editor/pathwatcher.h"

namespace Viewer {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, const QString &path = "");

public slots:
    void open();
    void quit();
    void zoomIn();
    void zoomOut();

private slots:
    void fileChanged();

private:
    void tryOpenFile(const QString &path);
    void createActions();
    void createMenus();
    void loadDocument();

    ScrollArea *_scroll;

    QAction *_openAction;
    QAction *_quitAction;
    QAction *_zoomInAction;
    QAction *_zoomOutAction;

    PathWatcher _path = PathWatcher("LexType Viewer");

    QFileSystemWatcher _watcher;
};

}

#endif // MAINWINDOW_H
