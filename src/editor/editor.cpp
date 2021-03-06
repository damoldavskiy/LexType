#include "editor.h"

#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QStaticText>
#include <QMenu>
#include <QStyle>

#include "styler.h"
#include "mathwriter.h"
#include "keyboardlayout.h"
#include "math.h"

int charClass(QChar symbol)
{
    if (symbol.isLetterOrNumber())
        return 0;
    else if (symbol.isSpace())
        return 1;
    return 2;
}

Editor::Editor(QWidget *parent, LineNumbers *numbers, bool readOnly)
    : QWidget(parent), _text(font(), Styler::get<int>("editor-tab-width")), _numbers(numbers), _readOnly(readOnly) // TODO Pass font parameter
{
    setCursor(Qt::IBeamCursor);
    setFocusPolicy(Qt::ClickFocus);

    connect(&_timer, &QTimer::timeout, this, &Editor::tick);
    _timer.start(Styler::get<int>("editor-tick-time"));

    updateSettings();
}

int Editor::textSize() const
{
    return _text.size();
}

QString Editor::selectedText() const
{
    if (_spos == -1)
        return "";

    int left = Math::min(_spos, _pos);
    int right = Math::max(_spos, _pos);

    return _text.mid(left, right - left);
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
    _text.insert(pos, text);
    _pos = pos + text.size();
    _spos = -1;
    emit changed();
    updateUi(true);
}

void Editor::remove(int pos, int count)
{
    _pos = pos;
    _spos = -1;
    _text.remove(pos, count);
    emit changed();
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
    _text.setFont(font(), Styler::get<int>("editor-tab-width"));
    _timer.setInterval(Styler::get<int>("editor-tick-time"));
    if (Styler::get<bool>("editor-flag-wordwrap"))
        _text.setWindowWidth(width());
    else
        _text.setWindowWidth(qInf());
    if (_numbers != nullptr) {
        _numbers->setVisible(Styler::get<bool>("editor-flag-numbers"));
        _numbers->setFont(font());
    }

    updateUi(false);
}

int Editor::nextWordEnd(int pos, int dir) const
{
    // TODO Customize
    for (pos += dir; pos + dir >= 0 && pos + dir < _text.size(); pos += dir)
        if (!_text[pos].isSpace() && charClass(_text[pos]) != charClass(_text[pos + dir]))
            return pos;
    if (pos <= 0)
        return 0;
    return _text.size() - 1;
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

    updateShiftToSelection();
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

        _text.remove(pos, before.size());
        _text.insert(pos, after);

        pos += after.size();
        _pos = pos;
    } while (all);

    _spos = -1;

    emit changed();

    updateUi(true);
}

void Editor::undo()
{
    if (!_text.canUndo())
        return;

    _pos = _text.undo();
    _spos = -1;

    emit changed();
    updateUi(false);
}

void Editor::redo()
{
    if (!_text.canRedo())
        return;

    _pos = _text.redo();
    _spos = -1;

    emit changed();
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

    emit changed();
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

    emit changed();
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
    if (_readOnly)
        return;
    QPointF point = findShift(_pos);
    update(point.x(), point.y(), qCeil(Styler::get<qreal>("editor-caret-width")) + 1, _text.fontHeight());
    _caret = !_caret;
}

void Editor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int width = size().width();
    int height = size().height();

    int line = _text.findLine(_pos);
    bool wordWrap = Styler::get<bool>("editor-flag-wordwrap");

    painter.fillRect(0, 0, width, height, Styler::get<QColor>("editor-back"));

    if (_numbers != nullptr) {
        _numbers->clear();
        _numbers->setMax(_text.lineCount());
        _numbers->setCurrent(line + 1);
    }

    qreal left, cwidth, top;
    int topLine = _text.findLine(_text.findPos(0, _yshift));
    top = _text.linesHeightBefore(topLine) - _yshift;
    for (int i = topLine; i < _text.lineCount(); ++i) {
        if (top - _text.fontHeight() > height)
            break;

        int pos = _text.lineStart(i);
        int end = pos + _text.lineSize(i);

        if (_numbers != nullptr)
            _numbers->add(top + _text.fontAscent(), i + 1);

        if (QRect(0, top, width, _text.lineHeight(i)).intersects(event->rect())) {
            if (i == line && Styler::get<bool>("editor-flag-line"))
                painter.fillRect(0, top, width, _text.lineHeight(i), Styler::get<QColor>("editor-line"));

            left = 0;
            cwidth = 0;
            do {
                if (left - _xshift > width && !wordWrap)
                    break;

                left += cwidth;

                if (pos < end) {
                    if (_text.isBreak(pos)) {
                        top += _text.fontHeight();
                        left = 0;
                    }

                    cwidth = _text.advanceWidth(left, pos);

                    if (QRect(left - _xshift, top, cwidth, _text.fontHeight()).intersects(event->rect())) {
                        if ((_spos != -1 && ((_spos <= pos && pos < _pos) || (_spos > pos && pos >= _pos))) || _highlighted.contains(pos)) {
                            qreal swidth = cwidth;
                            if (pos < end && _text.isBreak(pos + 1))
                                swidth = width - left + _xshift;
                            painter.fillRect(QRectF { left - _xshift, top, swidth, _text.fontHeight() }, _error.contains(pos) ? Styler::get<QColor>("editor-error") : Styler::get<QColor>("editor-selection-back"));
                            painter.setPen(Styler::get<QColor>("editor-selection-fore"));
                        } else
                            switch (_text.markup(pos).type()) {
                            case IntervalType::Regular:
                                painter.setPen(Styler::get<QColor>("editor-regular"));
                                break;
                            case IntervalType::Mathematics:
                                painter.setPen(Styler::get<QColor>("editor-mathematics"));
                                break;
                            case IntervalType::Command:
                                painter.setPen(Styler::get<QColor>("editor-command"));
                                break;
                            case IntervalType::Special:
                                painter.setPen(Styler::get<QColor>("editor-special"));
                                break;
                            case IntervalType::Comment:
                                painter.setPen(Styler::get<QColor>("editor-comment"));
                                break;
                            }

                        if (_text[pos] != '\t' && left + cwidth >= _xshift) // Drawable symbol
                            painter.drawStaticText(QPointF { left - _xshift, top }, _text.text(pos));
                    }
                }

                if (pos == _pos && _caret && (_spos == -1 || _spos == _pos) && hasFocus() && !_readOnly)
                    painter.fillRect({ QPointF { left - _xshift, top }, QSizeF { Styler::get<qreal>("editor-caret-width"), _text.fontHeight() } }, Styler::get<QColor>("editor-caret"));
            } while (pos++ < end);

            if (_spos != -1)
                if ((_spos <= end && end < _pos) || (_spos > end && end >= _pos))
                    painter.fillRect(QRectF { left - _xshift, top, width - left + _xshift, _text.fontHeight() }, Styler::get<QColor>("editor-selection-back"));
        } else {
            top += _text.lineHeight(i) - _text.fontHeight();
        }

        top += _text.fontHeight();
    }

    if (_numbers != nullptr)
        _numbers->update();
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    if (_readOnly)
        return;

    int line;
    int newpos = -1;

    switch (event->key()) {
    case Qt::Key_Backspace:
        if (_spos != -1)
            removeSelection();
        else if (_pos > 0)
            _text.remove(--_pos, 1);
        emit changed();
        emit typed(_pos, '\b');
        break;
    case Qt::Key_Delete:
        if (_spos != -1)
            removeSelection();
        else if (_pos < _text.size())
            _text.remove(_pos, 1);
        emit changed();
        emit typed(_pos + 1, '\b');
        break;
    case Qt::Key_PageUp:
        newpos = 0;
        // Fall through
    case Qt::Key_Home:
        if (newpos == -1) {
            if (event->modifiers() & Qt::ControlModifier)
                newpos = 0;
            else {
                line = _text.findLine(qMax(_pos - 1, 0));
                newpos = _text.lineStart(line);
            }
        }
        // Fall through
    case Qt::Key_Left:
        if (newpos == -1) {
            newpos = _pos - 1;
            if (event->modifiers() & Qt::ControlModifier)
                newpos = nextWordEnd(_pos, -1);
        }

        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
            if (_pos > 0)
                _pos = newpos;
        } else if (_spos != -1) {
            _pos = qMin(_pos, _spos);
            _spos = -1;
        } else if (_pos > 0) {
            _pos = newpos;
        }
        break;
    case Qt::Key_PageDown:
        newpos = _text.size();
        // Fall through
    case Qt::Key_End:
        if (newpos == -1) {
            if (event->modifiers() & Qt::ControlModifier)
                newpos = _text.size();
            else {
                line = _text.findLine(qMin(_pos + 1, _text.size()));
                newpos = _text.lineStart(line) + _text.lineSize(line);
            }
        }
        // Fall through
    case Qt::Key_Right:
        if (newpos == -1) {
            newpos = _pos + 1;
            if (event->modifiers() & Qt::ControlModifier)
                newpos = nextWordEnd(_pos - 1, 1) + 1;
        }

        if (event->modifiers() & Qt::ShiftModifier) {
            if (_spos == -1)
                _spos = _pos;
            if (_pos < _text.size())
                _pos = newpos;
        } else if (_spos != -1) {
            _pos = qMax(_pos, _spos);
            _spos = -1;
        } else if (_pos < _text.size()) {
            _pos = newpos;
        }
        break;
    case Qt::Key_Up:
        line = _text.findLine(_pos);
        if (line > 0 || _text.breaksInline(_pos) > 0) {
            if (event->modifiers() & Qt::ShiftModifier) {
                if (_spos == -1)
                    _spos = _pos;
            } else {
                _spos = -1;
            }
            QPointF point = _text.findShift(_pos);
            _pos = _text.findPos(point.x(), point.y() - _text.fontHeight());
        }
        break;
    case Qt::Key_Down:
        line = _text.findLine(_pos);
        if (line < _text.lineCount() - 1 || _text.breaksInline(_pos) < _text.visualLinesCount(line) - 1) {
            if (event->modifiers() & Qt::ShiftModifier) {
                if (_spos == -1)
                    _spos = _pos;
            } else {
                _spos = -1;
            }
            QPointF point = _text.findShift(_pos);
            _pos = _text.findPos(point.x(), point.y() + _text.fontHeight());
        }
        break;
    case Qt::Key_Return:
        type("\n");
        emit changed();
        emit typed(_pos - 1, '\n');
        break;
    case Qt::Key_Escape:
        return;
    default:
        QString text = event->text();
        if (text.size() > 0) {
            int skip = text.size();

            if (Styler::get<bool>("editor-flag-keyboard")) {
                if (text == "ё")
                    _lastLayout = QLocale::Russian;
                else if (text == "`")
                    _lastLayout = QLocale::English;

                Interval interval;
                QChar last = '\0';
                if (_pos > 0) {
                    interval = _text.markup(_pos - 1);
                    last = _text[_pos - 1];
                }

                bool math = interval.update(last, text[0]).isMath();
                if (text == "ё" || (_lastLayout == QLocale::Russian && math))
                    if (!(!math && interval.isEscape() && text == "ё" && Styler::get<bool>("editor-flag-yo")))
                        text = KeyboardLayout::pass(text[0]);
            }

            if (Styler::get<bool>("editor-flag-brackets")) {
                if (isOpenBracket(text[0])) {
                    int closing = findBracket(_text, _pos, 1, text[0]);
                    if (closing == -1 || findBracket(_text, closing, -1) != -1) {
                        text = text[0] + getClosing(text[0]);
                        skip = 1;
                    }
                } else if (isCloseBracket(text[0]) && _pos < _text.size() && text[0] == _text[_pos]) {
                    text = "";
                    skip = 1;
                }
            }

            type(text, skip);
            emit changed();
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

    if (QApplication::doubleClickInterval() > _lastDoubleClick.elapsed())
        if (event->x() == _doubleX && event->y() == _doubleY) {
            mouseTripleClick(event);
            return;
        }

    _pos = findPos(event->x(), event->y());
    _spos = _pos;
    updateUi(false);
}

void Editor::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::MouseButton::LeftButton)
        return;

    if (_text.size() > 0) { // Last symbol of word is selected until text is empty
        int pos = findPos(event->x(), event->y(), true);
        _spos = nextWordEnd(qMin(_text.size() - 1, pos + 1), -1);
        _pos = nextWordEnd(qMax(0, pos - 1), 1) + 1;
        updateUi(false);
    }

    _lastDoubleClick = QTime::currentTime();
    _lastDoubleClick.start();
    _doubleX = event->x();
    _doubleY = event->y();
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
    qreal x = _xshift;
    qreal y = _yshift;

    _xshift -= event->angleDelta().x();
    _yshift -= event->angleDelta().y();

    correctShift();

    if (x != _xshift || y != _yshift)
        update();
}

void Editor::focusInEvent(QFocusEvent *)
{
    _caret = true;
    _timer.start(Styler::get<int>("editor-tick-time"));
    update();
}

void Editor::focusOutEvent(QFocusEvent *)
{
    _timer.stop();
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

void Editor::resizeEvent(QResizeEvent *)
{
    if (Styler::get<bool>("editor-flag-wordwrap"))
        _text.setWindowWidth(width());
    correctShift();
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

void Editor::mouseTripleClick(QMouseEvent *event)
{
    // TODO Customize (select \n on the end of line)

    int pos = findPos(event->x(), event->y(), true);
    int line = _text.findLine(pos);
    if (_text.lineSize(line) == 0)
        return;

    _spos = _text.lineStart(line);
    _pos = _spos + _text.lineSize(line);
    updateUi(false);
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

void Editor::correctShift()
{
    Math::limit(_xshift, static_cast<qreal>(0), qMax(static_cast<qreal>(0), _text.width() - width() + 1));
    Math::limit(_yshift, static_cast<qreal>(0), _text.visualLinesCount() * _text.fontHeight() - _text.fontHeight());
}

// Shifts window to the caret
void Editor::updateShift()
{
    updateShift(_text.findShift(_pos));
}

void Editor::updateShiftToSelection()
{
    if (_spos != -1)
        updateShift(_text.findShift(_spos));
    updateShift();
}

void Editor::updateShift(QPointF point)
{
    Math::limit(_xshift, point.x() - width() + 1, qMin(point.x(), qMax(static_cast<qreal>(0), _text.width() - size().width() + 1)));
    Math::limit(_yshift, point.y() - height() + _text.fontHeight(), point.y());

    Q_ASSERT(_xshift >= 0);
    Q_ASSERT(_yshift >= 0);
}

void Editor::updateUi(bool resetCaret)
{
    if (resetCaret) {
        _timer.start(Styler::get<int>("editor-tick-time"));
        _caret = true;
    }
    highlightSpecial();
    updateShift();
    update();
}

void Editor::highlightSpecial()
{
    _highlighted.clear();
    _error.clear();
    if (_spos != -1 && _spos != _pos)
        return;
    if (_pos < _text.size())
        highlightBrace(_pos);
    if (_pos > 0)
        highlightBrace(_pos - 1);
}

void Editor::highlightBrace(int pos)
{
    int dir = 0;
    if (isOpenBracket(_text[pos]))
        dir = 1;
    else if (isOpenBracket(_text[pos], -1))
        dir = -1;

    if (dir != 0) {
        _highlighted.insert(pos);
        int spos = findBracket(_text, pos, dir);
        if (spos == -1)
            _error.insert(pos);
        else
            _highlighted.insert(spos);
    }
}

void Editor::type(const QString &text, int skip)
{
    if (skip == -1)
        skip = text.size();

    if (_spos != -1 && _spos != _pos) // _spos == _pos possible only if mouse button is pressed
        removeSelection();
    _spos = -1;

    // TODO Maybe more elegant?
    int pos = _pos;
    _pos += skip;
    _text.insert(pos, text);
}

int Editor::findPos(qreal x, qreal y, bool exact) const
{
    return _text.findPos(x + _xshift, y + _yshift, exact);
}

QPointF Editor::findShift(int pos) const
{
    return _text.findShift(pos) -= { _xshift, _yshift };
}
