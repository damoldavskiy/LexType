#ifndef FIGUREPATH_H
#define FIGUREPATH_H

#include "figurearrow.h"
#include "figurefillable.h"

/**
 * @brief Curve path figure (Bezie curves are used)
 */
class FigurePath : public FigureArrow, public FigureFillable
{
public:
    void setStart(QPointF point);
    void setEnd(QPointF point, bool modifier = false);
    bool release();
    void shift(qreal shiftX, qreal shiftY);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
    void clear();

    /**
     * @brief Returns vector of points which represent path
     * @return
     */
    const QVector<QPointF>& path() const;

    /**
     * @brief Sets path of figure
     * @param Path
     */
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
