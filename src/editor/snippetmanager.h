#ifndef SNIPPETMANAGER_H
#define SNIPPETMANAGER_H

#include "snippet.h"
#include "editor.h"

class SnippetManager
{
public:
    SnippetManager();

    void reset();
    void apply(Editor *editor, bool regular = true, bool math = true);

    QVector<Snippet> &snippets();
    const QVector<Snippet> &snippets() const;

    friend QDataStream &operator <<(QDataStream &stream, const SnippetManager &manager);
    friend QDataStream &operator >>(QDataStream &stream, SnippetManager &manager);

private:
    QVector<Snippet> _snippets;
};

Q_DECLARE_METATYPE(SnippetManager)

#endif // SNIPPETMANAGER_H
