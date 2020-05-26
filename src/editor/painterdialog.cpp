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

    _snippets = Styler::get<QVariant>("snippets").value<SnippetManager>();

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

    connect(_toolkit, &PainterToolkit::line, this, &PainterDialog::line);
    connect(_toolkit, &PainterToolkit::rectangle, this, &PainterDialog::rectangle);
    connect(_toolkit, &PainterToolkit::ellipse, this, &PainterDialog::ellipse);
    connect(_toolkit, &PainterToolkit::path, this, &PainterDialog::path);
    connect(_toolkit, &PainterToolkit::text, this, &PainterDialog::text);
    connect(_toolkit, &PainterToolkit::lineSolid, this, &PainterDialog::lineSolid);
    connect(_toolkit, &PainterToolkit::lineDash, this, &PainterDialog::lineDash);
    connect(_toolkit, &PainterToolkit::tipSoft, this, &PainterDialog::tipSoft);
    connect(_toolkit, &PainterToolkit::tipArrow, this, &PainterDialog::tipArrow);
    connect(_toolkit, &PainterToolkit::tipDouble, this, &PainterDialog::tipDouble);
    connect(_toolkit, &PainterToolkit::fillSolid, this, &PainterDialog::fillSolid);
    connect(_toolkit, &PainterToolkit::fillEmpty, this, &PainterDialog::fillEmpty);

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
