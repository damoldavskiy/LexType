#include "figurerectangle.h"

FigureRectangle::FigureRectangle(QPointF start)
    : Figure(start)
{ }

void FigureRectangle::update(QPointF point, bool modifier, StrokeModifier strokeModifier)
{
    Figure::update(point, modifier, strokeModifier);

    if (modifier) {
        if (abs(dx()) > abs(dy()))
            _end.setX(_start.x() + dy() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
        else
            _end.setY(_start.y() + dx() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
    }
}

void FigureRectangle::release()
{ }

void FigureRectangle::paint(QPainter *painter) const
{
    Figure::paint(painter);
    painter->drawRect({ _start, _end });
}

QString FigureRectangle::latex() const
{
    return "\\draw " + latexModifier() + pair(_start) + " rectangle " + pair(_end) + ";";
}
