#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QProcess>

#include "editor.h"
#include "pathwatcher.h"
#include "snippetmanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0, const QString &path = "");

public slots:
    void openFile();
    void closeFile();
    void save();
    void saveAs();
    void quit();
    void find();
    void toggleConsole();
    void compile();
    void painter();
    void wordWrap(bool checked);
    void options();
    void about();
    void aboutQt();
    void textChanged();
    void textTyped(int pos, QChar symbol);

private slots:
    void output();
    void compiled(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void tryOpenFile(const QString &path);
    void createActions();
    void createMenus();

    QSplitter *_splitter;
    LineNumbers *_numbers;
    Editor *_editor;
    Editor *_console;

    SnippetManager _snippets;

    QAction *_openAction;
    QAction *_closeAction;
    QAction *_saveAction;
    QAction *_saveAsAction;
    QAction *_quitAction;
    QAction *_undoAction;
    QAction *_redoAction;
    QAction *_findAction;
    QAction *_cutAction;
    QAction *_copyAction;
    QAction *_pasteAction;
    QAction *_selectAllAction;
    QAction *_toggleConsoleAction;
    QAction *_compileAction;
    QAction *_painterAction;
    QAction *_wordWrap;
    QAction *_optionsAction;
    QAction *_aboutAction;
    QAction *_aboutQtAction;

    PathWatcher _path = PathWatcher("LexType");
    QProcess _compilation;
    QTimer _timer;
};

#endif // MAINWINDOW_H
