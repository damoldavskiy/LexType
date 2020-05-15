#include "gapbuffer.h"

#include "math.h"

GapBuffer::~GapBuffer()
{
    delete [] _data;
}

void GapBuffer::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    ensureGap(pos, text.size());

    for (int i = 0; i < text.size(); ++i)
        _data[pos + i] = text[i];

    _gap += text.size();
    _gapSize -= text.size();
}

void GapBuffer::remove(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= size());
    Q_ASSERT(count >= 0);

    ensureGap(pos + count, 0);

    _gap -= count;
    _gapSize += count;
}

QString GapBuffer::mid(int pos, int count) const
{
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

QChar GapBuffer::operator [](int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    if (pos < _gap)
        return _data[pos];
    return _data[pos + _gapSize];
}

int GapBuffer::size() const
{
    return _allocated - _gapSize;
}

QString GapBuffer::toString() const
{
    return mid(0, size());
}

void GapBuffer::copy(QChar *target, int from, int to) const
{
    if (from < _gap)
        for (int i = from; i < Math::min(to, _gap); ++i)
            target[i] = _data[i];
    for (int i = _gap; i < to; ++i)
        target[i] = _data[i + _gapSize];
}

void GapBuffer::ensureGap(int pos, int count)
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
