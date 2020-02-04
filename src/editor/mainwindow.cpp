#include "mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QLayout>
#include <QStatusBar>
#include <QLabel>

#include "styler.h"
#include "painterdialog.h"
#include "mathwriter.h"

void writeText(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << text << endl;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("LexType");
    resize(640, 480);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    _numbers = new LineNumbers;
    _editor = new Editor(0, _numbers);
    layout->addWidget(_numbers);
    layout->addWidget(_editor);

    _console = new Editor(0, 0, Styler::editorLine());

    QWidget *editorWidget = new QWidget;
    editorWidget->setLayout(layout);

    _splitter = new QSplitter;
    _splitter->setOrientation(Qt::Vertical);
    _splitter->setHandleWidth(0);
    _splitter->addWidget(editorWidget);
    _splitter->addWidget(_console);
    _splitter->setSizes({ 1, 0 });
    setCentralWidget(_splitter);

    _editor->setFocus();

    createActions();
    createMenus();

    menuBar()->setStyleSheet(Styler::menuStyle());
    statusBar()->setStyleSheet(Styler::statusStyle());
}

void MainWindow::open()
{
    QString path = QFileDialog::getOpenFileName(this, "Open", "", "LexType (*.lex);;TeX (*.tex)");
    if (path != "") {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        _editor->setText(in.readAll());
        updateFileName(path);
        statusBar()->showMessage("Opened file: " + path);
    }
}

void MainWindow::save()
{
    saveFile(_fileInfo.filePath());
}

void MainWindow::saveAs()
{
    saveFile(QFileDialog::getSaveFileName(this, "Save", "", "LexType (*.lex);;TeX (*.tex)"));
}

void MainWindow::quit()
{
    close();
}

void MainWindow::compile()
{
    if (!_fileInfo.isReadable()) {
        statusBar()->showMessage("To perform compilation, save file");
        return;
    }

    save();

    QFileInfo fileInfo(_fileInfo.dir(), _fileInfo.baseName() + ".tex");
    writeText(fileInfo.filePath(), MathWriter::pass(_editor->text()));

    if (_compilation != nullptr)
        delete _compilation;
    _compilation = new QProcess;
    _compilation->setWorkingDirectory(fileInfo.dir().absolutePath());
    // TODO do we need it?
//    process->deleteLater();

    connect(_compilation, &QProcess::readyRead, this, &MainWindow::output);
    connect(_compilation, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::compiled);
    _splitter->setSizes({ 7, 3 });
    _console->setText("");
    _compilation->start("pdflatex", { fileInfo.filePath() });
}

void MainWindow::painter()
{
    PainterDialog *dialog = new PainterDialog();
    dialog->exec();
    QString latex = dialog->latex();
    if (latex == "")
        statusBar()->showMessage("Figure insertion canceled");
    else
        statusBar()->showMessage("Figure inserted");
    _editor->insert(dialog->latex());
}

void MainWindow::output()
{
    _console->insert(QString(_compilation->readAll()));
    statusBar()->showMessage("Compiling...");
}

void MainWindow::compiled(int exitCode, QProcess::ExitStatus)
{
    _console->insert("Process finished with exit code " + QString::number(exitCode));

    if (exitCode == 0) {
        _splitter->setSizes({ 1, 0 });
        statusBar()->showMessage("Compilation successful");
    } else {
        statusBar()->showMessage("Compilation failed");
    }
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
    if (path == "") {
        statusBar()->showMessage("File not saved (empty path)");
        return;
    }

    writeText(path, _editor->text());
    updateFileName(path);
    statusBar()->showMessage("Saved file " + path);
}

void MainWindow::updateFileName(const QString &path)
{
    _fileInfo.setFile(path);
    setWindowTitle(_fileInfo.fileName() + " - LexType");
}
