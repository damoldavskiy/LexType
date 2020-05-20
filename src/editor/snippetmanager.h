#ifndef SNIPPETMANAGER_H
#define SNIPPETMANAGER_H

#include "snippet.h"
#include "editor.h"

/**
 * @brief Manager for snippets range
 */
class SnippetManager
{
public:
    /**
     * @brief Constructs snippet manager
     */
    SnippetManager();

    /**
     * @brief Resets collection of snippets
     */
    void reset();

    /**
     * @brief Applies all stored snippets to editor
     * @param Pointer to editor
     * @param Apply regular snippets
     * @param Apply math snippets
     */
    void apply(Editor *editor, bool regular = true, bool math = true);

    /**
     * @brief Returns collection of snippets
     * @return Snippets
     */
    QVector<Snippet> &snippets();

    /**
     * @brief Returns collection of snippets
     * @return Snippets
     */
    const QVector<Snippet> &snippets() const;

    friend QDataStream &operator <<(QDataStream &stream, const SnippetManager &manager);
    friend QDataStream &operator >>(QDataStream &stream, SnippetManager &manager);

private:
    QVector<Snippet> _snippets;
};

Q_DECLARE_METATYPE(SnippetManager)

#endif // SNIPPETMANAGER_H
