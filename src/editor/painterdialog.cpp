#include "painterdialog.h"

#include <QLayout>
#include <QShortcut>

#include "figureline.h"
#include "figurerectangle.h"
#include "figureellipse.h"
#include "figurepath.h"
#include "figuretext.h"

PainterDialog::PainterDialog()
{
    setLayout(new QVBoxLayout);
    resize(480, 480);

    _painter = new Painter;

    layout()->setMargin(0);
    layout()->addWidget(_painter);

    new QShortcut(QKeySequence("Ctrl+D"), this, SLOT(accept()));
    new QShortcut(QKeySequence("Ctrl+Shift+D"), this, SLOT(close()));
    new QShortcut(QKeySequence("Ctrl+Z"), _painter, SLOT(undo()));
    new QShortcut(QKeySequence("Ctrl+Shift+Z"), _painter, SLOT(redo()));
    new QShortcut(QKeySequence("L"), this, SLOT(line()));
    new QShortcut(QKeySequence("E"), this, SLOT(ellipse()));
    new QShortcut(QKeySequence("R"), this, SLOT(rectangle()));
    new QShortcut(QKeySequence("P"), this, SLOT(path()));
    new QShortcut(QKeySequence("T"), this, SLOT(text()));
    new QShortcut(QKeySequence("S"), this, SLOT(lineSolid()));
    new QShortcut(QKeySequence("D"), this, SLOT(lineDash()));
    new QShortcut(QKeySequence("Q"), this, SLOT(tipSoft()));
    new QShortcut(QKeySequence("A"), this, SLOT(tipArrow()));
    new QShortcut(QKeySequence("W"), this, SLOT(tipDouble()));
    new QShortcut(QKeySequence("F"), this, SLOT(fillSolid()));
    new QShortcut(QKeySequence("G"), this, SLOT(fillEmpty()));
}

QString PainterDialog::latex() const
{
    return _accepted ? _painter->latex() : "";
}

void PainterDialog::accept()
{
    _accepted = true;
    close();
}

void PainterDialog::line()
{
    _painter->setFigure(new FigureLine);
}

void PainterDialog::ellipse()
{
    _painter->setFigure(new FigureEllipse);
}

void PainterDialog::rectangle()
{
    _painter->setFigure(new FigureRectangle);
}

void PainterDialog::path()
{
    _painter->setFigure(new FigurePath);
}

void PainterDialog::text()
{
    _painter->setFigure(new FigureText);
}

void PainterDialog::lineSolid()
{
    FigureContour *figure = dynamic_cast<FigureContour*>(_painter->figure());
    figure->setStroke(FigureContour::Solid);
}

void PainterDialog::lineDash()
{
    FigureContour *figure = dynamic_cast<FigureContour*>(_painter->figure());
    figure->setStroke(FigureContour::Dash);
}

void PainterDialog::tipSoft()
{
    FigureArrow *figure = dynamic_cast<FigureArrow*>(_painter->figure());
    figure->setTip(FigureArrow::Soft);
}

void PainterDialog::tipArrow()
{
    FigureArrow *figure = dynamic_cast<FigureArrow*>(_painter->figure());
    figure->setTip(FigureArrow::Arrow);
}

void PainterDialog::tipDouble()
{
    FigureArrow *figure = dynamic_cast<FigureArrow*>(_painter->figure());
    figure->setTip(FigureArrow::Double);
}

void PainterDialog::fillSolid()
{
    FigureFillable *figure = dynamic_cast<FigureFillable*>(_painter->figure());
    figure->setFill(FigureFillable::Solid);
}

void PainterDialog::fillEmpty()
{
    FigureFillable *figure = dynamic_cast<FigureFillable*>(_painter->figure());
    figure->setFill(FigureFillable::Empty);
}
