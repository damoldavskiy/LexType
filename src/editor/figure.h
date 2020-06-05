#ifndef FIGURE_H
#define FIGURE_H

#include <QPainter>

#include "math.h"

class Figure
{
public:
    virtual ~Figure();
    virtual void shift(qreal shiftX, qreal shiftY);
    virtual bool release();
    virtual void paint(QPainter *painter) const;
    virtual QString latex() const = 0;
    virtual void setStart(QPointF point);
    QPointF start() const;
    virtual void setEnd(QPointF end, bool modifier = false);
    QPointF end() const;
    virtual Figure* copy() const = 0;
    virtual void clear();

protected:
    static QString num(qreal n);
    static QString pair(QPointF point);

    virtual QStringList modifiers() const;
    QString modifier() const;

    qreal dx() const;
    qreal dy() const;

    QPointF _start;
    QPointF _end;
};

#endif // FIGURE_H
