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
    void add(qreal shift, int value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QVector<QPair<qreal, int>> _values;
    int _leftShift = 20;
    int _rightShift = 10;
    int _maxWidth = 0;
};

#endif // LINENUMBERS_H
