#include "styler.h"

#include <QColor>
#include <QFont>

bool Styler::init()
{
    if (_settings.allKeys().size() > 0)
        return false;

    set("widget-style", "QWidget { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }");
    set("menu-style", "QWidget { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }"
                      "QMenuBar::item { background: rgb(50, 50, 50); }"
                      "QMenuBar::item:selected { background: rgb(60, 60, 60); }"
                      "QMenuBar::item:pressed { background: rgb(70, 70, 70); }"
                      "QMenu::item:disabled { color: rgb(150, 150, 150); }"
                      "QMenu::item:selected { background: rgb(60, 60, 60); }"
                      "QMenu::item:pressed { background: rgb(70, 70, 70); }"
                      "QMenu::separator { background: rgb(70, 70, 70); height: 1px; }");
    set("toolbar-style", "QToolBar::separator { height: 10px; width: 10px; border: none; }");
    set("scroll-style", "QScrollArea { border: none; }"
                        "QAbstractScrollArea::corner { background: rgb(50, 50, 50); }"
                        "QScrollBar { background: rgb(50, 50, 50); margin: 0; }"
                        "QScrollBar::add-line, QScrollBar::sub-line { background: rgb(50, 50, 50); width: 0px; height: 0px; }"
                        "QScrollBar::handle { background: rgb(60, 60, 60); min-height: 30px; min-width: 30px; }");
    set("status-style", "QWidget { background: rgb(50, 50, 50); color: rgb(150, 150, 150); }");

    set("editor-back", QColor { 50, 50, 50 });
    set("editor-regular", QColor { 240, 240, 240 });
    set("editor-mathematics", QColor { 180, 180, 240 });
    set("editor-command", QColor { 220, 120, 120 });
    set("editor-special", QColor { 230, 160, 90 });
    set("editor-comment", QColor { 220, 70, 70 });
    set("editor-line", QColor { 55, 55, 55 });
    set("editor-selection", QColor { 40, 60, 130, 90 });
    set("editor-caret", QColor { 240, 240, 240 });

    set("numbers-back", QColor { 50, 50, 50 });
    set("numbers-fore", QColor { 100, 100, 100 });
    set("numbers-current", QColor { 150, 150, 150 });

    set("painter-back", QColor { 50, 50, 50 });
    set("painter-fore", QColor { 240, 240, 240 });

    set("viewer-back", QColor { 50, 50, 50 });
    set("viewer-fore", QColor { 240, 240, 240 });

    set("editor-font", QFont("Ubuntu", 11));

    set("editor-flag-line", true);
    set("editor-flag-numbers", true);
    set("editor-flag-keyboard", true);
    set("editor-flag-snippets-regular", true);
    set("editor-flag-snippets-math", true);
    set("editor-flag-autocompile", false);

    set("painter-flag-snippets", true);

    set("window-flag-askexit", true);

    return true;
}

QSettings Styler::_settings("DMSoft", "LexType");
