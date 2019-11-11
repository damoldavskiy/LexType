#ifndef MAXVECTOR_H
#define MAXVECTOR_H

#include <QVector>
#include <QDebug>

template <typename T>
class MaxVector
{
public:
    explicit MaxVector(int n)
        : _values(n), _max(-1)
    {
        if (n > 0)
            _max = 0;
    }

    void set(int pos, T value)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos < _values.size());
        Q_ASSERT(_max >= 0);
        Q_ASSERT(_max < _values.size());

        T oldValue = _values[pos];
        _values[pos] = value;
        if (pos == _max && value < oldValue)
            update();
        else if (value > _values[_max])
            _max = pos;
        Q_ASSERT(_max < _values.size());
    }

    void insert(int pos, int count, T value)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= _values.size());
        Q_ASSERT(count >= 1);

        if (pos <= _max)
            _max += count;

        _values.insert(pos, count, value);
    }

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

    int max() const
    {
        Q_ASSERT(_max >= 0);
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
