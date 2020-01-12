#include "mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    setCentralWidget(scroll);

    parent = new QWidget;
    scroll->setWidget(parent);

    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setMargin(0);
    layout->setSpacing(0);
    parent->setLayout(layout);

    createActions();
    createMenus();

    parent->setStyleSheet("QWidget { background: rgb(50, 50, 50); }");

    menuBar()->setStyleSheet("QWidget { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }"
                             "QMenu { background: rgb(50, 50, 50); }"
                             "QMenu::item { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }");

    scroll->setStyleSheet("QScrollArea { border: none; }"
                          "QAbstractScrollArea::corner { background: rgb(50, 50, 50); }"
                          "QScrollBar::add-line, QScrollBar::sub-line { background: rgb(50, 50, 50); width: 0px; height: 0px; }"
                          "QScrollBar::handle { background: rgb(60, 60, 60); min-height: 30px; min-width: 30px; }"
                          "QScrollBar { background: rgb(50, 50, 50); margin: 0; }");
}

void MainWindow::open()
{
    path = QFileDialog::getOpenFileName(this, "Open");
    loadDocument();
}

void MainWindow::quit()
{
    close();
}

void MainWindow::zoomIn()
{
    res *= 1.5;
    loadDocument();
}

void MainWindow::zoomOut()
{
    res /= 1.5;
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
    if (path != "")
    {
        while (layout->count() > 0)
            delete layout->takeAt(0)->widget();

        Poppler::Document *document = Poppler::Document::load(path);

        uchar back[4] = { static_cast<uchar>(_background.blue()), static_cast<uchar>(_background.green()), static_cast<uchar>(_background.red()), 0 };
        uchar fore[4] = { static_cast<uchar>(_foreground.blue()), static_cast<uchar>(_foreground.green()), static_cast<uchar>(_foreground.red()), 0 };

        for (int i = 0; i < document->numPages(); ++i) {
            QLabel *widget = new QLabel;

            Poppler::Page *page = document->page(i);
            QImage image = page->renderToImage(res, res);

            int size = image.byteCount();
            uchar *begin = image.bits();
            for (int i = 0; i < size; ++i) {
                uchar cur = begin[i];
                begin[i] = back[i % 4] * cur / 255 + fore[i % 4] * (255 - cur) / 255;
            }

            widget->setPixmap(QPixmap::fromImage(image));
            layout->addWidget(widget);

            delete page;
        }

        delete document;
    }

}
