#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "editor.h"
#include "snippet.h"
#include "snippetmanager.h"

class Algorithms
{
public:
    static bool apply(Editor *editor, const Snippet &snippet);
    static void apply(Editor *editor, const SnippetManager &manager, bool regular = true, bool math = true);
};

#endif // ALGORITHMS_H
