#ifndef PAINTERDIALOG_H
#define PAINTERDIALOG_H

#include <QDialog>

#include "paintertoolkit.h"
#include "painter.h"

class PainterDialog : public QDialog
{
    Q_OBJECT

public:
    PainterDialog(QWidget *parent = 0);
    QString latex() const;

public slots:
    void accept();
    void line();
    void ellipse();
    void rectangle();
    void path();
    void text();
    void lineSolid();
    void lineDash();
    void tipSoft();
    void tipArrow();
    void tipDouble();
    void fillSolid();
    void fillEmpty();

private:
    PainterToolkit *_toolkit;
    Painter *_painter;
    bool _accepted = false;
};

#endif // PAINTERDIALOG_H
