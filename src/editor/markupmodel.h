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
    Interval interval(int pos) const;
    void setInterval(int start, int end, Interval interval);

private:
    QVector<Interval> _intervals;
};

#endif // MARKUPMODEL_H
