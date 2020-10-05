#ifndef EDITOR_H
#define EDITOR_H

#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>

#include "text.h"
#include "linenumbers.h"

class Editor : public QWidget
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0, LineNumbers *numbers = 0);

    int textSize() const;
    QString selectedText() const;

    QString text() const;
    void setText(const QString &text);
    int caret() const;
    void setCaret(int value);

    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    QChar operator [](int pos) const;

    Interval markup(int pos) const;
    void updateSettings();

    int nextWordEnd(int pos, int dir) const;

signals:
    void typed(int pos, QChar symbol);
    void changed();

public slots:
    void find(const QString &substring, bool matchCase);
    void replace(const QString &before, const QString &after, bool all, bool matchCase);
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
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool event(QEvent *event);

    void mouseTripleClick(QMouseEvent *event);
    void removeSelection();
    void correctShift();
    void updateShift();
    void updateShiftToSelection();
    void updateShift(QPointF point);
    void updateUi(bool resetCaret);
    void highlightSpecial();
    void highlightBrace(int pos);
    void type(const QString &text, int skip = -1);

    int findPos(qreal x, qreal y, bool exact = false) const;
    QPointF findShift(int pos) const;

private:
    Text _text;
    LineNumbers *_numbers;

    int _pos = 0;
    int _spos = -1;
    qreal _xshift = 0;
    qreal _yshift = 0;

    QTimer _timer;
    bool _caret = true;

    QSet<int> _highlighted;
    QSet<int> _error;

    int _lastLayout = 0;

    QTime _lastDoubleClick;
    int _doubleX = -1;
    int _doubleY = -1;
};

#endif // EDITOR_H
