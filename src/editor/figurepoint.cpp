#include "figurepoint.h"

#include "styler.h"

qreal FigurePoint::radius() const
{
    return _modifier ? 4 : 2;
}

bool FigurePoint::release()
{
    return true;
}

void FigurePoint::setEnd(QPointF point, bool modifier)
{
    Figure::setEnd(point, modifier);
    _modifier = modifier;
}

void FigurePoint::paint(QPainter *painter) const
{
    QBrush brush = painter->brush();
    brush.setStyle(_modifier ? Qt::NoBrush : Qt::SolidPattern);
    brush.setColor(Styler::get<QColor>("painter-fore")); // TODO Move it to painter.cpp?
    painter->setBrush(brush);

    painter->drawEllipse(_end, radius(), radius());
}

QString FigurePoint::latex() const
{
    return "\\draw " + modifier() + pair(_end) + " ellipse (" + num(radius()) + " and " + num(radius()) + ");";
}

Figure* FigurePoint::copy() const
{
    FigurePoint *figure = new FigurePoint;
    figure->setStart(start());
    figure->setEnd(end());
    return figure;
}

QPointF FigurePoint::attract(QPointF point) const
{
    return findAttract({ _end }, point);
}

QStringList FigurePoint::modifiers() const
{
    return _modifier ? QStringList { "" } : QStringList { "fill=black" };
}
