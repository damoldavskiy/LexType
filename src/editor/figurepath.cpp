#include "figurepath.h"

void FigurePath::setStart(QPointF point)
{
    Figure::setStart(point);

    _path = { point };
}

void FigurePath::setEnd(QPointF point, bool)
{
    Figure::setEnd(point);

    if (_path.isEmpty() || Math::dist(_path.back(), point) > 4)
        _path.append(point);
}

void FigurePath::shift(qreal shiftX, qreal shiftY)
{
    Figure::shift(shiftX, shiftY);
    for (auto &point : _path)
        Math::shift(point, shiftX, shiftY);
}

bool FigurePath::release()
{
    if (_path.size() == 1) {
        return false;
    }

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

    return true;
}

void FigurePath::paint(QPainter *painter) const
{
    FigureContour::paint(painter);
    FigureFillable::paint(painter);
    QVector<QPointF> points = getPoints();
    QPainterPath painterPath(points[0]);

    for (int i = 3; i < points.size(); i += 3)
        painterPath.cubicTo(points[i - 2], points[i - 1], points[i]);

    int freeCount = points.size() % 3 - 1;
    // TODO Assert is not correct because of initial painting
    // Q_ASSERT(freeCount == 1 || freeCount == 2);

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

    if (points.size() > 1 && !_cycle) {
        if (tip() == Tip::Arrow || tip() == Tip::Double) {
            qreal dx = points.last().x() - points[points.size() - 2].x();
            qreal dy = points.last().y() - points[points.size() - 2].y();
            // TODO Why line doesn't need it?
            dy *= -1;
            qreal angle = qAtan2(dx, dy) + M_PI / 2;

            drawArrow(painter, points.last(), angle);
        }
        if (tip() == Tip::Double) {
            qreal dx = points.first().x() - points[1].x();
            qreal dy = points.first().y() - points[1].y();
            dy *= -1;
            qreal angle = qAtan2(dx, dy) + M_PI / 2;

            drawArrow(painter, points.first(), angle);
        }
    }
}

QString FigurePath::latex() const
{
    QVector<QPointF> points = getPoints();
    QString result = "\\draw " + modifier() + pair(points[0]);

    for (int i = 3; i < points.size(); i += 3)
        result += " .. controls " + pair(points[i - 2]) + " and " + pair(points[i - 1]) + " .. " + pair(points[i]);

    int freeCount = points.size() % 3 - 1;
//    Q_ASSERT(freeCount == 1 || freeCount == 2);

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

Figure* FigurePath::copy() const
{
    FigurePath* figure = new FigurePath;
    figure->setStart(start());
    figure->setEnd(end());
    figure->setPath(path());
    figure->setStroke(stroke());
    figure->setTip(tip());
    figure->setFill(fill());
    return figure;
}

void FigurePath::clear()
{
    _path.clear();
}

QStringList FigurePath::modifiers() const
{
    QStringList listArrow = FigureArrow::modifiers();
    QStringList listFill = FigureFillable::modifiers();

    if (_cycle) {
        listArrow.clear();
    }

    QStringList result = listArrow + listFill;
    result.removeDuplicates();
    return result;
}

QPointF FigurePath::attract(QPointF point) const
{
    return findAttract({ _start, _end }, point);
}

const QVector<QPointF>& FigurePath::path() const
{
    return _path;
}

void FigurePath::setPath(const QVector<QPointF> &value)
{
    _path = value;
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
