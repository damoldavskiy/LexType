#ifndef STYLER_H
#define STYLER_H

#include <QString>
#include <QColor>

class Styler
{
public:
    static QString widgetStyle();
    static QString menuStyle();
    static QString scrollStyle();
    static QString statusStyle();

    static QColor editorBack();
    static QColor editorRegular();
    static QColor editorMathematics();
    static QColor editorCommand();
    static QColor editorSpecial();
    static QColor editorLine();
    static QColor editorSelection();
    static QColor editorCursor();

    static QColor numbersBack();
    static QColor numbersFore();
    static QColor numbersCurrent();

    static QColor painterBack();
    static QColor painterFore();

    static QColor viewerBack();
    static QColor viewerFore();
};

#endif // STYLER_H
