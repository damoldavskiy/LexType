#ifndef PAINTERDIALOG_H
#define PAINTERDIALOG_H

#include <QDialog>

#include "painter.h"

class PainterDialog : public QDialog
{
    Q_OBJECT

public:
    PainterDialog();
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
    Painter *_painter;
    bool _accepted = false;
};

#endif // PAINTERDIALOG_H
