#ifndef STRINGGAPBUFFER_H
#define STRINGGAPBUFFER_H

#include <QString>

class StringGapBuffer
{
public:
    StringGapBuffer();
    StringGapBuffer(const StringGapBuffer &other);
    StringGapBuffer(StringGapBuffer &&other);

    StringGapBuffer& operator =(const StringGapBuffer &other);
    StringGapBuffer& operator =(StringGapBuffer &&other);

    ~StringGapBuffer();

    void insert(int pos, const QString &text);
    void append(const QString &text);
    void remove(int pos, int count = -1);

    int find(int pos, const QString &substring, bool matchCase) const;

    QString mid(int pos, int count = -1) const;
    QChar operator [](int pos) const;
    int size() const;

    QString toString() const;

private:
    void copy(QChar *target, int from, int to) const;
    void ensureGap(int pos, int size);

    QChar *_data;
    int _allocated = 8;
    int _gap = 0;
    int _gapSize = 8;
};

#endif // STRINGGAPBUFFER_H
