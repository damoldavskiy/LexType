#include "markupmodel.h"

Interval MarkupModel::interval(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < _intervals.size());

    return _intervals[pos];
}

int MarkupModel::size() const
{
    return _intervals.size();
}

void MarkupModel::set(int start, int end, Interval interval)
{
    for (int i = start; i < end; ++i)
        _intervals[i] = interval;
}

void MarkupModel::insert(int start, int end, Interval interval)
{
    _intervals.insert(start, end - start, interval);
}

void MarkupModel::remove(int start, int end)
{
    _intervals.remove(start, end - start);
}
