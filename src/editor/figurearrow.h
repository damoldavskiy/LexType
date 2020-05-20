#ifndef FIGUREARROW_H
#define FIGUREARROW_H

#include "figurecontour.h"

/**
 * @brief Represents figure which can be single or double directional arrow
 */
class FigureArrow : public virtual FigureContour
{
public:
    /**
     * @brief Tip type
     */
    enum Tip
    {
        Soft,
        Arrow,
        Double
    };

    /**
     * @brief Draws the arrow
     * @param Painter
     * @param Center of arrow
     * @param Angle from the horizontal axis
     */
    void drawArrow(QPainter *painter, QPointF point, qreal angle) const;

    /**
     * @brief Sets figure tip
     * @param Tip
     */
    void setTip(Tip value);

    /**
     * @brief Returns figure tip
     * @return Tip
     */
    Tip tip() const;

    QStringList modifiers() const;

private:
    Tip _tip = Tip::Soft;
};

#endif // FIGUREARROW_H
