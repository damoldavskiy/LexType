#ifndef FIGURELINE_H
#define FIGURELINE_H

#include "figurearrow.h"

class FigureLine : public FigureArrow
{
public:
    void setEnd(QPointF point, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
};

#endif // FIGURELINE_H
