#include "figureellipse.h"

void FigureEllipse::setEnd(QPointF point, bool modifier)
{
    Figure::setEnd(point, modifier);

    if (modifier) {
        if (abs(dx()) > abs(dy()))
            Figure::setEnd({ start().x() + dy() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1), end().y() }, modifier);
        else
            Figure::setEnd({ end().x(), start().y() + dx() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1) }, modifier);
    }
}

void FigureEllipse::paint(QPainter *painter) const
{
    FigureFillable::paint(painter);
    painter->drawEllipse({ _start.x() + dx() / 2, _start.y() + dy() / 2 }, dx() / 2, dy() / 2);
}

QString FigureEllipse::latex() const
{
    return "\\draw " + modifier() + pair({ _start.x() + dx() / 2, _start.y() + dy() / 2 }) + " ellipse (" + num(dx() / 2) + " and " + num(dy() / 2) + ");";
}

Figure* FigureEllipse::copy() const
{
    FigureEllipse *figure = new FigureEllipse;
    figure->setStart(start());
    figure->setEnd(end());
    figure->setStroke(stroke());
    figure->setFill(fill());
    return figure;
}
