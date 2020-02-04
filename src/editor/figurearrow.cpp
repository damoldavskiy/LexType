#include "figurearrow.h"

void FigureArrow::drawArrow(QPainter *painter, QPointF point, qreal angle) const
{
    painter->drawLine(point, { point.x() + 4 * qCos(angle + M_PI / 4), point.y() + 4 * qSin(angle + M_PI / 4) });
    painter->drawLine(point, { point.x() + 4 * qCos(angle - M_PI / 4), point.y() + 4 * qSin(angle - M_PI / 4) });
}

void FigureArrow::setTip(Tip value)
{
    _tip = value;
}

FigureArrow::Tip FigureArrow::tip() const
{
    return _tip;
}

QStringList FigureArrow::modifiers() const
{
    QStringList modifiers = FigureContour::modifiers();

    if (_tip == Tip::Arrow)
        modifiers.append("->");
    else if (_tip == Tip::Double)
        modifiers.append("<->");

    return modifiers;
}
