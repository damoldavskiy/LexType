#ifndef SNIPPETMANAGER_H
#define SNIPPETMANAGER_H

#include <QVector>

#include "snippet.h"

class SnippetManager
{
public:
    SnippetManager(bool reset = false);

    void reset();

    QVector<Snippet> &snippets();
    const QVector<Snippet> &snippets() const;

    friend QDataStream &operator <<(QDataStream &stream, const SnippetManager &manager);
    friend QDataStream &operator >>(QDataStream &stream, SnippetManager &manager);

private:
    QVector<Snippet> _snippets;
};

Q_DECLARE_METATYPE(SnippetManager)

#endif // SNIPPETMANAGER_H
