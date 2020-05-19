#ifndef FIGURERECTANGLE_H
#define FIGURERECTANGLE_H

#include "figurefillable.h"

/**
 * @brief Rectangle figure
 */
class FigureRectangle : public FigureFillable
{
public:
    void setEnd(QPointF point, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
};

#endif // FIGURERECTANGLE_H
