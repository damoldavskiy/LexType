#include "figurerectangle.h"

void FigureRectangle::setEnd(QPointF point, bool modifier/*, StrokeModifier strokeModifier*/)
{
    Figure::setEnd(point);

    if (modifier) {
        if (abs(dx()) > abs(dy()))
            _end.setX(start().x() + dy() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
        else
            _end.setY(start().y() + dx() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
    }
}

void FigureRectangle::paint(QPainter *painter) const
{
    FigureFillable::paint(painter);
    painter->drawRect({ start(), end() });
}

QString FigureRectangle::latex() const
{
    return "\\draw " + modifier() + pair(start()) + " rectangle " + pair(end()) + ";";
}

Figure* FigureRectangle::copy() const
{
    FigureRectangle *figure = new FigureRectangle;
    figure->setStart(start());
    figure->setEnd(end());
    figure->setStroke(stroke());
    figure->setFill(fill());
    return figure;
}
