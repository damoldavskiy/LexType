#include "algorithms.h"

bool Algorithms::apply(Editor *editor, const Snippet &snippet)
{
    // TODO Do not copy?
    bool regular = snippet.regular();
    QString value = snippet.value();
    QString pattern = snippet.pattern();
    int position = snippet.position();

    Q_ASSERT(value.size() >= position);

    int pos = editor->caret() - pattern.size();
    if (pos < 0)
        return false;

    if (regular ^ (editor->markup(editor->caret() - 1).type() != IntervalType::Mathematics))
        return false;

    for (int i = 0; i < pattern.size(); ++i)
        if ((*editor)[pos + i] != pattern[i])
            return false;

    editor->remove(pos, pattern.size());
    editor->insert(pos, value);
    editor->setCaret(pos + position);

    return true;
}

void Algorithms::apply(Editor *editor, const SnippetManager &manager, bool regular, bool math)
{
    for (const auto &snippet : manager.snippets()) {
        if ((snippet.regular() && !regular) || (!snippet.regular() && !math))
            continue;
        if (apply(editor, snippet))
            break;
    }
}
