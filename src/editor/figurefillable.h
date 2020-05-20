#ifndef FIGUREFILLABLE_H
#define FIGUREFILLABLE_H

#include "figurecontour.h"

/**
 * @brief Represents a figure which has internal area
 */
class FigureFillable : public virtual FigureContour
{
public:
    enum Fill
    {
        Solid,
        Empty
    };

    void paint(QPainter *painter) const;
    QStringList modifiers() const;

    /**
     * @brief Sets fill type of figure
     * @param Fill
     */
    void setFill(Fill value);

    /**
     * @brief Returns fill type of figure
     * @return Fill
     */
    Fill fill() const;

private:
    Fill _fill = Fill::Empty;
};

#endif // FIGUREFILLABLE_H
