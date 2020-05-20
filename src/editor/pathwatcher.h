#ifndef PATHWATCHER_H
#define PATHWATCHER_H

#include <QFileInfo>

/**
 * @brief Class for storaging and updating current opened file path
 */
class PathWatcher
{
public:
    /**
     * @brief Constructs path watcher
     * @param Name of application
     */
    PathWatcher(const QString &app);

    /**
     * @brief Calls open file dialog
     * @param Filter
     * @return True if new file selected
     */
    bool open(const QString &filter);

    /**
     * @brief Calls save file dialog
     * @param Filter
     * @return True if saving accepted
     */
    bool save(const QString &filter);

    /**
     * @brief Checks whether file exists or not
     * @return
     */
    bool exists() const;

    /**
     * @brief Path to selected file
     * @return Path
     */
    QString path() const;

    /**
     * @brief Selected file directory
     * @return Directory
     */
    QString dir() const;

    /**
     * @brief Returns base name of selected file
     * @return Base name
     */
    QString baseName() const;

    /**
     * @brief Returns the title for application
     * @return Title
     */
    QString title() const;

    /**
     * @brief Returns edition flag
     * @return True if file is edited
     */
    bool edited() const;

    /**
     * @brief Sets edition flag
     * @param True if file is edited
     */
    void setEdited(bool value);

private:
    bool update(const QString &path);

    QFileInfo _info;
    QString _app;
    bool _edited = false;
};

#endif // PATHWATCHER_H
