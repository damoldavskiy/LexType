#ifndef STYLER_H
#define STYLER_H

#include <QMap>
#include <QVariant>

class Styler
{
public:
    static void reset();

    template <typename T>
    static T get(const QString &key)
    {
        return _values[key].value<T>();
    }

    template <typename T>
    static void set(const QString &key, const T &value)
    {
        _values[key] = value;
    }

private:
    static QMap<QString, QVariant> _values;
};

#endif // STYLER_H
