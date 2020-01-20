#ifndef FIGURERECTANGLE_H
#define FIGURERECTANGLE_H

#include "figure.h"

class FigureRectangle : public Figure
{
public:
    FigureRectangle(QPointF start);

    void update(QPointF point, bool modifier, StrokeModifier strokeModifier);
    void release();
    void paint(QPainter *painter) const;
    QString latex() const;
};

#endif // FIGURERECTANGLE_H
