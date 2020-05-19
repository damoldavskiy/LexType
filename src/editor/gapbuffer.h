#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <QString>

/**
 * @brief Gap buffer implementation
 */
class GapBuffer
{
public:
    ~GapBuffer();
    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    int find(int pos, const QString &substring, bool matchCase) const;
    QString mid(int pos, int count) const;
    QChar operator [](int pos) const;
    int size() const;
    QString toString() const;

private:
    void copy(QChar *target, int from, int to) const;
    void ensureGap(int pos, int size);

    QChar* _data = new QChar[8];
    int _allocated = 8;
    int _gap = 0;
    int _gapSize = 8;
};

#endif // GAPBUFFER_H
