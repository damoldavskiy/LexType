#ifndef MULTIRANGE_H
#define MULTIRANGE_H

#include <QVector>

#include "range.h"

template <typename T>
class MultiRange
{
public:
    void add(int shift, QVector<T> &&values)
    {
        _shifts.append(shift);
        _values.append(std::move(values));
    }

    int find(int pos) const
    {
        for (int i = 0; i < _shifts.size(); ++i)
            if (_shifts[i] <= pos && pos < _shifts[i] + _values[i].size())
                return i;
        return -1;
    }

    const T& get(int index, int pos) const
    {
        Q_ASSERT(index >= 0 && index < _values.size());
        return _values[index][pos - _shifts[index]];
    }

    void clear()
    {
        _shifts.clear();
        _values.clear();
    }

private:
    QVector<int> _shifts;
    QVector<QVector<T>> _values;
};

#endif // MULTIRANGE_H
