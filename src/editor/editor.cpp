#include "editor.h"

#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QStaticText>
#include <QMenu>
#include <QStyle>

#include "styler.h"

template <typename T>
void limit(T& value, T min, T max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

Editor::Editor(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::IBeamCursor);
    setFocusPolicy(Qt::ClickFocus);

    connect(_timer, SIGNAL(timeout()), this, SLOT(tick()));
    _timer->start(_timerInterval);
}

QString Editor::text() const
{
    return _text.text();
}

void Editor::setText(const QString &text)
{
    if (_text.size() > 0)
        _text.remove(0, _text.size());
    _text.insert(0, text);
    _pos = 0;
    _spos = -1;
    updateGui(true);
}

void Editor::insert(const QString &text)
{
    _spos = -1;

    _text.insert(_pos, text);
    _pos += text.size();
    updateGui(true);
}

void Editor::undo()
{
    if (!_text.canUndo())
        return;

    _pos = _text.undo();
    _spos = -1;
    updateGui(false);
}

void Editor::redo()
{
    if (!_text.canRedo())
        return;

    _pos = _text.redo();
    _spos = -1;
    updateGui(false);
}

void Editor::cut()
{
    if (_spos == -1 || _spos == _pos) // _spos == _pos possible only if mouse button is pressed
        return;

    int start = qMin(_pos, _spos);
    int length = qMax(_pos, _spos) - start;
    QApplication::clipboard()->setText(_text.mid(start, length));
    removeSelection();
    updateGui(false);
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
    insertData(QApplication::clipboard()->text());
    updateGui(false);
}

void Editor::selectAll()
{
    if (_text.size() == 0)
        return;
    _spos = 0;
    _pos = _text.size();
    updateGui(false);
}

void Editor::tick()
{
    update();
    _caret = !_caret;
}

void Editor::paintEvent(QPaintEvent *)
{
    // TODO the font is default, not from Text
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
                painter.drawStaticText(QPointF { left - _xshift, top }, _text.text(pos));

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
        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
            if (_pos > 0)
                --_pos;
        } else if (_spos != -1) {
            _pos = qMin(_pos, _spos);
            _spos = -1;
        } else if (_pos > 0) {
            --_pos;
        }
        break;
    case Qt::Key_Right:
        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
            if (_pos < _text.size())
                ++_pos;
        } else if (_spos != -1) {
            _pos = qMax(_pos, _spos);
            _spos = -1;
        } else if (_pos < _text.size()) {
            ++_pos;
        }
        break;
    case Qt::Key_Up:
        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
        } else {
            _spos = -1;
        }
        line = _text.findLine(_pos);
        if (line > 0) {
            _pos -= _text.lineStart(line);
            _pos = qMin(_pos, _text.lineSize(line - 1));
            _pos += _text.lineStart(line - 1);
        }
        break;
    case Qt::Key_Down:
        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
        } else {
            _spos = -1;
        }
        line = _text.findLine(_pos);
        if (line < _text.lineCount() - 1) {
            _pos -= _text.lineStart(line);
            _pos = qMin(_pos, _text.lineSize(line + 1));
            _pos += _text.lineStart(line + 1);
        }
        break;
    case Qt::Key_Return:
        insertData("\n");
        break;
    case Qt::Key_Escape:
        break;
    default:
        QString text = event->text();
        if (text.size() > 0)
            insertData(text);
    }

    updateGui(true);
}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::LeftButton)
        return;
    _pos = findPos(event->x(), event->y());
    _spos = _pos;
    updateGui(false);
}

void Editor::mouseReleaseEvent(QMouseEvent *)
{
    if (_pos == _spos)
        _spos = -1;
}

void Editor::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::MouseButton::LeftButton) == 0)
        return;
    _pos = findPos(event->x(), event->y());
    updateGui(false);
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

void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setStyleSheet(Styler::menuStyle());

    QAction *cutAction, *copyAction, *pasteAction, *selectAllAction;

    cutAction = new QAction("Cut", &menu);
    cutAction->setEnabled(_spos != -1);
    connect(cutAction, &QAction::triggered, this, &Editor::cut);

    copyAction = new QAction("Copy", &menu);
    copyAction->setEnabled(_spos != -1);
    connect(copyAction, &QAction::triggered, this, &Editor::copy);

    pasteAction = new QAction("Paste", &menu);
    connect(pasteAction, &QAction::triggered, this, &Editor::paste);

    selectAllAction = new QAction("Select all");
    connect(selectAllAction, &QAction::triggered, this, &Editor::selectAll);

    menu.addActions({ cutAction, copyAction, pasteAction, selectAllAction });
    menu.exec(event->globalPos());
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

    limit(_yshift, y - size().height() + _text.fontHeight(), y);
    limit(_xshift, x - size().width() + 1, qMin(x, qMax(static_cast<qreal>(0), _text.width() - size().width() + 1)));

    Q_ASSERT(_yshift >= 0);
    Q_ASSERT(_xshift >= 0);
}

void Editor::updateGui(bool resetCaret)
{
    if (resetCaret) {
        _timer->start(_timerInterval);
        _caret = true;
    }
    updateShift();
    update();
}

void Editor::insertData(const QString &text)
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
