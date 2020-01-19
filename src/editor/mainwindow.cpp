#include "mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QProcess>
#include <QEventLoop>
#include <QLayout>

#include "styler.h"
#include "painterdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("LexType");
    resize(640, 480);

    QWidget *window = new QWidget;
    setCentralWidget(window);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    window->setLayout(layout);

    _numbers = new LineNumbers;
    _editor = new Editor(0, _numbers);
    layout->addWidget(_numbers);
    layout->addWidget(_editor);

    _editor->setFocus();

    createActions();
    createMenus();

    menuBar()->setStyleSheet(Styler::menuStyle());
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this, "Open");
    if (path != "") {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        _editor->setText(in.readAll());
        updateFileName(path);
    }
}

void MainWindow::save()
{
    if (_fileInfo.isWritable())
        saveFile(_fileInfo.filePath());
}

void MainWindow::saveAs()
{
    QString path = QFileDialog::getSaveFileName(this, "Save");

    if (path != "")
        saveFile(path);
}

void MainWindow::quit()
{
    close();
}

void MainWindow::compile()
{
    if (!_fileInfo.isReadable())
        return;

    save();
    QProcess process;
    process.setWorkingDirectory(_fileInfo.dir().absolutePath());
    process.start("pdflatex", { _fileInfo.filePath() });
    process.waitForFinished();
}

void MainWindow::painter()
{
    PainterDialog *dialog = new PainterDialog();
    dialog->exec();
    _editor->insert(dialog->latex());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int res = QMessageBox::question(this, "Quit", "Do you really want to exit the program?");
    if (res != QMessageBox::Yes)
        event->ignore();
}

void MainWindow::createActions()
{
    _openAction = new QAction("Open", this);
    _openAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(_openAction, &QAction::triggered, this, &MainWindow::open);

    _saveAction = new QAction("Save", this);
    _saveAction->setShortcut(QKeySequence("Ctrl+S"));
    connect(_saveAction, &QAction::triggered, this, &MainWindow::save);

    _saveAsAction = new QAction("Save as...", this);
    _saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(_saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    _quitAction = new QAction("Quit", this);
    connect(_quitAction, &QAction::triggered, this, &MainWindow::quit);

    _undoAction = new QAction("Undo", this);
    _undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    connect(_undoAction, &QAction::triggered, _editor, &Editor::undo);

    _redoAction = new QAction("Redo", this);
    _redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
    connect(_redoAction, &QAction::triggered, _editor, &Editor::redo);

    _cutAction = new QAction("Cut", this);
    _cutAction->setShortcut(QKeySequence("Ctrl+X"));
    connect(_cutAction, &QAction::triggered, _editor, &Editor::cut);

    _copyAction = new QAction("Copy", this);
    _copyAction->setShortcut(QKeySequence("Ctrl+C"));
    connect(_copyAction, &QAction::triggered, _editor, &Editor::copy);

    _pasteAction = new QAction("Paste", this);
    _pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    connect(_pasteAction, &QAction::triggered, _editor, &Editor::paste);

    _selectAllAction = new QAction("Select all", this);
    _selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    connect(_selectAllAction, &QAction::triggered, _editor, &Editor::selectAll);

    _compileAction = new QAction("Compile", this);
    _compileAction->setShortcut(QKeySequence("F5"));
    connect(_compileAction, &QAction::triggered, this, &MainWindow::compile);

    _painterAction = new QAction("Painter", this);
    _painterAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(_painterAction, &QAction::triggered, this, &MainWindow::painter);
}

void MainWindow::createMenus()
{
    QMenu *menu;

    menu = menuBar()->addMenu("File");
    menu->addActions({ _openAction, _saveAction, _saveAsAction, _quitAction });

    menu = menuBar()->addMenu("Edit");
    menu->addActions({ _undoAction, _redoAction });
    menu->addSeparator();
    menu->addActions({ _cutAction, _copyAction, _pasteAction, _selectAllAction });

    menu = menuBar()->addMenu("Tools");
    menu->addActions({ _compileAction, _painterAction });
}

void MainWindow::saveFile(const QString &path)
{
    Q_ASSERT(path.size() > 0);

    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << _editor->text() << endl;
    updateFileName(path);
}

void MainWindow::updateFileName(const QString &path)
{
    _fileInfo.setFile(path);
    setWindowTitle(_fileInfo.fileName() + " - LexType");
}
