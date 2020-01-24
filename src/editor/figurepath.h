#ifndef FIGUREPATH_H
#define FIGUREPATH_H

#include "figure.h"

class FigurePath : public Figure
{
public:
    FigurePath(QPointF start);

    void update(QPointF point, bool, StrokeModifier strokeModifier);
    void shift(qreal shiftX, qreal shiftY);
    void release();
    void paint(QPainter *painter) const;
    QString latex() const;

private:
    QVector<QPointF> getPoints() const;
    bool isCycle(const QVector<QPointF> &points);

    QVector<QPointF> _path;
    bool _cycle = false;

    qreal _threshold = 30;
    qreal _cycleDist = 30;
};

#endif // FIGUREPATH_H
