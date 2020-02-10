#include "mainwindow.h"

#include <QFileDialog> // TODO For FileInfo::ctor(QString, QString)
#include <QTextStream>
#include <QMessageBox>
#include <QEventLoop>
#include <QLayout>
#include <QStatusBar>

#include "styler.h"
#include "painterdialog.h"
#include "mathwriter.h"
#include "snippet.h"

void writeText(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out << text << endl;
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

    menuBar()->setStyleSheet(Styler::menuStyle());
    statusBar()->setStyleSheet(Styler::statusStyle());

    connect(_editor, SIGNAL(typed(int, QChar)), this, SLOT(typed(int, QChar)));
}

void MainWindow::open()
{
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
    PainterDialog *dialog = new PainterDialog();
    dialog->exec();
    QString latex = dialog->latex();
    if (latex == "")
        statusBar()->showMessage("Figure insertion canceled");
    else
        statusBar()->showMessage("Figure inserted");
    _editor->insert(_editor->caret(), dialog->latex());
}

void MainWindow::typed(int pos, QChar)
{
    // TODO Insert from clipborad doesn't emit Editor::typed
    _path.setEdited(true);
    setWindowTitle(_path.title());

    QVector<Snippet> snippets;

    if (_editor->markup(pos) == Interval::Mathematics) {
        snippets.append(Snippet("<<", "⟨"));
        snippets.append(Snippet(">>", "⟩"));

        snippets.append(Snippet("NN", "ℕ"));
        snippets.append(Snippet("ZZ", "ℤ"));
        snippets.append(Snippet("QQ", "ℚ"));
        snippets.append(Snippet("RR", "ℝ"));
        snippets.append(Snippet("CC", "ℂ"));
        snippets.append(Snippet("PP", "ℙ"));

        snippets.append(Snippet("..-", "⋯"));
        snippets.append(Snippet("..|", "⋮"));
        snippets.append(Snippet("..\\", "⋱"));

        snippets.append(Snippet("for", "∀"));
        snippets.append(Snippet("exi", "∃"));
        snippets.append(Snippet("nn", "∩"));
        snippets.append(Snippet("uu", "∪"));
        snippets.append(Snippet("⊂=", "⊆"));
        snippets.append(Snippet("cc", "⊂"));
        snippets.append(Snippet("⊃=", "⊇"));
        snippets.append(Snippet("pp", "⊃"));

        snippets.append(Snippet("ll", "≪"));
        snippets.append(Snippet("gg", "≫"));
        snippets.append(Snippet("le", "≤"));
        snippets.append(Snippet("ge", "≥"));

        snippets.append(Snippet("⇐>", "⇔"));
        snippets.append(Snippet("=>", "⇒"));
        snippets.append(Snippet("<=", "⇐"));
        snippets.append(Snippet("←>", "↔"));
        snippets.append(Snippet("->", "→"));
        snippets.append(Snippet("<-", "←"));

        snippets.append(Snippet("xx", "×"));
        snippets.append(Snippet("@", "∘"));
        snippets.append(Snippet("+-", "±"));
        snippets.append(Snippet("⋅*", "*"));
        snippets.append(Snippet("**", "⋅⋅⋅"));
        snippets.append(Snippet("*", "⋅"));
        snippets.append(Snippet("o.", "⊙"));
        snippets.append(Snippet("o+", "⊕"));
        snippets.append(Snippet("ox", "⊗"));

        snippets.append(Snippet("par", "∂"));
        snippets.append(Snippet("oo", "∞"));

        snippets.append(Snippet("sin", "sin")); // sin is not s\in
        snippets.append(Snippet("!ni", "∌"));
        snippets.append(Snippet("!in", "∉"));
        snippets.append(Snippet("in", "∈"));
        snippets.append(Snippet("ni", "∋"));

        snippets.append(Snippet("==", "≡"));
        snippets.append(Snippet("!=", "≠"));
        snippets.append(Snippet("~~", "≈"));
        snippets.append(Snippet("~=", "≅"));

        snippets.append(Snippet("sum", "∑"));
        snippets.append(Snippet("prod", "∏"));
        snippets.append(Snippet("∈t", "∫"));
        snippets.append(Snippet("∫i", "∫_{-∞}^{+∞}"));

        snippets.append(Snippet("!O", "∅"));

        snippets.append(Snippet("alp", "α"));
        snippets.append(Snippet("bet", "β"));
        snippets.append(Snippet("Gam", "Γ"));
        snippets.append(Snippet("gam", "γ"));
        snippets.append(Snippet("Del", "Δ"));
        snippets.append(Snippet("del", "δ"));
        snippets.append(Snippet("eps", "ɛ")); // varepsilon
        snippets.append(Snippet("zet", "ζ"));
        snippets.append(Snippet("eta", "η"));
        snippets.append(Snippet("The", "Θ"));
        snippets.append(Snippet("the", "ϑ")); // vartheta
        snippets.append(Snippet("iot", "ι"));
        snippets.append(Snippet("kap", "κ"));
        snippets.append(Snippet("Lam", "Λ"));
        snippets.append(Snippet("lam", "λ"));
        snippets.append(Snippet("mu", "μ"));
        snippets.append(Snippet("nu", "ν"));
        snippets.append(Snippet("Xi", "Ξ"));
        snippets.append(Snippet("xi", "ξ"));
        snippets.append(Snippet("Pi", "Π"));
        snippets.append(Snippet("pi", "π"));
        snippets.append(Snippet("rho", "ρ"));
        snippets.append(Snippet("Sig", "Σ"));
        snippets.append(Snippet("sig", "σ"));
        snippets.append(Snippet("tau", "τ"));
        snippets.append(Snippet("ups", "υ"));
        snippets.append(Snippet("Phi", "Φ"));
        snippets.append(Snippet("phi", "φ")); // varphi
        snippets.append(Snippet("chi", "χ"));
        snippets.append(Snippet("Psi", "Ψ"));
        snippets.append(Snippet("psi", "ψ"));
        snippets.append(Snippet("Ome", "Ω"));
        snippets.append(Snippet("ome", "ω"));
    } else {
        snippets.append(Snippet("sssec", "\\subsubsection{}", 15, true));
        snippets.append(Snippet("ssec", "\\subsection{}", 12, true));
        snippets.append(Snippet("sec", "\\section{}", 9, true));

        snippets.append(Snippet("section*{", "section{", true));
        snippets.append(Snippet("section{", "section*{", true));

        snippets.append(Snippet("thm", "\\begin{theorem}\n\t\n\\end{theorem}\n", 17, true));
        snippets.append(Snippet("def", "\\begin{definition}\n\t\n\\end{definition}\n", 20, true));
        snippets.append(Snippet("prf", "\\begin{proof}\n\t\n\\end{proof}\n", 15, true));
        snippets.append(Snippet("cor", "\\begin{corollary}\n\t\n\\end{corollary}\n", 19, true));
        snippets.append(Snippet("rmk", "\\begin{remark}\n\t\n\\end{remark}\n", 16, true));
        snippets.append(Snippet("lem", "\\begin{lemma}\n\t\n\\end{lemma}\n", 15, true));
        snippets.append(Snippet("exm", "\\begin{example}\n\t\n\\end{example}\n", 17, true));

        snippets.append(Snippet("\\begin{theorem}\n\t","\\begin{theorem}[]\n\t", 16, true));
        snippets.append(Snippet("\\begin{definition}\n\t", "\\begin{definition}[]\n\t", 19, true));
        snippets.append(Snippet("\\begin{proof}\n\t", "\\begin{proof}[]\n\t", 14, true));
        snippets.append(Snippet("\\begin{corollary}\n\t", "\\begin{corollary}[]\n\t", 18, true));
        snippets.append(Snippet("\\begin{remark}\n\t", "\\begin{remark}[]\n\t", 15, true));
        snippets.append(Snippet("\\begin{lemma}\n\t", "\\begin{lemma}[]\n\t", 14, true));
        snippets.append(Snippet("\\begin{example}\n\t", "\\begin{example}[]\n\t", 16, true));

        snippets.append(Snippet("doc", "\\documentclass{article}\n\\usepackage{lexpack}\n\n\\begin{document}\n\n\n\n\\end{document}\n", 64, true));
        snippets.append(Snippet("item", "\\begin{itemize}\n\t\\item \n\\end{itemize}\n", 23, true));
        snippets.append(Snippet("enum", "\\begin{enumerate}\n\t\\item \n\\end{enumerate}\n", 25, true));
    }

    for (const auto &snippet : snippets)
        if (snippet.apply(_editor))
            break;
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
    if (_path.edited() && QMessageBox::question(this, "Quit", "Do you really want to exit the program?") != QMessageBox::Yes)
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
