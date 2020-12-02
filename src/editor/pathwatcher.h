#ifndef PATHWATCHER_H
#define PATHWATCHER_H

#include <QFileInfo>

class PathWatcher
{
public:
    PathWatcher(const QString &app);
    bool open(const QString &filter);
    bool openExisting(const QString &path);
    void close();
    bool save(const QString &filter);
    bool exists() const;
    QString path() const;
    QString dir() const;
    QString baseName() const;
    QString title() const;
    bool edited() const;
    void setEdited(bool value);

private:
    bool update(const QString &path);

    QFileInfo _info;
    QString _app;
    bool _edited = false;
};

#endif // PATHWATCHER_H
