#ifndef SNIPPETMANAGER_H
#define SNIPPETMANAGER_H

#include "snippet.h"
#include "editor.h"

class SnippetManager
{
public:
    SnippetManager();
    void apply(Editor *editor, bool regular = true, bool math = true);

private:
    QVector<Snippet> _regular;
    QVector<Snippet> _math;
};

#endif // SNIPPETMANAGER_H
