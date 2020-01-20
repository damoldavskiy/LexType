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
    void cancel();
    void line();
    void ellipse();
    void rectangle();
    void path();
    void solid();
    void dashes();

private:
    Painter *_painter;
    bool _canceled = false;
};

#endif // PAINTERDIALOG_H
