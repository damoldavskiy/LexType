#ifndef FIGURE_H
#define FIGURE_H

#include <QPainter>

#include "math.h"

class Figure
{
public:
    enum Type
    {
        Line,
        Ellipse,
        Rectangle,
        Path
    };

    enum StrokeModifier
    {
        Solid,
        Dashes
    };

    Figure(QPointF start);
    virtual ~Figure();

    virtual void update(QPointF point, bool, StrokeModifier strokeModifier);
    virtual void shift(qreal shiftX, qreal shiftY);
    virtual void release();

    virtual void paint(QPainter *painter) const;
    virtual QString latex() const = 0;

protected:
    static QString num(qreal n);
    static QString pair(QPointF point);

    void applyPenModifier(QPen& pen) const;
    QString latexModifier() const;

    qreal dx() const;
    qreal dy() const;

    QPointF _start;
    QPointF _end;

    StrokeModifier _strokeModifier;
};

#endif // FIGURE_H
