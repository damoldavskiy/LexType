#ifndef MAXVECTOR_H
#define MAXVECTOR_H

#include <QVector>

template <typename T>
/**
 * @brief Vector that storages max element in it
 */
class MaxVector
{
public:
    /**
     * @brief Constructs MaxVector
     * @param Initial number of elements
     */
    explicit MaxVector(int n)
        : _values(n), _max(0)
    {
        Q_ASSERT(n > 0);
    }

    /**
     * @brief Sets element by index
     * @param Position of element
     * @param New element
     */
    void set(int pos, T value)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos < _values.size());

        T oldValue = _values[pos];
        _values[pos] = value;
        if (pos == _max && value < oldValue)
            update();
        else if (value > _values[_max])
            _max = pos;

        Q_ASSERT(_max < _values.size());
    }

    /**
     * @brief Inserts specified count of elements (by copying)
     * @param Position to insert
     * @param Count of elements inserted
     * @param Element to insert
     */
    void insert(int pos, int count, T value)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= _values.size());
        Q_ASSERT(count > 0);

        if (pos <= _max)
            _max += count;

        _values.insert(pos, count, value);
    }

    /**
     * @brief Removes specified count of elements
     * @param Start position
     * @param Count of elements
     */
    void remove(int pos, int count)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos < _values.size());
        Q_ASSERT(count > 0);
        Q_ASSERT(pos + count <= _values.size());

        _values.remove(pos, count);
        if (pos <= _max && _max < pos + count)
            update();
        else if (_max >= pos + count)
            _max -= count;
    }

    /**
     * @brief Returns element at index
     * @param Position
     * @return Element
     */
    T operator [](int pos) const
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos < _values.size());

        return _values[pos];
    }

    /**
     * @brief Returns maximum element
     * @return Maximum
     */
    T max() const
    {
        return _values[_max];
    }

protected:
    void update()
    {
        _max = 0;
        for (int i = 1; i < _values.size(); ++i)
            if (_values[i] > _values[_max])
                _max = i;
    }

private:
    QVector<T> _values;
    int _max;
};

#endif // MAXVECTOR_H
