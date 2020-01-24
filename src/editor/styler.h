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
    static QString statusLabelStyle();

    static QColor editorBack();
    static QColor editorFore();
    static QColor editorLine();
    static QColor editorSelection();

    static QColor numbersBack();
    static QColor numbersFore();

    static QColor painterBack();
    static QColor painterFore();
};

#endif // STYLER_H
