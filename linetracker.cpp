#include "linetracker.h"

#include <cassert>
#include <QDebug>

LineTracker::LineTracker(int n)
    : _lines(n)
{ }

void LineTracker::insert(int pos, int count)
{
    assert(pos >= 0);
    assert(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);
    assert(count > 0);
    int line = getLine(pos);
    _lines[line].size += count;
    for (int i = line + 1; i < _lines.size(); ++i)
        _lines[i].start += count;
}

void LineTracker::insertLine(int pos)
{
    assert(pos >= 0);
    assert(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);
    int line = getLine(pos);
    int start = _lines[line].start;
    int shift = pos - start;

    _lines.insert(line, { start, shift });
    _lines[line + 1].start += shift;
    _lines[line + 1].size -= shift;

    for (int i = line + 1; i < _lines.size(); ++i)
        ++_lines[i].start;
}

void LineTracker::remove(int pos, int count)
{
    assert(pos >= 0);
    assert(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);
    assert(count > 0);
    int line = getLine(pos);

    int deleted;
    while (count > 0)
    {
        if (_lines[line].start + _lines[line].size == pos)
        {
            _lines[line].size += _lines[line + 1].size;
            _lines.remove(line + 1);
            deleted = 1;
        }
        else
        {
            deleted = qMin(_lines[line].size - (pos - _lines[line].start), count);
            _lines[line].size -= deleted;
        }

        for (int i = line + 1; i < _lines.size(); ++i)
            _lines[i].start -= deleted;

        count -= deleted;
    }
}

int LineTracker::getLine(int pos) const
{
    assert(pos >= 0);
    assert(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);
    // TODO binary search
    for (int i = 0; i < _lines.size(); ++i)
        if (_lines[i].start <= pos && pos <= _lines[i].start + _lines[i].size)
            return i;
    assert(false);
}

int LineTracker::lineStart(int line) const
{
    assert(line >= 0);
    assert(line < _lines.size());
    return _lines[line].start;
}

int LineTracker::lineSize(int line) const
{
    assert(line >= 0);
    assert(line < _lines.size());
    return _lines[line].size;
}

int LineTracker::size() const
{
    return _lines.size();
}
