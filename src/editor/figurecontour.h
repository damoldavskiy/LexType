#ifndef FIGURECONTOUR_H
#define FIGURECONTOUR_H

#include "figure.h"

/**
 * @brief Represents a figure which has a contour
 */
class FigureContour : public virtual Figure
{
public:
    enum Stroke
    {
        Solid,
        Dash,
    };

    void paint(QPainter *painter) const;
    QStringList modifiers() const;

    void setStroke(Stroke value);
    Stroke stroke() const;

private:
    Stroke _stroke = Stroke::Solid;
};

#endif // FIGURECONTOUR_H
