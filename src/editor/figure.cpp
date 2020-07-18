#include "figure.h"

#include "styler.h"

Figure::~Figure()
{ }

void Figure::shift(qreal shiftX, qreal shiftY)
{
    Math::shift(_start, shiftX, shiftY);
    Math::shift(_end, shiftX, shiftY);
}

bool Figure::release()
{
    return Math::dist(_start, _end) > 0;
}

void Figure::paint(QPainter *) const
{ }

void Figure::setStart(QPointF point)
{
    _start = point;
}

QPointF Figure::start() const
{
    return _start;
}

void Figure::setEnd(QPointF point, bool)
{
    _end = point;
}

QPointF Figure::end() const
{
    return _end;
}

void Figure::clear()
{
    _start = { 0, 0 };
    _end = { 0, 0 };
}

QPointF Figure::attract(QPointF) const
{
    return { 0, 0 };
}

QString Figure::num(qreal n)
{
    return QString::number(n / 50);
}

QString Figure::pair(QPointF point)
{
    return "(" + num(point.x()) + "," + num(-point.y()) + ")";
}

QPointF Figure::findAttract(const QVector<QPointF> &points, QPointF from)
{
    for (QPointF point : points)
        if (Math::dist(point, from) <= Styler::get<int>("painter-attract-radius"))
            return point;

    return { 0, 0 };
}

QStringList Figure::modifiers() const
{
    return { };
}

QString Figure::modifier() const
{
    QStringList list = modifiers();

    if (list.size() == 0)
        return "";
    return "[" + list.join(',') + "] ";
}

qreal Figure::dx() const
{
    return _end.x() - _start.x();
}

qreal Figure::dy() const
{
    return _end.y() - _start.y();
}
