#ifndef PAINTERTOOLKIT_H
#define PAINTERTOOLKIT_H

#include <QToolBar>

class PainterToolkit : public QToolBar
{
    Q_OBJECT

public:
    PainterToolkit(QWidget* parent = 0);

signals:
    void line();
    void rectangle();
    void ellipse();
    void path();
    void text();
    void lineSolid();
    void lineDash();
    void tipSoft();
    void tipArrow();
    void tipDouble();
    void fillEmpty();
    void fillSolid();

private:
    void uncheckTools();
    void uncheckLine();
    void uncheckTip();
    void uncheckFill();

    void showLine(bool value);
    void showTip(bool value);
    void showFill(bool value);

    void createActions();
    void createMenus();

    QAction *_line;
    QAction *_rectangle;
    QAction *_ellipse;
    QAction *_path;
    QAction *_text;
    QAction *_lineSolid;
    QAction *_lineDash;
    QAction *_tipSoft;
    QAction *_tipArrow;
    QAction *_tipDouble;
    QAction *_fillEmpty;
    QAction *_fillSolid;
    QAction *_lineSeparator;
    QAction *_tipSeparator;
    QAction *_fillSeparator;
};

#endif // PAINTERTOOLKIT_H
