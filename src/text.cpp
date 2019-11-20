#include "text.h"

Text::Text(const QFont &font)
    : _tracker(1), _widths(1), _fm(font), _tabWidth(_fm.width('x') * 8), _cachedWidths(256)
{
    for (int i = 0; i < 256; ++i)
        _cachedWidths[i] = _fm.width(static_cast<QChar>(i));
}

void Text::insert(int pos, const QString &text)
{
    _data.insert(pos, text);
    int line = _tracker.find(pos);
    int start = _tracker[line].start;
    int end = start + _tracker[line].size;

    QPair<int, int> pair = _tracker.insert(pos, text);
    int added = pair.second;

    if (added > 0)
        _widths.insert(line, added, 0);

    if (pos == end) {
        qreal width = _widths[line];
        for (int i = end; i < start + _tracker[line].size; ++i)
            width += advanceWidth(width, i);
        _widths.set(line, width);
        ++line;
        --added;
    }

    for (int i = line; i < line + added + 1; ++i)
        _widths.set(i, lineWidth(i));
}

void Text::remove(int pos, int count)
{
    _data.remove(pos, count);

    QPair<int, int> pair = _tracker.remove(pos, count);
    int line = pair.first;
    int deleted = pair.second;

    if (deleted > 0)
        _widths.remove(line + 1, deleted);

    _widths.set(line, lineWidth(line));
}

QString Text::mid(int pos, int count) const
{
    return _data.mid(pos, count);
}

QChar Text::operator [](int pos) const
{
    return _data[pos];
}

int Text::findLine(int pos) const
{
    return _tracker.find(pos);
}

int Text::lineStart(int line) const
{
    return _tracker[line].start;
}

int Text::lineSize(int line) const
{
    return _tracker[line].size;
}

int Text::size() const
{
    return _data.size();
}

int Text::lineCount() const
{
    return _tracker.size();
}

qreal Text::width() const
{
    return _widths.max();
}

qreal Text::fontHeight() const
{
    return _fm.height();
}

qreal Text::fontAscent() const
{
    return _fm.ascent();
}

qreal Text::advanceWidth(qreal left, int pos) const
{
    Q_ASSERT(left >= 0);
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < _data.size());

    QChar symbol = _data[pos];

    if (symbol == '\t') {
        qreal space = _tabWidth * static_cast<int>(left / _tabWidth);
        if (space <= left)
            space += _tabWidth;
        return space - left;
    } else {
        int code = symbol.unicode();
        if (code < 256)
            return _cachedWidths[code];
        return _fm.width(symbol);
    }
}

qreal Text::lineWidth(int line) const
{
    Q_ASSERT(line >= 0);
    Q_ASSERT(line < _tracker.size());

    int pos = _tracker[line].start;
    int end = pos + _tracker[line].size;

    qreal width = 0;
    for (; pos < end; ++pos)
        width += advanceWidth(width, pos);

    return width;
}

QString Text::text() const
{
    return _data;
}
