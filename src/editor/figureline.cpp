#include "figureline.h"

FigureLine::FigureLine(QPointF start)
    : Figure(start)
{ }

void FigureLine::update(QPointF point, bool modifier)
{
    _end = point;

    if (modifier) {
        qreal len = Math::dist(_start, _end);
        qreal angle = qAtan2(dy(), dx());
        qreal step = M_PI / 12;
        qreal angleRound = static_cast<int>(angle / step) * step;
        if (abs(angle - angleRound) > step / 2)
            angleRound += angle > 0 ? step : -step;

        _end = { _start.x() + qCos(angleRound) * len, _start.y() + qSin(angleRound) * len };
    }
}

void FigureLine::paint(QPainter *painter) const
{
    painter->drawLine(_start, _end);
}

QString FigureLine::latex() const
{
    return "\\draw " + pair(_start) + " -- " + pair(_end) + ";";
}
