#ifndef SNIPPET_H
#define SNIPPET_H

#include <QString>

#include "editor.h"

/**
 * @brief Snippet structure (used with Editor)
 */
class Snippet
{
public:
    /**
     * @brief Constructs empty snippet
     */
    Snippet();

    /**
     * @brief Constructs snippet
     * @param True if snippet is regular, false otherwise
     * @param Pattern
     * @param Value (replacer)
     * @param Snippet is tabular
     */
    Snippet(bool regular, const QString &pattern, const QString &value, bool tabular = false);

    /**
     * @brief Constructs snippet
     * @param True if snippet is regular, false otherwise
     * @param Pattern
     * @param Value (replacer)
     * @param End caret position (relatively to start)
     * @param Snippet is tabular
     */
    Snippet(bool regular, const QString &pattern, const QString &value, int position, bool tabular = false);

    /**
     * @brief Applyes snippet to editor
     * @param Pointer to editor
     * @return True if snippet applied, false otherwise
     */
    bool apply(Editor *editor) const;

    /**
     * @brief True if snippet is regular, false if mathematic
     * @return Flag
     */
    bool regular() const;

    /**
     * @brief Sets regularity
     * @param True if snippet is regular, false if mathematic
     */
    void setRegular(bool value);

    /**
     * @brief Snippet pattern
     * @return Pattern
     */
    QString pattern() const;

    /**
     * @brief Sets snippet pattern
     * @param Pattern
     */
    void setPattern(const QString &pattern);

    /**
     * @brief Snippet value
     * @return Value
     */
    QString value() const;

    /**
     * @brief Sets snippet value
     * @param Value
     */
    void setValue(const QString &value);

    /**
     * @brief Returns snippet end caret position
     * @return Caret position
     */
    int position() const;

    /**
     * @brief Sets snippet end caret position
     * @param Caret position
     */
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
