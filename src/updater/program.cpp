#include "program.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <iostream>
#include <JlCompress.h>

// TODO Already defined in editor/mainwindow.cpp
QString readText(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    return in.readAll();
}

Program::Program(QObject *parent)
    : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
    connect(_manager, &QNetworkAccessManager::finished, this, &Program::reply);
}

void Program::run()
{
    std::cout << "Getting list of available updates..." << std::endl;
    _manager->get(QNetworkRequest(QUrl(_url)));
}

void Program::reply(QNetworkReply *reply)
{
    if (reply->request().url().toString().endsWith("latest")) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

        if (!QFile("version").exists())
            std::cout << "Version file not found.";
        else {
            QString current = readText("version").trimmed();
            QString latest = doc.object().value("tag_name").toString();
            if (current == latest) {
                std::cout << "Latest version already downloaded" << std::endl;
                emit finished();
                return;
            } else {
                std::cout << "Current version is " << current.toStdString() << ", latest is " << latest.toStdString() << ".";
            }
        }

        QJsonValue assets = doc.object().value("assets");
        for (const QJsonValue &assetValue : assets.toArray()) {
            QJsonObject asset = assetValue.toObject();
            QString url = asset.value("browser_download_url").toString();

            #ifdef __linux__
            if (!url.endsWith("linux.zip"))
            #elif _WIN32
            if (!url.endsWith("windows.zip"))
            #endif
                continue;

            std::cout << " Downloading latest release..." << std::endl;

            QNetworkRequest request;
            request.setUrl(QUrl(url));
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

            QNetworkReply *reply = _manager->get(request);
            connect(reply, &QNetworkReply::downloadProgress, this, &Program::progress);

            return;
        }

        std::cout << "No suitable releases found" << std::endl;
        emit finished();
    } else {
        std::cout << std::endl;

        QString zip = QDir::current().absoluteFilePath(_zipName);
        QString unzip = QDir::current().absoluteFilePath(_unzipName) + "/";

        QFile file(zip);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        JlCompress::extractDir(zip, unzip);
        file.remove();

        #ifdef __linux__
        QString program = "mv " + _unzipName + "/* .; rm -r " + _unzipName;
        QProcess::startDetached("sh", QStringList() << "-c" << program);
        #elif _WIN32
        QString program = "ping 127.0.0.1 -n 1 >nul & xcopy " + _unzipName + " . /e /y >nul & rd " + _unzipName + " /s /q";
        QProcess::startDetached("cmd", QStringList() << "/c" << program);
        #endif

        emit finished();
    }
}

void Program::progress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal == -1)
        return;

    qreal part = (qreal)bytesReceived / bytesTotal;
    int total = 50;
    int ticks = (int)(part * total);

    std::cout << "\r[" << std::string(ticks, '|') << std::string(total - ticks, ' ') << "] " << (int)(part * 100) << " %";
    std::cout.flush();
}
