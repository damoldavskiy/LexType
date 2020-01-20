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
    case Dots:
        pen.setStyle(Qt::DotLine);
        break;
    }
}

QString Figure::latexModifier() const
{
    switch (_strokeModifier) {
    case Solid:
        return "";
    case Dots:
        return "[dotted] ";
    default:
        return "";
    }
}

qreal Figure::dx() const
{
    return _end.x() - _start.x();
}

qreal Figure::dy() const
{
    return _end.y() - _start.y();
}
