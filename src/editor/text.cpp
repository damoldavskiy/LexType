#include "text.h"

Text::Text(const QFont &font, int tabCount)
    : _font(font, tabCount), _widths(1)
{
    _tracker.setFont(&_font);
}

void Text::setFont(const QFont &font, int tabCount)
{
    _font = CachedFont(font, tabCount);
}

void Text::insert(int pos, const QString &text)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    _tracker.insert(pos, text);
    insertLinesAdjust(pos, text);
    updateMarkup(pos);
}

void Text::insertLinesAdjust(int pos, const QString &text)
{
    int line = _tracker.find(pos);
    int added = _tracker.find(pos + text.size()) - line;

    if (added > 0)
        _widths.insert(line, added, 0);

    for (int i = line; i <= line + added; ++i)
        _widths.set(i, _tracker[i].width());
}

void Text::remove(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= size());

    removeLinesAdjust(pos, count);
    _tracker.remove(pos, count);
    updateMarkup(pos);
}

void Text::removeLinesAdjust(int pos, int count)
{
    int line = _tracker.find(pos);
    int deleted = _tracker.find(pos + count) - line;

    if (deleted > 0)
        _widths.remove(line + 1, deleted);

    _widths.set(line, _tracker[line].width());
}

int Text::find(int pos, const QString &substring, bool matchCase) const
{
    // TODO: More effective
    return _tracker.mid(0).indexOf(substring, pos, matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

int Text::undo()
{
    Action action = _tracker.undo();
    updateMarkup(action.index);

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
    Action action = _tracker.redo();
    updateMarkup(action.index);

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
    return _tracker.canUndo();
}

bool Text::canRedo() const
{
    return _tracker.canRedo();
}

QString Text::mid(int pos, int count) const
{
    return _tracker.mid(pos, count);
}

QChar Text::operator [](int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < size());

    int lineIndex = _tracker.find(pos);
    const Line& line = _tracker[lineIndex];
    pos -= line.start();

    int wordIndex = line.find(pos);
    const Word& word = line[wordIndex];
    pos -= word.start;

    if (pos < word.text.size())
        return word.text[pos];
    else
        return word.leading;
}

int Text::findLine(int pos) const
{
    return _tracker.find(pos);
}

int Text::lineStart(int line) const
{
    return _tracker[line].start();
}

int Text::lineSize(int line) const
{
    return _tracker[line].size();
}

int Text::size() const
{
    // TODO More effective
    return _tracker[_tracker.size() - 1].start() + _tracker[_tracker.size() - 1].size();
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
    return _font.height();
}

qreal Text::fontAscent() const
{
    return _font.ascent();
}

qreal Text::advanceWidth(qreal left, int pos) const
{
    Q_ASSERT(left >= 0);
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos < size());

    QChar symbol = operator [](pos);

    if (symbol == '\t')
        return Line::appendTab(left, _font.tabWidth()) - left;
    else
        return _font.width(symbol);
}

QString Text::text() const
{
    return _tracker.mid(0);
}

QStaticText Text::text(int pos) const
{
    return _font.get(operator [](pos));
}

void Text::updateMarkup(int) // TODO Unused parameter
{
    Interval interval = Interval::Regular;
    bool math = false;
    bool mline = false;

    // TODO Extremely inefficient
    QString data = _tracker.mid(0);

    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == '%' || (i > 0 && _markup.interval(i - 1) == Interval::Comment && data[i] != '\n')) {
            interval = Interval::Comment;
            math = false;
        } else {
            if (data[i] == '`') {
                if (math && data[i - 1] == '`' && !mline) {
                    mline = true;
                } else if (math && data[i - 1] != '`' && mline) {
                    // TODO _data[i + 1] SEGFAULT
                } else
                    math = !math;
                if (!math)
                    mline = false;
            }

            if (data[i] == '`' || math)
                interval = Interval::Mathematics;
            else
                interval = Interval::Regular;

            if ((interval == Interval::Regular && data[i] == '\\') || (i > 0 && _markup.interval(i - 1) == Interval::Command))
                interval = Interval::Command;

            if (i > 0 && _markup.interval(i - 1) == Interval::Command) {
                if (data[i].isLetter())
                    interval = Interval::Command;
                else if (data[i] != '\\') // \\ should be colored both
                    interval = Interval::Regular;
            }

            if (interval == Interval::Regular && (data[i] == '{' || data[i] == '}' || data[i] == '$'))
                interval = Interval::Special;
        }

        _markup.setInterval(i, i + 1, interval);
    }
}

Interval Text::markup(int pos) const
{
    return _markup.interval(pos);
}
