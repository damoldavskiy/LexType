#include "text.h"

QVector<QStaticText> cacheText(int start, int end, QFontMetricsF fm)
{
    QVector<QStaticText> values(end - start + 1);
    for (int i = 0; i < end - start; ++i) {
        values[i] = QStaticText(static_cast<QChar>(start + i));
        values[i].setTextWidth(fm.width(static_cast<QChar>(start + i)));
    }
    return values;
}

Text::Text(const QFont &font)
    : _tracker(1), _widths(1), _fm(font), _tabWidth(_fm.width('x') * 4)
{
    // TODO Cache in static context
    _cachedText.add(0x0000, cacheText(0x0000, 0x00FF, _fm)); // ASCII
    _cachedText.add(0x0400, cacheText(0x0400, 0x04FF, _fm)); // Cyrillic
}

void Text::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _data.size());

    _data.insert(pos, text);
    insertLinesAdjust(pos, text);
}

void Text::insertLinesAdjust(int pos, const QString &text)
{
    if (text == "")
        return;

    int line = _tracker.find(pos);
    int start = _tracker[line].start;
    int end = start + _tracker[line].size;

    QPair<int, int> pair = _tracker.insert(pos, text);
    int added = pair.second;

    if (added > 0)
        _widths.insert(line, added, 0);

    if (pos == end && text[0] != '\n') {
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
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= _data.size());

    _data.remove(pos, count);
    removeLinesAdjust(pos, count);
}

void Text::removeLinesAdjust(int pos, int count)
{
    QPair<int, int> pair = _tracker.remove(pos, count);
    int line = pair.first;
    int deleted = pair.second;

    if (deleted > 0)
        _widths.remove(line + 1, deleted);

    _widths.set(line, lineWidth(line));
}

int Text::undo()
{
    Action action = _data.undo();

    if (action.type == Action::Insert) {
        removeLinesAdjust(action.index, action.text.size());
        return action.index;
    } else {
        insertLinesAdjust(action.index, action.text);
        return action.index + action.text.size();
    }
}

int Text::redo()
{
    Action action = _data.redo();

    if (action.type == Action::Insert) {
        insertLinesAdjust(action.index, action.text);
        return action.index + action.text.size();
    } else {
        removeLinesAdjust(action.index, action.text.size());
        return action.index;
    }
}

bool Text::canUndo() const
{
    return _data.canUndo();
}

bool Text::canRedo() const
{
    return _data.canRedo();
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
        int index = _cachedText.find(code);
        if (index == -1)
            return _fm.width(symbol);
        return _cachedText.get(index, code).textWidth();
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
    return _data.text();
}

QStaticText Text::text(int pos) const
{
    QChar symbol = _data[pos];
    int code = symbol.unicode();

    int index = _cachedText.find(code);
    if (index == -1) {
        QStaticText text(symbol);
        text.setTextWidth(_fm.width(symbol));
        return text;
    }

    return _cachedText.get(index, code);
}
