#ifndef STYLER_H
#define STYLER_H

#include <QSettings>

class Styler
{
public:
    static void init();
    static void reset();

    template <typename T>
    static T get(const QString &key);

    template <typename T>
    static void set(const QString &key, const T &value);

private:
    static QSettings _settings;
    static bool _soft;
};

template <typename T>
T Styler::get(const QString &key)
{
    return _settings.value(key).value<T>();
}

template <typename T>
void Styler::set(const QString &key, const T &value)
{
    if (_soft && _settings.contains(key))
        return;
    _settings.setValue(key, value);
}

#endif // STYLER_H
