#include "memorydata.h"

void MemoryData::insert(int pos, const QString &text)
{
    _before.push({ pos, text, Action::Insert });
    _after.clear();
    _data.insert(pos, text);
}

void MemoryData::remove(int pos, int count)
{
    _before.push({ pos, _data.mid(pos, count), Action::Remove });
    _after.clear();
    _data.remove(pos, count);
}

Action MemoryData::undo()
{
    Q_ASSERT(canUndo());

    Action action = _before.pop();
    _after.push(action);

    if (action.type == Action::Insert)
        _data.remove(action.index, action.text.size());
    else
        _data.insert(action.index, action.text);

    return action;
}

Action MemoryData::redo()
{
    Q_ASSERT(canRedo());

    Action action = _after.pop();
    _before.push(action);

    if (action.type == Action::Insert)
        _data.insert(action.index, action.text);
    else
        _data.remove(action.index, action.text.size());

    return action;
}

int MemoryData::size() const
{
    return _data.size();
}

QString MemoryData::text() const
{
    return _data;
}

QString MemoryData::mid(int pos, int count) const
{
    return _data.mid(pos, count);
}

QChar MemoryData::operator [](int index) const
{
    return _data[index];
}

bool MemoryData::canUndo() const
{
    return _before.size() > 0;
}

bool MemoryData::canRedo() const
{
    return _after.size() > 0;
}
