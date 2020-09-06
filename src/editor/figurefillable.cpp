#include "figurefillable.h"

#include "styler.h"

void FigureFillable::paint(QPainter *painter) const
{
    FigureContour::paint(painter);

    QBrush brush = painter->brush();
    if (_fill == Fill::Solid)
        brush.setStyle(Qt::SolidPattern);
    else if (_fill == Fill::LinesHorizontal)
        brush.setStyle(Qt::HorPattern);
    else if (_fill == Fill::LinesVertical)
        brush.setStyle(Qt::VerPattern);
    else if (_fill == Fill::Empty)
        brush.setStyle(Qt::NoBrush);
    brush.setColor(Styler::get<QColor>("painter-fore")); // TODO Move it to painter.cpp?

    painter->setBrush(brush);
}

QStringList FigureFillable::modifiers() const
{
    QStringList list = FigureContour::modifiers();

    if (_fill == Fill::Solid)
        list.append("fill=black");
    else if (_fill == Fill::LinesHorizontal)
        list.append("pattern=horizontal lines");
    else if (_fill == Fill::LinesVertical)
        list.append("pattern=vertical lines");

    return list;
}

void FigureFillable::setFill(Fill value)
{
    _fill = value;
}

FigureFillable::Fill FigureFillable::fill() const
{
    return _fill;
}
