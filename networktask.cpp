#include <QtDebug>
#include <QSettings>
#include <QNetworkReply>
#include <QNetworkProxy>

#include "debug.h"
#include "networktask.h"

NetworkTask::NetworkTask(const TileId& tile, QObject *parent) :
    Task(parent), tile(tile)
{
    net = new QNetworkAccessManager(this);
    connect(net,SIGNAL(finished(QNetworkReply*)),
            this,SLOT(replyFinished(QNetworkReply*)));
}

TileId NetworkTask::tileId()
{
    return tile;
}

NetworkTask::~NetworkTask()
{
    delete net;
}

void NetworkTask::work()
{
    debugBlock(DEBUG_NETWORK)
    {
        qDebug() << "debugflags:" << debugflags;
        qDebug() << "NetworkTask::work()";
    }
    QSettings settings;
    QNetworkProxy proxy;
    QNetworkProxy noProxy;
    noProxy.setType(QNetworkProxy::NoProxy);
    //proxy.setType(QNetworkProxy::Socks5Proxy);
    //proxy.setHostName("localhost");
    //proxy.setPort(1080);
    proxy.setType(QNetworkProxy::HttpCachingProxy);
    proxy.setHostName(settings.value("ProxyHost","").toString());
    proxy.setPort(settings.value("ProxyPort",0).toInt());
    proxy.setUser(settings.value("ProxyUser","").toString());
    proxy.setPassword(settings.value("ProxyPass","").toString());
    if (settings.value("UseProxy",false).toBool())
    {
        debugBlock(DEBUG_NETWORK)
        qDebug() << "using proxy";
        net->setProxy(proxy);
    }

    //GoogleMaps
    if (tile.type == "GoogleMap")
    {
        net->get(QNetworkRequest(QUrl(QString("http://mt0.google.com/vt/lyrs=m@117&hl=en&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else if (tile.type == "GoogleSat")
    {
        QString galileo = QString("Galileo").left((tile.x * 3 + tile.y)%8);
        net->get(QNetworkRequest(QUrl(QString("http://khm%5.google.com/kh/v=109&x=%1&y=%2&z=%3&s=%4")
                                      .arg(tile.x)
                                      .arg(tile.y)
                                      .arg(tile.zoom)
                                      .arg(galileo)
                                      .arg(tile.x % 2))));
    }
    else if (tile.type == "OpenStreetMaps")
    {
        net->get(QNetworkRequest(QUrl(QString("http://tile.openstreetmap.org/%1/%2/%3.png")
                                     .arg(tile.zoom).arg(tile.x).arg(tile.y))));
    }
    else if (tile.type == "UHGoogleMap")
    {
        net->setProxy(noProxy);
        net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=map&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else if (tile.type == "UHGoogleSat")
    {
        net->setProxy(noProxy);
        net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=sat&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else
    {
        emit finished(this);
    }

    debugBlock(DEBUG_NETWORK)
    qDebug() << "network request for" << tile.type << tile.x << tile.y << tile.zoom;
}

void NetworkTask::replyFinished(QNetworkReply *reply)
{
    debugBlock(DEBUG_NETWORK)
    {
        qDebug() << "network reply for" << tile.type << tile.x << tile.y << tile.zoom;
    }
    if (reply->error() == 0)
    {
        emit tileData(tile.type,tile.x,tile.y,tile.zoom,reply->readAll());
    }
    else
    {
        debugBlock(DEBUG_NETWORK)
        {
            qDebug() << "    error code:" << reply->error() << "(" << reply->errorString() << ")";
        }
    }
    reply->deleteLater();
    emit finished(this);
}
