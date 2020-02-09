#ifndef SNIPPET_H
#define SNIPPET_H

#include <QString>

#include "editor.h"

class Snippet
{
public:
    Snippet();
    Snippet(const QString &pattern, const QString &value, bool tabular = false);
    Snippet(const QString &pattern, const QString &value, int position, bool tabular = false);
    bool apply(Editor *editor) const;

private:
    QString _pattern;
    QString _value;
    int _position;
};

#endif // SNIPPET_H
