#include "program.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <iostream>
#include <JlCompress.h>

Program::Program(QObject *parent)
    : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
    connect(_manager, &QNetworkAccessManager::finished, this, &Program::reply);
}

void Program::run()
{
    #ifdef _WIN32
    std::cout << "Windows is not supported" << std::endl;
    emit finished();
    return;
    #endif

    std::cout << "Getting list of available updates..." << std::endl;
    QNetworkReply *reply = _manager->get(QNetworkRequest(QUrl(_url)));
    connect(reply, &QNetworkReply::downloadProgress, this, &Program::progress);
}

void Program::reply(QNetworkReply *reply)
{
    std::cout << std::endl;

    if (reply->request().url().toString().endsWith("latest")) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

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

            QNetworkRequest request;
            request.setUrl(QUrl(url));
            request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

            std::cout << "Downloading latest release..." << std::endl;
            QNetworkReply *reply = _manager->get(request);
            connect(reply, &QNetworkReply::downloadProgress, this, &Program::progress);

            return;
        }

        std::cout << "No releases found" << std::endl;
        emit finished();
    } else {
        QString zip = QDir::current().absoluteFilePath(_zipName);
        QString unzip = QDir::current().absoluteFilePath(_unzipName);

        QFile file(zip);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();

        JlCompress::extractDir(zip, unzip);

        QString program = "rm *; mv " + unzip + "* .; rm -r " + unzip;

        QProcess::startDetached("sh", QStringList() << "-c" << program);

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
