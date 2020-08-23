#ifndef MARKUPMODEL_H
#define MARKUPMODEL_H

#include <QPair>

#include "interval.h"
#include "gapbuffer.h"

class MarkupModel
{
public:
    Interval interval(int pos) const;
    int size() const;

    void set(int start, int end, Interval interval);
    void insert(int start, int end, Interval interval);
    void remove(int start, int end);

private:
    GapBuffer<Interval> _intervals;
};

#endif // MARKUPMODEL_H
