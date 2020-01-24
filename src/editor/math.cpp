#include "math.h"

namespace Math {

qreal dist(QPointF p1, QPointF p2)
{
    qreal dx = p2.x() - p1.x();
    qreal dy = p2.y() - p1.y();
    return qSqrt(dx * dx + dy * dy);
}

void shift(QPointF &p, qreal shiftX, qreal shiftY)
{
    p.setX(p.x() + shiftX);
    p.setY(p.y() + shiftY);
}

}
