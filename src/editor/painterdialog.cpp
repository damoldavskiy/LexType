#include "painterdialog.h"

#include <QLayout>
#include <QShortcut>

PainterDialog::PainterDialog()
{
    setLayout(new QVBoxLayout);
    resize(480, 480);

    _painter = new Painter;

    layout()->setMargin(0);
    layout()->addWidget(_painter);

    new QShortcut(QKeySequence("Ctrl+D"), this, SLOT(close()));
    new QShortcut(QKeySequence("Ctrl+Shift+D"), this, SLOT(cancel()));
    new QShortcut(QKeySequence("Ctrl+Z"), _painter, SLOT(undo()));
    new QShortcut(QKeySequence("Ctrl+Shift+Z"), _painter, SLOT(redo()));
    new QShortcut(QKeySequence("L"), this, SLOT(line()));
    new QShortcut(QKeySequence("E"), this, SLOT(ellipse()));
    new QShortcut(QKeySequence("R"), this, SLOT(rectangle()));
    new QShortcut(QKeySequence("P"), this, SLOT(path()));
    new QShortcut(QKeySequence("S"), this, SLOT(solid()));
    new QShortcut(QKeySequence("D"), this, SLOT(dots()));
}

QString PainterDialog::latex() const
{
    return _canceled ? "" : _painter->latex();
}

void PainterDialog::cancel()
{
    _canceled = true;
    close();
}

void PainterDialog::line()
{
    _painter->setType(Figure::Line);
}

void PainterDialog::ellipse()
{
    _painter->setType(Figure::Ellipse);
}

void PainterDialog::rectangle()
{
    _painter->setType(Figure::Rectangle);
}

void PainterDialog::path()
{
    _painter->setType(Figure::Path);
}

void PainterDialog::solid()
{
    _painter->setStrokeModifier(Figure::Solid);
}

void PainterDialog::dots()
{
    _painter->setStrokeModifier(Figure::Dots);
}
