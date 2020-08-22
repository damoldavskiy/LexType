#ifndef LINE_H
#define LINE_H

#include <QString>
#include <QVector>
#include <QPair>

#include "word.h"
#include "cachedfont.h"

class Line
{
public:
    Line();

    QPair<Line, Line> split(int pos) const;

    void insert(int pos, Line line);
    void append(Line line);
    void remove(int pos, int count = -1);

    int find(int pos) const;

    void setFont(const CachedFont *font);
    void setStart(int start);

    int start() const;
    int size() const;
    int count() const;
//    qreal width() const;

    const Word &operator [](int word) const;
    QString mid(int pos, int count = -1) const;

    static QVector<Line> split(const QString &text, const CachedFont *font);
    static qreal appendTab(qreal left, qreal tab);

private:
    void updateWidth(int word);
    void updateStarts(int word);

    int _start;
    int _size;

    QVector<Word> _words;
    const CachedFont *_font;
};

#endif // LINE_H
