//#define QT_NO_DEBUG_OUTPUT
#include <QtDebug>
#include <QSettings>
#include <QNetworkReply>
#include <QNetworkProxy>

#include "networktask.h"

NetworkTask::NetworkTask(const TileId& tile, QObject *parent) :
    Task(parent), tile(tile)
{
    net = new QNetworkAccessManager(this);
    connect(net,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
}

NetworkTask::~NetworkTask()
{
    delete net;
}

void NetworkTask::work()
{
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

    //GoogleMaps
    if (tile.type == "GoogleMap")
    {
        net->get(QNetworkRequest(QUrl(QString("http://mt0.google.com/vt/lyrs=m@117&hl=en&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    if (tile.type == "GoogleSat")
    {
        QString galileo = QString("Galileo").left((tile.x * 3 + tile.y)%8);
        net->get(QNetworkRequest(QUrl(QString("http://khm%5.google.com/kh/v=109&x=%1&y=%2&z=%3&s=%4")
                                      .arg(tile.x)
                                      .arg(tile.y)
                                      .arg(tile.zoom)
                                      .arg(galileo)
                                      .arg(tile.x % 2))));
    }
    if (tile.type == "OpenStreetMaps")
    {
        net->get(QNetworkRequest(QUrl(QString("http://tile.openstreetmap.org/%1/%2/%3.png")
                                     .arg(tile.zoom).arg(tile.x).arg(tile.y))));
    }
    if (tile.type == "UHGoogleMap")
    {
        net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=map&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    if (tile.type == "UHGoogleSat")
    {
        net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=sat&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }

    qDebug() << "network request for" << tile.type << tile.x << tile.y << tile.zoom;
}

void NetworkTask::replyFinished(QNetworkReply *reply)
{
    qDebug() << "network reply for" << tile.type << tile.x << tile.y << tile.zoom;
    if (reply->error() == 0)
    {
        emit tileData(tile.type,tile.x,tile.y,tile.zoom,reply->readAll());
    }
    else
    {
        qDebug() << "    error code:" << reply->error() << "(" << reply->errorString() << ")";
    }
    reply->deleteLater();
    emit finished(this);
}
