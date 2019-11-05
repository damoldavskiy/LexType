#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTimer>

#include "linetracker.h"

class Editor : public QWidget
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);

public slots:
    void cut();
    void copy();
    void paste();
    void selectAll();
    void tick();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent* event);
    void focusInEvent(QFocusEvent* event);

    void removeSelection();
    void updateShift();
    void insert(const QString& text);

    int findPos(qreal x, qreal y) const;
    QPair<qreal, qreal> findShift(int pos) const;

private:
    QString _text;
    LineTracker _tr;
    QFontMetricsF _fm;
    int _pos, _spos, _timerInterval;
    qreal _xshift, _yshift, _tabWidth;
    QTimer _timer;
    bool _caret;
};

#endif // EDITOR_H
