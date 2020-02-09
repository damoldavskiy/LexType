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

QString Styler::statusStyle()
{
    return "QWidget { background: rgb(50, 50, 50); color: rgb(150, 150, 150); }";
}

QColor Styler::editorBack()
{
    return { 50, 50, 50 };
}

QColor Styler::editorRegular()
{
    return { 240, 240, 240 };
}

QColor Styler::editorMathematics()
{
    return { 180, 180, 240 };
}

QColor Styler::editorCommand()
{
    return { 220, 120, 120 };
}

QColor Styler::editorSpecial()
{
    return { 230, 160, 90 };
}

QColor Styler::editorLine()
{
    return { 55, 55, 55 };
}

QColor Styler::editorSelection()
{
    return { 40, 60, 130, 90 };
}

QColor Styler::editorCursor()
{
    return { 240, 240, 240 };
}

QColor Styler::numbersBack()
{
    return { 50, 50, 50 };
}

QColor Styler::numbersFore()
{
    return { 100, 100, 100 };
}

QColor Styler::numbersCurrent()
{
    return { 150, 150, 150 };
}

QColor Styler::painterBack()
{
    return { 50, 50, 50 };
}

QColor Styler::painterFore()
{
    return { 240, 240, 240 };
}

QColor Styler::viewerBack()
{
    return { 50, 50, 50 };
}

QColor Styler::viewerFore()
{
    return { 240, 240, 240 };
}
