#include "editor.h"

#include <QApplication>
#include <QClipboard>
#include <QShortcut>

#include <QDebug>

template <typename T>
void limit(T& value, T min, T max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

template <typename T>
void limitMax(T& value, T max)
{
    if (value > max)
        value = max;
}

Editor::Editor(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::IBeamCursor);
    setFocusPolicy(Qt::ClickFocus);

    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_X), this, SLOT(cut()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_C), this, SLOT(copy()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_V), this, SLOT(paste()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_A), this, SLOT(selectAll()));

    connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
    _timer->start(_timerInterval);
}

void Editor::cut()
{
    if (_spos == -1 || _spos == _pos) // _spos == _pos possible only if mouse button is pressed
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
    if (_spos == -1 || _spos == _pos) // _spos == _pos possible only if mouse button is pressed
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

    int line = _text.findLine(_pos);

    painter.fillRect(0, 0, width, height, _background);
    painter.setPen(_foreground);

    qreal left, cwidth, top;
    int topLine = _yshift / _text.fontHeight();
    top = topLine * _text.fontHeight() - _yshift;
    for (int i = topLine; i < _text.lineCount(); ++i) {
        if (top - _text.fontHeight() > height)
            break;

        int pos = _text.lineStart(i);
        int end = pos + _text.lineSize(i);

        if (i == line)
            painter.fillRect(0, top, width, _text.fontHeight(), _activeLine);

        left = 0;
        do {
            // TODO if wordwrap is enabled, continue writing to next line instead
            if (left - _xshift > width)
                break;

            if (pos == _pos && _caret && (_spos == -1 || _spos == _pos) && hasFocus())
                painter.drawLine(QPointF { left - _xshift, top }, QPointF { left - _xshift, top + _text.fontHeight() - 1 });

            // It should be possible to draw the caret on empty line
            if (pos == end)
                break;

            cwidth = _text.advanceWidth(left, pos);
            if (_text[pos] != '\t' && left + cwidth >= _xshift) // Drawable symbol
                painter.drawText(QPointF { left - _xshift, top + _text.fontAscent() }, static_cast<QString>(_text[pos]));

            // TODO replace with style range
            if (_spos != -1)
                if ((_spos <= pos && pos < _pos) || (_spos > pos && pos >= _pos))
                    painter.fillRect(QRectF { left - _xshift, top, cwidth, _text.fontHeight() }, _selection);

            left += cwidth;
        } while (pos++ < end);

        if (_spos != -1)
            if ((_spos <= end && end < _pos) || (_spos > end && end >= _pos))
                painter.fillRect(QRectF { left - _xshift, top, width - left + _xshift, _text.fontHeight() }, _selection);

        top += _text.fontHeight();
    }
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    int line;

    switch (event->key()) {
    case Qt::Key_Backspace:
        if (_spos != -1)
            removeSelection();
        else if (_pos > 0)
            _text.remove(--_pos, 1);
        break;
    case Qt::Key_Delete:
        if (_spos != -1)
            removeSelection();
        else if (_pos < _text.size())
            _text.remove(_pos, 1);
        break;
    case Qt::Key_Left:
        if (_spos != -1) {
            _pos = qMin(_pos, _spos);
            _spos = -1;
        } else if (_pos > 0) {
            --_pos;
        }
        break;
    case Qt::Key_Right:
        if (_spos != -1) {
            _pos = qMax(_pos, _spos);
            _spos = -1;
        } else if (_pos < _text.size()) {
            ++_pos;
        }
        break;
    case Qt::Key_Up:
        _spos = -1;
        line = _text.findLine(_pos);
        if (line > 0) {
            _pos -= _text.lineStart(line);
            _pos = qMin(_pos, _text.lineSize(line - 1));
            _pos += _text.lineStart(line - 1);
        }
        break;
    case Qt::Key_Down:
        _spos = -1;
        line = _text.findLine(_pos);
        if (line < _text.lineCount() - 1) {
            _pos -= _text.lineStart(line);
            _pos = qMin(_pos, _text.lineSize(line + 1));
            _pos += _text.lineStart(line + 1);
        }
        break;
    case Qt::Key_Return:
        insert("\n");
        break;
    case Qt::Key_Escape:
        break;
    default:
        QString text = event->text();
        if (text.size() > 0)
            insert(text);
    }

    _timer->start(_timerInterval);
    _caret = true;
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

void Editor::wheelEvent(QWheelEvent *event)
{
    _yshift -= event->angleDelta().y();
    _xshift -= event->angleDelta().x();

    limit(_yshift, static_cast<qreal>(0), (_text.lineCount() - 1) * _text.fontHeight());
    limit(_xshift, static_cast<qreal>(0), qMax(static_cast<qreal>(0), _text.width() - size().width() + 1));

    update();
}

void Editor::focusInEvent(QFocusEvent *)
{
    _caret = true;
    _timer->start(_timerInterval);
    update();
}

void Editor::removeSelection()
{
    Q_ASSERT(_spos != -1);
    Q_ASSERT(_spos != _pos);

    int length = qAbs(_pos - _spos);
    if (_pos > _spos)
        _pos = _spos;
    _spos = -1;

    _text.remove(_pos, length);
}

// Shifts window to the caret
void Editor::updateShift()
{
    qreal x, y;
    QPair<qreal, qreal> pair = findShift(_pos);
    x = pair.first;
    y = pair.second;

    limitMax(_xshift, qMax(static_cast<qreal>(0), _text.width() - size().width() + 1));
    limit(_yshift, y - size().height() + _text.fontHeight(), y);
    limit(_xshift, x - size().width() + 1, x);

    Q_ASSERT(_yshift >= 0);
    Q_ASSERT(_xshift >= 0);
}

void Editor::insert(const QString &text)
{
    if (_spos != -1 && _spos != _pos) // _spos == _pos possible only if mouse button is pressed
        removeSelection();
    _spos = -1;

    _text.insert(_pos, text);
    _pos += text.size();
}

int Editor::findPos(qreal x, qreal y) const
{
    x += _xshift;

    if (x < 0)
        x = 0;

    int line = (y + _yshift) / _text.fontHeight();
    if (line < 0)
        line = 0;
    if (line < _text.lineCount()) {
        int pos = _text.lineStart(line);
        int end = pos + _text.lineSize(line);

        qreal width, left;
        width = 0;
        left = 0;
        while (pos < end && left < x) {
            width = _text.advanceWidth(left, pos);
            left += width;
            pos++;
        }

        // TODO There was problem with parallel execution, add 'pos > 0 &&' statement if needed
        if (left - x > width / 2)
            --pos;

        Q_ASSERT(pos >= 0);
        Q_ASSERT(pos <= _text.size());

        return pos;
    } else {
        return _text.size();
    }
}

QPair<qreal, qreal> Editor::findShift(int pos) const
{
    Q_ASSERT(pos >= 0);
    Q_ASSERT(pos <= _text.size());

    int line = _text.findLine(pos);
    qreal y = line * _text.fontHeight();
    qreal x = 0;

    for (int i = _text.lineStart(line); i < pos; ++i)
        x += _text.advanceWidth(x, i);

    return { x, y };
}