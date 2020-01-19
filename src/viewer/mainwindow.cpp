#include "mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QLabel>

#include "../editor/styler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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

    window->setStyleSheet(Styler::widgetStyle());
    menuBar()->setStyleSheet(Styler::menuStyle());
    _scroll->setStyleSheet(Styler::scrollStyle());
}

void MainWindow::open()
{
    if (_fileInfo.isReadable())
        _watcher.removePath(_fileInfo.filePath());
    _fileInfo.setFile(QFileDialog::getOpenFileName(this, "Open"));
    _watcher.addPath(_fileInfo.filePath());
    loadDocument();
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
    if (!_fileInfo.isReadable())
        return;

    Poppler::Document *document = Poppler::Document::load(_fileInfo.filePath());
    if (document == nullptr)
        return;

    while (_layout->count() > 0)
        delete _layout->takeAt(0)->widget();

    uchar back[4] = { static_cast<uchar>(_background.blue()), static_cast<uchar>(_background.green()), static_cast<uchar>(_background.red()), 0 };
    uchar fore[4] = { static_cast<uchar>(_foreground.blue()), static_cast<uchar>(_foreground.green()), static_cast<uchar>(_foreground.red()), 0 };

    for (int i = 0; i < document->numPages(); ++i) {
        QLabel *widget = new QLabel;

        Poppler::Page *page = document->page(i);
        QImage image = page->renderToImage(_res, _res);
        image = image.scaled(image.width() / 4, image.height() / 4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        int size = image.byteCount();
        uchar *begin = image.bits();
        for (int i = 0; i < size; ++i) {
            uchar cur = begin[i];
            begin[i] = back[i % 4] * cur / 255 + fore[i % 4] * (255 - cur) / 255;
        }

        widget->setPixmap(QPixmap::fromImage(image));
        _layout->addWidget(widget);

        delete page;
    }

    delete document;
}
