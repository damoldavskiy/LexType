#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QScrollArea>
#include <QVBoxLayout>
#include <poppler-qt5.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void open();
    void quit();
    void zoomIn();
    void zoomOut();

private:
    void createActions();
    void createMenus();
    void loadDocument();

    QColor _background = { 50, 50, 50 };
    QColor _foreground = { 240, 240, 240 };

    QScrollArea *scroll;
    QWidget *parent;
    QVBoxLayout *layout;

    QAction *_openAction;
    QAction *_quitAction;
    QAction *_zoomInAction;
    QAction *_zoomOutAction;

    QString path;
    qreal res = 72;
};

#endif // MAINWINDOW_H
