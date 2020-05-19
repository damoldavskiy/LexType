#include "mainwindow.h"

#include <QFileDialog> // TODO For FileInfo::ctor(QString, QString)
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QLayout>
#include <QStatusBar>

#include "styler.h"
#include "finddialog.h"
#include "painterdialog.h"
#include "settingsdialog.h"
#include "mathwriter.h"

void writeText(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << text << '\n';
}

QString readText(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    return in.readAll();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // TODO Terrible approach
    if (Styler::init()) {
        _snippets.reset();
        Styler::set("snippets", QVariant::fromValue(_snippets));
    } else
        _snippets = Styler::get<QVariant>("snippets").value<SnippetManager>();

    setWindowTitle("LexType");
    resize(640, 480);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    _numbers = new LineNumbers;
    _editor = new Editor(0, _numbers);
    layout->addWidget(_numbers);
    layout->addWidget(_editor);

    _console = new Editor(0, 0);

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

    menuBar()->setStyleSheet(Styler::get<QString>("menu-style"));
    statusBar()->setStyleSheet(Styler::get<QString>("status-style"));

    connect(_editor, SIGNAL(typed(int, QChar)), this, SLOT(typed(int, QChar)));
}

void MainWindow::open()
{
//    if (_path.edited())
        // Do you want to save changes?

    if (_path.open("LexType (*.lex);;TeX (*tex)")) {
        _editor->setText(readText(_path.path()));
        setWindowTitle(_path.title());
        statusBar()->showMessage("Opened file: " + _path.path());
    }
}

void MainWindow::save()
{
    if (_path.exists()) {
        _path.setEdited(false);
        writeText(_path.path(), _editor->text());
        setWindowTitle(_path.title());
        statusBar()->showMessage("Saved file: " + _path.path());
    } else {
        statusBar()->showMessage("File not opened");
    }
}

void MainWindow::saveAs()
{
    QString selected;
    if (_path.save("LexType (*.lex);;TeX (*tex)"), &selected) {
        writeText(_path.path(), _path.path().endsWith(".tex") ? MathWriter::pass(_editor->text()) : _editor->text());
        setWindowTitle(_path.title());
        statusBar()->showMessage("Saved file: " + _path.path());
    }
}

void MainWindow::quit()
{
    close();
}

void MainWindow::find()
{
    FindDialog *dialog = new FindDialog;
    connect(dialog, &FindDialog::find, _editor, &Editor::find);
    connect(dialog, &FindDialog::replace, _editor, &Editor::replace);
    dialog->exec();
    delete dialog; // TODO Is it ok with connects?
}

void MainWindow::compile()
{
    if (!_path.exists()) {
        statusBar()->showMessage("To perform compilation, save file");
        return;
    }

    save();

    QFileInfo fileInfo(_path.dir(), _path.baseName() + ".tex");
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
    PainterDialog *dialog = new PainterDialog;
    dialog->exec();
    QString latex = dialog->latex();
    if (latex == "")
        statusBar()->showMessage("Figure insertion canceled");
    else {
        statusBar()->showMessage("Figure inserted");
        _path.setEdited(true);
    }
    _editor->insert(_editor->caret(), latex);
    delete dialog;
}

void MainWindow::options()
{
    SettingsDialog *dialog = new SettingsDialog;
    dialog->exec();
    _editor->updateSettings();
    _snippets = Styler::get<QVariant>("snippets").value<SnippetManager>();
    statusBar()->showMessage("Settings applied");
    delete dialog;
}

void MainWindow::typed(int, QChar)
{
    // TODO Insert from clipborad doesn't emit Editor::typed
    _path.setEdited(true);
    setWindowTitle(_path.title());

    _snippets.apply(_editor, Styler::get<bool>("editor-flag-snippets-regular"), Styler::get<bool>("editor-flag-snippets-math"));

    if (Styler::get<bool>("editor-flag-autocompile"))
        compile();
}

void MainWindow::output()
{
    _console->insert(_console->caret(), QString(_compilation->readAll()));
    statusBar()->showMessage("Compiling...");
}

void MainWindow::compiled(int exitCode, QProcess::ExitStatus)
{
    _console->insert(_console->caret(), "Process finished with exit code " + QString::number(exitCode));

    if (exitCode == 0) {
        _splitter->setSizes({ 1, 0 });
        statusBar()->showMessage("Compilation successful");
    } else {
        statusBar()->showMessage("Compilation failed");
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (Styler::get<bool>("window-flag-askexit") && _path.edited() && QMessageBox::question(this, "Quit", "Changes unsaved. Do you really want to exit the program?") != QMessageBox::Yes)
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

    _findAction = new QAction("Find", this);
    _findAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(_findAction, &QAction::triggered, this, &MainWindow::find);

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

    _optionsAction = new QAction("Options", this);
    connect(_optionsAction, &QAction::triggered, this, &MainWindow::options);
}

void MainWindow::createMenus()
{
    QMenu *menu;

    menu = menuBar()->addMenu("File");
    menu->addActions({ _openAction, _saveAction, _saveAsAction });
    menu->addSeparator();
    menu->addActions({ _quitAction });

    menu = menuBar()->addMenu("Edit");
    menu->addActions({ _undoAction, _redoAction });
    menu->addSeparator();
    menu->addActions({ _findAction });
    menu->addSeparator();
    menu->addActions({ _cutAction, _copyAction, _pasteAction, _selectAllAction });

    menu = menuBar()->addMenu("Tools");
    menu->addActions({ _compileAction, _painterAction });
    menu->addSeparator();
    menu->addActions({ _optionsAction });
}
