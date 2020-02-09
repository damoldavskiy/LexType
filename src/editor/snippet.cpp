#include "snippet.h"

// TODO Useless ctor
Snippet::Snippet()
{ }

Snippet::Snippet(const QString &pattern, const QString &value, bool tabular)
    : Snippet(pattern, value, value.size(), tabular)
{ }

Snippet::Snippet(const QString &pattern, const QString &value, int position, bool tabular)
{
    Q_ASSERT(position >= 0);
    Q_ASSERT(position <= value.size());

    _pattern = pattern;
    _value = value;
    _position = position;

    if (tabular)
        _pattern += '\t';
}

bool Snippet::apply(Editor *editor) const
{
    int pos = editor->caret() - _pattern.size();

    if (pos < 0)
        return false;

    // TODO Not effective (copying of entire text)
    for (int i = 0; i < _pattern.size(); ++i)
        if (editor->text()[pos + i] != _pattern[i])
            return false;

    editor->remove(pos, _pattern.size());
    editor->insert(pos, _value);
    editor->setCaret(pos + _position);

    return true;
}
