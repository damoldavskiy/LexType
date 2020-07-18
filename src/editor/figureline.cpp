#include "figureline.h"

void FigureLine::setEnd(QPointF point, bool modifier)
{
    Figure::setEnd(point);

    if (modifier) {
        qreal len = Math::dist(start(), end());
        qreal angle = qAtan2(dy(), dx());
        qreal step = M_PI / 12;
        qreal angleRound = static_cast<int>(angle / step) * step;
        if (abs(angle - angleRound) > step / 2)
            angleRound += angle > 0 ? step : -step;

        Figure::setEnd({ start().x() + qCos(angleRound) * len, start().y() + qSin(angleRound) * len });
    }
}

void FigureLine::paint(QPainter *painter) const
{
    FigureContour::paint(painter);
    painter->drawLine(start(), end());

    if (tip() == Tip::Arrow) {
        drawArrow(painter, end(), qAtan2(dy(), dx()) + M_PI);
    } else if (tip() == Tip::Double) {
        qreal angle = qAtan2(dy(), dx());
        drawArrow(painter, start(), angle);
        drawArrow(painter, end(), angle + M_PI);
    }
}

QString FigureLine::latex() const
{
    return "\\draw " + modifier() + pair(start()) + " -- " + pair(end()) + ";";
}

Figure* FigureLine::copy() const
{
    FigureLine* figure = new FigureLine;
    figure->setStart(start());
    figure->setEnd(end());
    figure->setStroke(stroke());
    figure->setTip(tip());
    return figure;
}

QPointF FigureLine::attract(QPointF point) const
{
    return findAttract({ _start, _end }, point);
}
