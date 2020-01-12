#include "styler.h"

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
