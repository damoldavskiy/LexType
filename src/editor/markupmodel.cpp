#include "markupmodel.h"

Interval MarkupModel::interval(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < _intervals.size());

    return _intervals[pos];
}

void MarkupModel::setInterval(int start, int end, Interval interval)
{
    if (_intervals.size() < end)
        _intervals.resize(end * 2);

    for (int i = start; i < end; ++i)
        _intervals[i] = interval;
}
