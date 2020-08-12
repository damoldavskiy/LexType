#include "keyboardlayout.h"

QChar KeyboardLayout::pass(QChar symbol)
{
    if (_cyril.indexOf(symbol) > -1)
        symbol = _latin[_cyril.indexOf(symbol)];
    else if (_latin.indexOf(symbol) > -1)
        symbol = _cyril[_latin.indexOf(symbol)];

    return symbol;
}

bool KeyboardLayout::isLatin(QChar symbol)
{
    return _latin.indexOf(symbol) > -1;
}

bool KeyboardLayout::isCyrillic(QChar symbol)
{
    return _cyril.indexOf(symbol) > -1;
}

bool KeyboardLayout::isCyrillicSpecific(QChar symbol)
{
    return _cyrilOnly.indexOf(symbol) > -1;
}

QString KeyboardLayout::_latin = "`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./ ~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?";
QString KeyboardLayout::_cyril = "ё1234567890-=йцукенгшщзхъ\\фывапролджэячсмитьбю. Ё!\"№;%:?*()_+ЙЦУКЕНГШЩЗХЪ/ФЫВАПРОЛДЖЭЯЧСМИТЬБЮ,";
QString KeyboardLayout::_cyrilOnly = "ёйцукенгшщзхъфывапролджэячсмитьбюЁЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
