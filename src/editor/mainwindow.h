#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QProcess>


#include "editor.h"
#include "pathwatcher.h"
#include "snippet.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void open();
    void save();
    void saveAs();
    void quit();
    void compile();
    void painter();
    void options();
    void typed(int pos, QChar symbol);

private slots:
    void output();
    void compiled(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createSnippets();
    void createActions();
    void createMenus();

    QSplitter *_splitter;
    LineNumbers *_numbers;
    Editor *_editor;
    Editor *_console;

    QVector<Snippet> _snippetsRegular;
    QVector<Snippet> _snippetsMath;

    QAction *_openAction;
    QAction *_saveAction;
    QAction *_saveAsAction;
    QAction *_quitAction;
    QAction *_undoAction;
    QAction *_redoAction;
    QAction *_cutAction;
    QAction *_copyAction;
    QAction *_pasteAction;
    QAction *_selectAllAction;
    QAction *_compileAction;
    QAction *_painterAction;
    QAction *_optionsAction;

    PathWatcher _path = PathWatcher("LexType");
    QProcess *_compilation = nullptr;
};

#endif // MAINWINDOW_H
