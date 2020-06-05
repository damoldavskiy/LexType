#include "editor.h"

#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QStaticText>
#include <QMenu>
#include <QStyle>

#include "styler.h"
#include "keyboardlayout.h"

template <typename T>
void limit(T& value, T min, T max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

Editor::Editor(QWidget *parent, LineNumbers *numbers)
    : QWidget(parent), _text(font()), _numbers(numbers) // TODO Pass font parameter
{
    setCursor(Qt::IBeamCursor);
    setFocusPolicy(Qt::ClickFocus);
    updateSettings();

    connect(_timer, &QTimer::timeout, this, &Editor::tick);
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
    insertText(0, text);
    _pos = 0;
    _spos = -1;
    updateUi(true);
}

int Editor::caret() const
{
    return _pos;
}

void Editor::setCaret(int value)
{
    Q_ASSERT(value >= 0);
    Q_ASSERT(value <= _text.size());
    _pos = value;
}

void Editor::insert(int pos, const QString &text)
{
    insertText(pos, text);
    _pos = pos + text.size();
    _spos = -1;
    updateUi(true);
}

void Editor::remove(int pos, int count)
{
    _pos = pos;
    _spos = -1;
    removeText(pos, count);
    updateUi(true);
}

QChar Editor::operator [](int pos) const
{
    return _text[pos];
}

Interval Editor::markup(int pos) const
{
    return _text.markup(pos);
}

void Editor::updateSettings()
{
    setFont(Styler::get<QFont>("editor-font"));
    _text.setFont(font());
    if (_numbers != nullptr) {
        _numbers->setVisible(Styler::get<bool>("editor-flag-numbers"));
        _numbers->setFont(font());
    }
}

void Editor::find(const QString &substring, bool matchCase)
{
    if (substring.size() == 0)
        return; // Don't update _spos

    int pos = _text.find(_spos == -1 ? _pos : _spos, substring, matchCase);
    if (pos == -1)
        return;

    _pos = pos;
    _spos = pos + substring.size();
    updateUi(true);
}

void Editor::replace(const QString &before, const QString &after, bool all, bool matchCase)
{
    if (before.size() == 0)
        return;

    int pos = all ? 0 : _pos;

    do {
        pos = _text.find(pos, before, matchCase);
        if (pos == -1)
            break;

        removeText(pos, before.size());
        insertText(pos, after);

        pos += after.size();
    } while (all);

    limit(_pos, 0, _text.size());
    _spos = -1;

    updateUi(true);
}

void Editor::undo()
{
    if (!_text.canUndo())
        return;

    _pos = _text.undo();
    _spos = -1;
    updateUi(false);
}

void Editor::redo()
{
    if (!_text.canRedo())
        return;

    _pos = _text.redo();
    _spos = -1;
    updateUi(false);
}

void Editor::cut()
{
    if (_spos == -1 || _spos == _pos) // _spos == _pos possible only if mouse button is pressed
        return;

    int start = qMin(_pos, _spos);
    int length = qMax(_pos, _spos) - start;
    QApplication::clipboard()->setText(_text.mid(start, length));
    removeSelection();
    updateUi(false);
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
    type(QApplication::clipboard()->text());
    updateUi(false);
}

void Editor::selectAll()
{
    if (_text.size() == 0)
        return;
    _spos = 0;
    _pos = _text.size();
    updateUi(false);
}

void Editor::tick()
{
    update();
    // TODO Do not repaint whole widget
    _caret = !_caret;
}

void Editor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int width = size().width();
    int height = size().height();

    int line = _text.findLine(_pos);

    painter.fillRect(0, 0, width, height, Styler::get<QColor>("editor-back"));

    if (_numbers != nullptr) {
        _numbers->clear();
        _numbers->setMax(_text.lineCount());
        _numbers->setCurrent(line + 1);
    }

    qreal left, cwidth, top;
    int topLine = _yshift / _text.fontHeight();
    top = topLine * _text.fontHeight() - _yshift;
    for (int i = topLine; i < _text.lineCount(); ++i) {
        if (top - _text.fontHeight() > height)
            break;

        int pos = _text.lineStart(i);
        int end = pos + _text.lineSize(i);

        if (_numbers != nullptr)
            _numbers->add(top + _text.fontAscent(), i + 1);

        if (i == line && Styler::get<bool>("editor-flag-line"))
            painter.fillRect(0, top, width, _text.fontHeight(), Styler::get<QColor>("editor-line"));

        left = 0;
        cwidth = 0;
        do {
            // TODO If wordwrap is enabled, continue writing to next line instead
            if (left - _xshift > width)
                break;

            left += cwidth;
            if (pos < end) {
                switch (_text.markup(pos)) {
                case Interval::Regular:
                    painter.setPen(Styler::get<QColor>("editor-regular"));
                    break;
                case Interval::Mathematics:
                    painter.setPen(Styler::get<QColor>("editor-mathematics"));
                    break;
                case Interval::Command:
                    painter.setPen(Styler::get<QColor>("editor-command"));
                    break;
                case Interval::Special:
                    painter.setPen(Styler::get<QColor>("editor-special"));
                    break;
                case Interval::Comment:
                    painter.setPen(Styler::get<QColor>("editor-comment"));
                    break;
                }

                cwidth = _text.advanceWidth(left, pos);
                if (_text[pos] != '\t' && left + cwidth >= _xshift) // Drawable symbol
                    painter.drawStaticText(QPointF { left - _xshift, top }, _text.text(pos));

                // TODO replace with style range
                if (_spos != -1)
                    if ((_spos <= pos && pos < _pos) || (_spos > pos && pos >= _pos))
                        painter.fillRect(QRectF { left - _xshift, top, cwidth, _text.fontHeight() }, Styler::get<QColor>("editor-selection"));
            }

            if (pos == _pos && _caret && (_spos == -1 || _spos == _pos) && hasFocus())
                painter.fillRect({ QPointF { left - _xshift, top }, QSizeF { 1, _text.fontHeight()} }, Styler::get<QColor>("editor-caret"));
        } while (pos++ < end);

        if (_spos != -1)
            if ((_spos <= end && end < _pos) || (_spos > end && end >= _pos))
                painter.fillRect(QRectF { left - _xshift, top, width - left + _xshift, _text.fontHeight() }, Styler::get<QColor>("editor-selection"));

        top += _text.fontHeight();
    }

    if (_numbers != nullptr)
        _numbers->update();
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    int line;

    switch (event->key()) {
    case Qt::Key_Backspace:
        if (_spos != -1)
            removeSelection();
        else if (_pos > 0)
            removeText(--_pos, 1);
        break;
    case Qt::Key_Delete:
        if (_spos != -1)
            removeSelection();
        else if (_pos < _text.size())
            removeText(_pos, 1);
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
        type("\n");
        emit typed(_pos - 1, '\n');
        break;
    case Qt::Key_Escape:
        return;
    default:
        QString text = event->text();
        if (text.size() > 0) {
            // TODO Refactor
            // TODO Symbol just after mathematics is reversed
            if (Styler::get<bool>("editor-flag-keyboard") && (text == "ё" || (_pos > 0 && _text.markup(_pos - 1) == Interval::Mathematics)))
                text = KeyboardLayout::pass(text[0]);
            type(text);
            emit typed(_pos - 1, text[0]);
        }
    }

    event->setAccepted(true);
    updateUi(true);
}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::LeftButton)
        return;
    _pos = findPos(event->x(), event->y());
    _spos = _pos;
    updateUi(false);
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
    updateUi(false);
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

void Editor::focusOutEvent(QFocusEvent *)
{
    _timer->stop();
    _caret = false;
    update();
}

void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setStyleSheet(Styler::get<QString>("menu-style"));

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

bool Editor::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent != nullptr && (keyEvent->modifiers() & Qt::ControlModifier) == 0 && keyEvent->text().size() > 0) {
            event->setAccepted(true);
            return true;
        }
    }

    return QWidget::event(event);
}

void Editor::removeSelection()
{
    Q_ASSERT(_spos != -1);
    Q_ASSERT(_spos != _pos);

    int length = qAbs(_pos - _spos);
    if (_pos > _spos)
        _pos = _spos;
    _spos = -1;

    removeText(_pos, length);
}

// Shifts window to the caret
void Editor::updateShift()
{
    if (_spos != -1)
        updateShift(findShift(_spos)); // TODO We can find this point faster, based on _pos point
    updateShift(findShift(_pos));
}

void Editor::updateShift(QPointF point)
{
    limit(_yshift, point.y() - size().height() + _text.fontHeight(), point.y());
    limit(_xshift, point.x() - size().width() + 1, qMin(point.x(), qMax(static_cast<qreal>(0), _text.width() - size().width() + 1)));

    Q_ASSERT(_yshift >= 0);
    Q_ASSERT(_xshift >= 0);
}

void Editor::updateUi(bool resetCaret)
{
    if (resetCaret) {
        _timer->start(_timerInterval);
        _caret = true;
    }
    updateShift();
    update();
}

void Editor::type(const QString &text)
{
    if (_spos != -1 && _spos != _pos) // _spos == _pos possible only if mouse button is pressed
        removeSelection();
    _spos = -1;

    // TODO Maybe more elegant?
    int pos = _pos;
    _pos += text.size();
    insertText(pos, text);
}

void Editor::insertText(int pos, const QString &text)
{
    _text.insert(pos, text);
//    emit inserted(pos, text);
}

void Editor::removeText(int pos, int count)
{
    _text.remove(pos, count);
//    emit removed(pos, count);
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

QPointF Editor::findShift(int pos) const
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

