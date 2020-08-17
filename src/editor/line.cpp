#include "line.h"

Line::Line()
    : _start(0), _size(0), _words(1)
{ }

QPair<Line, Line> Line::split(int pos) const
{
    Line first, second;

    int word = find(pos);
    first._words = _words.mid(0, word + 1);
    first._words[word].size = pos - first._words[word].start;
    first._words[word].leading = '\0';
    second._words = _words.mid(word);
    second._words[0].size = second._words[0].start + second._words[0].size - pos;
    second.updateStarts(0);

    first._size = pos;
    second._size = _size - pos;

    return { first, second };
}

void Line::insert(int pos, Line line)
{
    int word = find(pos);
    line._words[0].size += _words[word].size;
    _words.insert(word, line._words.size() - 1, Word());
    for (int i = 0; i < line._words.size(); ++i)
        _words[word + i] = std::move(line._words[i]);

    updateStarts(word);
    _size += line._size;
}

void Line::append(Line line)
{
    insert(_size, std::move(line));
}

void Line::remove(int pos, int count)
{
    if (count == -1)
        count = _size - pos;

    int word = find(pos);
    int lastWord = find(pos + count);
    if (word != lastWord) {
        _words[word].size = _words[lastWord].start + _words[lastWord].size - _words[word].start - count;
        _words.remove(word + 1, lastWord - word);
    } else {
        _words[word].size -= count;
    }

    updateStarts(word + 1);
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

    for (QChar symbol : text) {
        if (symbol == '\n') {
            lines.append(Line());
        } else {
            ++lines.last()._size;
            Word &lastWord = lines.last()._words.last();
            if (symbol == ' ' || symbol == '\t') {
                lastWord.leading = symbol;
                lines.last()._words.append(Word { lastWord.start + lastWord.size + 1 });
            } else {
                ++lastWord.size;
            }
        }
    }

    return lines;
}

int Line::find(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _size);

    int left = 0;
    int right = _words.size() - 1;
    int cur;

    while (true) {
        cur = (right + left) / 2;

        if (_words[cur].start > pos)
            right = cur - 1;
        else if (pos > _words[cur].start + _words[cur].size)
            left = cur + 1;
        else
            return cur;
    }

    Q_ASSERT(false);
}

void Line::updateStarts(int word)
{
    if (word == 0)
        _words[0].start = 0;

    for (int i = qMax(1, word); i < _words.size(); ++i)
        _words[i].start = _words[i - 1].start + _words[i - 1].size + 1;
}
