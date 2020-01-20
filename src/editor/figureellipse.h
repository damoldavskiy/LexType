#ifndef FIGUREELLIPSE_H
#define FIGUREELLIPSE_H

#include "figure.h"

class FigureEllipse : public Figure
{
public:
    FigureEllipse(QPointF start);

    void update(QPointF point, bool modifier, StrokeModifier strokeModifier);
    void paint(QPainter *painter) const;
    QString latex() const;
};

#endif // FIGUREELLIPSE_H
