#ifndef FIGUREPOINT_H
#define FIGUREPOINT_H

#include "figure.h"

class FigurePoint : public Figure
{
public:
    qreal radius() const;
    bool release();
    void setEnd(QPointF point, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
    QPointF attract(QPointF point) const;
    QStringList modifiers() const;

private:
    bool _modifier = false;
};

#endif // FIGUREPOINT_H
