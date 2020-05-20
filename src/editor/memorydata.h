#ifndef MEMORYDATA_H
#define MEMORYDATA_H

#include <QStack>

#include "action.h"
#include "gapbuffer.h"

/**
 * @brief Abstraction over internal data that saves changes and has undo/redo operations
 */
class MemoryData
{
public:
    /**
     * @brief Inserts text into position
     * @param Position
     * @param Text
     */
    void insert(int pos, const QString &text);

    /**
     * @brief Removes elements
     * @param Start position
     * @param Count of deleted elements
     */
    void remove(int pos, int count);

    /**
     * @brief Searches the substring
     * @param Initial index of search
     * @param Substring to find
     * @param Match case
     * @return Result position, -1 if not found
     */
    int find(int pos, const QString &substring, bool matchCase) const;

    /**
     * @brief Undo last action
     * @return Last action
     */
    Action undo();

    /**
     * @brief Redo last action
     * @return Last action
     */
    Action redo();

    /**
     * @brief Size of text
     * @return Size
     */
    int size() const;

    /**
     * @brief Converts text to string
     * @return String
     */
    QString text() const;

    /**
     * @brief Returns substring of text
     * @param Start position
     * @param Count of symbols
     * @return Substring
     */
    QString mid(int pos, int count) const;

    /**
     * @brief Returns symbol at position
     * @param Position
     * @return Symbol
     */
    QChar operator [](int pos) const;

    /**
     * @brief Checks possibility of undo action
     * @return True if possible
     */
    bool canUndo() const;

    /**
     * @brief Checks possibility of redo action
     * @return True if possible
     */
    bool canRedo() const;

private:
    GapBuffer _data;
    QStack<Action> _before;
    QStack<Action> _after;
};

#endif // MEMORYDATA_H
