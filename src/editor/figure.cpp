#include "figure.h"

Figure::Figure(QPointF start)
    : _start(start), _end(start)
{ }

Figure::~Figure()
{ }

void Figure::update(QPointF point, bool, StrokeModifier strokeModifier)
{
    _end = point;
    _strokeModifier = strokeModifier;
}

void Figure::shift(qreal shiftX, qreal shiftY)
{
    Math::shift(_start, shiftX, shiftY);
    Math::shift(_end, shiftX, shiftY);
}

void Figure::release()
{ }

void Figure::paint(QPainter *painter) const
{
    QPen pen = painter->pen();
    applyPenModifier(pen);
    painter->setPen(pen);
}

QString Figure::num(qreal n)
{
    return QString::number(n / 50);
}

QString Figure::pair(QPointF point)
{
    return "(" + num(point.x()) + "," + num(-point.y()) + ")";
}

void Figure::applyPenModifier(QPen& pen) const
{
    switch (_strokeModifier) {
    case Solid:
        pen.setStyle(Qt::SolidLine);
        break;
    case Dashes:
        pen.setStyle(Qt::DashLine);
        break;
    }
}

QString Figure::latexModifier() const
{
    switch (_strokeModifier) {
    case Solid:
        return "";
    case Dashes:
        return "[dashed] ";
    }

    return "";
}

qreal Figure::dx() const
{
    return _end.x() - _start.x();
}

qreal Figure::dy() const
{
    return _end.y() - _start.y();
}
