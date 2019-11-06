#ifndef LINETRACKER_H
#define LINETRACKER_H

#include <QVector>

struct Line
{
    int start;
    int size;
};

class LineTracker
{
public:
    explicit LineTracker(int n);
    void insert(int pos, int count);
    void insertLine(int pos);
    void remove(int pos, int count);
    int find(int pos) const;
    const Line &operator [](int line) const;
    int size() const;

private:
    QVector<Line> _lines;
};

#endif // LINETRACKER_H
