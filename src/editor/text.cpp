#include "text.h"

template <typename T>
void shift(QMap<int, T> &map, int start, int shift)
{
    QVector<QPair<int, QSet<int>>> values;
    for (auto it = map.lowerBound(start); it != map.end();) {
        values.append({ it.key() + shift, std::move(*it) });
        it = map.erase(it);
    }

    for (int i = 0; i < values.size(); ++i)
        map.insert(values[i].first, std::move(values[i].second));
}

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
    int line = _tracker.find(pos);
    int added = _tracker.find(pos + text.size()) - line;

    insertLinesAdjust(line, added);
    updateMarkup(pos);
}

void Text::insertLinesAdjust(int line, int added)
{
    if (added > 0) {
        _widths.insert(line, added, 0);
        shift(_lineBreaks, line, added);
    }

    for (int i = line; i <= line + added; ++i)
        _widths.set(i, lineWidth(i));

    updateLineBreaks(line, added + 1);

    _lastLine = -1;
    _lastWord = -1;
}

void Text::remove(int pos, int count)
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= size());

    int line = _tracker.find(pos);
    int deleted = _tracker.find(pos + count) - line;
    _tracker.remove(pos, count);

    removeLinesAdjust(line, deleted);
    updateMarkup(pos);
}

void Text::removeLinesAdjust(int line, int removed)
{
    if (removed > 0) {
        _widths.remove(line + 1, removed);

        for (auto it = _lineBreaks.lowerBound(line + 1); it != _lineBreaks.end();) {
            if (it.key() <= line + removed)
                it = _lineBreaks.erase(it);
            else
                break;
        }
        shift(_lineBreaks, line + removed, -removed);
    }

    _widths.set(line, lineWidth(line));

    updateLineBreaks(line, 1);

    _lastLine = -1;
    _lastWord = -1;
}

void Text::setWindowWidth(qreal width)
{
    if (width == _windowWidth)
        return;

    _windowWidth = width;

    updateLineBreaks(0, _tracker.size());
    for (int i = 0; i < _tracker.size(); ++i)
        _widths.set(i, lineWidth(i));
}

void Text::updateLineBreaks(int line, int count)
{
    for (int i = line; i < line + count; ++i) {
        qreal width = 0;

        auto it = _lineBreaks.find(i);
        if (it != _lineBreaks.end())
            _lineBreaks.erase(it);

        for (int j = 0; j < _tracker[i].count(); ++j) {
            width += _tracker[i][j].width;

            if (width + 1 >= _windowWidth) {
                if (j != 0 && _tracker[i][j].text.size() > 0)
                    _lineBreaks[i].insert(j);
                width = _tracker[i][j].width;
            }

            width += advanceLeading(width, _tracker[i][j]);
        }
    }
}

int Text::find(int pos, const QString &substring, bool matchCase) const
{
    // TODO: More effective
    return _tracker.mid(0).indexOf(substring, pos, matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

int Text::undo()
{
    const Action &action = _tracker.backward();

    int result;
    if (action.type == Action::Insert) {
        int line = _tracker.find(action.index);
        int changed = _tracker.find(action.index + action.text.size()) - line;
        _tracker.undo();

        removeLinesAdjust(line, changed);
        result = action.index;
    } else {
        _tracker.undo();
        int line = _tracker.find(action.index);
        int changed = _tracker.find(action.index + action.text.size()) - line;

        insertLinesAdjust(line, changed);
        result = action.index + action.text.size();
    }

    updateMarkup(action.index);
    return result;
}

int Text::redo()
{
    const Action &action = _tracker.foreward();

    int result;
    if (action.type == Action::Remove) {
        int line = _tracker.find(action.index);
        int changed = _tracker.find(action.index + action.text.size()) - line;
        _tracker.redo();

        removeLinesAdjust(line, changed);
        result = action.index;
    } else {
        _tracker.redo();
        int line = _tracker.find(action.index);
        int changed = _tracker.find(action.index + action.text.size()) - line;

        insertLinesAdjust(line, changed);
        result = action.index + action.text.size();
    }

    updateMarkup(action.index);
    return result;
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

    auto pair = findLineWord(pos);
    const Line &line = _tracker[pair.first];
    const Word &word = line[pair.second];

    pos -= line.start();
    if (pos == line.size())
        return '\n';

    pos -= word.start;
    if (pos == word.text.size())
        return word.leading;

    return word.text[pos];
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

qreal Text::lineHeight(int line) const
{
    auto set = _lineBreaks.find(line);
    return _font.height() * ((set == _lineBreaks.end() ? 0 : set->size()) + 1);
}

qreal Text::linesHeightBefore(int line) const
{
    qreal height = 0;
    for (int i = 0; i < line; ++i)
        height += lineHeight(i);
    return height;
}

qreal Text::lineWidth(int line) const
{
    qreal width = 0;
    qreal currentWidth = 0;

    for (int i = 0; i < _tracker[line].count(); ++i) {
        auto it = _lineBreaks.find(line);
        if (it != _lineBreaks.end() && it->contains(i)) {
            width = Math::max(width, currentWidth);
            currentWidth = 0;
        }

        currentWidth += _tracker[line][i].width;
        qreal leading = advanceLeading(currentWidth, _tracker[line][i]);
        if (it == _lineBreaks.end() || !it->contains(i + 1))
            currentWidth += leading;
    }

    return Math::max(width, currentWidth);
}

QPair<int, int> Text::findLineWord(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    bool newLine = false;
    if (_lastLine == -1) {
        newLine = true;
    } else {
        const Line &currentLine = _tracker[_lastLine];
        newLine = currentLine.start() > pos ||
                  currentLine.start() + currentLine.size() < pos;
    }

    if (newLine)
        _lastLine = _tracker.find(pos);
    const Line &line = _tracker[_lastLine];
    pos -= line.start();

    bool newWord = false;
    if (newLine || _lastWord == -1) {
        newWord = true;
    } else {
        const Word &word = line[_lastWord];
        newWord = word.start > pos ||
                  word.start + word.text.size() < pos;
    }

    if (newWord)
        _lastWord = line.find(pos);

    return { _lastLine, _lastWord };
}

bool Text::isBreak(int pos) const
{
    auto pair = findLineWord(pos);
    const Line &line = _tracker[pair.first];
    const Word &word = line[pair.second];

    pos -= line.start() + word.start;

    return pos == 0 && _lineBreaks[pair.first].contains(pair.second);
}

int Text::breaksInline(int pos) const
{
    int line = _tracker.find(pos);
    pos -= _tracker[line].start();
    int word = _tracker[line].find(pos);
    pos -= _tracker[line][word].start;

    auto set = _lineBreaks.find(line);
    if (set == _lineBreaks.end())
        return 0;

    int extra = 0;
    for (int index : *set)
        if (index <= word)
            ++extra;

    return extra;
}

int Text::visualLinesCount(int line) const
{
    auto set = _lineBreaks.find(line);
    return (set == _lineBreaks.end() ? 0 : set->size()) + 1;
}

int Text::size() const
{
    return _tracker[_tracker.size() - 1].start() + _tracker[_tracker.size() - 1].size();
}

int Text::lineCount() const
{
    return _tracker.size();
}

int Text::visualLinesCount() const
{
    int extra = 0;
    for (const auto &set : _lineBreaks)
        extra += set.size();
    return extra + _tracker.size();
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

qreal Text::advanceLeading(qreal left, const Word &word) const
{
    if (word.leading == '\t')
        return Line::appendTab(left, _font.tabWidth()) - left;
    else if (word.leading == ' ')
        return _font.width(' ');

    return 0;
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

int Text::findPos(qreal x, qreal y, bool exact) const
{
    if (x < 0)
        x = 0;

    int line = 0;
    qreal height = 0;
    for (; line < _tracker.size() && height + lineHeight(line) <= y; ++line)
        height += lineHeight(line);

    int word = 0;
    QMap<int, QSet<int>>::const_iterator set;
    if ((set = _lineBreaks.find(line)) != _lineBreaks.end()) {
        QVector<int> wordBreaks { 0 };
        for (int pos : *set)
            wordBreaks.append(pos);
        std::sort(wordBreaks.begin(), wordBreaks.end());

        word = wordBreaks[(y - height) / _font.height()];
    }

    if (line < _tracker.size()) {
        int pos = _tracker[line].start() + _tracker[line][word].start;
        int end = _tracker[line].start() + _tracker[line].size();

        qreal width, left;
        width = 0;
        left = 0;
        while (pos < end && left < x && !isBreak(pos + 1)) {
            width = advanceWidth(left, pos);
            left += width;
            pos++;
        }

        if (exact || left - x > width / 2)
            --pos;

        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= size());

        return pos;
    } else {
        return size();
    }
}

QPointF Text::findShift(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= size());

    int line = _tracker.find(pos);
    int word = _tracker[line].find(pos - _tracker[line].start());

    // TODO More effective
    qreal y = linesHeightBefore(line);

    int startWord = 0;
    QMap<int, QSet<int>>::const_iterator set;
    if ((set = _lineBreaks.find(line)) != _lineBreaks.end()) {
        QVector<int> wordBreaks { 0 };
        for (int pos : *set)
            if (pos <= word)
                wordBreaks.append(pos);
        std::sort(wordBreaks.begin(), wordBreaks.end());

        startWord = wordBreaks.back();
        y += (wordBreaks.size() - 1) * _font.height();
    }

    qreal x = 0;
    for (int i = startWord; i < word; ++i) {
        x += _tracker[line][i].width;
        x += advanceLeading(x, _tracker[line][i]);
    }

    for (int i = _tracker[line].start() + _tracker[line][word].start; i < pos; ++i)
        x += advanceWidth(x, i);

    return { x, y };
}

