#include "figure.h"

Figure::Figure(QPointF start)
    : _start(start), _end(start)
{ }

Figure::~Figure()
{ }

void Figure::update(QPointF point, bool)
{
    _end = point;
}

void Figure::release()
{ }

QString Figure::num(qreal n)
{
    return QString::number(n / 50);
}

QString Figure::pair(QPointF point)
{
    return "(" + num(point.x()) + "," + num(-point.y()) + ")";
}

qreal Figure::dx() const
{
    return _end.x() - _start.x();
}

qreal Figure::dy() const
{
    return _end.y() - _start.y();
}
