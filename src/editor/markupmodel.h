#ifndef MARKUPMODEL_H
#define MARKUPMODEL_H

#include <QColor>
#include <QVector>

#include "interval.h"

/**
 * @brief Storage for markup model in editor
 */
class MarkupModel
{
public:
    /**
     * @brief Returns markup by symbol position
     * @param Symbol position
     * @return Symbol markup
     */
    Interval interval(int pos) const;

    /**
     * @brief Sets markup of symbol range
     * @param Start position
     * @param End position
     * @param Markup type
     */
    void setInterval(int start, int end, Interval interval);

private:
    QVector<Interval> _intervals;
};

#endif // MARKUPMODEL_H
