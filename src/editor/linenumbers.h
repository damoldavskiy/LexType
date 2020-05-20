#ifndef LINENUMBERS_H
#define LINENUMBERS_H

#include <QWidget>
#include <QPainter>

/**
 * @brief Line numbers widget
 */
class LineNumbers : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs widget
     * @param Parent widget
     */
    LineNumbers(QWidget *parent = 0);

    /**
     * @brief Cleares widget and deletes numbers
     */
    void clear();

    /**
     * @brief Sets maximum possible number. Used for calculating width
     * @param Maximum line
     */
    void setMax(int value);

    /**
     * @brief Sets selected line. Used for highlighting
     * @param Selected line
     */
    void setCurrent(int value);

    /**
     * @brief Adds the number with specified shift from the top
     * @param Shift from top of widget to the baseline
     * @param Line number
     */
    void add(qreal shift, int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QVector<QPair<qreal, int>> _values;
    int _current = -1;
    int _leftShift = 10;
    int _rightShift = 5;
    int _maxWidth = 0;
};

#endif // LINENUMBERS_H
