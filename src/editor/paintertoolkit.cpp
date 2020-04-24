#include "paintertoolkit.h"

#include "styler.h"

PainterToolkit::PainterToolkit(QWidget* parent)
    : QToolBar(parent)
{
    setStyleSheet(Styler::get<QString>("toolbar-style"));
    setOrientation(Qt::Vertical);

    createActions();
    createMenus();

    _path->activate({ });
}

void PainterToolkit::uncheckTools()
{
    _line->setChecked(false);
    _rectangle->setChecked(false);
    _ellipse->setChecked(false);
    _path->setChecked(false);
    _text->setChecked(false);
}

void PainterToolkit::uncheckLine()
{
    _lineSolid->setChecked(false);
    _lineDash->setChecked(false);
}

void PainterToolkit::uncheckTip()
{
    _tipSoft->setChecked(false);
    _tipArrow->setChecked(false);
    _tipDouble->setChecked(false);
}

void PainterToolkit::uncheckFill()
{
    _fillEmpty->setChecked(false);
    _fillSolid->setChecked(false);
}

void PainterToolkit::showLine(bool value)
{
    if (value && _lineSeparator->isVisible()) {
        uncheckLine();
        _lineSolid->setChecked(true);
    }
    _lineSeparator->setVisible(value);
    _lineSolid->setVisible(value);
    _lineDash->setVisible(value);
}

void PainterToolkit::showTip(bool value)
{
    if (value && _tipSeparator->isVisible()) {
        uncheckTip();
        _tipSoft->setChecked(true);
    }
    _tipSeparator->setVisible(value);
    _tipSoft->setVisible(value);
    _tipArrow->setVisible(value);
    _tipDouble->setVisible(value);
}

void PainterToolkit::showFill(bool value)
{
    if (value && _fillSeparator->isVisible()) {
        uncheckFill();
        _fillEmpty->setChecked(true);
    }
    _fillSeparator->setVisible(value);
    _fillEmpty->setVisible(value);
    _fillSolid->setVisible(value);
}

void PainterToolkit::createActions()
{
    _line = new QAction(QIcon(":/line.png"), "Line", this);
    _line->setShortcut(QKeySequence("L"));
    _line->setCheckable(true);
    connect(_line, &QAction::triggered, this, [this] () {
        uncheckTools();
        _line->setChecked(true);
        showLine(true);
        showTip(true);
        showFill(false);
        emit line();
    });

    _rectangle = new QAction(QIcon(":/rectangle.png"), "Rectangle", this);
    _rectangle->setShortcut(QKeySequence("R"));
    _rectangle->setCheckable(true);
    connect(_rectangle, &QAction::triggered, this, [this] () {
        uncheckTools();
        _rectangle->setChecked(true);
        showLine(true);
        showTip(false);
        showFill(true);
        emit rectangle();
    });

    _ellipse = new QAction(QIcon(":/ellipse.png"), "Ellipse", this);
    _ellipse->setShortcut(QKeySequence("E"));
    _ellipse->setCheckable(true);
    connect(_ellipse, &QAction::triggered, this, [this] () {
        uncheckTools();
        _ellipse->setChecked(true);
        showLine(true);
        showTip(false);
        showFill(true);
        emit ellipse();
    });

    _path = new QAction(QIcon(":/path.png"), "Path", this);
    _path->setShortcut(QKeySequence("P"));
    _path->setCheckable(true);
    connect(_path, &QAction::triggered, this, [this] () {
        uncheckTools();
        _path->setChecked(true);
        showLine(true);
        showTip(true);
        showFill(true);
        emit path();
    });

    _text = new QAction(QIcon(":/text.png"), "Text", this);
    _text->setShortcut(QKeySequence("T"));
    _text->setCheckable(true);
    connect(_text, &QAction::triggered, this, [this] () {
        uncheckTools();
        _text->setChecked(true);
        showLine(false);
        showTip(false);
        showFill(false);
        emit text();
    });

    _lineSolid = new QAction(QIcon(":/lineSolid.png"), "Solid line", this);
    _lineSolid->setShortcut(QKeySequence("S"));
    _lineSolid->setCheckable(true);
    connect(_lineSolid, &QAction::triggered, this, [this] () {
        uncheckLine();
        _lineSolid->setChecked(true);
        emit lineSolid();
    });

    _lineDash = new QAction(QIcon(":/lineDash.png"), "Dash line", this);
    _lineDash->setShortcut(QKeySequence("D"));
    _lineDash->setCheckable(true);
    connect(_lineDash, &QAction::triggered, this, [this] () {
        uncheckLine();
        _lineDash->setChecked(true);
        emit lineDash();
    });

    _tipSoft = new QAction(QIcon(":/tipSoft.png"), "No arrow", this);
    _tipSoft->setShortcut(QKeySequence("Q"));
    _tipSoft->setCheckable(true);
    connect(_tipSoft, &QAction::triggered, this, [this] () {
        uncheckTip();
        _tipSoft->setChecked(true);
        emit tipSoft();
    });

    _tipArrow = new QAction(QIcon(":/tipArrow.png"), "Single arrow", this);
    _tipArrow->setShortcut(QKeySequence("A"));
    _tipArrow->setCheckable(true);
    connect(_tipArrow, &QAction::triggered, this, [this] () {
        uncheckTip();
        _tipArrow->setChecked(true);
        emit tipArrow();
    });

    _tipDouble = new QAction(QIcon(":/tipDouble.png"), "Double arrow", this);
    _tipDouble->setShortcut(QKeySequence("W"));
    _tipDouble->setCheckable(true);
    connect(_tipDouble, &QAction::triggered, this, [this] () {
        uncheckTip();
        _tipDouble->setChecked(true);
        emit tipDouble();
    });

    _fillEmpty = new QAction(QIcon(":/fillEmpty.png"), "Empty fill", this);
    _fillEmpty->setShortcut(QKeySequence("G"));
    _fillEmpty->setCheckable(true);
    connect(_fillEmpty, &QAction::triggered, this, [this] () {
        uncheckFill();
        _fillEmpty->setChecked(true);
        emit fillEmpty();
    });

    _fillSolid = new QAction(QIcon(":/fillSolid.png"), "Solid fill", this);
    _fillSolid->setShortcut(QKeySequence("F"));
    _fillSolid->setCheckable(true);
    connect(_fillSolid, &QAction::triggered, this, [this] () {
        uncheckFill();
        _fillSolid->setChecked(true);
        emit fillSolid();
    });
}

void PainterToolkit::createMenus()
{
    addActions({ _line, _rectangle, _ellipse, _path, _text});

    _lineSeparator = addSeparator();
    addActions({ _lineSolid, _lineDash });

    _tipSeparator = addSeparator();
    addActions({ _tipSoft, _tipArrow, _tipDouble });

    _fillSeparator = addSeparator();
    addActions({ _fillEmpty, _fillSolid });
}
