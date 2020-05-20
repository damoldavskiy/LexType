#ifndef STYLER_H
#define STYLER_H

#include <QSettings>

/**
 * @brief Storages applications settings
 */
class Styler
{
public:
    /**
     * @brief Initiates settings
     * @return True if settings are set to default, false if loaded
     */
    static bool init();

    template <typename T>
    /**
     * @brief Get settings field
     * @param Key
     * @return Value
     */
    static T get(const QString &key)
    {
        return _settings.value(key).value<T>();
    }

    template <typename T>
    /**
     * @brief Set settings field
     * @param Key
     * @param Value
     */
    static void set(const QString &key, const T &value)
    {
        _settings.setValue(key, value);
    }

private:
    static QSettings _settings;
};

#endif // STYLER_H
