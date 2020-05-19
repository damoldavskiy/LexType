#ifndef FIGUREARROW_H
#define FIGUREARROW_H

#include "figurecontour.h"

/**
 * @brief Represents figure which can be single or double directional arrow
 */
class FigureArrow : public virtual FigureContour
{
public:
    enum Tip
    {
        Soft,
        Arrow,
        Double
    };

    void drawArrow(QPainter *painter, QPointF point, qreal angle) const;

    void setTip(Tip value);
    Tip tip() const;

    QStringList modifiers() const;

private:
    Tip _tip = Tip::Soft;
};

#endif // FIGUREARROW_H
