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

    /**
     * @brief Inserts text in the position
     * @param Position
     * @param Text
     */
    void insert(int pos, const QString &text);

    /**
     * @brief Removes symbols from position
     * @param Start position
     * @param Count of deleted symbols
     */
    void remove(int pos, int count);

    /**
     * @brief Searches first match of substring
     * @param Start index for searching
     * @param Substring to find
     * @param Match case
     * @return Result position, -1 if not found
     */
    int find(int pos, const QString &substring, bool matchCase) const;

    /**
     * @brief Returns substring of text
     * @param First symbol position
     * @param Count of symbols
     * @return Substring
     */
    QString mid(int pos, int count) const;

    /**
     * @brief Returns symbol at specified index
     * @param Position of symbol
     * @return Symbol
     */
    QChar operator [](int pos) const;

    /**
     * @brief Returns count of symbols in text
     * @return Count
     */
    int size() const;

    /**
     * @brief Converts data to string
     * @return String representation
     */
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
