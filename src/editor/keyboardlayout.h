#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include <QString>

class KeyboardLayout
{
public:
    static QChar pass(QChar symbol);
    static bool isLatin(QChar symbol);
    static bool isCyrillic(QChar symbol);

private:
    static QString _latin;
    static QString _cyril;
};

#endif // KEYBOARDLAYOUT_H
