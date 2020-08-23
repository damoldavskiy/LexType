#ifndef MEMORYDATA_H
#define MEMORYDATA_H

#include <QStack>

#include "action.h"
#include "linetracker.h"

class MemoryData : public LineTracker
{
public:
    const Action& insert(int pos, const QString &text);
    const Action& remove(int pos, int count);

    const Action& undo();
    const Action& redo();

    const Action& backward() const;
    const Action& foreward() const;

    bool canUndo() const;
    bool canRedo() const;

private:
    QStack<Action> _before;
    QStack<Action> _after;
};

#endif // MEMORYDATA_H
