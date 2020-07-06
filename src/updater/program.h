#ifndef PROGRAM_H
#define PROGRAM_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Program : public QObject
{
    Q_OBJECT

public:
    Program(QObject *parent = 0);

public slots:
    void run();
    void reply(QNetworkReply *reply);
    void progress(qint64 bytesReceived, qint64 bytesTotal);

signals:
    void finished();

private:
    QNetworkAccessManager *_manager;

    QString _url = "https://api.github.com/repos/damoldavskiy/lextype/releases/latest";
    QString _zipName = "latest.zip";
    QString _unzipName = "latest/";
};

#endif // PROGRAM_H
