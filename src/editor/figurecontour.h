#ifndef FIGURECONTOUR_H
#define FIGURECONTOUR_H

#include "figure.h"

/**
 * @brief Represents a figure which has a contour
 */
class FigureContour : public virtual Figure
{
public:
    /**
     * @brief Enum type
     */
    enum Stroke
    {
        Solid,
        Dash,
    };

    void paint(QPainter *painter) const;
    QStringList modifiers() const;

    /**
     * @brief Sets stroke of figure
     * @param Stroke
     */
    void setStroke(Stroke value);

    /**
     * @brief Returns figure stroke
     * @return Stroke
     */
    Stroke stroke() const;

private:
    Stroke _stroke = Stroke::Solid;
};

#endif // FIGURECONTOUR_H
