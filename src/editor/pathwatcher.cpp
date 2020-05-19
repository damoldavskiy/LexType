#include "pathwatcher.h"

#include <QFileDialog>

PathWatcher::PathWatcher(const QString &app)
    : _app(app)
{ }

bool PathWatcher::open(const QString &filter)
{
    return update(QFileDialog::getOpenFileName(nullptr, "Open", "", filter));
}

bool PathWatcher::save(const QString &filter)
{
    return update(QFileDialog::getSaveFileName(nullptr, "Save", "", filter));
}

bool PathWatcher::exists() const
{
    return _info.exists();
}

QString PathWatcher::path() const
{
    return _info.filePath();
}

QString PathWatcher::dir() const
{
    return _info.dir().path();
}

QString PathWatcher::baseName() const
{
    return _info.baseName();
}

QString PathWatcher::title() const
{
    return _info.fileName() + (_edited ? "*" : "") + " - " + _app;
}

bool PathWatcher::edited() const
{
    return _edited;
}

void PathWatcher::setEdited(bool value)
{
    _edited = value;
}

bool PathWatcher::update(const QString &path)
{
    if (path == "")
        return false;

    _info.setFile(path);
    _edited = false;
    return true;
}
