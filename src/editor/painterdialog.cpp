#include "painterdialog.h"

#include <QLayout>
#include <QShortcut>

#include "figureline.h"
#include "figurerectangle.h"
#include "figureellipse.h"
#include "figurepath.h"
#include "figuretext.h"
#include "paintertoolkit.h"
#include "styler.h"

PainterDialog::PainterDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Painter");
    setStyleSheet(Styler::get<QString>("widget-style"));
    setLayout(new QVBoxLayout);
    resize(480, 480);

    _toolkit = new PainterToolkit;
    _painter = new Painter;
    _editor = new Editor;
    _editor->setVisible(false);
    _editor->setMaximumHeight(_editor->fontMetrics().height());
    connect(_editor, &Editor::typed, this, [this] (int, QChar symbol) {
        if (symbol == '\n') {
            _toolkit->reset();
            return;
        }
        _snippets.apply(_editor, false, Styler::get<bool>("painter-flag-snippets"));

        FigureText *figure = dynamic_cast<FigureText*>(_painter->figure());
        figure->setText(_editor->text());
    });
    connect(_painter, &Painter::released, this, [this] () {
        if (dynamic_cast<FigureText*>(_painter->figure()) == nullptr)
            return;
        _editor->setText("");
        _editor->setFocus();
    });

    layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->addWidget(_toolkit);
    layout()->addWidget(_painter);
    layout()->addWidget(_editor);

    connect(_toolkit, SIGNAL(line()), this, SLOT(line()));
    connect(_toolkit, SIGNAL(rectangle()), this, SLOT(rectangle()));
    connect(_toolkit, SIGNAL(ellipse()), this, SLOT(ellipse()));
    connect(_toolkit, SIGNAL(path()), this, SLOT(path()));
    connect(_toolkit, SIGNAL(text()), this, SLOT(text()));
    connect(_toolkit, SIGNAL(lineSolid()), this, SLOT(lineSolid()));
    connect(_toolkit, SIGNAL(lineDash()), this, SLOT(lineDash()));
    connect(_toolkit, SIGNAL(tipSoft()), this, SLOT(tipSoft()));
    connect(_toolkit, SIGNAL(tipArrow()), this, SLOT(tipArrow()));
    connect(_toolkit, SIGNAL(tipDouble()), this, SLOT(tipDouble()));
    connect(_toolkit, SIGNAL(fillSolid()), this, SLOT(fillSolid()));
    connect(_toolkit, SIGNAL(fillEmpty()), this, SLOT(fillEmpty()));

    new QShortcut(QKeySequence("Ctrl+D"), this, SLOT(accept()));
    new QShortcut(QKeySequence("Ctrl+Shift+D"), this, SLOT(close()));
    new QShortcut(QKeySequence("Ctrl+Z"), _painter, SLOT(undo()));
    new QShortcut(QKeySequence("Ctrl+Shift+Z"), _painter, SLOT(redo()));
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
    _editor->setVisible(false);
    _editor->setEnabled(false);
    _painter->setFigure(new FigureLine);
}

void PainterDialog::ellipse()
{
    _editor->setVisible(false);
    _editor->setEnabled(false);
    _painter->setFigure(new FigureEllipse);
}

void PainterDialog::rectangle()
{
    _editor->setVisible(false);
    _editor->setEnabled(false);
    _painter->setFigure(new FigureRectangle);
}

void PainterDialog::path()
{
    _editor->setVisible(false);
    _editor->setEnabled(false);
    _painter->setFigure(new FigurePath);
}

void PainterDialog::text()
{
    _editor->setVisible(true);
    _editor->setEnabled(true);
    _editor->setFocus();
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
