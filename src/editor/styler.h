#ifndef STYLER_H
#define STYLER_H

#include <QSettings>

class Styler
{
public:
    static bool init();
    static void reset();

    template <typename T>
    static T get(const QString &key)
    {
        return _settings.value(key).value<T>();
    }

    template <typename T>
    static void set(const QString &key, const T &value)
    {
        _settings.setValue(key, value);
    }

private:
    static QSettings _settings;
};

#endif // STYLER_H
