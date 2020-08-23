#include "snippet.h"

// TODO Useless ctor
Snippet::Snippet()
{ }

Snippet::Snippet(bool regular, const QString &pattern, const QString &value, bool tabular)
    : Snippet(regular, pattern, value, value.size(), tabular)
{ }

Snippet::Snippet(bool regular, const QString &pattern, const QString &value, int position, bool tabular)
{
    Q_ASSERT(position >= 0);
    Q_ASSERT(position <= value.size());

    _regular = regular;
    _pattern = pattern;
    _value = value;
    _position = position;

    if (tabular)
        _pattern += '\t';
}

bool Snippet::apply(Editor *editor) const
{
    Q_ASSERT(_value.size() >= _position);

    int pos = editor->caret() - _pattern.size();
    if (pos < 0)
        return false;

    if (_regular ^ (editor->markup(editor->caret() - 1).type() != IntervalType::Mathematics))
        return false;

    for (int i = 0; i < _pattern.size(); ++i)
        if ((*editor)[pos + i] != _pattern[i])
            return false;

    editor->remove(pos, _pattern.size());
    editor->insert(pos, _value);
    editor->setCaret(pos + _position);

    return true;
}

bool Snippet::regular() const
{
    return _regular;
}

void Snippet::setRegular(bool value)
{
    _regular = value;
}

QString Snippet::pattern() const
{
    return _pattern;
}

void Snippet::setPattern(const QString &pattern)
{
    _pattern = pattern;
}

QString Snippet::value() const
{
    return _value;
}

void Snippet::setValue(const QString &value)
{
    _value = value;
}

int Snippet::position() const
{
    return _position;
}

void Snippet::setPosition(int value)
{
    Q_ASSERT(value >= 0);
    _position = value;
}

QDataStream &operator <<(QDataStream &stream, const Snippet &snippet)
{
    stream << snippet._regular;
    stream << snippet._pattern;
    stream << snippet._value;
    stream << snippet._position;
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Snippet &snippet)
{
    stream >> snippet._regular;
    stream >> snippet._pattern;
    stream >> snippet._value;
    stream >> snippet._position;
    return stream;
}

