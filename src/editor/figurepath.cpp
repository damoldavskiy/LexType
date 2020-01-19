#include "figurepath.h"

FigurePath::FigurePath(QPointF start)
    : Figure(start), _path({ start })
{ }

void FigurePath::update(QPointF point, bool)
{
    _path.append(point);
}

void FigurePath::release()
{
    QVector<QPointF> filtered { _path[0] };

    for (int i = 1; i < _path.size(); ++i)
        if (Math::dist(filtered.last(), _path[i]) > 30)
            filtered.append(_path[i]);

    if (Math::dist(filtered.last(), _path.last()) > 0) {
        if (filtered.size() == 1)
            filtered.append(_path.last());
        else
            filtered.last() = _path.last();
    }

    if (filtered.size() > 2) {
        _path = filtered;
        _cycle = isCycle(_path);
    }
}

void FigurePath::paint(QPainter *painter) const
{
    QVector<QPointF> points = getPoints();
    QPainterPath painterPath(points[0]);

    for (int i = 3; i < points.size(); i += 3)
        painterPath.cubicTo(points[i - 2], points[i - 1], points[i]);

    int freeCount = points.size() % 3 - 1;
    Q_ASSERT(freeCount == 1 || freeCount == 2);

    if (freeCount == 1) {
        if (_cycle)
            painterPath.cubicTo(points.last(), points[0] - (points[1] - points[0]), points[0]);
        else
            painterPath.lineTo(points.last());
    } else if (_path.size() != 1) {
        if (_cycle)
            painterPath.cubicTo(points[points.size() - 2], points[0] - (points[1] - points[0]), points[0]);
        else
            painterPath.quadTo(points[points.size() - 2], points.last());
    }

    painter->drawPath(painterPath);
}

QString FigurePath::latex() const
{
    QVector<QPointF> points = getPoints();
    QString result = "\\draw " + pair(points[0]);

    for (int i = 3; i < points.size(); i += 3)
        result += " .. controls " + pair(points[i - 2]) + " and " + pair(points[i - 1]) + " .. " + pair(points[i]);

    int freeCount = points.size() % 3 - 1;
    Q_ASSERT(freeCount == 1 || freeCount == 2);

    if (freeCount == 1) {
        if (_cycle)
            result += " .. controls " + pair(points.last()) + " and " + pair(points[0] - (points[1] - points[0])) + " .. " + pair(points[0]);
        else
            result += " -- " + pair(points.last());
    } else if (_path.size() != 1) {
        if (_cycle)
            result += " .. controls " + pair(points[points.size() - 2]) + " and " + pair(points[0] - (points[1] - points[0])) + " .. " + pair(points[0]);
        else
            result += " .. controls " + pair(points[points.size() - 2]) + " .. " + pair(points.last());
    }

    return result + ";";
}

QVector<QPointF> FigurePath::getPoints() const
{
    QVector<QPointF> points { _path[0] };

    for (int i = 1; i < _path.size(); ++i) {
        points.append(_path[i]);
        if (i % 2 == 0 && i < _path.size() - 1)
            points.append((_path[i] + _path[i + 1]) / 2);
    }

    return points;
}

bool FigurePath::isCycle(const QVector<QPointF> &points)
{
    return Math::dist(points.first(), points.last()) < _cycleDist;
}

