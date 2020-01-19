#ifndef MATH_H
#define MATH_H

#include <QtMath>
#include <QPoint>

namespace Math {

qreal sign(qreal n);
qreal abs(qreal n);
qreal dist(QPointF p1, QPointF p2);

}

#endif // MATH_H
