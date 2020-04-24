#include "mainwindow.h"

#include <QFileDialog> // TODO For FileInfo::ctor(QString, QString)
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QLayout>
#include <QStatusBar>

#include "styler.h"
#include "painterdialog.h"
#include "settingsdialog.h"
#include "mathwriter.h"
#include "snippet.h"

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
    Styler::reset(); // TODO Should we do it in main.cpp?
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

    createSnippets();
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
    if (_path.save("LexType (*.lex);;TeX (*tex)")) {
        writeText(_path.path(), _editor->text());
        setWindowTitle(_path.title());
        statusBar()->showMessage("Saved file: " + _path.path());
    }
}

void MainWindow::quit()
{
    close();
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
    statusBar()->showMessage("Settings applied");
    delete dialog;
}

void MainWindow::typed(int pos, QChar)
{
    // TODO Insert from clipborad doesn't emit Editor::typed
    _path.setEdited(true);
    setWindowTitle(_path.title());

    if (Styler::get<bool>("editor-flag-snippets")) {
        QVector<Snippet> *snippets;
        if (_editor->markup(pos) == Interval::Mathematics)
            snippets = &_snippetsMath;
        else
            snippets = &_snippetsRegular;

        for (const auto &snippet : *snippets)
            if (snippet.apply(_editor))
                break;
    }

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

void MainWindow::createSnippets()
{
    _snippetsMath.append(Snippet("<<", "⟨"));
    _snippetsMath.append(Snippet(">>", "⟩"));

    _snippetsMath.append(Snippet("NN", "ℕ"));
    _snippetsMath.append(Snippet("ZZ", "ℤ"));
    _snippetsMath.append(Snippet("QQ", "ℚ"));
    _snippetsMath.append(Snippet("RR", "ℝ"));
    _snippetsMath.append(Snippet("CC", "ℂ"));
    _snippetsMath.append(Snippet("PP", "ℙ"));

    _snippetsMath.append(Snippet("..-", "⋯"));
    _snippetsMath.append(Snippet("..|", "⋮"));
    _snippetsMath.append(Snippet("..\\", "⋱"));

    _snippetsMath.append(Snippet("for", "∀"));
    _snippetsMath.append(Snippet("exi", "∃"));
    _snippetsMath.append(Snippet("nn", "∩"));
    _snippetsMath.append(Snippet("uu", "∪"));
    _snippetsMath.append(Snippet("⊂=", "⊆"));
    _snippetsMath.append(Snippet("cc", "⊂"));
    _snippetsMath.append(Snippet("⊃=", "⊇"));
    _snippetsMath.append(Snippet("pp", "⊃"));

    _snippetsMath.append(Snippet("ll", "≪"));
    _snippetsMath.append(Snippet("gg", "≫"));
    _snippetsMath.append(Snippet("le", "≤"));
    _snippetsMath.append(Snippet("ge", "≥"));

    _snippetsMath.append(Snippet("⇐>", "⇔"));
    _snippetsMath.append(Snippet("=>", "⇒"));
    _snippetsMath.append(Snippet("<=", "⇐"));
    _snippetsMath.append(Snippet("←>", "↔"));
    _snippetsMath.append(Snippet("->", "→"));
    _snippetsMath.append(Snippet("<-", "←"));

    _snippetsMath.append(Snippet("xx", "×"));
    _snippetsMath.append(Snippet("∘@", "@"));
    _snippetsMath.append(Snippet("@", "∘"));
    _snippetsMath.append(Snippet("+-", "±"));
    _snippetsMath.append(Snippet("⋅*", "*"));
    _snippetsMath.append(Snippet("**", "⋅⋅⋅"));
    _snippetsMath.append(Snippet("*", "⋅"));
    _snippetsMath.append(Snippet("o.", "⊙"));
    _snippetsMath.append(Snippet("o+", "⊕"));
    _snippetsMath.append(Snippet("ox", "⊗"));

    _snippetsMath.append(Snippet("par", "∂"));
    _snippetsMath.append(Snippet("oo", "∞"));

    _snippetsMath.append(Snippet("sin", "sin")); // sin is not s\in
    _snippetsMath.append(Snippet("!ni", "∌"));
    _snippetsMath.append(Snippet("!in", "∉"));
    _snippetsMath.append(Snippet("in", "∈"));
    _snippetsMath.append(Snippet("ni", "∋"));

    _snippetsMath.append(Snippet("==", "≡"));
    _snippetsMath.append(Snippet("!=", "≠"));
    _snippetsMath.append(Snippet("~~", "≈"));
    _snippetsMath.append(Snippet("~=", "≅"));

    _snippetsMath.append(Snippet("sum", "∑"));
    _snippetsMath.append(Snippet("prod", "∏"));
    _snippetsMath.append(Snippet("∈t", "∫"));
    _snippetsMath.append(Snippet("∫i", "∫_{-∞}^{+∞}"));

    _snippetsMath.append(Snippet("!O", "∅"));

    _snippetsMath.append(Snippet("alp", "α"));
    _snippetsMath.append(Snippet("bet", "β"));
    _snippetsMath.append(Snippet("Gam", "Γ"));
    _snippetsMath.append(Snippet("gam", "γ"));
    _snippetsMath.append(Snippet("Del", "Δ"));
    _snippetsMath.append(Snippet("del", "δ"));
    _snippetsMath.append(Snippet("eps", "ɛ")); // varepsilon
    _snippetsMath.append(Snippet("zet", "ζ"));
    _snippetsMath.append(Snippet("eta", "η"));
    _snippetsMath.append(Snippet("The", "Θ"));
    _snippetsMath.append(Snippet("the", "ϑ")); // vartheta
    _snippetsMath.append(Snippet("iot", "ι"));
    _snippetsMath.append(Snippet("kap", "κ"));
    _snippetsMath.append(Snippet("Lam", "Λ"));
    _snippetsMath.append(Snippet("lam", "λ"));
    _snippetsMath.append(Snippet("mu", "μ"));
    _snippetsMath.append(Snippet("nu", "ν"));
    _snippetsMath.append(Snippet("Xi", "Ξ"));
    _snippetsMath.append(Snippet("xi", "ξ"));
    _snippetsMath.append(Snippet("Pi", "Π"));
    _snippetsMath.append(Snippet("pi", "π"));
    _snippetsMath.append(Snippet("rho", "ρ"));
    _snippetsMath.append(Snippet("Sig", "Σ"));
    _snippetsMath.append(Snippet("sig", "σ"));
    _snippetsMath.append(Snippet("tau", "τ"));
    _snippetsMath.append(Snippet("ups", "υ"));
    _snippetsMath.append(Snippet("Phi", "Φ"));
    _snippetsMath.append(Snippet("phi", "φ")); // varphi
    _snippetsMath.append(Snippet("chi", "χ"));
    _snippetsMath.append(Snippet("Psi", "Ψ"));
    _snippetsMath.append(Snippet("psi", "ψ"));
    _snippetsMath.append(Snippet("Ome", "Ω"));
    _snippetsMath.append(Snippet("ome", "ω"));

    _snippetsRegular.append(Snippet("sssec", "\\subsubsection{}", 15, true));
    _snippetsRegular.append(Snippet("ssec", "\\subsection{}", 12, true));
    _snippetsRegular.append(Snippet("sec", "\\section{}", 9, true));

    _snippetsRegular.append(Snippet("section*{", "section{", true));
    _snippetsRegular.append(Snippet("section{", "section*{", true));

    _snippetsRegular.append(Snippet("thm", "\\begin{theorem}\n\t\n\\end{theorem}\n", 17, true));
    _snippetsRegular.append(Snippet("def", "\\begin{definition}\n\t\n\\end{definition}\n", 20, true));
    _snippetsRegular.append(Snippet("prf", "\\begin{proof}\n\t\n\\end{proof}\n", 15, true));
    _snippetsRegular.append(Snippet("cor", "\\begin{corollary}\n\t\n\\end{corollary}\n", 19, true));
    _snippetsRegular.append(Snippet("rmk", "\\begin{remark}\n\t\n\\end{remark}\n", 16, true));
    _snippetsRegular.append(Snippet("lem", "\\begin{lemma}\n\t\n\\end{lemma}\n", 15, true));
    _snippetsRegular.append(Snippet("exm", "\\begin{example}\n\t\n\\end{example}\n", 17, true));

    _snippetsRegular.append(Snippet("\\begin{theorem}\n\t","\\begin{theorem}[]\n\t", 16, true));
    _snippetsRegular.append(Snippet("\\begin{definition}\n\t", "\\begin{definition}[]\n\t", 19, true));
    _snippetsRegular.append(Snippet("\\begin{proof}\n\t", "\\begin{proof}[]\n\t", 14, true));
    _snippetsRegular.append(Snippet("\\begin{corollary}\n\t", "\\begin{corollary}[]\n\t", 18, true));
    _snippetsRegular.append(Snippet("\\begin{remark}\n\t", "\\begin{remark}[]\n\t", 15, true));
    _snippetsRegular.append(Snippet("\\begin{lemma}\n\t", "\\begin{lemma}[]\n\t", 14, true));
    _snippetsRegular.append(Snippet("\\begin{example}\n\t", "\\begin{example}[]\n\t", 16, true));

    _snippetsRegular.append(Snippet("doc", "\\documentclass{article}\n\\usepackage{lexpack}\n\n\\begin{document}\n\n\n\n\\end{document}\n", 64, true));
    _snippetsRegular.append(Snippet("item", "\\begin{itemize}\n\t\\item \n\\end{itemize}\n", 23, true));
    _snippetsRegular.append(Snippet("enum", "\\begin{enumerate}\n\t\\item \n\\end{enumerate}\n", 25, true));
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
    menu->addActions({ _cutAction, _copyAction, _pasteAction, _selectAllAction });

    menu = menuBar()->addMenu("Tools");
    menu->addActions({ _compileAction, _painterAction });
    menu->addSeparator();
    menu->addActions({ _optionsAction });
}
