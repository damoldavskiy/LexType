#ifndef MEMORYDATA_H
#define MEMORYDATA_H

#include <QStack>

#include "action.h"
#include "linetracker.h"

class MemoryData : public LineTracker
{
public:
    void insert(int pos, const QString &text);
    void remove(int pos, int count);

    Action undo();
    Action redo();

    bool canUndo() const;
    bool canRedo() const;

private:
    QStack<Action> _before;
    QStack<Action> _after;
};

#endif // MEMORYDATA_H
