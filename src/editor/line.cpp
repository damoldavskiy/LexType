#include "line.h"

Line::Line(const QString &text)
    : _start(0), _size(text.size())
{ }

QPair<Line, Line> Line::split(int pos) const
{
    pos -= _start;
    Line first, second;
    first._size = pos;
    second._size = _size - pos;
    return { first, second };
}

void Line::insert(int /*pos*/, Line line)
{
    _size += line._size;
}

void Line::append(Line line)
{
    insert(_size, line);
}

void Line::remove(int pos, int count)
{
    if (count == -1)
        count = _size - pos + _start;
    _size -= count;
}

void Line::setStart(int start)
{
    _start = start;
}

int Line::start() const
{
    return _start;
}

int Line::size() const
{
    return _size;
}

QVector<Line> Line::split(const QString &text)
{
    QVector<Line> lines(1);

    for (QChar c : text)
        if (c == '\n')
            lines.append(Line());
        else
            ++lines.last()._size;

    return lines;
}
