#ifndef MATH_H
#define MATH_H

#include <QtMath>
#include <QPoint>

namespace Math {

template <typename T>
/**
 * @brief Minimum of two elements
 * @param First element
 * @param Second element
 * @return Minumum
 */
T min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
/**
 * @brief Maximum of two elements
 * @param First element
 * @param Second element
 * @return Maximum
 */
T max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T>
/**
 * @brief Returns sign of element (1 or -1)
 * @param Element
 * @return Sign
 */
int sign(T n)
{
    return n < 0 ? -1 : 1;
}

template <typename T>
/**
 * @brief Returns absolute value of element
 * @param Element
 * @return Absolute
 */
T abs(T n)
{
    return n < 0 ? -n : n;
}

/**
 * @brief Calculates distance from one point to another
 * @param First point
 * @param Second point
 * @return Distance
 */
qreal dist(QPointF p1, QPointF p2);

/**
 * @brief Shifts point
 * @param Point to shift
 * @param Shift by X
 * @param Shift by Y
 */
void shift(QPointF &p, qreal shiftX, qreal shiftY);

}

#endif // MATH_H
