#include "figuretext.h"

void FigureText::setEnd(QPointF end, bool)
{
    Figure::setEnd(end);
    _text = pair(end);
}

void FigureText::paint(QPainter *painter) const
{
    qreal shift = painter->fontMetrics().ascent() - painter->fontMetrics().height();
    painter->drawText(QPointF { _end.x(), _end.y() + shift }, _text);
}

QString FigureText::latex() const
{
    return "\\draw " + modifier() + pair(_end) + " node [above right] {" + _text + "};";
}

Figure* FigureText::copy() const
{
    FigureText *figure = new FigureText;
    figure->setStart(start());
    figure->setEnd(end());
    figure->setText(text());
    return figure;
}

void FigureText::clear()
{
    setText("");
}

QString FigureText::text() const
{
    return _text;
}

void FigureText::setText(const QString &value)
{
    _text = value;
}
