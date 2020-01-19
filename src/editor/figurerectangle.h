#ifndef FIGURERECTANGLE_H
#define FIGURERECTANGLE_H

#include "figure.h"

class FigureRectangle : public Figure
{
public:
    FigureRectangle(QPointF start)
        : Figure(start)
    { }

    virtual void update(QPointF point, bool modifier)
    {
        _end = point;

        if (modifier) {
            if (abs(dx()) > abs(dy()))
                _end.setX(_start.x() + dy() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
            else
                _end.setY(_start.y() + dx() * (Math::sign(dx()) == Math::sign(dy()) ? 1 : -1));
        }
    }

    void release()
    { }

    void paint(QPainter *painter) const
    {
        painter->drawRect({ _start, _end });
    }

    QString latex() const
    {
        return "\\draw " + pair(_start) + " rectangle " + pair(_end) + ";";
    }
};

#endif // FIGURERECTANGLE_H
