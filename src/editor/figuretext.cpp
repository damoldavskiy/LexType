#include "figuretext.h"

bool FigureText::release()
{
    return !_text.isEmpty();
}

void FigureText::setEnd(QPointF end, bool)
{
    Figure::setEnd(end);
}

void FigureText::paint(QPainter *painter) const
{
    qreal shift = painter->fontMetrics().ascent() - painter->fontMetrics().height();
    // These constants repeat shift, that TikZ places with [above right]
    painter->drawText(QPointF { _end.x() + 2, _end.y() + shift - 6 }, _text);
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
    Figure::clear();
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
