#include "editor.h"

#include <QApplication>
#include <QClipboard>
#include <QScrollBar>
#include <QPainter>
#include <QKeyEvent>
#include <QShortcut>
#include <QDebug>
#include <cassert>

template <typename T>
void limit(T& value, T min, T max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

template <typename T>
void limitMin(T& value, T min)
{
    if (value < min)
        value = min;
}

Editor::Editor(QWidget *parent)
    : QWidget(parent), _tr(1), _fm(font()), _pos(0), _spos(-1), _timerInterval(600), _xshift(0), _yshift(0), _tabWidth(_fm.width('x') * 8), _timer(this), _caret(true)
{
    setCursor(Qt::IBeamCursor);
    setFocusPolicy(Qt::ClickFocus);

    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_X), this, SLOT(cut()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_C), this, SLOT(copy()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_V), this, SLOT(paste()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_A), this, SLOT(selectAll()));

    connect(&_timer, SIGNAL(timeout()), this, SLOT(tick()));
    _timer.start(_timerInterval);
}

void Editor::cut()
{
    if (_spos == -1 || _spos == _pos) // _spos == _pos possible only if paste while mouse button is pressed
        return;

    int start = qMin(_pos, _spos);
    int length = qMax(_pos, _spos) - start;
    QApplication::clipboard()->setText(_text.mid(start, length));
    removeSelection();
    updateShift();
    update();
}

void Editor::copy()
{
    if (_spos == -1)
        return;

    int start = qMin(_pos, _spos);
    int length = qMax(_pos, _spos) - start;
    QApplication::clipboard()->setText(_text.mid(start, length));
}

void Editor::paste()
{
    insert(QApplication::clipboard()->text());
    updateShift();
    update();
}

void Editor::selectAll()
{
    if (_text.size() == 0)
        return;
    _spos = 0;
    _pos = _text.size();
    updateShift();
    update();
}

void Editor::tick()
{
    update();
    _caret = !_caret;
}

void Editor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int width = size().width();
    int height = size().height();

    int line = _tr.getLine(_pos);

    painter.fillRect(0, 0, width, height, QColor(50, 50, 50));
    painter.setPen(QColor(240, 240, 240));

    qreal left, cwidth, space, top;
    int topLine = _yshift / _fm.height();
    top = - (_yshift - topLine * _fm.height());
    for (int i = topLine; i < _tr.size(); ++i)
    {
        if (top - _fm.height() > height)
            break;

        int pos = _tr.lineStart(i);
        int end = pos + _tr.lineSize(i);

        if (i == line)
            painter.fillRect(0, top, width, _fm.height(), QColor(60, 60, 60));

        left = 0;
        do
        {
            // TODO if wordwrap is enabled, continue writing to next line instead
            if (left - _xshift > width)
                break;

            if (pos == _pos && _caret && (_spos == -1 || _spos == _pos) && hasFocus())
                painter.drawLine(QPointF { left - _xshift, top }, QPointF { left - _xshift, top + _fm.height() - 1 });

            if (_text[pos] == '\t')
            {
                space = _tabWidth * (int)(left / _tabWidth);
                if (space <= left)
                    space += _tabWidth;
                cwidth = space - left;
            }
            else
            {
                cwidth = _fm.width(_text[pos]);
                if (left + cwidth >= _xshift)
                    painter.drawText(QPointF { left - _xshift, top + _fm.ascent() }, (QString)_text[pos]);
            }

            if (_spos != -1)
                if ((_spos <= pos && pos < _pos) || (_spos > pos && pos >= _pos))
                    painter.fillRect(QRectF { left - _xshift, top, cwidth, _fm.height()}, QColor(40, 60, 130,90));

            left += cwidth;
        }
        while (pos++ < end);

        top += _fm.height();
    }
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    int line;
    QString text = event->text();

    switch (event->key()) {
    case Qt::Key_Backspace:
        if (_spos != -1)
            removeSelection();
        else if (_pos > 0)
        {
            _text.remove(--_pos, 1);
            _tr.remove(_pos, 1);
        }
        break;
    case Qt::Key_Delete:
        if (_spos != -1)
            removeSelection();
        else if (_pos < _text.size())
        {
            _text.remove(_pos, 1);
            _tr.remove(_pos, 1);
        }
        break;
    case Qt::Key_Left:
        if (_spos != -1)
        {
            _pos = qMin(_pos, _spos);
            _spos = -1;
        }
        else if (_pos > 0)
            --_pos;
        break;
    case Qt::Key_Right:
        if (_spos != -1)
        {
            _pos = qMax(_pos, _spos);
            _spos = -1;
        }
        else if (_pos < _text.size())
            ++_pos;
        break;
    case Qt::Key_Up:
        _spos = -1;
        line = _tr.getLine(_pos);
        if (line > 0)
        {
            _pos -= _tr.lineStart(line);
            _pos = qMin(_pos, _tr.lineSize(line - 1));
            _pos += _tr.lineStart(line - 1);
        }
        break;
    case Qt::Key_Down:
        _spos = -1;
        line = _tr.getLine(_pos);
        if (line < _tr.size() - 1)
        {
            _pos -= _tr.lineStart(line);
            _pos = qMin(_pos, _tr.lineSize(line + 1));
            _pos += _tr.lineStart(line + 1);
        }
        break;
    case Qt::Key_Return:
        insert("\n");
        break;
    case Qt::Key_Escape:
        break;
    default:
        if (text.size() > 0)
            insert(text);
    }

    _caret = true;
    _timer.start(_timerInterval);
    updateShift();
    update();
}

void Editor::mousePressEvent(QMouseEvent *event)
{
    _pos = findPos(event->x(), event->y());
    _spos = _pos;
    updateShift();
    update();
}

void Editor::mouseReleaseEvent(QMouseEvent *)
{
    if (_pos == _spos)
        _spos = -1;
}

void Editor::mouseMoveEvent(QMouseEvent *event)
{
    _pos = findPos(event->x(), event->y());
    updateShift();
    update();
}

void Editor::resizeEvent(QResizeEvent *event)
{
    qreal lines = _tr.size() * _fm.height();
    int oldHeight = event->oldSize().height();
    int height = event->size().height();

    if (lines > size().height())
    {
        if (lines < size().height() + _yshift && oldHeight < height)
            _yshift += oldHeight - height;
    }
    else
        _yshift = 0;
}

void Editor::wheelEvent(QWheelEvent *event)
{
    _yshift -= event->angleDelta().y();
    _xshift -= event->angleDelta().x();

    // TODO _xshift should not be higher than the longest line, delete limitMin function
    limit(_yshift, (qreal)0, (_tr.size() - 1) * _fm.height());
    limitMin(_xshift, (qreal)0);

    update();
}

void Editor::focusInEvent(QFocusEvent *)
{
    _caret = true;
    _timer.start(_timerInterval);
    update();
}

void Editor::removeSelection()
{
    assert(_spos != -1);
    assert(_spos != _pos);
    int length = qAbs(_pos - _spos);
    if (_pos > _spos)
        _pos = _spos;
    _spos = -1;
    _text.remove(_pos, length);
    _tr.remove(_pos, length);
}

// Shifts window to the caret
void Editor::updateShift()
{
    qreal x, y;
    QPair<qreal, qreal> pair = findShift(_pos);
    x = pair.first;
    y = pair.second;

    limit(_yshift, y - size().height() + _fm.height(), y);
    limit(_xshift, x - size().width() + 1, x);

    assert(_yshift >= 0);
    assert(_xshift >= 0);
}

void Editor::insert(const QString& text)
{
    if (_spos != -1 && _spos != _pos) // _spos == _pos possible only if paste while mouse button is pressed
        removeSelection();
    _spos = -1;
    _text.insert(_pos, text);
    int count = 0;
    for (int i = 0; i < text.size(); ++i)
        if (text[i] == '\n')
        {
            if (count > 0)
            {
                _tr.insert(_pos, count);
                _pos += count;
                count = 0;
            }
            _tr.insertLine(_pos++);
        }
        else
            ++count;
    if (count > 0)
    {
        _tr.insert(_pos, count);
        _pos += count;
    }
}

int Editor::findPos(qreal x, qreal y) const
{
    x += _xshift;

    if (x < 0)
        x = 0;

    int line = (y + _yshift) / _fm.height();
    if (line < 0)
        line = 0;
    if (line < _tr.size())
    {
        int pos = _tr.lineStart(line);
        int end = pos + _tr.lineSize(line);

        qreal space, width, left;
        width = 0;
        left = 0;
        while (pos < end && left < x)
        {
            if (_text[pos] == '\t')
            {
                space = _tabWidth * (int)(left / _tabWidth);
                if (space <= left)
                    space += _tabWidth;
                width = space - left;
                left = space;
            }
            else
            {
                width = _fm.width(_text[pos]);
                left += width;
            }
            pos++;
        }

        // TODO There was problem with parallel execution, add 'pos > 0 &&' statement if needed
        if (left - x > width / 2)
            --pos;

        return pos;
    }
    else
        return _text.size();
}

QPair<qreal, qreal> Editor::findShift(int pos) const
{
    assert(pos >= 0);
    assert(pos <= _text.size());

    int line = _tr.getLine(pos);
    qreal y = line * _fm.height();
    qreal x = 0;

    qreal space;
    for (int i = _tr.lineStart(line); i < pos; ++i)
    {
        if (_text[i] == '\t')
        {
            space = _tabWidth * (int)(x / _tabWidth);
            if (space <= x)
                space += _tabWidth;
            x = space;
        }
        else
            x += _fm.width(_text[i]);
    }

    return { x, y };
}
