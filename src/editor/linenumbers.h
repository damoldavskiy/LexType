#ifndef LINENUMBERS_H
#define LINENUMBERS_H

#include <QWidget>
#include <QPainter>

class LineNumbers : public QWidget
{
    Q_OBJECT

public:
    LineNumbers(QWidget *parent = 0);
    void clear();
    void setMax(int value);
    void setCurrent(int value);
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
