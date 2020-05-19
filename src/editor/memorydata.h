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
    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    int find(int pos, const QString &substring, bool matchCase) const;
    Action undo();
    Action redo();
    int size() const;
    QString text() const;
    QString mid(int pos, int count) const;
    QChar operator [](int pos) const;
    bool canUndo() const;
    bool canRedo() const;

private:
    GapBuffer _data;
    QStack<Action> _before;
    QStack<Action> _after;
};

#endif // MEMORYDATA_H
