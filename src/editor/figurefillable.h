#ifndef FIGUREFILLABLE_H
#define FIGUREFILLABLE_H

#include "figurecontour.h"

class FigureFillable : public virtual FigureContour
{
public:
    enum Fill
    {
        Solid,
        LinesHorizontal,
        LinesVertical,
        Empty
    };

    void paint(QPainter *painter) const;
    QStringList modifiers() const;

    void setFill(Fill value);
    Fill fill() const;

private:
    Fill _fill = Fill::Empty;
};

#endif // FIGUREFILLABLE_H
