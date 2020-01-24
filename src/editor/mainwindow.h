#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileInfo>
#include <QSplitter>
#include <QProcess>
#include <QLabel>

#include "editor.h"

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

private slots:
    void output();
    void compiled(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createMenus();
    void saveFile(const QString &path);
    void updateFileName(const QString &path);

    QSplitter *_splitter;
    LineNumbers *_numbers;
    Editor *_editor;
    Editor *_console;
    QLabel *_status;

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

    QFileInfo _fileInfo;
    QProcess *_compilation = nullptr;
};

#endif // MAINWINDOW_H
