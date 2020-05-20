#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QStack>

#include "figure.h"
#include "figurepath.h"

/**
 * @brief Main painter widget
 */
class Painter : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs painter
     * @param Parent widget
     */
    Painter(QWidget *parent = 0);

    ~Painter();

    /**
     * @brief Returns LaTeX representation of current painting
     * @return LaTeX string
     */
    QString latex() const;

    /**
     * @brief Returns current figure
     * @return Figure
     */
    Figure* figure() const;

    /**
     * @brief Sets current figure
     * @param Pointer to new current figure
     */
    void setFigure(Figure* value);

signals:
    /**
     * @brief Called when user releases current figure
     */
    void released();

public slots:
    /**
     * @brief Undo last figure insert
     */
    void undo();

    /**
     * @brief Redo last figure insert
     */
    void redo();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPoint _mouse;

    QStack<Figure*> _figures;
    QStack<Figure*> _afterFigures;
    Figure* _current = new FigurePath;
};

#endif // PAINTER_H
