#include "math.h"

namespace Math {

qreal sign(qreal n)
{
    return n < 0 ? -1 : 1;
}

qreal abs(qreal n)
{
    return n < 0 ? -n : n;
}

qreal dist(QPointF p1, QPointF p2)
{
    qreal dx = p2.x() - p1.x();
    qreal dy = p2.y() - p1.y();
    return qSqrt(dx * dx + dy * dy);
}

}
