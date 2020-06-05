#ifndef FIGUREELLIPSE_H
#define FIGUREELLIPSE_H

#include "figurefillable.h"

class FigureEllipse : public FigureFillable
{
public:
    void setEnd(QPointF point, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
};

#endif // FIGUREELLIPSE_H
