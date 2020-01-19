#include "figureellipse.h"

FigureEllipse::FigureEllipse(QPointF start)
    : Figure(start)
{ }

void FigureEllipse::update(QPointF point, bool modifier)
{
    _end = point;

    if (modifier) {
        if (abs(dx()) > abs(dy()))
            _end.setX(_start.x() + dy() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
        else
            _end.setY(_start.y() + dx() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
    }
}

void FigureEllipse::paint(QPainter *painter) const
{
    painter->drawEllipse({ _start.x() + dx() / 2, _start.y() + dy() / 2 }, dx() / 2, dy() / 2);
}

QString FigureEllipse::latex() const
{
    return "\\draw " + pair({ _start.x() + dx() / 2, _start.y() + dy() / 2 }) + " ellipse (" + num(dx() / 2) + " and " + num(dy() / 2) + ");";
}
