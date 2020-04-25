#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

#include "text.h"
#include "linenumbers.h"
#include "styler.h"

class Editor : public QWidget
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0, LineNumbers *numbers = 0);
    QString text() const;
    void setText(const QString &text);
    int caret() const;
    void setCaret(int value);
    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    Interval markup(int pos) const;
    void updateSettings();

signals:
    void typed(int pos, QChar symbol);

public slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();

private slots:
    void tick();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    bool event(QEvent *event);

    void removeSelection();
    void updateShift();
    void updateUi(bool resetCaret);
    void type(const QString &text);

    void insertText(int pos, const QString &text);
    void removeText(int pos, int count);

    int findPos(qreal x, qreal y) const;
    QPair<qreal, qreal> findShift(int pos) const;
//    qreal advanceWidth(qreal left, int pos) const;
//    qreal lineWidth(int line) const;

private:
    Text _text;
    LineNumbers *_numbers;

    int _pos = 0;
    int _spos = -1;
    qreal _xshift = 0;
    qreal _yshift = 0;

    QTimer *_timer = new QTimer(this);
    bool _caret = true;

    int _timerInterval = 600;
};

#endif // EDITOR_H
