#ifndef FIGURE_H
#define FIGURE_H

#include <QPainter>

#include "math.h"

/**
 * @brief Base class for all figures
 */
class Figure
{
public:
    virtual ~Figure();

    /**
     * @brief Called when user shifts figure
     * @param Shift by X
     * @param Shift by Y
     */
    virtual void shift(qreal shiftX, qreal shiftY);

    /**
     * @brief Called when user releases mouse button
     * @return
     */
    virtual bool release();

    /**
     * @brief Paints figure
     * @param Painter which is used to paint the figure
     */
    virtual void paint(QPainter *painter) const;

    /**
     * @brief Converts figure into LaTeX representation
     * @return LaTeX string
     */
    virtual QString latex() const = 0;

    /**
     * @brief Sets initial point of figure
     * @param New point
     */
    virtual void setStart(QPointF point);

    /**
     * @brief Returns initial point of figure
     * @return Point
     */
    QPointF start() const;

    /**
     * @brief Sets last point of figure
     * @param New point
     * @param Does user uses modifier currently
     */
    virtual void setEnd(QPointF end, bool modifier = false);

    /**
     * @brief Returns last point of figure
     * @return Point
     */
    QPointF end() const;

    /**
     * @brief Copies figure
     * @return Pointer to copy of figure
     */
    virtual Figure* copy() const = 0;

    /**
     * @brief Clears figure information. Called when user starts painting new figure
     */
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
