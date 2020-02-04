#ifndef FIGUREPATH_H
#define FIGUREPATH_H

#include "figurearrow.h"
#include "figurefillable.h"

class FigurePath : public FigureArrow, public FigureFillable
{
public:
    void setStart(QPointF point);
    void setEnd(QPointF point, bool modifier = false);
    void release();
    void shift(qreal shiftX, qreal shiftY);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
    void clear();
    const QVector<QPointF>& path() const;
    void setPath(const QVector<QPointF>& value);
    QStringList modifiers() const;

private:
    QVector<QPointF> getPoints() const;
    bool isCycle(const QVector<QPointF> &points);

    QVector<QPointF> _path;
    bool _cycle = false;

    qreal _threshold = 30;
    qreal _cycleDist = 30;
};

#endif // FIGUREPATH_H
