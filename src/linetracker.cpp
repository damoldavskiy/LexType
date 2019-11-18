#include "linetracker.h"

LineTracker::LineTracker(int n)
    : _lines(n)
{ }

QPair<int, int> LineTracker::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);

    int line = find(pos);
    int startLine = line;
    int count = 0;
    for (int i = 0; i < text.size(); ++i)
        if (text[i] == '\n') {
            if (count > 0) {
                insertText(line, count);
                pos += count;
                count = 0;
            }
            insertLine(line, pos++ - _lines[line].start);
            ++line;
        } else {
            ++count;
        }

    if (count > 0)
        insertText(line, count);

    return { startLine, line - startLine };
}

void LineTracker::insertText(int line, int count)
{
    Q_ASSERT(line >= 0);
    Q_ASSERT(line < _lines.size());
    Q_ASSERT(count > 0);

    _lines[line].size += count;
    for (int i = line + 1; i < _lines.size(); ++i)
        _lines[i].start += count;
}

void LineTracker::insertLine(int line, int shift)
{
    Q_ASSERT(line >= 0);
    Q_ASSERT(line < _lines.size());
    Q_ASSERT(shift >= 0);

    int start = _lines[line].start;

    _lines.insert(line, { start, shift });
    _lines[line + 1].start += shift;
    _lines[line + 1].size -= shift;

    for (int i = line + 1; i < _lines.size(); ++i)
        ++_lines[i].start;
}

QPair<int, int> LineTracker::remove(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);
    Q_ASSERT(count > 0);

    int line = find(pos);

    int deleted;
    int deletedLines = 0;
    while (count > 0) {
        if (_lines[line].start + _lines[line].size == pos) {
            _lines[line].size += _lines[line + 1].size;
            _lines.remove(line + 1);
            ++deletedLines;
            deleted = 1;
        } else {
            deleted = qMin(_lines[line].size - (pos - _lines[line].start), count);
            _lines[line].size -= deleted;
        }

        for (int i = line + 1; i < _lines.size(); ++i)
            _lines[i].start -= deleted;

        count -= deleted;
    }

    return { line, deletedLines };
}

int LineTracker::find(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start + _lines[_lines.size() - 1].size);

    int left = 0;
    int right = _lines.size() - 1;
    int cur;

    while (true) {
        cur = (right + left) / 2;

        if (_lines[cur].start > pos)
            right = cur - 1;
        else if (pos > _lines[cur].start + _lines[cur].size)
            left = cur + 1;
        else
            return cur;
    }

    Q_ASSERT(false);
}

const Line &LineTracker::operator [](int line) const
{
    Q_ASSERT(line >= 0);
    Q_ASSERT(line < _lines.size());

    return _lines[line];
}

int LineTracker::size() const
{
    return _lines.size();
}
