//#define QT_NO_DEBUG_OUTPUT

#include <QtDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QApplication>
#include <QSettings>
#include <QtEndian>
#include <QFile>
#include <QUrl>
#include <QDir>

#include <unistd.h>


#include "constants.h"
#include "fetchtask.h"

FetchTask::FetchTask(const QString &tile_type, int x, int y, int zoom,
                     QObject *parent) :
    QObject(parent)
{
    this->tile_type = tile_type;
    this->tile_x = x;
    this->tile_y = y;
    this->tile_zoom = zoom;

    startEvent = QEvent::Type(QEvent::registerEventType());
    net = new QNetworkAccessManager(this);
    connect(net,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
    QSettings settings;
    QNetworkProxy proxy;
    //proxy.setType(QNetworkProxy::Socks5Proxy);
    //proxy.setHostName("localhost");
    //proxy.setPort(1080);
    proxy.setType(QNetworkProxy::HttpCachingProxy);
    proxy.setHostName(settings.value("ProxyHost","").toString());
    proxy.setPort(settings.value("ProxyPort",0).toInt());
    proxy.setUser(settings.value("ProxyUser","").toString());
    proxy.setPassword(settings.value("ProxyPass","").toString());
    net->setProxy(proxy);
}

FetchTask::~FetchTask()
{
    //qDebug() << this << "destroyed.";
}

void FetchTask::customEvent(QEvent *event)
{
    if (event->type() == startEvent)
    {
        state = Start;
        work();
    }
}

void FetchTask::start()
{
    qApp->postEvent(this,new QEvent(startEvent));
}

void FetchTask::work()
{
    QString cachedPath = QString("cache/%1/%2/%3_%4").arg(tile_type).arg(tile_zoom).arg(tile_x).arg(tile_y);
    QFile f(cachedPath);
    if (f.open(QIODevice::ReadOnly))
    {
        qDebug() << "Tile cached:" << tile_type << tile_x << tile_y << tile_zoom;
        emit tileData(tile_type,tile_x,tile_y,tile_zoom,f.readAll());
        emit finished(this);
        f.close();
        return;
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
    QSettings settings;
    QNetworkProxy proxy;
    //proxy.setType(QNetworkProxy::Socks5Proxy);
    //proxy.setHostName("localhost");
    //proxy.setPort(1080);
    proxy.setType(QNetworkProxy::HttpCachingProxy);
    proxy.setHostName(settings.value("ProxyHost","").toString());
    proxy.setPort(settings.value("ProxyPort",0).toInt());
    proxy.setUser(settings.value("ProxyUser","").toString());
    proxy.setPassword(settings.value("ProxyPass","").toString());
    manager->setProxy(proxy);

    //GoogleMaps
    net->get(QNetworkRequest(QUrl(QString("http://mt0.google.com/vt/lyrs=m@117&hl=en&x=%1&y=%2&z=%3")
                                      .arg(tile_x).arg(tile_y).arg(tile_zoom))));

    //GoogleMaps
    manager->get(QNetworkRequest(QUrl(QString("http://mt0.google.com/vt/lyrs=m@117&hl=en&x=%1&y=%2&z=%3")
                                     .arg(tile_x).arg(tile_y).arg(tile_zoom))));

    //GoogleSat
    //manager->get(QNetworkRequest(QUrl(QString("http://khm1.google.com/kh/v=83&x=%1&y=%2&z=%3&s=Galil")
    //                                 .arg(tile_x).arg(tile_y).arg(tile_zoom))));

    qDebug() << "request for" << tile_x << tile_y << tile_zoom;
}

void FetchTask::replyFinished(QNetworkReply *reply)
{
    qDebug() << "reply for" << tile_x << tile_y << tile_zoom;
    if (reply->error() == 0)
    {
        QDir d;
        d.mkpath(QString("cache/%1/%2").arg(tile_type).arg(tile_zoom));
        QString cachedPath = QString("cache/%1/%2/%3_%4").arg(tile_type).arg(tile_zoom).arg(tile_x).arg(tile_y);
        QFile f(cachedPath);
        QByteArray data(reply->readAll());
        if (f.open(QIODevice::WriteOnly))
        {
            f.write(data);
            f.close();
        }
        emit tileData(tile_type,tile_x,tile_y,tile_zoom,data);
    }
    else
    {
        qDebug() << "    error code:" << reply->error();
    }
    reply->deleteLater();
    net->deleteLater();
    emit finished(this);
}

QString FetchTask::getTileFileName(QString tileStyle, int x, int y, int zoom)
{
    int mgm_x = x >> 3;
    int mgm_y = y >> 3;
    QSettings settings;
    return QString("%1/%2_%3/%4_%5.mgm")
            .arg(settings.value(SettingsKeys::CachePath,"").toString())
            .arg(tileStyle)
            .arg(zoom)
            .arg(mgm_x)
            .arg(mgm_y);
}
