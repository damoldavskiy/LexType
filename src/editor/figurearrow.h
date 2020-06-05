#ifndef FIGUREARROW_H
#define FIGUREARROW_H

#include "figurecontour.h"

class FigureArrow : public virtual FigureContour
{
public:
    enum Tip
    {
        Soft,
        Arrow,
        Double
    };

    QStringList modifiers() const;

    void drawArrow(QPainter *painter, QPointF point, qreal angle) const;
    void setTip(Tip value);
    Tip tip() const;

private:
    Tip _tip = Tip::Soft;
};

#endif // FIGUREARROW_H
