#include "figurecontour.h"

void FigureContour::paint(QPainter *painter) const
{
    QPen pen = painter->pen();
    if (_stroke == Stroke::Solid)
        pen.setStyle(Qt::SolidLine);
    else if (_stroke == Stroke::Dash)
        pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
}

QStringList FigureContour::modifiers() const
{
    if (_stroke == Stroke::Dash)
        return { "dashed" };
    return { };
}

void FigureContour::setStroke(Stroke value)
{
    _stroke = value;
}

FigureContour::Stroke FigureContour::stroke() const
{
    return _stroke;
}
