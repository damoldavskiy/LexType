#include "linetracker.h"

LineTracker::LineTracker()
    : _lines(1), _font(nullptr)
{ }

void LineTracker::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size());

    QVector<Line> lines = Line::split(text, _font);

    int line = find(pos);
    QPair<Line, Line> pair = _lines[line].split(pos - _lines[line].start());
    lines.first().insert(0, pair.first);
    lines.last().append(pair.second);

    _lines.insert(line, lines.size() - 1, Line());
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
        _lines[line].remove(pos - _lines[line].start());
        _lines[lastLine].remove(0, pos + count - _lines[lastLine].start());
        _lines[line].append(std::move(_lines[lastLine]));
        _lines.remove(line + 1, lastLine - line);
    } else {
        _lines[line].remove(pos - _lines[line].start(), count);
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

QString LineTracker::mid(int pos, int count) const
{
    if (count == -1)
        count = _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size() - pos;

    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= _lines[_lines.size() - 1].start() + _lines[_lines.size() - 1].size());

    int line = find(pos);

    int start = pos - _lines[line].start();
    QString result = _lines[line].mid(start, qMin(count, _lines[line].size() - start));
    while (result.size() < count) {
        ++line;
        result.append('\n');
        if (result.size() < count)
            result.append(_lines[line].mid(0, qMin(count - result.size(), _lines[line].size())));
    }

    return result;
}

void LineTracker::setFont(const CachedFont *font)
{
    _font = font;
    for (int i = 0; i < _lines.size(); ++i)
        _lines[i].setFont(font);
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
