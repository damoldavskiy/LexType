#ifndef LINE_H
#define LINE_H

#include <QString>
#include <QVector>
#include <QPair>

#include "word.h"

class Line
{
public:
    explicit Line(const QString &text = "");

    QPair<Line, Line> split(int pos) const;

    void insert(int pos, Line line);
    void append(Line line);
    void remove(int pos, int count = -1);

    void setStart(int start);

    int start() const;
    int size() const;

    static QVector<Line> split(const QString &text);

private:
    int _start;
    int _size;
//    QVector<Word> _words;
};

#endif // LINE_H
