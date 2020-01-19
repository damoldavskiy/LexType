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

    Figure(QPointF start);
    virtual ~Figure();

    virtual void update(QPointF point, bool);
    virtual void release();

    virtual void paint(QPainter *painter) const = 0;
    virtual QString latex() const = 0;

protected:
    static QString num(qreal n);
    static QString pair(QPointF point);

    qreal dx() const;
    qreal dy() const;

    QPointF _start;
    QPointF _end;
};

#endif // FIGURE_H
