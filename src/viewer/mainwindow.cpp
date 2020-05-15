#include "mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QScrollBar>
#include <QLabel>
#include <QDebug>

#include "../editor/styler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Styler::init(); // TODO Should we do it in main.cpp?
    setWindowTitle("LexType Viewer");
    resize(640, 480);

    _scroll = new QScrollArea;
    _scroll->setWidgetResizable(true);
    setCentralWidget(_scroll);

    QWidget *window = new QWidget;
    _scroll->setWidget(window);

    _layout = new QVBoxLayout;
    _layout->setAlignment(Qt::AlignHCenter);
    _layout->setMargin(0);
    _layout->setSpacing(0);
    window->setLayout(_layout);

    connect(&_watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::fileChanged);

    createActions();
    createMenus();

    window->setStyleSheet(Styler::get<QString>("widget-style"));
    menuBar()->setStyleSheet(Styler::get<QString>("menu-style"));
    _scroll->setStyleSheet(Styler::get<QString>("scroll-style"));
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
    _res *= 1.5;
    loadDocument();
}

void MainWindow::zoomOut()
{
    _res /= 1.5;
    loadDocument();
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

    Poppler::Document *document = Poppler::Document::load(_path.path());
    if (document == nullptr)
        return;

    while (_layout->count() > 0)
        delete _layout->takeAt(0)->widget();

    QColor back = Styler::get<QColor>("viewer-back");
    QColor fore = Styler::get<QColor>("viewer-fore");
    uchar backArray[4] = { static_cast<uchar>(back.blue()), static_cast<uchar>(back.green()), static_cast<uchar>(back.red()), 0 };
    uchar foreArray[4] = { static_cast<uchar>(fore.blue()), static_cast<uchar>(fore.green()), static_cast<uchar>(fore.red()), 0 };

    for (int i = 0; i < document->numPages(); ++i) {
        QLabel *widget = new QLabel;

        Poppler::Page *page = document->page(i);
        QImage image = page->renderToImage(_res, _res);
        image = image.scaled(image.width() / 4, image.height() / 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        int size = image.byteCount();
        uchar *begin = image.bits();
        // TODO Parallel
        for (int i = 0; i < size; ++i) {
            uchar cur = begin[i];
            begin[i] = backArray[i % 4] * cur / 255 + foreArray[i % 4] * (255 - cur) / 255;
        }

        widget->setPixmap(QPixmap::fromImage(image));
        _layout->addWidget(widget);

        delete page;
    }

    delete document;
}
