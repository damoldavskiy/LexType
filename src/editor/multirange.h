#ifndef MULTIRANGE_H
#define MULTIRANGE_H

#include <QVector>

#include "range.h"

template <typename T>
/**
 * @brief Class is used for storaging cache (fast indexation over ranges)
 */
class MultiRange
{
public:
    /**
     * @brief Adds range at shift
     * @param shift
     * @param values
     */
    void add(int shift, QVector<T> &&values)
    {
        _shifts.append(shift);
        _values.append(std::move(values));
    }

    /**
     * @brief Returns the index of range containing element at specified position
     * @param Position
     * @return Index of range. -1 if not found
     */
    int find(int pos) const
    {
        for (int i = 0; i < _shifts.size(); ++i)
            if (_shifts[i] <= pos && pos < _shifts[i] + _values[i].size())
                return i;
        return -1;
    }

    /**
     * @brief Returns element of range at position
     * @param Index of range
     * @param Position of element
     * @return Element
     */
    const T& get(int index, int pos) const
    {
        Q_ASSERT(index >= 0 && index < _values.size());
        return _values[index][pos - _shifts[index]];
    }

    /**
     * @brief Cleares all data
     */
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
