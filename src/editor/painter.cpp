#include "painter.h"

#include "styler.h"
#include "figureline.h"
#include "figureellipse.h"
#include "figurerectangle.h"
#include "figurepath.h"

Painter::Painter(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::ClickFocus);
}

Painter::~Painter()
{
    // TODO Deal with figure and painter destructors
    for (auto& figure : _figures)
        delete figure;
    for (auto& figure : _afterFigures)
        delete figure;
}

QString Painter::latex() const
{
    if (_figures.size() == 0)
        return "";

    QString result;
    for (const auto& figure : _figures)
        result += "\t" + figure->latex() + "\n";

    return "\\begin{picture}\n" + result + "\\end{picture}\n";
}

void Painter::setType(Figure::Type type)
{
    _type = type;
}

void Painter::setStrokeModifier(Figure::StrokeModifier modifier)
{
    _strokeModifier = modifier;
}

void Painter::undo()
{
    if (_edit)
        return;
    if (!_figures.isEmpty())
        _afterFigures.push(_figures.pop());
    update();
}

void Painter::redo()
{
    if (_edit)
        return;
    if (!_afterFigures.isEmpty())
        _figures.push(_afterFigures.pop());
    update();
}

void Painter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    qreal width = size().width();
    qreal height = size().height();

    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(0, 0, width, height, Styler::painterBack());
    painter.setPen(Styler::painterFore());

    for (const auto &figure : _figures)
        figure->paint(&painter);
}

void Painter::keyPressEvent(QKeyEvent *event)
{
    if (_edit)
        _figures.last()->update(_mouse, event->modifiers() & Qt::ShiftModifier, _strokeModifier);
    update();
}

void Painter::keyReleaseEvent(QKeyEvent *event)
{
    if (_edit)
        _figures.last()->update(_mouse, event->modifiers() & Qt::ShiftModifier, _strokeModifier);
    update();
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    _mouse = event->pos();
    _edit = true;

    switch (_type) {
    case Figure::Line:
        _figures.append(new FigureLine(_mouse));
        break;
    case Figure::Ellipse:
        _figures.append(new FigureEllipse(_mouse));
        break;
    case Figure::Rectangle:
        _figures.append(new FigureRectangle(_mouse));
        break;
    case Figure::Path:
        _figures.append(new FigurePath(_mouse));
    }
    _afterFigures.clear();

    update();
}

void Painter::mouseReleaseEvent(QMouseEvent *)
{
    _edit = false;
    _figures.last()->release();
    update();
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    _mouse = event->pos();
    _figures.last()->update(_mouse, event->modifiers() & Qt::ShiftModifier, _strokeModifier);
    update();
}

void Painter::resizeEvent(QResizeEvent *event)
{
    qreal dx = event->size().width() - event->oldSize().width();
    qreal dy = event->size().height() - event->oldSize().height();

    for (auto &figure : _figures)
        figure->shift(dx / 2, dy / 2);
}
