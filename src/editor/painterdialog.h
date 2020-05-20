#ifndef PAINTERDIALOG_H
#define PAINTERDIALOG_H

#include <QDialog>

#include "paintertoolkit.h"
#include "painter.h"
#include "editor.h"
#include "snippetmanager.h"

/**
 * @brief Painter dialog. Used for getting TikZ picture from user
 */
class PainterDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Consturct painter dialog
     * @param Parent widget
     */
    PainterDialog(QWidget *parent = 0);

    /**
     * @brief Returns current LaTeX painting
     * @return LaTeX string
     */
    QString latex() const;

public slots:
    /**
     * @brief Accepts insertion of figure to editor and closes dialog
     */
    void accept();

    /**
     * @brief Switches to line
     */
    void line();

    /**
     * @brief Switches to ellipse
     */
    void ellipse();

    /**
     * @brief Switches to rectangle
     */
    void rectangle();

    /**
     * @brief Switches to path
     */
    void path();

    /**
     * @brief Switches to text
     */
    void text();

    /**
     * @brief Enables solid line
     */
    void lineSolid();

    /**
     * @brief Enables dash line
     */
    void lineDash();

    /**
     * @brief Enables soft tip
     */
    void tipSoft();

    /**
     * @brief Enables single-directional arrow
     */
    void tipArrow();

    /**
     * @brief Enables double-directional arrow
     */
    void tipDouble();

    /**
     * @brief Enables solid fill
     */
    void fillSolid();

    /**
     * @brief Enables empty fill
     */
    void fillEmpty();

private:
    PainterToolkit *_toolkit;
    Painter *_painter;
    Editor *_editor;
    SnippetManager _snippets;
    bool _accepted = false;
};

#endif // PAINTERDIALOG_H
