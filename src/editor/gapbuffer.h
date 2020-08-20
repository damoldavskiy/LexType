#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include "math.h"

template <typename T>
class GapBuffer
{
public:
    GapBuffer(int count = 0)
        : _data(new T[8])
    {
        insert(0, count, T());
    }

    GapBuffer(const GapBuffer &other)
    {
        *this = other;
    }

    GapBuffer(GapBuffer &&other)
    {
        *this = std::move(other);
    }

    GapBuffer& operator =(const GapBuffer &other)
    {
        if (this == &other)
            return *this;

        _allocated = other._allocated;
        _gap = other._gap;
        _gapSize = other._gapSize;

        _data = new T[_allocated];
        for (int i = 0; i < _allocated; ++i)
            _data[i] = other._data[i];

        return *this;
    }

    GapBuffer& operator =(GapBuffer &&other)
    {
        if (this == &other)
            return *this;

        _data = other._data;
        _allocated = other._allocated;
        _gap = other._gap;
        _gapSize = other._gapSize;

        other._data = new T[8];
        other._allocated = 8;
        other._gap = 0;
        other._gapSize = 8;

        return *this;
    }

    ~GapBuffer()
    {
        delete [] _data;
    }

    void insert(int pos, T element)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= size());

        ensureGap(pos, 1);
        _data[pos] = std::move(element);

        ++_gap;
        --_gapSize;
    }

    void insert(int pos, int count, T element)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= size());

        ensureGap(pos, count);

        for (int i = 0; i < count; ++i)
            _data[pos + i] = element;

        _gap += count;
        _gapSize -= count;
    }

    void append(T element)
    {
        insert(size(), std::move(element));
    }

    void remove(int pos, int count = -1)
    {
        if (count == -1)
            count = size() - pos;

        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos + count <= size());
        Q_ASSERT(count >= 0);

        ensureGap(pos + count, 0);

        _gap -= count;
        _gapSize += count;
    }

    GapBuffer<T> mid(int pos, int count = -1) const
    {
        if (count == -1)
            count = size() - pos;

        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos + count <= size());
        Q_ASSERT(count >= 0);

        if (pos >= _gap) {
            GapBuffer<T> copy;
            copy._data = new T[count];
            for (int i = 0; i < count; ++i)
                copy._data[i] = _data[_gapSize + pos + i];
            copy._allocated = count;
            copy._gapSize = 0;
            return std::move(copy);
        }

        GapBuffer copy;
        copy._data = new T[count];
        int before = Math::min(count, _gap - pos);
        for (int i = 0; i < before; ++i)
            copy._data[i] = _data[pos + i];
        for (int i = 0; i < count - (_gap - pos); ++i)
            copy._data[before + i] = _data[_gap + _gapSize + i];
        copy._allocated = count;
        copy._gapSize = 0;
        return std::move(copy);
    }

    const T& operator [](int pos) const
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= size());

        if (pos < _gap)
            return _data[pos];
        return _data[pos + _gapSize];
    }

    T& operator [](int pos)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= size());

        if (pos < _gap)
            return _data[pos];
        return _data[pos + _gapSize];
    }


    T& first()
    {
        return operator [](0);
    }

    T& last()
    {
        return operator [](size() - 1);
    }

    int size() const
    {
        return _allocated - _gapSize;
    }

private:
    void copy(T *target, int from, int to) const
    {
        if (from < _gap)
            for (int i = from; i < Math::min(to, _gap); ++i)
                target[i] = _data[i];
        for (int i = _gap; i < to; ++i)
            target[i] = _data[i + _gapSize];
    }

    void ensureGap(int pos, int count)
    {
        Q_ASSERT(pos >= 0);
        Q_ASSERT(count >= 0);

        if (_gapSize < count || pos + count > _allocated) {
            int allocated = Math::max(_allocated * 2, size() + count);
            int gapSize = allocated - size();
            T *data = new T[allocated];

            copy(data, 0, pos);
            copy(data + gapSize, pos, size());

            delete [] _data;
            _data = data;
            _gap = pos;
            _gapSize = gapSize;
            _allocated = allocated;
        }

        int direction = Math::sign(pos - _gap);

        for (; _gap != pos; _gap += direction)
            if (direction > 0)
                _data[_gap] = _data[_gap + _gapSize];
            else
                _data[_gap + _gapSize - 1] = _data[_gap - 1];
    }

    T *_data;
    int _allocated = 8;
    int _gap = 0;
    int _gapSize = 8;
};

#endif // GAPBUFFER_H
