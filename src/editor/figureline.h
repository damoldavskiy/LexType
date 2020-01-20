#ifndef FIGURELINE_H
#define FIGURELINE_H

#include "figure.h"

class FigureLine : public Figure
{
public:
    FigureLine(QPointF start);

    void update(QPointF point, bool modifier, StrokeModifier strokeModifier);
    void paint(QPainter *painter) const;
    QString latex() const;
};

#endif // FIGURELINE_H
