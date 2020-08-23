#include "line.h"

Line::Line()
    : _start(0), _size(0), _words(1), _font(nullptr)
{ }

QPair<Line, Line> Line::split(int pos) const
{
    Line first, second;

    first.setFont(_font);
    second.setFont(_font);

    int word = find(pos);
    int sep = pos - _words[word].start;
    int rem = _words[word].text.size() - sep;

    first._words = _words.mid(0, word + 1);
    first._words[word].text.remove(sep, rem);
    first._words[word].leading = '\0';
    second._words = _words.mid(word);
    second._words[0].text.remove(0, sep);
    second.updateStarts(0);

    first.updateWidth(word);
    second.updateWidth(0);

    first._size = pos;
    second._size = _size - pos;

    return { first, second };
}

void Line::insert(int pos, Line line)
{
    int word = find(pos);

    QPair<QString, QString> pair = { _words[word].text.mid(0, pos - _words[word].start), _words[word].text.mid(pos - _words[word].start) };
    line._words.first().text.insert(0, pair.first);
    line._words.last().text.append(pair.second);
    line._words.last().leading = _words[word].leading;

    line.updateWidth(0);
    if (line._words.size() > 0)
        line.updateWidth(line._words.size() - 1);

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
        _words[word].text.remove(pos - _words[word].start);
        _words[lastWord].text.remove(0, pos + count - _words[lastWord].start);
        _words[word].text.append(std::move(_words[lastWord].text.toString()));
        _words[word].leading = _words[lastWord].leading;
        _words.remove(word + 1, lastWord - word);
    } else {
        _words[word].text.remove(pos - _words[word].start, count);
    }

    updateWidth(word);
    updateStarts(word + 1);
    _size -= count;
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
        else if (pos > _words[cur].start + _words[cur].text.size())
            left = cur + 1;
        else
            return cur;
    }

    Q_ASSERT(false);
}


void Line::setFont(const CachedFont *font)
{
    _font = font;
    for (int i = 0; i < _words.size(); ++i)
        updateWidth(i);
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

int Line::count() const
{
    return _words.size();
}

const Word &Line::operator [](int word) const
{
    return _words[word];
}

QString Line::mid(int pos, int count) const
{
    if (count == -1)
        count = _size - pos;

    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos + count <= _size);

    int word = find(pos);

    QString result;
    for (int i = 0; i < count; ++i) // TODO More effective
        if (pos + i - _words[word].start < _words[word].text.size()) {
            result.append(_words[word].text[pos + i - _words[word].start]);
        } else {
            result.append(_words[word].leading);
            ++word;
        }

    return result;
}

QVector<Line> Line::split(const QString &text, const CachedFont *font)
{
    QVector<Line> lines(1);
    lines.first().setFont(font);

    for (QChar symbol : text) {
        if (symbol == '\n') {
            lines.append(Line());
            lines.last().setFont(font);
        } else {
            ++lines.last()._size;
            Word &lastWord = lines.last()._words.last();
            if (symbol == ' ' || symbol == '\t') {
                lastWord.leading = symbol;
                Word word;
                word.start = lastWord.start + lastWord.text.size() + 1;
                lines.last()._words.append(word);
            } else {
                lastWord.text.append(symbol);
                lastWord.width += font->width(symbol);
            }
        }
    }

    return lines;
}

qreal Line::appendTab(qreal left, qreal tab)
{
    qreal space = tab * static_cast<int>(left / tab);
    if (space <= left)
        space += tab;
    return space;
}

void Line::updateWidth(int word)
{
    _words[word].width = 0;
    for (int i = 0; i < _words[word].text.size(); ++i)
        _words[word].width += _font->width(_words[word].text[i]);
}

void Line::updateStarts(int word)
{
    if (word == 0)
        _words[0].start = 0;

    for (int i = qMax(1, word); i < _words.size(); ++i)
        _words[i].start = _words[i - 1].start + _words[i - 1].text.size() + 1;
}
