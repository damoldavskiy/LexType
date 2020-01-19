#include "styler.h"

QString Styler::widgetStyle()
{
    return "QWidget { background: rgb(50, 50, 50); }";
}

QString Styler::menuStyle()
{
    return "QWidget { background: rgb(50, 50, 50); color: rgb(240, 240, 240); }"
           "QMenuBar::item { background: rgb(50, 50, 50); }"
           "QMenuBar::item:selected { background: rgb(60, 60, 60); }"
           "QMenuBar::item:pressed { background: rgb(70, 70, 70); }"
           "QMenu::item:disabled { color: rgb(150, 150, 150); }"
           "QMenu::item:selected { background: rgb(60, 60, 60); }"
           "QMenu::item:pressed { background: rgb(70, 70, 70); }"
           "QMenu::separator { background: rgb(70, 70, 70); height: 1px; }";
}

QString Styler::scrollStyle()
{
    return "QScrollArea { border: none; }"
           "QAbstractScrollArea::corner { background: rgb(50, 50, 50); }"
           "QScrollBar { background: rgb(50, 50, 50); margin: 0; }"
           "QScrollBar::add-line, QScrollBar::sub-line { background: rgb(50, 50, 50); width: 0px; height: 0px; }"
           "QScrollBar::handle { background: rgb(60, 60, 60); min-height: 30px; min-width: 30px; }";
}