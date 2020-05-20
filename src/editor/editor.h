#ifndef EDITOR_H
#define EDITOR_H

#include <QPainter>
#include <QKeyEvent>
#include <QTimer>

#include "text.h"
#include "linenumbers.h"

/**
 * @brief Editor widget
 */
class Editor : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs editor
     * @param Parent widget
     * @param Line numbers widget to update
     */
    Editor(QWidget *parent = 0, LineNumbers *numbers = 0);

    /**
     * @brief Returns current text in editor
     * @return The stored text
     */
    QString text() const;

    /**
     * @brief Sets current text in editor
     * @param Text to store
     */
    void setText(const QString &text);

    /**
     * @brief Current caret position
     * @return Position
     */
    int caret() const;

    /**
     * @brief Sets caret position
     * @param New position
     */
    void setCaret(int value);

    /**
     * @brief Inserts specified text
     * @param Insertion position
     * @param Text to insert
     */
    void insert(int pos, const QString &text);

    /**
     * @brief Removes text from an editor
     * @param First removed symbol position
     * @param Count of symbols to delete
     */
    void remove(int pos, int count);

    /**
     * @brief Returns symbol in the position
     * @param Position of symbol
     * @return The symbol
     */
    QChar operator [](int pos) const;

    /**
     * @brief Gets markup in the position
     * @param Position of markup
     * @return Markup
     */
    Interval markup(int pos) const;

    /**
     * @brief Updated editor look using Styler
     */
    void updateSettings();

signals:
    /**
     * @brief Emitted when user types symbol
     * @param Position of new symbol
     * @param The new symbol
     */
    void typed(int pos, QChar symbol);

public slots:
    /**
     * @brief Finds substring and makes selection of it
     * @param Text to find
     * @param Match case while searching
     */
    void find(const QString &substring, bool matchCase);

    /**
     * @brief Replaces substring with another
     * @param Pattern text
     * @param Replacer text
     * @param If false, replace first find. Replace all found matches otherwise
     * @param Match case while searching
     */
    void replace(const QString &before, const QString &after, bool all, bool matchCase);

    /**
     * @brief Undo last action
     */
    void undo();

    /**
     * @brief Redo last action
     */
    void redo();

    /**
     * @brief Cut current selection
     */
    void cut();

    /**
     * @brief Copy current selection
     */
    void copy();

    /**
     * @brief Paste text from buffer
     */
    void paste();

    /**
     * @brief Select all text in editor
     */
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
    void updateShift(QPointF point);
    void updateUi(bool resetCaret);
    void type(const QString &text);

    void insertText(int pos, const QString &text);
    void removeText(int pos, int count);

    int findPos(qreal x, qreal y) const;
    QPointF findShift(int pos) const;
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
