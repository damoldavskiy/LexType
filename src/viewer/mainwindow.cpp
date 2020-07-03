#include "mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>

#include "../editor/styler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Styler::init(); // TODO Should we do it in main.cpp?
    setWindowTitle("LexType Viewer");
    resize(640, 480);

    _scroll = new ScrollArea;
    setCentralWidget(_scroll);

    connect(&_watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::fileChanged);

    createActions();
    createMenus();

    menuBar()->setStyleSheet(Styler::get<QString>("menu-style"));
}

void MainWindow::open()
{
    if (_path.exists())
        _watcher.removePath(_path.path());

    if (_path.open("PDF (*.pdf)")) {
        _watcher.addPath(_path.path());
        loadDocument();
        setWindowTitle(_path.title());
    }
}

void MainWindow::quit()
{
    close();
}

void MainWindow::zoomIn()
{
    _scroll->setDpi(_scroll->dpi() * 1.2);
}

void MainWindow::zoomOut()
{
    _scroll->setDpi(_scroll->dpi() / 1.2);
}

void MainWindow::fileChanged()
{
    loadDocument();
}

void MainWindow::createActions()
{
    _openAction = new QAction("Open", this);
    _openAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(_openAction, &QAction::triggered, this, &MainWindow::open);

    _quitAction = new QAction("Quit", this);
    connect(_quitAction, &QAction::triggered, this, &MainWindow::quit);

    _zoomInAction = new QAction("Zoom in", this);
    _zoomInAction->setShortcut(QKeySequence("Ctrl+="));
    connect(_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);

    _zoomOutAction = new QAction("Zoom out", this);
    _zoomOutAction->setShortcut(QKeySequence("Ctrl+-"));
    connect(_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
}

void MainWindow::createMenus()
{
    QMenu *menu;

    menu = menuBar()->addMenu("File");
    menu->addActions({ _openAction, _quitAction });

    menu = menuBar()->addMenu("View");
    menu->addActions({ _zoomInAction, _zoomOutAction });
}

void MainWindow::loadDocument()
{
    if (!_path.exists())
        return;

    _scroll->load(Poppler::Document::load(_path.path()));
}
