#ifndef MEMORYDATA_H
#define MEMORYDATA_H

#include <QStack>

#include "action.h"

class MemoryData
{
public:
    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    Action undo();
    Action redo();
    int size() const;
    QString text() const;
    QString mid(int pos, int count) const;
    QChar operator [](int index) const;
    bool canUndo() const;
    bool canRedo() const;

private:
    QString _data;
    QStack<Action> _before;
    QStack<Action> _after;
};

#endif // MEMORYDATA_H
