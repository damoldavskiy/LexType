#ifndef LINETRACKER_H
#define LINETRACKER_H

#include <QVector>
#include <QString>
#include <QPair>

#include "line.h"

class LineTracker
{
public:
    explicit LineTracker(int n);

    void insert(int pos, const QString &text);
    void remove(int pos, int count);

    int find(int pos) const;

    const Line &operator [](int line) const;
    int size() const;

private:
    void updateStarts(int line);

    QVector<Line> _lines;
};

#endif // LINETRACKER_H
