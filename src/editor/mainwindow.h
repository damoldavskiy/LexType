#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QProcess>

#include "editor.h"
#include "pathwatcher.h"
#include "snippetmanager.h"

/**
 * @brief Application main window class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs main window and application
     * @param Parent widget
     */
    MainWindow(QWidget *parent = 0);

public slots:
    /**
     * @brief Opens file
     */
    void open();

    /**
     * @brief Saves file without dialog
     */
    void save();

    /**
     * @brief Saves file with dialog
     */
    void saveAs();

    /**
     * @brief Quits the application
     */
    void quit();

    /**
     * @brief Opens find dialog
     */
    void find();

    /**
     * @brief Tries to compile current text
     */
    void compile();

    /**
     * @brief Opens painter dialog
     */
    void painter();

    /**
     * @brief Opens options dialog
     */
    void options();

    /**
     * @brief Updates application when users types data to editor
     * @param Position of typing
     * @param Sybsol typed
     */
    void typed(int pos, QChar symbol);

private slots:
    void output();
    void compiled(int exitCode, QProcess::ExitStatus exitStatus);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createMenus();

    QSplitter *_splitter;
    LineNumbers *_numbers;
    Editor *_editor;
    Editor *_console;

    SnippetManager _snippets;

    QAction *_openAction;
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
    QAction *_compileAction;
    QAction *_painterAction;
    QAction *_optionsAction;

    PathWatcher _path = PathWatcher("LexType");
    QProcess *_compilation = nullptr;
};

#endif // MAINWINDOW_H
