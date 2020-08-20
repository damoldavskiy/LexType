#include "memorydata.h"

void MemoryData::insert(int pos, const QString &text)
{
    _before.push({ pos, text, Action::Insert });
    _after.clear();
    LineTracker::insert(pos, text);
}

void MemoryData::remove(int pos, int count)
{
    _before.push({ pos, LineTracker::mid(pos, count), Action::Remove });
    _after.clear();
    LineTracker::remove(pos, count);
}

Action MemoryData::undo()
{
    Q_ASSERT(canUndo());

    Action action = _before.pop();
    _after.push(action);

    if (action.type == Action::Insert)
        LineTracker::remove(action.index, action.text.size());
    else
        LineTracker::insert(action.index, action.text);

    return action;
}

Action MemoryData::redo()
{
    Q_ASSERT(canRedo());

    Action action = _after.pop();
    _before.push(action);

    if (action.type == Action::Insert)
        LineTracker::insert(action.index, action.text);
    else
        LineTracker::remove(action.index, action.text.size());

    return action;
}

bool MemoryData::canUndo() const
{
    return _before.size() > 0;
}

bool MemoryData::canRedo() const
{
    return _after.size() > 0;
}
