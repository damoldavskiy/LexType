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
    out.setCodec("UTF-8");
    out << text << '\n';
}

QString readText(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    return in.readAll();
}

MainWindow::MainWindow(QWidget *parent, const QString &path)
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

    _console = new Editor(0, 0, true);

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

    _timer.setSingleShot(true);

    menuBar()->setStyleSheet(Styler::get<QString>("menu-style"));
    statusBar()->setStyleSheet(Styler::get<QString>("status-style"));

    connect(_editor, &Editor::changed, this, &MainWindow::textChanged);
    connect(_editor, &Editor::typed, this, &MainWindow::textTyped);

    connect(&_compilation, &QProcess::readyRead, this, &MainWindow::output);
    connect(&_compilation, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::compiled);

    connect(&_timer, &QTimer::timeout, this, &MainWindow::compile);

    if (path != "")
        tryOpenFile(path);
}

void MainWindow::openFile()
{
    if (_path.edited() && QMessageBox::question(this, "Open", "Changes unsaved. Do you really want to open another file?") != QMessageBox::Yes)
        return;

    if (_path.open("LexType (*.lex);;TeX (*tex);;All types (*.*)"))
        tryOpenFile(_path.path());
}

void MainWindow::closeFile()
{
    if (_path.edited() && QMessageBox::question(this, "Open", "Changes unsaved. Do you really want to close the document?") != QMessageBox::Yes)
        return;

    _path.close();
    _editor->setText("");
    setWindowTitle(_path.title());
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
    FindDialog *dialog = new FindDialog(_editor->selectedText());
    connect(dialog, &FindDialog::find, _editor, &Editor::find);
    connect(dialog, &FindDialog::replace, _editor, &Editor::replace);
    dialog->exec();
    delete dialog;
}

void MainWindow::toggleConsole()
{
    if (_splitter->sizes().at(1) == 0)
        _splitter->setSizes({ 7, 3 });
    else
        _splitter->setSizes({ 1, 0 });
}

void MainWindow::compile()
{
    if (!_path.exists()) {
        statusBar()->showMessage("To perform compilation, save file");
        return;
    }

    QFileInfo fileInfo(_path.dir(), _path.baseName() + ".tex");

    if (_path.edited() || !fileInfo.exists())
        save(); // .lex
    writeText(fileInfo.filePath(), MathWriter::pass(_editor->text())); // .tex

    if (_compilation.state() != QProcess::NotRunning) {
        _compilation.kill();
        _compilation.waitForFinished(-1);
    }

    _compilation.setWorkingDirectory(fileInfo.dir().absolutePath());

    if (!Styler::get<bool>("editor-flag-autocompile"))
        _splitter->setSizes({ 7, 3 });

    _console->setText("");
    _compilation.start("pdflatex", { fileInfo.filePath() });
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

void MainWindow::wordWrap(bool checked)
{
    Styler::set<bool>("editor-flag-wordwrap", checked);
    _editor->updateSettings();
}

void MainWindow::options()
{
    SettingsDialog *dialog = new SettingsDialog;
    dialog->exec();
    _editor->updateSettings();
    _console->updateSettings();
    _snippets = Styler::get<QVariant>("snippets").value<SnippetManager>();
    _wordWrap->setChecked(Styler::get<bool>("editor-flag-wordwrap"));
    statusBar()->showMessage("Settings applied");
    delete dialog;
}

void MainWindow::about()
{
    QMessageBox message(this);
    message.setTextFormat(Qt::RichText);
    message.setWindowTitle("About LexType");
    message.setText("LexType is a software environment targeted on effecive writing of any kind of lecture or technical notes.<br><br>Author: DA Moldavsky<br><br>News & updates: <a href=\"tg://resolve?domain=lextype\">@lextype</a>");
    message.exec();
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::textChanged()
{
    _path.setEdited(_editor->textSize() > 0);
    setWindowTitle(_path.title());

    if (Styler::get<bool>("editor-flag-autocompile") && _timer.remainingTime() == -1)
        _timer.start(Styler::get<int>("editor-autocompile-interval"));
}

void MainWindow::textTyped(int, QChar)
{
    _snippets.apply(_editor, Styler::get<bool>("editor-flag-snippets-regular"), Styler::get<bool>("editor-flag-snippets-math"));
}

void MainWindow::output()
{
    _console->insert(_console->caret(), QString(_compilation.readAll()));
    statusBar()->showMessage("Compiling...");
}

void MainWindow::compiled(int exitCode, QProcess::ExitStatus)
{
    _console->insert(_console->caret(), "Process finished with exit code " + QString::number(exitCode));

    if (exitCode == 0) {
        if (!Styler::get<bool>("editor-flag-autocompile"))
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

void MainWindow::tryOpenFile(const QString &path)
{
    if (_path.openExisting(path)) {
        _editor->setText(readText(_path.path()));
        setWindowTitle(_path.title());
        statusBar()->showMessage("Opened file: " + _path.path());
    } else {
        statusBar()->showMessage("File not exists: " + path);
    }
}

void MainWindow::createActions()
{
    _openAction = new QAction("Open", this);
    _openAction->setShortcut(QKeySequence("Ctrl+O"));
    connect(_openAction, &QAction::triggered, this, &MainWindow::openFile);

    _closeAction = new QAction("Close", this);
    _closeAction->setShortcut(QKeySequence("Ctrl+W"));
    connect(_closeAction, &QAction::triggered, this, &MainWindow::closeFile);

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

    _toggleConsoleAction = new QAction("Toggle console", this);
    _toggleConsoleAction->setShortcut(QKeySequence("F4"));
    connect(_toggleConsoleAction, &QAction::triggered, this, &MainWindow::toggleConsole);

    _compileAction = new QAction("Compile", this);
    _compileAction->setShortcut(QKeySequence("F5"));
    connect(_compileAction, &QAction::triggered, this, &MainWindow::compile);

    _painterAction = new QAction("Painter", this);
    _painterAction->setShortcut(QKeySequence("Ctrl+D"));
    connect(_painterAction, &QAction::triggered, this, &MainWindow::painter);

    _wordWrap = new QAction("Word wrap", this);
    _wordWrap->setCheckable(true);
    _wordWrap->setChecked(Styler::get<bool>("editor-flag-wordwrap"));
    connect(_wordWrap, &QAction::toggled, this, &MainWindow::wordWrap);

    _optionsAction = new QAction("Options", this);
    connect(_optionsAction, &QAction::triggered, this, &MainWindow::options);

    _aboutAction = new QAction("About", this);
    connect(_aboutAction, &QAction::triggered, this, &MainWindow::about);

    _aboutQtAction = new QAction("About Qt", this);
    connect(_aboutQtAction, &QAction::triggered, this, &MainWindow::aboutQt);
}

void MainWindow::createMenus()
{
    QMenu *menu;

    menu = menuBar()->addMenu("File");
    menu->addActions({ _openAction, _closeAction });
    menu->addSeparator();
    menu->addActions({ _saveAction, _saveAsAction });
    menu->addSeparator();
    menu->addActions({ _quitAction });

    menu = menuBar()->addMenu("Edit");
    menu->addActions({ _undoAction, _redoAction });
    menu->addSeparator();
    menu->addActions({ _findAction });
    menu->addSeparator();
    menu->addActions({ _cutAction, _copyAction, _pasteAction, _selectAllAction });

    menu = menuBar()->addMenu("Tools");
    menu->addActions({ _toggleConsoleAction, _compileAction, _painterAction });
    menu->addSeparator();
    menu->addActions({ _wordWrap, _optionsAction });

    menu = menuBar()->addMenu("Help");
    menu->addActions({ _aboutAction, _aboutQtAction });
}
