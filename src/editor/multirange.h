#ifndef MULTIRANGE_H
#define MULTIRANGE_H

#include <QVector>

#include "range.h"

template <typename T>
class MultiRange
{
public:
    void add(int shift, QVector<T> values)
    {
        shifts.append(shift);
        this->values.append(std::move(values));
    }

    int find(int pos) const
    {
        for (int i = 0; i < shifts.size(); ++i)
            if (shifts[i] <= pos && pos < shifts[i] + values[i].size())
                return i;
        return -1;
    }

    const T& get(int index, int pos) const
    {
        Q_ASSERT(index >= 0 && index < values.size());
        return values[index][pos - shifts[index]];
    }

    void clear()
    {
        shifts.clear();
        values.clear();
    }

private:
    QVector<int> shifts;
    QVector<QVector<T>> values;
};

#endif // MULTIRANGE_H
