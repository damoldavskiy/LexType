#include "painter.h"

#include "styler.h"
#include "figureline.h"
#include "figureellipse.h"
#include "figurerectangle.h"
#include "figurepath.h"
#include "figuretext.h"

Painter::Painter(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::CrossCursor);
    setFocusPolicy(Qt::ClickFocus);
}

Painter::~Painter()
{
    bool deleteCurrent = _figures.isEmpty() || _figures.last() != _current;

    // TODO Deal with figure and painter destructors
    for (auto figure : _figures)
        delete figure;
    for (auto figure : _afterFigures)
        delete figure;

    if (deleteCurrent)
        delete _current;
}

QString Painter::latex() const
{
    if (_figures.size() == 0)
        return "";

    QString result;
    for (auto figure : _figures)
        result += "\t" + figure->latex() + "\n";

    return "\\begin{picture}\n" + result + "\\end{picture}\n";
}

Figure* Painter::figure() const
{
    return _current;
}

void Painter::setFigure(Figure* value)
{
    if (!_figures.empty() && _figures.last() == _current)
        _figures.last() = value;

    delete _current;
    _current = value;
}

void Painter::undo()
{
    if (!_figures.isEmpty())
        _afterFigures.push(_figures.pop());
    update();
}

void Painter::redo()
{
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
    painter.fillRect(0, 0, width, height, Styler::get<QColor>("painter-back"));

    if (_attracted) {
        painter.setPen(Styler::get<QColor>("painter-highlight"));
        int r = Styler::get<int>("painter-attract-radius");
        painter.drawEllipse(_mouse, r, r);
    }

    painter.setPen(Styler::get<QColor>("painter-fore"));
    painter.setFont(QFont("CMU Serif", 14));

    for (auto figure : _figures)
        figure->paint(&painter);
}

void Painter::keyPressEvent(QKeyEvent *event)
{
    _current->setEnd(_mouse, event->modifiers() & Qt::ShiftModifier);
    update();
}

void Painter::keyReleaseEvent(QKeyEvent *event)
{
    _current->setEnd(_mouse, event->modifiers() & Qt::ShiftModifier);
    update();
}

void Painter::mousePressEvent(QMouseEvent *event)
{
    _mouse = realPoint(event->pos()).toPoint();
    _attracted = _mouse != QPoint { 0, 0 };
    if (_mouse == QPoint { 0, 0 })
        _mouse = event->pos();

    _figures.append(_current);
    _current->setStart(_mouse);
    _current->setEnd(_mouse, event->modifiers() & Qt::ShiftModifier);

    for (auto figure : _afterFigures)
        delete figure;
    _afterFigures.clear();

    update();
}

void Painter::mouseReleaseEvent(QMouseEvent *)
{
    _attracted = false;

    if (_current->release())
        _current = _current->copy();
    else
        _figures.removeLast();
    _current->clear();
    emit released();

    update();
}

void Painter::mouseMoveEvent(QMouseEvent *event)
{
    _mouse = realPoint(event->pos()).toPoint();
    _attracted = _mouse != QPoint { 0, 0 };
    if (_mouse == QPoint { 0, 0 })
        _mouse = event->pos();

    _current->setEnd(_mouse, event->modifiers() & Qt::ShiftModifier);
    update();
}

void Painter::resizeEvent(QResizeEvent *event)
{
    qreal dx = event->size().width() - event->oldSize().width();
    qreal dy = event->size().height() - event->oldSize().height();

    for (auto &figure : _figures)
        figure->shift(dx / 2, dy / 2);
}

QPointF Painter::realPoint(QPointF point) const
{
    QPointF attraced;
    for (auto figure : _figures)
        if (figure != _current && (attraced = figure->attract(point)) != QPointF { 0, 0 })
            break;
    return attraced;
}
