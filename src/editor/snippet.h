#ifndef SNIPPET_H
#define SNIPPET_H

#include <QString>

#include "editor.h"

class Snippet
{
public:
    Snippet();
    Snippet(bool regular, const QString &pattern, const QString &value, bool tabular = false);
    Snippet(bool regular, const QString &pattern, const QString &value, int position, bool tabular = false);
    bool apply(Editor *editor) const;
    bool regular() const;
    void setRegular(bool value);
    QString pattern() const;
    void setPattern(const QString &pattern);
    QString value() const;
    void setValue(const QString &value);
    int position() const;
    void setPosition(int value);
    friend QDataStream &operator <<(QDataStream &stream, const Snippet &snippet);
    friend QDataStream &operator >>(QDataStream &stream, Snippet &snippet);

private:
    bool _regular;
    QString _pattern;
    QString _value;
    int _position;
};

Q_DECLARE_METATYPE(Snippet)

#endif // SNIPPET_H
