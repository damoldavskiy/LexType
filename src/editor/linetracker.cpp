#include "linetracker.h"

LineTracker::LineTracker(int n)
    : _lines(n)
{ }

void LineTracker::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size());

    QVector<Line> lines = Line::split(text);

    int line = find(pos);
    QPair<Line, Line> pair = _lines[line].split(pos);
    lines.first().insert(0, pair.first);
    lines.last().append(pair.second);

    _lines.insert(line + 1, lines.size() - 1, Line());
    for (int i = 0; i < lines.size(); ++i)
        _lines[line + i] = std::move(lines[i]);

    updateStarts(line);
}

void LineTracker::remove(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size());
    Q_ASSERT(count > 0);

    int line = find(pos);
    int lastLine = find(pos + count);
    if (line != lastLine) {
        _lines[line].remove(pos);
        _lines[lastLine].remove(0, pos + count - _lines[lastLine].start());
        _lines[line].append(std::move(_lines[lastLine]));
        _lines.remove(line + 1, lastLine - line);
    } else {
        _lines[line].remove(pos, count);
    }

    updateStarts(line + 1);
}

int LineTracker::find(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size());

    int left = 0;
    int right = _lines.size() - 1;
    int cur;

    while (true) {
        cur = (right + left) / 2;

        if (_lines[cur].start() > pos)
            right = cur - 1;
        else if (pos > _lines[cur].start() + _lines[cur].size())
            left = cur + 1;
        else
            return cur;
    }

    Q_ASSERT(false);
}

const Line &LineTracker::operator [](int line) const
{
    return _lines[line];
}

int LineTracker::size() const
{
    return _lines.size();
}

void LineTracker::updateStarts(int line)
{
    if (line == 0)
        _lines[0].setStart(0);

    for (int i = qMax(1, line); i < _lines.size(); ++i)
        _lines[i].setStart(_lines[i - 1].start() + _lines[i - 1].size() + 1);
}
