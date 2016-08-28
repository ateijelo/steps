/*
 * networktask.cpp -- part of Steps, a simple maps app
 *
 * Copyright 2009-2016 Andy Teijelo <www.ateijelo.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    reply = NULL;
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

void NetworkTask::stop()
{
    fDebug(DEBUG_NETWORK) << "NetworkTask::stop()";
    if (reply != NULL)
    {
        reply->abort();
    }
}

void NetworkTask::work()
{
    fDebug(DEBUG_NETWORK) << "NetworkTask::work()";
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
        fDebug(DEBUG_NETWORK) << "using proxy";
        net->setProxy(proxy);
    }

    //GoogleMaps
    if (tile.type == "GoogleMap")
    {
        reply = net->get(QNetworkRequest(QUrl(QString("http://mt0.google.com/vt/lyrs=m@117&hl=en&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else if (tile.type == "GoogleSat")
    {
        QString galileo = QString("Galileo").left((tile.x * 3 + tile.y)%8);
        //https://khms0.google.com/kh/v=149&x=5&y=11&z=5
        reply = net->get(QNetworkRequest(QUrl(QString("http://khm%4.google.com/kh/v=180&x=%1&y=%2&z=%3")
                                      .arg(tile.x)
                                      .arg(tile.y)
                                      .arg(tile.zoom)
                                      //.arg(galileo)
                                      .arg(qrand() % 2))));
    }
    else if (tile.type == "OpenStreetMaps")
    {
        reply = net->get(QNetworkRequest(QUrl(QString("http://localhost:8008/ex/%1/%2/%3.png")
                                     .arg(tile.zoom).arg(tile.x).arg(tile.y))));
    }
    else if (tile.type == "UHGoogleMap")
    {
        net->setProxy(noProxy);
        reply = net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=map&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else if (tile.type == "UHGoogleSat")
    {
        net->setProxy(noProxy);
        reply = net->get(QNetworkRequest(QUrl(QString("http://lara.matcom.uh.cu/tiles/?type=sat&x=%1&y=%2&z=%3")
                                     .arg(tile.x).arg(tile.y).arg(tile.zoom))));
    }
    else
    {
        emit finished(this);
    }

    fDebug(DEBUG_NETWORK) << "network request for" << tile.type << tile.x << tile.y << tile.zoom;
}

void NetworkTask::replyFinished(QNetworkReply *r)
{
    fDebug(DEBUG_NETWORK) << "network reply for" << tile.type << tile.x << tile.y << tile.zoom;
    if (r->error() == 0)
    {
        emit tileData(tile.type,tile.x,tile.y,tile.zoom,r->readAll());
    }
    else
    {
        fDebug(DEBUG_NETWORK) << "    error code:" << r->error() << "(" << r->errorString() << ")";
    }
    r->deleteLater();
    this->reply = NULL;
    emit finished(this);
}
