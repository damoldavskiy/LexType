#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

#include "text.h"

class Editor : public QWidget
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);
    QString text() const;

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
    void wheelEvent(QWheelEvent *event);
    void focusInEvent(QFocusEvent *event);

    void removeSelection();
    void updateShift();
    void insert(const QString &text);

    int findPos(qreal x, qreal y) const;
    QPair<qreal, qreal> findShift(int pos) const;
    qreal advanceWidth(qreal left, int pos) const;
    qreal lineWidth(int line) const;

private:
    Text _text = Text(font());

    int _pos = 0;
    int _spos = -1;
    qreal _xshift = 0;
    qreal _yshift = 0;

    QTimer *_timer = new QTimer(this);
    bool _caret = true;

    int _timerInterval = 600;

    QColor _background = { 50, 50, 50 };
    QColor _foreground = { 240, 240, 240 };
    QColor _activeLine = { 60, 60, 60 };
    QColor _selection = { 40, 60, 130, 90 };
};

#endif // EDITOR_H
