#ifndef PAINTERTOOLKIT_H
#define PAINTERTOOLKIT_H

#include <QToolBar>

/**
 * @brief Toolbar widget for painter dialog
 */
class PainterToolkit : public QToolBar
{
    Q_OBJECT

public:
    /**
     * @brief Constructs painter toolkit (tool bar with painter tools)
     * @param Parent widget
     */
    PainterToolkit(QWidget* parent = 0);

    /**
     * @brief Resets selection
     */
    void reset();

signals:
    /**
     * @brief Line selected
     */
    void line();

    /**
     * @brief Rectangle selected
     */
    void rectangle();

    /**
     * @brief Ellipse selected
     */
    void ellipse();

    /**
     * @brief Path selected
     */
    void path();

    /**
     * @brief Text selected
     */
    void text();

    /**
     * @brief Solid line selected
     */
    void lineSolid();

    /**
     * @brief Dash line selected
     */
    void lineDash();

    /**
     * @brief Soft tip selected
     */
    void tipSoft();

    /**
     * @brief Single-directional arrow selected
     */
    void tipArrow();

    /**
     * @brief Double-directional arrow selected
     */
    void tipDouble();

    /**
     * @brief Emtpy fill selected
     */
    void fillEmpty();

    /**
     * @brief Solid fill selected
     */
    void fillSolid();

private:
    void uncheckTools();
    void uncheckLine();
    void uncheckTip();
    void uncheckFill();

    void showLine(bool value);
    void showTip(bool value);
    void showFill(bool value);

    void createActions();
    void createMenus();

    QAction *_line;
    QAction *_rectangle;
    QAction *_ellipse;
    QAction *_path;
    QAction *_text;
    QAction *_lineSolid;
    QAction *_lineDash;
    QAction *_tipSoft;
    QAction *_tipArrow;
    QAction *_tipDouble;
    QAction *_fillEmpty;
    QAction *_fillSolid;
    QAction *_lineSeparator;
    QAction *_tipSeparator;
    QAction *_fillSeparator;
};

#endif // PAINTERTOOLKIT_H
