#ifndef MATH_H
#define MATH_H

#include <QtMath>
#include <QPoint>

namespace Math {

template <typename T>
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
int sign(T n)
{
    return n < 0 ? -1 : 1;
}

template <typename T>
T abs(T n)
{
    return n < 0 ? -n : n;
}

template <typename T>
void limit(T& value, T min, T max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

qreal dist(QPointF p1, QPointF p2);

void shift(QPointF &p, qreal shiftX, qreal shiftY);

}

#endif // MATH_H
