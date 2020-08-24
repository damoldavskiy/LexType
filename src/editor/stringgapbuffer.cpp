#include "stringgapbuffer.h"

#include "math.h"

StringGapBuffer::StringGapBuffer()
{ }

StringGapBuffer::StringGapBuffer(const StringGapBuffer &other)
{
    this->operator =(other);
}

StringGapBuffer::StringGapBuffer(StringGapBuffer &&other)
{
    this->operator =(std::move(other));
}

StringGapBuffer& StringGapBuffer::operator =(const StringGapBuffer &other)
{
    if (this == &other)
        return *this;

    delete [] _data;

    _allocated = other._allocated;
    _gap = other._gap;
    _gapSize = other._gapSize;

    _data = new QChar[_allocated];
    for (int i = 0; i < _allocated; ++i)
        _data[i] = other._data[i];

    return *this;
}

StringGapBuffer& StringGapBuffer::operator =(StringGapBuffer &&other)
{
    if (this == &other)
        return *this;

    delete [] _data;

    _data = other._data;
    _allocated = other._allocated;
    _gap = other._gap;
    _gapSize = other._gapSize;

    other._data = new QChar[8];
    other._allocated = 8;
    other._gap = 0;
    other._gapSize = 8;

    return *this;
}

StringGapBuffer::~StringGapBuffer()
{
    delete [] _data;
}

void StringGapBuffer::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    ensureGap(pos, text.size());

    for (int i = 0; i < text.size(); ++i)
        _data[pos + i] = text[i];

    _gap += text.size();
    _gapSize -= text.size();
}

void StringGapBuffer::append(const QString &text)
{
    insert(size(), text);
}

void StringGapBuffer::remove(int pos, int count)
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

int StringGapBuffer::find(int pos, const QString &substring, bool matchCase) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    return toString().indexOf(substring, pos, matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

QString StringGapBuffer::mid(int pos, int count) const
{
    if (count == -1)
        count = size() - pos;

    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= size());
    Q_ASSERT(count >= 0);

    if (pos >= _gap)
        return QString(_data + _gapSize + pos, count);

    QString result(_data + pos, Math::min(count, _gap - pos));
    if (count > _gap - pos)
        result += QString(_data + _gap + _gapSize, count - (_gap - pos));

    return result;
}

QChar StringGapBuffer::operator [](int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    if (pos < _gap)
        return _data[pos];
    return _data[pos + _gapSize];
}

int StringGapBuffer::size() const
{
    return _allocated - _gapSize;
}

QString StringGapBuffer::toString() const
{
    return mid(0, size());
}

void StringGapBuffer::copy(QChar *target, int from, int to) const
{
    if (from < _gap)
        for (int i = from; i < Math::min(to, _gap); ++i)
            target[i] = _data[i];
    for (int i = _gap; i < to; ++i)
        target[i] = _data[i + _gapSize];
}

void StringGapBuffer::ensureGap(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(count >= 0);

    if (_gapSize < count || pos + count > _allocated) {
        int allocated = Math::max(_allocated * 2, size() + count);
        int gapSize = allocated - size();
        QChar *data = new QChar[allocated];

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
